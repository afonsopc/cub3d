#!/usr/bin/env python3
# Sons e músicas originais do cub3d (versão sem Wolfenstein): tudo
# sintetizado aqui (ondas quadradas, triangulares, serra e ruído), nenhuma
# gravação de terceiros. Precisa de numpy e do `lame` para os MP3.
#
#   python3 scripts/oms_art/sounds.py      # escreve em assets/oms3d/sounds
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


def t_(dur):
    return np.arange(int(dur * SR)) / SR


def osc(kind, freq, dur, duty=0.5):
    t = t_(dur)
    f = np.broadcast_to(np.asarray(freq, dtype=float), t.shape) if np.ndim(freq) else np.full(t.shape, float(freq))
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


def noise(dur, seed=0):
    return np.random.default_rng(seed).uniform(-1, 1, int(dur * SR))


def lowpass(x, a):
    y = np.empty_like(x)
    acc = 0.0
    for i, v in enumerate(x):
        acc += a * (v - acc)
        y[i] = acc
    return y


def env(n, attack=0.005, decay=None, dur=None):
    t = np.arange(n) / SR
    e = np.minimum(1, t / max(attack, 1e-4))
    if decay:
        e *= np.exp(-t / decay)
    e[-min(n, 80):] *= np.linspace(1, 0, min(n, 80))
    return e


def sweep(f0, f1, dur, curve=1.0):
    t = np.linspace(0, 1, int(dur * SR)) ** curve
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


def zap(f0, f1, dur, kind="sq", duty=0.3, nz=0.0, seed=0):
    x = osc(kind, sweep(f0, f1, dur, 0.6), dur, duty)
    if nz:
        x = x + nz * noise(dur, seed)
    return x * env(len(x), 0.002, dur / 3)


def thud(dur=0.18, f=70, seed=1):
    x = osc("sin", sweep(f * 2, f, dur), dur) + 0.5 * lowpass(noise(dur, seed), 0.08)
    return x * env(len(x), 0.001, dur / 4)


