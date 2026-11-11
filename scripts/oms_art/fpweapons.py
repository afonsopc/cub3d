# Armas na mão (primeira pessoa), desenhadas em píxeis a 64x64 como nos
# jogos VGA do início dos anos 90: a mão e o antebraço sobem do fundo ao
# centro e seguram a arma com o cano para a frente, para dentro do ecrã,
# vista de trás e um pouco de cima (encurtada), com a corrediça/caixa e a
# mira traseira à vista. Disparo: recuo e clarão na ponta do cano, ao
# centro. A faca dá uma estocada para a frente. Tudo desenhado aqui.
import math

import numpy as np
from PIL import Image, ImageDraw

from vox import add_outline

SKIN = (222, 164, 120)
SKIN_L = (244, 196, 156)
SKIN_D = (170, 112, 76)
SKIN_DD = (126, 78, 50)
SLEEVE = (110, 78, 46)  # casaco de bombazina castanho
SLEEVE_L = (140, 102, 62)
SLEEVE_D = (76, 52, 30)
ST = (70, 74, 84)  # aço
ST_L = (132, 138, 150)
ST_LL = (186, 192, 204)
ST_D = (40, 42, 50)
ST_DD = (22, 22, 28)
WOOD = (120, 74, 38)
WOOD_L = (160, 104, 56)
BRASS = (214, 170, 70)
BRASS_D = (150, 110, 40)
COPPER = (190, 110, 60)
BLADE = (190, 196, 206)
BLADE_L = (236, 240, 246)
BLADE_D = (120, 126, 138)
FL_O = (255, 140, 30)
FL_Y = (255, 226, 90)
FL_W = (255, 255, 230)
OUTLINE = (20, 16, 20)


class Pen:
    """Desenha polígonos através de uma transformação (recuo, estocada)."""

    def __init__(self, dx=0.0, dy=0.0, s=1.0, tilt=0.0, pivot=(32, 64), sx=1.0, sy=1.0, res=128):
        # desenha-se em coordenadas de 64x64 mas grava-se a 128x128, para a
        # arma pequena manter o detalhe
        self.k = res / 64
        self.im = Image.new("RGBA", (res, res), (0, 0, 0, 0))
        self.d = ImageDraw.Draw(self.im)
        self.dx, self.dy, self.s, self.tilt, self.pivot = dx, dy, s, tilt, pivot
        self.sx, self.sy = sx, sy

    def T(self, p):
        x, y = p
        px, py = self.pivot
        x, y = px + (x - px) * self.s * self.sx, py + (y - py) * self.s * self.sy
        # inclinação: o que está mais acima sobe ainda mais (cano a levantar)
        y -= self.tilt * max(0.0, (py - y)) / 40.0
        return ((x + self.dx * self.s) * self.k, (y + self.dy * self.s) * self.k)

    def poly(self, pts, col):
        self.d.polygon([self.T(p) for p in pts], fill=(*col, 255))

    def rect(self, x0, y0, x1, y1, col):
        self.poly([(x0, y0), (x1, y0), (x1, y1), (x0, y1)], col)

    def line(self, a, b, col, w=1):
        self.d.line([self.T(a), self.T(b)], fill=(*col, 255), width=max(1, int(round(w * self.k * self.s))))

    def ellipse(self, cx, cy, rx, ry, col):
        (x0, y0), (x1, y1) = self.T((cx - rx, cy - ry)), self.T((cx + rx, cy + ry))
        self.d.ellipse([x0, y0, x1, y1], fill=(*col, 255))

    def array(self):
        return np.array(self.im)


def flash(arr, cx, cy, big=True, k=1.0):
    """Clarão em estrela na ponta do cano (desenhado por cima, sem pivô)."""
    im = Image.fromarray(arr, "RGBA")
    d = ImageDraw.Draw(im)
    for r, col, n in ((13 if big else 7, FL_O, 8), (8 if big else 4, FL_Y, 8), (4 if big else 2, FL_W, 6)):
        pts = []
        for i in range(n * 2):
            a = math.pi * i / n + (0.2 if col == FL_Y else 0)
            rr = (r if i % 2 == 0 else r * 0.45) * k
            pts.append((cx + math.cos(a) * rr, cy + math.sin(a) * rr * 0.8))
        d.polygon(pts, fill=(*col, 255))
    return np.array(im)


