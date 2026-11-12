#!/usr/bin/env python3
# "Ressurreição", hino do Movimento Nacionalista Português, cantado por coro
# de homens em uníssono com banda. Hino em 6/8 (semínima com ponto ~111 por
# minuto), ré maior. A gravação está afinada a meio caminho entre dois tons
# (~48 cents acima de ré bemol, ~52 abaixo de ré): a afinação foi medida uma
# só vez para a gravação toda e o hino foi escrito em ré maior, com todas as
# notas lidas com essa mesma correcção (nunca nota a nota).
# Transcrito de ouvido da gravação de referência (voz separada com demucs,
# pYIN + basic-pitch, salience média por colcheia das quatro estrofes iguais,
# limpa para a escala e para os acordes da banda); não há partitura pública.
# Forma da gravação: introdução da banda com o tema (2 x 8 compassos), cinco
# estrofes de 16 compassos (4 versos de 4 compassos, cada verso entra com uma
# anacruse no 2.º tempo do compasso anterior) A, A' (acaba na tónica), B
# ("Das rosas em festa..."), A, A com coda, e o tema da banda outra vez.
# Ciclo de 72 compassos, pela ordem da gravação: tema T + A + A2 + B + A; a
# última estrofe acaba em lá 7 e volta ao ré do tema, por isso o loop encaixa.
# Escrito à altura do canto (ré 3 a ré 4); o lead toca uma oitava acima.
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import chip  # noqa: E402

TITLE = 'Ressurreição'
BPM = 166.3  # semínima; 6/8 com a semínima com ponto a ~110.9
BAR = 12
METER = '6/8'
TRANSPOSE_TO_REF = -1  # tom da gravação = escrito + isto (+48 cents: a gravação fica entre ré bemol e ré)
METHOD = 'ear'

SECTIONS = {
    'T': [
        'A3:12', 'B3:4 C#4:2 D4:2 C#4:4', 'G3:12', 'F#3:2 G3:4 F#3:6',
        'F#3:8 G3:4', 'G3:2 F#3:8 E3:2', 'D3:4 E3:2 F#3:6', 'F#3:10 E3:2',
    ],
    'A': [
        'D3:4 D3:2 D3:6', 'D3:4 F#3:8', 'A3:12', 'A3:6 B3:4 C#4:2',
        'D4:4 D4:2 D4:6', 'D4:4 C#4:6 B3:2', 'C#4:12', 'C#4:6 B3:4 C#4:2',
        'D4:4 D4:2 D4:6', 'D4:4 C#4:8', 'A3:12', 'D3:6 E3:4 F#3:2',
        'E3:4 G3:8', 'G3:12', 'E3:4 F#3:6 E3:2', 'F#3:4 E3:2 G3:4 F#3:2',
    ],
    'A2': [
        'D3:4 D3:2 D3:6', 'D3:4 F#3:8', 'A3:12', 'A3:6 B3:4 C#4:2',
        'D4:4 D4:2 D4:6', 'D4:4 C#4:6 B3:2', 'C#4:12', 'C#4:6 B3:4 C#4:2',
        'D4:4 D4:2 D4:6', 'D4:4 C#4:8', 'A3:12', 'D3:6 E3:4 F#3:2',
        'G3:12', 'G3:12', 'D3:12', 'D3:8 r:2 D3:2',
    ],
    'B': [
        'E3:12', 'G3:12', 'F#3:12', 'F#3:12',
        'G3:12', 'G3:2 A3:4 B3:6', 'A#3:12', 'A#3:12',
        'B3:12', 'C#4:2 D4:4 C#4:6', 'A3:12', 'F#3:2 D3:10',
        'E3:4 G3:8', 'G3:8 F#3:4', 'D3:4 F#3:6 E3:2', 'F#3:4 E3:4 F#3:4',
    ],
}
CHORDS = {
    'T': [
        'D', 'D', 'Em', 'Em',
        'A', 'A7', 'D', 'D',
    ],
    'A': [
        'D', 'D', 'A7', 'A7',
        'Bm', 'Bm', 'F#', 'F#',
        'G', 'G|D', 'D', 'Bm',
        'Em', 'Em', 'A7', 'A7',
    ],
    'A2': [
        'D', 'D', 'A7', 'A7',
        'Bm', 'Bm', 'F#', 'F#',
        'G', 'G|D', 'D', 'Bm',
        'Em', 'A7', 'D', 'D',
    ],
    'B': [
        'Em', 'A7', 'D', 'D',
        'G', 'G', 'F#', 'F#',
        'G', 'G|Bm', 'D', 'Bm',
        'Em', 'Em', 'A7', 'A7',
    ],
}
FORM = ['T', 'A', 'A2', 'B', 'A']
REF_START = 13.41  # segundos na gravação de referência (só para a verificação)
CONFIDENCE = 'MÉDIA. Fiáveis: compasso 6/8 (autocorrelação dos ataques com subdivisão em três, ~0.18 s por colcheia), andamento (~111 semínimas com ponto, ligeira deriva), ré maior com a gravação a ~48 cents (entre ré bemol e ré: afinação global, medida uma vez), a forma (estrofes de 16 compassos; a 1.ª, 2.ª, 4.ª e 5.ª são a mesma música, croma do acompanhamento 0.94-0.98 com atraso de 16/48/64 compassos) e os acordes (croma da banda separada: ré, lá 7, si menor, fá sustenido, sol, mi menor). A melodia de A é a média por colcheia das quatro estrofes iguais; B ("Das rosas em festa...") e o fim de A2 só têm uma passagem. Duvidosos: o ritmo silábico (coro em legato, ataques pouco nítidos), as anacruses de cada verso, as notas que o coro canta a meio (mi/fá sustenido e lá sustenido/si na secção B) e o tema da banda da introdução, que se ouve mal por trás. Verificação: drift-tolerant 0.90, frame 0.81 (score.py). Sem partitura nem MIDI.'


def build(preview_dir=None):
    return chip.render(sys.modules[__name__], 'ressurreicao', preview_dir=preview_dir, lead_octave=1)


if __name__ == "__main__":
    print(build(sys.argv[1] if len(sys.argv) > 1 else None))