def sfx():
    write("pistol", mix(zap(1400, 260, 0.22, "sq", 0.25), 0.4 * thud(0.12, 90)))
    rifle = seq(*[zap(1200 - i * 60, 300, 0.07, "sq", 0.3, 0.15, i) for i in range(3)], gap=0.015)
    write("rifle", rifle)
    write("cannon", mix(zap(500, 70, 0.35, "saw", nz=0.4), thud(0.3, 50), 0.4 * zap(2200, 900, 0.08, "sq", 0.5)))
    wh = lowpass(noise(0.22, 3), 0.25) * np.sin(np.linspace(0, np.pi, int(0.22 * SR)))
    write("blade", mix(wh, 0.3 * osc("sin", sweep(900, 1400, 0.22), 0.22) * env(int(0.22 * SR), 0.01, 0.1)))
    write("sentinel-fire", mix(zap(900, 180, 0.25, "saw", nz=0.2, seed=5), 0.3 * thud(0.1, 120)))
    notes = {"pickup1": [72, 76, 79], "pickup2": [74, 79, 83], "pickup3": [76, 79, 84, 88]}
    for name, ns in notes.items():
        write(name, seq(*[osc("sq", 440 * 2 ** ((n - 69) / 12), 0.06, 0.25) * env(int(0.06 * SR), 0.002, 0.05) for n in ns]))
    write("get-ammo", seq(thud(0.05, 300, 2), osc("sq", 1600, 0.04, 0.5) * env(int(0.04 * SR), 0.001, 0.02),
                          osc("sq", 2000, 0.05, 0.5) * env(int(0.05 * SR), 0.001, 0.03), gap=0.02))
    for name, base in (("get-rifle", 60), ("get-cannon", 55)):
        ns = [base, base + 4, base + 7, base + 12, base + 16]
        write(name, seq(*[osc("tri", 440 * 2 ** ((n - 57) / 12), 0.07) * env(int(0.07 * SR), 0.002, 0.08) for n in ns]))
    write("heal1", osc("tri", sweep(400, 900, 0.3), 0.3) * env(int(0.3 * SR), 0.01, 0.2))
    write("heal2", seq(*[osc("tri", f, 0.09) * env(int(0.09 * SR), 0.005, 0.07) for f in (523, 659, 784, 1046)]))
    write("bump", thud(0.12, 60, 4), vol=0.6)
    write("empty", seq(lowpass(noise(0.02, 6), 0.5) * env(int(0.02 * SR), 0.001, 0.005), np.zeros(int(0.05 * SR)),
                       lowpass(noise(0.02, 7), 0.5) * env(int(0.02 * SR), 0.001, 0.005)), vol=0.5)
    hiss = lowpass(noise(0.45, 8), 0.3) * np.linspace(1, 0.2, int(0.45 * SR))
    motor = osc("saw", sweep(80, 140, 0.45), 0.45) * 0.4
    write("door-open", mix(hiss * 0.6, motor * env(int(0.45 * SR), 0.05)), vol=0.6)
    write("door-close", mix((hiss * 0.5)[::-1], osc("saw", sweep(140, 80, 0.4), 0.4) * 0.4, np.concatenate([np.zeros(int(0.38 * SR)), thud(0.15, 55, 9)])), vol=0.6)
    grind = lowpass(noise(1.2, 10), 0.05) * (0.7 + 0.3 * osc("sin", 6, 1.2)) + 0.3 * osc("saw", 45, 1.2)
    write("secret-wall", grind * env(int(1.2 * SR), 0.1), vol=0.6)
    # alerta da Sentinela: dois bipes de sirene
    beep = lambda f: osc("sq", f, 0.13, 0.5) * env(int(0.13 * SR), 0.003, 0.2)
    write("alert", seq(beep(988), beep(740), beep(988), gap=0.02), vol=0.6)
    # Sentinela abatida: varrimento a descer + estalidos (8 variações)
    for i in range(8):
        r = random.Random(i)
        f0 = r.uniform(700, 1200)
        d = r.uniform(0.35, 0.6)
        x = osc(r.choice(["sq", "saw"]), sweep(f0, 50, d, 0.8), d, 0.4) * env(int(d * SR), 0.003, d / 2)
        crack = lowpass(noise(0.25, 20 + i), 0.4) * env(int(0.25 * SR), 0.001, 0.05)
        sparks = np.zeros(int((d + 0.2) * SR))
        for _ in range(5):
            p = r.randint(0, len(sparks) - 400)
            sparks[p : p + 300] += noise(300 / SR, i * 7 + p)[:300] * np.linspace(1, 0, 300) * 0.5
        write(f"robot-down{i + 1}", mix(x, crack, sparks), vol=0.7)
    # Cão-Robô: latido eletrónico e desligar
    yap = lambda f: osc("sq", sweep(f, f * 0.6, 0.09), 0.09, 0.3) * env(int(0.09 * SR), 0.002, 0.05)
    write("hound-bark", seq(yap(1300), yap(1150), gap=0.06), vol=0.6)
    write("hound-down", mix(osc("sq", sweep(1000, 60, 0.6, 0.7), 0.6, 0.25) * env(int(0.6 * SR), 0.003, 0.3),
                            lowpass(noise(0.3, 30), 0.3) * env(int(0.3 * SR), 0.001, 0.08)), vol=0.7)


# ------------------------------------------------------------------ música


def midi(n):
    return 440 * 2 ** ((n - 69) / 12)


def drum(kind, seed):
    if kind == "k":
        d = 0.18
        return osc("sin", sweep(150, 45, d, 0.5), d) * env(int(d * SR), 0.001, 0.07)
    if kind == "s":
        d = 0.16
        return (0.8 * noise(d, seed) + 0.3 * osc("tri", 190, d)) * env(int(d * SR), 0.001, 0.05)
    d = 0.04
    return noise(d, seed) * env(int(d * SR), 0.001, 0.012) * 0.4


