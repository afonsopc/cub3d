#!/usr/bin/env python3
# "Partida para o Ultramar" (Conjunto Os Peles Vermelhas, anos 60): a
# despedida de um soldado que parte para Angola. Balada em 6/8, lá menor
# (a gravação está ~38 cents abaixo do lá 440). Transcrita de ouvido da
# gravação de referência (voz isolada com demucs, pYIN + basic-pitch, média
# das três vezes que cada frase é cantada); não há partitura pública. A
# melodia é quase recitada à volta do mi, com bordaduras de fá e descidas
# para o si e o lá graves. Escrita como é cantada e tocada uma oitava acima.
# Forma: estrofe de 32 compassos (A + B); na gravação repete-se tal e qual,
# por isso o loop encaixa (B acaba em mi com sétima e volta ao lá menor).
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import chip  # noqa: E402

TITLE = 'Partida para o Ultramar'
BPM = 113.8
BAR = 12
METER = '6/8'
TRANSPOSE_TO_REF = 0  # tom da gravação = escrito + isto (cantado uma oitava abaixo do que soa aqui)
METHOD = 'ear'

SECTIONS = {
    'A': [
        'r:2 E4:4 E4:2 E4:4', 'E4:6 F4:2 E4:2 F4:2', 'E4:12', 'E4:6 F4:2 E4:2 D4:1 C4:1',
        'B3:12', 'E4:4 D4:2 E4:2 D4:2 E4:2', 'E4:2 D4:10', 'D4:6 E4:2 D4:2 B3:2',
        'B3:2 A3:4 A4:4 E4:2', 'E4:6 F4:2 E4:2 F4:2', 'E4:12', 'A4:6 G4:2 F4:2 D4:2',
        'D4:12', 'E4:2 F4:6 E4:2 D4:2', 'C4:6 E4:2 D4:4', 'D4:2 E4:4 D4:6',
    ],
    'B': [
        'A3:6 E4:2 F4:2 D4:2', 'D4:6 E4:1 F4:1 E4:2 C4:2', 'C4:6 E4:2 D4:4', 'B3:4 C4:2 D4:6',
        'E4:12', 'D4:6 E4:2 F4:2 E4:2', 'D4:2 C4:4 E4:6', 'B3:6 D4:4 B3:2',
        'A3:6 A4:4 F4:2', 'F4:2 D4:6 F4:4', 'D4:2 C4:6 E4:4', 'C4:2 B3:6 C4:2 D4:2',
        'B3:2 E4:10', 'D4:6 F4:1 E4:3 D4:2', 'E4:2 D4:2 C4:2 E4:6', 'E4:2 D4:10',
    ],
}
CHORDS = {
    'A': [
        'Am', 'Am', 'Am', 'Am|E7',
        'Em', 'Em', 'Em', 'Em|B7',
        'Am', 'Am', 'Am', 'Am|G7',
        'Dm', 'Dm|E7', 'Am|E7', 'E7',
    ],
    'B': [
        'Am|Dm', 'Dm|E7', 'Am', 'C|E7',
        'Am|E7', 'Dm|E7', 'Am', 'E7',
        'Am|C', 'Dm|E7', 'Am', 'E7',
        'Am', 'Dm|E7', 'Am', 'E7',
    ],
}
FORM = ['A', 'B']
REF_START = 11.20  # segundos na gravação de referência (só para a verificação)
CONFIDENCE = 'BAIXA-MÉDIA. Gravação velha e fraca (o próprio autor do vídeo diz que o áudio não é bom), voz recitada quase sempre à volta do mi. Fiáveis: compasso 6/8 (colcheia ~0.264 s, autocorrelação em grupos de 3), andamento, tonalidade (lá menor, ~38 cents abaixo), a estrofe de 32 compassos (repete-se aos 61.8 s; a segunda metade volta aos ~138 s) e as notas longas que fecham cada frase (mi, si, ré, lá). Incertos: o ritmo silábico dentro de cada compasso, fá natural vs. mi alto (a voz fica a meio, ~64.6), os saltos de oitava no lá (lá3/lá4) e os acordes, inferidos do croma da banda separada (Am, Dm, Em, E7), não de partitura.'


def build(preview_dir=None):
    return chip.render(sys.modules[__name__], 'partida-ultramar', preview_dir=preview_dir, lead_octave=1, passes=2)


if __name__ == "__main__":
    print(build(sys.argv[1] if len(sys.argv) > 1 else None))
