# Personagens originais (voxels): a Sentinela (robô de patrulha, substitui o
# soldado) e o Cão-Robô (substitui o cão). Nada aqui vem de outro jogo.
from vox import Model, box, chain, cyl_x, cyl_y, cyl_z, ellipsoid, rot_x, rot_y, rot_z, transformed

ARMOR = (212, 206, 188)
ARMOR_D = (164, 158, 144)
ACCENT = (230, 118, 40)
JOINT = (66, 70, 82)
METAL = (118, 126, 140)
DARK = (40, 42, 50)
VISOR = (70, 232, 244)
CORE = (120, 246, 255)
RED = (255, 64, 56)
GUN = (58, 62, 72)
FLASH_Y = (255, 236, 120)
FLASH_O = (255, 150, 40)
SPARK = (255, 220, 90)

EMISSIVE = (VISOR, CORE, RED, FLASH_Y, FLASH_O, SPARK)


def _rounded(x0, x1, y0, y1, z0, z1, r=1.5):
    """Caixa com as arestas verticais cortadas (para não parecer um tijolo)."""

    def inside(x, y, z):
        dx = max(x0 + r - x, 0, x - (x1 - r))
        dy = max(y0 + r - y, 0, y - (y1 - r))
        return dx * dx + dy * dy <= r * r

    return inside


def _rbox(m, x0, x1, y0, y1, z0, z1, color, r=1.5, xf=None):
    from vox import _fill

    _fill(m, (x0, x1), (y0, y1), (z0, z1), _rounded(x0, x1, y0, y1, z0, z1, r), color, xf)


