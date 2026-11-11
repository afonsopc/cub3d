# Bandeira nacional e estandarte, desenhados a 128x128 com cores lisas e
# poucas formas, para as armas se lerem: esfera armilar como anel amarelo
# de 2 px com contorno escuro de 1 px, uma faixa horizontal e uma diagonal,
# e ao centro o escudo branco com bordadura vermelha e 5 quinas azuis de
# 2x2 em cruz. Sem ruído nem ondulação nas armas.
import math

import numpy as np

GREEN = (0, 102, 0)
RED = (230, 0, 0)
YELLOW = (250, 210, 0)
DARK = (34, 22, 8)
WHITE = (250, 250, 246)
BLUE = (0, 40, 170)
MAG = (255, 0, 255)


def arms(R=17):
    """RGBA quadrado com as armas; R = raio exterior do anel amarelo."""
    S = 2 * R + 5
    c = (S - 1) / 2
    yy, xx = np.mgrid[0:S, 0:S]
    dx, dy = xx - c, yy - c
    d = np.hypot(dx, dy)
    yellow = (d <= R) & (d > R - 2)
    inside = d <= R - 2
    yellow |= inside & (np.abs(dy) <= 0.6)  # faixa do horizonte (2 px)
    a = math.radians(-35)
    dist_diag = np.abs(dx * math.sin(a) - dy * math.cos(a))
    yellow |= inside & (dist_diag <= 1.0)  # faixa diagonal
    # contorno escuro de 1 px à volta de todo o amarelo
    pad = np.pad(yellow, 1)
    grown = np.zeros_like(yellow)
    for oy in (-1, 0, 1):
        for ox in (-1, 0, 1):
            grown |= pad[1 + oy : 1 + oy + S, 1 + ox : 1 + ox + S]
    outline = grown & ~yellow
    img = np.zeros((S, S, 4), np.uint8)
    img[outline] = (*DARK, 255)
    img[yellow] = (*YELLOW, 255)
    # escudo: retângulo com fundo redondo, contorno escuro, bordadura
    w = max(5, int(round(R * 0.42)))  # meia largura
    h = int(round(w * 2.3))  # altura
    top = int(round(c - h * 0.5))
    cx = int(round(c))

    def in_shield(x, y, m):
        if y < top + m or y > top + h - m or abs(x - cx) > w - m:
            return False
        ym = top + h * 0.55
        if y > ym:
            rx = w - m + 0.5
            ry = (top + h - m) - ym + 0.5
            return ((x - cx) / rx) ** 2 + ((y - ym) / ry) ** 2 <= 1
        return True

    b = max(2, int(round(w / 3)))
    for y in range(S):
        for x in range(S):
            if in_shield(x, y, -1):
                img[y, x] = (*DARK, 255)
            if in_shield(x, y, 0):
                img[y, x] = (*RED, 255)
            if in_shield(x, y, b):
                img[y, x] = (*WHITE, 255)
    # 5 quinas 2x2 em cruz
    qy = top + b + (h - 2 * b) // 2 - 1
    sp = 3
    for ox, oy in ((0, 0), (0, -sp), (0, sp), (-sp, 0), (sp, 0)):
        x0, y0 = cx - 1 + ox, qy + oy
        img[y0 : y0 + 2, x0 : x0 + 2] = (*BLUE, 255)
    return img


def paste(dst, src, x, y):
    h, w = src.shape[:2]
    m = src[..., 3] > 0
    reg = dst[y : y + h, x : x + w]
    reg[m] = src[..., : reg.shape[2]][m]
    if dst.shape[2] == 4:
        reg[m, 3] = 255
    return dst


def shade_cloth(img, x0, x1, y0, y1, amp=0.1):
    """Sombreado suave por colunas (o pano 'cai'), sem deformar nada."""
    for x in range(x0, x1):
        t = (x - x0) / max(1, x1 - x0)
        k = 1.0 - amp * 0.5 + amp * 0.5 * math.cos(t * math.pi * 2.6)
        img[y0:y1, x, :3] = np.clip(img[y0:y1, x, :3].astype(float) * k, 0, 255)
    return img


