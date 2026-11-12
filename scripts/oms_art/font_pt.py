#!/usr/bin/env python3
# Acrescenta à letra IBM VGA 8x16 do jogo (assets/fonts/IBM-VGA-8x16, um BMP
# por byte) as letras acentuadas do português, nas posições Latin-1: o texto
# dos mapas está em UTF-8 e o jogo converte-o para Latin-1 ao desenhar
# (story_text). Cada letra é a letra base com o acento desenhado por cima.
import os

import numpy as np
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
DIR = os.path.join(ROOT, "assets", "fonts", "IBM-VGA-8x16")

ACUTE = [(8, 0), (9, 0), (7, 1), (8, 1)]
GRAVE = [(6, 0), (7, 0), (7, 1), (8, 1)]
CIRC = [(7, 0), (8, 0), (6, 1), (9, 1)]
TILDE = [(5, 1), (6, 0), (7, 0), (8, 1), (9, 1), (10, 0)]
DIAER = [(5, 1), (6, 1), (9, 1), (10, 1)]

LETTERS = {
    "À": ("A", GRAVE), "Á": ("A", ACUTE), "Â": ("A", CIRC), "Ã": ("A", TILDE),
    "É": ("E", ACUTE), "Ê": ("E", CIRC), "Í": ("I", ACUTE), "Ó": ("O", ACUTE),
    "Ô": ("O", CIRC), "Õ": ("O", TILDE), "Ú": ("U", ACUTE), "Ü": ("U", DIAER),
    "à": ("a", GRAVE), "á": ("a", ACUTE), "â": ("a", CIRC), "ã": ("a", TILDE),
    "é": ("e", ACUTE), "ê": ("e", CIRC), "í": ("i", ACUTE), "ó": ("o", ACUTE),
    "ô": ("o", CIRC), "õ": ("o", TILDE), "ú": ("u", ACUTE), "ü": ("u", DIAER),
}


def load(ch):
    a = np.array(Image.open(os.path.join(DIR, f"{ord(ch)}.bmp")).convert("L")) > 128
    return a


def top_row(a):
    rows = np.where(a.any(axis=1))[0]
    return rows.min() if len(rows) else 6


def save(a, code):
    """Mesmo formato dos BMP da letra: cabeçalho do 'a' (paleta com magenta =
    transparente no índice 0 e branco no 1) e 16 linhas de 1 bit, de baixo
    para cima."""
    raw = open(os.path.join(DIR, "97.bmp"), "rb").read()
    off = int.from_bytes(raw[10:14], "little")
    rows = b""
    for r in a[::-1]:
        v = 0
        for x in range(16):
            v = (v << 1) | int(bool(r[x]))
        rows += v.to_bytes(2, "big") + b"\0\0"
    open(os.path.join(DIR, f"{code}.bmp"), "wb").write(raw[:off] + rows)


def main():
    for ch, (base, marks) in LETTERS.items():
        a = load(base).copy()
        if base == "i":
            a[:6] = False  # tira o ponto
        top = 6 if base.islower() else top_row(a)
        y0 = max(0, top - 3)
        for x, y in marks:
            a[y0 + y, x] = True
        save(a, ord(ch))
    for ch, base in (("Ç", "C"), ("ç", "c")):
        a = load(base).copy()
        bottom = np.where(a.any(axis=1))[0].max()
        for x, y in ((7, 1), (8, 2), (7, 3)):
            if bottom + y < 16:
                a[bottom + y, x] = True
        save(a, ord(ch))
    for ch, base in (("º", "o"), ("ª", "a")):
        a = load(base)
        small = np.zeros_like(a)
        rows = np.where(a.any(axis=1))[0]
        b = a[rows.min() : rows.max() + 1]
        small[2 : 2 + b.shape[0]] = b
        small[2 + b.shape[0] + 1, 4:10] = True
        save(small, ord(ch))
    print("ok", len(LETTERS) + 4)


if __name__ == "__main__":
    main()
