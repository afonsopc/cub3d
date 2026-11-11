#!/usr/bin/env python3
# Sons e músicas do cub3d, versão "Estado Novo" (estilo dos jogos de 1992):
# tudo sintetizado aqui (ondas quadradas/pulso, triangulares, FM de dois
# operadores, ruído e formantes de voz), nenhuma gravação nem amostra de
# terceiros. As melodias são escritas neste ficheiro e são originais.
#
#   - tiros: pistola tipo Walther, rajada da FBP, metralhadora pesada,
#     tiro do inimigo (agente da PIDE / GNR), faca;
#   - apito do guarda ("Alto!"), gritos de queda (voz por formantes),
#     latido e ganido do cão de guarda;
#   - portas pesadas, parede secreta de pedra, apanhar cravos, o "Avante!",
#     a mala de escudos, munições, armas, comida e estojo de primeiros
#     socorros;
#   - músicas: marcha de banda filarmónica (nível 1), fado (nível 2 da sala
#     42), marcha sombria da fortaleza (nível 2) e marcha da liberdade (mega).
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
    """Apito de polícia: senoide aguda com o trilo da bolinha + sopro."""
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

    # cravo: duas notas de caixinha de música
    bell = lambda f, d=0.22: E(fm(f, d, ratio=3.5, index=1.6, idecay=0.05), 0.001, d / 3)
    write("pickup1", seq(bell(1568), bell(2093, 0.3)), vol=0.6)
    # Avante!: folhear de jornal + sinal
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


# ------------------------------------------------------------------ música

NOTE = {"C": 0, "D": 2, "E": 4, "F": 5, "G": 7, "A": 9, "B": 11}


def nm(s):
    """'C#5' -> número MIDI."""
    p = NOTE[s[0]]
    i = 1
    while i < len(s) and s[i] in "#b":
        p += 1 if s[i] == "#" else -1
        i += 1
    return p + 12 * (int(s[i:]) + 1)


def phrase(text):
    """'A4:2 C5:1 r:1' -> [(midi|None, passos)]."""
    out = []
    for tok in text.split():
        n, d = tok.split(":")
        out.append((None if n == "r" else nm(n), float(d)))
    return out


def hz(m):
    return 440 * 2 ** ((m - 69) / 12)


CHORDS = {
    "Am": (57, 60, 64), "E": (52, 56, 59), "E7": (52, 56, 62), "Dm": (50, 53, 57), "C": (48, 52, 55),
    "G": (55, 59, 62), "F": (53, 57, 60), "Em": (52, 55, 59), "B7": (47, 51, 57), "Gm": (55, 58, 62),
    "Bb": (46, 50, 53), "A": (45, 49, 52), "G7": (55, 59, 65),
}


class Track:
    def __init__(self, seconds):
        self.out = np.zeros(int(seconds * SR) + SR)

    def put(self, x, sec, g=1.0):
        i = int(sec * SR)
        if i >= len(self.out):
            return
        n = min(len(x), len(self.out) - i)
        self.out[i : i + n] += g * x[:n]

    def notes(self, notes, t0, step, inst, g=1.0, transpose=0):
        t = t0
        for m, d in notes:
            if m is not None:
                self.put(inst(hz(m + transpose), d * step), t, g)
            t += d * step
        return t


# instrumentos

def brass(f, dur):
    d = dur * 0.95
    x = 0.55 * osc("sq", f * (1 + 0.003 * np.sin(2 * np.pi * 5 * t_(d))), d, 0.25) + 0.5 * fm(f, d, 1.0, 2.2, 0.08)
    return x * env(len(x), 0.02, None) * np.exp(-t_(d) / max(0.4, d * 1.5))


def tuba(f, dur):
    d = min(dur * 0.9, 0.5)
    return E(0.7 * osc("tri", f, d) + 0.3 * osc("sq", f, d, 0.5) * 0.5, 0.008, 0.25)


def pluck(f, dur, decay=0.18):
    d = max(dur, 0.12)
    x = 0.6 * osc("sq", f, d, 0.125) + 0.5 * fm(f, d, 2.0, 1.4, 0.03)
    return E(x, 0.001, decay)


def guitarra(f, dur):
    """Guitarra portuguesa: notas longas em trémulo (repicado)."""
    if dur <= 0.3:
        return pluck(f, dur, 0.16)
    k = 0.055
    out = np.zeros(int(dur * SR))
    tt = 0.0
    i = 0
    while tt < dur - 0.02:
        p = pluck(f, k * 1.6, 0.05) * (1.0 if i == 0 else 0.6)
        s = int(tt * SR)
        n = min(len(p), len(out) - s)
        out[s : s + n] += p[:n]
        tt += k
        i += 1
    return out


