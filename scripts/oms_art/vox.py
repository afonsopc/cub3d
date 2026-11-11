# Pequeno motor de voxels para desenhar os sprites do cub3d (versão sem
# Wolfenstein). Cada modelo é uma lista de pontos coloridos (x, y, z) em
# unidades de voxel; renderiza-se em projeção ortográfica a partir de qualquer
# ângulo, com sombreado simples e contorno escuro, para ter sprites em 8
# direções coerentes entre si. Tudo original, desenhado por código.
import math

import numpy as np

MAGENTA = (255, 0, 255)


class Model:
    """Conjunto de voxels: dict (x, y, z) inteiros -> cor (r, g, b)."""

    def __init__(self):
        self.v = {}

    def put(self, x, y, z, c):
        self.v[(int(round(x)), int(round(y)), int(round(z)))] = c

    def merge(self, other):
        self.v.update(other.v)
        return self


def _fill(model, xs, ys, zs, inside, color, xf=None):
    for x in np.arange(xs[0], xs[1] + 0.001, 0.5):
        for y in np.arange(ys[0], ys[1] + 0.001, 0.5):
            for z in np.arange(zs[0], zs[1] + 0.001, 0.5):
                if inside(x, y, z):
                    c = color(x, y, z) if callable(color) else color
                    if c is None:
                        continue
                    p = (x, y, z) if xf is None else xf((x, y, z))
                    model.put(*p, c)


def box(m, x0, x1, y0, y1, z0, z1, color, xf=None):
    _fill(m, (x0, x1), (y0, y1), (z0, z1), lambda x, y, z: True, color, xf)


def ellipsoid(m, cx, cy, cz, rx, ry, rz, color, xf=None):
    _fill(
        m,
        (cx - rx, cx + rx),
        (cy - ry, cy + ry),
        (cz - rz, cz + rz),
        lambda x, y, z: ((x - cx) / rx) ** 2 + ((y - cy) / ry) ** 2 + ((z - cz) / rz) ** 2 <= 1.0,
        color,
        xf,
    )


def cyl_z(m, cx, cy, r, z0, z1, color, xf=None):
    _fill(m, (cx - r, cx + r), (cy - r, cy + r), (z0, z1), lambda x, y, z: (x - cx) ** 2 + (y - cy) ** 2 <= r * r, color, xf)


def cyl_y(m, cx, cz, r, y0, y1, color, xf=None):
    _fill(m, (cx - r, cx + r), (y0, y1), (cz - r, cz + r), lambda x, y, z: (x - cx) ** 2 + (z - cz) ** 2 <= r * r, color, xf)


def cyl_x(m, cy, cz, r, x0, x1, color, xf=None):
    _fill(m, (x0, x1), (cy - r, cy + r), (cz - r, cz + r), lambda x, y, z: (y - cy) ** 2 + (z - cz) ** 2 <= r * r, color, xf)


def rot_x(pivot, deg):
    """Rodar à volta do eixo x. Com -deg, o que está por baixo do pivô vai
    para -y (para a frente): perna/braço a avançar."""
    a = math.radians(deg)
    ca, sa = math.cos(a), math.sin(a)
    px, py, pz = pivot

    def f(p):
        x, y, z = p
        dy, dz = y - py, z - pz
        return (x, py + dy * ca - dz * sa, pz + dy * sa + dz * ca)

    return f


def rot_y(pivot, deg):
    a = math.radians(deg)
    ca, sa = math.cos(a), math.sin(a)
    px, py, pz = pivot

    def f(p):
        x, y, z = p
        dx, dz = x - px, z - pz
        return (px + dx * ca + dz * sa, y, pz - dx * sa + dz * ca)

    return f


def rot_z(pivot, deg):
    a = math.radians(deg)
    ca, sa = math.cos(a), math.sin(a)
    px, py, pz = pivot

    def f(p):
        x, y, z = p
        dx, dy = x - px, y - py
        return (px + dx * ca - dy * sa, py + dx * sa + dy * ca, z)

    return f


def chain(*fs):
    def f(p):
        for g in fs:
            p = g(p)
        return p

    return f


def transformed(model, xf):
    # cada voxel leva 8 sub-pontos, para a rotação não deixar buracos
    out = Model()
    for (x, y, z), c in model.v.items():
        for dx in (-0.3, 0.3):
            for dy in (-0.3, 0.3):
                for dz in (-0.3, 0.3):
                    out.put(*xf((x + dx, y + dy, z + dz)), c)
    return out


LIGHT = np.array([-0.45, -0.75, 0.55])
LIGHT = LIGHT / np.linalg.norm(LIGHT)

_NEI = [(dx, dy, dz) for dx in (-1, 0, 1) for dy in (-1, 0, 1) for dz in (-1, 0, 1) if (dx, dy, dz) != (0, 0, 0)]


