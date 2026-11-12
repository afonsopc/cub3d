#!/usr/bin/env python3
# "Fado Angola" (da compilação "Two hours of Portuguese songs in the Colonial
# War"; autor e intérprete não anunciados na gravação): fado corrido em 2/4,
# lá menor, ~163 bpm (o baixo da guitarra em tónica-dominante a cada tempo e o
# acorde no contratempo). Voz masculina com guitarra portuguesa e viola. A
# gravação está a meio caminho entre lá bemol menor e lá menor (~50 cents
# abaixo do lá 440); escrita em lá menor. Transcrita de ouvido da gravação de
# referência (voz separada com demucs, pYIN + basic-pitch por semicolcheia,
# alinhada às batidas; a 1.ª e a 3.ª passagens conferem a 2.ª); não há
# partitura pública. O cantor oscila um quarto de tom acima da guitarra, por
# isso os pares de meios-tons que a voz alterna (dó/dó#, ré/mib, lá/sib)
# foram escritos na nota da escala. Escrita na oitava do canto e tocada uma
# oitava acima (lead_octave=1). Ciclo de 106 compassos = uma passagem completa
# da gravação: I (anacruse e a frase do ré-fá-mi-ré# sobre fá, si 7 e mi 7, que
# a guitarra toca sozinha na introdução), V (a estrofe, que desce ao mi grave
# do modo frígio, passa por dó maior e acaba no mi longo) e X (a volta da
# guitarra: ré menor, fá, si 7, mi 7, lá menor), que leva de novo à anacruse
# de I, por isso o loop encaixa.
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import chip  # noqa: E402

TITLE = 'Fado Angola'
BPM = 163.3
BAR = 8
TRANSPOSE_TO_REF = -1  # tom da gravação = escrito + isto (lá menor escrito; a gravação soa ~50 cents acima de lá bemol menor)
METHOD = 'ear'