def viola(f, dur):
    return E(0.5 * osc("tri", f, max(dur, 0.1)) + 0.3 * osc("sq", f, max(dur, 0.1), 0.25), 0.002, 0.12)


def pad(f, dur):
    x = 0.5 * osc("saw", f, dur) + 0.5 * osc("saw", f * 1.005, dur)
    return smooth(x, 6) * env(int(dur * SR), 0.3) * np.minimum(1, (dur - t_(dur)) / 0.3 + 0.001)


def snare(seed, g=1.0, dur=0.12):
    return g * E(0.8 * noise(dur, seed) + 0.3 * osc("tri", 185, dur), 0.001, 0.045)


def bassdrum(seed):
    d = 0.22
    return mix(E(osc("sin", sweep(120, 42, d, 0.5), d), 0.001, 0.09), 0.1 * E(noise(0.02, seed), 0.001, 0.005))


def cymbal(seed, dur=0.5):
    return 0.35 * E(hp(hp(noise(dur, seed))), 0.001, dur / 3)


def timpani(f, seed):
    d = 0.6
    return E(fm(f, d, 1.5, 1.2, 0.05) + 0.3 * smooth(noise(d, seed), 20), 0.002, 0.2)


def hat(seed):
    return 0.25 * E(hp(noise(0.03, seed)), 0.0005, 0.008)


def finish(name, tr, kbps=56):
    x = np.tanh(tr.out * 1.3)
    write(name, crush(x, 8, 1), kbps=kbps, vol=0.62)


# ---------- music-1: marcha de banda filarmónica (2/4, lá menor, tensa)

M1_A = [
    "E4:2 A4:2 A4:1 B4:1 C5:2", "B4:2 A4:2 E4:4", "G#4:2 B4:2 B4:1 C5:1 D5:2", "C5:2 B4:2 G#4:4",
    "A4:2 C5:2 E5:3 D5:1", "F5:2 E5:1 D5:1 C5:2 A4:2", "B4:2 G#4:2 E4:2 B4:2", "A4:6 r:2",
]
M1_A_CH = ["Am", "Am", "E", "E", "Am", "Dm", "E", "Am"]
M1_B = [
    "G4:2 C5:2 E5:2 G5:2", "F5:2 D5:2 B4:4", "C5:2 E5:2 A5:3 G5:1", "G#5:2 E5:2 B4:4",
    "A5:2 F5:2 C5:2 F5:2", "E5:2 C5:2 G4:4", "B4:1 C5:1 D5:1 E5:1 G#4:2 B4:2", "A4:4 E4:2 A4:2",
]
M1_B_CH = ["C", "G", "Am", "E", "F", "C", "E", "Am"]


def march(tr, sections, t0, step, bar_steps, lead=brass, lead_g=0.32, harmony=None, drums="march", bass_g=0.4,
          roll_every=8):
    t = t0
    bar = 0
    for mel, chords in sections:
        for i, (m, ch) in enumerate(zip(mel, chords)):
            notes = phrase(m)
            tr.notes(notes, t, step, lead, lead_g)
            if harmony:
                tr.notes([(n - harmony if n else None, d) for n, d in notes], t, step, lead, lead_g * 0.45)
            root = CHORDS[ch][0] - 12
            fifth = root + 7 if ch not in ("E", "E7", "B7", "A") else root - 5
            # oom-pah: baixo nos tempos, acorde nos contratempos
            beats = bar_steps // 4
            for b in range(beats):
                tb = t + b * 4 * step
                tr.put(tuba(hz(root if b % 2 == 0 else fifth), 2 * step), tb, bass_g)
                for n in CHORDS[ch]:
                    tr.put(pluck(hz(n), step * 1.2, 0.06), tb + 2 * step, 0.07)
            # caixa de rufo
            if drums:
                last = (bar % roll_every) == roll_every - 1
                for s in range(bar_steps):
                    ts = t + s * step
                    if s % 4 == 0:
                        tr.put(bassdrum(bar * 31 + s), ts, 0.55)
                    if last and s >= bar_steps // 2:
                        for k in range(2):
                            tr.put(snare(bar * 97 + s * 2 + k, 0.25 + 0.05 * k, 0.06), ts + k * step / 2)
                    elif s % 4 == 2 or (s % 8 == 7 and drums == "march"):
                        tr.put(snare(bar * 97 + s), ts, 0.33)
                    if s % 2 == 0:
                        tr.put(hat(bar * 53 + s), ts, 0.6)
                if bar % 8 == 0:
                    tr.put(cymbal(bar), t, 1.0)
            t += bar_steps * step
            bar += 1
    return t


