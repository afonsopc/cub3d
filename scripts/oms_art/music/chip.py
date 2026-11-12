#!/usr/bin/env python3
# Motor de chiptune para as marchas do cub3d (Estado Novo): canal de pulso
# (melodia), segundo pulso (contracanto/terceiras), triangular (baixo),
# arpejo de pulso curto (acordes) e ruído (caixa, bombo, pratos), no mesmo
# formato dos assets/oms3d/sounds/music-*.mp3: mono, 22050 Hz, MP3 56 kbps.
#
# Cada canção vive no seu ficheiro (hino_mocidade.py, a_portuguesa.py, ...)
# com a melodia escrita à mão em texto editável e chama arrange() daqui.
# O ficheiro sai em loop perfeito: comprimento exato em compassos e a cauda
# das notas que passa do fim é dobrada para o início.
#
# Notação das frases (igual a sounds.py): "C5:4 D5:2 r:2" = nota:passos,
# passos de semicolcheia; 'r' = pausa. Acordes: "C", "Am", "G7", "F#m",
# "Bbdim", "Caug", "Cm7"; "C|G7" divide o compasso em duas metades.
import json
import os
import subprocess
import tempfile
import wave

import numpy as np

SR = 22050
HERE = os.path.dirname(os.path.abspath(__file__))

NOTE = {"C": 0, "D": 2, "E": 4, "F": 5, "G": 7, "A": 9, "B": 11}
QUAL = {"": (0, 4, 7), "m": (0, 3, 7), "7": (0, 4, 7, 10), "m7": (0, 3, 7, 10), "dim": (0, 3, 6),
        "aug": (0, 4, 8), "maj7": (0, 4, 7, 11), "sus4": (0, 5, 7)}


# ------------------------------------------------------------------ notação

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


def chord(name):
    """'F#m7' -> (raiz MIDI na oitava 3, intervalos)."""
    p = NOTE[name[0]]
    i = 1
    while i < len(name) and name[i] in "#b":
        p += 1 if name[i] == "#" else -1
        i += 1
    return 48 + p % 12, QUAL[name[i:]]


def hz(m):
    return 440 * 2 ** ((m - 69) / 12)


def check(sections, bar):
    for name, bars in sections.items():
        for i, b in enumerate(bars):
            tot = sum(d for _, d in phrase(b))
            if abs(tot - bar) > 1e-6:
                raise ValueError(f"secção {name} compasso {i + 1}: {tot} passos em vez de {bar}: {b}")


# ------------------------------------------------------------------ osciladores

def t_(dur):
    return np.arange(int(dur * SR)) / SR


def pulse(freq, dur, duty=0.25):
    n = int(dur * SR)
    f = np.broadcast_to(np.asarray(freq, dtype=float), (n,))
    ph = np.cumsum(f) / SR % 1.0
    return np.where(ph < duty, 1.0, -1.0)


def tri(freq, dur, steps=16):
    """Triangular quantizada em 16 degraus, como no canal do NES."""
    n = int(dur * SR)
    f = np.broadcast_to(np.asarray(freq, dtype=float), (n,))
    ph = np.cumsum(f) / SR % 1.0
    x = 4 * np.abs(ph - 0.5) - 1
    return np.round(x * steps / 2) / (steps / 2)


def lfsr_noise(dur, seed=1, short=False):
    """Ruído de registo de deslocamento (15 bits); 'short' dá o zumbido metálico."""
    n = int(dur * SR)
    rng = np.random.default_rng(seed)
    x = rng.integers(0, 2, n).astype(float) * 2 - 1
    if short:
        x = np.resize(x[:93], n)
    return x


def adsr(n, a=0.004, d=0.08, s=0.6, r=0.03):
    t = np.arange(n) / SR
    e = np.where(t < a, t / max(a, 1e-4), s + (1 - s) * np.exp(-(t - a) / max(d, 1e-4)))
    k = min(n, int(r * SR))
    if k:
        e[-k:] *= np.linspace(1, 0, k)
    return e


def steps_vol(e, levels=16):
    """Volume em degraus (4 bits), sabor de consola."""
    return np.round(e * levels) / levels


# ------------------------------------------------------------------ instrumentos