def song(name, bpm, key, prog, scale, seed, lead_kind="sq", feel="straight", bars_per_pass=8, passes=3, kbps=64):
    """prog: graus (0..6) da escala, um por compasso; a melodia é escrita
    por frases de 2 compassos com motivos que se repetem e variam."""
    rng = random.Random(seed)
    step = 60 / bpm / 4
    nbars = bars_per_pass * passes
    total = int(nbars * 16 * step * SR) + SR
    out = np.zeros(total)

    def put(x, at):
        i = int(at * SR)
        n = min(len(x), total - i)
        out[i : i + n] += x[:n]

    def deg(d, octave=0):
        return key + scale[d % len(scale)] + 12 * (d // len(scale) + octave)

    # motivos: ritmos (16 passos) e contornos
    motifs = []
    for _ in range(4):
        rhythm = [i for i in range(16) if rng.random() < (0.55 if i % 2 == 0 else 0.22)]
        if 0 not in rhythm:
            rhythm = [0] + rhythm
        contour = [rng.choice([-2, -1, 0, 1, 2, 3]) for _ in rhythm]
        motifs.append((rhythm, contour))
    for bar in range(nbars):
        root = prog[bar % len(prog)]
        p = bar // bars_per_pass
        t0 = bar * 16 * step
        # baixo
        for s in range(0, 16, 2 if feel == "straight" else 4):
            n = deg(root, -2) + (12 if (s // 2) % 4 == 3 else 0)
            dur = step * (1.8 if feel == "straight" else 3.6)
            put(0.32 * osc("tri", midi(n), dur) * env(int(dur * SR), 0.003, 0.25), t0 + s * step)
        # acordes em arpejo suave
        chord = [deg(root), deg(root + 2), deg(root + 4)]
        for s in range(0, 16, 1 if feel == "straight" else 2):
            if (s % 4) == 0 or feel != "straight":
                n = chord[(s // (1 if feel == "straight" else 2)) % 3]
                put(0.07 * osc("sq", midi(n), step * 0.9, 0.125) * env(int(step * 0.9 * SR), 0.002, 0.06), t0 + s * step)
        # bateria (a primeira volta sem caixa, para entrar devagar)
        for s in range(16):
            sw = step * 0.18 if feel == "swing" and s % 2 else 0
            if s in (0, 8) or (feel == "drive" and s in (0, 4, 8, 12)) or (s == 10 and bar % 2):
                put(0.55 * drum("k", s), t0 + s * step)
            if p > 0 and s in (4, 12):
                put(0.35 * drum("s", bar * 16 + s), t0 + s * step)
            if s % 2 == 0 or feel == "drive":
                put(0.25 * drum("h", bar * 16 + s), t0 + s * step + sw)
        # melodia (calada na primeira metade da primeira volta)
        if p == 0 and bar % bars_per_pass < bars_per_pass // 2:
            continue
        rhythm, contour = motifs[(bar // 2 + (1 if bar % 2 else 0)) % len(motifs)]
        d = root + 2 + (7 if p == 2 else 0)
        for i, s in enumerate(rhythm):
            d += contour[i]
            d = max(root - 1, min(root + 9, d))
            nxt = rhythm[i + 1] if i + 1 < len(rhythm) else 16
            dur = (nxt - s) * step * 0.92
            n = deg(d, 1 if p != 1 else 0)
            v = 0.16 if lead_kind != "tri" else 0.24
            vib = midi(n) * (1 + 0.004 * np.sin(2 * np.pi * 5.5 * t_(dur)))
            put(v * osc(lead_kind, vib, dur, 0.25) * env(int(dur * SR), 0.004, 0.35), t0 + s * step)
    out = np.tanh(out * 1.4)
    write(name, out, kbps=kbps, vol=0.7)


MINOR = [0, 2, 3, 5, 7, 8, 10]
DORIAN = [0, 2, 3, 5, 7, 9, 10]
MAJOR = [0, 2, 4, 5, 7, 9, 11]


def music():
    song("music-1", 128, 57, [0, 5, 3, 4, 0, 5, 6, 4], MINOR, 11, "sq", "drive")
    song("music-2", 100, 50, [0, 3, 0, 4, 5, 3, 1, 4], DORIAN, 23, "tri", "swing")
    song("music-3", 116, 52, [0, 6, 5, 6, 0, 3, 4, 4], MINOR, 37, "saw", "straight")
    song("music-mega", 140, 60, [0, 4, 5, 3, 0, 4, 3, 4], MAJOR, 51, "sq", "drive")


if __name__ == "__main__":
    what = sys.argv[1:] or ["sfx", "music"]
    if "sfx" in what:
        sfx()
    if "music" in what:
        music()
