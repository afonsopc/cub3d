#!/usr/bin/env python3
# Plantas originais da campanha (versão Estado Novo) e validador dos mapas.
#
#   python3 scripts/oms_art/maps.py          # reescreve os mapas e valida
#   python3 scripts/oms_art/maps.py --check  # só valida
#
# Ficção de arcada: o jogador é um soldado do Exército Português que defende
# o quartel-general (missao1, "Defesa do Quartel-General") e o forte
# (missao2, "Defesa do Forte") de uma invasão estrangeira fictícia. As plantas são desenhadas aqui, sala a sala, e não seguem nenhum
# nível de outro jogo. missao1_mega e missao1_42 usam a planta da missao1
# com o cabeçalho próprio de cada variante.
#
# Legenda (iguais nos 4 mapas da campanha):
#   paredes  f pedra   g arco   k retrato   i estandarte   J lápide
#            q cartaz da época   r rombo com o estandarte dos invasores
#            a azulejo   B painel de azulejos   b grade   c arrecadação de armas
#            j madeira   l emblema   m retrato em madeira   A Mocidade
#            F tijolo   O rocha   p porta de elevador (parede)
#   portas   v (passa-se de este para oeste)   h (de norte para sul)
#            u porta da saída   s t w x T paredes falsas (segredos)
#            D porta trancada (arsenal / paiol: precisa da chave)
#            V portão de grade (abre-se com a alavanca >)
#   alavancas n saída (precisa dos documentos / cargas)   > portão
#            y segredo para a missao1_mega
#   puzzles  3 chave (arsenal / paiol)   4 planos do Forte / cargas
#   inimigos e atirador invasor   R comissário   L soldado de capacete   d cão
#   jogador  E W (missao1, 4 lugares)   N (missao2)
#   largados : ração   ; munições   } socorros   ( medalha   ) cruz de guerra
#            * ouro   { pistola   [ faca   ] FBP   2 G3
#   objetos  . planta  , candeeiro de teto  / lustre  = vaso de barro
#            ' pipa  ? poço  + poço com água  - mesa  | papéis  " sacos de areia
#            \ esfera armilar  ~ mesa e cadeiras  ! jarra  @ candeeiro
#            # bandeira  $ planta em vaso de latão  % bidão  ^ coluna
#            & lanterna  < máquina de escrever
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import story  # noqa: E402
from collections import deque

ROOT = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
MAPS = os.path.join(ROOT, "maps")
T = "assets/oms3d/textures"
S = "assets/oms3d/sounds"


# ------------------------------------------------------------------ desenho


class Plan:
    def __init__(self, w, h, fill=" "):
        self.w, self.h = w, h
        self.g = [[fill] * w for _ in range(h)]

    def room(self, x0, y0, x1, y1, wall="f", force=False):
        """Paredes no contorno (inclusive) e chão por dentro. Uma parede já
        desenhada (partilhada com outra sala) fica como está, salvo force."""
        for y in range(y0, y1 + 1):
            for x in range(x0, x1 + 1):
                if x in (x0, x1) or y in (y0, y1):
                    if force or self.g[y][x] not in WALLISH:
                        self.g[y][x] = wall
                else:
                    self.g[y][x] = "0"

    def solid(self, x0, y0, x1, y1, wall="f"):
        for y in range(y0, y1 + 1):
            for x in range(x0, x1 + 1):
                self.g[y][x] = wall

    def floor(self, x0, y0, x1, y1):
        for y in range(y0, y1 + 1):
            for x in range(x0, x1 + 1):
                self.g[y][x] = "0"

    def put(self, x, y, c):
        cur = self.g[y][x]
        if c in "ny>":
            pass
        elif c in WALLISH or c in "vhustwxTDV":
            if cur not in WALLISH and c not in WALLISH:
                raise ValueError(f"{c!r} at {x},{y} must go on a wall (is {cur!r})")
        elif cur != "0":
            raise ValueError(f"{c!r} at {x},{y} must go on the floor (is {cur!r})")
        self.g[y][x] = c

    def puts(self, pts, c):
        for x, y in pts:
            self.g[y][x] = c

    def seal(self):
        """Fecha com pedra todo o vazio encostado a chão (nichos, cantos)."""
        for y in range(self.h):
            for x in range(self.w):
                if self.g[y][x] != " ":
                    continue
                for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1), (1, 1), (-1, -1), (1, -1), (-1, 1)):
                    nx, ny = x + dx, y + dy
                    if 0 <= nx < self.w and 0 <= ny < self.h and self.g[ny][nx] not in WALLISH | {" "} | set("vhustwxTDVny>"):
                        self.g[y][x] = "f"
                        break

    def text(self):
        rows = []
        for r in self.g:
            s = "".join(r).replace("0", " ").rstrip()
            rows.append(s)
        while rows and not rows[0]:
            rows.pop(0)
        return "\n".join(rows)


WALLISH = set("fgkiJqraBbcjlmAFOp1")


