# Objetos do cenário, pontos a apanhar e armas (tema: defesa do forte). Os
# móveis e objetos são voxels de baixa resolução; os pontos pequenos
# (medalha, cruz de guerra, caixa de ouro, ração com pão e chouriço, estojo)
# são píxeis desenhados à mão em 64x64, como nos jogos VGA. Tudo original.
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



def sandbags():
    """Trincheira de sacos de areia (onde os invasores entraram)."""
    m = Model()
    r = random.Random(11)
    for row, (z, n, off) in enumerate(((0, 4, 0.0), (3.2, 4, 2.5), (6.4, 3, 1.2), (9.6, 2, 3.6))):
        for i in range(n):
            x = -11 + off + i * 6.0
            c = (166, 146, 100) if (i + row) % 2 else (146, 128, 86)
            c = tuple(int(v * r.uniform(0.92, 1.06)) for v in c)
            ellipsoid(m, x, 0, z + 1.6, 3.0, 3.4, 1.8, c)
            box(m, x - 0.3, x + 0.3, -3.5, -3.2, z + 0.6, z + 2.6, (110, 96, 64))
    return m


def lantern():
    """Lanterna de campanha pendurada do teto."""
    m = Model()
    cyl_z(m, 0, 0, 0.4, 52, 60, IRON)
    box(m, -2.2, 2.2, -2.2, 2.2, 50.5, 51.5, (60, 80, 50))
    for x in (-2, 2):
        for y in (-2, 2):
            box(m, x - 0.3, x + 0.3, y - 0.3, y + 0.3, 45, 50.5, (60, 80, 50))
    cyl_z(m, 0, 0, 1.6, 45.5, 50, LAMP)
    box(m, -2.4, 2.4, -2.4, 2.4, 44.2, 45.2, (60, 80, 50))
    return m


def sprite_canvas():
    return np.zeros((64, 64, 4), np.uint8)


def shadow(img, cx, cy, rx, ry=1.6):
    for y in range(64):
        for x in range(64):
            if ((x - cx) / rx) ** 2 + ((y - cy) / ry) ** 2 <= 1 and img[y, x, 3] == 0:
                img[y, x] = (18, 16, 16, 255)
    return img


RIBBON = [(0, 110, 0), (0, 110, 0), (220, 20, 20), (220, 20, 20), (220, 20, 20)]


def _ribbon(img, x0, y0, w, h):
    for x in range(w):
        c = RIBBON[int(x / w * len(RIBBON))]
        img[y0 : y0 + h, x0 + x] = (*c, 255)
    img[y0, x0 : x0 + w] = (250, 220, 120, 255)
    img[y0 + h - 1, x0 : x0 + w] = (130, 20, 20, 255)


def medal():
    """Medalha militar (pontos): fita verde e vermelha e disco de bronze."""
    img = sprite_canvas()
    _ribbon(img, 25, 34, 14, 12)
    img[46:48, 30:34] = (*GOLD_D, 255)
    for y in range(48, 62):
        for x in range(22, 42):
            d = math.hypot(x - 31.5, y - 54.5)
            if d <= 7.2:
                img[y, x] = (205, 140, 70, 255) if d < 5.5 else (150, 96, 44, 255)
                if d < 3.5 and abs(x - 31.5) + abs(y - 54.5) < 3.6:
                    img[y, x] = (250, 210, 140, 255)
    img[50, 28:31] = (255, 236, 190, 255)
    shadow(img, 32, 62, 7)
    return img


def war_cross():
    """Cruz de guerra dourada sobre uma almofada verde (mais pontos)."""
    img = sprite_canvas()
    img[50:60, 16:48] = (30, 90, 40, 255)
    img[50, 16:48] = (70, 140, 80, 255)
    img[59, 16:48] = (16, 50, 22, 255)
    for y in range(28, 54):
        for x in range(18, 46):
            dx, dy = abs(x - 31.5), abs(y - 40.5)
            if max(dx, dy) > 12:
                continue
            if dy <= 2.5 + dx * 0.45 and dx <= 12 or dx <= 2.5 + dy * 0.45 and dy <= 12:
                img[y, x] = (*GOLD, 255)
                if dx < 1 or dy < 1:
                    img[y, x] = (255, 236, 140, 255)
    for y in range(36, 46):
        for x in range(27, 37):
            if math.hypot(x - 31.5, y - 40.5) <= 4.2:
                img[y, x] = (200, 20, 24, 255)
    img[39:42, 30:33] = (255, 255, 240, 255)
    shadow(img, 32, 62, 18)
    return img