# ------------------------------------------------------------------ mãos


def sleeve(p, pts):
    p.poly(pts, SLEEVE)
    (x0, y0), (x1, y1), (x2, y2), (x3, y3) = pts
    # vincos da bombazina
    for t in (0.25, 0.5, 0.75):
        a = (x0 + (x1 - x0) * t, y0)
        b = (x3 + (x2 - x3) * t, y3)
        p.line(a, b, SLEEVE_D)
    p.line((x3, y3), (x2, y2), SLEEVE_L)
    p.poly([(x3, y3), (x2, y2), (x2, y2 + 2), (x3, y3 + 2)], SLEEVE_D)  # punho


def fist(p, x0, x1, ytop, ybot):
    """Mão fechada vista de trás: costas da mão e nós dos dedos."""
    p.poly([(x0, ybot), (x1, ybot), (x1 - 1, ytop + 3), (x0 + 1, ytop + 3)], SKIN)
    p.poly([(x1 - 4, ybot), (x1, ybot), (x1 - 1, ytop + 3), (x1 - 4, ytop + 3)], SKIN_D)
    w = (x1 - x0 - 2) / 4
    for i in range(4):
        a = x0 + 1 + i * w
        p.poly([(a, ytop + 4), (a + w - 1, ytop + 4), (a + w - 1.5, ytop), (a + 0.5, ytop)], SKIN_L if i < 3 else SKIN)
        p.line((a + w - 1, ytop + 1), (a + w - 1, ytop + 5), SKIN_DD)
    p.line((x0 + 2, ytop + 8), (x1 - 5, ytop + 8), SKIN_D)  # tendões


# ------------------------------------------------------------------ pistola
# Coordenadas finais em 64x64: o ecrã do jogo mostra esta imagem com a
# altura da vista, por isso o centro da mira fica em y ~ 30.


GUN_SCALE = 0.46  # arma pequena, em baixo ao centro: o topo fica a ~70% da vista


def pistol_frame(dx=0, dy=0, tilt=0):
    p = Pen(dx, dy, GUN_SCALE, tilt, sx=1.3)
    sleeve(p, [(17, 64), (47, 64), (45, 58), (20, 58)])
    fist(p, 20, 45, 47, 59)
    # traseira da corrediça
    p.rect(24, 43, 40, 49, ST)
    p.rect(24, 43, 40, 44, ST_L)
    p.rect(39, 44, 40, 49, ST_D)
    p.rect(30, 48, 34, 51, ST_DD)  # cão
    p.rect(31, 47, 33, 48, ST_L)
    # mira traseira: dois postes e o entalhe ao centro
    p.rect(25, 40, 30, 43, ST_L)
    p.rect(34, 40, 39, 43, ST_L)
    p.rect(25, 40, 30, 41, ST_LL)
    p.rect(34, 40, 39, 41, ST_LL)
    # topo da corrediça, comprido, a fugir para a frente
    p.poly([(25, 40), (39, 40), (36, 25), (28, 25)], ST)
    p.poly([(30, 40), (34, 40), (33, 25), (31, 25)], ST_L)
    p.line((32, 40), (32, 26), ST_LL)
    p.poly([(39, 40), (41, 44), (37, 26), (36, 25)], ST_D)
    p.poly([(25, 40), (23, 44), (27, 26), (28, 25)], ST_D)
    for y in (38, 36, 34):  # estrias
        k = (40 - y) / 15
        p.line((25 + 3 * k, y), (39 - 3 * k, y), ST_D)
    p.poly([(34.5, 32), (37, 32), (36.6, 28.5), (34.5, 28.5)], ST_DD)  # janela de ejeção
    p.rect(31, 22, 33, 25, ST_LL)  # mira da frente
    # polegar ao longo do lado esquerdo
    p.poly([(19, 56), (23, 50), (26, 44), (23, 43), (18, 50)], SKIN)
    p.poly([(23, 43), (26, 44), (25.5, 45.5), (22.5, 44.5)], SKIN_L)
    return p, p.T((32, 23))