def music1():
    step = 60 / 112 / 4
    tr = Track(80)
    t = 0.0
    # entrada: caixa sozinha 2 compassos
    for s in range(16):
        tr.put(snare(900 + s, 0.25 if s % 2 else 0.35, 0.06), t + s * step)
    t += 16 * step
    secs = [(M1_A, M1_A_CH), (M1_A, M1_A_CH), (M1_B, M1_B_CH), (M1_A, M1_A_CH)]
    t = march(tr, secs, t, step, 8)
    t = march(tr, [(M1_B, M1_B_CH), (M1_A, M1_A_CH)], t, step, 8, harmony=3)
    t = march(tr, [(M1_A, M1_A_CH)], t, step, 8, harmony=12, lead_g=0.28)
    tr.out = tr.out[: int(t * SR) + 4000]
    finish("music-1", tr)


# ---------- music-2: fado (4/4 lento, mi menor harmónico)

F_A = [
    "B4:4 E5:2 F#5:2 G5:6 F#5:2", "E5:4 C5:2 B4:2 A4:8", "D#5:2 E5:2 F#5:4 A5:2 G5:2 F#5:4", "G5:2 F#5:2 E5:4 B4:8",
    "E5:4 G5:2 E5:2 C5:8", "A4:2 C5:2 E5:4 D5:2 C5:2 B4:4", "A4:2 B4:2 C5:2 D#5:2 F#5:4 D#5:4", "E5:12 r:4",
]
F_A_CH = ["Em", "Am", "B7", "Em", "C", "Am", "B7", "Em"]
F_B = [
    "C5:2 E5:2 A5:6 G5:2 F#5:2 E5:2", "G5:4 F#5:2 E5:2 B4:8", "F5:4 E5:2 D#5:2 E5:8", "D#5:2 F#5:2 A5:4 G5:2 F#5:2 D#5:4",
    "E5:2 G5:2 B5:4 A5:2 G5:2 F#5:4", "A5:2 G5:2 F#5:2 E5:2 C5:8", "B4:2 D#5:2 F#5:2 A5:2 G5:2 F#5:2 D#5:4", "E5:16",
]
F_B_CH = ["Am", "Em", "F", "B7", "Em", "Am", "B7", "Em"]


def music2():
    step = 60 / 72 / 4
    tr = Track(100)
    t = 0.0
    secs = [(None, F_A_CH[:2]), (F_A, F_A_CH), (F_B, F_B_CH), (F_A, F_A_CH)]
    for mel, chords in secs:
        for i, ch in enumerate(chords):
            notes = CHORDS[ch]
            root = notes[0] - 12
            # viola: baixo no 1 e no 3, acorde arpejado nos outros tempos
            for b in range(4):
                tb = t + b * 4 * step
                bass = root if b in (0, 2) else (root + 7 if b == 1 else root + 4 + (ch[-1] == "m") * -1)
                tr.put(tuba(hz(bass), 3 * step), tb, 0.3)
                for k, n in enumerate((notes[1], notes[2], notes[0] + 12)):
                    tr.put(viola(hz(n), step * 1.5), tb + (1 + k) * step, 0.09)
            # baixo a caminhar no fim de cada compasso para o seguinte
            if mel is not None:
                tr.notes(phrase(mel[i]), t, step, guitarra, 0.3)
            t += 16 * step
    tr.out = tr.out[: int(t * SR) + 6000]
    finish("music-2", tr)


# ---------- music-3: marcha sombria da fortaleza (ré menor, lenta)

D_A = [
    "D4:6 F4:2 A4:8", "G4:4 F4:4 E4:8", "F4:6 D4:2 Bb3:8", "C#4:4 E4:4 A4:8",
    "A4:6 Bb4:2 A4:4 F4:4", "G4:6 Bb4:2 D5:8", "C#5:4 Bb4:2 A4:2 G4:4 E4:4", "D4:16",
]
D_A_CH = ["Dm", "Dm", "Bb", "A", "Dm", "Gm", "A", "Dm"]
D_CTR = [
    "A3:8 D4:8", "Bb3:8 A3:8", "D4:8 F4:8", "E4:8 C#4:8",
    "F4:8 D4:8", "D4:8 G4:8", "E4:8 A3:8", "A3:16",
]


