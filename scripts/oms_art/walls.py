# Paredes 64x64 (quartel-general e forte a defender): pedra cinzenta de
# castelo, azulejo, madeira, tijolo, bandeiras, retratos e cartazes da época,
# e as marcas da invasão (rombo na parede com o estandarte vermelho dos
# invasores). Tudo desenhado por código.
import math
import random

import numpy as np
from PIL import Image, ImageDraw

from pixart import grid, paste
from pixfont import centered, draw_text, text_width

S = 64

# cores base
GREY = (124, 122, 118)
GOLD = (222, 172, 52)
GOLD_D = (150, 104, 26)
GOLD_L = (255, 226, 120)
GREEN = (26, 110, 52)
GREEN_D = (12, 66, 30)
RED = (186, 24, 26)
RED_D = (112, 10, 14)
WHITE = (238, 236, 226)
BLACK = (14, 12, 12)
AZ = (36, 74, 168)  # azul de azulejo
AZ_D = (20, 40, 110)
AZ_W = (236, 238, 232)
PAPER = (226, 214, 176)
PAPER_D = (176, 160, 120)
INK = (40, 36, 34)


def noise(seed, scale=1.0, shape=(S, S)):
    return np.random.default_rng(seed).normal(0, scale, shape)


def rgb(c):
    return np.array(c, float)


# ------------------------------------------------------------------ materiais


def stone(seed, base=GREY, rows=(9, 11, 10, 12, 10, 12), jitter=0.13):
    """Blocos de pedra irregulares, aresta clara em cima/esquerda e escura
    em baixo/direita, com textura granulada dentro de cada bloco."""
    rng = random.Random(seed)
    base = rgb(base)
    img = np.zeros((S, S, 3)) + base * 0.34
    y = 0
    gn = noise(seed + 7, 1.0)
    for h in rows:
        x = -rng.randint(0, 18)
        while x < S:
            w = rng.randint(13, 24)
            tone = base * rng.uniform(1 - jitter, 1 + jitter)
            tint = np.array([rng.uniform(-6, 6), rng.uniform(-4, 4), rng.uniform(-6, 4)])
            for yy in range(y + 1, min(S, y + h)):
                for xx in range(x + 1, x + w):
                    if not 0 <= xx < S:
                        continue
                    c = tone + tint + gn[yy, xx] * 7
                    if yy == y + 1 or xx == x + 1:
                        c = c * 1.25
                    elif yy == y + h - 1 or xx == x + w - 1:
                        c = c * 0.66
                    img[yy, xx] = c
            # lascas
            for _ in range(rng.randint(0, 2)):
                cx, cy = rng.randint(x + 3, x + w - 3), rng.randint(y + 2, y + h - 2)
                if 0 <= cx < S - 1 and cy < S - 1:
                    img[cy, cx] *= 0.6
                    img[cy, cx + 1] *= 0.8
            x += w
        y += h
        if y >= S:
            break
    return np.clip(img, 0, 255)


