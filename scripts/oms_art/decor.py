# Objetos do cenário e armas (voxels), todos originais.
import math

from characters import CORE, DARK, GUN, JOINT, METAL, RED, SPARK, VISOR, ACCENT, ARMOR, ARMOR_D, _rbox
from vox import Model, box, cyl_x, cyl_y, cyl_z, ellipsoid, rot_x, rot_y, rot_z, transformed

GOLD = (236, 190, 60)
GOLD_D = (176, 128, 36)
WOOD = (150, 98, 56)
WOOD_D = (104, 66, 38)
STONE = (150, 150, 158)
STONE_D = (104, 104, 114)
LEAF = (70, 160, 76)
LEAF_D = (44, 112, 56)
BLUE = (62, 104, 200)
BLUE_D = (40, 66, 140)
CLAY = (190, 104, 64)
WHITE = (236, 236, 228)
GREEN = (60, 180, 90)
TOXIC = (150, 230, 60)
WATER = (70, 150, 230)
LAMP = (255, 240, 170)
BREAD = (214, 160, 84)
APPLE = (220, 60, 50)
PLATE = (220, 220, 214)
CYAN = VISOR

EMISSIVE = (CORE, VISOR, RED, SPARK, LAMP, TOXIC, (255, 236, 120), (255, 150, 40))


def ammo():
    m = Model()
    _rbox(m, -6, 6, -4, 4, 0, 6, (86, 96, 70), r=1)
    box(m, -6.1, 6.1, -4.1, 4.1, 2.5, 3.5, GOLD)
    for x in (-3.5, 0, 3.5):
        cyl_z(m, x, 0, 1.3, 6, 9, METAL)
        cyl_z(m, x, 0, 0.8, 9, 9.6, CYAN)
    return m


def statue():
    m = Model()
    _rbox(m, -8, 8, -6, 6, 0, 6, STONE_D, r=1)
    _rbox(m, -6.5, 6.5, -5, 5, 6, 8, STONE, r=1)
    # obelisco com orbe por cima
    for i, z in enumerate(range(8, 38, 2)):
        w = 4.2 - i * 0.12
        box(m, -w, w, -w * 0.8, w * 0.8, z, z + 2, STONE if i % 2 == 0 else (160, 160, 168))
    box(m, -1, 1, -3.6, -3.4, 14, 32, GOLD)
    ellipsoid(m, 0, 0, 41, 3.6, 3.6, 3.6, CYAN)
    for a in range(0, 360, 60):
        x, y = math.cos(math.radians(a)) * 3, math.sin(math.radians(a)) * 3
        box(m, x - 0.5, x + 0.5, y - 0.5, y + 0.5, 37, 38, GOLD)
    return m


def barrel(color=(132, 92, 58), band=(70, 72, 80), mark=None):
    m = Model()
    for z in range(0, 22):
        r = 7.0 + 0.9 * math.sin(math.pi * z / 21)
        cyl_z(m, 0, 0, r, z, z + 0.5, color if z not in (3, 4, 17, 18) else band)
    if mark:
        box(m, -2.5, 2.5, -8.6, -7.6, 8, 13, mark)
        box(m, -1, 1, -8.8, -8.4, 9, 12, DARK)
    return m


def plant(pot, leaf, leaf_d, tall=26):
    m = Model()
    for z in range(0, 9):
        r = 4 + z * 0.25
        cyl_z(m, 0, 0, r, z, z + 0.5, pot)
    cyl_z(m, 0, 0, 5.8, 8.5, 9.5, pot)
    cyl_z(m, 0, 0, 0.8, 9, tall - 6, (96, 70, 44))
    for i in range(14):
        a = i * 137.5
        h = 10 + (i * 7) % (tall - 10)
        r = 2 + (tall - h) * 0.35
        x, y = math.cos(math.radians(a)) * r, math.sin(math.radians(a)) * r
        ellipsoid(m, x, y, h, 3.0, 3.0, 2.2, leaf if i % 3 else leaf_d)
    ellipsoid(m, 0, 0, tall - 2, 4, 4, 3, leaf)
    return m