def missao1():
    """Defesa do Quartel-General. Os invasores já entraram: parte-se da rua
    (sul) para a portaria e o corredor dos gabinetes. Cadeia de objetivos:
    a chave do arsenal está com o oficial de dia (fundo do corredor, a
    leste); o arsenal (porta da fechadura dourada, no átrio) tem a FBP e a
    alavanca que abre o portão do pátio; no gabinete do comando, depois do
    pátio, estão os planos do Forte, sem os quais a alavanca da saída não
    funciona. Salas pequenas, esquinas e portas para as emboscadas."""
    P = Plan(44, 38)
    P.room(15, 13, 27, 23, "f")  # átrio (marco: esfera armilar e 4 colunas)
    P.room(15, 3, 27, 13, "a")  # pátio dos azulejos (marco: o poço)
    P.room(18, 32, 24, 37, "f")  # rua / entrada (início)
    P.room(16, 26, 26, 32, "f")  # portaria
    P.room(10, 26, 16, 32, "f")  # arrecadação (segredo, alavanca do bónus)
    P.room(6, 23, 37, 26, "j")  # corredor dos gabinetes
    P.room(0, 22, 6, 29, "j")  # sala de espera
    P.room(37, 20, 43, 28, "j")  # gabinete do oficial de dia (chave)
    P.room(27, 14, 35, 22, "f")  # arsenal (porta trancada, alavanca)
    P.room(35, 14, 40, 19, "f")  # cofre (segredo)
    P.room(5, 12, 15, 21, "F")  # celas
    P.room(8, 4, 15, 11, "f")  # material apreendido (segredo)
    P.room(27, 2, 36, 10, "j")  # gabinete do comando (planos)
    P.room(36, 4, 42, 10, "f")  # sala da saída

    # corredor: porta a meio e dois nichos
    P.put(28, 25, "j")
    P.put(28, 24, "j")
    P.put(28, 24, "v")
    P.floor(12, 23, 12, 23)
    P.floor(33, 23, 33, 23)
    # celas: grade com duas portas e parede entre as celas
    for x in range(6, 15):
        P.put(x, 16, "b")
    P.puts([(10, 16), (10, 13), (10, 14), (10, 15)], "F")
    P.puts([(8, 16), (12, 16)], "h")

    # --- portas
    P.put(21, 32, "h")  # rua -> portaria
    P.put(21, 26, "h")  # portaria -> corredor
    P.put(6, 24, "v")  # corredor -> sala de espera
    P.put(37, 24, "v")  # corredor -> oficial de dia
    P.put(21, 23, "h")  # corredor -> átrio
    P.put(15, 18, "v")  # átrio -> celas
    P.put(27, 18, "D")  # átrio -> arsenal (trancada: chave do arsenal)
    P.put(21, 13, "V")  # átrio -> pátio (portão: alavanca do arsenal)
    P.put(27, 6, "v")  # pátio -> comando
    P.put(36, 7, "u")  # comando -> saída
    P.put(42, 7, "n")  # alavanca da saída (precisa dos planos)
    P.puts([(42, 5), (42, 6), (42, 8), (42, 9)], "p")
    P.put(31, 14, ">")  # alavanca do portão, no arsenal
    # segredos
    P.put(16, 29, "x")  # pedra solta na portaria -> arrecadação
    P.put(10, 29, "y")  # alavanca secreta (nível bónus)
    P.put(35, 16, "x")  # pedra solta no arsenal -> cofre
    P.put(15, 8, "x")  # pedra solta no pátio -> material apreendido

    # --- decoração das paredes
    P.puts([(18, 34), (24, 34)], "i")
    P.put(21, 37, "J")
    P.puts([(18, 26), (24, 26)], "q")
    P.put(26, 29, "r")
    P.puts([(16, 27), (16, 31)], "k")
    P.puts([(9, 23), (15, 23), (25, 23), (31, 23), (35, 23)], "A")
    P.puts([(10, 26), (14, 26), (30, 26), (34, 26)], "m")
    P.put(0, 25, "l")
    P.put(43, 24, "l")
    P.put(40, 20, "m")
    P.puts([(18, 13), (24, 13)], "B")
    P.puts([(15, 15), (27, 15), (15, 21), (27, 21)], "i")
    P.puts([(18, 23), (24, 23)], "k")
    P.puts([(29, 22), (33, 22), (35, 20)], "c")
    P.put(27, 20, "r")
    P.puts([(5, 14), (5, 18)], "r")
    P.puts([(18, 3), (24, 3), (15, 5), (15, 11), (27, 11)], "B")
    P.put(21, 3, "q")
    P.put(31, 2, "k")
    P.puts([(29, 2), (34, 2)], "i")
    P.put(31, 10, "l")
    P.put(39, 4, "J")
    P.put(11, 4, "k")

    # --- jogadores (o primeiro é o local)
    P.puts([(21, 35), (19, 36), (23, 36), (21, 36)], "N")

    # --- objetos, itens e inimigos
    items = {
        # rua
        "#": [(19, 33), (23, 33)], ",": [(21, 34)],
        # portaria
        "-": [(19, 28)], "@": [(17, 27)], ";": [(25, 31)], ":": [(25, 27)],
        "e": [(24, 28), (17, 30)],
        # arrecadação (segredo)
        "*": [(12, 28), (14, 30)], "}": [(11, 31)], "'": [(11, 27)],
    }
    for c, pts in items.items():
        P.puts(pts, c)
    # corredor
    P.puts([(10, 24), (17, 25), (24, 24), (32, 24)], ",")
    P.put(12, 23, "d")
    P.put(8, 25, "e")
    P.put(33, 23, ":")
    # sala de espera
    P.put(3, 25, "~")
    P.puts([(1, 23), (5, 23)], "!")
    P.put(1, 28, "$")
    P.put(4, 27, "e")
    P.put(2, 27, "(")
    P.put(5, 28, ";")
    # oficial de dia: a chave do arsenal na secretária
    P.put(40, 22, "-")
    P.put(39, 21, "<")
    P.put(42, 21, "@")
    P.put(38, 21, "$")
    P.put(41, 21, "3")
    P.puts([(41, 25)], "R")
    P.put(38, 27, "e")
    P.put(42, 27, ":")
    # átrio
    P.puts([(18, 16), (24, 16), (18, 20), (24, 20)], "^")
    P.put(21, 18, "\\")
    P.puts([(21, 16), (21, 20)], "/")
    P.puts([(16, 14), (26, 14), (16, 22), (26, 22)], "!")
    P.puts([(25, 15), (17, 21)], "e")
    P.put(23, 19, "d")
    P.put(16, 17, ";")
    P.put(26, 21, ":")
    # arsenal
    P.put(31, 19, "]")
    P.puts([(29, 21), (33, 21)], ";")
    P.put(34, 21, "%")
    P.put(29, 16, '"')
    P.put(33, 17, "e")
    # cofre
    P.puts([(37, 15), (39, 17)], "*")
    P.puts([(36, 18), (38, 18)], "(")
    P.put(39, 15, ")")
    # celas
    P.put(6, 13, "}")
    P.put(9, 13, "=")
    P.put(7, 14, "&")
    P.put(14, 13, ";")
    P.put(11, 13, ")")
    P.put(13, 19, "e")
    P.put(7, 18, "d")
    P.put(10, 18, ",")
    P.put(6, 20, "-")
    # pátio
    P.put(21, 8, "+")
    P.puts([(17, 5), (25, 5), (17, 11), (25, 11)], ".")
    P.puts([(19, 4), (23, 4)], "=")
    P.puts([(18, 8), (25, 7)], "e")
    P.put(23, 11, "d")
    P.put(16, 4, ";")
    P.put(26, 12, ":")
    # material apreendido
    P.puts([(10, 6), (13, 9)], "*")
    P.put(11, 8, ")")
    P.put(9, 10, '"')
    P.put(14, 5, "|")
    P.put(12, 10, "}")
    # gabinete do comando: os planos na secretária do Major
    P.put(32, 4, "~")
    P.put(32, 6, "4")
    P.put(29, 3, "@")
    P.puts([(28, 3), (35, 3)], "#")
    P.puts([(28, 9), (35, 9)], "$")
    P.put(34, 5, "R")
    P.put(30, 8, "e")
    P.put(31, 9, ":")
    # sala da saída
    P.put(39, 7, ",")
    P.seal()
    return P


