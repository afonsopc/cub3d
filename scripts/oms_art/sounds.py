#!/usr/bin/env python3
# Sons e músicas do cub3d (defesa do forte, estilo dos jogos de 1992): tudo
# sintetizado aqui (ondas quadradas/pulso, triangulares, FM de dois
# operadores, ruído e formantes de voz), nenhuma gravação nem amostra de
# terceiros.
#
#   - tiros: pistola tipo Walther, rajada da FBP, G3, tiro do inimigo, faca;
#   - apito de alerta, gritos de queda (voz por formantes), latido e ganido
#     do cão de ataque;
#   - portas pesadas, parede secreta de pedra, medalhas, cruz de guerra,
#     ouro, munições, armas, ração e estojo de primeiros socorros;
#   - a música está em scripts/oms_art/music (ver CREDITS.md).
#
# Precisa de numpy e do `lame` para os MP3.
#
#   python3 scripts/oms_art/sounds.py      # escreve em assets/oms3d/sounds
import math
import os
import random
import subprocess
import sys
import tempfile
import wave

import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.abspath(os.path.join(HERE, "..", "..", "assets", "oms3d", "sounds"))
SR = 22050


# ------------------------------------------------------------------ base


def t_(dur):
    return np.arange(int(dur * SR)) / SR


def osc(kind, freq, dur, duty=0.5):
    n = int(dur * SR)
    f = np.asarray(freq, dtype=float)
    f = f[:n] if f.ndim else np.full(n, float(f))
    if len(f) < n:
        f = np.concatenate([f, np.full(n - len(f), f[-1] if len(f) else 0.0)])
    ph = np.cumsum(f) / SR % 1.0
    if kind == "sq":
        return np.where(ph < duty, 1.0, -1.0)
    if kind == "tri":
        return 4 * np.abs(ph - 0.5) - 1
    if kind == "saw":
        return 2 * ph - 1
    if kind == "sin":
        return np.sin(2 * np.pi * ph)
    raise ValueError(kind)


def fm(freq, dur, ratio=1.0, index=2.0, idecay=0.15):
    """FM de dois operadores (à la AdLib): o índice cai com o tempo."""
    t = t_(dur)
    idx = index * np.exp(-t / idecay) + index * 0.25
    mod = np.sin(2 * np.pi * freq * ratio * t)
    return np.sin(2 * np.pi * freq * t + idx * mod)


def noise(dur, seed=0):
    return np.random.default_rng(seed).uniform(-1, 1, int(dur * SR))


def smooth(x, k):
    """Passa-baixo barato (média móvel de k amostras)."""
    k = max(1, int(k))
    if k == 1:
        return x
    return np.convolve(x, np.ones(k) / k, mode="same")


def hp(x):
    return np.concatenate([[0.0], np.diff(x)])


def env(n, attack=0.005, decay=None):
    t = np.arange(n) / SR
    e = np.minimum(1, t / max(attack, 1e-4))
    if decay:
        e = e * np.exp(-t / decay)
    k = min(n, 80)
    e[-k:] *= np.linspace(1, 0, k)
    return e


def E(x, attack=0.005, decay=None):
    return x * env(len(x), attack, decay)


def sweep(f0, f1, dur, curve=1.0):
    t = np.linspace(0, 1, max(1, int(dur * SR))) ** curve
    return f0 * (f1 / f0) ** t


def mix(*parts):
    n = max(len(p) for p in parts)
    out = np.zeros(n)
    for p in parts:
        out[: len(p)] += p
    return out


def seq(*parts, gap=0.0):
    out = []
    for p in parts:
        out.append(p)
        if gap:
            out.append(np.zeros(int(gap * SR)))
    return np.concatenate(out)


def at(x, sec):
    return np.concatenate([np.zeros(int(sec * SR)), x])


def crush(x, bits=7, hold=2):
    """Som de placa de som de 1992: menos bits e meia taxa de amostragem."""
    y = np.repeat(x[::hold], hold)[: len(x)]
    q = 2 ** (bits - 1)
    return np.round(y * q) / q


def reson(x, freq, bw):
    """Ressoador de 2 polos; freq pode variar no tempo (formantes)."""
    n = len(x)
    f = np.broadcast_to(np.asarray(freq, dtype=float), (n,))
    r = math.exp(-math.pi * bw / SR)
    c2 = -r * r
    c1 = 2 * r * np.cos(2 * np.pi * f / SR)
    g = 1 - r
    y = np.zeros(n)
    y1 = y2 = 0.0
    for i in range(n):
        v = g * x[i] + c1[i] * y1 + c2 * y2
        y[i] = v
        y2, y1 = y1, v
    return y


