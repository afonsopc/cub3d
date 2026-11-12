// Cenários headless da IA dos inimigos. Precisa de um build com -DAI_TRACE
// e os mapas de teste
// (gerados por tests/ai/make_maps.py) embutidos em tests/:
//
//   make -f emcc.Makefile NAME=/tmp/aitest/cub3d OBJ_DIR=/tmp/aiobj \
//     EXTRA_CFLAGS=-DAI_TRACE EXTRA_LDLIBS="--preload-file tests/ai/maps@tests"
//   PLAYWRIGHT=/caminho/para/playwright-core node tests/ai/run.mjs /tmp/aitest
//
// Lê as linhas "AI ..." da consola do jogo e verifica cada comportamento.
import http from "node:http";
import fs from "node:fs";
import path from "node:path";

const dir = path.resolve(process.argv[2] || ".");
const pw = await import(path.join(process.env.PLAYWRIGHT || "playwright-core", "index.mjs"));
const browserType = pw.chromium;

// página de arranque que passa o mapa como argumento
const shell = fs.readFileSync(path.join(dir, "cub3d.html"), "utf8").replace(
  "var Module={",
  'var Module={arguments:[new URLSearchParams(location.search).get("map")],',
);
fs.writeFileSync(path.join(dir, "aitest.html"), shell);
const types = { ".html": "text/html", ".js": "text/javascript", ".wasm": "application/wasm", ".data": "application/octet-stream" };
const server = http.createServer((req, res) => {
  const f = path.join(dir, decodeURIComponent(new URL(req.url, "http://x").pathname));
  if (!f.startsWith(dir) || !fs.existsSync(f)) return res.writeHead(404).end();
  res.writeHead(200, { "Content-Type": types[path.extname(f)] || "application/octet-stream" });
  fs.createReadStream(f).pipe(res);
}).listen(0);
const port = server.address().port;
const browser = await browserType.launch({ headless: true, channel: process.env.CHANNEL || undefined });

async function play(map, steps, ms) {
  const page = await browser.newPage({ viewport: { width: 1024, height: 768 } });
  const log = [];
  page.on("console", (m) => { const t = m.text(); if (t.startsWith("AI ") || t.startsWith("STORY ")) log.push(t); });
  await page.goto(`http://localhost:${port}/aitest.html?map=tests/${map}.cub`);
  await page.waitForFunction('typeof gameStarted !== "undefined" && gameStarted', null, { timeout: 60000 });
  await page.waitForTimeout(2500);
  await page.click("canvas").catch(() => {});
  for (const [key, hold] of steps) {
    if (key === "wait") { await page.waitForTimeout(hold); continue; }
    await page.keyboard.down(key); await page.waitForTimeout(hold); await page.keyboard.up(key);
    await page.waitForTimeout(100);
  }
  await page.waitForTimeout(ms);
  await page.close();
  if (process.env.AI_DEBUG) console.log(`--- ${map}\n` + log.join("\n"));
  return log;
}

// "AI 3 e spawn at 8.50 3.50" -> {id: 3, x: 8, y: 3}
function spawns(log) {
  return log.map((l) => l.match(/^AI (\d+) (.) spawn at ([\d.]+) ([\d.]+)/)).filter(Boolean)
    .map((m) => ({ id: +m[1], c: m[2], x: Math.floor(+m[3]), y: Math.floor(+m[4]) }));
}
const has = (log, re) => log.some((l) => re.test(l));
const results = [];
function check(name, ok, detail) {
  results.push({ name, ok, detail });
  console.log(`${ok ? "PASS" : "FAIL"}  ${name}${detail ? "  — " + detail : ""}`);
}