def missao2():
    """Defesa do Forte. Chega-se de elevador (sul) à portaria. Cadeia de
    objetivos: o portão do pátio só abre pela alavanca do corpo da guarda
    (leste); a chave do paiol está na caserna (oeste do pátio); o paiol (a
    porta vermelha, a leste do pátio) guarda a G3 e as cargas de demolição,
    e tem a saída para a gruta do mar, onde se fecha a porta do mar."""
    P = Plan(46, 42)
    P.room(16, 17, 28, 29, "f")  # pátio (marco: a bandeira)
    P.room(19, 35, 25, 41, "f")  # elevador (início)
    P.room(17, 29, 27, 35, "f")  # portaria do forte
    P.room(27, 28, 35, 36, "F")  # corpo da guarda (alavanca do portão)
    P.room(8, 28, 17, 36, "F")  # pavilhão prisional
    P.room(11, 36, 17, 41, "F")  # isolamento (segredo)
    P.room(6, 16, 16, 26, "j")  # caserna (chave do paiol)
    P.room(17, 7, 27, 17, "f")  # capela
    P.room(19, 2, 25, 7, "f")  # sacristia (segredo)
    P.room(28, 19, 36, 27, "F")  # paiol (porta trancada, cargas)
    # gruta do mar: rocha maciça com galerias escavadas
    P.solid(29, 0, 45, 18, "O")
    for x0, y0, x1, y1 in (
        (32, 15, 32, 18),  # túnel do paiol
        (30, 9, 36, 14),  # gruta de cima
        (37, 12, 39, 13),  # passagem
        (40, 4, 44, 14),  # gruta funda
        (40, 2, 41, 3),  # subida
        (34, 1, 38, 3),  # porta do mar
    ):
        P.floor(x0, y0, x1, y1)
    P.solid(33, 11, 33, 12, "O")  # rocha no meio da gruta de cima
    P.solid(42, 8, 42, 9, "O")

    # pavilhão prisional: grade com duas celas
    for y, c in zip(range(29, 36), "bvbFbvb"):
        P.put(12, y, "F")
        P.put(12, y, c)
    P.puts([(9, 32), (10, 32), (11, 32)], "F")

    # --- portas
    P.put(22, 35, "h")  # elevador -> portaria
    P.put(22, 29, "V")  # portaria -> pátio (portão: alavanca da guarda)
    P.put(27, 32, "v")  # portaria -> corpo da guarda
    P.put(17, 32, "v")  # portaria -> pavilhão prisional
    P.put(35, 32, ">")  # alavanca do portão
    P.put(22, 17, "h")  # pátio -> capela
    P.put(16, 22, "v")  # pátio -> caserna
    P.put(28, 23, "D")  # pátio -> paiol (trancada: chave do paiol)
    P.put(32, 19, "h")  # paiol -> gruta
    P.put(39, 2, "u")  # porta do mar
    P.put(33, 2, "n")  # alavanca da saída (precisa das cargas)
    P.puts([(33, 1), (33, 3)], "p")
    P.puts([(x, 41) for x in range(20, 25)], "p")  # porta do elevador
    # segredos
    P.put(22, 7, "t")  # quadro falso na capela -> sacristia
    P.put(14, 36, "t")  # quadro falso no pavilhão -> isolamento

    # --- decoração das paredes
    P.puts([(19, 35), (25, 35)], "J")
    P.puts([(20, 29), (24, 29)], "i")
    P.puts([(17, 30), (27, 34)], "q")
    P.put(35, 29, "r")
    P.put(31, 28, "l")
    P.puts([(31, 36), (27, 30)], "q")
    P.puts([(16, 19), (16, 26), (28, 18), (28, 28)], "i")
    P.puts([(19, 17), (25, 17)], "k")
    P.puts([(20, 7), (24, 7)], "i")
    P.puts([(17, 10), (27, 10), (17, 14), (27, 14)], "J")
    P.put(8, 31, "r")
    P.puts([(9, 16), (13, 16)], "A")
    P.put(6, 21, "m")
    P.put(36, 23, "c")
    P.puts([(30, 27), (34, 27)], "c")
    P.put(22, 2, "k")

    # --- jogador
    P.put(22, 39, "N")

    def many(c, pts):
        P.puts(pts, c)

    # elevador
    many(",", [(22, 37)])
    many(":", [(20, 40)])
    # portaria
    many("-", [(19, 30)])
    many("@", [(18, 30)])
    many("#", [(18, 34), (26, 34)])
    many("e", [(25, 31)])
    many("R", [(19, 33)])
    many(";", [(26, 30)])
    # corpo da guarda: a FBP e a alavanca
    many("~", [(30, 30), (31, 34)])
    many("]", [(29, 34)])
    many("L", [(32, 30)])
    many("e", [(29, 35)])
    many(":", [(34, 35)])
    many(",", [(31, 32)])
    # pavilhão prisional
    many("d", [(15, 30)])
    many("e", [(14, 34)])
    many("}", [(9, 29)])
    many("&", [(10, 30)])
    many(";", [(9, 35)])
    many("(", [(11, 33)])
    many(",", [(15, 32)])
    # isolamento
    many("*", [(13, 38), (15, 39)])
    many(")", [(16, 37)])
    many("}", [(12, 40)])
    # pátio: sacos de areia como abrigo, a bandeira ao meio
    many("#", [(22, 23)])
    many('"', [(19, 21), (25, 21), (19, 26), (25, 26)])
    many("e", [(18, 19), (26, 27)])
    many("L", [(26, 19)])
    many("d", [(20, 25)])
    many(";", [(17, 28)])
    many(":", [(27, 18)])
    # caserna: camaratas e a chave no cacifo do oficial
    many("-", [(8, 19), (8, 23), (12, 19), (12, 23)])
    many("3", [(14, 17)])
    many("L", [(13, 22)])
    many("e", [(9, 21)])
    many(";", [(7, 17)])
    many(":", [(15, 25)])
    many("'", [(7, 25)])
    # capela
    many("^", [(19, 10), (25, 10), (19, 14), (25, 14)])
    many("/", [(22, 10), (22, 14)])
    many("!", [(20, 8), (24, 8)])
    many("#", [(18, 8), (26, 8)])
    many(")", [(21, 9)])
    many("e", [(20, 12)])
    many("L", [(25, 12)])
    many(":", [(26, 16)])
    many(";", [(18, 16)])
    # sacristia
    many("*", [(21, 3), (23, 3)])
    many("(", [(20, 5), (24, 5)])
    many("}", [(22, 4)])
    # paiol: a G3 e as cargas de demolição
    many("2", [(30, 21)])
    many("4", [(34, 25)])
    many("%", [(35, 20), (35, 26)])
    many("'", [(29, 26), (30, 26)])
    many("L", [(34, 21)])
    many(";", [(33, 26)])
    many(",", [(32, 23)])
    # gruta
    many("d", [(35, 10)])
    many(":", [(31, 13)])
    many("e", [(43, 10)])
    many("L", [(41, 5)])
    many(";", [(44, 13)])
    many("}", [(44, 5)])
    many("e", [(36, 1)])
    many("=", [(30, 9)])
    P.seal()
    return P


