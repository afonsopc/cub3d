#!/usr/bin/env python3
# Gera toda a arte do cub3d que substitui a do Wolfenstein 3D: paredes,
# portas, objetos, inimigos (Sentinela e Cão-Robô), armas, barra de estado,
# algarismos da barra e cartazes. Tudo desenhado aqui por código (voxels e
# píxeis), sem imagens de terceiros. Uso (precisa de numpy e pillow):
#
#   python3 scripts/oms_art/gen.py          # escreve em assets/oms3d, etc.
#
# Os BMP saem em 8 bits com paleta; o magenta (255,0,255) é o transparente.
import math
import os
import random
import sys

import numpy as np
from PIL import Image, ImageDraw

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
ROOT = os.path.abspath(os.path.join(HERE, "..", ".."))

from vox import render  # noqa: E402
import characters as ch  # noqa: E402
import decor as dc  # noqa: E402

OUT = os.path.join(ROOT, "assets", "oms3d", "textures")
MAG = np.array([255, 0, 255], dtype=np.uint8)


# ------------------------------------------------------------------ util


def save(arr, rel, root=OUT):
    """arr: HxWx3 (RGB) ou HxWx4 (RGBA; alfa 0 -> magenta)."""
    path = os.path.join(root, rel)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    a = np.asarray(arr)
    if a.shape[2] == 4:
        rgb = a[..., :3].copy()
        mask = a[..., 3] == 0
    else:
        rgb = a.copy()
        mask = np.all(rgb == MAG, axis=2)
    rgb[mask] = (0, 0, 0)
    im = Image.fromarray(rgb.astype(np.uint8), "RGB")
    q = im.quantize(colors=255, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)
    pal = q.getpalette()[: 255 * 3]
    pal += [0, 0, 0] * (255 - len(pal) // 3)
    pal += [255, 0, 255]
    idx = np.array(q, dtype=np.uint8)
    # nenhuma cor visível pode cair no magenta
    idx[mask] = 255
    out = Image.fromarray(idx, "P")
    out.putpalette(pal)
    out.save(path)


def rgba_to_canvas(rgba):
    a = rgba.copy()
    return a


def noise(h, w, seed, scale=1.0):
    r = np.random.default_rng(seed)
    return r.normal(0, scale, (h, w))


# ------------------------------------------------------------------ paredes (64x64)

S = 64


def stone(base, seed, rows=(10, 12, 9, 11, 10, 12), mortar=None):
    rng = random.Random(seed)
    base = np.array(base, dtype=float)
    img = np.zeros((S, S, 3))
    mortar = np.array(mortar if mortar is not None else base * 0.45)
    img[:] = mortar
    y = 0
    for i, h in enumerate(rows):
        x = -rng.randint(0, 20)
        while x < S:
            w = rng.randint(14, 26)
            tone = base * rng.uniform(0.82, 1.12)
            for yy in range(y + 1, min(S, y + h - 1)):
                for xx in range(max(0, x + 1), min(S, x + w - 1)):
                    c = tone.copy()
                    if yy == y + 1 or xx == x + 1:
                        c = c * 1.22
                    elif yy == y + h - 2 or xx == x + w - 2:
                        c = c * 0.72
                    img[yy, xx] = c
            x += w
        y += h
        if y >= S:
            break
    img += noise(S, S, seed, 6)[..., None]
    # salpicos
    r = np.random.default_rng(seed + 1)
    spots = r.random((S, S)) < 0.04
    img[spots] *= 0.8
    return np.clip(img, 0, 255)


def bricks(base, seed, bh=8, bw=16):
    rng = random.Random(seed)
    base = np.array(base, dtype=float)
    img = np.zeros((S, S, 3)) + base * 0.5
    for row in range(S // bh):
        off = (bw // 2) * (row % 2)
        for col in range(-1, S // bw + 1):
            x0 = col * bw + off
            tone = base * rng.uniform(0.85, 1.1)
            for yy in range(row * bh + 1, row * bh + bh - 1):
                for xx in range(max(0, x0 + 1), min(S, x0 + bw - 1)):
                    c = tone
                    if yy == row * bh + 1:
                        c = tone * 1.15
                    elif yy == row * bh + bh - 2:
                        c = tone * 0.78
                    img[yy, xx] = c
    img += noise(S, S, seed, 5)[..., None]
    return np.clip(img, 0, 255)


def wood(seed, base=(122, 80, 46)):
    base = np.array(base, dtype=float)
    img = np.zeros((S, S, 3))
    rng = np.random.default_rng(seed)
    for x in range(S):
        plank = x // 16
        ph = rng.uniform(0, 6)
        for y in range(S):
            g = math.sin(y * 0.18 + ph + math.sin((x % 16) * 0.5 + plank) * 1.4)
            t = 0.88 + 0.12 * g + (0.04 if plank % 2 else -0.02)
            img[y, x] = base * t
        if x % 16 == 0:
            img[:, x] = base * 0.45
        elif x % 16 == 15:
            img[:, x] = base * 0.7
    img[0:2] = base * 0.5
    img[S - 2 : S] = base * 0.5
    img += noise(S, S, seed, 3)[..., None]
    return np.clip(img, 0, 255)


def cave(seed):
    rng = random.Random(seed)
    img = np.zeros((S, S, 3)) + np.array([60, 40, 80])
    for _ in range(26):
        cx, cy, r = rng.uniform(0, S), rng.uniform(0, S), rng.uniform(6, 13)
        tone = np.array([110, 74, 140]) * rng.uniform(0.8, 1.15)
        for y in range(S):
            for x in range(S):
                for ox in (-S, 0, S):
                    for oy in (-S, 0, S):
                        d = math.hypot(x - cx - ox, y - cy - oy) / r
                        if d < 1:
                            img[y, x] = tone * (1.15 - 0.4 * d)
    img += noise(S, S, seed, 6)[..., None]
    return np.clip(img, 0, 255)


def draw_star(img, cx, cy, r_out, r_in, color, rot=-90):
    im = Image.fromarray(img.astype(np.uint8))
    d = ImageDraw.Draw(im)
    pts = []
    for i in range(10):
        a = math.radians(rot + i * 36)
        r = r_out if i % 2 == 0 else r_in
        pts.append((cx + math.cos(a) * r, cy + math.sin(a) * r))
    d.polygon(pts, fill=tuple(int(c) for c in color))
    return np.array(im).astype(float)


def emblem(img, cx, cy, r):
    """Emblema próprio: estrela dourada dentro de um anel."""
    im = Image.fromarray(img.astype(np.uint8))
    d = ImageDraw.Draw(im)
    d.ellipse((cx - r, cy - r, cx + r, cy + r), fill=(40, 52, 110), outline=(236, 190, 60), width=max(1, int(r // 5)))
    img = np.array(im).astype(float)
    return draw_star(img, cx, cy, r * 0.72, r * 0.3, (236, 190, 60))


def rod(img, x0, x1, y):
    img[y : y + 2, x0:x1] = (190, 150, 60)
    img[y + 2, x0:x1] = (110, 80, 30)
    img[y - 1 : y + 3, x0 - 2 : x0] = (236, 190, 60)
    img[y - 1 : y + 3, x1 : x1 + 2] = (236, 190, 60)
    return img


def banner(img, x0, x1, y0, y1, cloth, deco=True):
    cloth = np.array(cloth, dtype=float)
    img = rod(img, x0 - 2, x1 + 2, y0 - 3)
    for x in range(x0, x1):
        fold = 0.85 + 0.15 * math.cos((x - x0) / (x1 - x0) * math.pi * 3)
        for y in range(y0, y1):
            img[y, x] = cloth * fold
    # ponta em V
    for x in range(x0, x1):
        depth = int(4 * (1 - abs((x - x0) / (x1 - x0) * 2 - 1)))
        for y in range(y1, y1 + depth):
            img[y, x] = cloth * 0.8
    img[y0:y1, x0] = cloth * 0.55
    img[y0:y1, x1 - 1] = cloth * 0.55
    if deco:
        img[y0 + 2 : y0 + 3, x0 + 1 : x1 - 1] = (236, 190, 60)
        img[y1 - 3 : y1 - 2, x0 + 1 : x1 - 1] = (236, 190, 60)
    return img


def painting(img, x0, y0, x1, y1):
    """Quadro com paisagem (céu, montes, sol)."""
    img[y0:y1, x0:x1] = (150, 110, 40)
    img[y0 + 1 : y1 - 1, x0 + 1 : x1 - 1] = (220, 176, 70)
    ix0, iy0, ix1, iy1 = x0 + 3, y0 + 3, x1 - 3, y1 - 3
    h = iy1 - iy0
    for y in range(iy0, iy1):
        t = (y - iy0) / h
        img[y, ix0:ix1] = (90 + 80 * t, 140 + 60 * t, 220 - 40 * t)
    sx, sy = ix0 + (ix1 - ix0) * 0.7, iy0 + h * 0.3
    for y in range(iy0, iy1):
        for x in range(ix0, ix1):
            if math.hypot(x - sx, y - sy) < 3.2:
                img[y, x] = (255, 226, 120)
            hill = iy0 + h * (0.62 + 0.12 * math.sin((x - ix0) * 0.35))
            hill2 = iy0 + h * (0.78 + 0.08 * math.sin((x - ix0) * 0.22 + 2))
            if y > hill:
                img[y, x] = (70, 130, 80)
            if y > hill2:
                img[y, x] = (50, 100, 60)
    return img


def plaque(img, x0, y0, x1, y1):
    img[y0:y1, x0:x1] = (150, 110, 40)
    img[y0 + 1 : y1 - 1, x0 + 1 : x1 - 1] = (40, 44, 58)
    return emblem(img, (x0 + x1) / 2, (y0 + y1) / 2, min(x1 - x0, y1 - y0) / 2 - 3)


def arch(img):
    # abertura escura em arco com pedra de fecho
    for y in range(S):
        for x in range(S):
            inside = (abs(x - 32) < 16 and y > 30) or (math.hypot(x - 32, y - 30) < 16)
            ring = (abs(x - 32) < 19 and y > 30) or (math.hypot(x - 32, y - 30) < 19)
            if inside:
                img[y, x] = (22, 22, 30)
            elif ring:
                img[y, x] = img[y, x] * 1.15
    img[S - 4 :, 13:51] = (60, 56, 52)
    return emblem(img, 32, 30, 8)


def bars(img, robot=False):
    img[10:58, 8:56] = (14, 14, 20)
    if robot:
        # cabeça de robô atrás das grades
        img[34:50, 22:42] = (150, 146, 136)
        img[34:36, 22:42] = (190, 184, 168)
        img[40:43, 25:39] = (60, 110, 120)
        img[50:56, 18:46] = (90, 92, 104)
    for x in range(10, 56, 6):
        img[10:58, x : x + 2] = (130, 136, 150)
        img[10:58, x] = (180, 186, 200)
    img[10:12, 8:56] = (110, 116, 130)
    img[56:58, 8:56] = (110, 116, 130)
    return img


def metal_door():
    img = np.zeros((S, S, 3)) + np.array([88, 112, 120])
    img += noise(S, S, 11, 3)[..., None]
    for y in range(S):
        img[y, :] *= 0.95 + 0.05 * math.sin(y * 0.6)
    for (x0, y0, x1, y1) in ((6, 6, 58, 28), (6, 34, 58, 58)):
        img[y0:y1, x0:x1] *= 0.86
        img[y0, x0:x1] = (150, 176, 184)
        img[y1 - 1, x0:x1] = (50, 66, 72)
        img[y0:y1, x0] = (150, 176, 184)
        img[y0:y1, x1 - 1] = (50, 66, 72)
    img[29:33, :] = (236, 180, 52)
    for x in range(0, S, 8):
        img[29:33, x : x + 4] = (40, 40, 46)
    img[14:20, 50:54] = (70, 232, 244)
    for x, y in ((3, 3), (60, 3), (3, 60), (60, 60)):
        img[y : y + 2, x : x + 2] = (40, 50, 56)
    return np.clip(img, 0, 255)


def door_frame():
    img = stone((120, 120, 128), 21)
    img[:, 22:42] = (70, 74, 84)
    img[:, 22:24] = (120, 126, 140)
    img[:, 40:42] = (40, 42, 50)
    img[:, 30:34] = (52, 56, 64)
    for y in range(4, S, 10):
        img[y : y + 3, 26:38] = (236, 180, 52)
    return img


def lift_door():
    img = np.zeros((S, S, 3)) + np.array([150, 156, 166])
    for x in range(S):
        img[:, x] *= 0.92 + 0.08 * math.sin(x * 1.3)
    img[:, 31:33] = (50, 52, 60)
    img[:4, :] = (90, 96, 110)
    img[S - 4 :, :] = (90, 96, 110)
    # setas
    for i in range(6):
        img[8 + i, 16 - i : 16 + i + 1] = (70, 232, 244)
        img[8 + i, 48 - i : 48 + i + 1] = (70, 232, 244)
    return np.clip(img + noise(S, S, 5, 2)[..., None], 0, 255)


def lift_panel():
    img = np.zeros((S, S, 3)) + np.array([110, 116, 128])
    img[8:56, 18:46] = (60, 64, 74)
    img[8, 18:46] = (150, 156, 170)
    for i, y in enumerate((14, 26, 38)):
        img[y : y + 7, 26:38] = (236, 180, 52) if i == 1 else (70, 232, 244)
        img[y + 6, 26:38] = (40, 40, 46)
    img[48:52, 22:42] = (255, 64, 56)
    return np.clip(img + noise(S, S, 6, 2)[..., None], 0, 255)


def lever(img, down=True):
    img[22:46, 24:40] = (64, 68, 78)
    img[22, 24:40] = (130, 136, 150)
    img[31:35, 29:35] = (40, 40, 46)
    if down:
        for i in range(12):
            img[33 + i, 31:33] = (180, 180, 190)
        img[44:48, 29:35] = (255, 64, 56)
    else:
        for i in range(12):
            img[21 + i, 31:33] = (180, 180, 190)
        img[18:22, 29:35] = (60, 200, 90)
    return img


def dark(img, k=0.68):
    return np.clip(np.asarray(img) * k, 0, 255)


GREY = (128, 128, 136)
BLUE = (62, 86, 170)


def walls():
    w = {}
    w["stone0"] = stone(GREY, 1)
    w["stone1"] = stone(GREY, 2, rows=(8, 14, 10, 12, 8, 12))
    w["stone-painting"] = painting(stone(GREY, 3), 14, 12, 50, 46)
    w["stone-banner"] = emblem(banner(stone(GREY, 4), 18, 46, 6, 50, (44, 70, 160)), 32, 26, 8)
    w["stone-plaque"] = plaque(stone(GREY, 5), 18, 16, 46, 44)
    w["stone-arch"] = arch(stone(GREY, 6))
    w["blue0"] = stone(BLUE, 7)
    w["blue1"] = stone(BLUE, 8, rows=(12, 10, 12, 10, 12, 8))
    w["blue-cell"] = bars(stone(BLUE, 9))
    w["blue-cell-robot"] = bars(stone(BLUE, 10), robot=True)
    w["wood"] = wood(12)
    w["wood-emblem"] = emblem(wood(13), 32, 30, 13)
    w["wood-painting"] = painting(wood(14), 14, 10, 50, 48)
    w["brick"] = bricks((150, 70, 52), 15)
    w["cave"] = cave(16)
    w["door"] = metal_door()
    w["door-frame"] = door_frame()
    w["lift-door"] = lift_door()
    w["lift-panel"] = lift_panel()
    w["lever-down"] = lever(stone(GREY, 17), True)
    w["lever-up"] = lever(stone(GREY, 17), False)
    return w


# ------------------------------------------------------------------ fonte de pixéis (rótulos)

GLYPHS = {
    "A": ["010", "101", "111", "101", "101"],
    "B": ["110", "101", "110", "101", "110"],
    "C": ["011", "100", "100", "100", "011"],
    "D": ["110", "101", "101", "101", "110"],
    "E": ["111", "100", "110", "100", "111"],
    "I": ["111", "010", "010", "010", "111"],
    "L": ["100", "100", "100", "100", "111"],
    "M": ["10001", "11011", "10101", "10001", "10001"],
    "N": ["1001", "1101", "1011", "1001", "1001"],
    "O": ["010", "101", "101", "101", "010"],
    "P": ["110", "101", "110", "100", "100"],
    "S": ["011", "100", "010", "001", "110"],
    "T": ["111", "010", "010", "010", "010"],
    "U": ["101", "101", "101", "101", "111"],
    "V": ["101", "101", "101", "101", "010"],
    "Ç": ["011", "100", "100", "011", "010"],
    "Ã": ["010", "101", "101", "111", "101"],
    "Õ": ["011", "010", "101", "101", "010"],
    "3": ["111", "001", "011", "001", "111"],
    "D3": [],
    " ": ["0", "0", "0", "0", "0"],
}


def text_width(s, scale):
    return sum((len(GLYPHS[c][0]) + 1) * scale for c in s) - scale


def draw_text(img, s, x, y, scale, color, shadow=None):
    for c in s:
        g = GLYPHS[c]
        if c in "ÃÕ":  # til por cima
            for gx, gy in ((1, -2), (2, -2), (0, -1), (1, -1)):
                img[y + gy * scale : y + (gy + 1) * scale, x + gx * scale : x + (gx + 1) * scale] = color
        for gy, row in enumerate(g):
            for gx, bit in enumerate(row):
                if bit == "1":
                    if shadow is not None:
                        img[y + gy * scale + 1 : y + (gy + 1) * scale + 1, x + gx * scale + 1 : x + (gx + 1) * scale + 1] = shadow
                    img[y + gy * scale : y + (gy + 1) * scale, x + gx * scale : x + (gx + 1) * scale] = color
        x += (len(g[0]) + 1) * scale
    return img


# ------------------------------------------------------------------ cara do HUD (ecrã com olhos)

SCREEN_COLORS = [
    (70, 220, 140),
    (100, 220, 120),
    (170, 220, 90),
    (220, 210, 70),
    (236, 170, 60),
    (236, 120, 60),
    (230, 70, 60),
]


def face(state, look, dead=False, cheat=False):
    """31x32, depois aumenta 2x. state 0 (inteiro) .. 6 (quase a cair)."""
    W, H = 31, 32
    img = np.zeros((H, W, 3)) + np.array([30, 32, 40])
    # caixa do ecrã
    img[1:31, 1:30] = (150, 156, 170)
    img[1, 1:30] = (200, 206, 220)
    img[2:30, 1] = (200, 206, 220)
    img[30, 1:30] = (90, 94, 106)
    img[2:30, 29] = (90, 94, 106)
    # antena
    img[0:2, 14:17] = (230, 118, 40)
    scr = np.array(SCREEN_COLORS[min(state, 6)], dtype=float)
    bg = scr * 0.22
    img[4:24, 4:27] = bg
    for y in range(4, 24, 2):
        img[y, 4:27] *= 0.8  # linhas de varrimento
    eye = scr if not dead else np.array([120, 120, 130])
    ox = (-2, 0, 2)[look]
    if dead:
        for i in range(5):
            for cx in (10, 20):
                img[9 + i, cx - 2 + i] = eye
                img[9 + i, cx + 2 - i] = eye
        img[18, 10:21] = eye
    elif cheat:
        gold = np.array([255, 214, 80])
        img[9:14, 7:14] = (20, 20, 24)
        img[9:14, 17:24] = (20, 20, 24)
        img[10, 14:17] = (20, 20, 24)
        img[10, 8:10] = gold
        img[10, 18:20] = gold
        for x in range(10, 21):
            img[18 + (1 if x in (10, 20) else 0) - (1 if 13 <= x <= 17 else 0), x] = gold
    else:
        tired = state >= 4
        for cx in (10 + ox, 20 + ox):
            top = 9 + (2 if tired else 0)
            img[top:15, cx - 2 : cx + 2] = eye
            img[top, cx - 2 : cx + 2] = np.minimum(eye * 1.3, 255)
        # boca: sorriso -> reta -> triste
        mood = [2, 1, 1, 0, 0, -1, -1][state]
        for x in range(10, 21):
            t = (x - 15) / 5
            yy = 19 + int(round(mood * (1 - t * t)))
            img[yy, x + ox // 2] = eye
    # rachas no vidro com o dano
    rng = random.Random(state)
    for k in range(max(0, state - 1)):
        x, y = rng.randint(5, 25), rng.randint(5, 22)
        for _ in range(rng.randint(4, 8)):
            if 4 <= y < 24 and 4 <= x < 27:
                img[y, x] = (220, 230, 240)
            x += rng.choice((-1, 0, 1))
            y += rng.choice((-1, 1))
    # botões
    img[26:28, 6:9] = (230, 118, 40)
    img[26:28, 11:14] = (70, 232, 244)
    img[26:28, 23:26] = (60, 64, 74)
    return np.repeat(np.repeat(img, 2, axis=0), 2, axis=1)


def hud_bar(state, look, dead=False, cheat=False):
    W, H = 640, 80
    img = np.zeros((H, W, 3)) + np.array([26, 30, 40])
    for y in range(H):
        img[y] *= 0.9 + 0.1 * (y % 4 == 0)
    img[0:2] = (230, 118, 40)
    img[2:4] = (120, 66, 30)
    panels = [(8, 83, ""), (86, 198, "PONTOS"), (202, 262, ""), (332, 404, "VIDA"), (408, 478, "MUNIÇÃO"), (482, 632, "")]
    for x0, x1, label in panels:
        img[8:74, x0:x1] = (14, 18, 26)
        img[8, x0:x1] = (60, 70, 90)
        img[73, x0:x1] = (60, 70, 90)
        img[8:74, x0] = (60, 70, 90)
        img[8:74, x1 - 1] = (60, 70, 90)
        if label:
            tw = text_width(label, 2)
            img = draw_text(img, label, (x0 + x1) // 2 - tw // 2, 13, 2, (236, 190, 60), shadow=(70, 50, 20))
    # painel do emblema e luzes
    img = emblem(img, 45, 41, 24)
    for i, x in enumerate(range(210, 256, 12)):
        on = i < max(1, 4 - state // 2) and not dead
        img[30:50, x : x + 8] = (70, 232, 244) if on else (40, 50, 60)
        img[30, x : x + 8] = (190, 250, 255) if on else (60, 70, 80)
    # cara
    f = face(state, look, dead, cheat)
    img[8 : 8 + f.shape[0], 266 : 266 + f.shape[1]] = f
    return np.clip(img, 0, 255)


# ------------------------------------------------------------------ algarismos 14x30

SEG = {
    "0": "abcdef",
    "1": "bc",
    "2": "abged",
    "3": "abgcd",
    "4": "fgbc",
    "5": "afgcd",
    "6": "afgedc",
    "7": "abc",
    "8": "abcdefg",
    "9": "abcdfg",
}


def digit(ch):
    W, H = 7, 15
    g = np.zeros((H, W), dtype=bool)
    if ch == "%":
        g[1:3, 0:2] = True
        g[12:14, 5:7] = True
        for i in range(13):
            x = 6 - int(i * 6 / 12)
            g[1 + i, x] = True
    else:
        s = SEG[ch]
        if "a" in s:
            g[0:2, 1:6] = True
        if "g" in s:
            g[6:8, 1:6] = True
        if "d" in s:
            g[13:15, 1:6] = True
        if "f" in s:
            g[1:7, 0:2] = True
        if "b" in s:
            g[1:7, 5:7] = True
        if "e" in s:
            g[8:14, 0:2] = True
        if "c" in s:
            g[8:14, 5:7] = True
    big = np.repeat(np.repeat(g, 2, axis=0), 2, axis=1)
    img = np.zeros((30, 14, 3), dtype=np.uint8) + MAG
    img[big] = (250, 250, 250)
    # meia-sombra na metade de baixo de cada traço para dar volume
    for y in range(30):
        for x in range(14):
            if big[y, x] and (y + 1 >= 30 or not big[y + 1, x]):
                img[y, x] = (200, 200, 200)
    return img


# ------------------------------------------------------------------ cartazes


def poster_mission():
    """Substitui a capa do Wolfenstein no corredor do hub: o cartaz da missão."""
    W = 128
    img = np.zeros((W, W, 3))
    for y in range(W):
        t = y / W
        img[y] = (16 + 30 * t, 18 + 20 * t, 40 + 30 * t)
    rng = random.Random(3)
    for _ in range(40):
        x, y = rng.randint(0, W - 1), rng.randint(0, 70)
        img[y, x] = (200, 210, 255)
    # chão com grelha
    for y in range(96, W):
        img[y] = (40, 30, 50)
        if y % 6 == 0:
            img[y] = (230, 118, 40)
    for x in range(0, W, 12):
        img[96:, x] = (120, 66, 30)
    rob = render(ch.sentinel(pose="aim"), size=(128, 128), yaw=20, pitch=10, px=1.7,
                 anchor=(64, 118), emissive=ch.EMISSIVE, ground_shadow=False)
    m = rob[..., 3] > 0
    img[m] = rob[..., :3][m]
    img[3:22, 3:125] = (14, 18, 26)
    img[3, 3:125] = (230, 118, 40)
    img[21, 3:125] = (230, 118, 40)
    img = draw_text(img, "MISSÃO", 64 - text_width("MISSÃO", 2) // 2, 8, 2, (236, 190, 60), shadow=(70, 50, 20))
    img[0:2] = (60, 64, 74)
    img[W - 2 :] = (60, 64, 74)
    img[:, 0:2] = (60, 64, 74)
    img[:, W - 2 :] = (60, 64, 74)
    return np.clip(img, 0, 255)


def wood128():
    a = wood(30)
    return np.concatenate([np.concatenate([a, a], 1)] * 2, 0)


def banner128(cloth, inner):
    img = wood128()
    for x in range(26, 102):
        fold = 0.85 + 0.15 * math.cos((x - 26) / 76 * math.pi * 4)
        img[20:118, x] = np.array(cloth) * fold
    img[20:118, 26] = np.array(cloth) * 0.5
    img[20:118, 101] = np.array(cloth) * 0.5
    img = rod(img, 22, 106, 16)
    ih, iw = inner.shape[:2]
    y0, x0 = 66 - ih // 2, 64 - iw // 2
    mask = ~np.all(inner == MAG, axis=2) if inner.dtype == np.uint8 else np.ones(inner.shape[:2], bool)
    region = img[y0 : y0 + ih, x0 : x0 + iw]
    region[mask] = inner[mask]
    return np.clip(img, 0, 255)


def oms_wall():
    arms = Image.open(os.path.join(ROOT, "assets", "textures", "oms.bmp")).convert("RGB").resize((60, 60), Image.NEAREST)
    a = np.array(arms)
    bgmask = (a.sum(axis=2) < 40)  # fundo preto do brasão
    a = a.copy()
    a[bgmask] = MAG
    return banner128((54, 120, 70), a)


def pv_wall():
    p = np.array(Image.open(os.path.join(HERE, "src", "pv-portrait.png")).convert("RGB"))
    framed = np.zeros((p.shape[0] + 6, p.shape[1] + 6, 3), dtype=np.uint8)
    framed[:] = (220, 176, 70)
    framed[1:-1, 1:-1] = (150, 110, 40)
    framed[3:-3, 3:-3] = p
    return banner128((110, 110, 120), framed)


def portugal():
    img = stone(GREY, 40)
    img = banner(img, 16, 48, 6, 54, (230, 230, 226), deco=False)
    # escudo de Portugal simplificado: bordadura vermelha, campo branco, 5 escudetes azuis
    for y in range(18, 46):
        for x in range(21, 43):
            dy = y - 18
            half = 11 if dy < 18 else 11 - (dy - 18) * 1.1
            if abs(x - 31.5) <= half:
                inner = abs(x - 31.5) <= half - 3 and 21 <= y <= 42
                img[y, x] = (240, 240, 240) if inner else (200, 30, 36)
    for y, x in ((22, 28), (22, 33), (24, 30), (26, 28), (26, 33)):
        pass
    for cy, cx in ((25, 31), (30, 26), (30, 31), (30, 36), (35, 31)):
        img[cy - 2 : cy + 2, cx - 1 : cx + 2] = (30, 60, 160)
    for cy, cx in ((19, 24), (19, 31), (19, 38), (27, 22), (27, 41), (38, 24), (38, 38)):
        img[cy : cy + 2, cx : cx + 2] = (240, 200, 60)
    return img


# ------------------------------------------------------------------ main

DIRS = {
    "front": 0,
    "front-right": 45,
    "right": 90,
    "back-right": 135,
    "back": 180,
    "back-left": 225,
    "left": 270,
    "front-left": 315,
}


def characters():
    E = ch.EMISSIVE
    walk = [(25, 20, 0.0), (0, 0, 0.6), (-25, -20, 0.0), (0, 0, 0.6)]
    for name, yaw in DIRS.items():
        save(render(ch.sentinel(), yaw=yaw, emissive=E), f"billboards/sentinel/still-{name}.bmp")
        for i, (l, a, b) in enumerate(walk, 1):
            save(render(ch.sentinel(leg=l, arm=a, bob=b), yaw=yaw, emissive=E), f"billboards/sentinel/{name}{i}.bmp")
        for i in range(4):
            save(render(ch.hound(phase=i / 4), yaw=yaw, emissive=E), f"billboards/hound/{name}{i + 1}.bmp")
    save(render(ch.sentinel(pose="aim"), emissive=E), "billboards/sentinel/aim.bmp")
    save(render(ch.sentinel(pose="shoot", flash=True), emissive=E), "billboards/sentinel/shoot.bmp")
    save(render(ch.sentinel(pose="halt"), emissive=E), "billboards/sentinel/halt.bmp")
    save(render(ch.sentinel_hit(), emissive=E), "billboards/sentinel/hit.bmp")
    for s in range(1, 6):
        save(render(ch.sentinel_death(s), emissive=E, center=True), f"billboards/sentinel/death{s}.bmp")
    for i, j in enumerate((-10, -30, -18), 1):
        save(render(ch.hound(phase=0.0, jaw=j, gait=0.2), emissive=E), f"billboards/hound/bite{i}.bmp")
    save(render(ch.hound(phase=0.25, jaw=-8, gait=0.3), yaw=8, emissive=E), "billboards/hound/hit.bmp")
    for s in range(1, 4):
        save(render(ch.hound_death(s), emissive=E, center=True), f"billboards/hound/death{s}.bmp")


def decor_items():
    for name, (fn, opt) in dc.DECOR.items():
        m = fn()
        if opt.get("hang"):
            zmax = max(p[2] for p in m.v)
            r = render(m, pitch=14, px=2, anchor=(64, zmax * 2 * math.cos(math.radians(14)) + 1),
                       emissive=dc.EMISSIVE, ground_shadow=False)
        else:
            r = render(m, pitch=16, px=2.6, emissive=dc.EMISSIVE)
        save(r, f"billboards/{name}.bmp")


def fit(rgba, size):
    a = rgba[..., 3] > 0
    ys, xs = np.where(a)
    crop = rgba[ys.min() : ys.max() + 1, xs.min() : xs.max() + 1]
    im = Image.fromarray(crop, "RGBA")
    W, H = size
    k = min(W / im.width, H / im.height)
    im = im.resize((max(1, int(im.width * k)), max(1, int(im.height * k))), Image.NEAREST)
    out = Image.new("RGBA", size, (0, 0, 0, 0))
    out.paste(im, ((W - im.width) // 2, (H - im.height) // 2))
    return np.array(out)


def weapons():
    E = dc.EMISSIVE
    guns = {"blaster": (dc.blaster, (103, 69)), "rifle": (dc.rifle, (78, 24)), "cannon": (dc.cannon, (104, 30))}
    for name, (fn, icon) in guns.items():
        g = fn()
        frames = [dict(), dict(lift=1.5), dict(recoil=2, flash=True, lift=1.5), dict(recoil=1, lift=1), dict(lift=0.5)]
        for i, kw in enumerate(frames):
            m = dc.hand_model(g, **kw)
            r = render(m, yaw=160, pitch=50, px=2.8, anchor=(72, 116), emissive=E, ground_shadow=False)
            save(r, f"items/{name}/screen/{i}.bmp")
        side = render(g, yaw=90, pitch=0, px=3, anchor=(64, 64), emissive=E, ground_shadow=False, size=(128, 128))
        save(fit(side, icon), f"items/{name}/icon.bmp")
    for i in range(5):
        r = render(dc.blade_swing(i), yaw=170, pitch=30, px=3.0, anchor=(70, 120), emissive=E, ground_shadow=False)
        save(r, f"items/blade/screen/{i}.bmp")
    side = render(dc.blade(), yaw=90, pitch=0, px=3, anchor=(64, 100), emissive=E, ground_shadow=False)
    save(fit(side, (74, 94)), "items/blade/icon.bmp")


def hud():
    for s in range(7):
        for look in range(3):
            save(hud_bar(s, look), f"hud/stats/{s}-{look}.bmp")
    save(hud_bar(6, 1, dead=True), "hud/stats/7-0.bmp")
    save(hud_bar(0, 1, cheat=True), "hud/stats/7-1.bmp")
    save(hud_bar(6, 1, dead=True), "hud/stats/7-2.bmp")
    fdir = os.path.join(ROOT, "assets", "fonts", "hud")
    for c in "%0123456789":
        save(digit(c), f"{ord(c)}.bmp", root=fdir)


def main():
    what = sys.argv[1:] or ["walls", "chars", "decor", "weapons", "hud", "posters"]
    if "walls" in what:
        for name, img in walls().items():
            save(img, f"{name}.bmp")
            save(dark(img), f"{name}-dark.bmp")
    if "chars" in what:
        characters()
    if "decor" in what:
        decor_items()
    if "weapons" in what:
        weapons()
    if "hud" in what:
        hud()
    if "posters" in what:
        t = os.path.join(ROOT, "assets", "textures")
        save(poster_mission(), "missao.bmp", root=t)
        save(oms_wall(), "oms-wall.bmp", root=t)
        save(pv_wall(), "pv-wall.bmp", root=t)
        save(portugal(), "portugal.bmp", root=os.path.join(ROOT, "assets", "pt", "textures"))


if __name__ == "__main__":
    main()
