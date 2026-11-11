#!/usr/bin/env python3
# Troca, nos mapas, os caminhos da arte antiga (assets/wolf3d) pelos da arte
# original (assets/oms3d) e dá nomes novos aos mapas da campanha. Correu uma
# vez; fica para referência de que ficheiro substituiu qual.
import os
import re
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))

WALLS = {
    "grey-wall0": "stone0", "grey-wall1": "stone1", "grey-wall2": "stone1",
    "grey-wall-hitler": "stone-painting", "grey-wall-nazi": "stone-banner",
    "eagle-wall": "stone-plaque", "eagle-arch": "stone-arch",
    "blue-wall0": "blue0", "blue-wall1": "blue1", "blue-wall-cage": "blue-cell",
    "blue-wall-cage-skull": "blue-cell-robot", "wood-wall": "wood",
    "wood-wall-eagle": "wood-emblem", "wood-wall-hitler": "wood-painting",
    "red-wall": "brick", "purple-wall": "cave", "door": "door", "door-frame": "door-frame",
    "electric-door": "lift-door", "electric-door-button": "lift-panel",
    "lever-down": "lever-down", "lever-up": "lever-up",
}
BILLBOARDS = {
    "ammo": "ammo", "armour": "statue", "barrel": "barrel", "blue-plant": "plant-blue",
    "blue-pot": "vase-blue", "bones": "scrap", "brown-pot": "pot-clay",
    "ceiling-light": "ceiling-light", "chandelier": "chandelier", "dog-food": "battery",
    "food": "food", "gold-plant": "plant-gold", "green-barrel": "barrel-toxic",
    "holy-cross": "gem", "holy-cup": "trophy", "lamp": "lamp", "medic-kit": "medkit",
    "nazi-flag": "banner", "pillar": "pillar", "skeleton-ground": "broken-robot",
    "skeleton-hang": "cables", "table-and-chairs": "table-chairs", "table": "table",
    "treasure": "treasure", "well-water": "well-water", "well": "well",
}
ITEMS = {"pistol": "blaster", "machinegun": "rifle", "gatling": "cannon", "knife": "blade"}
SOUNDS = {
    "atkpistol": "pistol", "atkmachinegun": "rifle", "atkgatling": "cannon", "atkknife": "blade",
    "bonus1": "pickup1", "bonus2": "pickup2", "bonus3": "pickup3", "closedoor": "door-close",
    "opendoor": "door-open", "dogbark": "hound-bark", "dogdeath": "hound-down",
    "getammo": "get-ammo", "getgatling": "get-cannon", "getmachine": "get-rifle",
    "halt": "alert", "health1": "heal1", "health2": "heal2", "hitwall": "bump",
    "nazifire": "sentinel-fire", "noammo": "empty", "pushwall": "secret-wall",
    "getthem": "music-1", "wonderin": "music-2", "xfunkie": "music-3", "copypro": "music-mega",
    **{f"deathscream{n}": f"robot-down{i}" for i, n in enumerate((1, 2, 4, 5, 6, 7, 8, 9), 1)},
}
MAPS = {"e1m1": "missao1", "e1m2": "missao2", "e1m1_mega": "missao1_mega", "e1m1_42": "missao1_42"}


def new_path(old):
    rel = old[len("assets/wolf3d/"):]
    if rel.startswith("sounds/"):
        return f"assets/oms3d/sounds/{SOUNDS[rel[7:-4]]}.mp3"
    rel = rel[len("textures/"):]
    if rel.startswith("hud/"):
        return f"assets/oms3d/textures/{rel}"
    if rel.startswith("items/"):
        _, kind, rest = rel.split("/", 2)
        return f"assets/oms3d/textures/items/{ITEMS[kind]}/{rest}"
    if rel.startswith("billboards/soldier/"):
        return "assets/oms3d/textures/billboards/sentinel/" + rel.split("/", 2)[2]
    if rel.startswith("billboards/dog/"):
        return "assets/oms3d/textures/billboards/hound/" + rel.split("/", 2)[2]
    if rel.startswith("billboards/"):
        return f"assets/oms3d/textures/billboards/{BILLBOARDS[rel[11:-4]]}.bmp"
    name = rel[:-4]
    suffix = ""
    if name.endswith("-dark"):
        name, suffix = name[:-5], "-dark"
    return f"assets/oms3d/textures/{WALLS[name]}{suffix}.bmp"


def main():
    missing = []
    mapdir = os.path.join(ROOT, "maps")
    for fn in sorted(os.listdir(mapdir)):
        if not fn.endswith(".cub"):
            continue
        p = os.path.join(mapdir, fn)
        s = open(p).read()

        def sub(m):
            try:
                n = new_path(m.group(0))
            except KeyError:
                missing.append(m.group(0))
                return m.group(0)
            if not os.path.exists(os.path.join(ROOT, n)):
                missing.append(n)
            return n

        s = re.sub(r"assets/wolf3d/[A-Za-z0-9_./-]+?\.(?:bmp|mp3)", sub, s)
        s = s.replace("assets/fonts/wolf3d", "assets/fonts/hud")
        s = s.replace("assets/textures/wolf3d.bmp", "assets/textures/missao.bmp")
        for a, b in sorted(MAPS.items(), key=lambda kv: -len(kv[0])):
            s = s.replace(f"maps/{a}.cub", f"maps/{b}.cub")
        open(p, "w").write(s)
        base = fn[:-4]
        if base in MAPS:
            os.rename(p, os.path.join(mapdir, MAPS[base] + ".cub"))
    if missing:
        print("em falta:", sorted(set(missing)))
        sys.exit(1)


if __name__ == "__main__":
    main()