# ------------------------------------------------------------------ definições


def block_for(lines, ident):
    return [l for l in lines if l.startswith(ident + "_")]


def enemy_block(e_lines, ident, folder, health, score, weapon, prox="2"):
    out = []
    for l in e_lines:
        k, _, v = l.partition(" ")
        k = ident + k[1:]
        v = v.replace("billboards/sentinel/", f"billboards/{folder}/")
        if k == f"{ident}_MAX_HEALTH":
            v = str(health)
        elif k == f"{ident}_SCORE":
            v = str(score)
        elif k == f"{ident}_1_INVENTORY":
            v = weapon
        elif k == f"{ident}_OPTIMAL_PROXIMITY":
            v = prox
        k = k.replace("_USING_Y_", f"_USING_{weapon}_")
        out.append(f"{k} {v}")
    if weapon in "QX" and not any(l.startswith(f"{ident}_USING_{weapon}_") for l in out):
        out.append(f"{ident}_USING_{weapon}_1_SPRITE {T}/billboards/{folder}/shoot.bmp:{T}/billboards/{folder}/aim.bmp")
    return out


def wall(ident, name, side=None):
    side = side or name
    return [
        f"{ident}_TYPE WALL",
        f"{ident}_NO {T}/{name}.bmp",
        f"{ident}_SO {T}/{name}.bmp",
        f"{ident}_WE {T}/{side}-dark.bmp",
        f"{ident}_EA {T}/{side}-dark.bmp",
    ]


