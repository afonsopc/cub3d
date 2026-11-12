#!/usr/bin/env python3
# "Adeus Guiné", canção de soldado da Guerra Colonial (da compilação "Two
# hours of Portuguese songs in the Colonial War"); voz de homem com conjunto
# (acordeão/guitarras e uma nota de sol sempre a soar por baixo, como um
# bordão). Dó menor nas estrofes e dó maior na parte final de cada volta;
# 2/4, ~101 bpm. Transcrita de ouvido da gravação de referência (voz isolada
# com demucs, pYIN + basic-pitch, alinhada às batidas e votada semicolcheia a
# semicolcheia pelas três vezes que a volta é cantada); não há partitura
# pública. Escrita na oitava do canto e tocada uma oitava acima
# (lead_octave=1). Forma: uma volta de 54 compassos, igual às três da
# gravação: A1 e A2 (estrofe em dó menor, "sol, ré-mib, ... dó" a abrir e a
# descida para o mib grave a fechar) e B (a parte em dó maior que sobe ao
# fá e ao sol e acaba no dó). O dó longo do fim de B é o mesmo que abre a
# volta seguinte na gravação, por isso o loop encaixa.
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import chip  # noqa: E402

TITLE = 'Adeus Guiné'
BPM = 101.9
BAR = 8
TRANSPOSE_TO_REF = 0  # tom da gravação = escrito + isto (dó menor/maior, ~-5 cents)
METHOD = 'ear'

SECTIONS = {
    'A1': [
        'C4:4 r:1 G3:3', 'D4:4 Eb4:4', 'Eb4:2 D4:2 Eb4:4', 'C4:8',
        'C4:4 r:4', 'r:8', 'r:2 Bb3:2 A3:2 Ab3:2', 'G3:2 F3:4 r:2',
        'F3:4 r:2 Eb4:2', 'D4:6 r:2', 'r:2 D4:2 C4:1 D4:2 C4:1', 'B3:6 r:2',
        'r:2 D4:2 B3:2 A3:2', 'Ab3:3 G3:5', 'r:2 Ab3:2 G3:2 F3:2', 'G3:1 F3:1 Eb3:6',
        'Eb3:2 r:6',
    ],
    'A2': [
        'r:6 G3:2', 'G3:2 Eb4:3 D4:1 Eb4:2', 'D4:1 C4:3 r:4', 'C4:6 r:2',
        'r:4 C4:2 C4:2', 'F4:2 Eb4:2 C4:2 Bb3:2', 'Ab3:4 r:4', 'r:4 Ab3:4',
        'Ab3:2 G3:2 Ab3:2 r:2', 'G3:2 Ab3:2 r:1 Bb3:3', 'G3:6 D4:2', 'D4:2 Eb4:2 E4:2 F4:2',
        'G4:2 D4:6', 'C4:2 B3:4 C4:2', 'C4:3 r:3 C4:2', 'C4:5 r:3',
        'r:2 G3:6', 'G3:4 r:4',
    ],
    'B': [
        'E4:2 D4:2 C4:4', 'C4:4 D4:2 E4:2', 'E4:4 E4:2 F4:2', 'E4:2 D4:1 C4:3 r:2',
        'r:1 D4:3 C4:1 B3:2 Ab3:1', 'Ab3:2 r:2 C4:3 E3:1', 'F3:2 G3:1 A3:1 Bb3:1 A3:1 G3:2', 'A3:2 B3:4 r:1 B3:1',
        'B3:6 r:2', 'r:2 G3:6', 'G3:4 r:3 D4:1', 'D4:1 C4:4 B3:3',
        'B3:5 r:1 D4:1 E4:1', 'E4:1 F4:4 E4:1 G4:2', 'E4:1 D4:3 C4:3 G3:1', 'A3:2 B3:2 A3:2 r:1 G3:1',
        'A3:1 B3:1 A3:2 r:4', 'A3:1 B3:2 r:1 G3:2 B3:1 D4:1', 'D4:2 C4:2 r:3 C4:1',
    ],
}
CHORDS = {
    'A1': [
        'Cm', 'Cm', 'Cm', 'Cm',
        'Cm', 'Cm', 'Cm', 'Fm',
        'G7', 'D7', 'D7', 'G',
        'G', 'G', 'G', 'Ab',
        'Eb',
    ],
    'A2': [
        'Cm', 'Cm', 'Cm', 'Cm',
        'Cm', 'Fm', 'Fm', 'Fm',
        'Fm', 'Fm', 'Cm', 'C7',
        'G', 'G', 'G', 'C',
        'C', 'C',
    ],
    'B': [
        'C', 'C', 'C', 'C',
        'G7', 'C', 'F', 'G',
        'G', 'G', 'G', 'G7',
        'G', 'G7', 'C', 'C',
        'F', 'G', 'C',
    ],
}
FORM = ['A1', 'A2', 'B']
HARMONY = ['B']  # a parte em dó maior com terceiras por baixo
REF_START = 23.39  # segundos na gravação de referência (só para a verificação)
CONFIDENCE = 'BAIXA-MÉDIA. Gravação ao vivo/antiga, voz com muito vibrato e afinação larga (oscila meio tom), por isso as notas foram votadas pelas três voltas. Fiáveis: compasso 2/4, andamento (~101 bpm, a banda acelera e abranda um pouco), a volta de 54 compassos (repete-se exactamente de 108 em 108 batidas: 23.4 s, 87 s, 151.7 s), dó menor nas estrofes e dó maior em B, e a parte B (igual nas três voltas). Duvidosos: o ritmo silábico das estrofes A (cantadas soltas, mudam de volta para volta), mib/mi e lá/láb em algumas notas de passagem, e os acordes, inferidos do croma do acompanhamento separado, que tem um sol sempre a soar por baixo. O excerto começa com ~10 s do fim de outra faixa e uma introdução instrumental (12-23 s) que não foi incluída.'


def build(preview_dir=None):
    return chip.render(sys.modules[__name__], 'adeus-guine', preview_dir=preview_dir, lead_octave=1, passes=1)


if __name__ == "__main__":
    print(build(sys.argv[1] if len(sys.argv) > 1 else None))
