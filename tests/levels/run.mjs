// Joga a campanha do princípio ao fim, em Chrome headless, com o piloto
// automático de teste (F10, src/utils/loop/autopilot.c): o soldado anda com
// as teclas normais, abre portas, apanha a chave, puxa a alavanca, recolhe
// os documentos e sai pela alavanca da saída, a combater pelo caminho.
// Precisa de um build com -DAI_TRACE:
//
//   make -f emcc.Makefile NAME=/tmp/aitest/cub3d OBJ_DIR=/tmp/aiobj EXTRA_CFLAGS=-DAI_TRACE
//   PLAYWRIGHT=/caminho/para/playwright-core node tests/levels/run.mjs /tmp/aitest [pasta-das-capturas]
//
// Verifica, para cada nível: que a ordem dos objetivos é a esperada, que se
// chega às estatísticas e que a campanha acaba no final. (A porta trancada
// sem chave e o portão de alavanca, com teclas a sério, estão em
// tests/ai/run.mjs, cenário "puzzle".)
import http from "node:http";
import fs from "node:fs";
import path from "node:path";

const dir = path.resolve(process.argv[2] || ".");
const shots = process.argv[3] ? path.resolve(process.argv[3]) : null;
if (shots) fs.mkdirSync(shots, { recursive: true });
const pw = await import(path.join(process.env.PLAYWRIGHT || "playwright-core", "index.mjs"));
const shell = fs.readFileSync(path.join(dir, "cub3d.html"), "utf8").replace(
  "var Module={",
  'var Module={arguments:[new URLSearchParams(location.search).get("map")],',
);
fs.writeFileSync(path.join(dir, "leveltest.html"), shell);
const types = { ".html": "text/html", ".js": "text/javascript", ".wasm": "application/wasm", ".data": "application/octet-stream" };
const server = http.createServer((req, res) => {
  const f = path.join(dir, decodeURIComponent(new URL(req.url, "http://x").pathname));
  if (!f.startsWith(dir) || !fs.existsSync(f)) return res.writeHead(404).end();
  res.writeHead(200, { "Content-Type": types[path.extname(f)] || "application/octet-stream" });
  fs.createReadStream(f).pipe(res);
}).listen(0);
const port = server.address().port;
const browser = await pw.chromium.launch({ headless: true, channel: "chrome", args: ["--autoplay-policy=no-user-gesture-required"] });
const results = [];
function check(name, ok, detail) {
  results.push({ name, ok });
  console.log(`${ok ? "PASS" : "FAIL"}  ${name}${detail ? "  — " + detail : ""}`);
}

const viewport = process.env.MOBILE ? { width: 390, height: 844 } : { width: 1024, height: 768 };
const page = await browser.newPage({ viewport, hasTouch: !!process.env.MOBILE, isMobile: !!process.env.MOBILE });
const log = [];
let waiters = [];
page.on("console", (m) => {
  const t = m.text();
  if (!/^(STORY|AUTO|AI \d+ . (spawn|dies))/.test(t)) return;
  log.push(`${((Date.now() - t0) / 1000).toFixed(1)}s ${t}`);
  if (process.env.VERBOSE || /^STORY (key|objective|toast|play|next|phase|stats|radio)/.test(t)) console.log("   ", log.at(-1));
  for (const w of waiters) if (w.re.test(t)) w.resolve(t);
  waiters = waiters.filter((w) => !w.re.test(t));
});
const t0 = Date.now();
function waitLog(re, ms) {
  return new Promise((resolve) => {
    const w = { re, resolve };
    waiters.push(w);
    setTimeout(() => { waiters = waiters.filter((x) => x !== w); resolve(null); }, ms);
  });
}
let shotN = 0;
async function shot(name) {
  if (!shots) return;
  await page.screenshot({ path: path.join(shots, `${String(++shotN).padStart(2, "0")}-${name}.png`) });
}
async function key(k, hold = 80) {
  await page.keyboard.down(k); await page.waitForTimeout(hold); await page.keyboard.up(k);
}
// passa os ecrãs da história (texto a escrever-se: 1.º toque completa, 2.º avança)
async function throughStory(until = /^STORY play /) {
  const done = waitLog(until, 90000);
  let ok = null;
  done.then((v) => (ok = v ?? false));
  while (ok === null) { await key("Enter"); await page.waitForTimeout(700); }
  return ok;
}

