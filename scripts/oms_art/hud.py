# Barra de estado e cara do jogador: um soldado português dos anos 60/70
# (capacete verde-oliva, patilhas, bigode, gola da farda) que se vai ferindo
# e ensanguentando à medida que perde vida. Píxeis escritos à mão aqui.
import random

import numpy as np

from pixart import grid, paste, scale
from pixfont import G, centered, draw_text, text_width

# metade esquerda da cara (16 colunas, espelhada para 32x32)
FACE = [
    "kkkkkkkkkkkkkkkk",
    "kkkkkkkkkoGGGGGG",
    "kkkkkkoGGGGGgggG",
    "kkkkkoGGGGGgggGG",
    "kkkkoGGGGGGGGGGG",
    "kkkoGGGGGGGGGGGG",
    "kkoGGGGGGGGGGGGG",
    "kkoGGGGGGGGGGGGG",
    "koDDDDDDDDDDDDDD",
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
    "n": (196, 138, 98), "w": (92, 100, 58), "c": (112, 118, 70),
    "G": (88, 100, 56), "g": (128, 140, 84), "D": (46, 52, 30), "o": (30, 34, 20),
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
        # sorriso largo e óculos escuros
        f[21, 12:20, :3] = (240, 236, 226)
        f[22, 13:19, :3] = (150, 70, 60)
        f[11:13, 8:14, :3] = (20, 20, 24)  # óculos escuros
        f[11:13, 18:24, :3] = (20, 20, 24)
        f[11, 14:18, :3] = (20, 20, 24)
        f[11, 9:11, :3] = (200, 210, 230)
        f[11, 19:21, :3] = (200, 210, 230)
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
    # grelha: margem 6, intervalos de 4, todas as caixas de y 6 a 74.
    # Os números são desenhados pelo jogo a meio da barra (y ~ 40-66).
    panels = [(6, 80, ""), (84, 196, "PONTOS"), (200, 260, "FORÇA"), (264, 330, None),
              (334, 406, "VIDA"), (410, 480, "BALAS"), (484, 634, "")]
    LABEL_Y = 12
    for x0, x1, label in panels:
        img = bevel_box(img, x0, 6, x1, 74)
        if label:
            img = centered(img, label, (x0 + x1) / 2, LABEL_Y, 2, GOLD, shadow=(10, 10, 20))
    # emblema: esfera armilar com o escudo e a palavra PÁTRIA, centrados
    import flags

    a = flags.arms(15)  # 35x35
    cx = (6 + 80) // 2
    top = 6 + (68 - (35 + 4 + 10)) // 2
    paste(img, a, cx - a.shape[1] // 2, top)
    img = centered(img, "PÁTRIA", cx, top + 39, 2 if text_width("PÁTRIA", 2) <= 66 else 1,
                   (236, 230, 210), shadow=(10, 10, 20))
    # quatro lâmpadas de força, centradas como grupo
    bw, gap = 8, 5
    x = (200 + 260) // 2 - (4 * bw + 3 * gap) // 2
    for i in range(4):
        on = i < max(1, 4 - state // 2) and not dead
        xx = x + i * (bw + gap)
        img[30:66, xx : xx + bw] = (230, 40, 40) if on else (70, 26, 30)
        img[30, xx : xx + bw] = (255, 170, 150) if on else (90, 50, 50)
        img[65, xx : xx + bw] = (110, 10, 14)
    # cara (32x32 -> 64x64), por trás um fundo próprio
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
    """Cartaz da entrada da campanha: DEFENDER A PÁTRIA -- o forte ao fim da
    tarde com a bandeira nacional no alto da muralha."""
    import flags

    W = 128
    img = np.zeros((W, W, 3))
    for y in range(W):
        t = y / W
        img[y] = (60 + 120 * t, 80 + 60 * t, 120 - 30 * t)
    sil = (34, 30, 28)
    img[84:W, :] = sil
    for x in range(0, W, 12):
        img[76:84, x : x + 7] = sil
    img[52:84, 82:110] = sil  # torre
    for x in range(82, 110, 7):
        img[46:52, x : x + 4] = sil
    img[96:W, :] = (26, 22, 22)
    # mastro e bandeira em cima da torre
    img[14:47, 94:96] = (60, 50, 40)
    fl = np.zeros((22, 32, 3))
    fl[:, :13] = flags.GREEN
    fl[:, 13:] = flags.RED
    ar = flags.arms(8)
    m = ar[..., 3] > 0
    oy, ox = 11 - ar.shape[0] // 2, 13 - ar.shape[1] // 2
    region = fl[max(0, oy) : oy + ar.shape[0], max(0, ox) : ox + ar.shape[1]]
    region[m[: region.shape[0], : region.shape[1]]] = ar[..., :3][m][: region[m[: region.shape[0], : region.shape[1]]].shape[0]]
    img[14:36, 96:128] = fl[:, :32]
    img[4:34, 4:90] = (20, 18, 18)
    img[4, 4:90] = GOLD
    img[33, 4:90] = GOLD
    img = centered(img, "DEFENDER", 47, 9, 2, (240, 230, 200))
    img = centered(img, "A PÁTRIA", 47, 21, 2, (240, 210, 60))
    img = centered(img, "DEFESA DO FORTE", 64, 112, 1, (240, 230, 200), shadow=(0, 0, 0))
    img[0:2] = (60, 40, 30)
    img[W - 2 :] = (60, 40, 30)
    img[:, 0:2] = (60, 40, 30)
    img[:, W - 2 :] = (60, 40, 30)
    return np.clip(img, 0, 255)