def wall42(ident, tex):
    return [f"{ident}_TYPE WALL"] + [f"{ident}_{d} assets/42lisboa/{tex}" for d in ("NO", "SO", "WE", "EA")]


def secret(ident, sprite, sides, direction):
    return [
        f"{ident}_TYPE DOOR",
        f"{ident}_TARGETABLE TRUE",
        f"{ident}_ANIMATION_DELAY 2.3",
        f"{ident}_CLOSEABLE FALSE",
        f"{ident}_DOOR_SPRITE {T}/{sprite}.bmp",
        f"{ident}_SIDES_SPRITE {T}/{sides}.bmp",
        f"{ident}_DIRECTION {direction}",
        f"{ident}_OPEN_SOUND {S}/secret-wall.mp3",
        f"{ident}_OPEN_SOUND_VOLUME 0.8",
    ]


ENEMY_WEAPONS = {
    "Q": [
        "Q_TYPE WEAPON",
        f"Q_CANT_USE_SOUND {S}/empty.mp3",
        "Q_DAMAGE 5",
        "Q_AMMO_USAGE 1",
        "Q_NAME Pistola-metralhadora do invasor",
        f"Q_USE_SOUND {S}/rifle.mp3",
        "Q_USE_SOUND_VOLUME 0.4",
        "Q_DESCRIPTION Pistola-metralhadora de tambor do soldado invasor",
        "Q_USE_DELAY 0.45",
        f"Q_SCREEN_SPRITE {T}/items/rifle/screen/0.bmp",
        f"Q_SCREEN_USE_SPRITE {T}/items/rifle/screen/2.bmp:{T}/items/rifle/screen/3.bmp",
        f"Q_ICON_SPRITE {T}/items/rifle/icon.bmp",
    ],
    "X": [
        "X_TYPE WEAPON",
        f"X_CANT_USE_SOUND {S}/empty.mp3",
        "X_DAMAGE 11",
        "X_AMMO_USAGE 1",
        "X_NAME Pistola do comissario",
        f"X_USE_SOUND {S}/sentinel-fire.mp3",
        "X_USE_SOUND_VOLUME 0.5",
        "X_DESCRIPTION Pistola do comissario invasor",
        "X_USE_DELAY 1.25",
        f"X_SCREEN_SPRITE {T}/items/rifle/screen/0.bmp",
        f"X_SCREEN_USE_SPRITE {T}/items/rifle/screen/2.bmp:{T}/items/rifle/screen/3.bmp",
        f"X_ICON_SPRITE {T}/items/rifle/icon.bmp",
    ],
}

STRINGS = {
    "U": ("Racao de combate", "Racao de combate e pao com chourico"),
    "Z": ("Municoes", "Caixa de municoes"),
    "o": ("Cruz de guerra", "Condecoracao por bravura"),
    "z": ("Medalha", "Medalha militar"),
    "7": ("Caixa de ouro", "Barras de ouro do cofre do forte"),
    "I": ("Primeiros socorros", "Estojo de primeiros socorros"),
    "Y": ("Espingarda do invasor", "Espingarda de assalto do atirador invasor"),
    "C": ("Dentes do cao", "Mordedura do cao de ataque"),
    "H": ("Stoi!", "Grito de alerta do invasor"),
    "P": ("Pistola Walther", "Pistola semiautomatica, boa a curta distancia"),
    "G": ("Espingarda G3", "Espingarda automatica G3, potente e rapida"),
    "K": ("Faca", "Faca de combate, para corpo a corpo"),
    "M": ("FBP m/948", "Pistola-metralhadora portuguesa, boa a media distancia"),
}

NEW_IDS = "LRQXqrABFOJp^&<T3456DV>"
# chaves que este script acrescenta a definições escritas à mão
EXTRA_KEYS = ("n_KEY ", "n_LOCKED_TEXT ", "n_ON_SPRITE ", "y_ON_SPRITE ", "LOCKED_SOUND", "LEVER_SOUND")
# dificuldade (mais vida nos itens, menos dano do inimigo, mais balas)
OVERRIDES = {"Y_DAMAGE": "7", "Y_USE_DELAY": "0.9", "U_HEALTH": "15", "I_HEALTH": "30", "Z_AMMO": "12"}
MIN_AMMO = 24