def sentinel(leg=0.0, arm=0.0, bob=0.0, pose="still", flash=False, dim=False):
    """leg/arm: ângulos de balanço ao andar (graus). pose: still, aim, shoot,
    halt, hit."""
    m = Model()
    vis = (40, 70, 80) if dim else VISOR
    core = (50, 80, 90) if dim else CORE
    up = (0, 0, bob)

    def lift(f=None):
        def g(p):
            if f is not None:
                p = f(p)
            return (p[0], p[1], p[2] + bob)

        return g

    # pernas (pivô na anca)
    for side, sw in ((-1, leg), (1, -leg)):
        cx = side * 4.0
        hip = (cx, 0, 19)
        knee_bend = max(0.0, -sw) * 0.6
        f_thigh = lift(rot_x(hip, sw))
        box(m, cx - 1.5, cx + 1.5, -1.5, 1.5, 12, 19, JOINT, xf=f_thigh)
        knee = (cx, 0, 12)
        f_shin = lift(chain(rot_x(knee, knee_bend), rot_x(hip, sw)))
        _rbox(m, cx - 2.2, cx + 2.2, -2.0, 2.0, 2.5, 12, ARMOR, r=1.0, xf=f_shin)
        box(m, cx - 2.0, cx + 2.0, -2.6, -2.0, 9, 12.5, ACCENT, xf=f_shin)  # joelheira
        box(m, cx - 2.6, cx + 2.6, -4.5, 2.2, 0, 2.5, DARK, xf=f_shin)  # pé
        box(m, cx - 2.6, cx + 2.6, -4.6, -3.8, 0.5, 2.0, ACCENT, xf=f_shin)
    f = lift()
    # bacia e tronco
    _rbox(m, -6, 6, -2.8, 2.8, 18, 21.5, JOINT, r=1.2, xf=f)
    _rbox(m, -7.5, 7.5, -4.2, 4.2, 21.5, 34, ARMOR, r=2.5, xf=f)
    _rbox(m, -6.5, 6.5, -4.4, 4.4, 21.5, 23, ARMOR_D, r=2.2, xf=f)  # cinto
    box(m, -7.6, 7.6, -4.0, 4.0, 30, 31, ACCENT, xf=f)  # faixa
    cyl_y(m, 0, 27.5, 2.2, -4.8, -3.5, DARK, xf=f)
    cyl_y(m, 0, 27.5, 1.4, -5.0, -4.6, core, xf=f)  # núcleo
    # mochila
    _rbox(m, -5, 5, 3.5, 7, 23, 33, METAL, r=1.2, xf=f)
    for z in (25, 27.5, 30):
        box(m, -3.5, 3.5, 6.9, 7.2, z, z + 0.6, DARK, xf=f)
    # pescoço e cabeça
    cyl_z(m, 0, 0, 2.0, 33.5, 36, JOINT, xf=f)
    _rbox(m, -4.8, 4.8, -4.2, 4.2, 36, 44, ARMOR, r=2.2, xf=f)
    box(m, -4.9, 4.9, -3.2, 3.2, 43.5, 44.5, ARMOR_D, xf=f)
    box(m, -3.8, 3.8, -4.6, -4.0, 39, 41, vis, xf=f)  # viseira
    box(m, -4.9, -4.4, -2, 2, 38, 42, ACCENT, xf=f)  # orelhas
    box(m, 4.4, 4.9, -2, 2, 38, 42, ACCENT, xf=f)
    cyl_z(m, 3, 2, 0.5, 44, 48, METAL, xf=f)  # antena
    box(m, 2.5, 3.5, 1.5, 2.5, 48, 49, RED, xf=f)
    # ombros
    for side in (-1, 1):
        ellipsoid(m, side * 9.2, 0, 32, 3.0, 3.2, 2.6, ACCENT, xf=f)
    # braços: o direito (lado -x no ecrã quando olha para nós) leva a arma
    for side in (-1, 1):
        sh = (side * 9.5, 0, 31)
        if side == -1:
            if pose in ("aim", "shoot"):
                a = -80
            elif pose == "halt":
                a = -15
            elif pose == "hit":
                a = 25
            else:
                a = -25 + arm * 0.4
        else:
            if pose == "halt":
                a = -160
            elif pose in ("aim", "shoot"):
                a = -70
            elif pose == "hit":
                a = 30
            else:
                a = -arm
        fa = lift(rot_x(sh, a))
        box(m, side * 9.5 - 1.3, side * 9.5 + 1.3, -1.3, 1.3, 24, 30, JOINT, xf=fa)
        _rbox(m, side * 9.5 - 2.0, side * 9.5 + 2.0, -2.0, 2.0, 17, 24.5, ARMOR, r=0.9, xf=fa)
        box(m, side * 9.5 - 1.6, side * 9.5 + 1.6, -1.6, 1.6, 15, 17, DARK, xf=fa)  # mão
        if side == -1:
            # arma: cano para a frente a partir da mão
            box(m, -11.2, -7.8, -9.5, 1.5, 14, 17, GUN, xf=fa)
            box(m, -10.4, -8.6, -12, -9.5, 14.6, 16.4, METAL, xf=fa)
            box(m, -10.0, -9.0, -12.4, -12, 15, 16, VISOR if not dim else DARK, xf=fa)
            if pose in ("aim", "shoot") and side == -1:
                if pose == "aim":
                    # mira a apontar para nós a partir de um braço erguido
                    pass
        if side == 1 and pose == "halt":
            box(m, 8.0, 11.0, -1.0, 1.0, 13, 15.5, DARK, xf=fa)  # palma aberta
    if pose == "aim":
        # a mira olha para quem vê: viseira mais forte e cano à frente do peito
        box(m, -3.8, 3.8, -4.6, -4.0, 39, 41, RED, xf=f)
    if flash:
        # clarão de disparo à frente do cano (no ecrã: à frente do peito)
        ellipsoid(m, -6, -16, 26, 3.5, 1.5, 3.5, FLASH_O, xf=f)
        ellipsoid(m, -6, -17.5, 26, 2.0, 1.2, 2.0, FLASH_Y, xf=f)
    return m


def sentinel_hit():
    m = sentinel(pose="hit")
    m = transformed(m, rot_x((0, 0, 0), 8))
    from vox import ellipsoid as el

    for x, z in ((-5, 30), (4, 36), (7, 25)):
        el(m, x, -6, z, 0.8, 0.8, 0.8, SPARK)
    return m


def sentinel_death(stage):
    """5 fases: faíscas, inclina, cai, no chão, desfeito."""
    from vox import ellipsoid as el

    dim = stage >= 3
    base = sentinel(pose="hit" if stage < 3 else "still", dim=dim)
    angle = (12, 35, 62, 86, 90)[stage - 1]
    m = transformed(base, rot_y((8, 0, 0), -angle))
    if stage == 1:
        for x, z in ((-4, 38), (5, 30), (0, 26), (6, 40)):
            el(m, x, -6, z, 0.9, 0.9, 0.9, SPARK)
    if stage >= 2:
        for x, z in ((-14, 12), (-6, 22)):
            el(m, x, -6, z, 0.8, 0.8, 0.8, SPARK) if stage == 2 else None
    if stage >= 4:
        # peças soltas e fumo no chão
        for x, y in ((-20, -3), (-16, 4), (14, 2)):
            box(m, x - 1, x + 1, y - 1, y + 1, 0, 1.5, METAL)
    if stage == 5:
        m = transformed(m, lambda p: (p[0], p[1], p[2] * 0.7))
        box(m, -26, -22, -2, 2, 0, 2, ARMOR_D)  # cabeça solta
    return m


