# Inimigos em voxels de baixa resolução (aspeto de píxeis grossos dos jogos
# VGA): uma força de invasão estrangeira e fictícia, ao estilo das fardas
# soviéticas dos anos 60/70 -- o atirador (gorro de pele com estrela
# vermelha e espingarda de assalto tipo AK), o soldado de capacete com
# pistola-metralhadora de tambor tipo PPSh, o comissário (capote e boné de
# pala com banda vermelha, pistola) e o cão de ataque. Só adultos e só
# combatentes. Tudo original.
import math

from vox import Model, box, chain, cyl_x, cyl_y, cyl_z, ellipsoid, rot_x, rot_y, rot_z, transformed

SKIN = (206, 150, 112)
SKIN_D = (164, 110, 80)
EYE = (24, 18, 16)
HAIR = (40, 30, 24)
BLOOD = (150, 10, 12)
BLOOD_D = (96, 4, 8)
FLASH_Y = (255, 236, 120)
FLASH_O = (255, 150, 40)
GUN = (44, 44, 48)
GUN_L = (96, 96, 104)
STOCK = (110, 66, 34)
WHITE = (236, 232, 220)

EMISSIVE = (FLASH_Y, FLASH_O)

STAR = (210, 20, 24)

# sentinel/: atirador com espingarda de assalto
ATIRADOR = dict(
    name="atirador", coat=None, shirt=(128, 122, 78), shirt_d=(98, 92, 56), tie=None,
    trousers=(106, 100, 62), shoes=(26, 22, 18), hat="ushanka", hat_c=(96, 72, 46),
    band=(70, 52, 32), gun="ak", belt=(66, 46, 26), strap=False, moustache=False, boots=True,
)
# legiao/: soldado de capacete com pistola-metralhadora de tambor
SOLDADO = dict(
    name="soldado", coat=None, shirt=(104, 108, 64), shirt_d=(78, 82, 46), tie=None,
    trousers=(96, 94, 58), shoes=(24, 20, 16), hat="helmet", hat_c=(70, 86, 52), band=None,
    gun="ppsh", belt=(70, 50, 28), strap=True, moustache=True, boots=True,
)
# gnr/: comissário de capote e boné de pala
COMISSARIO = dict(
    name="comissario", coat=(96, 90, 72), coat_d=(70, 64, 50), shirt=(120, 116, 80),
    tie=(60, 56, 40), trousers=(70, 66, 50), shoes=(18, 16, 14), hat="cap",
    hat_c=(88, 92, 60), band=STAR, gun="pistol", belt=(60, 40, 22), moustache=True,
    boots=True,
)

def _gun(m, kind, xf, flash=False):
    """Arma na mão direita (lado -x), cano para -y a partir da mão (z~15)."""
    hx = -7.0
    if kind == "pistol":
        box(m, hx - 0.8, hx + 0.8, -4.5, 0.5, 14.5, 16.5, GUN, xf=xf)
        box(m, hx - 0.6, hx + 0.6, -0.5, 1.0, 12.5, 15, GUN_L, xf=xf)
        tip = -5.5
    elif kind == "smg":
        box(m, hx - 0.9, hx + 0.9, -9, 3, 14.5, 16.5, GUN, xf=xf)
        box(m, hx - 0.5, hx + 0.5, -12, -9, 15, 16, GUN_L, xf=xf)
        box(m, hx - 0.6, hx + 0.6, -5, -3, 11, 14.5, GUN, xf=xf)  # carregador
        box(m, hx - 0.8, hx + 0.8, 3, 7, 14, 16, STOCK, xf=xf)
        tip = -13
    elif kind == "ppsh":
        box(m, hx - 0.9, hx + 0.9, -7, 2, 14.5, 16.5, GUN, xf=xf)
        box(m, hx - 1.0, hx + 1.0, -11, -7, 14.8, 16.4, GUN_L, xf=xf)  # manga
        box(m, hx - 1.2, hx + 1.2, -6.5, -2.5, 10.5, 14.5, GUN, xf=xf)  # tambor
        box(m, hx - 0.9, hx + 0.9, 2, 7, 13.5, 16, STOCK, xf=xf)
        tip = -12
    elif kind == "ak":
        box(m, hx - 0.8, hx + 0.8, -4, 3, 14.5, 16.5, GUN, xf=xf)
        box(m, hx - 0.9, hx + 0.9, -8, -4, 14.6, 16.4, STOCK, xf=xf)  # guarda-mão
        box(m, hx - 0.4, hx + 0.4, -13, -8, 15.2, 16, GUN, xf=xf)
        box(m, hx - 0.6, hx + 0.6, -3, -1, 11.5, 14.5, GUN_L, xf=xf)  # carregador curvo
        box(m, hx - 0.6, hx + 0.6, -4, -2.5, 10, 11.5, GUN_L, xf=xf)
        box(m, hx - 0.9, hx + 0.9, 3, 8, 13.6, 16, STOCK, xf=xf)
        tip = -13
    else:
        box(m, hx - 0.9, hx + 0.9, -6, 7, 14, 16.5, STOCK, xf=xf)
        box(m, hx - 0.5, hx + 0.5, -16, -6, 15, 16.2, GUN, xf=xf)
        box(m, hx - 0.6, hx + 0.6, -2, 1, 16.5, 17.2, GUN_L, xf=xf)
        tip = -17
    if flash:
        ellipsoid(m, hx, tip - 2, 15.5, 2.6, 1.6, 2.6, FLASH_O, xf=xf)
        ellipsoid(m, hx, tip - 3, 15.5, 1.5, 1.2, 1.5, FLASH_Y, xf=xf)