# puzzles de cada nível: (chave, documento, porta trancada, textos)
PUZZLES = {
    "1": {
        "key": ("Chave do arsenal", "key-arsenal"),
        "doc": ("Planos de defesa do Forte", "docs"),
        "door": "door-arsenal",
        "door_text": "Trancada. Falta a chave do arsenal.",
        "gate_text": "O portão abre pela alavanca do arsenal.",
        "exit_text": "Ainda não tem os planos do Forte.",
    },
    "2": {
        "key": ("Chave do paiol", "key-paiol"),
        "doc": ("Cargas de demolição", "charges"),
        "door": "door-paiol",
        "door_text": "Trancado. Falta a chave do paiol.",
        "gate_text": "O portão abre pela alavanca da guarda.",
        "exit_text": "Sem as cargas não fecha a porta do mar.",
    },
}


def puzzle_defs(level):
    z = PUZZLES[level]
    out = []
    for drop, item, (name, art) in (("3", "5", z["key"]), ("4", "6", z["doc"])):
        out += [
            f"{drop}_TYPE DROP", f"{drop}_ITEM {item}", f"{drop}_AUTO_USE TRUE", f"{drop}_HARD FALSE", "",
            f"{item}_TYPE COLLECTIBLE", f"{item}_NAME {name}", f"{item}_DESCRIPTION {name}",
            f"{item}_SINGLE_USE TRUE", f"{item}_GIVES {item}",
            f"{item}_USE_SOUND {S}/key.mp3", f"{item}_USE_SOUND_VOLUME 0.6",
            f"{item}_ICON_SPRITE {T}/billboards/{art}.bmp",
            f"{item}_ICON_USE_SPRITE {T}/hud/keys/{art}.bmp", "",
        ]
    out += [
        "D_TYPE DOOR", "D_TARGETABLE TRUE", "D_AUTO_CLOSE_DELAY 8000.0", "D_ANIMATION_DELAY 1",
        f"D_DOOR_SPRITE {T}/{z['door']}.bmp", f"D_SIDES_SPRITE {T}/door-frame.bmp", "D_DIRECTION WEST",
        f"D_OPEN_SOUND {S}/door-open.mp3", "D_OPEN_SOUND_VOLUME 0.4",
        f"D_CLOSE_SOUND {S}/door-close.mp3", "D_CLOSE_SOUND_VOLUME 0.4",
        "D_KEY 5", f"D_LOCKED_TEXT {z['door_text']}", "",
        "V_TYPE DOOR", "V_TARGETABLE TRUE", "V_ANIMATION_DELAY 2", "V_CLOSEABLE FALSE",
        f"V_DOOR_SPRITE {T}/gate.bmp", f"V_SIDES_SPRITE {T}/door-frame.bmp", "V_DIRECTION NORTH",
        f"V_OPEN_SOUND {S}/gate.mp3", "V_OPEN_SOUND_VOLUME 0.7",
        "V_KEY >", "V_KEY_OPENS TRUE", f"V_LOCKED_TEXT {z['gate_text']}", "",
        ">_TYPE ELEVATOR", ">_TARGETABLE TRUE", ">_GIVES >",
    ] + [f">_{d} {T}/lever-gate-down.bmp" for d in ("NO", "SO", "WE", "EA")] + [
        f">_ON_SPRITE {T}/lever-gate-up.bmp", "",
        "n_KEY 6", f"n_LOCKED_TEXT {z['exit_text']}", f"n_ON_SPRITE {T}/lever-up.bmp",
        f"y_ON_SPRITE {T}/lever-up.bmp",
        f"LOCKED_SOUND {S}/locked.mp3", "LOCKED_SOUND_VOLUME 0.6",
        f"LEVER_SOUND {S}/lever.mp3", "LEVER_SOUND_VOLUME 0.7", "",
    ]
    return out


def defs_for(variant, e_lines, level):
    out = puzzle_defs(level)
    if variant == "42":
        # na variante 42 os inimigos não disparam (a arma deles é um item
        # "still"); os novos fazem o mesmo
        wq = wx = next(l.split()[1] for l in e_lines if l.startswith("e_1_INVENTORY "))
    else:
        wq, wx = "Q", "X"
    out += enemy_block(e_lines, "L", "legiao", 150, 200, wq, "3")
    out += [""]
    out += enemy_block(e_lines, "R", "gnr", 120, 150, wx, "2.5")
    out += [""]
    if variant != "42":
        out += ENEMY_WEAPONS["Q"] + [""] + ENEMY_WEAPONS["X"] + [""]
    if variant == "42":
        walls = [
            wall42("q", "cluster1-world-left.bmp"),
            wall42("r", "cluster1-right.bmp"),
            wall42("A", "cluster1-left.bmp"),
            wall42("B", "window.bmp"),
            wall42("F", "cluster2-wall.bmp"),
            wall42("O", "masonry-room.bmp"),
            wall42("J", "wall-of-fame.bmp"),
            wall42("p", "cluster2-door.bmp"),
        ]
        bb = {"^": "assets/42lisboa/bookshelf.bmp", "&": "assets/42lisboa/white-board-front.bmp",
              "<": "assets/42lisboa/pc-table-1.bmp"}
        sec = ["T_TYPE DOOR", "T_TARGETABLE TRUE", "T_ANIMATION_DELAY 2.3", "T_CLOSEABLE FALSE",
               "T_DOOR_SPRITE assets/42lisboa/cluster2-wall.bmp", "T_SIDES_SPRITE assets/42lisboa/cluster2-wall.bmp",
               "T_DIRECTION NORTH", f"T_OPEN_SOUND {S}/secret-wall.mp3", "T_OPEN_SOUND_VOLUME 0.8"]
    else:
        walls = [
            wall("q", "stone-poster"),
            wall("r", "stone-censura"),
            wall("A", "wood-mocidade"),
            wall("B", "azulejo-panel"),
            wall("F", "brick"),
            wall("O", "cave"),
            wall("J", "stone-plaque"),
            [f"p_TYPE WALL"] + [f"p_{d} {T}/lift-door.bmp" for d in ("NO", "SO", "WE", "EA")],
        ]
        bb = {"^": f"{T}/billboards/pillar.bmp", "&": f"{T}/billboards/cables.bmp",
              "<": f"{T}/billboards/battery.bmp"}
        sec = secret("T", "wood-emblem", "wood", "NORTH")
    for w in walls:
        out += w + [""]
    for c, spr in bb.items():
        out += [f"{c}_TYPE BILLBOARD"]
        if c == "&" and variant != "42":
            out += [f"{c}_HARD FALSE", f"{c}_Y_CENTERED TRUE"]
        out += [f"{c}_STILL_SPRITE {spr}", ""]
    out += sec + [""]
    return out


