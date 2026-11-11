# Objetos do cenário, pontos a apanhar e armas do tema Estado Novo. Os
# móveis e objetos são voxels de baixa resolução; os pontos pequenos (cravo,
# jornal Avante!, mala de escudos, comida, estojo) são píxeis desenhados à
# mão em 64x64, como nos jogos VGA. Tudo original.
import math
import random

import numpy as np

from characters import SKIN, SKIN_D
from pixart import grid, paste
from pixfont import centered, draw_text
from vox import Model, box, cyl_x, cyl_y, cyl_z, ellipsoid, rot_x, transformed

GOLD = (222, 172, 52)
GOLD_D = (150, 104, 26)
WOOD = (130, 82, 44)
WOOD_D = (88, 54, 28)
WOOD_L = (170, 118, 66)
STONE = (150, 146, 136)
STONE_D = (104, 100, 92)
IRON = (60, 60, 64)
IRON_L = (110, 110, 116)
LEAF = (60, 130, 60)
LEAF_D = (36, 90, 40)
AZ = (36, 74, 168)
AZ_W = (236, 238, 232)
CLAY = (176, 92, 54)
WHITE = (236, 232, 220)
RED = (190, 24, 26)
GREEN = (26, 110, 52)
LAMP = (255, 236, 160)
PAPER = (222, 212, 180)
BRASS = (196, 150, 60)

EMISSIVE = (LAMP, (255, 236, 120), (255, 150, 40),
            (240, 196, 40), (40, 26, 8), (200, 22, 24), (240, 238, 230), (20, 50, 170))


# ------------------------------------------------------------------ voxels


def ammo():
    m = Model()
    box(m, -7, 7, -4, 4, 0, 6, (86, 92, 60))
    box(m, -7.1, 7.1, -4.1, 4.1, 5, 6, (60, 64, 40))
    box(m, -1, 1, -4.3, -4.0, 2, 4, (200, 190, 140))
    for x in (-4.5, -1.5, 1.5, 4.5):
        box(m, x - 1, x + 1, -2, 2, 6, 8, (60, 60, 60))
        for y in (-1.2, 0, 1.2):
            box(m, x - 0.4, x + 0.4, y - 0.4, y + 0.4, 8, 10, BRASS)
    return m


def statue():
    """Monumento com a esfera armilar sobre um pedestal."""
    m = Model()
    box(m, -8, 8, -8, 8, 0, 4, STONE_D)
    box(m, -6, 6, -6, 6, 4, 22, STONE)
    box(m, -7, 7, -7, 7, 20, 23, STONE_D)
    box(m, -2, 2, -2, 2, 23, 26, GOLD_D)
    cz, r = 34, 8
    for i in range(160):
        a = 2 * math.pi * i / 160
        c, s = math.cos(a) * r, math.sin(a) * r
        m.put(c, 0, cz + s, GOLD)  # anel frontal
        m.put(0, c, cz + s, GOLD)  # anel lateral
        m.put(c, s, cz, GOLD_D)  # equador
        # eclíptica inclinada
        m.put(c, s * 0.9, cz + c * 0.5, (255, 226, 120))
    for z in range(int(cz - r - 2), int(cz + r + 3)):
        m.put(0, 0, z, GOLD_D)
    return m


def barrel(color=WOOD, band=IRON):
    """Pipa de vinho de madeira com arcos de ferro."""
    m = Model()
    for z in range(0, 22):
        r = 6.0 + 1.2 * math.sin(math.pi * z / 21)
        c = band if z in (2, 3, 18, 19, 10) else (color if int(z) % 2 else tuple(int(v * 0.9) for v in color))
        cyl_z(m, 0, 0, r, z, z + 0.5, c)
    for a in range(0, 360, 30):  # aduelas
        x, y = math.cos(math.radians(a)) * 7.1, math.sin(math.radians(a)) * 7.1
        box(m, x - 0.3, x + 0.3, y - 0.3, y + 0.3, 4, 17, WOOD_D)
    cyl_z(m, 0, 0, 5.8, 21.5, 22, WOOD_D)
    return m


def drum():
    m = Model()
    for z in range(0, 22):
        c = (50, 90, 50) if z not in (4, 5, 16, 17) else (36, 66, 36)
        cyl_z(m, 0, 0, 6.5, z, z + 0.5, c)
    cyl_z(m, 0, 0, 6.5, 21.5, 22, (70, 110, 70))
    cyl_z(m, 3, 0, 1, 22, 22.6, IRON)
    box(m, -2, 2, -6.8, -6.4, 9, 13, (220, 200, 60))
    return m