# ------------------------------------------------------------------ FBP


def fbp_frame(dx=0, dy=0, tilt=0):
    p = Pen(dx, dy, GUN_SCALE, tilt)
    # mão esquerda no carregador (baixo, à esquerda)
    sleeve(p, [(2, 64), (22, 64), (24, 55), (7, 53)])
    p.rect(20, 45, 25, 55, ST_D)  # carregador
    p.rect(20, 45, 21, 55, ST)
    fist(p, 8, 25, 46, 57)
    # mão direita no punho
    sleeve(p, [(28, 64), (52, 64), (50, 58), (31, 58)])
    fist(p, 30, 50, 48, 59)
    # tampa traseira redonda do tubo
    p.ellipse(32, 47, 7.5, 5.5, ST)
    p.ellipse(32, 46.4, 7.5, 4.6, ST_L)
    p.ellipse(32, 47, 5, 3.4, ST)
    p.ellipse(32, 47.4, 2, 1.5, ST_DD)
    # tubo da caixa a fugir para a frente
    p.poly([(25, 45), (39, 45), (35.5, 27), (28.5, 27)], ST)
    p.poly([(29.5, 45), (34.5, 45), (33, 27), (31, 27)], ST_L)
    p.line((32, 45), (32, 28), ST_LL)
    p.poly([(39, 45), (41, 48), (36.5, 28), (35.5, 27)], ST_D)
    p.poly([(25, 45), (23, 48), (27.5, 28), (28.5, 27)], ST_D)
    # mira de dioptro (anel) por cima do tubo
    p.rect(30.8, 38, 33.2, 43, ST_D)
    p.ellipse(32, 37.5, 3, 2.6, ST_LL)
    p.ellipse(32, 37.5, 1.3, 1.1, ST_DD)
    # manípulo de armar à direita
    p.rect(39, 36, 44, 38.5, ST_L)
    p.rect(43, 35, 45, 39.5, ST_LL)
    # cano à vista
    p.poly([(30, 27), (34, 27), (33.4, 20), (30.6, 20)], ST_D)
    p.poly([(31.3, 27), (32.7, 27), (32.4, 20), (31.6, 20)], ST_L)
    p.rect(31, 17, 33, 20, ST_LL)  # ponto de mira
    return p, p.T((32, 19))


# ------------------------------------------------------------------ metralhadora


