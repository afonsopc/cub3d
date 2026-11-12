#!/usr/bin/env python3
# Gera as dez canções em 8 bits (MP3 mono 22050 Hz, 56 kbps, em loop) nesta
# pasta e, opcionalmente, pré-escutas de 30 s noutra pasta. Escreve também
# loops.json com a duração e o tempo de cada faixa.
#
#   python3 scripts/oms_art/music/render_all.py [pasta_das_previews]
#
# Precisa de numpy e do `lame`.
import importlib
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
sys.dont_write_bytecode = True

SONGS = [
    "hino_mocidade",
    "a_portuguesa",
    "angola_e_nossa",
    "hino_legiao",
    "marcha_legiao",
    "deus_patria_rei",
    "partida_ultramar",
    "a_nossa_terra",
    "patria_ditosa",
    "hora_despedida",
]


def main():
    preview = sys.argv[1] if len(sys.argv) > 1 else None
    info = {}
    for name in SONGS:
        mod = importlib.import_module(name)
        r = mod.build(preview)
        r["title"] = mod.TITLE
        r["method"] = mod.METHOD
        info[r.pop("file")] = r
        print(name, r)
    with open(os.path.join(HERE, "loops.json"), "w") as f:
        json.dump(info, f, ensure_ascii=False, indent=2)
        f.write("\n")


if __name__ == "__main__":
    main()