def pole_flag():
    """Bandeira num mastro (128x128 RGBA): 2/5 verde, 3/5 vermelho, armas
    ao centro da divisão (~60% da altura do pano)."""
    W = 128
    img = np.zeros((W, W, 4), np.uint8)
    x0, x1, y0, y1 = 28, 116, 14, 72  # pano 88x58
    split = x0 + int(round((x1 - x0) * 0.4))
    img[y0:y1, x0:split] = (*GREEN, 255)
    img[y0:y1, split:x1] = (*RED, 255)
    img = shade_cloth(img, x0, x1, y0, y1)
    img[y0:y1, x1 - 1] = (*DARK, 255)
    img[y1 - 1, x0:x1] = (*DARK, 255)
    img[y0, x0:x1] = (*DARK, 255)
    a = arms(17)
    s = a.shape[0]
    paste(img, a, split - s // 2, (y0 + y1) // 2 - s // 2)
    # mastro, remate e base
    img[8:120, 23:28] = (120, 84, 46, 255)
    img[8:120, 23] = (170, 124, 72, 255)
    img[8:120, 27] = (70, 48, 26, 255)
    for y in range(2, 10):
        for x in range(20, 31):
            if (x - 25) ** 2 + (y - 6) ** 2 <= 12:
                img[y, x] = (230, 180, 50, 255)
    img[116:124, 14:38] = (104, 100, 92, 255)
    img[116, 14:38] = (150, 146, 136, 255)
    img[123, 14:38] = (60, 58, 54, 255)
    img[y0 + 2 : y0 + 5, 27:29] = (40, 40, 44, 255)
    img[y1 - 5 : y1 - 2, 27:29] = (40, 40, 44, 255)
    for y in range(124, 128):  # sombra
        for x in range(8, 48):
            if ((x - 26) / 20) ** 2 + ((y - 126) / 2) ** 2 <= 1 and img[y, x, 3] == 0:
                img[y, x] = (18, 16, 16, 255)
    return img


def wall_banner(stone64):
    """Estandarte vertical (verde em cima, 2/5) pendurado na pedra, 128x128."""
    img = np.repeat(np.repeat(np.asarray(stone64, float), 2, 0), 2, 1)
    x0, x1, y0, y1 = 36, 92, 16, 98
    # sombra na parede e varão
    img[y0 + 2 : y1 + 8, x0 + 3 : x1 + 3] *= 0.55
    img[y0 - 7 : y0 - 3, x0 - 6 : x1 + 6] = (220, 172, 52)
    img[y0 - 7, x0 - 6 : x1 + 6] = (255, 226, 120)
    img[y0 - 4, x0 - 6 : x1 + 6] = (150, 104, 26)
    img[y0 - 9 : y0 - 1, x0 - 10 : x0 - 6] = (255, 226, 120)
    img[y0 - 9 : y0 - 1, x1 + 6 : x1 + 10] = (255, 226, 120)
    split = y0 + int(round((y1 - y0) * 0.4))
    img[y0:split, x0:x1] = GREEN
    img[split:y1, x0:x1] = RED
    # ponta em V e franja dourada
    for x in range(x0, x1):
        t = (x - x0) / (x1 - x0)
        depth = int(round(10 * (1 - abs(t * 2 - 1))))
        img[y1 : y1 + depth, x] = RED
        img[y1 + depth : y1 + depth + 1, x] = DARK
        if x % 3 == 0:
            img[y1 + depth + 1 : y1 + depth + 4, x] = (230, 180, 50)
    rgba = np.zeros((128, 128, 4), np.uint8)
    rgba[..., :3] = np.clip(img, 0, 255)
    rgba[..., 3] = 255
    rgba = shade_cloth(rgba, x0, x1, y0, y1 + 10, amp=0.12)
    rgba[y0:y1, x0] = (*DARK, 255)
    rgba[y0:y1, x1 - 1] = (*DARK, 255)
    a = arms(19)
    s = a.shape[0]
    paste(rgba, a, (x0 + x1) // 2 - s // 2, split - s // 2)
    return rgba[..., :3].astype(float)


def reference(scale=8):
    """Referência das armas ampliadas 8x, sobre a divisão verde/vermelha."""
    a = arms(17)
    s = a.shape[0] + 8
    bg = np.zeros((s, s, 3), np.uint8)
    bg[:, : s // 2] = GREEN
    bg[:, s // 2 :] = RED
    out = bg.copy()
    m = a[..., 3] > 0
    out[4:-4, 4:-4][m] = a[..., :3][m]
    return np.repeat(np.repeat(out, scale, 0), scale, 1)
