#!/usr/bin/env python3
# "Deus, Pátria, Rei" (canção monárquica), marcha em lá menor, 2/4.
# Transcrita de ouvido da gravação de referência (coro masculino); não há
# partitura pública. Escrita uma oitava acima do canto. Ciclo de 48 compassos:
# estrofe A (16) + estrofe B (12) + refrão C (20); o último compasso do refrão
# traz a anacruse de volta à estrofe, por isso o loop encaixa.
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import chip  # noqa: E402

TITLE = 'Deus, Pátria, Rei'
BPM = 119.0
BAR = 8
TRANSPOSE_TO_REF = -12  # tom da gravação = escrito + isto
METHOD = 'ear'

SECTIONS = {
    'A': [
        'C4:2 D#4:2 E4:4', 'E4:6 A4:2', 'C5:8', 'C5:4 B4:4',
        'A4:4 G#4:2 B4:2', 'B4:4 G#4:2 E4:2', 'F4:4 E4:4', 'E4:8',
        'D4:4 F4:2 A4:2', 'A4:4 F4:2 D4:2', 'B3:2 C4:6', 'C4:8',
        'B3:4 D4:2 F4:2', 'E4:2 F4:2 B3:4', 'A3:8', 'A3:4 r:4',
    ],
    'B': [
        'A3:4 B3:2 C4:2', 'C4:2 D#4:2 E4:2 F4:2', 'E4:6 A4:2', 'A4:4 F4:2 E4:2',
        'E4:2 D4:2 C4:2 E4:2', 'D#4:4 B3:2 C4:2', 'C4:8', 'C4:4 r:2 D4:2',
        'D4:4 E4:2 F4:2', 'F4:1 G4:1 A4:2 B4:2 C5:2', 'C5:6 E4:2', 'E4:4 r:2 A4:2',
    ],
    'C': [
        'A4:2 G#4:1 G4:2 G#4:1 A4:2', 'A4:2 F#4:2 G#4:2 B4:2', 'B4:8', 'B4:6 D5:2',
        'D5:4 C5:2 B4:2', 'B4:2 D5:2 C5:4', 'C5:6 A4:2', 'A4:4 C5:2 B4:2',
        'B4:4 G#4:4', 'G#4:4 A4:2 C5:2', 'C5:8', 'C#5:6 D5:2',
        'D5:4 C5:2 B4:2', 'B4:2 D5:4 C5:2', 'C5:6 A4:2', 'A4:4 C5:2 B4:2',
        'B4:2 C5:2 B4:2 G#4:2', 'G#4:3 B4:1 C5:2 A4:2', 'A4:8', 'A4:4 A3:2 B3:2',
    ],
}
CHORDS = {
    'A': [
        'Am', 'E', 'Am', 'Am',
        'E', 'E', 'Dm', 'Am',
        'Dm', 'Dm', 'G7', 'C',
        'Dm', 'E', 'Am', 'Am',
    ],
    'B': [
        'Am', 'E7', 'E', 'Am',
        'Am', 'B7', 'Am', 'Am',
        'Dm', 'Dm', 'Am', 'E',
    ],
    'C': [
        'Am', 'E', 'E', 'E7',
        'E7', 'E7|Am', 'Am', 'Am',
        'E', 'E|Am', 'Am', 'A7',
        'Dm', 'E7', 'Am', 'Dm',
        'E', 'E|Am', 'Am', 'Am|E',
    ],
}
FORM = ['A', 'B', 'C']
REF_START = 4.26  # segundos na gravação de referência (só para a verificação)
CONFIDENCE = 'Ear transcription; key (A minor), 2/4 meter, 119 bpm and the 48-bar cycle (16+12+20, repeated 4x in the reference) are solid. Verse bars 1-6 and 9-12 and most of the refrain are consistent across all four sung repeats. Shaky: verse bar 8 (E held vs. a passing C), bars 13-14 (B-D-F / E-F-B cadence), verse second half bars 6-8 (D#/B/C/C# chromatic area), refrain bars 1-2 (chromatic A-G#-G-G# neighbour figure) and the rhythm details of dotted/16th notes. Chords are inferred from the melody, not from a score.'


def build(preview_dir=None):
    return chip.render(sys.modules[__name__], 'deus-patria-rei', preview_dir=preview_dir, lead_octave=1)


if __name__ == "__main__":
    print(build(sys.argv[1] if len(sys.argv) > 1 else None))
