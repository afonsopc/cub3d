#!/usr/bin/env python3
# "Moçambique é Portugal!" (anos 60), marcha patriótica cantada por uma voz
# de homem com banda (metais, baixo tónica-quinta, caixa), em 2/4, lá maior
# com empréstimos do menor (fá e dó naturais nas descidas), ~93 semínimas
# por minuto (a banda marca as colcheias). Transcrita de ouvido da gravação
# de referência (voz separada com demucs, altura por soma de harmónicos +
# pYIN + basic-pitch, alinhada às batidas e votada entre as duas vezes que a
# 1.ª parte é cantada); não há partitura pública. Escrita na oitava do canto
# e tocada uma oitava acima (lead_octave=1).
# Forma (o ciclo é a 2.ª metade da gravação, de 86 s ao fim): P, a frase que
# desce pelos acordes lá - sol - fá - mi, com respostas da banda; V, a
# estrofe sobre lá e mi; R, a parte nova em ré / mi / lá; C, o remate que
# passa por fá# menor e fá e acaba no lá agudo, que volta ao lá do início.
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import chip  # noqa: E402

TITLE = 'Moçambique é Portugal!'
BPM = 93.1
BAR = 8
TRANSPOSE_TO_REF = 0  # tom da gravação = escrito + isto (a voz anda ~30 cents abaixo)
METHOD = 'ear'

SECTIONS = {
    'P': [
        'r:4 E3:4', 'E3:2 A3:3 B3:1 A3:2', 'G3:8', 'r:8',
        'r:8', 'r:6 G3:2', 'G3:1 G#3:1 A3:4 G3:1 A3:1', 'F3:8',
        'r:8', 'r:8', 'r:1 G3:1 A3:2 C4:1 A3:1 Ab3:2', 'Bb3:1 A3:2 Ab3:3 G3:1 F3:1',
        'E3:4 r:4', 'r:8', 'r:8',
    ],
    'V': [
        'r:4 D3:1 E3:3', 'F3:1 G#3:1 A3:2 r:1 E3:1 C#3:1 D3:1', 'C3:4 r:4', 'F3:1 E3:4 D3:1 C#3:2',
        'C#3:1 B2:3 r:2 B2:1 C#3:1', 'D3:1 C#3:1 B2:2 r:1 C#3:1 D3:1 E3:1', 'E3:8', 'E3:5 r:3',
        'r:2 C#3:1 D3:2 E3:3', 'r:1 F3:1 A3:1 C4:1 B3:3 C4:1', 'B3:1 C#4:2 A3:3 r:2', 'r:1 E4:1 C#4:2 D4:2 C#4:1 B3:1',
        'D4:1 B3:3 r:3 G#3:1', 'A3:2 C4:1 B3:1 C4:1 E4:1 r:1 B3:1', 'G#3:2 A3:6',
    ],
    'R': [
        'A3:7 r:1', 'r:2 E3:1 G#3:1 A3:1 B3:1 C#4:2', 'B3:1 A3:2 B3:1 D4:1 B3:1 A3:1 B3:1', 'G#3:1 F#3:7',
        'F#3:1 F3:4 A3:2 B3:1', 'B3:2 E3:1 G#3:2 A3:1 B3:2', 'A3:2 G#3:2 F#3:2 G#3:1 F#3:1', 'F#3:2 E3:6',
        'E3:3 r:5', 'r:1 G3:2 F#3:2 E3:2 r:1', 'D3:1 C#3:1 F#3:3 G#3:1 F#3:1 E3:1', 'E3:1 D3:2 r:3 C#3:2',
        'B2:2 D3:3 C#3:2 E3:1', 'E3:2 r:3 C#3:2 r:1', 'r:1 B2:1 C#3:1 E3:1 D3:1 C#3:2 E3:1', 'E3:8',
        'E3:4 r:4',
    ],
    'C': [
        'r:2 A3:1 C#4:1 E4:1 D4:1 C#4:1 B3:1', 'B3:1 A3:2 B3:1 D4:1 C#4:1 A3:1 B3:1', 'A3:1 F#3:7', 'F#3:4 r:4',
        'r:2 E3:1 G#3:3 B3:2', 'A3:1 G#3:1 E4:1 C#4:2 G3:2 A3:1', 'B3:2 A3:6', 'A3:6 r:2',
        'r:2 D4:1 A3:2 G3:1 A3:1 r:1', 'E3:2 D3:2 r:2 C#3:1 D3:1', 'E3:4 r:3 C#3:1', 'C#3:2 A3:4 G#3:1 B3:1',
        'B3:1 G#3:2 D4:1 G#3:4', 'G#3:3 A3:1 B3:2 r:2', 'r:2 G#3:3 B3:1 A3:2', 'G#3:4 F#3:1 E3:2 B3:1',
        'B3:1 E4:1 A4:6', 'A4:8', 'A4:2 r:6',
    ],
}
CHORDS = {
    'P': [
        'A', 'A7', 'G', 'G7',
        'G7', 'G', 'G|C7', 'C7',
        'F', 'F', 'F|Fm', 'Fm',
        'E7', 'E7', 'E',
    ],
    'V': [
        'Dm', 'Dm|A7', 'Am', 'A',
        'E7', 'E7', 'A', 'A7',
        'Dm', 'Dm|E7', 'A7', 'A',
        'E7', 'E7', 'A',
    ],
    'R': [
        'A|E7', 'A', 'A', 'D',
        'Dm|E7', 'E', 'E7', 'A',
        'A', 'A7', 'A|E7', 'E',
        'E7', 'E7', 'E7', 'A',
        'A',
    ],
    'C': [
        'A', 'A|E7', 'D', 'D|E7',
        'E', 'E7', 'F#m', 'F#m',
        'F', 'F|A', 'A', 'A',
        'E', 'E', 'E7', 'E7',
        'A', 'A', 'A',
    ],
}
FORM = ['P', 'V', 'R', 'C']
REF_START = 86.03  # segundos na gravação de referência (só para a verificação)
CONFIDENCE = 'BAIXA-MÉDIA. Gravação velha, voz com muito vibrato e afinação instável (~30 cents abaixo, e varia), mal apanhada pelo pYIN. Fiáveis: o andamento (colcheia da banda a 0.322 s, constante de 5 s ao fim), o compasso binário com baixo tónica-quinta, a tonalidade (lá, a acabar no lá agudo), a sequência de acordes lá - sol - fá - mi da frase P (repete-se 42.5 s depois, igual), a estrofe V (conferida entre as duas passagens) e as notas longas (lá, sol, fá, mi, fá#). Incertos: o ritmo silábico dentro de cada compasso, dó/dó# e fá/fá# quando a voz fica a meio, as notas de R e C (cantadas só uma vez, sem segunda passagem para votar), os lás agudos (podem ser metais e não a voz) e os acordes de fá# menor e fá no remate, inferidos do croma da banda separada, não de partitura. A introdução (recitada sobre a banda, 5-43 s) fica de fora.'


def build(preview_dir=None):
    return chip.render(sys.modules[__name__], 'mocambique-e-portugal', preview_dir=preview_dir, lead_octave=1)


if __name__ == "__main__":
    print(build(sys.argv[1] if len(sys.argv) > 1 else None))
