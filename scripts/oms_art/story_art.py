#!/usr/bin/env python3
# Painéis da história (320x200, como os ecrãs VGA dos jogos de 1992), o
# fundo das ordens e das estatísticas e o retrato do comandante para o
# rádio. Desenhados aqui por código; o texto é posto pelo jogo por cima
# (faixa escura em baixo). Tudo ficção: a invasão é de um exército
# estrangeiro inventado.
import math
import os
import random
import sys

import numpy as np
from PIL import Image, ImageDraw

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

import flags  # noqa: E402
import hud  # noqa: E402
from pixart import grid, paste, scale  # noqa: E402
from pixfont import centered  # noqa: E402

ROOT = os.path.abspath(os.path.join(HERE, "..", ".."))
OUT = os.path.join(ROOT, "assets", "oms3d", "story")
W, H = 320, 200
TEXT_Y = 136  # o texto do jogo começa a ~70% da altura

STAR_RED = (200, 24, 28)
NIGHT = [(8, 10, 26), (24, 30, 60)]


def canvas(top, bottom):
    img = np.zeros((H, W, 3))
    for y in range(H):
        t = y / H
        img[y] = np.array(top) * (1 - t) + np.array(bottom) * t
    return img


def text_band(img, y0=TEXT_Y):
    img[y0:] = img[y0:] * 0.18 + np.array([10, 10, 14]) * 0.82
    img[y0] = (200, 160, 60)
    img[y0 + 1] = (90, 70, 30)
    return img


def top_band(img, y1=16):
    """Faixa escura em cima, onde o jogo escreve SALTAR (fora da ilustração)."""
    img[:y1] = img[:y1] * 0.15 + np.array([10, 10, 14]) * 0.85
    img[y1 - 1] = (90, 70, 30)
    return img


def dither_cone(img, apex, top, bottom, col, strength=0.55):
    """Feixe de luz suave: triângulo com ponteado que esmorece ao longe."""
    ax, ay = apex
    x1 = top[0]
    for x in range(min(ax, x1), max(ax, x1) + 1):
        t = abs(x - ax) / max(1, abs(x1 - ax))
        ya = ay + (top[1] - ay) * t
        yb = ay + (bottom[1] - ay) * t
        for y in range(int(ya), int(yb) + 1):
            if not (0 <= y < H and 0 <= x < W):
                continue
            a = strength * (1 - t) ** 0.8
            if (x + y) % 2 == 0 or t < 0.08:
                img[y, x] = img[y, x] * (1 - a) + np.array(col) * a
    return img


def dither_cone_v(img, apex, ytop, xl, xr, col, strength=0.45):
    """Holofote: cone vertical com ponteado, mais forte junto à base."""
    ax, ay = apex
    for y in range(ytop, ay + 1):
        t = (ay - y) / max(1, ay - ytop)
        xa = ax + (xl - ax) * t
        xb = ax + (xr - ax) * t
        for x in range(int(xa), int(xb) + 1):
            if 0 <= x < W and (x + y) % 2 == 0:
                a = strength * (1 - 0.6 * t)
                img[y, x] = img[y, x] * (1 - a) + np.array(col) * a
    return img


def frame(img):
    img[0:2] = (90, 70, 30)
    img[-2:] = (90, 70, 30)
    img[:, 0:2] = (90, 70, 30)
    img[:, -2:] = (90, 70, 30)
    return img


def draw(img):
    im = Image.fromarray(np.clip(img, 0, 255).astype(np.uint8))
    return im, ImageDraw.Draw(im)


def star(d, cx, cy, r, col):
    pts = []
    for i in range(10):
        a = math.radians(-90 + i * 36)
        rr = r if i % 2 == 0 else r * 0.42
        pts.append((cx + math.cos(a) * rr, cy + math.sin(a) * rr))
    d.polygon(pts, fill=col)


def stars_sky(img, n, seed, ymax):
    rng = random.Random(seed)
    for _ in range(n):
        x, y = rng.randrange(W), rng.randrange(ymax)
        img[y, x] = (200, 200, 220) if rng.random() < 0.7 else (255, 240, 180)
    return img


