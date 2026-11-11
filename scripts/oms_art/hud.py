# Barra de estado e cara do jogador: um resistente português dos anos 70
# (cabelo farto, patilhas, bigode, colarinho aberto) que se vai ferindo e
# ensanguentando à medida que perde vida. Píxeis escritos à mão aqui.
import random

import numpy as np

from pixart import grid, paste, scale
from pixfont import G, centered, draw_text, text_width

# metade esquerda da cara (16 colunas, espelhada para 32x32)
FACE = [
    "kkkkkkkkkkkkkkkk",
    "kkkkkkkkkhhhhhhh",
    "kkkkkkhhhHHHHHHH",
    "kkkkkhHHHHHHHHHh",
    "kkkkhHHHHHHHHHHH",
    "kkkhHHHHHHHHHHHH",
    "kkkHHHHHHHHHHHhH",
    "kkHHHHHHHHSSSHHH",
    "kkHHHHHHSSSSSSSS",
    "kkHHHHHdSSSSSSSS",
    "kkHHHHdSbbbbSSSS",
    "kkHHHHdSSSSSSSSS",
    "kkHHHHdSSSSSSSSS",
    "kkkHHHdsSSSSSSsS",
    "kkkHHsdsSSSSSSsS",
    "kkkkssdsSSSSSSsS",
    "kkkksddsSSSSSsdS",
    "kkkkkHdsSSSSSdDd",
    "kkkkkHdsSSSSmmmm",
    "kkkkkkdsSSmmmmmm",
    "kkkkkkdsSmmSSSSS",
    "kkkkkkdssmSSSlll",
    "kkkkkkkdssSSSSSS",
    "kkkkkkkdsssSSSSS",
    "kkkkkkkkdssssSSS",
    "kkkkkkkkkddsssss",
    "kkkkkkkkkkkddsss",
    "kkkkkkkkkkkkndnn",
    "kkkkkkcccccwwnnn",
    "kkkkcccccccwwwnn",
    "kkkcccccccccwwwn",
    "kkccccccccccwwwn",
]
FCOL = {
    "k": None, "h": (92, 60, 34), "H": (56, 34, 20), "S": (224, 170, 128), "s": (190, 132, 94),
    "d": (142, 90, 60), "D": (96, 56, 36), "b": (50, 30, 18), "m": (60, 36, 22), "l": (150, 70, 60),
    "n": (196, 138, 98), "w": (226, 222, 200), "c": (110, 78, 46),
}
BLOOD = (170, 10, 14)
BLOOD_D = (110, 4, 8)
BRUISE = (120, 70, 110)

# feridas acumuladas por estado (coordenadas na cara de 32x32)
WOUNDS = [
    [],
    [("b", 21, 8), ("b", 22, 8), ("b", 22, 9)],
    [("b", 15, 18), ("b", 16, 18), ("u", 8, 13), ("u", 9, 13), ("u", 12, 13)],
    [("b", 22, 10), ("b", 22, 11), ("b", 23, 12), ("b", 16, 19), ("u", 8, 10), ("b", 10, 7), ("b", 11, 7)],
    [("b", 23, 13), ("b", 23, 14), ("b", 24, 15), ("b", 10, 8), ("b", 9, 9), ("B", 15, 20), ("b", 18, 22), ("b", 19, 23)],
    [("b", 24, 16), ("b", 24, 17), ("B", 22, 8), ("B", 21, 9), ("b", 8, 9), ("b", 8, 16), ("b", 9, 17), ("b", 12, 24), ("b", 13, 25), ("u", 20, 14), ("u", 21, 14)],
    [("B", 12, 8), ("B", 13, 8), ("b", 14, 9), ("b", 7, 14), ("b", 7, 15), ("B", 16, 21), ("b", 17, 24), ("b", 17, 25), ("b", 25, 18), ("b", 25, 19), ("B", 23, 9), ("b", 20, 26)],
]


def base_face():
    return grid(FACE, FCOL, mirror=True)