// 1. tiro com a porta fechada: B ouve, C não
{
  const log = await play("hear_closed", [["Space", 80]], 1500);
  const s = spawns(log);
  const b = s.find((e) => e.x === 8);
  const c = s.find((e) => e.x === 13);
  check("tiro alerta o guarda na sala ligada por abertura", b && has(log, new RegExp(`^AI ${b.id} . heard`)));
  check("tiro não passa a porta fechada", c && !has(log, new RegExp(`^AI ${c.id} . heard`)),
    c && log.filter((l) => l.startsWith(`AI ${c.id} `)).join(" | "));
}
// 2. com a porta aberta pelo jogador, C ouve
{
  const log = await play("hear_open", [["e", 80], ["wait", 1800], ["Space", 80]], 1500);
  const c = spawns(log).find((e) => e.x === 13);
  check("tiro passa a porta aberta", c && has(log, new RegExp(`^AI ${c.id} . heard`)));
}
// 3. o guarda abre a porta para chegar ao jogador
{
  const log = await play("door", [["Space", 80]], 7000);
  const e = spawns(log)[0];
  check("o guarda ouve pelo corredor", e && has(log, new RegExp(`^AI ${e.id} . heard`)));
  check("o guarda abre a porta no caminho", e && has(log, new RegExp(`^AI ${e.id} opens door`)));
  check("o guarda chega a ver o jogador", e && has(log, new RegExp(`^AI ${e.id} sees player`)));
}
// 4. grupo: ninguém fica preso
{
  const log = await play("crowd", [["Space", 80]], 9000);
  const s = spawns(log);
  const engaged = s.filter((e) => has(log, new RegExp(`^AI ${e.id} (sees player|.*->attack)`)));
  const stuck = s.map((e) => log.filter((l) => l.startsWith(`AI ${e.id} stuck`)).length);
  check("todos os inimigos chegam ao jogador", engaged.length === s.length, `${engaged.length}/${s.length}`);
  check("nenhum fica preso de forma repetida", stuck.every((n) => n <= 3), `re-routes: ${stuck.join(",")}`);
}

// 5. rádio: início, sítio (andar 1 casa), ver um atirador, vida baixa
{
  const log = await play("radio", [["w", 500], ["Space", 80]], 15000);
  check("rádio ao começar", has(log, /^STORY radio \d+: teste inicio/));
  check("rádio ao chegar a um sítio", has(log, /^STORY radio \d+: teste sitio/));
  check("rádio ao ver um atirador", has(log, /^STORY radio \d+: teste atirador/));
  check("rádio com pouca vida", has(log, /^STORY radio \d+: teste vida/));
}

// 6. puzzle com teclas: portão fechado, porta trancada, alavanca, chave, documentos
{
  const log = await play("puzzle", [
    ["e", 80], ["wait", 600],                 // portão: recusa
    ["ArrowLeft", 1000], ["wait", 900], ["e", 80], ["wait", 600], ["ArrowLeft", 1000], // porta trancada: recusa
    ["ArrowLeft", 500], ["w", 500], ["e", 80], ["wait", 2600], // alavanca a oeste
    ["s", 500], ["ArrowRight", 500], ["w", 1150], ["wait", 300], // pelo portão até à chave
    ["ArrowRight", 1000], ["w", 1500],        // volta até à porta trancada
    ["e", 80], ["wait", 1300], ["w", 700],    // abre-a com a chave: documentos
  ], 1500);
  check("portão fechado recusa sem a alavanca", has(log, /^STORY toast O port/));
  check("porta trancada recusa sem a chave", has(log, /^STORY toast Trancada/));
  check("alavanca abre o portão", has(log, /^STORY key >/));
  check("apanha a chave", has(log, /^STORY key 5/));
  check("porta trancada abre com a chave e dá acesso aos documentos", has(log, /^STORY key 6/),
    log.filter((l) => l.startsWith("STORY")).join(" | "));
}

await browser.close();
server.close();
const failed = results.filter((r) => !r.ok).length;
console.log(failed ? `${failed} falharam` : "todos passaram");
process.exit(failed ? 1 : 0);
