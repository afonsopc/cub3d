#!/usr/bin/env python3
# Gera os mapas de teste da IA (tests/ai/maps) com as definições do
# missao1.cub e plantas pequenas, cada uma para um comportamento.
import os
import re

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
    # rádio: mensagem ao chegar a um sítio, ao ver um atirador, com pouca vida
    "radio": [
        "fffffffff",
        "f       f",
        "f   e   f",
        "f       f",
        "f       f",
        "f   N   f",
        "fffffffff",
    ],
    # puzzle (jogado com teclas): o portão V só abre pela alavanca >, a
    # porta D só com a chave 3, e atrás dela estão os documentos 4
    "puzzle": [
        "fffffffff",
        "f   3   f",
        "f       f",
        "ffffVffff",
        "f       f",
        ">   N   f",
        "ffffDffff",
        "f   4   f",
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
    # sem os ecrãs da história (introdução, ordens, rádio): só a IA
    prefixes = ("LEVEL_NAME", "INTRO_", "BRIEFING_", "STATS_SPRITE", "ENDING_", "RADIO_", "OBJECTIVE_")
    head = "\n".join(l for l in head.split("\n") if not l.startswith(prefixes))
    head = head.replace("MAX_HEALTH 100\n", "MAX_HEALTH 100000\n")
    head = re.sub(r"\nAMMO \d+\n", "\nAMMO 99\n", head)
    os.makedirs(os.path.join(HERE, "maps"), exist_ok=True)
    for name, rows in GRIDS.items():
        extra = ""
        if name == "radio":
            h = head.replace("MAX_HEALTH 100000\n", "MAX_HEALTH 100\n")
            extra = ("\nRADIO_PORTRAIT_SPRITE assets/oms3d/story/radio-major.bmp\n"
                     "RADIO_1_TEXT teste inicio\n"
                     "RADIO_2_TEXT teste sitio\nRADIO_2_AT 4 5\n"
                     "RADIO_3_TEXT teste atirador\nRADIO_3_SEE e\n"
                     "RADIO_4_TEXT teste vida\nRADIO_4_HEALTH 90\n")
        else:
            h = head
        with open(os.path.join(HERE, "maps", f"{name}.cub"), "w") as f:
            f.write(h + extra + "\nF 100, 100, 100\nC 50, 50, 50\n\n" + "\n".join(rows) + "\n")
    print("ok", ", ".join(GRIDS))


if __name__ == "__main__":
    main()