def eyes(f, look, tired=False, closed=False, glow=False):
    ox = (-1, 0, 1)[look]
    for x0 in (9, 19):
        if closed:
            f[12, x0 : x0 + 4, :3] = (60, 36, 22)
            f[12, x0 : x0 + 4, 3] = 255
            continue
        top = 12 if tired else 11
        for y in range(top, 13):
            for x in range(x0, x0 + 4):
                f[y, x, :3] = (240, 236, 226)
        for y in range(top, 13):
            px = x0 + 1 + ox
            f[y, px : px + 2, :3] = (230, 200, 60) if glow else (50, 34, 20)
        if tired:
            f[11, x0 : x0 + 4, :3] = (170, 116, 80)
    return f


def face(state, look, dead=False, cheat=False):
    f = base_face()
    if cheat:
        f = eyes(f, 1, glow=True)
        # sorriso largo e um cravo atrás da orelha
        f[21, 12:20, :3] = (240, 236, 226)
        f[22, 13:19, :3] = (150, 70, 60)
        for (x, y) in ((27, 13), (28, 13), (27, 14), (28, 14), (29, 14), (28, 15), (29, 12), (26, 12)):
            f[y, x] = (226, 30, 40, 255)
        f[16:19, 27, :3] = (60, 140, 60)
        f[16:19, 27, 3] = 255
        return f
    st = 6 if dead else state
    for k in range(st + 1):
        for kind, x, y in WOUNDS[k]:
            c = {"b": BLOOD, "B": BLOOD_D, "u": BRUISE}[kind]
            f[y, x, :3] = c
            # o sangue escorre um píxel
            if kind != "u" and k >= 3 and y + 1 < 27 and f[y + 1, x, 3]:
                f[y + 1, x, :3] = BLOOD_D
    if dead:
        sk = f[..., :3].astype(float)
        mask = np.all(f[..., :3] == FCOL["S"], axis=2) | np.all(f[..., :3] == FCOL["s"], axis=2)
        sk[mask] = sk[mask] * 0.78 + np.array([40, 44, 50]) * 0.22
        f[..., :3] = sk.astype(np.uint8)
        f = eyes(f, 1, closed=True)
        f[21, 13:19, :3] = (90, 40, 36)
    else:
        f = eyes(f, look, tired=state >= 4)
        if state >= 5:
            f[21, 13:19, :3] = (90, 40, 36)  # boca a ranger
    return f


# ------------------------------------------------------------------ barra

NAVY = np.array([30, 44, 92], float)
NAVY_D = np.array([14, 20, 46], float)
NAVY_L = np.array([80, 104, 170], float)
GOLD = (232, 190, 80)

CRAVO_SMALL = [
    "..rRr.rR..",
    ".rRRrrRRr.",
    "rRrRRrRrRr",
    ".rRrRRRrr.",
    "..rrRRrr..",
    "...gGGg...",
    "....Gg....",
    "....gG.gl.",
    "....Ggl...",
    "...gG.....",
    "...Gg.....",
    "..gG......",
]
CRAVO_C = {"r": (150, 10, 20), "R": (226, 30, 40), "g": (30, 90, 40), "G": (70, 150, 60), "l": (90, 170, 70)}


def bevel_box(img, x0, y0, x1, y1, inset=True):
    img[y0:y1, x0:x1] = NAVY_D if inset else NAVY
    a, b = (NAVY_D * 0.6, NAVY_L) if inset else (NAVY_L, NAVY_D * 0.6)
    img[y0, x0:x1] = a
    img[y0:y1, x0] = a
    img[y1 - 1, x0:x1] = b
    img[y0:y1, x1 - 1] = b
    return img