def plant(pot, leaf=LEAF, leaf_d=LEAF_D, tall=26, tiles=False):
    m = Model()
    for z in range(0, 9):
        r = 4 + z * 0.25
        c = pot
        if tiles:
            c = AZ if (z // 2) % 2 else AZ_W
        cyl_z(m, 0, 0, r, z, z + 0.5, c)
    cyl_z(m, 0, 0, 5.8, 8.5, 9.5, pot if not tiles else AZ)
    cyl_z(m, 0, 0, 0.8, 9, tall - 6, (96, 70, 44))
    for i in range(14):
        a = i * 137.5
        h = 10 + (i * 7) % (tall - 10)
        r = 2 + (tall - h) * 0.35
        x, y = math.cos(math.radians(a)) * r, math.sin(math.radians(a)) * r
        ellipsoid(m, x, y, h, 3.0, 3.0, 2.2, leaf if i % 3 else leaf_d)
    ellipsoid(m, 0, 0, tall - 2, 4, 4, 3, leaf)
    return m


def vase():
    m = Model()
    for z in range(0, 24):
        t = z / 23
        r = 3 + 4.5 * math.sin(math.pi * min(1, t * 1.25)) if t < 0.8 else 2.6 + (t - 0.8) * 6
        c = AZ if z % 6 in (0, 3) or z in (10, 11, 12) and False else AZ_W
        if 9 <= z <= 13:
            c = AZ if z % 2 else AZ_W
        cyl_z(m, 0, 0, r, z, z + 0.5, c)
    return m


def pot_clay():
    m = Model()
    for z in range(0, 14):
        r = 4 + 3.0 * math.sin(math.pi * z / 15)
        cyl_z(m, 0, 0, r, z, z + 0.5, CLAY if z not in (5, 10) else (130, 64, 36))
    for s in (-1, 1):
        box(m, s * 6.8 - 0.6, s * 6.8 + 0.6, -0.6, 0.6, 9, 13, CLAY)
    return m


def files():
    """Processos espalhados pelo chão: pastas e folhas."""
    m = Model()
    r = random.Random(4)
    for i in range(12):
        x, y = r.uniform(-12, 12), r.uniform(-6, 6)
        w, d = r.uniform(2.5, 4), r.uniform(3, 4.5)
        c = r.choice([PAPER, PAPER, (200, 180, 120), (170, 120, 70)])
        box(m, x - w, x + w, y - d, y + d, 0, 0.6 + (i % 3) * 0.5, c)
    box(m, -4, 4, -3, 3, 0, 4, (170, 120, 70))  # maço de pastas
    box(m, -4.1, 4.1, -0.3, 0.3, 0, 4.2, (150, 30, 30))  # fita
    return m


def books():
    """Pilha de livros apreendidos."""
    m = Model()
    r = random.Random(7)
    z = 0
    cols = [(130, 30, 30), (30, 60, 110), (40, 90, 50), (120, 90, 40), (60, 40, 70)]
    for i in range(7):
        w, d, h = r.uniform(4, 6), r.uniform(3, 4), r.uniform(1.4, 2.4)
        x, y = r.uniform(-1.5, 1.5), r.uniform(-1, 1)
        box(m, x - w, x + w, y - d, y + d, z, z + h, cols[i % len(cols)])
        box(m, x - w + 0.4, x + w - 0.4, y - d - 0.1, y + d - 0.4, z + 0.3, z + h - 0.3, PAPER)
        z += h
    box(m, 6, 14, -4, 3, 0, 2, cols[0])
    box(m, -15, -8, -2, 4, 0, 1.6, cols[1])
    return m


def ceiling_light():
    m = Model()
    cyl_z(m, 0, 0, 0.5, 50, 60, IRON)
    for z in range(0, 5):
        cyl_z(m, 0, 0, 2 + z * 1.3, 50 - z, 50.5 - z, (40, 90, 60))
    cyl_z(m, 0, 0, 2.2, 44.5, 46, LAMP)
    return m


def chandelier():
    m = Model()
    cyl_z(m, 0, 0, 0.5, 50, 60, GOLD_D)
    cyl_z(m, 0, 0, 1.5, 46, 50, GOLD)
    for a in range(0, 360, 45):
        x, y = math.cos(math.radians(a)) * 8, math.sin(math.radians(a)) * 8
        box(m, min(0, x) - 0.5, max(0, x) + 0.5, min(0, y) - 0.5, max(0, y) + 0.5, 46, 47, GOLD)
        cyl_z(m, x, y, 0.9, 47, 49, WHITE)
        cyl_z(m, x, y, 0.6, 49, 50.5, LAMP)
    return m


def chains():
    m = Model()
    for x in (-5, 0, 5):
        top = 60
        n = 6 if x else 8
        for i in range(n):
            z = top - i * 2.2
            if i % 2:
                box(m, x - 0.3, x + 0.3, -1, 1, z - 1.2, z, IRON_L)
            else:
                box(m, x - 1, x + 1, -0.3, 0.3, z - 1.2, z, IRON)
        if x == 0:
            cyl_y(m, 0, top - n * 2.2 - 1.6, 1.8, -0.5, 0.5, IRON_L)  # argola
    return m


def typewriter():
    """Máquina de escrever num móvel baixo."""
    m = Model()
    box(m, -7, 7, -5, 5, 0, 12, WOOD)
    box(m, -7.2, 7.2, -5.2, 5.2, 11, 12.5, WOOD_D)
    box(m, -6, 6, -4.5, -4.3, 2, 10, WOOD_D)
    box(m, -5, 5, -4, 3, 12.5, 15, (34, 34, 38))
    box(m, -5.5, 5.5, 1, 4, 15, 18, (34, 34, 38))
    cyl_x(m, 2.5, 18.5, 1.2, -7, 7, (20, 20, 22))
    box(m, -4, 4, 2, 2.4, 18, 23, WHITE)  # folha
    for x in range(-4, 5, 2):
        for y in (-3, -1.5, 0):
            box(m, x - 0.4, x + 0.4, y - 0.4, y + 0.4, 15, 15.6, (200, 196, 180))
    return m


def lamp():
    m = Model()
    cyl_z(m, 0, 0, 3.5, 0, 1.2, IRON)
    cyl_z(m, 0, 0, 0.6, 1, 30, IRON_L)
    for z in range(0, 7):
        cyl_z(m, 0, 0, 5.5 - z * 0.5, 28 + z, 28.5 + z, (190, 170, 120))
    cyl_z(m, 0, 0, 2, 27.5, 28.5, LAMP)
    return m


# cores das armas: sem sombreado (ver EMISSIVE), para se lerem bem
ARMS_Y = (240, 196, 40)
ARMS_K = (40, 26, 8)
ARMS_R = (200, 22, 24)
ARMS_W = (240, 238, 230)
ARMS_B = (20, 50, 170)

FLAG_ARMS = [
    # 20 colunas x 20 linhas (de cima para baixo); g/r = pano, Y = esfera,
    # k = contorno escuro, R = bordadura, W = branco, B = quina azul
    "ggggggggrrrrrrrrrrrr",
    "ggggggggrrrrrrrrrrrr",
    "ggggggkkkkkkrrrrrrrr",
    "ggggkkYYYYYYkkrrrrrr",
    "gggkYYkkkkkkYYkrrrrr",
    "ggkYkkRRRRRRkkYkrrrr",
    "ggkYkRRYRYRYRkYkrrrr",
    "gkYYYRWWWBWWRYYYkrrr",
    "gkYkYRWWWWWWRYkYkrrr",
    "gkYkYYWBWBWBRYkYkrrr",
    "gkYYYRWWWWWWRYYYkrrr",
    "gkYkkRWWWBWWRkkYkrrr",
    "ggkYkRYWWWWYRkYkrrrr",
    "ggkYkkRRWWRRkkYkrrrr",
    "gggkYYkkRRkkYYkrrrrr",
    "ggggkkYYYYYYkkrrrrrr",
    "ggggggkkkkkkrrrrrrrr",
    "ggggggggrrrrrrrrrrrr",
    "ggggggggrrrrrrrrrrrr",
    "ggggggggrrrrrrrrrrrr",
]


def flag():
    """Bandeira verde e vermelha num mastro, com as armas (esfera armilar
    com contorno escuro e escudo branco de bordadura vermelha e quinas)."""
    m = Model()
    cyl_z(m, 0, 0, 3.5, 0, 2, STONE_D)
    cyl_z(m, 0, 0, 0.6, 2, 50, (90, 70, 40))
    ellipsoid(m, 0, 0, 50.5, 1.2, 1.2, 1.2, GOLD)
    cols = {"g": GREEN, "r": RED, "Y": ARMS_Y, "k": ARMS_K, "R": ARMS_R,
            "W": ARMS_W, "B": ARMS_B}
    for x in range(0, 20):
        wave = math.sin(x * 0.45) * 1.2
        for z in range(28, 48):
            ch = FLAG_ARMS[47 - z][x]
            m.put(x + 1, wave, z, cols[ch])
    return m


def pillar():
    m = Model()
    box(m, -6, 6, -6, 6, 0, 3, STONE_D)
    cyl_z(m, 0, 0, 4.2, 3, 54, STONE)
    for a in range(0, 360, 45):
        x, y = math.cos(math.radians(a)) * 4.1, math.sin(math.radians(a)) * 4.1
        box(m, x - 0.3, x + 0.3, y - 0.3, y + 0.3, 5, 52, STONE_D)
    box(m, -6, 6, -6, 6, 54, 58, STONE_D)
    return m


def table(chairs=False):
    m = Model()
    box(m, -11, 11, -7, 7, 13, 15, WOOD)
    box(m, -11.2, 11.2, -7.2, 7.2, 13, 13.6, WOOD_D)
    for x in (-10, 10):
        for y in (-6, 6):
            box(m, x - 0.8, x + 0.8, y - 0.8, y + 0.8, 0, 13, WOOD_D)
    if chairs:
        for x in (-16, 16):
            box(m, x - 3, x + 3, -3, 3, 8, 9.5, WOOD)
            for dx in (-2.4, 2.4):
                for dy in (-2.4, 2.4):
                    box(m, x + dx - 0.5, x + dx + 0.5, dy - 0.5, dy + 0.5, 0, 8, WOOD_D)
            bx = x + (2.6 if x > 0 else -2.6)
            box(m, bx - 0.5, bx + 0.5, -3, 3, 9.5, 19, WOOD)
        # candeeiro de mesa e papéis
        box(m, -6, -1, -3, 2, 15, 15.4, PAPER)
        cyl_z(m, 5, 0, 1.5, 15, 16, IRON)
        cyl_z(m, 5, 0, 0.4, 16, 20, IRON)
        cyl_z(m, 5, 0, 2.2, 20, 22, (40, 100, 60))
    return m


def well(water):
    m = Model()
    for z in range(0, 12):
        for a in range(0, 360, 4):
            for rr in (7.5, 8.5):
                x, y = math.cos(math.radians(a)) * rr, math.sin(math.radians(a)) * rr
                c = STONE if (z // 3 + a // 30) % 2 else STONE_D
                m.put(x, y, z, c)
    if water:
        cyl_z(m, 0, 0, 7, 9, 9.5, (50, 90, 150))
    for s in (-1, 1):
        box(m, s * 8.5 - 0.8, s * 8.5 + 0.8, -0.8, 0.8, 12, 28, WOOD)
    box(m, -9.5, 9.5, -0.8, 0.8, 27, 28.5, WOOD_D)
    cyl_z(m, 0, 0, 0.3, 18, 27, (140, 120, 90))
    box(m, -1.8, 1.8, -1.8, 1.8, 15, 18, IRON_L)
    return m


# ------------------------------------------------------------------ píxeis à mão (64x64)

CRAVO = [
    "....rRr.rR......",
    "...rRRrrRRr.....",
    "..rRrRRrRrRr....",
    ".rRRrrRRRrRRr...",
    ".rRrRRrRrRRrr...",
    "..rrRrRRrRrr....",
    "...rrrRRrrr.....",
    "....gGGGg.......",
    ".....gGg........",
    "......Gg........",
    "......Gg..gl....",
    "......gG.gl.....",
    "......Gggl......",
    "......Gg........",
    ".....gG.........",
    ".....Gg.........",
    ".....gG.........",
    "....gG..........",
    "....Gg..........",
    "....g...........",
]
CRAVO_C = {"r": (150, 10, 20), "R": (226, 30, 40), "g": (30, 90, 40), "G": (70, 150, 60), "l": (90, 170, 70)}


def sprite_canvas():
    return np.zeros((64, 64, 4), np.uint8)


def shadow(img, cx, cy, rx, ry=1.6):
    for y in range(64):
        for x in range(64):
            if ((x - cx) / rx) ** 2 + ((y - cy) / ry) ** 2 <= 1 and img[y, x, 3] == 0:
                img[y, x] = (18, 16, 16, 255)
    return img


def cravo():
    img = sprite_canvas()
    g = np.repeat(np.repeat(grid(CRAVO, CRAVO_C), 2, axis=0), 2, axis=1)
    paste(img, g, 18, 22)
    shadow(img, 28, 62, 8)
    return img


def avante():
    """Maço de jornais clandestinos Avante! atados com cordel."""
    img = sprite_canvas()
    x0, x1, y0 = 18, 46, 46
    for i in range(4):
        y = y0 + i * 3
        img[y : y + 3, x0 - i % 2 : x1 - i % 2] = (*PAPER, 255)
        img[y + 2, x0 - i % 2 : x1 - i % 2] = (170, 160, 130, 255)
    img[y0 - 12 : y0, x0 + 2 : x1 + 2] = (*PAPER, 255)  # o de cima, inclinado
    img[y0 - 12 : y0 - 6, x0 + 3 : x1 + 1] = (200, 24, 28, 255)
    rgb = img[..., :3].copy()
    rgb = draw_text(rgb, "AVANTE!", x0 + 4, y0 - 11, 1, (250, 240, 220))
    img[..., :3] = rgb
    for y in range(y0 - 4, y0 - 1):
        img[y, x0 + 4 : x1, :3] = (90, 86, 80)
        img[y, x0 + 4 + (y % 2) * 6 : x0 + 8 + (y % 2) * 6, :3] = (*PAPER,)
    img[y0 - 5 : y0 + 12, 31:33] = (130, 100, 60, 255)  # cordel
    img[y0 + 4, x0 - 1 : x1 + 1] = (130, 100, 60, 255)
    shadow(img, 32, 59, 16)
    return img


def suitcase():
    """Mala aberta cheia de notas de escudos."""
    img = sprite_canvas()
    # tampa aberta
    img[30:44, 14:50] = (90, 56, 30, 255)
    img[31:43, 15:49] = (120, 76, 40, 255)
    img[32:42, 17:47] = (150, 40, 40, 255)  # forro
    # caixa
    img[46:58, 12:52] = (110, 68, 34, 255)
    img[46, 12:52] = (160, 110, 60, 255)
    img[57, 12:52] = (60, 36, 18, 255)
    img[50:52, 12:52] = (60, 40, 20, 255)
    # notas (verde e castanho)
    r = random.Random(3)
    for i in range(12):
        x, y = r.randint(14, 40), r.randint(42, 47)
        c = r.choice([(120, 150, 100), (150, 130, 90), (110, 140, 120)])
        img[y : y + 3, x : x + 9] = (*c, 255)
        img[y + 1, x + 3 : x + 6] = (70, 90, 60, 255)
    img[28:31, 28:36] = (200, 170, 80, 255)  # pega
    img[47:50, 30:34] = (220, 190, 90, 255)  # fecho
    rgb = img[..., :3].copy()
    rgb = draw_text(rgb, "ESC", 26, 35, 1, (240, 210, 120))
    img[..., :3] = rgb
    shadow(img, 32, 59, 22)
    return img


def food():
    """Prato com pão e sardinhas."""
    img = sprite_canvas()
    for y in range(50, 60):
        for x in range(12, 52):
            if ((x - 32) / 20) ** 2 + ((y - 55) / 5) ** 2 <= 1:
                img[y, x] = (230, 228, 220, 255) if ((x - 32) / 16) ** 2 + ((y - 55) / 3.6) ** 2 > 1 else (210, 206, 196, 255)
                if ((x - 32) / 20) ** 2 + ((y - 55) / 5) ** 2 > 0.85:
                    img[y, x] = (40, 70, 160, 255)
    # sardinhas
    for k, (y, x0) in enumerate(((51, 18), (54, 22), (49, 26))):
        for x in range(x0, x0 + 14):
            t = (x - x0) / 14
            h = 2 if 0.15 < t < 0.85 else 1
            img[y - h // 2 : y + h, x] = (160, 170, 180, 255) if t < 0.85 else (110, 120, 130, 255)
            img[y - 1, x] = (90, 100, 120, 255) if t < 0.85 else img[y - 1, x]
        img[y, x0 + 2] = (20, 20, 20, 255)
    # pão
    for y in range(44, 55):
        for x in range(34, 50):
            d = ((x - 42) / 8) ** 2 + ((y - 50) / 5.5) ** 2
            if d <= 1:
                img[y, x] = (190, 130, 60, 255) if d > 0.5 or y > 50 else (220, 170, 90, 255)
    img[47, 38:46] = (150, 96, 40, 255)
    return img


def medkit():
    img = sprite_canvas()
    img[42:60, 16:48] = (236, 232, 220, 255)
    img[42, 16:48] = (255, 255, 250, 255)
    img[59, 16:48] = (150, 146, 140, 255)
    img[42:60, 47] = (170, 166, 160, 255)
    img[45:57, 29:35] = (200, 20, 24, 255)
    img[48:54, 26:38] = (200, 20, 24, 255)
    img[39:42, 27:37] = (60, 60, 60, 255)
    img[40, 28:36] = (0, 0, 0, 0)
    return img


# ------------------------------------------------------------------ catálogo

DECOR = {
    "ammo": (ammo, {}),
    "statue": (statue, {}),
    "barrel": (barrel, {}),
    "plant-blue": (lambda: plant(AZ, tall=30, tiles=True), {}),
    "vase-blue": (vase, {}),
    "scrap": (files, {}),
    "pot-clay": (pot_clay, {}),
    "ceiling-light": (ceiling_light, {"hang": True}),
    "chandelier": (chandelier, {"hang": True}),
    "battery": (typewriter, {}),
    "plant-gold": (lambda: plant(BRASS, (80, 150, 60), (50, 110, 40), 34), {}),
    "barrel-toxic": (drum, {}),
    "lamp": (lamp, {}),
    "pillar": (pillar, {}),
    "broken-robot": (books, {}),
    "cables": (chains, {"hang": True}),
    "table-chairs": (lambda: table(True), {}),
    "table": (table, {}),
    "well-water": (lambda: well(True), {}),
    "well": (lambda: well(False), {}),
}

PIXEL = {
    "gem": cravo,
    "trophy": avante,
    "treasure": suitcase,
    "food": food,
    "medkit": medkit,
}


# ------------------------------------------------------------------ armas
# Modelos com o cano para -y. Na mão (primeira pessoa) vêem-se de trás.

STEEL = (52, 54, 60)
STEEL_L = (104, 108, 116)
GRIP = (96, 58, 30)
SLEEVE = (96, 70, 44)  # casaco de bombazina castanho
SLEEVE_D = (70, 50, 32)


def pistol():
    """Pistola semiautomática ao estilo Walther dos anos 40-60."""
    m = Model()
    box(m, -1.6, 1.6, -15, 3, 1.5, 4.5, STEEL)  # corrediça
    box(m, -1.7, 1.7, -15, 3, 3.6, 4.5, STEEL_L)
    for y in (0, 1.2):
        box(m, -1.8, 1.8, y, y + 0.5, 2, 4.4, (24, 24, 26))  # estrias
    box(m, -0.9, 0.9, -8, -5, 4.4, 4.7, (24, 24, 26))  # janela de ejeção
    box(m, -1.0, 1.0, -16.5, -15, 2.4, 3.6, (20, 20, 22))  # boca
    box(m, -0.4, 0.4, -14.5, -13.5, 4.5, 5.3, STEEL_L)  # mira
    box(m, -0.5, 0.5, 2, 3, 4.5, 5.5, STEEL_L)
    box(m, -1.6, 1.6, -0.5, 3.5, -6, 1.5, GRIP)  # punho
    box(m, -1.7, 1.7, -2, 0, -1.5, 1.5, STEEL)  # guarda-mato
    return m


def fbp():
    """Pistola-metralhadora portuguesa de cano à vista e coronha de arame."""
    m = Model()
    cyl_y(m, 0, 3, 2.0, -10, 8, STEEL)  # caixa tubular
    cyl_y(m, 0, 3, 0.9, -20, -10, STEEL_L)  # cano
    cyl_y(m, 0, 3, 1.3, -21, -20, STEEL)
    box(m, -0.8, 0.8, -6, -3, -7, 1, STEEL)  # carregador
    box(m, -1.4, 1.4, 1, 4, -6, 1.5, GRIP)  # punho
    box(m, -0.4, 0.4, -17, -16, 4, 5.4, STEEL_L)  # mira
    for s in (-1.6, 1.6):
        box(m, s - 0.3, s + 0.3, 8, 18, 2.2, 2.8, STEEL_L)  # coronha de arame
    box(m, -1.8, 1.8, 17, 18.5, 0, 3, STEEL_L)
    return m


def mg():
    """Metralhadora de fita: caixa grossa, cano com manga furada e fita."""
    m = Model()
    box(m, -3.2, 3.2, -8, 10, 0, 6, STEEL)
    box(m, -3.3, 3.3, -8, 10, 5, 6, STEEL_L)
    cyl_y(m, 0, 3, 2.4, -24, -8, (40, 42, 46))
    for y in range(-23, -9, 3):
        for a in range(0, 360, 60):
            x, z = math.cos(math.radians(a)) * 2.4, 3 + math.sin(math.radians(a)) * 2.4
            box(m, x - 0.4, x + 0.4, y - 0.4, y + 0.4, z - 0.4, z + 0.4, (14, 14, 16))
    cyl_y(m, 0, 3, 1.0, -27, -24, STEEL_L)
    box(m, -1.6, 1.6, 4, 8, -7, 0, GRIP)
    box(m, -0.6, 0.6, -4, 4, 6, 8, STEEL_L)  # pega de transporte
    # fita de munições a sair da esquerda
    for i in range(7):
        y = -4 + i * 1.6
        box(m, -10 + i * 0.6, -3.3, y - 0.6, y + 0.6, 2, 3.2, BRASS)
    return m


def knife():
    m = Model()
    box(m, -1.0, 1.0, -1.2, 1.2, 0, 7, (80, 50, 26))  # cabo
    for z in (1.5, 3.5, 5.5):
        box(m, -1.1, 1.1, -1.3, 1.3, z, z + 0.4, BRASS)
    box(m, -2.6, 2.6, -1.4, 1.4, 7, 8, STEEL_L)
    for z in range(8, 22):
        w = 1.3 if z < 19 else 1.3 - (z - 18) * 0.38
        box(m, -w, w, -0.4, 0.4, z, z + 1, (176, 180, 188) if z % 3 else (214, 218, 224))
    return m


def hand_model(gun, recoil=0.0, flash=False, lift=0.0, two=False):
    """Arma + mão e manga do casaco vistas de trás."""
    m = Model()
    g = transformed(gun, rot_x((0, 0, 0), -recoil * 6))
    for p, c in g.v.items():
        m.put(p[0], p[1] + recoil, p[2] + lift, c)
    box(m, -2.6, 2.6, -0.5, 5, -7 + lift, -1 + lift, SKIN)  # mão
    box(m, -2.8, 2.8, 0, 3, -2 + lift, 1.5 + lift, SKIN_D)  # dedos
    box(m, -3.2, 3.2, 4, 20, -16 + lift, -5 + lift, SLEEVE)
    box(m, -3.3, 3.3, 4, 6, -16 + lift, -5 + lift, SLEEVE_D)  # punho da manga
    if two:
        box(m, -9, -4, -10, -4, -2 + lift, 2 + lift, SKIN)  # mão de apoio
        box(m, -12, -6, -4, 14, -12 + lift, -2 + lift, SLEEVE)
    if flash:
        ys = min(p[1] for p in gun.v)
        ellipsoid(m, 0, ys - 3, 3 + lift, 4.5, 2.5, 4.5, (255, 150, 40))
        ellipsoid(m, 0, ys - 4, 3 + lift, 2.5, 1.5, 2.5, (255, 236, 120))
    return m


def knife_swing(stage):
    ang = (0, -35, -75, -45, -10)[stage]
    side = (0, 2, 4, 2, 0)[stage]
    m = Model()
    b = transformed(knife(), rot_x((0, 0, 0), ang))
    for p, c in b.v.items():
        m.put(p[0] + side, p[1], p[2], c)
    box(m, -2.6 + side, 2.6 + side, -2, 2.5, -3, 3, SKIN)
    box(m, -3.2 + side, 3.2 + side, 0, 14, -16, -3, SLEEVE)
    box(m, -3.3 + side, 3.3 + side, 0, 2, -16, -3, SLEEVE_D)
    return m
