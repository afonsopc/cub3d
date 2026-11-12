#!/usr/bin/env python3
# Gera toda a arte do cub3d (defesa do forte, estilo VGA do início dos anos
# 90): paredes, portas, objetos, inimigos de uma invasão fictícia (atirador,
# soldado de capacete, comissário e cão de ataque), armas, barra de estado
# com a cara do soldado português, algarismos e cartazes. Tudo desenhado aqui por código
# (voxels de baixa resolução e píxeis escritos à mão), sem imagens de
# terceiros. Uso (precisa de numpy e pillow):
#
#   python3 scripts/oms_art/gen.py          # escreve em assets/oms3d, etc.
#
# Os BMP saem em 8 bits com a paleta própria de palette.py; o magenta
# (255,0,255) é o transparente. Os sprites desenham-se a 64x64 e gravam-se
# a 128x128 com píxeis duplicados (o aspeto grosso dos jogos da época).
import math
import os
import sys

import numpy as np
from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
ROOT = os.path.abspath(os.path.join(HERE, "..", ".."))

from vox import render  # noqa: E402
import characters as ch  # noqa: E402
import decor as dc  # noqa: E402
import fpweapons as fw  # noqa: E402
import hud as hd  # noqa: E402
import palette  # noqa: E402
import walls as wl  # noqa: E402

OUT = os.path.join(ROOT, "assets", "oms3d", "textures")
MAG = np.array([255, 0, 255], dtype=np.uint8)


def save(arr, rel, root=OUT):
    """arr: HxWx3 (RGB, magenta = transparente) ou HxWx4 (alfa 0 = transparente)."""
    path = os.path.join(root, rel)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    a = np.asarray(arr)
    if a.shape[2] == 4:
        rgb = a[..., :3].astype(float)
        mask = a[..., 3] == 0
    else:
        rgb = a.astype(float)
        mask = np.all(a.astype(int) == MAG, axis=2)
    idx = palette.snap(np.clip(rgb, 0, 255))
    idx[mask] = 255
    out = Image.fromarray(idx.astype(np.uint8), "P")
    out.putpalette(palette.flat())
    out.save(path)


def x2(a):
    return np.repeat(np.repeat(a, 2, axis=0), 2, axis=1)


def sprite(model, yaw=0.0, pitch=12.0, px=1.0, center=False, emissive=ch.EMISSIVE, **kw):
    """Desenha a 64x64 e devolve 128x128 com píxeis duplicados."""
    r = render(model, size=(64, 64), yaw=yaw, pitch=pitch, px=px, anchor=kw.pop("anchor", (32, 62)),
               emissive=emissive, center=center, **kw)
    return x2(r)


def dark(img, k=0.68):
    return np.clip(np.asarray(img, float) * k, 0, 255)


# ------------------------------------------------------------------ main

DIRS = {
    "front": 0,
    "front-right": 45,
    "right": 90,
    "back-right": 135,
    "back": 180,
    "back-left": 225,
    "left": 270,
    "front-left": 315,
}


# pasta -> farda. Os nomes de pasta antigos ficam (os mapas apontam para lá):
# sentinel/ atirador, legiao/ soldado de capacete, gnr/ comissário, hound/ cão.
GUARDS = {"sentinel": ch.ATIRADOR, "legiao": ch.SOLDADO, "gnr": ch.COMISSARIO}


def characters(only=None):
    walk = [(26, 22, 0.0), (0, 0, 0.6), (-26, -22, 0.0), (0, 0, 0.6)]
    for folder, sp in GUARDS.items():
        if only and folder not in only:
            continue
        d = f"billboards/{folder}"
        for name, yaw in DIRS.items():
            save(sprite(ch.human(sp), yaw=yaw), f"{d}/still-{name}.bmp")
            for i, (l, a, b) in enumerate(walk, 1):
                save(sprite(ch.human(sp, leg=l, arm=a, bob=b), yaw=yaw), f"{d}/{name}{i}.bmp")
        save(sprite(ch.human(sp, pose="aim")), f"{d}/aim.bmp")
        save(sprite(ch.human(sp, pose="shoot", flash=True)), f"{d}/shoot.bmp")
        save(sprite(ch.human(sp, pose="halt")), f"{d}/halt.bmp")
        save(sprite(ch.human(sp, pose="hit", blood=1)), f"{d}/hit.bmp")
        for s in range(1, 6):
            save(sprite(ch.human_death(sp, s), center=True), f"{d}/death{s}.bmp")
    if only and "hound" not in only:
        return
    for name, yaw in DIRS.items():
        for i in range(4):
            save(sprite(ch.dog(phase=i / 4), yaw=yaw), f"billboards/hound/{name}{i + 1}.bmp")
    for i, j in enumerate((-10, -30, -18), 1):
        save(sprite(ch.dog(phase=0.0, jaw=j, gait=0.2)), f"billboards/hound/bite{i}.bmp")
    save(sprite(ch.dog(phase=0.25, jaw=-8, gait=0.3, blood=1), yaw=8), "billboards/hound/hit.bmp")
    for s in range(1, 4):
        save(sprite(ch.dog_death(s), center=True), f"billboards/hound/death{s}.bmp")