def mg_frame(dx=0, dy=0, tilt=0):
    p = Pen(dx, dy, GUN_SCALE, tilt)
    # fita de munições a cair pela esquerda
    for i in range(7):
        x, y = 18 - i * 2.4, 40 + i * 3.4
        p.poly([(x, y), (x + 4, y - 1), (x + 5, y + 2), (x + 1, y + 3)], BRASS)
        p.poly([(x, y), (x + 1.5, y - 0.4), (x + 2.4, y + 2.6), (x + 1, y + 3)], COPPER)
        p.line((x + 4, y - 1), (x + 5, y + 2), BRASS_D)
    # mãos
    sleeve(p, [(0, 64), (17, 64), (20, 57), (3, 55)])
    fist(p, 4, 21, 47, 58)
    sleeve(p, [(25, 64), (49, 64), (47, 58), (28, 58)])
    fist(p, 27, 47, 49, 59)
    # traseira da caixa
    p.rect(19, 40, 45, 51, ST)
    p.rect(19, 40, 45, 41, ST_L)
    p.rect(44, 41, 45, 51, ST_D)
    p.rect(28, 44, 36, 49, ST_D)
    p.rect(29, 44, 35, 45, ST_L)
    # tampa de alimentação a fugir para a frente
    p.poly([(20, 40), (44, 40), (39, 27), (25, 27)], ST)
    p.poly([(28, 40), (36, 40), (34, 27), (30, 27)], ST_L)
    p.line((32, 40), (32, 28), ST_LL)
    p.line((22, 36), (42, 36), ST_D)  # dobradiça da tampa
    p.poly([(44, 40), (47, 43), (41, 28), (39, 27)], ST_D)
    p.poly([(20, 40), (17, 43), (23, 28), (25, 27)], ST_D)
    # mira de lâmina com entalhe
    p.rect(28.5, 31, 35.5, 35, ST_D)
    p.rect(28.5, 31, 31, 32.5, ST_LL)
    p.rect(33, 31, 35.5, 32.5, ST_LL)
    # pega de transporte
    p.line((39, 38), (44, 31), ST_LL, 2)
    p.line((44, 31), (41, 28), ST_L, 2)
    # manga do cano com furos
    p.poly([(27.5, 27), (36.5, 27), (35, 17), (29, 17)], ST_D)
    p.poly([(30.5, 27), (33.5, 27), (33, 17), (31, 17)], ST)
    for j, y in enumerate(range(18, 27, 2)):
        for k in (-1, 1):
            x = 32 + k * (2.4 + (y - 17) * 0.12)
            p.rect(x - 0.6, y, x + 0.6, y + 1, ST_DD)
    p.poly([(29.5, 17), (34.5, 17), (33.6, 14), (30.4, 14)], ST_L)  # quebra-chamas
    return p, p.T((32, 14))


# ------------------------------------------------------------------ faca


def knife_frame(stage):
    """0 parada, 1 recua, 2 estocada, 3 braço esticado, 4 volta."""
    dx, dy, s = ((0, 0, 1.0), (4, 6, 1.06), (-1, -6, 0.86), (-2, -10, 0.74), (1, -2, 0.95))[stage]
    p = Pen(dx, dy, s * GUN_SCALE, 0, pivot=(34, 64), sx=1.25)
    sleeve(p, [(21, 64), (51, 64), (48, 57), (25, 57)])
    fist(p, 25, 48, 47, 59)
    # guarda de latão
    p.rect(24, 44, 47, 47.5, BRASS)
    p.rect(24, 44, 47, 45, (240, 210, 120))
    p.rect(24, 46.5, 47, 47.5, BRASS_D)
    # lâmina para a frente (encurtada), gume à esquerda
    p.poly([(31, 44), (41, 44), (37, 29), (35, 29)], BLADE)
    p.poly([(31, 44), (35.5, 44), (35.6, 29), (35, 29)], BLADE_L)
    p.poly([(38.5, 44), (41, 44), (37, 29), (36.5, 29)], BLADE_D)
    p.line((36, 43), (36, 30), BLADE_L)
    p.poly([(35, 29), (37, 29), (36, 25)], BLADE_L)  # ponta
    # polegar por cima
    p.poly([(23, 52), (27, 47), (31, 45), (31, 48), (26, 52)], SKIN)
    return p.array()


# ------------------------------------------------------------------ fotogramas

FRAMES = [
    dict(dy=0, tilt=0, fire=None),
    dict(dy=-1, tilt=0.5, fire=None),
    dict(dy=-4, tilt=2.5, fire="big"),
    dict(dy=-2, tilt=1.2, fire="small"),
    dict(dy=-1, tilt=0.3, fire=None),
]

GUNS = {"blaster": pistol_frame, "rifle": fbp_frame, "cannon": mg_frame}


def gun_frames(name):
    out = []
    for f in FRAMES:
        p, tip = GUNS[name](0, f["dy"], f["tilt"])
        a = p.array()
        a = add_outline(a, OUTLINE)
        if f["fire"]:
            k = 2 * GUN_SCALE
            a = flash(a, tip[0], tip[1] - 2 * k, big=f["fire"] == "big", k=k)
        out.append(a)
    return out


def knife_frames():
    return [add_outline(knife_frame(i), OUTLINE) for i in range(5)]