# ---------------------------------------------------------------- Cão-Robô

HB = (96, 104, 120)  # corpo
HB_D = (64, 70, 84)
HY = (236, 180, 52)  # amarelo de aviso


def hound(phase=0.0, jaw=0.0, dim=False, gait=1.0):
    """Quadrúpede: comprido ao longo de y (frente = -y). phase em [0,1)."""
    import math

    m = Model()
    eye = (60, 30, 30) if dim else RED
    bob = abs(math.sin(phase * 2 * math.pi)) * 0.8 * gait
    lift = lambda f=None: (lambda p: (lambda q: (q[0], q[1], q[2] + bob))(f(p) if f else p))
    # patas: pares diagonais em fase
    legs = [(-4, -9, 0.0), (4, 9, 0.0), (4, -9, 0.5), (-4, 9, 0.5)]
    for x, y, off in legs:
        a = math.sin((phase + off) * 2 * math.pi) * 30 * gait
        hip = (x, y, 13)
        fl = lift(rot_x(hip, a))
        box(m, x - 1.4, x + 1.4, y - 1.4, y + 1.4, 6, 13, HB_D, xf=fl)
        knee = (x, y, 6)
        fk = lift(chain(rot_x(knee, -abs(a) * 0.5), rot_x(hip, a)))
        box(m, x - 1.2, x + 1.2, y - 1.2, y + 1.2, 1.5, 6.5, JOINT, xf=fk)
        box(m, x - 1.6, x + 1.6, y - 2.6, y + 1.4, 0, 1.6, DARK, xf=fk)
    f = lift()
    _rbox(m, -5.5, 5.5, -11, 11, 11, 18, HB, r=2.0, xf=f)
    box(m, -5.6, 5.6, -3, 3, 17.6, 18.4, HY, xf=f)  # faixa de aviso no dorso
    for yy in (-8, -4, 0, 4, 8):
        box(m, -5.7, 5.7, yy, yy + 1.2, 12.5, 13.2, HB_D, xf=f)
    # cauda (antena)
    fz = lift(rot_x((0, 11, 17), 35))
    cyl_y(m, 0, 17, 0.6, 11, 17, METAL, xf=fz)
    box(m, -0.8, 0.8, 16.5, 18, 16.3, 17.7, RED if not dim else DARK, xf=fz)
    # pescoço e cabeça
    box(m, -2.5, 2.5, -14, -10, 15, 20, HB_D, xf=f)
    _rbox(m, -4, 4, -19, -12, 18, 24, HB, r=1.6, xf=f)
    box(m, -4.1, 4.1, -15, -14, 18, 24, HY, xf=f)
    # focinho com maxila
    _rbox(m, -2.8, 2.8, -23, -18, 19.5, 22.5, HB, r=0.8, xf=f)
    fj = lift(rot_x((0, -18, 19.5), jaw))
    box(m, -2.6, 2.6, -22.5, -18, 17.5, 19.3, HB_D, xf=fj)
    if jaw < -5:
        box(m, -2.2, 2.2, -22.4, -21.8, 19.3, 20.0, (230, 230, 230), xf=f)  # dentes
    # olhos e orelhas
    box(m, -3.2, -1.6, -19.6, -19.0, 21.5, 23, eye, xf=f)
    box(m, 1.6, 3.2, -19.6, -19.0, 21.5, 23, eye, xf=f)
    for s in (-1, 1):
        box(m, s * 3 - 0.8, s * 3 + 0.8, -15, -13.4, 24, 27, HB_D, xf=f)
    return m


def hound_death(stage):
    from vox import ellipsoid as el

    base = hound(phase=0.0, jaw=-15 if stage == 1 else 0, dim=stage >= 2, gait=0.3)
    ang = (25, 70, 90)[stage - 1]
    m = transformed(base, rot_y((6, 0, 0), -ang))
    if stage == 3:
        m = transformed(m, lambda p: (p[0], p[1], p[2] * 0.75))
    if stage == 1:
        for x, z in ((-3, 22), (4, 16), (0, 26)):
            el(m, x, -12, z, 0.8, 0.8, 0.8, SPARK)
    return m