def human(sp, leg=0.0, arm=0.0, bob=0.0, pose="still", flash=False, blood=0):
    """Figura humana adulta (~50 voxels de altura), a olhar para -y.
    pose: still, walk, aim, shoot, halt, hit."""
    m = Model()

    def lift(f=None):
        def g(p):
            if f is not None:
                p = f(p)
            return (p[0], p[1], p[2] + bob)

        return g

    tro = sp["trousers"]
    # pernas
    for side, sw in ((-1, leg), (1, -leg)):
        cx = side * 2.3
        hip = (cx, 0, 22)
        knee = (cx, 0, 11)
        bend = max(0.0, -sw) * 0.7
        f_th = lift(rot_x(hip, sw))
        f_sh = lift(chain(rot_x(knee, bend), rot_x(hip, sw)))
        box(m, cx - 1.8, cx + 1.8, -1.8, 1.8, 11, 22, tro, xf=f_th)
        if sp.get("boots"):
            box(m, cx - 1.9, cx + 1.9, -1.9, 1.9, 2, 11, tro, xf=f_sh)
            box(m, cx - 2.0, cx + 2.0, -2.0, 2.0, 1.5, 9, sp["shoes"], xf=f_sh)
        else:
            box(m, cx - 1.8, cx + 1.8, -1.8, 1.8, 2, 11, tro, xf=f_sh)
        box(m, cx - 1.9, cx + 1.9, -4.2, 1.8, 0, 2, sp["shoes"], xf=f_sh)
    f = lift()
    shirt = sp["shirt"]
    shirt_d = sp.get("shirt_d", shirt)
    # tronco
    box(m, -6.0, 6.0, -3.0, 3.0, 21, 38, shirt, xf=f)
    if sp.get("coat"):
        coat, coat_d = sp["coat"], sp["coat_d"]
        box(m, -6.2, 6.2, -3.3, 3.3, 21, 38, coat, xf=f)
        # aba da gabardina (fica parada, as pernas aparecem por baixo)
        for z in range(12, 22):
            w = 6.2 + (22 - z) * 0.08
            box(m, -w, w, -3.4, 3.4, z, z + 1, coat if z % 6 else coat_d, xf=f)
        box(m, -6.3, 6.3, -3.5, 3.5, 25, 26.5, sp["belt"], xf=f)  # cinto
        box(m, -0.8, 0.8, -3.7, -3.4, 25, 26.5, (170, 150, 90), xf=f)
        # lapelas abertas, camisa e gravata
        box(m, -2.0, 2.0, -3.5, -3.2, 30, 38, sp["shirt"], xf=f)
        box(m, -0.6, 0.6, -3.7, -3.4, 29, 37.5, sp["tie"], xf=f)
        box(m, -3.2, -2.0, -3.7, -3.3, 31, 38, coat_d, xf=f)
        box(m, 2.0, 3.2, -3.7, -3.3, 31, 38, coat_d, xf=f)
        for z in (23, 28, 33):
            box(m, -1.6, -1.0, -3.6, -3.3, z, z + 0.6, (40, 34, 26), xf=f)
    else:
        box(m, -5.8, 5.8, -3.0, 3.0, 21, 23.5, sp["belt"], xf=f)
        box(m, -0.9, 0.9, -3.2, -2.9, 21.5, 23.2, (190, 160, 70), xf=f)  # fivela
        box(m, -5.7, 5.7, -2.9, 2.9, 34, 38, shirt_d, xf=f)  # ombros
        for x in (-3.0, 3.0):
            box(m, x - 1.2, x + 1.2, -3.2, -2.9, 29, 32, shirt_d, xf=f)  # bolsos
        if sp.get("strap"):
            # correia a tiracolo
            for i in range(14):
                z = 37 - i
                x = -4.5 + i * 0.68
                box(m, x - 0.6, x + 0.6, -3.3, 3.3, z, z + 1, sp["belt"], xf=f)
        box(m, -1.6, 1.6, -3.2, -2.8, 36.5, 38, shirt_d, xf=f)  # gola
    # pescoço e cabeça
    box(m, -1.5, 1.5, -1.5, 1.5, 38, 40, SKIN_D, xf=f)
    box(m, -3.5, 3.5, -3.3, 3.3, 39.5, 48, SKIN, xf=f)
    box(m, -3.6, 3.6, -1.0, 3.4, 44, 48, HAIR, xf=f)  # cabelo dos lados/trás
    box(m, -3.8, -3.4, -1.2, 1.0, 42.5, 45, SKIN_D, xf=f)  # orelhas
    box(m, 3.4, 3.8, -1.2, 1.0, 42.5, 45, SKIN_D, xf=f)
    box(m, -2.2, -1.2, -3.5, -3.3, 44, 45, EYE, xf=f)  # olhos
    box(m, 1.2, 2.2, -3.5, -3.3, 44, 45, EYE, xf=f)
    box(m, -0.5, 0.5, -3.9, -3.3, 42.5, 44.5, SKIN_D, xf=f)  # nariz
    if sp.get("moustache"):
        box(m, -1.8, 1.8, -3.6, -3.3, 41.4, 42.3, HAIR, xf=f)
    else:
        box(m, -1.2, 1.2, -3.5, -3.3, 41.2, 41.8, (140, 80, 64), xf=f)
    hat, hc = sp["hat"], sp["hat_c"]
    if hat == "fedora":
        cyl_z(m, 0, 0, 5.8, 47.5, 48.5, hc, xf=f)
        cyl_z(m, 0, 0, 3.9, 48.5, 52, hc, xf=f)
        cyl_z(m, 0, 0, 4.0, 48.5, 49.7, sp["band"], xf=f)
        box(m, -0.8, 0.8, -4.0, 4.0, 51.5, 52.3, hc, xf=f)  # vinco
    elif hat == "ushanka":
        # gorro de pele com abas e estrela vermelha à frente
        cyl_z(m, 0, 0, 4.4, 46.5, 51, hc, xf=f)
        cyl_z(m, 0, 0, 4.7, 46.5, 48, sp["band"], xf=f)
        for sx in (-1, 1):
            box(m, sx * 4.0 - 0.7, sx * 4.0 + 0.7, -2.2, 2.4, 41.5, 47, sp["band"], xf=f)
        box(m, -1.0, 1.0, -4.9, -4.6, 48.6, 50.4, STAR, xf=f)
    elif hat == "helmet":
        ellipsoid(m, 0, 0, 47.5, 4.9, 4.9, 4.8, hc, xf=f)
        cyl_z(m, 0, 0, 5.3, 46, 47, sp.get("shirt_d", hc), xf=f)
        box(m, -3.0, 3.0, -3.6, -3.3, 45, 45.6, EYE, xf=f)  # sombra da pala
        box(m, -0.8, 0.8, -5.0, -4.7, 48.5, 50, STAR, xf=f)  # estrela
    else:  # boné de pala
        cyl_z(m, 0, 0, 3.9, 47.5, 49.5, hc, xf=f)
        cyl_z(m, 0, 0, 4.0, 47.5, 48.3, sp["band"], xf=f)
        box(m, -4.1, 4.1, -4.2, 0.6, 49.5, 51, hc, xf=f)
        box(m, -3.0, 3.0, -5.6, -3.3, 47.5, 48.1, (16, 16, 18), xf=f)  # pala
        box(m, -0.7, 0.7, -4.3, -4.0, 48.2, 49.6, (230, 200, 60), xf=f)  # estrela
    # braços (lado -x leva a arma)
    two_hand = sp["gun"] in ("rifle", "smg", "ak", "ppsh")
    for side in (-1, 1):
        sh = (side * 6.6, 0, 36.5)
        if pose in ("aim", "shoot"):
            a = -85 if side == -1 or two_hand else -10
        elif pose == "halt":
            # "Alto!": braço esticado para o lado, mão aberta (não é
            # nenhuma saudação de braço erguido)
            a = -15 if side == -1 else 0
        elif pose == "hit":
            a = 30
        elif two_hand:
            a = -45 if side == -1 else -55
        else:
            a = (-20 + arm * 0.4) if side == -1 else -arm
        turn = 0
        if pose in ("aim", "shoot") and (side == -1 or two_hand):
            turn = 24 * -side
        fa = lift(chain(rot_x(sh, a), rot_z(sh, turn)))
        if pose == "halt" and side == 1:
            fa = lift(chain(rot_x(sh, -20), rot_y(sh, -78)))
        sleeve = sp["coat"] if sp.get("coat") else shirt
        box(m, side * 6.6 - 1.4, side * 6.6 + 1.4, -1.4, 1.4, 24, 37.5, sleeve, xf=fa)
        box(m, side * 6.6 - 1.0, side * 6.6 + 1.0, -1.0, 1.0, 21.5, 24, SKIN, xf=fa)  # mão
        if side == 1 and two_hand and pose not in ("halt", "hit"):
            pass
    gun_arm = -85 if pose in ("aim", "shoot") else (30 if pose == "hit" else (-45 if two_hand else (-20 + arm * 0.4)))
    if pose == "halt":
        gun_arm = -15
    gturn = 24 if pose in ("aim", "shoot") else 0
    fg = lift(chain(rot_x((-6.6, 0, 36.5), gun_arm), rot_z((-6.6, 0, 36.5), gturn)))
    # a arma sai da mão (z ~ 22.5 do braço), no referencial do braço
    gm = Model()
    _gun(gm, sp["gun"], None, flash=flash)
    for (x, y, z), c in gm.v.items():
        p = fg((x + 0.4, y, z + 7.5))
        m.put(*p, c)
    if blood:
        import random

        r = random.Random(blood)
        for _ in range(3 + blood * 2):
            x, z = r.uniform(-4, 4), r.uniform(24, 36)
            box(m, x - 0.7, x + 0.7, -3.9, -3.4, z - 0.7, z + 0.7, BLOOD, xf=f)
    return m