def lead(f, dur, duty=0.25, vib=0.0045):
    d = max(dur * 0.92, 0.04)
    t = t_(d)
    v = 1 + vib * np.sin(2 * np.pi * 5.5 * t) * np.clip((t - 0.18) / 0.15, 0, 1)
    x = pulse(f * v, d, duty)
    return x * steps_vol(adsr(len(x), 0.003, 0.12, 0.55, 0.02))


def lead2(f, dur):
    return lead(f, dur, 0.125, 0.0)


def bass(f, dur):
    d = max(min(dur * 0.85, 0.6), 0.05)
    x = tri(f, d)
    return x * adsr(len(x), 0.002, 0.2, 0.75, 0.015)


def stab(f, dur):
    d = max(dur, 0.04)
    x = pulse(f, d, 0.125)
    return x * steps_vol(adsr(len(x), 0.001, 0.05, 0.0, 0.01))


def snare(seed, g=1.0, dur=0.11):
    n = lfsr_noise(dur, seed)
    body = tri(sweep(240, 160, dur), dur)
    x = 0.85 * n + 0.35 * body
    return g * x * adsr(len(x), 0.0005, 0.04, 0.0, 0.01)


def kick(seed=0):
    d = 0.16
    x = tri(sweep(150, 45, d), d)
    return x * adsr(len(x), 0.0005, 0.07, 0.0, 0.01)


def hat(seed, g=1.0):
    x = lfsr_noise(0.035, seed, short=True)
    return 0.3 * g * x * adsr(len(x), 0.0003, 0.008, 0.0, 0.005)


def crash(seed, dur=0.6):
    x = lfsr_noise(dur, seed)
    x = np.concatenate([[0.0], np.diff(x)]) * 0.5
    return 0.4 * x * adsr(len(x), 0.0005, dur / 3, 0.0, 0.05)


def sweep(f0, f1, dur):
    t = np.linspace(0, 1, max(1, int(dur * SR)))
    return f0 * (f1 / f0) ** t


# ------------------------------------------------------------------ pista

class Track:
    def __init__(self, seconds):
        self.out = np.zeros(int(seconds * SR) + SR * 4)

    def put(self, x, sec, g=1.0):
        i = int(round(sec * SR))
        if i < 0 or i >= len(self.out):
            return
        n = min(len(x), len(self.out) - i)
        self.out[i:i + n] += g * x[:n]


def harmony_below(m, chord_pcs, lo=3):
    """Nota do acorde logo abaixo da melodia (pelo menos 'lo' meios-tons)."""
    for k in range(lo, 10):
        if (m - k) % 12 in chord_pcs:
            return m - k
    return m - 5


def bar_chords(ch):
    return [c for c in ch.split("|")]


# ------------------------------------------------------------------ arranjo