def vase(color, dark):
    m = Model()
    for z in range(0, 24):
        t = z / 23
        r = 3 + 4.5 * math.sin(math.pi * min(1, t * 1.25)) if t < 0.8 else 2.6 + (t - 0.8) * 6
        cyl_z(m, 0, 0, r, z, z + 0.5, color if z % 8 != 4 else dark)
    return m


def pot_clay():
    m = Model()
    for z in range(0, 10):
        r = 5 + 2.5 * math.sin(math.pi * z / 12)
        cyl_z(m, 0, 0, r, z, z + 0.5, CLAY if z != 7 else (150, 76, 46))
    return m


def scrap():
    m = Model()
    import random

    r = random.Random(4)
    for _ in range(16):
        x, y = r.uniform(-14, 14), r.uniform(-6, 6)
        w, d, h = r.uniform(1.5, 4), r.uniform(1.5, 3), r.uniform(1, 3)
        c = r.choice([METAL, JOINT, ARMOR_D, ACCENT, GUN])
        box(m, x - w, x + w, y - d, y + d, 0, h, c)
    cyl_y(m, 3, 3, 2.5, -2, 2, ARMOR)
    box(m, 1.5, 4.5, -2.2, -2.0, 2.5, 3.5, (40, 70, 80))
    return m


def broken_robot():
    m = Model()
    _rbox(m, -10, 6, -4, 4, 0, 5, ARMOR_D, r=1.5)  # tronco deitado
    box(m, -10, 6, -4.1, -3.9, 3.0, 3.8, ACCENT)
    _rbox(m, 9, 16, -4, 4, 0, 6, ARMOR, r=2)  # cabeça
    box(m, 10, 15, -4.3, -4.0, 2.5, 4, (40, 70, 80))
    box(m, -18, -10, -5, -2, 0, 2.5, JOINT)
    box(m, -18, -10, 2, 5, 0, 2.5, JOINT)
    box(m, -3, 2, 5, 9, 0, 2, METAL)
    return m


def ceiling_light(color=LAMP):
    m = Model()
    cyl_z(m, 0, 0, 0.5, 52, 60, METAL)
    for z in range(0, 5):
        cyl_z(m, 0, 0, 2 + z * 1.2, 51 - z, 51.5 - z, (90, 96, 108))
    cyl_z(m, 0, 0, 5.5, 46, 47, color)
    return m


def chandelier():
    m = Model()
    cyl_z(m, 0, 0, 0.5, 50, 60, GOLD_D)
    cyl_z(m, 0, 0, 1.5, 46, 50, GOLD)
    for a in range(0, 360, 45):
        x, y = math.cos(math.radians(a)) * 8, math.sin(math.radians(a)) * 8
        box(m, min(0, x) - 0.5, max(0, x) + 0.5, min(0, y) - 0.5, max(0, y) + 0.5, 46, 47, GOLD)
        cyl_z(m, x, y, 0.9, 47, 49, WHITE)
        cyl_z(m, x, y, 0.6, 49, 50.5, LAMP)
    return m


def battery():
    m = Model()
    _rbox(m, -3, 3, -2, 2, 0, 7, (60, 64, 74), r=0.8)
    box(m, -3.1, 3.1, -2.1, 2.1, 2, 5, GREEN)
    box(m, -1, 1, -1, 1, 7, 8, METAL)
    box(m, -0.5, 0.5, -2.3, -2.1, 3, 4, (240, 240, 240))
    return m


def food():
    m = Model()
    cyl_z(m, 0, 0, 7, 0, 1, PLATE)
    ellipsoid(m, -2, 0, 2.5, 4, 2.5, 2, BREAD)
    ellipsoid(m, 3.5, -1, 2.5, 2, 2, 2, APPLE)
    ellipsoid(m, 3, 2.5, 2.3, 1.8, 1.8, 1.8, (110, 190, 60))
    box(m, 3.4, 3.8, -1.2, -0.8, 4.2, 5.2, (90, 60, 30))
    return m


