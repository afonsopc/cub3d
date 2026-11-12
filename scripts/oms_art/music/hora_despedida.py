#!/usr/bin/env python3
# "Na hora da despedida (Adeus!)" (1966), fado-canção cantado por Ada de
# Castro na revista "Tudo à Mostra" (Teatro Maria Vitória); lá menor, 2/4,
# ~127 bpm. Transcrito de ouvido da gravação de referência (voz separada com
# demucs, pYIN + basic-pitch, alinhado às batidas e conferido com a 2.ª
# passagem); não há partitura pública. Escrito na oitava do canto e tocado
# uma oitava acima (lead_octave=1). Ciclo de 82 compassos = uma passagem
# completa da gravação: A (as duas estrofes, "Vais p'ra longe meu amor ...")
# e B (a parte do "Adeus ..."); o fim de B (ré, sol#, lá) leva de volta ao lá
# longo do início, como na gravação, por isso o loop encaixa.
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import chip  # noqa: E402

TITLE = 'Na hora da despedida (Adeus!)'
BPM = 127.4
BAR = 8
TRANSPOSE_TO_REF = 0  # tom da gravação = escrito + isto (lá menor, +17 cents)
METHOD = 'ear'

SECTIONS = {
    'A': [
        'A3:8', 'A3:6 r:2', 'B3:4 C4:4', 'C4:3 B3:2 A3:3',
        'E4:2 F4:4 E4:2', 'E4:8', 'E4:2 r:3 B3:3', 'C4:4 C4:2 B3:2',
        'B3:1 C4:2 E4:5', 'D4:8', 'D4:6 r:2', 'B4:4 B4:4',
        'B4:3 A4:3 G#4:2', 'E4:4 F4:4', 'F4:4 E4:2 D4:2', 'E4:3 E4:1 E4:2 E4:2',
        'E4:3 E4:2 D4:3', 'C4:8', 'C4:6 r:2', 'B3:4 C4:4',
        'C4:3 B3:5', 'F4:4 E4:4', 'E4:8', 'r:2 D4:2 E4:4',
        'G4:3 F4:4 A4:1', 'A4:8', 'A4:6 r:2', 'r:2 B4:6',
        'B4:4 B4:3 A4:1', 'A4:4 A4:4', 'A4:5 G#4:3', 'B4:8',
        'B4:6 r:2', 'r:2 G4:2 G#4:2 A4:2', 'C5:3 B4:2 A4:1 G#4:1 A4:1', 'A4:8',
        'A4:8', 'A4:2 r:2 B3:1 C4:3', 'E4:3 E4:5', 'E4:8',
        'E4:3 r:1 G#3:1 A3:3',
    ],
    'B': [
        'A3:4 B3:2 C4:2', 'F4:2 E4:4 r:2', 'A3:4 r:2 C4:2', 'E4:2 D4:6',
        'r:2 D4:6', 'D4:2 r:4 D4:2', 'D4:1 E4:4 F4:3', 'F4:8',
        'F4:2 r:2 B3:4', 'B3:4 C4:2 D4:2', 'F4:2 E4:4 B3:2', 'B3:8',
        'r:1 F4:3 E4:4', 'r:3 B3:3 C4:2', 'B3:4 r:4', 'r:2 E4:6',
        'r:1 E4:2 F4:2 G4:3', 'G4:2 r:3 A4:3', 'Bb4:2 A4:2 r:4', 'F4:6 r:2',
        'r:3 A4:3 G4:1 F4:1', 'F4:4 A4:4', 'A4:1 G4:2 F4:5', 'E4:4 D#4:2 E4:2',
        'E4:3 F4:5', 'E4:3 G4:5', 'G4:4 F4:4', 'F4:3 E4:5',
        'E4:8', 'E4:2 r:5 A3:1', 'B3:1 C4:3 r:4', 'r:2 E4:6',
        'E4:3 r:5', 'A3:6 r:2', 'C4:1 F4:2 E4:5', 'E4:1 B3:5 C4:1 B3:1',
        'D4:2 F4:1 E4:5', 'E4:4 F4:2 E4:2', 'E4:2 r:2 B3:2 C4:2', 'D4:8',
        'D4:3 G#3:2 G3:2 G#3:1',
    ],
}
CHORDS = {
    'A': [
        'Am', 'Am', 'Am', 'Am',
        'Am', 'Am', 'Am|E7', 'Am',
        'E7', 'E7', 'E7', 'E7',
        'E7', 'E7', 'E7', 'E7',
        'E7', 'Am', 'Am', 'Am',
        'Am|E7', 'Am', 'Am', 'Am',
        'A7', 'Dm', 'Dm', 'E7',
        'E7', 'Am', 'E7', 'E7',
        'E7', 'E7', 'E7|Am', 'Am',
        'Am', 'Am', 'Am', 'Am',
        'Am|E7',
    ],
    'B': [
        'Am', 'Am', 'Am', 'E7',
        'E7', 'E7', 'E7|Dm', 'Dm',
        'Dm|E7', 'E7', 'E7', 'E7',
        'E7', 'E7|Am', 'E7', 'Am',
        'Am', 'C7', 'Dm', 'Dm',
        'Dm', 'G7', 'Dm|G7', 'C',
        'C', 'C', 'C|Dm', 'Dm|E7',
        'E7', 'E7', 'Am', 'Am',
        'Am', 'Am', 'Am|E7', 'E7',
        'E7', 'E7', 'E7', 'E7',
        'E7',
    ],
}
FORM = ['A', 'B']
HARMONY = ['B']  # a parte do "Adeus" com terceiras por baixo
REF_START = 5.77  # segundos na gravação de referência (só para a verificação)
CONFIDENCE = 'MÉDIA. Fado com rubato e ornamentos; tonalidade (lá menor), compasso (2/4, baixo tónica-dominante), andamento (~127 bpm) e a forma (uma passagem de 82 compassos que a gravação repete quase igual a partir de 83 s) são fiáveis. As notas seguem a 1.ª passagem (voz separada, pYIN + basic-pitch por semicolcheia), conferidas com a 2.ª; o ritmo fino das sílabas e as notas de passagem cromáticas (fá#, ré#, sib) são aproximados e o rubato foi achatado num tempo fixo. Acordes inferidos da melodia e do baixo da guitarra, não de partitura.'


def build(preview_dir=None):
    return chip.render(sys.modules[__name__], 'hora-da-despedida', preview_dir=preview_dir, lead_octave=1)


if __name__ == "__main__":
    print(build(sys.argv[1] if len(sys.argv) > 1 else None))