def render(model, size=(128, 128), yaw=0.0, pitch=12.0, px=2.0, anchor=None, outline=(24, 20, 28),
           emissive=(), ground_shadow=True, center=False):
    """Projeção ortográfica. O modelo olha para -y; yaw roda o modelo
    (90 = virado para a direita do ecrã). anchor = (x, y) do ponto (0,0,0)
    no ecrã. emissive: cores que não levam sombreado (luzes, olhos)."""
    W, H = size
    if anchor is None:
        anchor = (W / 2, H - 2)
    pts = np.array(list(model.v.keys()), dtype=float)
    cols = np.array(list(model.v.values()), dtype=float)
    keyset = model.v
    # normal aproximada: média das direções para vizinhos vazios
    normals = np.zeros_like(pts)
    for i, (x, y, z) in enumerate(model.v.keys()):
        n = [0.0, 0.0, 0.0]
        for dx, dy, dz in _NEI:
            if (x + dx, y + dy, z + dz) not in keyset:
                n[0] += dx
                n[1] += dy
                n[2] += dz
        normals[i] = n
    ln = np.linalg.norm(normals, axis=1, keepdims=True)
    ln[ln == 0] = 1
    normals = normals / ln
    a = math.radians(yaw)
    ca, sa = math.cos(a), math.sin(a)
    rx = pts[:, 0] * ca - pts[:, 1] * sa
    ry = pts[:, 0] * sa + pts[:, 1] * ca
    nx = normals[:, 0] * ca - normals[:, 1] * sa
    ny = normals[:, 0] * sa + normals[:, 1] * ca
    nz = normals[:, 2]
    # luz fixa em relação ao ecrã (vem de cima, à esquerda, da frente)
    lam = np.clip(nx * LIGHT[0] + ny * LIGHT[1] + nz * LIGHT[2], -1, 1)
    shade = 0.55 + 0.55 * np.clip(lam, 0, 1) - 0.12 * np.clip(-lam, 0, 1)
    emis = np.zeros(len(pts), dtype=bool)
    for e in emissive:
        emis |= np.all(cols == np.array(e, dtype=float), axis=1)
    shade[emis] = 1.0
    shaded = np.clip(cols * shade[:, None], 0, 255)
    if center:
        anchor = (W / 2 - (rx.min() + rx.max()) / 2 * px, anchor[1])
    p = math.radians(pitch)
    sx = anchor[0] + rx * px
    sy = anchor[1] - (pts[:, 2] * math.cos(p) + ry * math.sin(p)) * px
    depth = ry * math.cos(p) - pts[:, 2] * math.sin(p)
    img = np.zeros((H, W, 3), dtype=float)
    zb = np.full((H, W), np.inf)
    s = int(math.ceil(px))
    order = np.argsort(depth)[::-1]  # longe primeiro
    for i in order:
        x0 = int(math.floor(sx[i] - px / 2))
        y0 = int(math.floor(sy[i] - px / 2))
        for yy in range(y0, y0 + s + (1 if px % 1 else 0)):
            if yy < 0 or yy >= H:
                continue
            for xx in range(x0, x0 + s + (1 if px % 1 else 0)):
                if 0 <= xx < W and depth[i] <= zb[yy, xx]:
                    zb[yy, xx] = depth[i]
                    img[yy, xx] = shaded[i]
    mask = np.isfinite(zb)
    out = np.zeros((H, W, 4), dtype=np.uint8)
    out[..., :3] = img.astype(np.uint8)
    out[..., 3] = mask * 255
    if ground_shadow:
        out = add_shadow(out, model, yaw, pitch, px, anchor)
    out[..., 3] = np.where(out[..., 3] > 0, 255, 0).astype(np.uint8)
    if outline is not None:
        out = add_outline(out, outline)
    return out


def add_shadow(rgba, model, yaw, pitch, px, anchor):
    """Sombra elíptica suave no chão, por baixo do modelo."""
    pts = np.array(list(model.v.keys()), dtype=float)
    a = math.radians(yaw)
    rx = pts[:, 0] * math.cos(a) - pts[:, 1] * math.sin(a)
    ry = pts[:, 0] * math.sin(a) + pts[:, 1] * math.cos(a)
    p = math.radians(pitch)
    cx = anchor[0] + (rx.min() + rx.max()) / 2 * px
    hw = (rx.max() - rx.min()) / 2 * px + 3
    cy = anchor[1] - ((ry.min() + ry.max()) / 2) * math.sin(p) * px
    hh = max(2.0, (ry.max() - ry.min()) / 2 * math.sin(p) * px + 1.5)
    H, W = rgba.shape[:2]
    out = rgba.copy()
    for y in range(H):
        for x in range(W):
            d = ((x - cx) / hw) ** 2 + ((y - cy) / hh) ** 2
            if d <= 1 and out[y, x, 3] == 0:
                out[y, x] = (18, 16, 22, 255)
    return out


def add_outline(rgba, color):
    a = rgba[..., 3] > 0
    H, W = a.shape
    edge = np.zeros_like(a)
    for dy, dx in ((1, 0), (-1, 0), (0, 1), (0, -1)):
        sh = np.zeros_like(a)
        ys = slice(max(0, dy), H + min(0, dy))
        yd = slice(max(0, -dy), H + min(0, -dy))
        xs = slice(max(0, dx), W + min(0, dx))
        xd = slice(max(0, -dx), W + min(0, -dx))
        sh[yd, xd] = a[ys, xs]
        edge |= sh & ~a
    out = rgba.copy()
    out[edge] = (*color, 255)
    return out