def rewrite(path, grid, variant, level):
    lines = open(path, encoding="utf-8").read().split("\n")
    while lines and not lines[-1].strip():
        lines.pop()
    # cabeçalho = tudo até à última linha vazia (como no parser em C)
    last = max(i for i, l in enumerate(lines) if not l.strip())
    head = lines[: last + 1]
    e_lines = block_for(head, "e")
    # tirar definições que este script gera (para poder correr várias vezes)
    head = [l for l in head if not any(l.startswith(c + "_") for c in NEW_IDS)]
    head = [l for l in head if not l.startswith(story.PREFIXES) and not l.startswith(EXTRA_KEYS)]
    for i, l in enumerate(head):
        k, _, v = l.partition(" ")
        if k in OVERRIDES:
            head[i] = f"{k} {OVERRIDES[k]}"
        elif k == "AMMO" and v.isdigit() and int(v) < MIN_AMMO:
            head[i] = f"AMMO {MIN_AMMO}"
    # textos
    for i, l in enumerate(head):
        m = re.match(r"^(.)_(NAME|DESCRIPTION) ", l)
        if m and m.group(1) in STRINGS:
            name, desc = STRINGS[m.group(1)]
            head[i] = f"{m.group(1)}_{m.group(2)} {name if m.group(2) == 'NAME' else desc}"
    # inserir antes das cores F/C do chão e teto
    idx = next(i for i, l in enumerate(head) if re.match(r"^F\s+\d", l))
    head = (head[:idx] + defs_for(variant, e_lines, level) + [""]
            + story.lines_for(os.path.basename(path), variant, grid) + [""] + head[idx:])
    s = "\n".join(head)
    s = re.sub(r"\n{3,}", "\n\n", s)
    if not s.endswith("\n\n"):
        s = s.rstrip("\n") + "\n\n"
    open(path, "w", encoding="utf-8").write(s + grid + "\n")


# ------------------------------------------------------------------ validador


def parse(path):
    lines = open(path, encoding="utf-8").read().split("\n")
    while lines and not lines[-1].strip():
        lines.pop()
    last = max(i for i, l in enumerate(lines) if not l.strip())
    types = {}
    for l in lines[:last]:
        if l.strip():
            k, _, v = l.strip().partition(" ")
            if k in types:
                raise SystemExit(f"{path}: duplicate type {k}")
            types[k] = v.strip()
    return types, lines[last + 1 :]


