#!/usr/bin/env python3
# "A Portuguesa" (Alfredo Keil, 1890), na versão oficial de 1957 (Estado
# Novo). Melodia do canto conferida com a gravação de referência e com o
# MIDI da Wikimedia Commons ("A portuguesa.mid", versão oficial); o original
# está em mi bemol maior, aqui escrito em dó maior (+3 = tom da gravação).
#
# Correções face à transcrição de 1910 usada em music-mega:
#   - o coro começa na tónica aguda ("ÀS AR-mas" = dó5 si4), não em lá4;
#   - "pela PÁ-tria lu-tar" sobe ao mi5 (não ré5) e repousa no dó5;
#   - "Contra os canhões" = lá lá sol fá sol | dó (versão de 1957);
#   - acrescentada a 2.ª metade da estrofe ("Entre as brumas da memória ...
#     Que há-de guiar-te à vitória"), com a passagem em dó menor.
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import chip  # noqa: E402

TITLE = "A Portuguesa (versão oficial de 1957)"
BPM = 103
BAR = 16
TRANSPOSE_TO_REF = 3
METHOD = "score"

SECTIONS = {
    # estrofe: "Heróis do mar, nobre povo ... O esplendor de Portugal!"
    "V1": [
        "r:11 G4:1 E4:3 C4:1",
        "A4:8 G4:7 A4:1",
        "G4:4 F4:4 E4:3 C4:1 B3:3 C4:1",
        "E4:8 F#4:3 G4:1 A4:3 B4:1",
        "G4:8 r:4 G4:3 B4:1",
        "D5:8 B4:4 G4:2 F4:2",
        "F4:8 E4:3 E4:1 G4:3 E4:1",
        "C4:8 C4:2 D4:2 E4:2 G4:2",
        "D4:8 r:8",
    ],
    # "Entre as brumas da memória, Ó Pátria, sente-se a voz ... à vitória!"
    "V2": [
        "r:6 E4:2 C4:4 C4:2 E4:2",
        "F4:6 G4:2 E4:4 E4:2 G4:2",
        "C5:6 G4:2 G4:2 Bb4:4 Ab4:2",
        "G4:4 D4:10 F4:2",
        "Eb4:4 G4:4 G4:4 F4:2 Eb4:2",
        "D4:14 F4:2",
        "Eb4:4 G4:3 G4:1 G4:4 F4:2 Eb4:2",
        "G4:6 r:2 G4:7 G4:1",
    ],
    # coro: "Às armas, às armas! ... Contra os canhões marchar, marchar!"
    "C": [
        "C5:8 B4:7 F4:1",
        "A4:8 G4:6 D4:2",
        "E4:4 E4:4 G4:4 E4:4",
        "D4:12 r:3 G4:1",
        "C5:8 B4:7 F4:1",
        "A4:8 G4:4 E4:1 G4:1 C5:2",
        "E5:8 D5:7 C5:1",
        "C5:10 E4:2 F4:2 G4:2",
        "A4:4 A4:4 G4:4 F4:2 G4:2",
        "C5:12 r:4",
    ],
}
CHORDS = {
    "V1": ["G", "Dm|Em", "G7|Am", "Am|D7", "G", "G7", "Dm|C", "C", "G"],
    "V2": ["G7|C", "G7|C", "Cm", "G|G7", "Cm", "G7", "Cm", "G"],
    "C": ["C|G7", "F|C", "C", "G", "C|G7", "F|C", "C|G7", "Am", "Dm|G7", "C"],
}
FORM = ["V1", "V2", "C"]
HARMONY = ["C"]  # coro com a melodia na oitava de cima e terceiras por baixo
REF_START = 13.97  # compasso de anacruse "He-" na gravação (s2, já afinada a 440)
CONFIDENCE = "melodia da partitura oficial (MIDI) conferida na gravação: coro e estrofe batem; 2.ª metade da estrofe só pela partitura"


def build(preview_dir=None):
    return chip.render(sys.modules[__name__], "a-portuguesa", preview_dir=preview_dir, lead_octave=1, intro_bars=0)


if __name__ == "__main__":
    print(build(sys.argv[1] if len(sys.argv) > 1 else None))