def bricks(seed, base=(132, 58, 40), bh=8, bw=16):
    rng = random.Random(seed)
    base = rgb(base)
    img = np.zeros((S, S, 3)) + rgb((70, 64, 58))
    gn = noise(seed, 1)
    for row in range(S // bh):
        off = (bw // 2) * (row % 2)
        for col in range(-1, S // bw + 1):
            x0 = col * bw + off
            tone = base * rng.uniform(0.8, 1.12)
            for yy in range(row * bh + 1, row * bh + bh):
                for xx in range(x0 + 1, x0 + bw):
                    if not 0 <= xx < S:
                        continue
                    c = tone + gn[yy, xx] * 6
                    if yy == row * bh + 1:
                        c = c * 1.2
                    elif yy == row * bh + bh - 1:
                        c = c * 0.7
                    img[yy, xx] = c
    # manchas de humidade em baixo
    for y in range(40, S):
        img[y] *= 1 - (y - 40) / 90
    return np.clip(img, 0, 255)


def wood(seed, base=(116, 70, 36)):
    """Lambrim de madeira: tábuas verticais com veio e moldura de painel."""
    base = rgb(base)
    rng = np.random.default_rng(seed)
    img = np.zeros((S, S, 3))
    for x in range(S):
        plank = x // 8
        ph = rng.uniform(0, 6) if x % 8 == 0 else ph  # noqa: F821
        k = 0.9 + 0.08 * ((plank * 37) % 5) / 4
        for y in range(S):
            g = math.sin(y * 0.33 + ph + math.sin(x * 0.7) * 0.8)
            img[y, x] = base * (k + 0.1 * g)
        if x % 8 == 0:
            img[:, x] = base * 0.5
        elif x % 8 == 7:
            img[:, x] = base * 0.76
    img += noise(seed, 3)[..., None]
    # moldura do lambrim
    for y0 in (0, 62):
        img[y0 : y0 + 2] = base * 0.45
    img[30:34] = base * 1.18
    img[30] = base * 1.4
    img[33] = base * 0.5
    return np.clip(img, 0, 255)


def rock(seed):
    """Rocha bruta do forte, junto ao mar."""
    rng = random.Random(seed)
    img = np.zeros((S, S, 3)) + rgb((58, 54, 48))
    for _ in range(30):
        cx, cy, r = rng.uniform(0, S), rng.uniform(0, S), rng.uniform(5, 12)
        tone = rgb((112, 104, 90)) * rng.uniform(0.75, 1.15)
        for y in range(S):
            for x in range(S):
                for ox in (-S, 0, S):
                    for oy in (-S, 0, S):
                        d = math.hypot(x - cx - ox, (y - cy - oy) * 1.3) / r
                        if d < 1:
                            img[y, x] = tone * (1.2 - 0.5 * d) if (x - cx - ox) + (y - cy - oy) < 0 else tone * (1.0 - 0.45 * d)
    img += noise(seed, 5)[..., None]
    # salitre esverdeado em baixo
    for y in range(44, S):
        img[y] = img[y] * 0.9 + rgb((30, 60, 50)) * 0.1 * (y - 44) / 20
    return np.clip(img, 0, 255)


def plaster(seed, base=(214, 206, 186)):
    img = np.zeros((S, S, 3)) + rgb(base)
    img += noise(seed, 5)[..., None]
    return np.clip(img, 0, 255)


# ------------------------------------------------------------------ azulejo


def tile_pattern(kind):
    """Um azulejo de 16x16 (4 azulejos por parede de 64)."""
    t = np.zeros((16, 16, 3)) + rgb(AZ_W)
    for y in range(16):
        for x in range(16):
            dx, dy = x - 7.5, y - 7.5
            r = math.hypot(dx, dy)
            if kind == 0:
                # flor de quatro pétalas ao centro + quartos de flor nos cantos
                a = math.atan2(dy, dx)
                petal = 4.6 + 1.8 * math.cos(4 * a)
                if r < petal:
                    t[y, x] = AZ if r > 1.6 else AZ_W
                if r < 1.0:
                    t[y, x] = AZ_D
                cr = min(math.hypot(x - cx, y - cy) for cx in (0, 15) for cy in (0, 15))
                if cr < 3.2:
                    t[y, x] = AZ
            else:
                # losango com cruz e pontos
                if abs(dx) + abs(dy) < 7.2 and abs(dx) + abs(dy) > 5.4:
                    t[y, x] = AZ
                if (abs(dx) < 0.8 and abs(dy) < 4) or (abs(dy) < 0.8 and abs(dx) < 4):
                    t[y, x] = AZ_D
                if min(abs(dx), abs(dy)) > 5.5:
                    t[y, x] = AZ
    t[0, :] = t[0, :] * 0.86
    t[:, 0] = t[:, 0] * 0.86
    return t


def azulejo(seed, kind=0, dado=True):
    """Parede de azulejo azul e branco; por cima um friso e pedra."""
    img = np.zeros((S, S, 3))
    tl = tile_pattern(kind)
    for ty in range(4):
        for tx in range(4):
            img[ty * 16 : ty * 16 + 16, tx * 16 : tx * 16 + 16] = tl
    img += noise(seed, 3)[..., None]
    if dado:
        # friso superior
        img[0:6] = rgb(AZ_D)
        img[2:4] = rgb(GOLD) * 0.9
        for x in range(0, S, 4):
            img[1, x : x + 2] = rgb(AZ_W)
            img[4, x + 2 : x + 4] = rgb(AZ_W)
        # rodapé
        img[60:] = rgb(AZ_D) * 0.8
    # vidrado: brilho em cada azulejo
    for ty in range(4):
        for tx in range(4):
            img[ty * 16 + 2, tx * 16 + 2 : tx * 16 + 5] = np.minimum(img[ty * 16 + 2, tx * 16 + 2 : tx * 16 + 5] * 1.08, 255)
    return np.clip(img, 0, 255)


def azulejo_panel():
    """Painel figurativo de azulejo: uma caravela no mar, em azul."""
    img = azulejo(60, 1, dado=True)
    x0, y0, x1, y1 = 6, 9, 58, 57
    img[y0:y1, x0:x1] = rgb(AZ_W)
    # moldura de volutas
    for x in range(x0, x1):
        for y in (y0, y0 + 1, y1 - 2, y1 - 1):
            img[y, x] = AZ_D if (x + y) % 4 < 2 else AZ
    for y in range(y0, y1):
        for x in (x0, x0 + 1, x1 - 2, x1 - 1):
            img[y, x] = AZ_D if (x + y) % 4 < 2 else AZ
    # céu com nuvens tracejadas
    for y in range(y0 + 3, y0 + 14, 3):
        for x in range(x0 + 4, x1 - 4):
            if (x * 3 + y * 5) % 11 < 3:
                img[y, x] = rgb(AZ) * 1.3
    # mar
    for y in range(44, y1 - 2):
        for x in range(x0 + 2, x1 - 2):
            if (x + (y % 3) * 2) % 6 < 3:
                img[y, x] = AZ
            else:
                img[y, x] = rgb(AZ) * 1.6
    # casco
    for y in range(38, 45):
        w = 16 - (y - 38) * 1.4
        for x in range(int(32 - w), int(32 + w)):
            img[y, x] = AZ_D
    img[37, 14:50] = AZ_D
    # mastros e velas com a cruz
    for mx, top, sw in ((24, 16, 6), (34, 13, 8), (44, 20, 5)):
        img[top:38, mx] = AZ_D
        for y in range(top + 2, top + 2 + sw * 2):
            ww = sw - abs(y - (top + 2 + sw)) * 0.25
            for x in range(int(mx - ww), int(mx + ww) + 1):
                img[y, x] = rgb(AZ) * 1.45
        cy = top + 2 + sw
        img[cy - 2 : cy + 3, mx] = AZ_D
        img[cy, mx - 2 : mx + 3] = AZ_D
    # flâmula
    img[12, 35:40] = AZ
    return np.clip(img, 0, 255)


# ------------------------------------------------------------------ emblemas


def armillary(img, cx, cy, r, col=GOLD, dark=GOLD_D):
    """Esfera armilar: anel, equador, eclíptica inclinada e meridiano."""
    img = np.asarray(img, float)
    H, W = img.shape[:2]

    def put(x, y, c):
        xi, yi = int(round(x)), int(round(y))
        if 0 <= xi < W and 0 <= yi < H:
            img[yi, xi] = c

    n = int(r * 12)
    for i in range(n):
        a = 2 * math.pi * i / n
        put(cx + math.cos(a) * r, cy + math.sin(a) * r, col)
        put(cx + math.cos(a) * (r - 1), cy + math.sin(a) * (r - 1), dark)
        # meridiano (elipse estreita)
        put(cx + math.cos(a) * r * 0.35, cy + math.sin(a) * r, col)
        # equador e trópicos
        for k, w in ((0.0, 1.0), (-0.45, 0.89), (0.45, 0.89)):
            put(cx + math.cos(a) * r * w, cy + k * r + math.sin(a) * r * 0.18, col if k == 0 else dark)
    # eclíptica (faixa larga inclinada)
    for i in range(n):
        a = 2 * math.pi * i / n
        ex, ey = math.cos(a) * r, math.sin(a) * r * 0.3
        rot = math.radians(-30)
        x = ex * math.cos(rot) - ey * math.sin(rot)
        y = ex * math.sin(rot) + ey * math.cos(rot)
        put(cx + x, cy + y, GOLD_L)
        put(cx + x, cy + y + 1, col)
    # eixo
    for t in range(-int(r) - 2, int(r) + 3):
        put(cx + t * 0.12, cy + t, dark)
    return img


def cross_of_christ(img, cx, cy, R, red=RED, white=WHITE):
    """Cruz da Ordem de Cristo: cruz pátea vermelha com cruz branca dentro."""
    img = np.asarray(img, float)
    H, W = img.shape[:2]
    for y in range(int(cy - R) - 1, int(cy + R) + 2):
        for x in range(int(cx - R) - 1, int(cx + R) + 2):
            if not (0 <= x < W and 0 <= y < H):
                continue
            dx, dy = abs(x - cx), abs(y - cy)
            if max(dx, dy) > R:
                continue
            f = lambda t: 0.7 + t * 0.5  # noqa: E731
            if dy <= f(dx) or dx <= f(dy):
                img[y, x] = red
                if (dx < 0.6 and dy < R - 1) or (dy < 0.6 and dx < R - 1):
                    img[y, x] = white
    return img


def shield_pt(img, cx, cy, w=7, h=8):
    """Escudo português muito simplificado (bordadura e quinas)."""
    for y in range(h):
        half = w if y < h - 3 else w - (y - (h - 3)) * 2
        for x in range(-half, half + 1):
            border = abs(x) >= half - 1 or y == 0 or y >= h - 2
            img[cy + y, cx + x] = RED if border else WHITE
    for dx, dy in ((0, 2), (-2, 4), (0, 4), (2, 4), (0, 6)):
        if dy < h - 1:
            img[cy + dy, cx + dx] = AZ
    return img


FLAG_GREEN = (0, 102, 0)
FLAG_RED = (255, 0, 0)
ARMS_GOLD = (240, 196, 40)
ARMS_GOLD_D = (170, 120, 10)
QUINA = (20, 50, 170)


def pt_arms(img, cx, cy, R):
    """Armas da bandeira nacional, legíveis: esfera armilar amarela (anel,
    faixa do horizonte e faixa diagonal) e por cima o escudo branco com
    bordadura vermelha, 7 castelos amarelos e 5 quinas azuis em cruz."""
    img = np.asarray(img, float)
    H, W = img.shape[:2]

    def put(x, y, c):
        xi, yi = int(math.floor(x)), int(math.floor(y))
        if 0 <= xi < W and 0 <= yi < H:
            img[yi, xi] = c

    def ellipse_line(rx, ry, rot, col, off=0.0):
        n = int(R * 30)
        for i in range(n):
            a = 2 * math.pi * i / n
            ex, ey = math.cos(a) * rx, math.sin(a) * ry + off
            x = ex * math.cos(rot) - ey * math.sin(rot)
            y = ex * math.sin(rot) + ey * math.cos(rot)
            put(cx + x, cy + y, col)

    t = max(1, int(round(R / 7)))
    for k in range(t + 1):  # anel exterior (com aresta escura por dentro)
        ellipse_line(R - k, R - k, 0, ARMS_GOLD if k < t else ARMS_GOLD_D)
    ellipse_line(R - 0.5, R * 0.2, 0, ARMS_GOLD)  # horizonte
    rot = math.radians(-30)
    ellipse_line(R - 0.5, R * 0.18, rot, ARMS_GOLD, -R * 0.08)  # faixa diagonal
    if R >= 10:
        ellipse_line(R - 0.5, R * 0.18, rot, ARMS_GOLD, R * 0.14)
    # escudo
    w = max(3, int(round(R * 0.6)))
    h = max(4, int(round(R * 0.72)))
    b = max(1, int(round(R / 7)))
    x0, x1 = int(cx) - w, int(cx) + w
    y0, y1 = int(cy) - h + 1, int(cy) + h
    ym = y0 + (y1 - y0) // 2

    def inside(x, y, m):
        if x < x0 + m or x > x1 - m or y < y0 + m or y > y1 - m:
            return False
        if y > ym:
            rx = (x1 - x0) / 2 - m + 0.5
            ry = (y1 - m) - ym + 0.5
            return ((x - (x0 + x1) / 2) / rx) ** 2 + ((y - ym) / ry) ** 2 <= 1
        return True

    for y in range(y0, y1 + 1):
        for x in range(x0, x1 + 1):
            if inside(x, y, 0):
                put(x, y, WHITE if inside(x, y, b) else FLAG_RED)
    # 7 castelos amarelos na bordadura (3 em cima, 2 de cada lado)
    yb = y0 + (y1 - y0) * 0.62
    for px, py in ((x0, y0), (int(cx) - b // 2 - (0 if b > 1 else 0), y0), (x1 - b + 1, y0),
                   (x0, int((y0 + yb) / 2)), (x1 - b + 1, int((y0 + yb) / 2)),
                   (x0 + (1 if R > 8 else 0), int(yb)), (x1 - b + 1 - (1 if R > 8 else 0), int(yb))):
        for dy in range(b):
            for dx in range(b):
                put(px + dx, py + dy, ARMS_GOLD)
    # 5 quinas azuis em cruz
    q = max(1, int(round(R / 7)))
    sp = q + 1
    qx, qy = int(cx) - q // 2, int(cy) - q // 2 + (1 if R >= 10 else 0)
    for ox, oy in ((0, 0), (0, -sp), (0, sp), (-sp, 0), (sp, 0)):
        for dy in range(q):
            for dx in range(q):
                put(qx + ox + dx, qy + oy + dy, QUINA)
    return img


def frame(img, x0, y0, x1, y1, col=GOLD, dark=GOLD_D):
    img[y0:y1, x0:x1] = dark
    img[y0 + 1 : y1 - 1, x0 + 1 : x1 - 1] = col
    img[y0 + 1, x0 + 1 : x1 - 1] = GOLD_L
    img[y0 + 1 : y1 - 1, x0 + 1] = GOLD_L
    img[y0 + 3 : y1 - 3, x0 + 3 : x1 - 3] = dark
    # sombra na parede
    img[y1 : y1 + 1, x0 + 2 : x1 + 1] *= 0.55
    img[y0 + 2 : y1 + 1, x1 : x1 + 1] *= 0.55
    return img


# ------------------------------------------------------------------ retrato

# Retrato estilizado de um ditador sério (cabelo grisalho penteado para trás,
# entradas, rosto magro, colarinho alto e fato escuro). Metade esquerda,
# espelhada. 11 colunas -> 22 de largura, 28 de altura.
PORTRAIT = [
    "bbbbbbbbbbb",
    "bbbbbbbbbbb",
    "bbbbbbhhhhh",
    "bbbbbhhHHhh",
    "bbbbhhHhSSS",
    "bbbbhhSSSSS",
    "bbbbhSSSSSS",
    "bbbbhSSSSSs",
    "bbbbdSSSSSs",
    "bbbbdsEEEss",
    "bbbsdsSeWsS",
    "bbbssssssss",
    "bbbbdsssssS",
    "bbbbdsssssS",
    "bbbbbdsssdS",
    "bbbbbdsssdd",
    "bbbbbbdsssd",
    "bbbbbbdssmm",
    "bbbbbbbdsss",
    "bbbbbbbbdds",
    "bbbbbbbbbdd",
    "bbbbbbbwwww",
    "bbbbbcwwwwt",
    "bbbccccwwct",
    "bbcccccCcct",
    "bccccccCcct",
    "cccccccCccc",
    "ccccccccccc",
]
PCOL = {
    "b": (44, 52, 40), "h": (128, 126, 120), "H": (190, 188, 182), "S": (226, 186, 150),
    "s": (190, 142, 106), "d": (120, 80, 58), "E": (60, 46, 40), "e": (30, 22, 20),
    "W": (240, 236, 230), "m": (110, 60, 52), "w": (236, 234, 228), "c": (22, 22, 28),
    "C": (60, 60, 72), "t": (40, 40, 52),
}


def portrait_img():
    return grid(PORTRAIT, PCOL, mirror=True)


def portrait(img, x0, y0):
    """Quadro dourado de 30x36 com o retrato."""
    img = frame(img, x0, y0, x0 + 30, y0 + 36)
    p = portrait_img()
    return paste(img, p, x0 + 4, y0 + 4)


# ------------------------------------------------------------------ bandeiras e cartazes


def rod(img, x0, x1, y):
    img[y : y + 2, x0:x1] = GOLD
    img[y + 2, x0:x1] = GOLD_D
    img[y - 1 : y + 3, x0 - 2 : x0] = GOLD_L
    img[y - 1 : y + 3, x1 : x1 + 2] = GOLD_L
    return img


def arms_outline(img, cx, cy, r):
    """Contorno escuro à volta da esfera armilar, para se destacar do pano."""
    n = int(r * 14)
    for i in range(n):
        a = 2 * math.pi * i / n
        for rr in (r + 1, r + 1.6):
            x, y = int(round(cx + math.cos(a) * rr)), int(round(cy + math.sin(a) * rr))
            if 0 <= y < img.shape[0] and 0 <= x < img.shape[1]:
                img[y, x] = (40, 26, 8)
    return img


def shield_clear(img, cx, cy, w=4, h=9):
    """Escudo legível: branco com bordadura vermelha, contorno escuro e 5
    quinas azuis em cruz (o topo em cy)."""
    for y in range(-1, h + 1):
        half = w + 1 if y < h - 3 else w + 1 - (y - (h - 3)) * 2
        for x in range(-half, half + 1):
            img[cy + y, cx + x] = (30, 20, 16)  # contorno
    for y in range(h):
        half = w if y < h - 3 else w - (y - (h - 3)) * 2
        for x in range(-half, half + 1):
            border = abs(x) >= half or y == 0 or y >= h - 1
            img[cy + y, cx + x] = RED if border else WHITE
    m = cy + h // 2 - 1
    for dx, dy in ((0, -2), (-2, 0), (0, 0), (2, 0), (0, 2)):
        img[m + dy, cx + dx] = QUINA
    for dx, dy in ((-w, 0), (0, 0), (w, 0), (-w, 3), (w, 3)):  # castelos
        img[cy + dy, cx + dx] = ARMS_GOLD
    return img


def flag_banner(img, x0, x1, y0, y1):
    """Estandarte vertical verde e vermelho com a esfera armilar e a Cruz de
    Cristo, franja dourada em baixo."""
    img = rod(img, x0 - 2, x1 + 2, y0 - 3)
    split = y0 + int((y1 - y0) * 0.4)
    for x in range(x0, x1):
        fold = 0.82 + 0.18 * math.cos((x - x0) / (x1 - x0) * math.pi * 3)
        for y in range(y0, y1):
            img[y, x] = rgb(GREEN if y < split else RED) * fold
    for x in range(x0, x1):
        depth = int(5 * (1 - abs((x - x0) / (x1 - x0) * 2 - 1)))
        for y in range(y1, y1 + depth):
            img[y, x] = rgb(RED) * 0.8
        if x % 2 == 0:
            img[y1 + depth : y1 + depth + 2, x] = GOLD
    img[y0:y1, x0] = rgb(GREEN_D)
    img[y0:y1, x1 - 1] = rgb(RED_D)
    cx = (x0 + x1) / 2
    img = cross_of_christ(img, cx, y0 + 5, 4)
    img = arms_outline(img, cx, split + 2, 10)
    img = armillary(img, cx, split + 2, 10, col=ARMS_GOLD)
    img = shield_clear(img, int(cx), split - 2, 4, 9)
    return img


def poster_licao(img):
    """Cartaz escolar de propaganda ao estilo 'A Lição de Salazar': a casa
    humilde com a cruz, o sol, e a trilogia."""
    x0, y0, x1, y1 = 13, 6, 51, 58
    img[y0 + 1 : y1 + 1, x0 + 1 : x1 + 1] *= 0.5
    img[y0:y1, x0:x1] = PAPER
    img[y0:y1, x0] = PAPER_D
    img[y0, x0:x1] = PAPER_D
    # faixa de título
    img[y0 + 2 : y0 + 10, x0 + 2 : x1 - 2] = GREEN
    img = centered(img, "A LIÇÃO", (x0 + x1) / 2, y0 + 4, 1, (250, 240, 200))
    # quadro: céu, sol, campo, casa com cruz
    qx0, qy0, qx1, qy1 = x0 + 3, y0 + 12, x1 - 3, y1 - 19
    for y in range(qy0, qy1):
        t = (y - qy0) / (qy1 - qy0)
        img[y, qx0:qx1] = (150 + 70 * t, 190 + 30 * t, 220 - 20 * t)
    for y in range(qy0, qy1):
        for x in range(qx0, qx1):
            if math.hypot(x - (qx1 - 7), y - (qy0 + 6)) < 3.5:
                img[y, x] = (255, 214, 90)
            if y > qy0 + 13 + 2 * math.sin(x * 0.3):
                img[y, x] = (90, 150, 60)
    hx, hy = qx0 + 8, qy0 + 10
    img[hy : hy + 8, hx : hx + 12] = WHITE
    for i in range(5):
        img[hy - 1 - i, hx + i : hx + 12 - i] = RED
    img[hy + 3 : hy + 8, hx + 5 : hx + 8] = (110, 70, 40)
    img[hy + 2, hx + 1 : hx + 3] = AZ
    img[hy + 2, hx + 9 : hx + 11] = AZ
    # cruz no alto do monte
    img[qy0 + 3 : qy0 + 11, qx0 + 24] = (80, 60, 40)
    img[qy0 + 5, qx0 + 22 : qx0 + 27] = (80, 60, 40)
    # trilogia
    img = centered(img, "DEUS", (x0 + x1) / 2, y1 - 18, 1, INK)
    img = centered(img, "PÁTRIA", (x0 + x1) / 2, y1 - 12, 1, (150, 20, 20))
    img = centered(img, "FAMÍLIA", (x0 + x1) / 2, y1 - 6, 1, GREEN_D)
    return img


def trilogia_plaque(img):
    x0, y0, x1, y1 = 14, 10, 50, 54
    img = frame(img, x0, y0, x1, y1)
    img[y0 + 3 : y1 - 3, x0 + 3 : x1 - 3] = (36, 40, 34)
    img = cross_of_christ(img, (x0 + x1) / 2 - 0.5, y0 + 10, 5)
    img = centered(img, "DEUS", (x0 + x1) / 2, y0 + 19, 1, GOLD_L, shadow=(10, 10, 10))
    img = centered(img, "PÁTRIA", (x0 + x1) / 2, y0 + 26, 1, GOLD_L, shadow=(10, 10, 10))
    img = centered(img, "FAMÍLIA", (x0 + x1) / 2, y0 + 33, 1, GOLD_L, shadow=(10, 10, 10))
    return img


def mocidade(img):
    """Emblema (só decoração) da Mocidade Portuguesa: escudo verde e
    vermelho com a Cruz de Cristo e MP; por baixo a fivela do cinto com o S."""
    cx = 32
    x0, x1, y0, y1 = 18, 46, 8, 42
    for y in range(y0, y1):
        t = (y - y0) / (y1 - y0)
        half = 14 if t < 0.6 else 14 - (t - 0.6) / 0.4 * 13
        for x in range(int(cx - half), int(cx + half)):
            edge = x <= cx - half + 1 or x >= cx + half - 2 or y <= y0 + 1
            img[y, x] = GOLD if edge else (GREEN if x < cx else RED)
    img = cross_of_christ(img, cx - 0.5, y0 + 10, 6)
    img = draw_text(img, "MP", cx - 4, y0 + 20, 1, GOLD_L, shadow=(20, 10, 0))
    # fivela S
    bx0, by0 = 24, 46
    img[by0 : by0 + 12, bx0 - 8 : bx0] = (70, 52, 30)
    img[by0 : by0 + 12, bx0 + 16 : bx0 + 24] = (70, 52, 30)
    img[by0 + 1 : by0 + 11, bx0 - 8 : bx0] = (96, 72, 42)
    img[by0 + 1 : by0 + 11, bx0 + 16 : bx0 + 24] = (96, 72, 42)
    img[by0 - 1 : by0 + 13, bx0 : bx0 + 16] = GOLD_D
    img[by0 : by0 + 12, bx0 + 1 : bx0 + 15] = GOLD
    img = draw_text(img, "S", bx0 + 4, by0 + 1, 2, (110, 70, 10))
    img[by0, bx0 + 1 : bx0 + 15] = GOLD_L
    return img


# ------------------------------------------------------------------ portas, grades, elevador


def arch(img):
    for y in range(S):
        for x in range(S):
            inside = (abs(x - 32) < 15 and y > 28) or (math.hypot(x - 32, y - 28) < 15)
            ring = (abs(x - 32) < 19 and y > 28) or (math.hypot(x - 32, y - 28) < 19)
            if inside:
                img[y, x] = (16, 14, 14)
            elif ring:
                img[y, x] = img[y, x] * 1.12
    # gradeamento de ferro no arco
    for x in range(19, 46, 5):
        for y in range(14, 64):
            if (abs(x - 32) < 15 and y > 28) or math.hypot(x - 32, y - 28) < 15:
                img[y, x] = (60, 60, 64)
    img[S - 3 :, 12:52] = (70, 66, 60)
    return cross_of_christ(img, 32, 9, 4)


def cell_bars(base, cot=False):
    img = base.copy()
    img[8:60, 7:57] = (18, 16, 16)
    # interior: parede caiada, tarimba, balde, riscos na parede
    img[10:44, 9:55] = (70, 66, 58)
    img[44:58, 9:55] = (40, 36, 32)
    if cot:
        img[40:46, 12:40] = (96, 74, 46)
        img[38:40, 12:40] = (150, 140, 110)
        img[46:52, 13:15] = (60, 46, 30)
        img[46:52, 37:39] = (60, 46, 30)
        img[50:56, 45:51] = (90, 92, 96)
        img[50, 45:51] = (140, 142, 146)
        for i, x in enumerate(range(16, 34, 2)):
            img[18:24, x] = (150, 144, 128)
            if i % 5 == 4:
                for k in range(8):
                    img[18 + k * 6 // 8, x - 8 + k] = (150, 144, 128)
    for x in range(9, 56, 6):
        img[8:60, x : x + 2] = (92, 94, 100)
        img[8:60, x] = (150, 152, 158)
    for y in (8, 9, 32, 33, 58, 59):
        img[y, 7:57] = (110, 112, 118)
    return img


def prison_door():
    """Porta de cela/serviço: aço pintado de verde-escuro, rebites e postigo."""
    base = rgb((64, 84, 70))
    img = np.zeros((S, S, 3)) + base
    img += noise(11, 3)[..., None]
    for y in range(S):
        img[y] *= 0.94 + 0.06 * math.sin(y * 0.5)
    img[0:2] = base * 0.5
    img[62:] = base * 0.5
    for (x0, y0, x1, y1) in ((5, 5, 59, 30), (5, 34, 59, 59)):
        img[y0:y1, x0:x1] *= 0.9
        img[y0, x0:x1] = base * 1.35
        img[y1 - 1, x0:x1] = base * 0.55
        img[y0:y1, x0] = base * 1.35
        img[y0:y1, x1 - 1] = base * 0.55
    # postigo com grades
    img[10:24, 22:42] = (14, 14, 14)
    for x in range(24, 42, 4):
        img[10:24, x] = (120, 126, 120)
    img[9, 21:43] = base * 1.5
    img[24, 21:43] = base * 0.4
    for x in range(3, 62, 6):
        for y in (3, 60):
            img[y, x] = base * 1.6
            img[y + 1, x] = base * 0.4
    # puxador
    img[40:46, 52:55] = (180, 170, 120)
    img[46, 52:55] = (90, 80, 50)
    return np.clip(img, 0, 255)


def door_frame():
    img = stone(21)
    img[:, 22:42] = (62, 60, 56)
    img[:, 22:24] = (120, 116, 108)
    img[:, 40:42] = (34, 32, 30)
    for y in range(6, S, 14):
        img[y : y + 4, 28:36] = (40, 38, 36)
        img[y, 28:36] = (90, 86, 80)
    return img


def lift_door():
    """Porta de elevador de grade de latão (anos 30)."""
    img = np.zeros((S, S, 3)) + rgb((26, 22, 18))
    for x in range(2, 62, 6):
        img[:, x : x + 2] = (170, 130, 50)
        img[:, x] = (220, 180, 80)
    for y in range(0, S, 8):
        for x in range(S):
            d = (x + y) % 12
            if d in (0, 1):
                img[y : y + 8, x] = np.maximum(img[y : y + 8, x], (130, 96, 36))
    img[:3] = GOLD_D
    img[61:] = GOLD_D
    img[:, 31:33] = (60, 44, 20)
    return np.clip(img + noise(5, 2)[..., None], 0, 255)


def lift_panel():
    img = wood(6)
    img[10:54, 18:46] = (150, 110, 40)
    img[11:53, 19:45] = (200, 160, 70)
    img[13:51, 21:43] = (46, 36, 24)
    img = centered(img, "PISO", 32, 15, 1, GOLD_L)
    for i, y in enumerate((24, 33, 42)):
        for yy in range(y, y + 6):
            for xx in range(28, 37):
                if math.hypot(xx - 32, yy - y - 2.5) < 3.5:
                    img[yy, xx] = (240, 210, 120) if i != 0 else (230, 60, 40)
        img[y + 1, 31:33] = (255, 250, 220)
    return np.clip(img, 0, 255)


def lever(img, down=True):
    img[20:46, 24:40] = (60, 58, 54)
    img[20, 24:40] = (128, 124, 116)
    img[45, 24:40] = (30, 28, 26)
    img[31:35, 29:35] = (30, 30, 30)
    if down:
        for i in range(12):
            img[33 + i, 31:33] = (180, 176, 168)
        img[44:48, 29:35] = (200, 30, 30)
    else:
        for i in range(12):
            img[21 + i, 31:33] = (180, 176, 168)
        img[17:21, 29:35] = (40, 160, 60)
    img = centered(img, "SAÍDA", 32, 50, 1, (230, 220, 190), shadow=(20, 20, 20))
    return img


def dark(img, k=0.68):
    return np.clip(np.asarray(img, float) * k, 0, 255)


def star(img, cx, cy, r, col):
    pts = []
    for i in range(10):
        a = math.radians(-90 + i * 36)
        rr = r if i % 2 == 0 else r * 0.42
        pts.append((cx + math.cos(a) * rr, cy + math.sin(a) * rr))
    im = Image.fromarray(np.clip(img, 0, 255).astype(np.uint8))
    ImageDraw.Draw(im).polygon(pts, fill=tuple(int(c) for c in col))
    return np.array(im).astype(float)


def invader_wall(img):
    """Parede arrombada pelos invasores: rombo com pedras soltas, estrela
    vermelha pintada à pressa e o estandarte vermelho deles, rasgado."""
    rng = random.Random(5)
    # rombo
    for y in range(30, 62):
        for x in range(4, 30):
            d = math.hypot((x - 16) / 12, (y - 48) / 14) + rng.uniform(-0.12, 0.12)
            if d < 0.85:
                img[y, x] = (14, 12, 12)
            elif d < 1.0:
                img[y, x] = img[y, x] * 0.55
    for _ in range(14):
        x, y = rng.randint(2, 32), rng.randint(56, 62)
        img[y : y + 2, x : x + 3] = (100, 96, 88)
    # fendas
    for sx, sy in ((28, 34), (8, 32)):
        x, y = sx, sy
        for _ in range(16):
            img[y, x] = (30, 28, 26)
            x += rng.choice((-1, 0, 1))
            y -= 1
    # estrela pintada com tinta a escorrer
    img = star(img, 20, 15, 10, (190, 20, 24))
    for x in (14, 19, 25):
        img[22 : 22 + rng.randint(3, 8), x] = (170, 16, 20)
    # estandarte vermelho dos invasores, rasgado
    x0, x1, y0, y1 = 36, 58, 6, 50
    img[y0 - 2 : y0, x0 - 2 : x1 + 2] = (120, 90, 40)
    for x in range(x0, x1):
        fold = 0.82 + 0.18 * math.cos((x - x0) / (x1 - x0) * math.pi * 3)
        bottom = y1 - (abs(((x - x0) * 7) % 9 - 4) if x > x0 + 6 else 0)
        for y in range(y0, bottom):
            img[y, x] = np.array((196, 22, 24)) * fold
    img = star(img, 47, 16, 6, (250, 210, 40))
    img[24:26, 41:53] = (250, 210, 40)
    return img


def armoury(base):
    """Arrecadação com grade: espingardas no armeiro e caixas de munições."""
    img = base.copy()
    img[8:60, 7:57] = (18, 16, 16)
    img[10:58, 9:55] = (52, 46, 38)
    img[14:16, 11:53] = (96, 70, 40)  # prateleira do armeiro
    img[44:46, 11:53] = (96, 70, 40)
    for x in range(14, 52, 6):
        img[16:44, x] = (40, 40, 44)  # cano
        img[30:44, x - 1 : x + 2] = (110, 70, 36)  # coronha
        img[22:26, x + 1] = (60, 60, 64)  # carregador
    for x0 in (12, 32):
        img[48:56, x0 : x0 + 16] = (86, 92, 56)
        img[48, x0 : x0 + 16] = (120, 128, 80)
        img[51:53, x0 + 6 : x0 + 10] = (220, 200, 120)
    for x in range(9, 56, 6):
        img[8:60, x : x + 2] = (92, 94, 100)
        img[8:60, x] = (150, 152, 158)
    for y in (8, 9, 32, 33, 58, 59):
        img[y, 7:57] = (110, 112, 118)
    return img


def walls():
    w = {}
    w["stone0"] = stone(1)
    w["stone1"] = stone(2, rows=(8, 13, 10, 12, 9, 12))
    w["stone-painting"] = portrait(stone(3), 17, 11)
    import flags

    w["stone-banner"] = flags.wall_banner(stone(4))  # 128x128, para as armas se lerem
    w["stone-plaque"] = trilogia_plaque(stone(5))
    w["stone-arch"] = arch(stone(6))
    w["stone-poster"] = poster_licao(stone(7))
    w["stone-censura"] = invader_wall(stone(8))  # nome antigo do ficheiro
    w["blue0"] = azulejo(9, 0)
    w["blue1"] = azulejo(10, 1)
    w["azulejo-panel"] = azulejo_panel()
    w["blue-cell"] = cell_bars(stone(11, base=(116, 112, 104)))
    w["blue-cell-robot"] = armoury(stone(12, base=(116, 112, 104)))
    w["wood"] = wood(13)
    w["wood-emblem"] = armillary(cross_of_christ(wood(14), 32, 14, 5), 32, 38, 13)
    w["wood-painting"] = portrait(wood(15), 17, 12)
    w["wood-mocidade"] = mocidade(wood(16))
    w["brick"] = bricks(17)
    w["cave"] = rock(18)
    w["door"] = prison_door()
    w["door-frame"] = door_frame()
    w["lift-door"] = lift_door()
    w["lift-panel"] = lift_panel()
    w["lever-down"] = lever(stone(19), True)
    w["lever-up"] = lever(stone(19), False)
    return w
