# Letra de píxeis 3x5 (maiúsculas, algarismos e acentos portugueses), para
# rótulos, cartazes e a barra de estado. Desenhada aqui.
import numpy as np

G = {
    "A": "010 101 111 101 101", "B": "110 101 110 101 110", "C": "011 100 100 100 011",
    "D": "110 101 101 101 110", "E": "111 100 110 100 111", "F": "111 100 110 100 100",
    "G": "011 100 101 101 011", "H": "101 101 111 101 101", "I": "111 010 010 010 111",
    "J": "001 001 001 101 010", "K": "101 101 110 101 101", "L": "100 100 100 100 111",
    "M": "10001 11011 10101 10001 10001", "N": "1001 1101 1011 1001 1001",
    "O": "010 101 101 101 010", "P": "110 101 110 100 100", "Q": "010 101 101 110 011",
    "R": "110 101 110 101 101", "S": "011 100 010 001 110", "T": "111 010 010 010 010",
    "U": "101 101 101 101 111", "V": "101 101 101 101 010", "W": "10001 10001 10101 11011 10001",
    "X": "101 101 010 101 101", "Y": "101 101 010 010 010", "Z": "111 001 010 100 111",
    "0": "111 101 101 101 111", "1": "010 110 010 010 111", "2": "110 001 010 100 111",
    "3": "110 001 010 001 110", "4": "101 101 111 001 001", "5": "111 100 110 001 110",
    "6": "011 100 111 101 111", "7": "111 001 010 010 010", "8": "111 101 111 101 111",
    "9": "111 101 111 001 110", "!": "1 1 1 0 1", ".": "0 0 0 0 1", ",": "00 00 00 01 10",
    "·": "0 0 1 0 0", "-": "000 000 111 000 000", "/": "001 001 010 100 100", " ": "00 00 00 00 00",
    "'": "1 1 0 0 0", ":": "0 1 0 1 0",
}
ACUTE = [(2, -2), (1, -1)]
TILDE = [(1, -2), (2, -2), (0, -1), (1, -1)]
CIRC = [(1, -2), (0, -1), (2, -1)]
ACC = {
    "Á": ("A", ACUTE), "É": ("E", ACUTE), "Í": ("I", ACUTE), "Ó": ("O", ACUTE), "Ú": ("U", ACUTE),
    "Ã": ("A", TILDE), "Õ": ("O", TILDE), "Ê": ("E", CIRC), "Â": ("A", CIRC), "Ô": ("O", CIRC),
    "Ç": ("C", [(1, 5), (0, 6)]),
}


def glyph(c):
    base, marks = ACC.get(c, (c, []))
    rows = G[base].split(" ")
    return rows, marks


def text_width(s, scale=1):
    return sum((len(glyph(c)[0][0]) + 1) * scale for c in s) - scale


def draw_text(img, s, x, y, scale=1, color=(255, 255, 255), shadow=None):
    img = np.asarray(img)
    H, W = img.shape[:2]

    def dot(px, py, col):
        y0, x0 = y + py * scale, cx + px * scale
        if 0 <= y0 and y0 + scale <= H and 0 <= x0 and x0 + scale <= W:
            img[y0 : y0 + scale, x0 : x0 + scale] = col

    cx = x
    for c in s:
        rows, marks = glyph(c)
        pts = [(gx, gy) for gy, r in enumerate(rows) for gx, b in enumerate(r) if b == "1"] + marks
        if shadow is not None:
            for gx, gy in pts:
                y0, x0 = y + gy * scale + 1, cx + gx * scale + 1
                if 0 <= y0 and y0 + scale <= H and 0 <= x0 and x0 + scale <= W:
                    img[y0 : y0 + scale, x0 : x0 + scale] = shadow
        for gx, gy in pts:
            dot(gx, gy, color)
        cx += (len(rows[0]) + 1) * scale
    return img


def centered(img, s, cx, y, scale=1, color=(255, 255, 255), shadow=None):
    return draw_text(img, s, int(cx - text_width(s, scale) / 2 + 0.5), y, scale, color, shadow)
