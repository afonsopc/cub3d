# Arte de píxeis escrita à mão em texto: cada carácter é uma cor. Usado para
# o retrato, a cara da barra de estado, emblemas e ícones.
import numpy as np

T = None  # transparente


def grid(rows, colors, mirror=False):
    """rows: lista de strings; colors: dict carácter -> (r,g,b) ou None.
    mirror=True: as linhas são a metade esquerda e espelham-se."""
    if mirror:
        rows = [r + r[::-1] for r in rows]
    H, W = len(rows), max(len(r) for r in rows)
    out = np.zeros((H, W, 4), np.uint8)
    for y, r in enumerate(rows):
        for x, ch in enumerate(r):
            c = colors.get(ch)
            if c is None:
                continue
            out[y, x, :3] = c
            out[y, x, 3] = 255
    return out


def paste(dst, src, x, y):
    """dst: HxWx3 float/uint8; src: hxwx4 (alfa 0 = não pinta)."""
    h, w = src.shape[:2]
    H, W = dst.shape[:2]
    for yy in range(h):
        ty = y + yy
        if not 0 <= ty < H:
            continue
        for xx in range(w):
            tx = x + xx
            if 0 <= tx < W and src[yy, xx, 3]:
                dst[ty, tx, :3] = src[yy, xx, :3]
                if dst.shape[2] == 4:
                    dst[ty, tx, 3] = 255
    return dst


def scale(src, k):
    return np.repeat(np.repeat(src, k, axis=0), k, axis=1)
