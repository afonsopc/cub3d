#!/usr/bin/env python3
# Hino da Mocidade Portuguesa, marcha em 4/4, escrita em sol maior.
# Transcrita de ouvido da gravação de referência (coro antigo com muito ruído;
# média da 2.ª e 3.ª estrofes, que se repetem a cada 40 compassos); não há
# partitura pública. A gravação está em fá sustenido (-1). Estrofe de 40
# compassos: A (subida até à dominante), B (descida por graus), C, D.
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import chip  # noqa: E402

TITLE = 'Hino da Mocidade Portuguesa'
BPM = 132.5
BAR = 16
TRANSPOSE_TO_REF = -1  # tom da gravação = escrito + isto
METHOD = 'ear'

SECTIONS = {
    'A': [
        'G4:12 G#4:4', 'G#4:8 G4:8', 'G4:12 A4:4', 'A4:2 E4:10 G4:4',
        'G4:12 A4:4', 'A4:2 B4:14', 'C5:4 B4:4 D5:8', 'D5:12 B4:4',
        'D5:12 G4:4', 'G4:16',
    ],
    'B': [
        'G4:10 E4:6', 'E4:6 F4:4 E4:2 F4:4', 'F4:4 E4:12', 'E4:6 D4:10',
        'D4:12 G3:4', 'C4:16', 'G4:6 A4:4 G4:4 E4:2', 'E4:8 F4:8',
        'F4:8 E4:4 D4:4', 'D4:4 C4:12',
    ],
    'C': [
        'C4:8 A3:8', 'A3:16', 'B3:8 E4:8', 'E4:12 C4:4',
        'C4:12 D4:4', 'D4:12 E4:4', 'E4:8 A3:4 F4:4', 'F4:4 A3:12',
        'A3:4 D4:12', 'F4:4 E4:4 D4:4 C4:4',
    ],
    'D': [
        'A3:12 C4:4', 'A3:12 B3:4', 'G4:16', 'E4:16',
        'E4:8 C4:4 E4:4', 'E4:4 A4:12', 'G4:6 D4:6 G4:4', 'B3:2 D4:14',
        'G3:4 E4:4 D4:4 B3:2 C4:2', 'G4:16',
    ],
}
CHORDS = {
    'A': [
        'G', 'G', 'G', 'Em',
        'G', 'Am', 'Em|D7', 'D7',
        'G', 'G',
    ],
    'B': [
        'C', 'F', 'C', 'Em|D7',
        'G', 'C', 'C', 'F',
        'F|D7', 'C',
    ],
    'C': [
        'C|Am', 'Am', 'Em', 'C',
        'C', 'D7', 'Am', 'F',
        'D7', 'F|D7',
    ],
    'D': [
        'Am', 'Am', 'C', 'C',
        'C', 'Am', 'C|G', 'D7',
        'G|D7', 'G',
    ],
}
FORM = ['A', 'B', 'C', 'D']
REF_START = 94.42  # segundos na gravação de referência (só para a verificação)
CONFIDENCE = 'Weakest of the two: old, noisy choir record with little bass. Tempo, 40-bar strophe length, tonal centre (F# in the record) and the contour of phrases A (rise to the held dominant) and B (stepwise descent) are fairly solid; rhythms inside bars are simplified to long notes; octave choices in C/D and the chromatic G-naturals in A are uncertain; chords are inferred from the melody, not heard.'


def build(preview_dir=None):
    return chip.render(sys.modules[__name__], 'hino-mocidade', preview_dir=preview_dir, lead_octave=1)


if __name__ == "__main__":
    print(build(sys.argv[1] if len(sys.argv) > 1 else None))
