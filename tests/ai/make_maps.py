#!/usr/bin/env python3
# Gera os mapas de teste da IA (tests/ai/maps) com as definições do
# missao1.cub e plantas pequenas, cada uma para um comportamento.
import os

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", ".."))

GRIDS = {
    # tiro na sala A: o guarda em B (ligada por uma abertura) ouve; o guarda
    # em C, atrás de uma porta fechada, não ouve
    "hear_closed": [
        "fffffffffffffffff",
        "f    f    f     f",
        "f  N      v  e  f",
        "f    f  e f     f",
        "fffffffffffffffff",
    ],
    # o jogador abre a porta (E) e dispara: agora o guarda em C ouve
    "hear_open": [
        "fffffffffffffffff",
        "f    f    f     f",
        "f        Ev  e  f",
        "f    f  e f     f",
        "fffffffffffffffff",
    ],
    # o som chega a B pelo corredor; o caminho mais curto é pela porta h,
    # que o guarda tem de abrir
    "door": [
        "fffffffff",
        "f N     f",
        "f fffhf f",
        "f f   f f",
        "f f e f f",
        "f f   f f",
        "f       f",
        "fffffffff",
    ],
    # quatro guardas e um cão contornam colunas até ao jogador
    "crowd": [
        "fffffffffffffff",
        "f      f      f",
        "f e  e f  \\   f",
        "f             f",
        "f e  d f  \\   f",
        "f      f   N  f",
        "fffffffffffffff",
    ],
}


def main():
    src = open(os.path.join(ROOT, "maps", "missao1.cub")).read()
    head = src[: src.index("\nF 100")]
    head = head.replace("MAX_HEALTH 100\n", "MAX_HEALTH 100000\n")
    head = head.replace("AMMO 8\n", "AMMO 99\n")
    os.makedirs(os.path.join(HERE, "maps"), exist_ok=True)
    for name, rows in GRIDS.items():
        with open(os.path.join(HERE, "maps", f"{name}.cub"), "w") as f:
            f.write(head + "\nF 100, 100, 100\nC 50, 50, 50\n\n" + "\n".join(rows) + "\n")
    print("ok", ", ".join(GRIDS))


if __name__ == "__main__":
    main()
