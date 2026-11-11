# Paleta própria de 255 cores ao estilo VGA do início dos anos 90 (rampas de
# cor do escuro ao claro, como nos jogos de 320x200 daquela época). Não é a
# paleta de nenhum jogo: as rampas são escolhidas aqui. O índice 255 fica
# reservado para o magenta transparente.
import numpy as np

# (nome, âncoras escuro -> meio -> claro)
RAMPS = [
    ("grey", (6, 6, 8), (110, 110, 114), (250, 250, 250)),
    ("stone", (14, 12, 10), (118, 112, 100), (236, 228, 210)),
    ("wood", (16, 8, 4), (128, 78, 40), (236, 196, 140)),
    ("skin", (34, 14, 8), (196, 132, 96), (255, 226, 196)),
    ("red", (24, 0, 0), (176, 22, 22), (255, 150, 130)),
    ("blood", (20, 0, 2), (120, 8, 10), (210, 40, 40)),
    ("gold", (30, 18, 0), (204, 150, 30), (255, 240, 160)),
    ("orange", (30, 10, 0), (220, 110, 20), (255, 220, 150)),
    ("green", (0, 18, 6), (30, 120, 50), (180, 236, 170)),
    ("olive", (12, 14, 4), (92, 100, 46), (210, 216, 150)),
    ("blue", (0, 6, 30), (30, 70, 170), (190, 214, 255)),
    ("steel", (8, 12, 18), (86, 102, 118), (210, 222, 232)),
    ("khaki", (20, 16, 8), (150, 132, 88), (240, 230, 190)),
    ("brick", (20, 6, 4), (140, 60, 40), (240, 170, 140)),
    ("purple", (12, 4, 18), (96, 56, 120), (220, 190, 236)),
]

N = 17  # 15 rampas x 17 = 255


def _ramp(d, m, l, n=N):
    d, m, l = (np.array(x, float) for x in (d, m, l))
    out = []
    for i in range(n):
        t = i / (n - 1)
        if t < 0.55:
            c = d + (m - d) * (t / 0.55)
        else:
            c = m + (l - m) * ((t - 0.55) / 0.45)
        out.append(c)
    return out


def build():
    cols = []
    for _, d, m, l in RAMPS:
        cols += _ramp(d, m, l)
    pal = np.clip(np.array(cols), 0, 255).astype(np.uint8)
    assert len(pal) == 255
    return pal


PAL = build()
_PALF = PAL.astype(np.float32)
# pesos perceptuais simples
_W = np.array([0.30, 0.59, 0.11], np.float32) * 3


def snap(rgb):
    """rgb: HxWx3 -> índices HxW (0..254) da cor mais próxima."""
    a = np.asarray(rgb, np.float32).reshape(-1, 3)
    out = np.empty(len(a), np.uint8)
    step = 8192
    for i in range(0, len(a), step):
        d = ((a[i : i + step, None, :] - _PALF[None]) ** 2 * _W).sum(-1)
        out[i : i + step] = d.argmin(1)
    return out.reshape(np.asarray(rgb).shape[:2])


def flat():
    """Paleta de 256 cores (lista plana para o PIL), 255 = magenta."""
    p = PAL.reshape(-1).tolist() + [255, 0, 255]
    return p


def apply(rgb):
    """Devolve a imagem já com as cores da paleta (HxWx3)."""
    return PAL[snap(rgb)]