def gem():
    m = Model()
    for z in range(0, 12):
        r = 3.5 * (1 - abs(z - 4) / 8) if z >= 4 else 0.8 + z * 0.7
        cyl_z(m, 0, 0, max(0.6, r), z, z + 0.5, (80, 230, 140) if z % 3 else (40, 170, 100))
    return m


def trophy():
    m = Model()
    cyl_z(m, 0, 0, 3.5, 0, 1.5, GOLD_D)
    cyl_z(m, 0, 0, 0.9, 1.5, 6, GOLD)
    for z in range(6, 13):
        cyl_z(m, 0, 0, 1.5 + (z - 6) * 0.45, z, z + 0.5, GOLD)
    for s in (-1, 1):
        box(m, s * 4.5 - 0.6, s * 4.5 + 0.6, -0.5, 0.5, 8, 11.5, GOLD_D)
    return m


def lamp():
    m = Model()
    cyl_z(m, 0, 0, 5, 0, 1, (60, 64, 74))
    cyl_z(m, 0, 0, 0.7, 1, 30, (90, 96, 108))
    for z in range(0, 6):
        cyl_z(m, 0, 0, 6 - z * 0.6, 30 + z, 30.5 + z, (60, 150, 110))
    cyl_z(m, 0, 0, 3.6, 29.5, 30.2, LAMP)
    return m


def medkit():
    m = Model()
    _rbox(m, -7, 7, -4, 4, 0, 8, WHITE, r=1)
    box(m, -2, 2, -1, 1, 8, 9.5, (170, 170, 170))
    box(m, -1, 1, -4.3, -4.0, 1.5, 6.5, GREEN)
    box(m, -2.5, 2.5, -4.3, -4.0, 3, 5, GREEN)
    return m


def banner():
    m = Model()
    cyl_z(m, 0, 0, 4, 0, 1.5, (60, 64, 74))
    cyl_z(m, 0, 0, 0.7, 1.5, 48, (120, 120, 128))
    ellipsoid(m, 0, 0, 48.5, 1.3, 1.3, 1.3, GOLD)
    box(m, 0.5, 13, -0.5, 0.5, 26, 46, BLUE)
    box(m, 0.5, 13, -0.6, 0.6, 26, 27, GOLD)
    # estrela de cinco pontas (emblema próprio)
    cx, cz = 6.8, 37
    for i in range(10):
        a = math.radians(90 + i * 36)
        r = 5 if i % 2 == 0 else 2.2
        for t in [k / 6 for k in range(7)]:
            x, z = cx + math.cos(a) * r * t, cz + math.sin(a) * r * t
            box(m, x - 0.5, x + 0.5, -0.8, -0.6, z - 0.5, z + 0.5, GOLD)
    return m


def pillar():
    m = Model()
    _rbox(m, -7, 7, -7, 7, 0, 3, STONE_D, r=1)
    for z in range(3, 52):
        cyl_z(m, 0, 0, 5.2 if z % 9 else 5.6, z, z + 0.5, STONE if z % 9 else STONE_D)
    _rbox(m, -7, 7, -7, 7, 52, 55, STONE_D, r=1)
    return m


def cables():
    m = Model()
    for x, dz in ((-3, 0), (0, 6), (3, 2)):
        for z in range(28 + dz, 60):
            xo = x + math.sin(z * 0.4) * 0.6
            box(m, xo - 0.5, xo + 0.5, -0.5, 0.5, z, z + 1, DARK if x else (180, 60, 40))
    box(m, -1.5, 1.5, -1.5, 1.5, 32, 35, METAL)
    ellipsoid(m, 0, 0, 31, 1.2, 1.2, 1.2, SPARK)
    return m