SECTIONS = {
    'I': [
        'r:8', 'r:2 C4:2 C#4:2 D4:2', 'D4:8', 'D4:8',
        'D4:8', 'D4:6 C#4:1 D4:1', 'D4:1 E4:1 F4:6', 'F4:8',
        'F4:8', 'E4:1 D4:1 F4:2 D4:4', 'E4:2 F4:3 E4:2 F4:1', 'E4:1 F4:2 E4:5',
        'E4:8', 'E4:6 F4:1 E4:1', 'E4:4 D#4:4', 'D#4:8',
        'D#4:7 r:1', 'D#4:5 F#4:3', 'E4:8', 'F4:4 E4:1 F4:3',
        'F4:4 E4:2 D4:2', 'C4:2 B3:3 A3:1 G#3:2', 'A3:8', 'A3:8',
        'A3:8', 'A3:5 G#3:2 r:1',
    ],
    'V': [
        'A3:2 F3:4 E3:2', 'F3:3 E3:3 A3:2', 'A3:2 B3:2 A3:4', 'C4:3 A3:1 C4:4',
        'C4:8', 'C4:3 r:1 F3:4', 'F3:4 E3:4', 'E3:4 r:4',
        'E3:1 G#3:1 A3:3 F3:3', 'A3:1 D4:2 B3:2 C4:1 B3:1 D4:1', 'D4:3 C4:1 B3:2 C4:1 E4:1', 'E4:4 F4:1 E4:3',
        'E4:1 C4:1 B3:3 A3:3', 'A3:8', 'A3:2 r:6', 'B3:2 C4:2 C4:1 B3:1 C4:2',
        'C4:8', 'C4:3 r:1 D4:2 C4:2', 'C4:1 D4:5 C4:2', 'D4:4 G3:2 C4:1 D4:1',
        'D4:3 C4:5', 'C4:8', 'C4:2 G3:3 C4:3', 'C4:1 B3:1 A3:3 B3:1 A3:2',
        'A3:8', 'A3:8', 'A3:2 B3:1 A3:1 G#3:3 r:1', 'A3:1 B3:1 r:1 G#3:3 A3:2',
        'G#3:1 E3:1 F3:2 E3:1 F3:3', 'F3:2 E3:2 r:4', 'r:4 E3:2 F3:2', 'E3:2 F3:1 G3:1 G#3:1 A3:3',
        'A3:3 G#3:1 A3:4', 'A3:8', 'r:1 A3:3 B3:2 C4:2', 'A3:4 B3:1 A3:1 F3:2',
        'F3:4 r:4', 'A3:2 r:5 A3:1', 'A3:2 r:2 E3:2 A3:1 B3:1', 'B3:1 C4:1 B3:2 E3:1 A3:2 C4:1',
        'C4:3 B3:3 D4:1 C4:1', 'C4:4 B3:3 C4:1', 'B3:1 C4:1 B3:2 C4:2 r:2', 'A3:2 C4:2 B3:2 A3:2',
        'A3:8', 'A3:4 r:4', 'A3:3 r:1 F3:2 G3:2', 'A3:1 G3:4 B3:2 D4:1',
        'D4:1 B3:2 C4:3 D4:2', 'E4:3 F4:2 G4:3', 'F4:4 E4:1 G4:2 F4:1', 'F4:1 E4:7',
        'E4:8', 'E4:5 F4:3', 'F4:1 E4:5 D4:2', 'E4:6 D4:2',
        'E4:6 C4:1 D4:1', 'E4:1 D4:1 E4:2 D4:3 C4:1', 'B3:4 D4:2 r:1 A3:1', 'A3:8',
        'A3:8', 'A3:4 r:2 A3:2', 'E4:8', 'D4:2 E4:6',
        'E4:8', 'E4:8', 'E4:8', 'E4:4 F4:2 D4:2',
        'D4:2 E4:4 D4:2', 'D4:4 r:4',
    ],
    'X': [
        'F3:2 A3:2 D4:2 A3:2', 'C4:2 A3:2 F3:2 A3:2', 'C4:3 A3:1 F#3:2 A3:2', 'B3:3 A3:1 F#3:2 A3:2',
        'G#3:2 B3:2 E4:2 B3:2', 'D4:3 C4:1 B3:2 G#3:2', 'B3:2 A3:2 G#3:2 F3:2', 'E3:2 G#3:2 B3:2 D4:2',
        'C4:3 B3:1 A3:2 E3:2', 'A3:4 r:4',
    ],
}
CHORDS = {
    'I': [
        'Am', 'E7', 'F', 'F',
        'Dm', 'Dm', 'F', 'F',
        'E7', 'E7', 'Am', 'Am',
        'Am', 'B7', 'B7', 'B7',
        'B7', 'B7', 'E7', 'E7',
        'E7', 'E7', 'Am', 'Am',
        'Am', 'E7',
    ],
    'V': [
        'Am|E7', 'E7', 'E7', 'Am',
        'F', 'F', 'E7', 'E7',
        'E7', 'E7', 'E7', 'E7',
        'E7|Am', 'Am', 'Am', 'G7|C',
        'C', 'C', 'G7', 'G7',
        'G7|C', 'C', 'C|G7', 'Am',
        'Am', 'F', 'Dm|E7', 'E7',
        'E7', 'E7', 'E7', 'Am|E7',
        'Am', 'Am', 'Dm', 'Dm',
        'Dm', 'Dm', 'E7', 'E7',
        'E7', 'E7', 'E7|Am', 'Am|E7',
        'Am', 'Am', 'G7', 'G7',
        'G7', 'G7', 'C', 'C',
        'C', 'B7', 'E7', 'E7',
        'E7', 'E7', 'E7', 'Am',
        'Am', 'Am|E7', 'Am', 'Am',
        'Am|E7', 'E7', 'Am', 'E7',
        'Am|E7', 'Dm',
    ],
    'X': [
        'Dm', 'F', 'F|D7', 'B7',
        'E7', 'E7', 'E7', 'E7',
        'F|Am', 'Am',
    ],
}
FORM = ['I', 'V', 'X']
HARMONY = ['I']  # a frase da guitarra da introdução com terceiras por baixo
REF_START = 80.34  # segundos na gravação de referência (só para a verificação)
CONFIDENCE = 'MÉDIA-BAIXA. Fiáveis: andamento (~163 bpm, 2/4, baixo da guitarra a cada tempo; o fadista abranda nos fins de frase), tonalidade (lá menor com mi 7, si 7, fá e dó; a gravação está ~50 cents abaixo, entre lá bemol e lá) e a forma: o ciclo de 106 compassos repete-se tal e qual (cromagrama do acompanhamento separado com atraso de 212 batidas: 3 s -> 80 s -> 158 s). As notas seguem a 2.ª passagem (80-158 s), conferidas com a 1.ª estrofe (20-80 s, letra e ornamentos diferentes) e com o início da 3.ª. Duvidosos: a voz anda um quarto de tom acima da guitarra e alterna meios-tons vizinhos (dó/dó#, ré/mib, mi/fá, lá/sib), escritos na nota da escala; o ritmo das sílabas e o rubato (achatado num tempo fixo); os compassos com voz fraca (I21-I25, V4-V7, V20-V23, V40-V45, V58-V61), escritos pelo contorno e pela 1.ª estrofe; a volta da guitarra X foi escrita sobre os acordes (só alguns sons da guitarra se ouvem no separado), não nota a nota. Acordes inferidos do baixo e do cromagrama do acompanhamento, não de partitura.'


def build(preview_dir=None):
    return chip.render(sys.modules[__name__], 'fado-angola', preview_dir=preview_dir, lead_octave=1)


if __name__ == "__main__":
    print(build(sys.argv[1] if len(sys.argv) > 1 else None))
