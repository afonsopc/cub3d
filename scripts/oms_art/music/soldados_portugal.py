#!/usr/bin/env python3
# "Soldados de Portugal": marcha cantada por um coro de homens com banda
# (fanfarra), da compilação de canções da Guerra Colonial; 2/4, dó maior,
# ~88 bpm à semínima. Transcrita de ouvido da gravação de referência (voz
# separada com demucs, basic-pitch + pYIN por semicolcheia, votação entre as
# três vezes que a estrofe é cantada, limpa e ajustada aos acordes da banda);
# não há partitura pública. O coro (muitas vozes, espalhadas) canta 35 a 100
# cents abaixo da banda; essa afinação foi medida de forma contínua e
# descontada antes da votação, e as notas a meio decididas pelo acorde da
# banda. Escrita na oitava do coro (ré3-ré4) e tocada uma oitava acima
# (lead_octave=1).
# Forma: I = o toque de clarim do início da gravação (sol-mi-dó-sol-dó);
# A = a estrofe de 32 compassos (descida lá-sol-fá sobre dó-lá7-rém-sol7,
# subida para o dó, a frase em mi7-lám-fá-fám e o fecho longo em dó), que acaba
# no dó e volta ao clarim, por isso o loop encaixa.
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import chip  # noqa: E402

TITLE = 'Soldados de Portugal'
BPM = 88.0
BAR = 8
TRANSPOSE_TO_REF = 0  # tom da gravação = escrito + isto (dó maior; a banda ~15 cents abaixo, o coro ~40 abaixo dela)
METHOD = 'ear'

SECTIONS = {
    'I': [
        'G4:8', 'G4:6 E4:2', 'E4:3 C4:1 E4:2 G3:2', 'r:1 C4:7',
        'C4:8', 'C4:4 r:4',
    ],
    'A': [
        'r:2 A3:2 A3:2 F3:1 G3:1', 'G3:2 G3:2 A3:2 A3:2', 'F3:2 F3:2 F3:2 F3:1 D3:1', 'D3:1 F3:1 F3:2 A3:4',
        'B3:1 C4:2 B3:2 A3:3', 'A3:2 B3:2 A3:2 G3:2', 'A3:2 G3:6', 'G3:3 r:1 G3:1 A3:3',
        'A3:2 B3:2 C4:2 B3:1 C4:1', 'C4:2 C4:1 B3:1 A3:3 B3:1', 'C4:1 G#3:1 B3:2 B3:2 C4:2', 'A3:1 G3:2 B3:1 C4:2 B3:1 A3:1',
        'A3:2 B3:2 A3:4', 'A3:2 B3:1 A3:1 A3:2 B3:2', 'G3:1 C4:1 A3:1 G3:2 C4:1 B3:2', 'A3:1 G3:1 F3:2 G3:1 F3:1 G3:2',
        'r:1 G3:1 A3:6', 'A3:2 G3:2 A3:2 G3:2', 'F3:6 r:1 F3:1', 'F3:3 A3:1 F3:1 A3:3',
        'A3:1 G#3:3 C4:3 B3:1', 'C4:6 B3:2', 'B3:1 A3:7', 'A3:2 C4:2 A3:2 C4:2',
        'D4:2 C4:1 D4:1 C4:1 D4:2 C4:1', 'Ab3:2 F3:2 r:1 C4:1 Ab3:2', 'C4:1 B3:1 G3:6', 'G3:4 r:1 A3:2 C4:1',
        'G3:1 A3:1 C4:2 B3:2 r:2', 'C4:2 B3:2 C4:2 r:1 C4:1', 'C4:2 C4:2 C4:2 C4:2', 'C4:3 E3:1 F3:2 G3:2',
    ],
}
CHORDS = {
    'I': [
        'C', 'C', 'C', 'C',
        'C', 'C|G7',
    ],
    'A': [
        'C', 'A7', 'Dm', 'G7',
        'G7', 'G7', 'C', 'C',
        'C', 'C', 'E7|G', 'G',
        'Dm|G7', 'G7', 'C', 'C',
        'C', 'Am|C', 'Dm|G7', 'G7',
        'E7', 'E7', 'Am', 'Am',
        'F', 'Fm', 'C', 'C',
        'C', 'C', 'C', 'C',
    ],
}
FORM = ['I', 'A']
REF_FORM = ['A']  # só a estrofe se compara com a voz (o clarim é instrumental)
REF_START = 29.06  # segundos na gravação de referência (só para a verificação)
CONFIDENCE = 'BAIXA-MÉDIA. Fiáveis: compasso 2/4 (baixo nos tempos, colcheia ~0.34 s), andamento (~88 bpm), tonalidade (dó maior, banda afinada a 440), a estrofe de 32 compassos (cantada três vezes: 29 s, 73 s e 126 s), os acordes (croma e baixo da banda separada: dó, lá7, rém, sol7, mi7, lám, fá, fám) e o toque de clarim do início (sol-mi-dó-sol-dó). A banda não dobra o coro (basic-pitch do no_vocals: só acordes em contratempo e figuras de fanfarra), por isso a referência é a voz. O coro canta abaixo da banda e espalhado; medi a afinação de forma contínua: altura fina da voz (CQT a 20 cents com soma de harmónicos), média circular da parte fraccionária em janelas de 4 compassos, dobrada para o lado baixo (-110 a -10 cents) e alisada; dá entre -35 e -100 cents (em média ~-65) contra a banda. Com essa compensação votei as três estrofes por semicolcheia (pYIN + altura fina) e decidi as notas a meio pelo acorde. score.py com a referência compensada: drift-tolerant 0.74 na 1.ª estrofe (0.39 e 0.47 na 2.ª e 3.ª, onde a voz quase não é detectada e a afinação muda mais); sem compensação dá 0.23. O desenho da melodia é razoável; as notas exactas, o ritmo silábico (simplificado) e o fecho (compassos 28-31) continuam duvidosos.'


def build(preview_dir=None):
    return chip.render(sys.modules[__name__], 'soldados-de-portugal', preview_dir=preview_dir, lead_octave=1, passes=2)


if __name__ == "__main__":
    print(build(sys.argv[1] if len(sys.argv) > 1 else None))
