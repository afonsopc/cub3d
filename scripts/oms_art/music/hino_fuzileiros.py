#!/usr/bin/env python3
# "Hino dos Fuzileiros": hino cantado em uníssono por um coro de homens com
# banda, em 4/4, lá bemol maior (a banda está ~10 cents acima do lá 440; o
# coro sobe e desce até meio tom à volta disso). Transcrito de ouvido da
# gravação de referência (voz isolada com demucs, pYIN + basic-pitch, votação
# por semicolcheia das duas primeiras estrofes, a terceira só para confirmar);
# não há partitura pública. O coro canta na oitava grave (dó3 a mi bemol 4):
# escrito como é cantado e tocado uma oitava acima.
# Forma: a banda abre com a última frase do hino (I, 4 compassos: ré bemol,
# lá bemol, si bemol com sétima, mi bemol com sétima, lá bemol) e depois o coro
# canta três estrofes iguais de 24 compassos (A), sem interlúdio. O loop é
# I + A + A (segunda vez em terceiras); A acaba em lá bemol e volta ao ré
# bemol da introdução, tal como a gravação passa de uma estrofe à seguinte.
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import chip  # noqa: E402

TITLE = 'Hino dos Fuzileiros'
BPM = 110.6
BAR = 16
TRANSPOSE_TO_REF = 0  # tom da gravação = escrito + isto (cantado uma oitava abaixo do que soa aqui)
METHOD = 'ear'

_A = [
    'C3:6 F3:2 Ab3:4 G3:4', 'Eb3:10 C3:2 Db3:2 C3:2', 'Db3:6 Eb3:2 F3:4 Eb3:2 Db3:2', 'C3:12 r:2 C3:2',
    'Db3:6 Eb3:2 F3:8', 'Ab3:8 Eb3:8', 'F3:8 F3:4 Bb3:4', 'G3:12 Ab3:2 Bb3:2',
    'C4:8 Eb4:2 Db4:2 C4:2 Bb3:2', 'Bb3:8 G3:2 Eb3:6', 'Ab3:10 C4:2 Bb3:2 Ab3:2', 'G3:12 Ab3:2 G3:2',
    'F3:8 F3:6 Ab3:2', 'Ab3:8 Eb3:4 C4:4', 'Bb3:10 C4:6', 'Bb3:12 Ab3:2 Bb3:2',
    'C4:8 Eb4:2 Db4:6', 'Bb3:8 G3:2 Eb3:4 G3:2', 'Ab3:10 C4:4 Bb3:2', 'G3:16',
    'F3:16', 'Ab3:12 C4:4', 'Bb3:10 G3:6', 'Ab3:12 r:4',
]
_A_CH = [
    'Fm', 'Cm', 'Db', 'C',
    'Db|Bb7', 'Ab', 'Bb7', 'Eb',
    'Ab', 'Eb', 'Fm', 'C',
    'Db', 'Ab', 'Bb7', 'Eb',
    'Ab', 'Eb', 'Fm', 'C',
    'Db', 'Ab', 'Bb7|Eb7', 'Ab',
]
SECTIONS = {
    'I': _A[20:],
    'A': _A,
    'B': _A,
}
CHORDS = {
    'I': _A_CH[20:],
    'A': _A_CH,
    'B': _A_CH,
}
HARMONY = ('B',)
FORM = ['I', 'A', 'B']
REF_FORM = ['A', 'B']  # a verificação compara as duas primeiras estrofes cantadas
REF_START = 15.89  # segundos na gravação de referência (só para a verificação)
CONFIDENCE = 'MÉDIA-BAIXA. Fiáveis: andamento (110.6 semínimas por minuto, igual nas três estrofes), compasso 4/4 (a harmonia muda de 2.17 em 2.17 s), tonalidade (lá bemol maior, a banda ~10 cents acima), a forma (introdução da banda com a última frase e três estrofes de 24 compassos que se repetem tal e qual, aos 15.9, 68.0 e 120.0 s) e os acordes, tirados do croma e do baixo da banda separada e iguais nas estrofes (fá menor, dó maior como dominante dela, ré bemol, si bemol com sétima, mi bemol). Incertos: o coro em uníssono desafina e escorrega entre notas (fica muitas vezes a meio entre dó e ré bemol, fá e fá sustenido, lá bemol e lá), por isso as notas foram ajustadas à escala e aos acordes; os ritmos curtos dentro de cada compasso (sílabas) são aproximados e as notas longas que abrem cada compasso são o que é seguro.'


def build(preview_dir=None):
    return chip.render(sys.modules[__name__], 'hino-fuzileiros', preview_dir=preview_dir, lead_octave=1, passes=1)


if __name__ == "__main__":
    print(build(sys.argv[1] if len(sys.argv) > 1 else None))