def ship(d, x, y, s, col=(30, 30, 38)):
    """Navio de guerra em silhueta: casco, superestrutura, mastro, canhão,
    vigias acesas e a esteira na água."""
    lit = tuple(min(255, c + 26) for c in col)
    d.polygon([(x, y), (x + 60 * s, y), (x + 54 * s, y + 8 * s), (x + 6 * s, y + 8 * s)], fill=col)
    d.line([(x + 2 * s, y), (x + 58 * s, y)], fill=lit)
    d.rectangle([x + 20 * s, y - 10 * s, x + 36 * s, y], fill=col)
    d.rectangle([x + 24 * s, y - 15 * s, x + 32 * s, y - 10 * s], fill=col)
    d.line([(x + 28 * s, y - 15 * s), (x + 28 * s, y - 26 * s)], fill=col)
    d.line([(x + 24 * s, y - 22 * s), (x + 32 * s, y - 22 * s)], fill=col)
    d.line([(x + 40 * s, y - 3 * s), (x + 52 * s, y - 6 * s)], fill=col, width=max(1, int(2 * s)))
    d.rectangle([x + 38 * s, y - 4 * s, x + 44 * s, y], fill=col)
    for k in range(4):
        px = x + (10 + k * 11) * s
        d.point((px, y + 3 * s), fill=(230, 200, 110))
    star(d, x + 28 * s, y - 5 * s, 2.5 * s, STAR_RED)
    d.line([(x - 10 * s, y + 9 * s), (x + 64 * s, y + 9 * s)], fill=(60, 80, 120))


def intro1():
    """A frota estrangeira chega de noite à costa."""
    img = canvas(NIGHT[0], NIGHT[1])
    img = stars_sky(img, 90, 1, 90)
    im, d = draw(img)
    d.ellipse([250, 18, 270, 38], fill=(230, 230, 200))
    d.rectangle([0, 92, W, TEXT_Y], fill=(14, 22, 48))
    for y in range(94, TEXT_Y, 4):
        d.line([(0, y), (W, y)], fill=(22, 34, 70))
    d.polygon([(0, 70), (60, 64), (90, 80), (120, 92), (0, 92)], fill=(20, 18, 16))
    d.rectangle([30, 44, 38, 66], fill=(200, 196, 180))  # farol
    d.polygon([(28, 44), (40, 44), (34, 38)], fill=(150, 30, 30))
    for i, (x, y, s) in enumerate(((150, 96, 1.0), (215, 90, 0.8), (270, 98, 0.6))):
        ship(d, x, y, s)
    for x, y in ((140, 116), (175, 120), (205, 114)):  # barcaças
        d.polygon([(x, y), (x + 18, y), (x + 15, y + 5), (x + 3, y + 5)], fill=(40, 40, 46))
        d.line([(x - 6, y + 6), (x + 22, y + 6)], fill=(60, 80, 120))
    img = np.array(im).astype(float)
    img = dither_cone(img, (38, 48), (150, 30), (150, 64), (255, 236, 170))  # feixe do farol
    img[45:51, 33:36] = (255, 240, 180)
    return frame(top_band(text_band(img)))


