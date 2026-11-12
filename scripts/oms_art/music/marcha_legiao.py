#!/usr/bin/env python3
# Marcha da Legião Portuguesa ("A Legião em Marcha"), 4/4, lá maior (tom da
# gravação). Transcrita de ouvido: tema dos cornetins (T, notas repetidas em
# colcheias) alternado com a estrofe do coro (V). A gravação faz T V T V T V T V
# T; aqui ficam T V T V T para caber nos 60-120 s, com o 2.º V em terceiras.
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import chip  # noqa: E402

TITLE = 'Marcha da Legião Portuguesa'
BPM = 135.2
BAR = 16
TRANSPOSE_TO_REF = 0  # tom da gravação = escrito + isto
METHOD = 'ear'

SECTIONS = {
    'T': [
        'A4:2 A4:2 A4:4 E4:2 E4:6', 'F#4:2 F#4:2 F#4:4 E4:8', 'D5:2 D5:2 D5:4 C#5:2 C#5:2 C#5:2 A4:2', 'B4:4 B4:4 E5:8',
        'A4:2 A4:2 A4:4 E4:2 E4:6', 'F#4:2 F#4:2 F#4:4 E4:8', 'D5:2 D5:2 D5:4 C#5:4 C#5:4', 'B4:4 G#4:2 E4:2 A4:8',
    ],
    'V': [
        'A4:4 E5:4 C#5:4 A4:2 C#5:2', 'B4:2 A4:2 G#4:2 B4:2 A4:8', 'F#4:4 D5:4 C#5:4 E5:4', 'C#5:8 B4:8',
        'A4:4 E5:4 C#5:4 A4:2 C#5:2', 'B4:2 A4:2 G#4:2 B4:2 A4:8', 'F#4:4 D5:4 C#5:4 E5:4', 'C#5:4 B4:4 A4:8',
    ],
}
CHORDS = {
    'T': [
        'A', 'Bm|A', 'D|A', 'E',
        'A', 'Bm|A', 'D|A', 'E|A',
    ],
    'V': [
        'A', 'E|A', 'D|A', 'A|E',
        'A', 'E|A', 'D|A', 'E|A',
    ],
}
SECTIONS['V2'] = SECTIONS['V']
CHORDS['V2'] = CHORDS['V']
HARMONY = ['V2']  # 2.ª estrofe com terceiras por baixo
FORM = ['T', 'V', 'T', 'V2', 'T']  # a gravação: T V T V T V T V T
REF_START = 0.24  # segundos na gravação de referência (só para a verificação)
CONFIDENCE = 'Tema (T) sólido: notas e forma claras na banda e repetidas 5 vezes. Estrofe (V) cantada por coro masculino grave: alturas bem audíveis, ritmo exacto de colcheias/semínimas aproximado; acordes deduzidos do croma (baixo pouco legível).'


def build(preview_dir=None):
    return chip.render(sys.modules[__name__], 'marcha-legiao', preview_dir=preview_dir)


if __name__ == "__main__":
    print(build(sys.argv[1] if len(sys.argv) > 1 else None))
