#!/usr/bin/env python3
# "Pátria Ditosa" (António Lages, 1967, cantada por Maria Pereira), 4/4,
# sol maior na estrofe e sol menor no refrão, como na gravação (+11 cents).
# Transcrita de ouvido da gravação de referência (voz separada com demucs,
# pYIN + basic-pitch, compassos marcados pelo baixo e pelos acordes do
# acompanhamento); não há partitura pública. A voz canta com um vibrato muito
# largo (±1 meio-tom) e glissandos de baixo para cima, aqui escritos como
# arpejos. Escrita à altura do canto; o lead toca uma oitava acima. Ciclo de 34
# compassos: estrofe V (10) + refrão R1, R2, R3 (8 cada); o refrão acaba na
# dominante (ré 7), que volta ao sol da estrofe, por isso o loop encaixa.
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import chip  # noqa: E402

TITLE = 'Pátria Ditosa'
BPM = 129.3
BAR = 16
TRANSPOSE_TO_REF = 0  # tom da gravação = escrito + isto (o lead soa uma oitava acima)
METHOD = 'ear'

SECTIONS = {
    'V': [
        'G3:4 B3:3 D4:2 G4:7', 'Bb3:2 A3:2 G3:8 F#3:4', 'G3:5 A3:1 C4:1 E4:1 G4:4 F#4:1 D4:1 C4:1 B3:1', 'A3:3 B3:1 C4:12',
        'A3:7 B3:1 C4:2 D4:2 F#4:2 A4:2', 'A4:3 G4:1 F#4:6 E4:2 D4:2 F#4:2', 'A4:4 G4:1 E4:1 C4:1 Bb3:1 A3:4 C4:1 D4:1 C4:2', 'Bb3:8 A3:2 Bb3:2 A3:2 G3:2',
        'G3:2 A3:1 Bb3:1 C4:1 D4:2 F4:1 G4:2 F4:1 Eb4:1 D4:1 Bb3:1 A3:2', 'G3:4 F#3:5 A3:2 r:2 F#4:3',
    ],
    'R1': [
        'G4:4 Eb4:1 Bb3:7 C4:1 D4:1 Eb4:1 F4:1', 'F4:2 Eb4:14', 'Eb4:2 D4:3 B3:2 D4:1 G4:1 A4:2 C5:2 A4:3', 'A4:1 G4:11 F#4:4',
        'B3:1 C#4:1 D4:2 C4:1 G3:5 Bb3:1 Eb4:3 D4:2', 'C4:6 B3:3 Bb3:2 G3:1 F#3:4', 'D4:2 E4:1 G4:1 A4:8 G4:4', 'A4:2 G4:12 r:2',
    ],
    'R2': [
        'r:3 Bb4:12 A4:1', 'A4:1 G4:1 E4:1 Eb4:1 D4:1 C4:8 E4:1 A4:2', 'A4:1 G4:6 F#4:3 E4:1 D4:1 B3:1 C4:1 F#4:1 Bb4:1', 'A4:1 C5:1 Bb4:1 A4:2 G4:2 E4:1 Eb4:1 D4:7',
        'D4:5 C4:1 B3:2 A3:2 C4:1 C#4:1 Eb4:1 E4:3', 'Eb4:1 C#4:1 D4:2 C4:12', 'C4:8 A3:1 G3:6 C4:1', 'D4:3 C4:1 B3:1 D4:11',
    ],
    'R3': [
        'D4:5 A4:2 Bb4:8 A4:1', 'A4:2 G4:2 F4:1 Eb4:2 D4:3 C4:1 Bb3:2 C4:2 D4:1', 'G4:1 A4:4 G4:1 F#4:1 F4:2 F#4:1 Eb4:1 C4:1 B3:2 C4:2', 'Bb4:5 A4:1 G4:1 A4:1 G4:2 E4:1 C#4:1 D4:4',
        'D4:1 C4:1 B3:1 C4:1 C#4:1 D4:5 C#4:1 Bb3:1 G3:3 C#4:1', 'Eb4:4 D4:3 C4:9', 'C4:2 D4:5 Eb4:1 F4:1 F#4:1 G4:3 A4:3', 'G4:16',
    ],
}
CHORDS = {
    'V': [
        'G', 'D7', 'G', 'D7',
        'D7', 'D7', 'D7', 'Gm|D7',
        'Gm', 'D7',
    ],
    'R1': [
        'G|C7', 'Cm', 'Cm|C7', 'Gm',
        'Gm|D7', 'D7', 'D7', 'Eb|D7',
    ],
    'R2': [
        'Gm', 'Cm', 'Cm|D7', 'Cm|Gm',
        'Gm|D7', 'D7', 'D7', 'Cm|D7',
    ],
    'R3': [
        'D7|Gm', 'Gm|Cm', 'Cm', 'Cm|D7',
        'Gm', 'D7', 'D7', 'Cm|D7',
    ],
}
FORM = ['V', 'R1', 'R2', 'R3']
REF_START = 7.37  # segundos na gravação de referência (só para a verificação)
CONFIDENCE = 'MÉDIA-BAIXA. Tonalidade (sol: estrofe maior, refrão menor com dó menor e ré 7), andamento (~129 bpm, 4/4, banda ao vivo com ligeira deriva) e harmonia do refrão são fiáveis (baixo e cromagrama do acompanhamento separado). A melodia é a linha da 1.ª estrofe (7.4-70.5 s), com o vibrato (±1 meio-tom) alisado; estrofe e R1 batem certo com a 2.ª estrofe (~0.5 por passo, letra diferente) e R2 com a repetição final (0.6-0.85). Duvidoso: os glissandos de entrada (G3-B3-D4-G4, si natural ou bemol), as notas graves fracas dos compassos 2 e 10 da estrofe e os ornamentos de uma semicolcheia. Sem partitura nem MIDI.'


def build(preview_dir=None):
    return chip.render(sys.modules[__name__], 'patria-ditosa', preview_dir=preview_dir, lead_octave=1)


if __name__ == "__main__":
    print(build(sys.argv[1] if len(sys.argv) > 1 else None))