def human_death(sp, stage):
    """5 fases: atingido, joelhos a ceder, a cair, no chão, imóvel em sangue."""
    base = human(sp, pose="hit", blood=stage + 1)
    if stage == 1:
        m = transformed(base, rot_x((0, 0, 0), 10))
        return m
    angle = (0, 0, 28, 62, 86, 90)[stage]
    # cai para o lado (como nos jogos da época), pivô no pé
    squash = (1, 1, 0.82, 0.7, 0.62, 0.62)[stage]
    m = transformed(base, lambda p: (p[0], p[1], p[2] * squash))
    m = transformed(m, rot_y((5, 0, 0), -angle))
    if stage >= 4:
        r = 7 if stage == 4 else 10
        for x in range(-r - 18, r - 14):
            for y in range(-r // 2, r // 2 + 1):
                if ((x + 16) / r) ** 2 + (y / (r / 2)) ** 2 <= 1:
                    m.put(x, y, 0, BLOOD if (x + y) % 5 else BLOOD_D)
    return m


# ---------------------------------------------------------------- cão de guarda

FUR = (120, 96, 70)
FUR_D = (78, 60, 40)
SADDLE = (30, 26, 24)
NOSE = (20, 16, 14)
TONGUE = (200, 80, 80)
TEETH = (236, 232, 220)


def dog(phase=0.0, jaw=0.0, gait=1.0, blood=0):
    m = Model()
    bob = abs(math.sin(phase * 2 * math.pi)) * 0.8 * gait

    def lift(f=None):
        return lambda p: (lambda q: (q[0], q[1], q[2] + bob))(f(p) if f else p)

    legs = [(-3.0, -7, 0.0), (3.0, 7, 0.0), (3.0, -7, 0.5), (-3.0, 7, 0.5)]
    for x, y, off in legs:
        a = math.sin((phase + off) * 2 * math.pi) * 32 * gait
        hip = (x, y, 11)
        fl = lift(rot_x(hip, a))
        box(m, x - 1.2, x + 1.2, y - 1.3, y + 1.3, 5, 12, FUR, xf=fl)
        knee = (x, y, 5)
        fk = lift(chain(rot_x(knee, -abs(a) * 0.5), rot_x(hip, a)))
        box(m, x - 0.9, x + 0.9, y - 0.9, y + 0.9, 1, 5.5, FUR, xf=fk)
        box(m, x - 1.1, x + 1.1, y - 2.0, y + 1.0, 0, 1.2, FUR_D, xf=fk)
    f = lift()
    ellipsoid(m, 0, 0, 13, 4.3, 9.5, 3.8, FUR, xf=f)
    ellipsoid(m, 0, 1, 15.3, 4.0, 6.5, 1.9, SADDLE, xf=f)  # sela preta do dorso
    ellipsoid(m, 0, -7, 13.5, 4.0, 3.4, 4.0, FUR, xf=f)  # peito
    # cauda
    fz = lift(rot_x((0, 9, 14), 50))
    cyl_y(m, 0, 14, 1.0, 9, 17, FUR_D, xf=fz)
    # pescoço e cabeça
    box(m, -2.0, 2.0, -11, -7, 14, 19, FUR, xf=f)
    box(m, -3.0, 3.0, -14, -9.5, 17, 22, FUR, xf=f)
    box(m, -3.1, 3.1, -11, -9.5, 20, 22, SADDLE, xf=f)
    box(m, -1.6, 1.6, -17.5, -13.5, 18, 20.4, FUR_D, xf=f)  # focinho
    box(m, -0.8, 0.8, -17.9, -17.2, 19.4, 20.6, NOSE, xf=f)
    fj = lift(rot_x((0, -13.5, 18), jaw))
    box(m, -1.4, 1.4, -17, -13.5, 16.8, 18, FUR_D, xf=fj)
    if jaw < -5:
        box(m, -1.2, 1.2, -16.8, -16.4, 17.9, 18.6, TEETH, xf=f)
        box(m, -0.8, 0.8, -16.0, -14.0, 17.2, 17.8, TONGUE, xf=fj)
    box(m, -2.2, -1.2, -14.2, -13.9, 20.5, 21.3, (220, 160, 40), xf=f)  # olhos
    box(m, 1.2, 2.2, -14.2, -13.9, 20.5, 21.3, (220, 160, 40), xf=f)
    for s in (-1, 1):  # orelhas em bico
        for i in range(4):
            w = 1.0 - i * 0.22
            box(m, s * 1.8 - w, s * 1.8 + w, -11.5, -10.5, 22 + i, 23 + i, SADDLE, xf=f)
    if blood:
        box(m, -3.7, -3.4, -3, 1, 12, 15, BLOOD, xf=f)
    return m


def dog_death(stage):
    base = dog(phase=0.0, jaw=-15 if stage == 1 else 0, gait=0.3, blood=1)
    ang = (25, 70, 90)[stage - 1]
    m = transformed(base, rot_y((4, 0, 0), -ang))
    if stage == 3:
        m = transformed(m, lambda p: (p[0], p[1], p[2] * 0.8))
        for x in range(-14, -2):
            for y in range(-6, 7):
                if ((x + 8) / 6) ** 2 + (y / 6) ** 2 <= 1:
                    m.put(x, y, 0, BLOOD)
    return m