def gold_crate():
    """Caixa de madeira aberta com barras de ouro (muitos pontos)."""
    img = sprite_canvas()
    img[42:60, 10:54] = (110, 70, 36, 255)
    img[42, 10:54] = (160, 110, 60, 255)
    img[59, 10:54] = (60, 36, 18, 255)
    img[48:50, 10:54] = (80, 50, 24, 255)
    img[54:56, 10:54] = (80, 50, 24, 255)
    for i, (x, y) in enumerate(((14, 37), (24, 37), (34, 37), (44, 37), (19, 32), (29, 32), (39, 32))):
        if x + 9 > 54:
            continue
        img[y : y + 5, x : x + 9] = (*GOLD, 255)
        img[y, x + 1 : x + 8] = (255, 236, 140, 255)
        img[y + 4, x : x + 9] = (*GOLD_D, 255)
    rgb = img[..., :3].copy()
    rgb = draw_text(rgb, "OURO", 24, 50, 1, (240, 210, 120))
    img[..., :3] = rgb
    shadow(img, 32, 61, 24)
    return img


def food():
    """Ração de combate (lata verde-oliva) e pão com chouriço."""
    img = sprite_canvas()
    # lata
    img[40:58, 10:30] = (86, 98, 54, 255)
    img[40, 10:30] = (130, 142, 90, 255)
    img[57, 10:30] = (50, 58, 30, 255)
    img[40:58, 10] = (60, 70, 36, 255)
    rgb = img[..., :3].copy()
    rgb = draw_text(rgb, "RAÇÃO", 11, 46, 1, (230, 226, 190))
    img[..., :3] = rgb
    # pão com chouriço
    for y in range(44, 60):
        for x in range(32, 56):
            d = ((x - 44) / 11) ** 2 + ((y - 52) / 7) ** 2
            if d <= 1:
                img[y, x] = (200, 140, 66, 255) if d > 0.45 or y > 52 else (226, 176, 96, 255)
    for (x, y) in ((38, 51), (42, 49), (46, 51), (50, 49), (40, 55), (48, 55)):
        img[y : y + 3, x : x + 3] = (160, 30, 30, 255)
        img[y, x] = (210, 90, 70, 255)
    shadow(img, 32, 61, 24)
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
    "broken-robot": (sandbags, {}),
    "cables": (lantern, {"hang": True}),
    "table-chairs": (lambda: table(True), {}),
    "table": (table, {}),
    "well-water": (lambda: well(True), {}),
    "well": (lambda: well(False), {}),
}

PIXEL = {
    "gem": medal,
    "trophy": war_cross,
    "treasure": gold_crate,
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


def g3():
    """Espingarda automática G3 (ícone): coronha, caixa, carregador reto,
    guarda-mão verde, tubo de armar e túnel do ponto de mira."""
    m = Model()
    box(m, -1.6, 1.6, 6, 16, -1, 3.5, (40, 42, 46))  # coronha
    box(m, -1.7, 1.7, -6, 6, 0, 4.5, STEEL)  # caixa
    box(m, -1.0, 1.0, -3, 1, -7, 0, (50, 52, 56))  # carregador
    box(m, -1.2, 1.2, 3, 6, -5, 0, (40, 42, 46))  # punho
    box(m, -1.9, 1.9, -16, -6, 0.5, 4.0, (64, 74, 48))  # guarda-mão
    cyl_y(m, 0, 2.2, 0.6, -24, -16, STEEL)  # cano
    cyl_y(m, 0, 4.6, 0.7, -16, 0, STEEL_L)  # tubo de armar
    box(m, -0.8, 0.8, -23, -21, 2.6, 6, STEEL_L)  # túnel do ponto de mira
    box(m, -0.9, 0.9, 2, 4, 4.5, 6.4, STEEL_L)  # alça
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