// telemóvel: anda com o joystick esquerdo e abre a porta com o botão E,
// com toques a sério (CDP), e confirma que o soldado saiu do sítio
async function touchWalk() {
  const cdp = await page.context().newCDPSession(page);
  const box = await page.locator("#leftJoystick").boundingBox();
  const btn = await page.locator("#actionBtn").boundingBox();
  const cx = box.x + box.width / 2, cy = box.y + box.height / 2;
  const pos = async () => { const p = waitLog(/^AUTO pos/, 3000); await key("F11"); const t = await p; return t && t.split(" ").slice(2).map(Number); };
  const p0 = await pos();
  const stick = async (dy, ms) => {
    await cdp.send("Input.dispatchTouchEvent", { type: "touchStart", touchPoints: [{ x: cx, y: cy, id: 1 }] });
    for (let i = 1; i <= 5; i++) await cdp.send("Input.dispatchTouchEvent", { type: "touchMove", touchPoints: [{ x: cx, y: cy + dy * i / 5, id: 1 }] });
    await page.waitForTimeout(ms);
    await cdp.send("Input.dispatchTouchEvent", { type: "touchEnd", touchPoints: [] });
  };
  const tap = async () => {
    const x = btn.x + btn.width / 2, y = btn.y + btn.height / 2;
    await cdp.send("Input.dispatchTouchEvent", { type: "touchStart", touchPoints: [{ x, y, id: 2 }] });
    await page.waitForTimeout(120);
    await cdp.send("Input.dispatchTouchEvent", { type: "touchEnd", touchPoints: [] });
  };
  await stick(-50, 350);
  await tap();
  await page.waitForTimeout(1300);
  await shot("telemovel-porta-aberta");
  await stick(-50, 900);
  const p1 = await pos();
  await shot("telemovel-andou");
  check("telemóvel: o joystick anda e o botão E abre a porta", p0 && p1 && p0[1] - p1[1] > 1.8,
    `${p0} -> ${p1}`);
}

const start = process.env.MAP || "maps/missao1.cub";
await page.goto(`http://localhost:${port}/leveltest.html?map=${start}${process.env.MOBILE ? "&mobile=true" : ""}`);
await page.waitForFunction('typeof gameStarted !== "undefined" && gameStarted', null, { timeout: 90000 });
await page.waitForTimeout(2500);
await page.click("canvas").catch(() => {});

const LEVELS = [
  { name: "quartel-general", objectives: ["5", ">", "6"] },
  { name: "forte", objectives: [">", "5", "6"] },
];
const only = start.includes("missao2") ? LEVELS.slice(1) : LEVELS;
for (const lv of only) {
  const from = log.length;
  const play = await throughStory();
  check(`${lv.name}: começa a jogar`, !!play, play || "");
  await page.waitForTimeout(1500);
  await shot(`${lv.name}-inicio`);
  if (process.env.MOBILE && lv === only[0]) await touchWalk();
  await key("F10");
  let deaths = 0;
  const keysSeen = [];
  const end = Date.now() + 8 * 60 * 1000;
  while (Date.now() < end) {
    const ev = await waitLog(/^STORY (key .|phase 3|play )|^AUTO (no path|off)/, 20000);
    if (!ev) continue;
    if (/^STORY key/.test(ev)) {
      keysSeen.push(ev.slice(-1));
      await page.waitForTimeout(ev.endsWith(">") ? 60 : 400);
      await shot(`${lv.name}-tem-${{ 5: "chave", 6: "documento", ">": "alavanca" }[ev.slice(-1)] || ev.slice(-1)}`);
    } else if (/^STORY play/.test(ev)) {
      deaths++;
      console.log(`    morreu (${deaths}); recomeça o nível`);
      keysSeen.length = 0;
      await page.waitForTimeout(800);
      await key("F10");
    } else if (/^STORY phase 3/.test(ev)) {
      break;
    }
  }
  const lvlog = log.slice(from);
  check(`${lv.name}: objetivos pela ordem ${lv.objectives.join(" ")}`, keysSeen.join(" ") === lv.objectives.join(" "), keysSeen.join(" "));
  check(`${lv.name}: chega às estatísticas`, lvlog.some((l) => /STORY phase 3/.test(l)), `mortes: ${deaths}`);
  const stats = lvlog.find((l) => /STORY stats/.test(l));
  if (stats) console.log("   ", stats);
  await page.waitForTimeout(2500);
  await shot(`${lv.name}-estatisticas`);
}
// o final da campanha: estatísticas -> final -> volta ao hub
const fin = await throughStory(/^STORY (next map|phase 4)/);
check("a campanha mostra o final", !!fin && /phase 4|hub/.test(fin), fin || "");
if (/phase 4/.test(fin || "")) {
  await page.waitForTimeout(3000);
  await shot("final");
}
fs.writeFileSync(path.join(shots || dir, "playthrough.log"), log.join("\n") + "\n");
await browser.close();
server.close();
const failed = results.filter((r) => !r.ok).length;
console.log(failed ? `${failed} falharam` : "todos passaram");
process.exit(failed ? 1 : 0);
