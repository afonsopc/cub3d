#!/usr/bin/env python3
# "Angola é nossa!" (1961), marcha em 4/4, escrita em dó maior (a gravação
# está em mi bemol, +3). Transcrita de ouvido da gravação de referência; não
# há partitura pública. Forma: coro (que a banda também toca na introdução),
# estrofe em sequência ascendente (V), 2.ª metade da estrofe duas vezes (Q, Qb),
# coro e o remate final (E), que repousa na tónica e volta ao início.
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import chip  # noqa: E402

TITLE = 'Angola é nossa!'
BPM = 126.0
BAR = 16
TRANSPOSE_TO_REF = 3  # tom da gravação = escrito + isto
METHOD = 'ear'

SECTIONS = {
    'C': [
        'C4:12 G3:4', 'A4:12 E4:2 C4:2', 'F4:14 C4:2', 'D4:12 C4:2 D4:2',
        'E4:4 C4:4 F4:8', 'E4:4 C4:4 F4:4 C5:4', 'E4:2 G4:2 G3:4 E4:8', 'F4:10 A3:6',
    ],
    'V': [
        'G3:4 G3:2 C4:2 C4:8', 'G3:4 G3:2 C4:2 C4:2 C4:2 E4:2 G4:2', 'G4:4 E4:4 G3:4 D4:4', 'D4:2 C4:4 G3:2 G3:2 A3:2 B3:2 C4:2',
        'C4:4 D4:2 E4:2 C4:2 D4:2 E4:2 F4:2', 'F4:2 G4:4 A4:2 A3:2 B3:2 C#4:2 D4:2', 'D4:4 E4:2 F#4:2 D4:2 E4:2 F#4:2 G4:2', 'G4:4 A4:2 B4:2 C5:8',
    ],
    'Q': [
        'C5:8 B4:4 A4:2 F4:2', 'E4:2 G4:4 C4:4 D4:4 F4:2', 'A4:4 F4:4 G4:4 F4:2 E4:2', 'C4:2 E4:6 C4:4 D4:2 E4:2',
        'F4:4 A4:4 G4:4 F4:2 E4:2', 'E4:4 C4:2 E4:2 E4:4 E4:2 F4:2', 'D4:12 C4:4', 'G4:12 C4:2 B4:2',
    ],
    'Qb': [
        'C5:8 B4:4 A4:2 E4:2', 'G4:6 C4:4 D4:4 F4:2', 'F4:2 A4:2 F4:4 F4:2 G4:2 F4:4', 'C4:2 E4:6 C4:4 D4:2 E4:2',
        'F4:4 A4:4 G4:4 F4:2 E4:2', 'E4:4 C4:4 A4:4 F4:2 G4:2', 'E4:8 F4:2 D4:2 G3:4',
    ],
    'E': [
        'D4:8 F4:2 D4:6', 'F4:8 A4:4 D4:4', 'C5:4 F4:2 D4:4 A3:2 D4:4', 'C5:2 F#4:2 D4:4 A3:4 D4:4',
        'D4:4 A4:4 F#4:2 C5:2 D5:2 B4:2', 'E5:16',
    ],
}
CHORDS = {
    'C': [
        'C', 'Am', 'F', 'Dm|G7',
        'C|F', 'C|F', 'C|G7', 'F',
    ],
    'V': [
        'C', 'C', 'C|G7', 'G7|C',
        'C', 'F|A7', 'D7', 'G7',
    ],
    'Q': [
        'F|G7', 'C|G7', 'F|C', 'C|Am',
        'Dm|G7', 'C', 'G7', 'G7',
    ],
    'Qb': [
        'F|G7', 'C|G7', 'F|C', 'C|Am',
        'Dm|G7', 'C', 'C|G7',
    ],
    'E': [
        'Dm', 'Dm', 'Dm', 'D7',
        'D7|G7', 'C',
    ],
}
FORM = ['C', 'V', 'Q', 'Qb', 'C', 'E']
REF_START = 32.81  # segundos na gravação de referência (só para a verificação)
CONFIDENCE = 'Key (Eb major in the record), tempo, bar grid and form (chorus-intro, 8-bar rising verse, 8+7-bar second verse half, chorus, F7-Bb7-Eb tag) are solid. Pitch classes of the chorus long notes and of the verse sequence are solid; octave of the verse scale run (bar V6-V7) and the inner 8th-notes of Q bars 3-6 and chorus bars 5-8 are best guesses (band + choir in octaves/thirds, separated vocals were too weak).'


def build(preview_dir=None):
    return chip.render(sys.modules[__name__], 'angola-e-nossa', preview_dir=preview_dir, lead_octave=1)


if __name__ == "__main__":
    print(build(sys.argv[1] if len(sys.argv) > 1 else None))