def arrange(song, out_name, passes=None, target=(60, 120), meter=None, tempo=None, transpose=0,
            lead_octave=0, intro_bars=1, swing=0.0):
    """Gera a marcha em loop. 'song' é um módulo/dicionário com BPM, BAR,
    SECTIONS, CHORDS e FORM. Devolve (amostras, info)."""
    g = song if isinstance(song, dict) else vars(song)
    bpm = tempo or g["BPM"]
    bar = g["BAR"]
    meter = meter or g.get("METER") or {8: "2/4", 12: "3/4", 16: "4/4", 6: "6/8"}.get(bar, "4/4")
    sections, chords, form = g["SECTIONS"], g["CHORDS"], g["FORM"]
    check(sections, bar)
    step = 60 / bpm / 4
    bar_s = bar * step
    form_bars = sum(len(sections[s]) for s in form)
    # quantas passagens cabem entre target[0] e target[1]
    if passes is None:
        passes = 1
        while (intro_bars + form_bars * (passes + 1)) * bar_s <= target[1] and \
                (intro_bars + form_bars * passes) * bar_s < target[0]:
            passes += 1
    plan = [(s, p) for p in range(passes) for s in form]
    total_bars = intro_bars + sum(len(sections[s]) for s, _ in plan)
    length = total_bars * bar_s
    tr = Track(length)
    beats = {"2/4": [0, 4], "3/4": [0, 4, 8], "4/4": [0, 4, 8, 12], "6/8": [0, 6], "12/8": [0, 6, 12, 18]}[meter]
    compound = meter in ("6/8", "12/8")

    # intro: rufo de caixa sobre o baixo na dominante do 1.º acorde
    first = bar_chords(chords[form[0]][0])[0]
    r0, _ = chord(first)
    t = 0.0
    for b in range(intro_bars):
        for s in range(bar):
            tt = t + s * step
            last = b == intro_bars - 1
            tr.put(snare(4000 + b * 64 + s, 0.22 + 0.18 * (s / bar) + (0.15 if s in beats else 0), 0.07), tt)
            if last and s >= bar - 4:
                tr.put(snare(4100 + s, 0.25, 0.05), tt + step / 2)
        for k in beats:
            tr.put(bass(hz(r0 - 12 + 7 + transpose), step * 3), t + k * step, 0.42)
        t += bar_s

    bar_i = 0
    for sname, p in plan:
        mel = sections[sname]
        chs = chords[sname]
        for i, (mtxt, ctxt) in enumerate(zip(mel, chs)):
            notes = phrase(mtxt)
            halves = bar_chords(ctxt)
            # --- melodia
            pos = 0.0
            for m, d in notes:
                if m is not None:
                    ts = t + pos * step
                    sw = swing * step if (pos % 2 == 1) else 0.0
                    ch_name = halves[min(int(pos // (bar / len(halves))), len(halves) - 1)]
                    cr, iv = chord(ch_name)
                    pcs = {(cr + x) % 12 for x in iv}
                    mm = m + transpose + 12 * lead_octave
                    if p % 2 == 0 and sname not in g.get("HARMONY", ()):
                        tr.put(lead(hz(mm), d * step), ts + sw, 0.30)
                    else:
                        tr.put(lead(hz(mm), d * step, 0.125), ts + sw, 0.27)
                        tr.put(lead(hz(harmony_below(mm, pcs)), d * step, 0.5, 0.0), ts + sw, 0.12)
                pos += d
            # --- baixo e acordes
            for h, cname in enumerate(halves):
                cr, iv = chord(cname)
                h0 = h * bar / len(halves)
                h1 = (h + 1) * bar / len(halves)
                for j, k in enumerate(beats):
                    if not (h0 <= k < h1):
                        continue
                    root = cr - 12 + transpose
                    fifth = root + iv[2]
                    note = root if j % 2 == 0 else fifth
                    blen = (6 if compound else 4) * step
                    tr.put(bass(hz(note), blen), t + k * step, 0.46)
                    # contratempo: acorde em arpejo curto (oom-PAH)
                    off = k + (3 if compound else 2)
                    if off < h1 or (off < bar and len(halves) == 1):
                        for q, x in enumerate(iv[:3]):
                            tr.put(stab(hz(cr + x + 12 + transpose), step * 0.9), t + off * step + q * 0.012, 0.07)
                    if compound:
                        for off2 in (k + 2, k + 4):
                            if off2 < bar:
                                for x in iv[:3]:
                                    tr.put(stab(hz(cr + x + 12 + transpose), step * 0.8), t + off2 * step, 0.05)
            # --- bateria de marcha
            phrase_end = (i % 4) == 3
            # compasso de anacruse (começa com pausa de meio compasso ou mais): rufo crescente
            lead_rest = notes[0][1] if notes and notes[0][0] is None else 0
            if lead_rest >= bar / 2:
                for s in range(int(lead_rest)):
                    tr.put(snare(5000 + bar_i * 64 + s, 0.18 + 0.2 * s / lead_rest, 0.06), t + s * step)
                    tr.put(snare(5100 + bar_i * 64 + s, 0.14 + 0.15 * s / lead_rest, 0.05), t + (s + 0.5) * step)
                tr.put(kick(), t, 0.62)
                t += bar_s
                bar_i += 1
                continue
            for s in range(bar):
                ts = t + s * step
                if s == 0 or (s in beats and not compound and beats.index(s) % 2 == 0):
                    tr.put(kick(), ts, 0.62)
                elif compound and s in beats:
                    tr.put(kick(), ts, 0.5)
                if phrase_end and s >= bar - (6 if bar >= 12 else 4):
                    tr.put(snare(bar_i * 131 + s, 0.22 + 0.04 * (s % 2), 0.06), ts)
                    tr.put(snare(bar_i * 131 + s + 50, 0.18, 0.05), ts + step / 2)
                elif not compound and s in beats and beats.index(s) % 2 == 1:
                    tr.put(snare(bar_i * 97 + s, 0.34), ts)
                elif compound and s in (3, 9, 15, 21):
                    tr.put(snare(bar_i * 97 + s, 0.26), ts)
                elif not compound and s % 8 == 7:
                    tr.put(snare(bar_i * 97 + s, 0.16, 0.05), ts)
                if s % 2 == 0:
                    tr.put(hat(bar_i * 53 + s, 0.9 if s in beats else 0.6), ts)
            if i == 0 and sname == form[0]:
                tr.put(crash(bar_i), t, 0.8)
            t += bar_s
            bar_i += 1

    n = int(round(length * SR))
    x = tr.out[:n].copy()
    tail = tr.out[n:]
    x[: len(tail)] += tail  # cauda dobrada para o início: loop sem costura
    info = {"bpm": bpm, "bars": total_bars, "seconds": round(n / SR, 3), "intro_seconds": round(intro_bars * bar_s, 3),
            "passes": passes, "meter": meter}
    return x, info


# ------------------------------------------------------------------ só a melodia (para a verificação)

def melody_only(song, transpose=0, tempo=None, sections_order=None):
    g = song if isinstance(song, dict) else vars(song)
    bpm = tempo or g["BPM"]
    step = 60 / bpm / 4
    order = sections_order or g["FORM"]
    bars = [b for s in order for b in g["SECTIONS"][s]]
    tot = sum(sum(d for _, d in phrase(b)) for b in bars) * step
    tr = Track(tot)
    t = 0.0
    for b in bars:
        for m, d in phrase(b):
            if m is not None:
                x = pulse(hz(m + transpose), d * step * 0.9, 0.5) * adsr(int(d * step * 0.9 * SR), 0.003, 0.2, 0.8, 0.02)
                tr.put(x, t, 0.5)
            t += d * step
    return tr.out[: int(tot * SR)]


# ------------------------------------------------------------------ saída

def finish(x, rms_db=-16.0):
    """Saturação leve, 8 bits e o mesmo volume médio (~-16 dB RMS) das music-*.mp3 do jogo."""
    x = np.tanh(np.asarray(x, dtype=float) * 1.3)
    x = x / (np.max(np.abs(x)) or 1)
    q = 2 ** 7
    x = np.round(x * q) / q  # 8 bits
    rms = np.sqrt(np.mean(x ** 2)) or 1
    x = x * (10 ** (rms_db / 20) / rms)
    peak = np.max(np.abs(x))
    if peak > 0.95:
        x = np.tanh(x / peak * 1.2) / np.tanh(1.2) * 0.95
    return x


def write_mp3(x, path, kbps=56):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    pcm = (np.clip(x, -1, 1) * 32767).astype(np.int16)
    with tempfile.NamedTemporaryFile(suffix=".wav", delete=False) as f:
        tmp = f.name
    with wave.open(tmp, "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(SR)
        w.writeframes(pcm.tobytes())
    subprocess.run(["lame", "--silent", "-m", "m", "-b", str(kbps), "--resample", "22.05", tmp, path], check=True)
    os.unlink(tmp)


def write_wav(x, path):
    pcm = (np.clip(x, -1, 1) * 32767).astype(np.int16)
    with wave.open(path, "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(SR)
        w.writeframes(pcm.tobytes())


def render(song, name, out_dir=HERE, preview_dir=None, **kw):
    x, info = arrange(song, name, **kw)
    y = finish(x)
    write_mp3(y, os.path.join(out_dir, name + ".mp3"))
    if preview_dir:
        n = int(30 * SR)
        p = y[:n].copy()
        k = int(1.5 * SR)
        p[-k:] *= np.linspace(1, 0, k)
        write_mp3(p, os.path.join(preview_dir, name + "-30s.mp3"))
    info["file"] = name + ".mp3"
    return info