def write(name, x, kbps=64, vol=0.8):
    os.makedirs(OUT, exist_ok=True)
    x = np.asarray(x, dtype=float)
    peak = np.max(np.abs(x)) or 1
    x = x / peak * vol
    pcm = (x * 32767).astype(np.int16)
    with tempfile.NamedTemporaryFile(suffix=".wav", delete=False) as f:
        tmp = f.name
    with wave.open(tmp, "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(SR)
        w.writeframes(pcm.tobytes())
    subprocess.run(["lame", "--silent", "-m", "m", "-b", str(kbps), "--resample", "22.05", tmp,
                    os.path.join(OUT, name + ".mp3")], check=True)
    os.unlink(tmp)


# ------------------------------------------------------------------ efeitos


def thud(dur=0.18, f=70, seed=1):
    x = osc("sin", sweep(f * 2.2, f, dur, 0.5), dur) + 0.5 * smooth(noise(dur, seed), 18)
    return E(x, 0.001, dur / 4)


def shot(seed, body=0.16, crack=0.05, low=110, dark=4, crunch=True):
    """Tiro: estalo de ruído + corpo abafado + pancada grave."""
    c = E(noise(crack, seed), 0.0005, crack / 3)
    b = E(smooth(noise(body, seed + 1), dark), 0.001, body / 3.5)
    th = E(osc("sin", sweep(low * 2, low * 0.45, body, 0.4), body), 0.001, body / 3)
    x = mix(1.1 * c, 0.9 * b, 0.9 * th)
    x = np.tanh(x * 2.2)
    return crush(x, 7, 2) if crunch else x


def click(seed, dur=0.018, k=2):
    return E(hp(noise(dur, seed)) * 0.8 + smooth(noise(dur, seed + 9), k), 0.0005, dur / 4)


GLOTTAL_VOWELS = {
    "a": (730, 1090, 2440),
    "uh": (640, 1190, 2390),
    "o": (570, 840, 2410),
    "u": (330, 870, 2240),
    "e": (530, 1840, 2480),
    "i": (300, 2200, 2900),
}


def voice(f0, vowels, dur, breath=0.15, seed=0):
    """Voz de homem por formantes: fonte glótica + 3 ressoadores que
    deslizam entre as vogais dadas."""
    n = int(dur * SR)
    f0 = np.interp(np.linspace(0, 1, n), np.linspace(0, 1, len(f0)), f0)
    jit = 1 + 0.02 * smooth(np.random.default_rng(seed).normal(0, 1, n), 200) * 8
    src = osc("saw", f0 * jit, dur) * -1
    src = src + breath * noise(dur, seed)
    pos = np.linspace(0, len(vowels) - 1, n)
    out = np.zeros(n)
    for k, (bw, g) in enumerate(((90, 1.0), (110, 0.6), (160, 0.3))):
        fr = np.interp(pos, np.arange(len(vowels)), [GLOTTAL_VOWELS[v][k] for v in vowels])
        out += g * reson(src, fr, bw)
    return out


def creak(dur, seed, f=380):
    """Dobradiça: atrito em impulsos irregulares filtrados (stick-slip)."""
    rng = np.random.default_rng(seed)
    n = int(dur * SR)
    imp = np.zeros(n)
    i = 0
    rate = np.interp(np.arange(n), [0, n - 1], [f * 0.7, f * 1.25])
    while i < n:
        imp[i] = rng.uniform(0.6, 1.0)
        i += max(1, int(SR / rate[i] * rng.uniform(0.8, 1.3)))
    y = reson(imp, f * 2.1, 220) + 0.6 * reson(imp, f * 4.3, 300)
    return y * np.sin(np.linspace(0, np.pi, n)) ** 0.6


def whistle(dur, f, seed):
    """Apito de alerta: senoide aguda com o trilo da bolinha + sopro."""
    t = t_(dur)
    rng = random.Random(seed)
    trill = 1 + 0.035 * np.sign(np.sin(2 * np.pi * rng.uniform(26, 32) * t))
    w = osc("sin", f * trill, dur) + 0.25 * osc("sin", f * 2 * trill, dur)
    w = w * (0.75 + 0.25 * np.sin(2 * np.pi * 29 * t))
    b = 0.12 * hp(noise(dur, seed))
    return E(w + b, 0.012) * np.minimum(1, (dur - t) / 0.03)


def sfx():
    write("pistol", shot(1, body=0.17, crack=0.05, low=120, dark=3), vol=0.72)
    burst = mix(*[at(shot(10 + i, body=0.11, crack=0.04, low=140 - i * 6, dark=3), i * 0.085) for i in range(3)])
    write("rifle", burst)
    heavy = mix(shot(20, body=0.28, crack=0.06, low=70, dark=6),
                at(0.25 * click(21, 0.02), 0.11))  # alimentação da fita
    write("cannon", heavy)
    write("sentinel-fire", smooth(shot(30, body=0.22, crack=0.04, low=100, dark=7), 3), vol=0.75)
    n = int(0.2 * SR)
    swish = smooth(noise(0.2, 3), 3) * np.sin(np.linspace(0, np.pi, n)) ** 2
    ring = 0.15 * E(osc("sin", sweep(2400, 3200, 0.2), 0.2), 0.01, 0.06)
    write("blade", crush(mix(swish, ring), 7, 2), vol=0.7)

    # medalha: duas notas de caixinha de música
    bell = lambda f, d=0.22: E(fm(f, d, ratio=3.5, index=1.6, idecay=0.05), 0.001, d / 3)
    write("pickup1", seq(bell(1568), bell(2093, 0.3)), vol=0.6)
    # cruz de guerra: estojo a abrir + sinal
    rustle = np.concatenate([E(smooth(hp(noise(0.05, 40 + i)), 2), 0.003, 0.02) for i in range(4)])
    write("pickup2", mix(0.7 * rustle, at(seq(bell(1319, 0.12), bell(1760, 0.12), bell(2637, 0.3)), 0.12)), vol=0.65)
    # mala de escudos: moedas a cair + arpejo grande
    coins = mix(*[at(bell(2600 + 400 * (i % 3), 0.12) * 0.5, 0.03 * i + 0.01 * (i % 2)) for i in range(8)])
    arp = seq(*[bell(f, 0.09) for f in (1047, 1319, 1568, 2093)], bell(2637, 0.4))
    write("pickup3", mix(coins, at(arp, 0.1)), vol=0.65)

    write("get-ammo", seq(click(50, 0.02), np.zeros(int(0.05 * SR)), click(51, 0.03), thud(0.05, 260, 52) * 0.4), vol=0.6)

    def fanfare(notes, step=0.075):
        return seq(*[E(0.6 * osc("sq", 440 * 2 ** ((m - 69) / 12), step, 0.25)
                       + 0.4 * fm(440 * 2 ** ((m - 69) / 12), step, 1, 1.5, 0.05), 0.003, step * 1.5) for m in notes])
    cock = seq(click(60, 0.02), np.zeros(int(0.06 * SR)), click(61, 0.035))
    write("get-rifle", seq(cock, np.zeros(int(0.05 * SR)), fanfare([67, 72, 76, 79])), vol=0.65)
    write("get-cannon", seq(cock, thud(0.08, 90, 62) * 0.6, fanfare([60, 64, 67, 72, 76, 79], 0.07)), vol=0.65)
    warm = lambda f, d: E(0.6 * osc("tri", f, d) + 0.3 * fm(f, d, 2, 0.8, 0.1), 0.008, d / 2)
    write("heal1", seq(warm(523, 0.1), warm(659, 0.1), warm(784, 0.22)), vol=0.6)
    write("heal2", seq(warm(587, 0.08), warm(740, 0.08), warm(880, 0.08), warm(1175, 0.3)), vol=0.6)
    write("bump", thud(0.14, 55, 4), vol=0.55)
    write("empty", seq(click(70, 0.015), np.zeros(int(0.06 * SR)), click(71, 0.012)), vol=0.5)

    # portas pesadas (madeira forrada a ferro)
    latch = mix(click(80, 0.03), thud(0.08, 140, 81) * 0.5)
    slide = smooth(noise(0.55, 82), 30) * 0.5
    write("door-open", mix(0.6 * latch, at(creak(0.5, 83, 360), 0.05), at(1.6 * slide, 0.05)), vol=0.65)
    slam = mix(thud(0.3, 48, 84) * 1.2, E(smooth(noise(0.12, 85), 4), 0.001, 0.03))
    write("door-close", mix(0.9 * creak(0.35, 86, 420), at(1.4 * slide[: int(0.33 * SR)], 0.0), at(slam, 0.32),
                            at(click(87, 0.03) * 0.5, 0.42)), vol=0.65)
    # parede secreta: pedra a arrastar
    g = smooth(noise(1.3, 90), 40) * (0.7 + 0.3 * np.abs(np.sin(2 * np.pi * 3.3 * t_(1.3))))
    grit = smooth(noise(1.3, 91), 6) * 0.25 * (np.random.default_rng(92).random(int(1.3 * SR)) < 0.08)
    rumble = 0.35 * osc("tri", 38, 1.3)
    write("secret-wall", E(mix(g * 3, grit * 3, rumble), 0.1) * np.linspace(1, 0.4, int(1.3 * SR)), vol=0.6)

    # apito: curto-curto-longo, duas notas
    write("alert", seq(whistle(0.12, 2900, 1), whistle(0.12, 2900, 2), whistle(0.42, 2550, 3), gap=0.05), vol=0.55)

    # gritos de queda (8 variações, voz de homem adulto)
    shapes = [
        ([150, 175, 120, 85], ["a", "a", "o"], 0.55),
        ([130, 140, 95, 70], ["uh", "uh", "u"], 0.45),
        ([170, 200, 150, 90], ["a", "e", "uh"], 0.6),
        ([120, 125, 90], ["o", "u"], 0.4),
        ([160, 190, 175, 100, 70], ["a", "a", "a", "o"], 0.75),
        ([140, 110, 80], ["uh", "o"], 0.38),
        ([185, 160, 110], ["e", "a", "uh"], 0.5),
        ([110, 130, 100, 75], ["o", "a", "u"], 0.6),
    ]
    for i, (f0, vw, d) in enumerate(shapes):
        v = voice(f0, vw, d, breath=0.2, seed=100 + i)
        v = v * env(len(v), 0.015) * np.linspace(1, 0.15, len(v)) ** 0.7
        gasp = 0.12 * E(smooth(noise(0.1, 110 + i), 3), 0.01, 0.04)
        hitp = 0.5 * thud(0.1, 90, 120 + i)
        x = mix(hitp, at(v, 0.02), at(gasp, d + 0.02))
        write(f"robot-down{i + 1}", crush(np.tanh(x * 1.5), 7, 2), vol=0.75)

    # cão de guarda
    def bark(seed, f=520):
        d = 0.16
        f0 = sweep(f, f * 0.62, d, 0.7)
        src = osc("saw", f0, d) * 0.7 + 0.6 * noise(d, seed)
        y = reson(src, 900, 250) + 0.7 * reson(src, 1700, 300) + 0.3 * reson(src, 2800, 400)
        return E(np.tanh(y * 3), 0.004, 0.05)
    write("hound-bark", crush(seq(bark(130), np.zeros(int(0.1 * SR)), bark(131, 480)), 7, 2), vol=0.7)
    d = 0.4
    f0 = np.concatenate([sweep(800, 1300, 0.08), sweep(1300, 520, d - 0.08)])
    src = osc("saw", f0, d) * 0.7 + 0.25 * noise(d, 140)
    yelp = E(reson(src, 1100, 200) + 0.6 * reson(src, 2300, 300), 0.005, 0.14)
    whine = 0.25 * E(reson(osc("saw", sweep(700, 450, 0.35), 0.35), 1000, 200), 0.05, 0.12)
    write("hound-down", crush(mix(yelp, at(whine, 0.42), at(thud(0.15, 70, 141) * 0.4, 0.15)), 7, 2), vol=0.7)


def puzzle_sfx():
    """Fechadura trancada, chave, alavanca e portão de ferro."""
    rattle = mix(*[at(click(200 + i, 0.02, 2) * (1 - 0.2 * i), 0.07 * i) for i in range(3)])
    write("locked", mix(rattle, at(thud(0.09, 180, 205) * 0.6, 0.22)), vol=0.6)
    bell = lambda f, d=0.16: E(fm(f, d, ratio=3.1, index=1.2, idecay=0.04), 0.001, d / 3)
    jingle = mix(*[at(bell(2300 + 350 * (i % 3), 0.1) * 0.5, 0.045 * i) for i in range(5)])
    write("key", mix(jingle, at(seq(bell(1319, 0.1), bell(1976, 0.28)), 0.12)), vol=0.6)
    ratchet = np.concatenate([click(210 + i, 0.012, 2) * 0.6 for i in range(6)])
    write("lever", seq(ratchet, thud(0.16, 75, 216) * 1.2, at(click(217, 0.03), 0.0)), vol=0.7)
    d = 1.6
    grind = smooth(noise(d, 220), 12) * (0.6 + 0.4 * np.abs(np.sin(2 * np.pi * 5 * t_(d))))
    squeal = 0.12 * E(osc("saw", sweep(520, 610, d), d), 0.2)
    chain = mix(*[at(click(230 + i, 0.015, 2) * 0.4, 0.11 * i) for i in range(13)])
    clang = thud(0.3, 60, 240) * 1.3
    write("gate", mix(E(grind * 1.5, 0.08) * np.linspace(1, 0.5, int(d * SR)), squeal, chain, at(clang, d - 0.05)), vol=0.65)


# A música do jogo (seis marchas em 8 bits) está em scripts/oms_art/music
# (render_all.py) e é copiada para assets/oms3d/sounds/music.


if __name__ == "__main__":
    if sys.argv[1:] != ["puzzles"]:
        sfx()
    puzzle_sfx()