def decor_items():
    for name, (fn, opt) in dc.DECOR.items():
        m = fn()
        if opt.get("hang"):
            zmax = max(p[2] for p in m.v)
            r = sprite(m, pitch=14, px=1.0, anchor=(32, zmax * math.cos(math.radians(14)) + 1),
                       emissive=dc.EMISSIVE, ground_shadow=False)
        else:
            r = sprite(m, pitch=16, px=1.3, emissive=dc.EMISSIVE)
        save(r, f"billboards/{name}.bmp")
    for name, fn in dc.PIXEL.items():
        save(x2(fn()), f"billboards/{name}.bmp")
    import flags

    save(flags.pole_flag(), "billboards/banner.bmp")  # já a 128x128


def fit(rgba, size):
    a = rgba[..., 3] > 0
    ys, xs = np.where(a)
    crop = rgba[ys.min() : ys.max() + 1, xs.min() : xs.max() + 1]
    im = Image.fromarray(crop, "RGBA")
    W, H = size
    k = min(W / im.width, H / im.height)
    im = im.resize((max(1, int(im.width * k)), max(1, int(im.height * k))), Image.NEAREST)
    out = Image.new("RGBA", size, (0, 0, 0, 0))
    out.paste(im, ((W - im.width) // 2, (H - im.height) // 2))
    return np.array(out)


def weapons():
    E = dc.EMISSIVE
    # pastas antigas: blaster = pistola, rifle = FBP, cannon = metralhadora, blade = faca
    guns = {"blaster": (dc.pistol, (103, 69), False), "rifle": (dc.fbp, (78, 24), True),
            "cannon": (dc.g3, (104, 30), True)}
    for name, (fn, icon, two) in guns.items():
        g = fn()
        for i, fr in enumerate(fw.gun_frames(name)):
            save(fr, f"items/{name}/screen/{i}.bmp")
        side = render(g, yaw=90, pitch=0, px=1.5, anchor=(32, 32), emissive=E, ground_shadow=False, size=(64, 64))
        save(fit(x2(side), icon), f"items/{name}/icon.bmp")
    for i, fr in enumerate(fw.knife_frames()):
        save(fr, f"items/blade/screen/{i}.bmp")
    side = render(dc.knife(), yaw=90, pitch=0, px=1.5, anchor=(32, 50), emissive=E, ground_shadow=False, size=(64, 64))
    save(fit(x2(side), (74, 94)), "items/blade/icon.bmp")


def hud():
    for s in range(7):
        for look in range(3):
            save(hd.hud_bar(s, look), f"hud/stats/{s}-{look}.bmp")
    save(hd.hud_bar(6, 1, dead=True), "hud/stats/7-0.bmp")
    save(hd.hud_bar(0, 1, cheat=True), "hud/stats/7-1.bmp")
    save(hd.hud_bar(6, 1, dead=True), "hud/stats/7-2.bmp")
    fdir = os.path.join(ROOT, "assets", "fonts", "hud")
    for c in "%0123456789":
        save(hd.digit(c), f"{ord(c)}.bmp", root=fdir)


def main():
    what = sys.argv[1:] or ["walls", "chars", "decor", "weapons", "hud", "posters"]
    if "walls" in what:
        for name, img in wl.walls().items():
            save(img, f"{name}.bmp")
            save(dark(img), f"{name}-dark.bmp")
    if "chars" in what:
        characters()
    if "decor" in what:
        decor_items()
    if "weapons" in what:
        weapons()
    if "hud" in what:
        hud()
    if "posters" in what:
        t = os.path.join(ROOT, "assets", "textures")
        save(hd.poster_mission(), "missao.bmp", root=t)


if __name__ == "__main__":
    main()
