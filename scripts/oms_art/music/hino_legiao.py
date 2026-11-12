#!/usr/bin/env python3
# Hino da Legião Portuguesa (1937), 4/4, escrito em mi menor (a gravação
# está em mi bemol menor, ~35 cents abaixo do lá 440). Transcrito de ouvido de
# uma gravação muito fraca (locução por cima, banda abafada, oscilação do disco);
# é a linha predominante, limpa e ajustada à escala, sem partitura que a
# confirme. Forma: I (entrada da banda), A (frase da banda), B e C (coro).
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import chip  # noqa: E402

TITLE = 'Hino da Legião Portuguesa'
BPM = 128.8
BAR = 16
TRANSPOSE_TO_REF = -1  # tom da gravação = escrito + isto
METHOD = 'ear'

SECTIONS = {
    'I': [
        'D5:10 C5:2 B4:2 A4:2',
    ],
    'A': [
        'C5:2 D5:2 F#5:2 C5:2 B4:4 G4:4', 'G4:2 D5:2 A4:2 D5:2 B4:2 A4:2 B4:2 D5:2', 'E5:2 D5:2 A4:2 G4:2 G4:2 A4:2 G4:2 B4:2', 'B4:4 B4:2 E5:4 F#5:2 G5:4',
        'G5:2 B4:4 E5:4 D5:2 G5:2 F#5:2', 'B4:2 F#4:2 A4:2 B4:2 D#5:4 F#5:2 E5:2', 'G5:2 C5:2 G4:2 A4:2 C5:2 D5:2 A4:2 G4:2', 'E4:2 D5:2 A4:2 D5:2 A4:2 D#5:2 A4:2 C5:2',
    ],
    'B': [
        'C5:6 B4:4 C5:2 D#5:4', 'E5:2 C5:4 B4:6 D#5:4', 'D#5:2 D5:2 B4:4 G4:2 A4:2 D#5:2 D5:2', 'C5:4 B4:12',
        'C5:4 E5:2 G5:2 C5:2 E5:2 B4:4', 'B4:8 C5:2 E5:2 C5:4', 'B4:2 C5:6 G4:6 B4:2', 'B4:6 G4:2 A4:2 G4:6',
    ],
    'C': [
        'G4:2 B4:2 C5:2 A4:2 E4:2 C5:4 B4:2', 'B4:4 C5:8 A4:4', 'B4:2 C5:2 E5:2 D#5:2 E5:2 G5:2 E5:4', 'E5:2 A5:2 E5:2 F#5:2 E5:2 D#5:6',
        'D5:2 B4:2 A4:2 C5:2 B4:4 D5:2 D#5:2', 'D#5:2 E5:2 D5:2 C5:2 B4:6 E5:2', 'D5:8 B4:2 G4:2 C5:2 A4:2', 'D#5:2 D5:4 C5:2 B4:8',
    ],
}
CHORDS = {
    'I': [
        'G|B7',
    ],
    'A': [
        'Am|Em', 'G|B7', 'Em', 'Em',
        'Em', 'B7', 'C|Am', 'D|B7',
    ],
    'B': [
        'Am|B7', 'Am|B7', 'B7', 'Em',
        'C|Em', 'Em|Am', 'Am|Em', 'Em',
    ],
    'C': [
        'Em|Am', 'Em|Am', 'Em', 'Am|B7',
        'B7', 'Am|Em', 'G|Em', 'Em|B7',
    ],
}
FORM = ['I', 'A', 'B', 'C']
REF_START = 13.12  # segundos na gravação de referência (só para a verificação)
CONFIDENCE = 'BAIXA. Gravação de ~1937 com locução por cima (0-10 s), banda abafada (95 % da energia < 1.8 kHz), oscilação de altura de ±0.3 meio-tom e coro grave sem linha clara. Tonalidade (mi b menor, sensível ré natural) e andamento (128.8 bpm, 4/4) são fiáveis; as notas são a linha predominante extraída por Viterbi + basic-pitch, ajustada à escala, e não foram confirmadas por partitura. Não se encontrou partitura nem MIDI online.'


def build(preview_dir=None):
    return chip.render(sys.modules[__name__], 'hino-legiao', preview_dir=preview_dir)


if __name__ == "__main__":
    print(build(sys.argv[1] if len(sys.argv) > 1 else None))