def dark_lead(f, dur):
    d = dur * 0.95
    x = 0.5 * osc("tri", f, d) + 0.35 * osc("sq", f, d, 0.125) + 0.3 * fm(f, d, 0.5, 1.5, 0.3)
    return x * env(len(x), 0.04) * np.exp(-t_(d) / max(0.6, d))


def music3():
    step = 60 / 84 / 4
    tr = Track(100)
    t = 0.0
    passes = [None, "lead", "lead+ctr", "high"]
    bars = 0
    for p in passes:
        for i, ch in enumerate(D_A_CH if p else D_A_CH[:4]):
            bar_len = 16 * step
            root = CHORDS[ch][0] - 24
            tr.put(pad(hz(root), bar_len), t, 0.16)
            tr.put(pad(hz(root + 7), bar_len), t, 0.08)
            for b in (0, 2):
                tr.put(timpani(hz(CHORDS[ch][0] - 24), bars * 7 + b), t + b * 4 * step, 0.5)
            for s in range(16):
                ts = t + s * step
                if s in (4, 12):
                    tr.put(snare(bars * 41 + s, 0.3), ts)
                if s in (13, 14, 15) and i % 2 == 1:
                    tr.put(snare(bars * 41 + s + 100, 0.14, 0.06), ts)
                if s % 4 == 0:
                    tr.put(hat(bars * 13 + s), ts, 0.4)
            if p in ("lead", "lead+ctr"):
                tr.notes(phrase(D_A[i]), t, step, dark_lead, 0.3)
            if p == "lead+ctr":
                tr.notes(phrase(D_CTR[i]), t, step, dark_lead, 0.16)
            if p == "high":
                tr.notes(phrase(D_A[i]), t, step, dark_lead, 0.24, transpose=12)
                tr.notes(phrase(D_CTR[i]), t, step, dark_lead, 0.14)
            t += bar_len
            bars += 1
    tr.out = tr.out[: int(t * SR) + 8000]
    finish("music-3", tr)


# ---------- music-mega: marcha da liberdade (dó maior, viva)

L_A = [
    "G4:2 C5:2 E5:2 G5:2", "E5:3 D5:1 C5:4", "D5:2 G5:2 B5:2 G5:2", "F5:3 E5:1 D5:4",
    "E5:2 G5:2 C6:3 B5:1", "A5:2 F5:2 C5:2 A5:2", "G5:2 F5:2 D5:2 B4:2", "C5:6 r:2",
]
L_A_CH = ["C", "C", "G", "G", "C", "F", "G", "C"]
L_B = [
    "A4:2 C5:2 F5:4", "E5:2 G5:2 C5:4", "D5:1 E5:1 F5:2 G5:2 B5:2", "C6:4 G5:4",
    "A5:2 G5:2 F5:2 A5:2", "G5:2 E5:2 C5:4", "D5:2 G5:2 F5:2 D5:2", "C5:4 G4:2 C5:2",
]
L_B_CH = ["F", "C", "G", "C", "F", "C", "G7", "C"]


def music_mega():
    step = 60 / 132 / 4
    tr = Track(80)
    t = 0.0
    for s in range(32):
        tr.put(snare(700 + s, 0.22 + 0.15 * (s % 4 == 0), 0.06), t + s * step)
        if s % 8 == 0:
            tr.put(bassdrum(s), t + s * step, 0.55)
    t += 32 * step
    t = march(tr, [(L_A, L_A_CH), (L_B, L_B_CH), (L_A, L_A_CH), (L_B, L_B_CH)], t, step, 8, lead_g=0.3)
    t = march(tr, [(L_A, L_A_CH), (L_B, L_B_CH)], t, step, 8, harmony=4, lead_g=0.3)
    t = march(tr, [(L_A, L_A_CH), (L_B, L_B_CH)], t, step, 8, harmony=12, lead_g=0.27)
    tr.put(cymbal(999, 1.2), t, 1.2)
    tr.put(brass(hz(72), 0.9), t, 0.35)
    tr.put(brass(hz(64), 0.9), t, 0.2)
    tr.put(tuba(hz(36), 0.9), t, 0.4)
    tr.out = tr.out[: int((t + 1.0) * SR)]
    finish("music-mega", tr)


def music():
    music1()
    music2()
    music3()
    music_mega()


if __name__ == "__main__":
    what = sys.argv[1:] or ["sfx", "music"]
    if "sfx" in what:
        sfx()
    if "music" in what:
        music()
