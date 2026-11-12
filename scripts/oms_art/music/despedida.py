#!/usr/bin/env python3
# "Despedida", canção da guerra colonial (gravação da compilação "Two hours of
# Portuguese songs in the Colonial War"; sem nome de autor ou intérprete
# anunciado). Valsa lenta em 3/4, ré menor, ~153 semínimas por minuto (a
# gravação está ~13 cents acima do lá 440). Voz masculina com vibrato largo e
# arrastos: transcrita de ouvido da gravação de referência (voz isolada com
# demucs, pYIN + basic-pitch, alturas médias por tempo para tirar o vibrato,
# votadas entre as duas estrofes); não há partitura pública. Escrita na oitava
# do canto e tocada uma oitava acima (lead_octave=1).
# Forma: uma estrofe de 64 compassos, em frases de 4 compassos que começam
# quase sempre com a descida mi-ré e uma nota longa (lá, sol, si bemol, dó);
# acordes de ré menor, lá com sétima, sol menor, dó com sétima e si bemol. Na
# gravação a estrofe repete-se igual, e o fim (fá, mi, ré) leva de volta à
# descida mi-ré do início, por isso o loop encaixa.
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import chip  # noqa: E402

TITLE = 'Despedida'
BPM = 152.7
BAR = 12
METER = '3/4'
TRANSPOSE_TO_REF = 0  # tom da gravação = escrito + isto (ré menor, +13 cents)
METHOD = 'ear'

SECTIONS = {
    'A': [
        'E3:4 D3:4 E3:4', 'A3:12', 'A3:12', 'A3:4 r:4 E3:4',
        'E3:4 D3:8', 'G3:12', 'G3:12', 'G3:6 r:2 F#3:2 G3:2',
        'Bb3:4 A3:4 G3:4', 'r:8 E3:4', 'E3:12', 'E3:4 r:4 F3:4',
        'E3:8 F3:2 G3:2', 'A3:12', 'A3:12', 'A3:4 r:2 C#3:2 D3:2 E3:2',
        'E3:6 D3:6', 'A3:12', 'A3:12', 'A3:4 r:4 A3:4',
        'A3:2 Bb3:4 A3:6', 'Bb3:12', 'Bb3:12', 'Bb3:8 E3:4',
        'G3:4 F3:2 E3:4 G3:2', 'A3:12', 'A3:12', 'A3:6 r:2 E3:4',
        'E3:12', 'E3:2 D3:10', 'D3:12', 'D3:4 C#3:4 r:4',
    ],
    'B': [
        'D3:8 F3:4', 'G3:12', 'G3:8 F#3:4', 'G3:4 A3:4 Bb3:4',
        'C4:8 D4:4', 'C4:4 Bb3:4 A3:4', 'A3:12', 'A3:12',
        'A3:8 Bb3:2 A3:2', 'A3:4 G3:8', 'G3:6 F#3:6', 'G3:4 r:8',
        'F3:4 E3:4 F#3:2 G3:2', 'G3:4 A3:8', 'A3:12', 'A3:8 r:4',
        'D3:8 E3:4', 'F3:4 G3:8', 'G3:12', 'G3:10 D4:2',
        'C4:12', 'D4:2 C4:2 Bb3:4 A3:4', 'A3:12', 'A3:4 D4:4 Bb3:4',
        'Bb3:2 A3:10', 'A3:8 G3:4', 'G3:12', 'G3:8 r:4',
        'F3:12', 'F3:4 E3:4 D3:4', 'A3:12', 'r:4 F3:8',
    ],
}
CHORDS = {
    'A': [
        'A7', 'Dm', 'Dm', 'Dm',
        'Dm', 'A7', 'A7', 'A7',
        'A7', 'A7', 'A7', 'A7',
        'A7', 'Dm', 'Dm', 'Dm',
        'Dm|A7', 'Dm', 'Dm', 'Dm',
        'Dm', 'Gm', 'Gm', 'Gm',
        'Gm|A7', 'Dm', 'Dm', 'Dm',
        'A7', 'Dm', 'Dm', 'Dm|A7',
    ],
    'B': [
        'Dm', 'Gm', 'Gm', 'Gm',
        'C7', 'Dm', 'Dm', 'Dm',
        'Bb', 'A7', 'A7', 'A7',
        'A7', 'Dm', 'Dm', 'Dm',
        'Dm', 'Gm', 'Gm', 'Gm',
        'C7', 'Dm', 'Dm', 'Dm',
        'Bb', 'A7', 'A7', 'A7',
        'Gm', 'Dm', 'Dm', 'Dm|A7',
    ],
}
FORM = ['A', 'B']
REF_START = 29.83  # segundos na gravação de referência (só para a verificação)
CONFIDENCE = 'MÉDIA-BAIXA. Compasso (3/4: o baixo marca de 3 em 3 tempos), andamento (~153), tonalidade (ré menor) e a forma (estrofe de 64 compassos que a gravação repete a partir de 105 s) são fiáveis, e as notas longas (lá, sol, si bemol, mi, ré) também. Mas o cantor tem um vibrato de quase um meio-tom para cada lado e arrasta as entradas, por isso as notas curtas de passagem e o ritmo fino são aproximados; na 2.ª estrofe a segunda metade é quase só falada, logo os compassos 33-64 seguem sobretudo a 1.ª estrofe. O mi é cantado baixo (quase mi bemol), mas é mi: fica uma quarta abaixo do lá e o acompanhamento toca lá com sétima por baixo. O lá longo do penúltimo compasso é um instrumento (a voz cala-se aí). Acordes tirados do croma e do baixo do acompanhamento (ré m, lá7, sol m, dó7, si bemol), não de partitura.'


def build(preview_dir=None):
    return chip.render(sys.modules[__name__], 'despedida', preview_dir=preview_dir, lead_octave=1)


if __name__ == "__main__":
    print(build(sys.argv[1] if len(sys.argv) > 1 else None))