def table(chairs=False):
    m = Model()
    cyl_z(m, 0, 0, 10, 13, 15, WOOD)
    cyl_z(m, 0, 0, 10.4, 13, 13.6, WOOD_D)
    cyl_z(m, 0, 0, 1.4, 1, 13, (90, 90, 96))
    cyl_z(m, 0, 0, 5, 0, 1.2, (70, 70, 76))
    if chairs:
        for s in (-1, 1):
            x = s * 14
            box(m, x - 3, x + 3, -3, 3, 8, 9.5, WOOD)
            for lx in (-2.3, 2.3):
                for ly in (-2.3, 2.3):
                    box(m, x + lx - 0.5, x + lx + 0.5, ly - 0.5, ly + 0.5, 0, 8, WOOD_D)
            bx = x + s * 2.5
            box(m, bx - 0.6, bx + 0.6, -3, 3, 9.5, 20, WOOD)
        cyl_z(m, -3, -2, 1.2, 15, 18, (200, 220, 230))
        cyl_z(m, 4, 2, 1.2, 15, 18, (200, 220, 230))
    return m


def treasure():
    m = Model()
    _rbox(m, -8, 8, -5, 5, 0, 8, WOOD, r=0.8)
    for x in (-8.1, 8.1):
        box(m, x - 0.3, x + 0.3, -5.2, 5.2, 0, 8, GOLD_D)
    box(m, -8.2, 8.2, -5.3, -5.0, 3, 4.5, GOLD_D)
    import random

    r = random.Random(7)
    for _ in range(30):
        x, y = r.uniform(-6.5, 6.5), r.uniform(-3.5, 3.5)
        h = 8 + 2.5 * (1 - (x / 7) ** 2)
        ellipsoid(m, x, y, h, 1.4, 1.4, 0.8, GOLD if r.random() < 0.8 else (80, 230, 140))
    return m