def validate(path, exit_char="n"):
    types, grid = parse(path)
    errs = []
    ids = {}
    for k, v in types.items():
        if k.endswith("_TYPE"):
            ids[k[:-5]] = v
    for c in " 0\t":
        ids[c] = "AIR"
    ids["1"] = "WALL"
    for c in "NSEW":
        ids[c] = "PLAYER"
    H = len(grid)
    W = max(len(r) for r in grid)
    g = [r.ljust(W) for r in grid]

    def kind(x, y):
        if not (0 <= x < W and 0 <= y < H):
            return "OUT"
        return ids.get(g[y][x], "UNDEF")

    used = set("".join(grid))
    for c in sorted(used):
        if c not in ids:
            errs.append(f"char {c!r} has no definition")
    players = [(x, y) for y in range(H) for x in range(W) if kind(x, y) == "PLAYER"]
    if not players:
        errs.append("no player")
    solid = {"WALL", "ELEVATOR"}
    # portas: orientação
    for y in range(H):
        for x in range(W):
            if kind(x, y) == "DOOR":
                d = types.get(f"{g[y][x]}_DIRECTION", "NORTH")
                if d in ("WEST", "EAST"):
                    wa, pa = [(x, y - 1), (x, y + 1)], [(x - 1, y), (x + 1, y)]
                else:
                    wa, pa = [(x - 1, y), (x + 1, y)], [(x, y - 1), (x, y + 1)]
                if not all(kind(*p) in solid for p in wa):
                    errs.append(f"door {g[y][x]} at {x},{y} ({d}) lacks walls at {wa}")
                if any(kind(*p) in solid | {"OUT"} for p in pa):
                    errs.append(f"door {g[y][x]} at {x},{y} ({d}) is blocked at {pa}")
    # alcance e fecho
    start = players[0]
    seen = {start}
    q = deque([start])
    leak = []
    while q:
        x, y = q.popleft()
        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            nx, ny = x + dx, y + dy
            k = kind(nx, ny)
            if k == "OUT":
                leak.append((x, y))
                continue
            if k in solid or (nx, ny) in seen:
                continue
            seen.add((nx, ny))
            q.append((nx, ny))
    if leak:
        errs.append(f"map open at {leak[:5]}")
    for y in range(H):
        for x in range(W):
            k = kind(x, y)
            if k in ("CHARACTER", "DROP", "PLAYER", "BILLBOARD") and (x, y) not in seen:
                errs.append(f"{g[y][x]} at {x},{y} unreachable")
    exits = [(x, y) for y in range(H) for x in range(W) if g[y][x] == exit_char and kind(x, y) == "ELEVATOR"]
    for ex, ey in exits:
        if not any((ex + dx, ey + dy) in seen for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))):
            errs.append(f"exit {exit_char} at {ex},{ey} unreachable")
    if not exits:
        errs.append(f"no exit {exit_char}")
    if any(k.startswith("OBJECTIVE_") for k in types):
        order, why = solve(types, g, kind, start, exit_char)
        print(f"   percurso: {' -> '.join(order)}")
        if why:
            errs.append(why)
    count = {}
    for c in used:
        count[c] = sum(r.count(c) for r in grid)
    enemies = {c: count[c] for c in used if ids.get(c) == "CHARACTER"}
    return errs, (W, H), enemies, len(players)


def solve(types, g, kind, start, exit_char):
    """Joga o puzzle no papel: anda só por onde o jogador pode (objetos duros
    tapam, portas trancadas e portões só com a chave), apanha o que dá
    chaves e puxa alavancas, até à saída. Devolve a ordem e o erro."""
    H, W = len(g), len(g[0])
    held = set()
    order = []
    def hard(x, y):
        c = g[y][x]
        k = kind(x, y)
        if k in ("WALL", "ELEVATOR", "OUT", "UNDEF"):
            return True
        if k == "BILLBOARD":
            return types.get(f"{c}_HARD", "TRUE") != "FALSE"
        if k == "DOOR":
            key = types.get(f"{c}_KEY")
            return bool(key) and key not in held
        return False
    def gives_drop(c):
        item = types.get(f"{c}_ITEM")
        return types.get(f"{item}_GIVES") if item else None
    objectives = [types[f"OBJECTIVE_{i}_DONE"] for i in range(1, 9) if f"OBJECTIVE_{i}_DONE" in types]
    for _ in range(20):
        seen = {start}
        q = deque([start])
        while q:
            x, y = q.popleft()
            for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                nx, ny = x + dx, y + dy
                if (nx, ny) in seen or hard(nx, ny):
                    continue
                seen.add((nx, ny))
                q.append((nx, ny))
        got = False
        for y in range(H):
            for x in range(W):
                c = g[y][x]
                k = kind(x, y)
                near = any((x + dx, y + dy) in seen for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)))
                if k == "DROP" and (x, y) in seen and gives_drop(c) and gives_drop(c) not in held:
                    held.add(gives_drop(c))
                    order.append(f"{gives_drop(c)}@{x},{y}")
                    got = True
                elif k == "ELEVATOR" and near and types.get(f"{c}_GIVES") and types[f"{c}_GIVES"] not in held \
                        and types.get(f"{c}_KEY", "") in held | {""}:
                    held.add(types[f"{c}_GIVES"])
                    order.append(f"{types[f'{c}_GIVES']}@{x},{y}")
                    got = True
                elif c == exit_char and k == "ELEVATOR" and near and types.get(f"{c}_KEY", "") in held | {""}:
                    missing = [o for o in objectives if o not in held]
                    order.append(f"saída@{x},{y}")
                    return order, (f"objectives not done at exit: {missing}" if missing else None)
        if not got:
            break
    return order, f"exit {exit_char} unreachable with {sorted(held)}"


CAMPAIGN = {
    "missao1.cub": ("1", "base"),
    "missao1_mega.cub": ("1", "mega"),
    "missao1_42.cub": ("1", "42"),
    "missao2.cub": ("2", "base"),
}


def main():
    if "--check" not in sys.argv:
        g1 = missao1().text()
        g2 = missao2().text()
        for name, (which, variant) in CAMPAIGN.items():
            grid = g1 if which == "1" else g2
            if variant == "mega":
                grid = grid.replace("y", "f")
            rewrite(os.path.join(MAPS, name), grid, variant, which)
    ok = True
    for name in list(CAMPAIGN) + ["hub.cub"]:
        errs, size, enemies, np_ = validate(os.path.join(MAPS, name), "a" if name == "hub.cub" else "n")
        print(f"{name}: {size[0]}x{size[1]}, players {np_}, enemies {enemies}, {'OK' if not errs else 'ERRORS'}")
        for e in errs[:30]:
            print("   ", e)
        ok &= not errs
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