def hud_bar(state, look, dead=False, cheat=False):
    W, H = 640, 80
    img = np.zeros((H, W, 3)) + NAVY
    rng = np.random.default_rng(1)
    img += rng.normal(0, 3, (H, W))[..., None]
    img[0:2] = NAVY_L
    img[2] = NAVY_L * 0.8
    img[H - 2 :] = NAVY_D
    panels = [(8, 83, ""), (86, 198, "PONTOS"), (202, 262, "FORÇA"), (332, 404, "VIDA"), (408, 478, "BALAS"), (482, 632, "")]
    for x0, x1, label in panels:
        img = bevel_box(img, x0, 6, x1, 75)
        if label:
            img = centered(img, label, (x0 + x1) / 2, 11, 2 if text_width(label, 2) < x1 - x0 - 6 else 1, GOLD, shadow=(10, 10, 20))
    # emblema do jogador: cravo e a data
    c = scale(grid(CRAVO_SMALL, CRAVO_C), 4)
    paste(img, c, 26, 10)
    img = centered(img, "25·IV", 45, 61, 2, (236, 230, 210), shadow=(10, 10, 20))
    # quatro lâmpadas de força
    for i, x in enumerate(range(210, 256, 12)):
        on = i < max(1, 4 - state // 2) and not dead
        img[20:48, x : x + 8] = (230, 40, 40) if on else (70, 26, 30)
        img[20, x : x + 8] = (255, 170, 150) if on else (90, 50, 50)
        img[47, x : x + 8] = (110, 10, 14)
    # cara (32x32 -> 64x64), por trás um fundo próprio
    img = bevel_box(img, 264, 6, 330, 75)
    f = scale(face(state, look, dead, cheat), 2)
    img[8:72, 265:329] = NAVY_D * 1.6
    paste(img, f, 265, 8)
    return np.clip(img, 0, 255)


# ------------------------------------------------------------------ algarismos 14x30

PCT = "101 001 010 100 101"


def digit(ch):
    """Algarismos grossos: a letra 3x5 com colunas 2-3-2 e linhas de 3."""
    rows = (PCT if ch == "%" else G[ch]).split(" ")
    cw, rh = (2, 3, 2), 3
    g = np.zeros((15, 7), bool)
    for gy, r in enumerate(rows):
        x = 0
        for gx, b in enumerate(r):
            if b == "1":
                g[gy * rh : gy * rh + rh, x : x + cw[gx]] = True
            x += cw[gx]
    big = np.repeat(np.repeat(g, 2, axis=0), 2, axis=1)
    img = np.zeros((30, 14, 3), np.uint8) + np.array([255, 0, 255], np.uint8)
    img[big] = (250, 250, 240)
    for y in range(30):
        for x in range(14):
            if big[y, x] and (y + 1 >= 30 or not big[y + 1, x]):
                img[y, x] = (190, 186, 170)
            if big[y, x] and (y == 0 or not big[y - 1, x]):
                img[y, x] = (255, 255, 255)
    return img


# ------------------------------------------------------------------ cartaz da campanha (128x128)


def poster_mission():
    """Cartaz da entrada da campanha: o forte ao entardecer e um cravo
    gigante, OPERAÇÃO CRAVO."""
    W = 128
    img = np.zeros((W, W, 3))
    for y in range(W):
        t = y / W
        img[y] = (120 + 90 * t, 30 + 40 * t, 30 + 10 * t)
    # sol
    for y in range(W):
        for x in range(W):
            if (x - 92) ** 2 + (y - 70) ** 2 < 18 ** 2:
                img[y, x] = (250, 200, 90)
    # forte (silhueta)
    sil = (30, 18, 20)
    img[78:W, :] = sil
    for x in range(0, W, 10):
        img[72:78, x : x + 6] = sil
    img[56:78, 14:34] = sil
    for x in range(14, 34, 6):
        img[52:56, x : x + 4] = sil
    img[90:W, :] = (24, 14, 16)
    # cravo grande
    c = scale(grid(CRAVO_SMALL, CRAVO_C), 6)
    paste(img, c, 34, 38)
    img[4:34, 4:124] = (20, 16, 18)
    img[4, 4:124] = GOLD
    img[33, 4:124] = GOLD
    img = centered(img, "OPERAÇÃO", 64, 9, 2, (240, 230, 200))
    img = centered(img, "CRAVO", 64, 21, 2, (236, 50, 50))
    img = centered(img, "LIBERDADE", 64, 112, 2, (240, 230, 200), shadow=(0, 0, 0))
    img[0:2] = (60, 40, 30)
    img[W - 2 :] = (60, 40, 30)
    img[:, 0:2] = (60, 40, 30)
    img[:, W - 2 :] = (60, 40, 30)
    return np.clip(img, 0, 255)