def well(water):
    m = Model()
    for z in range(0, 11):
        for a in range(0, 360, 12):
            x, y = math.cos(math.radians(a)) * 9, math.sin(math.radians(a)) * 9
            c = STONE if ((z // 3) + (a // 24)) % 2 else STONE_D
            box(m, x - 1.4, x + 1.4, y - 1.4, y + 1.4, z, z + 1, c)
    if water:
        cyl_z(m, 0, 0, 7.6, 8, 9, WATER)
    else:
        cyl_z(m, 0, 0, 7.6, 1, 2, (30, 30, 36))
    return m


DECOR = {
    "ammo": (ammo, {}),
    "statue": (statue, {}),
    "barrel": (barrel, {}),
    "plant-blue": (lambda: plant(BLUE, LEAF, LEAF_D, 30), {}),
    "vase-blue": (lambda: vase(BLUE, BLUE_D), {}),
    "scrap": (scrap, {}),
    "pot-clay": (pot_clay, {}),
    "ceiling-light": (ceiling_light, {"hang": True}),
    "chandelier": (chandelier, {"hang": True}),
    "battery": (battery, {}),
    "food": (food, {}),
    "plant-gold": (lambda: plant(GOLD, (110, 190, 70), (70, 140, 50), 34), {}),
    "barrel-toxic": (lambda: barrel((70, 130, 60), (40, 70, 40), TOXIC), {}),
    "gem": (gem, {}),
    "trophy": (trophy, {}),
    "lamp": (lamp, {}),
    "medkit": (medkit, {}),
    "banner": (banner, {}),
    "pillar": (pillar, {}),
    "broken-robot": (broken_robot, {}),
    "cables": (cables, {"hang": True}),
    "table-chairs": (lambda: table(True), {}),
    "table": (table, {}),
    "treasure": (treasure, {}),
    "well-water": (lambda: well(True), {}),
    "well": (lambda: well(False), {}),
}


# ------------------------------------------------------------------ armas
# Modelos com o cano para -y. Na mão (primeira pessoa) vêem-se de trás.


def blaster():
    m = Model()
    _rbox(m, -2.2, 2.2, -10, 3, 0, 4.2, (70, 76, 90), r=0.8)
    box(m, -2.3, 2.3, -10, 3, 2.6, 3.2, ACCENT)
    cyl_y(m, 0, 2.2, 1.2, -14, -10, METAL)
    cyl_y(m, 0, 2.2, 0.8, -14.6, -14, CYAN)
    box(m, -1.6, 1.6, 0, 4, -6, 0, (50, 54, 64))  # punho
    box(m, -1.8, 1.8, -6, -3, 4.2, 5.2, CYAN)  # mira
    return m


def rifle():
    m = Model()
    _rbox(m, -2.6, 2.6, -16, 8, 0, 5, (64, 70, 82), r=0.8)
    box(m, -2.7, 2.7, -16, 8, 3.2, 3.8, ACCENT)
    cyl_y(m, 0, 2.5, 1.3, -22, -16, METAL)
    cyl_y(m, 0, 2.5, 0.8, -22.6, -22, CYAN)
    box(m, -1.5, 1.5, -8, -4, -6, 0, (50, 54, 64))  # carregador
    box(m, -1.6, 1.6, 2, 6, -6, 0, (50, 54, 64))
    box(m, -1.2, 1.2, -12, -2, 5, 6.5, (40, 44, 52))
    box(m, -0.6, 0.6, -12.4, -11.8, 5.6, 6.2, RED)
    return m


def cannon():
    m = Model()
    _rbox(m, -4, 4, -12, 10, 0, 7, (80, 86, 100), r=1.5)
    box(m, -4.1, 4.1, -2, 2, 0, 7.2, ACCENT)
    for a in range(0, 360, 60):
        x = math.cos(math.radians(a)) * 2.2
        z = 3.5 + math.sin(math.radians(a)) * 2.2
        cyl_y(m, x, z, 0.9, -24, -12, METAL)
    cyl_y(m, 0, 3.5, 1.2, -24.5, -23.8, CYAN)
    cyl_y(m, 0, 3.5, 3.6, -14, -12, (50, 54, 64))
    box(m, -1.8, 1.8, 2, 6, -6, 0, (50, 54, 64))
    return m


def blade():
    m = Model()
    box(m, -1.2, 1.2, -1.2, 1.2, 0, 7, (50, 54, 64))  # cabo
    box(m, -2.8, 2.8, -1.6, 1.6, 7, 8.5, ACCENT)
    for z in range(9, 22):
        w = 1.4 if z < 20 else 1.4 - (z - 19) * 0.45
        box(m, -w, w, -0.5, 0.5, z, z + 1, CYAN if z % 4 else (190, 250, 255))
    return m


def hand_model(gun, recoil=0.0, flash=False, lift=0.0):
    """Arma + mão vista de trás. recoil empurra a arma para trás e para cima."""
    m = Model()
    g = transformed(gun, rot_x((0, 0, 0), -recoil * 6))
    for p, c in g.v.items():
        m.put(p[0], p[1] + recoil, p[2] + lift, c)
    # luva e antebraço a sair de baixo
    _rbox(m, -3.4, 3.4, -1, 9, -10 + lift, -2 + lift, (56, 60, 70), r=1.2)
    _rbox(m, -3.6, 3.6, 1, 6, -4 + lift, 1 + lift, ACCENT, r=1.2)
    box(m, -3.0, 3.0, 9, 20, -16 + lift, -6 + lift, ARMOR)
    if flash:
        ys = min(p[1] for p in gun.v)  # ponta do cano
        ellipsoid(m, 0, ys - 3, 3 + lift, 4, 2.5, 4, (255, 150, 40))
        ellipsoid(m, 0, ys - 4, 3 + lift, 2.2, 1.5, 2.2, (255, 236, 120))
    return m


def blade_swing(stage):
    """0 parada, 1..4 golpe."""
    ang = (0, -35, -75, -45, -10)[stage]
    side = (0, 2, 4, 2, 0)[stage]
    m = Model()
    b = transformed(blade(), rot_x((0, 0, 0), ang))
    for p, c in b.v.items():
        m.put(p[0] + side, p[1], p[2], c)
    _rbox(m, -3.4 + side, 3.4 + side, -2, 3, -4, 2, (56, 60, 70), r=1.2)
    box(m, -3.0 + side, 3.0 + side, 0, 14, -16, -4, ARMOR)
    return m