def intro2():
    """Paraquedistas inimigos sobre a cidade adormecida."""
    img = canvas((14, 12, 28), (40, 30, 50))
    img = stars_sky(img, 50, 2, 70)
    im, d = draw(img)
    rng = random.Random(3)
    x = 0
    while x < W:  # telhados
        w = rng.randint(14, 30)
        h = rng.randint(20, 44)
        d.rectangle([x, TEXT_Y - h, x + w, TEXT_Y], fill=(26, 22, 26))
        d.polygon([(x - 2, TEXT_Y - h), (x + w // 2, TEXT_Y - h - 8), (x + w + 2, TEXT_Y - h)], fill=(70, 30, 24))
        for wy in range(TEXT_Y - h + 6, TEXT_Y - 4, 8):
            if rng.random() < 0.25:
                d.rectangle([x + 4, wy, x + 7, wy + 3], fill=(230, 190, 90))
        x += w + 2
    img2 = np.array(im).astype(float)
    for (sx, sy) in ((40, 18), (110, 30), (160, 20), (230, 26), (290, 18)):  # holofotes
        img2 = dither_cone_v(img2, (sx + 10, TEXT_Y - 30), sy, sx - 7, sx + 7, (200, 200, 230))
    im, d = draw(img2)
    for (px, py) in ((60, 30), (100, 54), (150, 40), (200, 62), (245, 34), (280, 58)):
        d.pieslice([px - 9, py - 8, px + 9, py + 8], 180, 360, fill=(170, 160, 130))
        d.line([(px - 8, py), (px, py + 10)], fill=(90, 90, 80))
        d.line([(px + 8, py), (px, py + 10)], fill=(90, 90, 80))
        d.rectangle([px - 1, py + 10, px + 1, py + 15], fill=(60, 64, 40))
    return frame(top_band(text_band(np.array(im).astype(float))))


def commander_face():
    """Major Andrade: cabelo e bigode grisalhos, boné de pala com emblema."""
    f = hud.base_face()
    col = f[..., :3].astype(int)
    hair = np.all(col == hud.FCOL["H"], axis=2) | np.all(col == hud.FCOL["h"], axis=2)
    col[hair] = (150, 148, 140)
    must = np.all(col == hud.FCOL["m"], axis=2)
    col[must] = (190, 188, 180)
    helmet = (np.all(col == hud.FCOL["G"], axis=2) | np.all(col == hud.FCOL["g"], axis=2)
              | np.all(col == hud.FCOL["o"], axis=2))
    col[helmet] = (82, 92, 56)
    brim = np.all(col == hud.FCOL["D"], axis=2)
    col[brim] = (20, 20, 18)
    col[3:5, 14:18] = (230, 196, 70)  # emblema dourado do boné
    col[28:32, 8:24][np.any(col[28:32, 8:24] != 0, axis=2)] = (96, 104, 62)
    f[..., :3] = col
    f = hud.eyes(f, 1)
    return f


def portrait(img, face, x, y, k):
    big = scale(face, k)
    paste(img, big, x, y)
    return img


def intro3():
    """No Quartel-General, o Major reúne os últimos homens."""
    img = canvas((60, 46, 34), (34, 26, 20))
    im, d = draw(img)
    for x in range(0, W, 20):  # lambrim
        d.line([(x, 0), (x, TEXT_Y)], fill=(48, 36, 26))
    d.rectangle([200, 14, 300, 70], fill=(20, 24, 40))  # janela com a noite
    d.line([(250, 14), (250, 70)], fill=(70, 50, 30), width=2)
    d.line([(200, 42), (300, 42)], fill=(70, 50, 30), width=2)
    img = np.array(im).astype(float)
    fl = np.zeros((40, 60, 3))
    fl[:, :24] = flags.GREEN
    fl[:, 24:] = flags.RED
    a = flags.arms(12)
    m = a[..., 3] > 0
    oy, ox = 20 - a.shape[0] // 2, 24 - a.shape[1] // 2
    fl[oy : oy + a.shape[0], ox : ox + a.shape[1]][m] = a[..., :3][m]
    img[18:58, 120:180] = fl
    im, d = draw(img)
    d.rectangle([20, 100, 300, TEXT_Y], fill=(70, 50, 30))  # mesa com mapa
    d.rectangle([60, 96, 220, 112], fill=(200, 190, 150))
    for x in range(64, 220, 12):
        d.line([(x, 97), (x + 6, 111)], fill=(120, 110, 80))
    star(d, 150, 104, 4, STAR_RED)
    img = np.array(im).astype(float)
    img = portrait(img, commander_face(), 20, 30, 2)
    return frame(top_band(text_band(img)))


def silva_face():
    return hud.face(0, 1)


def intro4():
    """Sargento Silva, com a G3, e o Forte ao fundo."""
    img = canvas((120, 80, 70), (40, 30, 40))
    im, d = draw(img)
    d.rectangle([0, 100, W, TEXT_Y], fill=(30, 26, 26))
    d.rectangle([180, 64, 300, 100], fill=(30, 26, 26))
    for x in range(180, 300, 12):
        d.rectangle([x, 58, x + 7, 64], fill=(30, 26, 26))
    d.rectangle([250, 30, 252, 64], fill=(50, 40, 30))
    img = np.array(im).astype(float)
    fl = np.zeros((14, 22, 3))
    fl[:, :9] = flags.GREEN
    fl[:, 9:] = flags.RED
    img[30:44, 252:274] = fl
    img = portrait(img, silva_face(), 30, 16, 4)
    return frame(top_band(text_band(img)))


def ending1():
    """Madrugada: a bandeira no alto do Forte e os barcos inimigos a fugir."""
    img = canvas((250, 170, 110), (90, 100, 150))
    im, d = draw(img)
    d.ellipse([130, 76, 190, 136], fill=(255, 220, 150))
    d.rectangle([0, 96, W, TEXT_Y], fill=(50, 80, 120))
    for y in range(98, TEXT_Y, 4):
        d.line([(0, y), (W, y)], fill=(80, 110, 150))
    d.rectangle([0, 60, 110, 100], fill=(40, 34, 32))
    for x in range(0, 110, 12):
        d.rectangle([x, 54, x + 7, 60], fill=(40, 34, 32))
    d.rectangle([70, 18, 72, 60], fill=(60, 50, 40))
    for i, (x, y, s) in enumerate(((210, 104, 0.5), (260, 100, 0.4))):
        ship(d, x, y, s, (60, 60, 70))
    img = np.array(im).astype(float)
    fl = np.zeros((22, 34, 3))
    fl[:, :14] = flags.GREEN
    fl[:, 14:] = flags.RED
    a = flags.arms(7)
    m = a[..., 3] > 0
    oy, ox = 11 - a.shape[0] // 2, 14 - a.shape[1] // 2
    fl[oy : oy + a.shape[0], ox : ox + a.shape[1]][m] = a[..., :3][m]
    img[18:40, 72:106] = fl
    return frame(top_band(text_band(img)))


def ending2():
    """Silva com a cruz de guerra ao peito."""
    img = canvas((40, 48, 30), (20, 24, 16))
    img = portrait(img, silva_face(), 112, 18, 3)
    im, d = draw(img)
    cx, cy = 160, 120
    d.rectangle([cx - 5, cy - 14, cx + 5, cy - 6], fill=flags.GREEN)
    d.rectangle([cx - 1, cy - 14, cx + 5, cy - 6], fill=flags.RED)
    d.polygon([(cx, cy - 6), (cx + 7, cy), (cx, cy + 7), (cx - 7, cy)], fill=(236, 190, 60))
    return frame(top_band(text_band(np.array(im).astype(float))))


def briefing(which):
    """Ordens: título em cima, planta em papel ao centro, o Major ao lado."""
    img = canvas((40, 34, 28), (24, 20, 16))
    img[:30] = (14, 12, 12)
    img[29] = (200, 160, 60)
    im, d = draw(img)
    d.rectangle([110, 38, 300, 118], fill=(214, 204, 168))
    rng = random.Random(which)
    if which == 1:  # planta do quartel
        for (x0, y0, x1, y1) in ((120, 46, 170, 80), (176, 46, 250, 70), (256, 46, 292, 110),
                                 (120, 86, 200, 110), (206, 76, 250, 110)):
            d.rectangle([x0, y0, x1, y1], outline=(60, 50, 40), width=2)
    else:  # planta do forte (estrela)
        pts = []
        for i in range(10):
            a = math.radians(-90 + i * 36)
            r = 36 if i % 2 == 0 else 22
            pts.append((205 + math.cos(a) * r * 1.4, 78 + math.sin(a) * r))
        d.polygon(pts, outline=(60, 50, 40))
        d.line([(120, 110), (170, 104), (210, 112), (290, 106)], fill=(40, 70, 140), width=2)
    for _ in range(4):
        x, y = rng.randint(130, 280), rng.randint(50, 100)
        star(d, x, y, 4, STAR_RED)
    d.line([(150, 100), (190, 60)], fill=(30, 80, 30), width=2)
    img = np.array(im).astype(float)
    img = portrait(img, commander_face(), 24, 40, 2)
    return frame(text_band(img, 124))


def stats_bg():
    """Painel limpo: faixa do título com as armas nos cantos e um só quadro
    escuro para as linhas de números (sem riscas)."""
    img = np.zeros((H, W, 3)) + np.array([22, 26, 36])
    img[:52] = (12, 14, 20)
    img[51] = (200, 160, 60)
    img[52] = (90, 70, 30)
    a = flags.arms(16)
    for x in (10, W - 10 - a.shape[1]):
        paste(img, a, x, (52 - a.shape[0]) // 2)
    img[62:178, 36:284] = (14, 16, 24)
    img[62, 36:284] = (70, 76, 96)
    img[177, 36:284] = (70, 76, 96)
    img[62:178, 36] = (70, 76, 96)
    img[62:178, 283] = (70, 76, 96)
    return frame(img)


def save(arr, name):
    os.makedirs(OUT, exist_ok=True)
    import palette

    a = np.clip(np.asarray(arr, float), 0, 255)
    if a.shape[2] == 4:
        a = a[..., :3]
    idx = palette.snap(a)
    im = Image.fromarray(idx.astype(np.uint8), "P")
    im.putpalette(palette.flat())
    im.save(os.path.join(OUT, name))


def main():
    save(intro1(), "intro-1.bmp")
    save(intro2(), "intro-2.bmp")
    save(intro3(), "intro-3.bmp")
    save(intro4(), "intro-4.bmp")
    save(briefing(1), "briefing-1.bmp")
    save(briefing(2), "briefing-2.bmp")
    save(stats_bg(), "stats.bmp")
    save(ending1(), "ending-1.bmp")
    save(ending2(), "ending-2.bmp")
    f = commander_face()
    rgba = f.copy()
    rgba[..., 3] = 255
    bg = rgba[..., :3].copy()
    bg[f[..., 3] == 0] = (30, 44, 92)
    save(scale(bg, 2), "radio-major.bmp")
    print("ok")


if __name__ == "__main__":
    main()
