#!/usr/bin/env python3
# "A nossa terra" (marcha de Tavares Belo, 1955, cantada por Anita Guerreiro),
# 2/4, ré menor com o refrão em ré maior. Transcrita de ouvido da gravação de
# referência (voz separada com demucs, pYIN + basic-pitch, votação por
# semicolcheia entre as duas estrofes, limpa e ajustada à escala); não há
# partitura pública. Escrita uma oitava abaixo e tocada uma oitava acima
# (lead_octave=1). Ciclo de 51 compassos: anacruse + estrofe A ("Não sei de
# terra..."), B ("Não há no mundo..."), C ("Tenho vaidade...") e o refrão R
# ("Se o destino em má hora..."), que acaba na tónica e volta à anacruse.
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import chip  # noqa: E402

TITLE = 'A nossa terra'
BPM = 132.1
BAR = 8
TRANSPOSE_TO_REF = 0  # tom da gravação = escrito + isto
METHOD = 'ear'

SECTIONS = {
    'A': [
        'r:2 A3:2 D4:4', 'D4:2 A3:2 D4:2 F4:2', 'A4:2 F4:2 D4:2 Bb4:2', 'A4:2 F4:2 D4:2 A4:2',
        'A4:4 D4:2 E4:2', 'F4:2 E4:2 A4:4', 'G4:2 F4:2 E4:2 F4:2', 'F4:2 E4:2 D4:4',
        'D4:6 C#4:2', 'C#4:2 F4:2 A4:2 C5:2', 'A4:6 C5:2', 'C5:2 A4:2 Bb4:4',
        'C5:2 Bb4:2 F4:4', 'G4:2 Ab4:4 C5:2', 'Bb4:2 F4:2 G4:4', 'E4:1 F4:1 Bb4:4 A4:2',
        'Bb4:2 A4:6',
    ],
    'B': [
        'A4:3 Bb4:3 A4:2', 'A4:1 G4:1 D4:2 E4:2 A4:2', 'A4:1 G4:3 E4:2 A4:2', 'A4:1 F4:3 D4:2 A4:2',
        'Bb4:2 F4:1 G4:1 C#4:4', 'E4:2 G4:2 Bb4:1 A4:3', 'E4:1 G4:2 F4:2 A4:3', 'A4:8',
        'A4:3 Bb4:3 A4:2', 'A4:1 G4:2 E4:2 A4:3', 'G4:4 A4:3 Bb4:1', 'A4:2 F4:1 D4:2 F4:1 A4:2',
        'F4:3 C#4:5', 'G4:2 A4:2 Bb4:2 A4:2', 'E4:1 C#4:3 D4:4',
    ],
    'C': [
        'C#4:2 D4:6', 'r:1 A3:2 B3:5', 'r:1 Bb3:2 C#4:1 D4:4', 'C#4:2 E4:6',
        'E4:2 F4:4 A3:2',
    ],
    'R': [
        'A3:2 B3:6', 'A3:2 G4:3 F#4:3', 'F#4:1 A4:7', 'A4:8',
        'A4:5 F#4:1 D4:2', 'D4:2 A4:3 F#4:3', 'r:1 D4:2 A4:5', 'A4:1 G4:2 D4:1 E4:1 D4:3',
        'D4:1 C4:1 B3:1 Bb3:5', 'Bb3:8', 'r:3 Ab4:1 G4:1 F#4:3', 'F#4:2 A4:2 A4:1 D4:2 E4:1',
        'E4:1 F4:1 G4:3 C#4:3', 'D4:8',
    ],
}
CHORDS = {
    'A': [
        'A7', 'Dm', 'Dm', 'Dm',
        'A7', 'Dm|A7', 'A7', 'A7|Dm',
        'Dm|A7', 'F', 'F', 'C7',
        'C7', 'C7', 'C7', 'C7|F',
        'F',
    ],
    'B': [
        'F', 'C7', 'C7', 'Dm',
        'Gm|A7', 'A7', 'A7|Dm', 'Dm',
        'Dm', 'A7', 'A7', 'Dm',
        'Dm|A7', 'Gm|Dm', 'A7|Dm',
    ],
    'C': [
        'Dm', 'A7', 'A7|Dm', 'A7',
        'A7',
    ],
    'R': [
        'A7', 'G|D', 'D', 'D',
        'D', 'D', 'D', 'D|G',
        'G|Gm', 'Gm', 'D', 'D|A7',
        'A7', 'Dm',
    ],
}
FORM = ['A', 'B', 'C', 'R']
REF_START = 6.32  # segundos na gravação de referência (só para a verificação)
CONFIDENCE = 'MÉDIA. Andamento (132 bpm, 2/4), tonalidade (ré menor; refrão em ré maior com o iv menor, sol menor, em "saudade"), afinação da gravação (+19 cents) e a forma de 51 compassos são sólidos; a estrofe repete-se aos 62 s e as notas resultam da votação por semicolcheia entre as duas estrofes (voz separada com demucs, pYIN + basic-pitch), ajustadas à mão. Duvidosos: o vibrato largo da cantora confunde lá/si bemol e si/si bemol (compassos A16-B1, B9, C2-C3, R9-R10: medido si bemol 3 em R9-R10), o cromatismo sol-sol#-dó de A14 e o ritmo exato das colcheias pontuadas. Acordes inferidos da melodia, não de partitura.'


def build(preview_dir=None):
    return chip.render(sys.modules[__name__], 'a-nossa-terra', preview_dir=preview_dir, lead_octave=1, passes=2)


if __name__ == "__main__":
    print(build(sys.argv[1] if len(sys.argv) > 1 else None))
