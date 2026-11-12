# Arte dos puzzles (portas trancadas, portões de alavanca, chaves e
# documentos): paredes 64x64, objetos no chão (64x64, gravados a 128 com
# píxeis duplicados) e ícones 16x16 da barra de estado (gravados a 32).
# Tudo desenhado aqui, como o resto (walls.py, decor.py, hud.py).
import math

import numpy as np

from pixart import grid
from pixfont import centered, draw_text
import walls as wl

S = 64


# ------------------------------------------------------------------ paredes


def locked_door(label, paint, plate=(222, 172, 52)):
    """Porta de aço reforçada com chapa de fechadura dourada e o nome
    pintado à pistola (ARSENAL, PAIOL). Lê-se de longe que é diferente."""
    base = np.array(paint, float)
    img = np.zeros((S, S, 3)) + base
    img += wl.noise(41, 3)[..., None]
    img[0:2] = base * 0.5
    img[62:] = base * 0.5
    # duas faixas diagonais amarelas e pretas em cima e em baixo
    for y0 in (4, 52):
        for y in range(y0, y0 + 8):
            for x in range(3, 61):
                img[y, x] = (214, 170, 30) if ((x + y) // 4) % 2 else (24, 22, 20)
        img[y0 - 1, 3:61] = base * 1.4
        img[y0 + 8, 3:61] = base * 0.5
    # rebites
    for x in range(5, 60, 7):
        for y in (14, 49):
            img[y, x] = base * 1.7
            img[y + 1, x] = base * 0.4
    # nome a stencil, com sombra
    img = centered(img, label, 32, 20, 2 if len(label) <= 5 else 1, (236, 230, 210), shadow=(20, 18, 16))
    # chapa da fechadura com o buraco da chave
    px0, py0 = 26, 31
    img[py0 : py0 + 14, px0 : px0 + 12] = plate
    img[py0, px0 : px0 + 12] = (255, 230, 140)
    img[py0 + 13, px0 : px0 + 12] = (120, 84, 20)
    img[py0 : py0 + 14, px0 + 11] = (150, 104, 26)
    for y in range(py0 + 3, py0 + 7):
        for x in range(px0 + 4, px0 + 8):
            if math.hypot(x - (px0 + 5.5), y - (py0 + 4.5)) < 2.1:
                img[y, x] = (16, 12, 10)
    img[py0 + 6 : py0 + 11, px0 + 5 : px0 + 7] = (16, 12, 10)
    return np.clip(img, 0, 255)


def gate():
    """Portão de grade de ferro, com o corredor escuro por trás e uma placa
    que diz que abre por alavanca."""
    img = np.zeros((S, S, 3)) + np.array((22, 20, 22), float)
    for y in range(S):
        img[y] *= 0.8 + 0.4 * (y / S)
    for x in range(3, 62, 7):
        img[:, x : x + 3] = (74, 76, 82)
        img[:, x] = (130, 134, 142)
        img[:, x + 2] = (40, 40, 46)
    for y in (6, 30, 56):
        img[y : y + 4, :] = (66, 68, 74)
        img[y, :] = (124, 128, 136)
        img[y + 3, :] = (34, 34, 40)
        for x in range(4, 62, 7):
            img[y + 1 : y + 3, x : x + 2] = (150, 152, 160)
    # pontas de lança em baixo
    for x in range(3, 62, 7):
        img[60:64, x : x + 3] = (50, 50, 56)
    # placa ao centro
    img[36:47, 14:50] = (150, 32, 30)
    img[36, 14:50] = (210, 80, 70)
    img[46, 14:50] = (80, 14, 14)
    img = centered(img, "ALAVANCA", 32, 39, 1, (240, 230, 200))
    return np.clip(img + wl.noise(43, 2)[..., None], 0, 255)


def lever_gate(down):
    return wl.lever(wl.stone(44), down, label="PORTÃO")


def walls():
    return {
        "door-arsenal": locked_door("ARSENAL", (70, 86, 64)),
        "door-paiol": locked_door("PAIOL", (110, 46, 40)),
        "gate": gate(),
        "lever-gate-down": lever_gate(True),
        "lever-gate-up": lever_gate(False),
    }


# ------------------------------------------------------------------ objetos no chão


def canvas():
    return np.zeros((64, 64, 4), np.uint8)


def shadow(img, cx, cy, rx, ry=1.6):
    for y in range(64):
        for x in range(64):
            if ((x - cx) / rx) ** 2 + ((y - cy) / ry) ** 2 <= 1 and img[y, x, 3] == 0:
                img[y, x] = (18, 16, 16, 255)
    return img


def floor_key(metal, metal_d, metal_l, tag):
    """Chave grande deitada no chão, com a etiqueta de cartão atada."""
    img = canvas()
    # argola
    for y in range(44, 62):
        for x in range(12, 30):
            d = math.hypot(x - 20.5, y - 52.5)
            if 4.2 <= d <= 7.6:
                img[y, x] = (*(metal if d < 6.6 else metal_d), 255)
                if d < 5.2 and y < 52:
                    img[y, x] = (*metal_l, 255)
    # haste
    img[51:55, 28:50] = (*metal, 255)
    img[51, 28:50] = (*metal_l, 255)
    img[54, 28:50] = (*metal_d, 255)
    # palhetão
    img[55:60, 42:45] = (*metal, 255)
    img[55:58, 46:49] = (*metal, 255)
    img[59, 42:45] = (*metal_d, 255)
    # etiqueta de cartão com fio
    for i in range(7):
        img[46 - i, 16 + i] = (200, 190, 160, 255)
    img[34:42, 22:34] = (*tag, 255)
    img[34, 22:34] = tuple(min(255, c + 50) for c in tag) + (255,)
    img[41, 22:34] = tuple(c // 2 for c in tag) + (255,)
    img[36:40, 24] = (40, 30, 20, 255)
    shadow(img, 32, 61, 20)
    return img


def floor_docs():
    """Pasta de cartão com o carimbo vermelho SECRETO e a fita atada."""
    img = canvas()
    img[42:60, 12:52] = (206, 176, 108, 255)
    img[42, 12:52] = (236, 214, 150, 255)
    img[59, 12:52] = (130, 104, 60, 255)
    img[42:60, 12] = (160, 130, 80, 255)
    img[44:58, 50:52] = (240, 236, 220, 255)  # folhas por baixo
    img[50:52, 12:52] = (150, 30, 30, 255)  # fita
    img[47:56, 30:34] = (150, 30, 30, 255)
    rgb = img[..., :3].copy()
    rgb = draw_text(rgb, "SECRETO", 16, 44, 1, (190, 20, 24))
    img[..., :3] = rgb
    shadow(img, 32, 61, 22)
    return img


def floor_charges():
    """Caixa de cargas de demolição: madeira verde-oliva, faixa amarela."""
    img = canvas()
    img[38:60, 12:52] = (86, 98, 54, 255)
    img[38, 12:52] = (128, 140, 88, 255)
    img[59, 12:52] = (46, 54, 28, 255)
    img[38:60, 12] = (62, 72, 38, 255)
    img[44:50, 12:52] = (214, 170, 30, 255)
    rgb = img[..., :3].copy()
    rgb = draw_text(rgb, "CARGAS", 21, 45, 1, (24, 22, 20))
    img[..., :3] = rgb
    # rastilho enrolado em cima
    for a in range(0, 360, 12):
        x = int(40 + 5 * math.cos(math.radians(a)))
        y = int(34 + 2.5 * math.sin(math.radians(a)))
        img[y, x] = (60, 50, 40, 255)
    shadow(img, 32, 61, 22)
    return img


GOLD = ((222, 172, 52), (150, 104, 26), (255, 226, 120))
IRON = ((150, 152, 160), (80, 82, 90), (210, 212, 220))

PIXEL = {
    "key-arsenal": lambda: floor_key(*GOLD, (196, 30, 30)),
    "key-paiol": lambda: floor_key(*IRON, (30, 110, 52)),
    "docs": floor_docs,
    "charges": floor_charges,
}


# ------------------------------------------------------------------ ícones da barra (16x16)

ICONS = {
    "key-arsenal": ([
        "................",
        "....ggg.........",
        "...gLLLg........",
        "..gL...Lg.......",
        "..gL...Lg.......",
        "..gL...Lg.......",
        "...gLLLgg.......",
        "....gggGg.......",
        ".......Gg.......",
        ".......Gg.......",
        ".......GgGG.....",
        ".......Gg.......",
        ".......GgGGG....",
        ".......Gg.......",
        ".......gg.......",
        "................",
    ], {"g": (150, 104, 26), "G": (222, 172, 52), "L": (255, 226, 120)}),
    "key-paiol": ([
        "................",
        "....ggg.........",
        "...gLLLg........",
        "..gL...Lg.......",
        "..gL...Lg.......",
        "..gL...Lg.......",
        "...gLLLgg.......",
        "....gggGg.......",
        ".......Gg.......",
        ".......Gg.......",
        ".......GgGG.....",
        ".......Gg.......",
        ".......GgGGG....",
        ".......Gg.......",
        ".......gg.......",
        "................",
    ], {"g": (80, 82, 90), "G": (150, 152, 160), "L": (210, 212, 220)}),
    "docs": ([
        "................",
        "..PPPPP.........",
        ".PYYYYYPPPPPPP..",
        ".PYYYYYYYYYYYYP.",
        ".PYYYYYYYYYYYYP.",
        ".PYRRRRRRRRRYYP.",
        ".PYRYYYYYYYRYYP.",
        ".PYRRRRRRRRRYYP.",
        ".PYYYYYYYYYYYYP.",
        ".PYYYYYYYYYYYYP.",
        ".PwwwwwwwwwwwwP.",
        ".PYYYYYYYYYYYYP.",
        ".PYYYYYYYYYYYYP.",
        ".PPPPPPPPPPPPPP.",
        "................",
        "................",
    ], {"P": (130, 104, 60), "Y": (214, 186, 120), "R": (190, 20, 24), "w": (150, 30, 30)}),
    "charges": ([
        "................",
        "........ff......",
        ".......f..f.....",
        "......f.........",
        "..RRRRRRRRRRRR..",
        "..RrRrRrRrRrRr..",
        "..RrRrRrRrRrRr..",
        "..YYYYYYYYYYYY..",
        "..YkYkYkYkYkYY..",
        "..YYYYYYYYYYYY..",
        "..RrRrRrRrRrRr..",
        "..RrRrRrRrRrRr..",
        "..RrRrRrRrRrRr..",
        "..RRRRRRRRRRRR..",
        "................",
        "................",
    ], {"R": (170, 30, 26), "r": (120, 16, 14), "Y": (214, 170, 30), "k": (24, 22, 20), "f": (70, 60, 50)}),
}


def icon(name):
    rows, cols = ICONS[name]
    return grid(rows, cols)
