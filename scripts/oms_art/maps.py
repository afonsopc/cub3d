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
#   alavancas n saída   y segredo para a missao1_mega
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
        if c in "ny":
            pass
        elif c in WALLISH or c in "vhustwxT":
            if cur not in WALLISH and c not in WALLISH:
                raise ValueError(f"{c!r} at {x},{y} must go on a wall (is {cur!r})")
        elif cur != "0":
            raise ValueError(f"{c!r} at {x},{y} must go on the floor (is {cur!r})")
        self.g[y][x] = c

    def puts(self, pts, c):
        for x, y in pts:
            self.g[y][x] = c

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
    """Defesa do Quartel-General. Os invasores já entraram: parte-se da
    entrada (sudoeste), recupera-se a portaria, o átrio, o corredor dos
    gabinetes, o pátio de azulejos e o gabinete do comando, onde fica a
    saída para o forte."""
    P = Plan(63, 55)
    # rua / entrada (início)
    P.room(0, 44, 10, 54, "f")
    # portaria
    P.room(10, 40, 22, 54, "f")
    # arrecadação (segredo com a alavanca da missao1_mega)
    P.room(10, 34, 22, 40, "f")
    # átrio
    P.room(22, 34, 46, 54, "f")
    # corredor dos gabinetes
    P.room(10, 30, 46, 34, "j")
    # gabinetes
    P.room(10, 18, 22, 30, "j")
    P.room(22, 18, 34, 30, "j")
    P.room(34, 18, 46, 30, "j")
    # arquivo e cofre
    P.room(46, 18, 62, 40, "f")
    P.room(46, 18, 52, 24, "f")
    # canil
    P.room(46, 40, 62, 54, "F")
    # celas e sala de interrogatório
    P.room(0, 18, 10, 44, "F")
    # material apreendido (segredo)
    P.room(0, 2, 14, 18, "f")
    # pátio de azulejos (com maciço entre o pátio e os gabinetes)
    P.room(14, 2, 44, 15, "a", force=True)
    P.solid(14, 16, 44, 17, "f")
    P.floor(28, 16, 28, 17)
    # gabinete do director e sala da saída
    P.room(44, 2, 62, 18, "f")
    P.room(56, 12, 62, 18, "f")

    # --- celas: parede interior com grades, 4 celas e interrogatório
    for y in range(19, 44):
        P.put(4, y, "F")
    for y in (24, 30, 36):
        for x in range(1, 4):
            P.put(x, y, "F")
    for x in range(4, 10):
        P.put(x, 36, "F")
    for y in (20, 22, 26, 28, 32, 34, 38, 39, 41, 42):
        P.put(4, y, "b")
    P.put(4, 26, "c")
    P.put(4, 38, "c")
    P.puts([(4, 21), (4, 27), (4, 33), (4, 40)], "v")
    P.put(7, 36, "h")
    P.put(10, 32, "v")  # para o corredor

    # --- portas principais
    P.put(10, 49, "v")  # rua -> portaria
    P.put(22, 47, "v")  # portaria -> átrio
    P.put(34, 34, "h")  # átrio -> corredor
    P.put(46, 47, "v")  # átrio -> canil
    P.put(54, 40, "h")  # canil -> arquivo
    P.put(46, 32, "v")  # corredor -> arquivo
    P.puts([(16, 30), (28, 30), (40, 30)], "h")  # gabinetes
    P.floor(28, 18, 28, 18)  # gabinete do meio -> passagem para o pátio
    P.put(28, 15, "h")  # passagem -> pátio
    P.puts([(27, 16), (27, 17), (29, 16), (29, 17)], "f")
    P.put(44, 8, "v")  # pátio -> gabinete do director
    P.put(56, 15, "u")  # porta da saída
    P.put(62, 15, "n")  # alavanca da saída
    P.puts([(62, 13), (62, 14), (62, 16), (62, 17)], "p")
    # segredos
    P.put(16, 40, "t")  # retrato falso na portaria -> arrecadação
    P.put(13, 35, "y")  # alavanca secreta
    P.put(46, 21, "s")  # emblema falso no gabinete do director-adjunto -> cofre
    P.put(14, 9, "x")  # pedra falsa no pátio -> material apreendido

    # --- decoração das paredes
    # rua
    P.puts([(5, 44), (0, 49)], "i")
    P.put(5, 54, "J")
    # portaria
    P.puts([(13, 54), (19, 54)], "q")
    P.put(22, 43, "r")
    P.put(22, 51, "q")
    # átrio
    P.puts([(26, 34), (30, 34), (38, 34), (42, 34)], "i")
    P.puts([(27, 54), (41, 54)], "k")
    P.puts([(31, 54), (37, 54)], "i")
    P.put(34, 54, "J")
    P.puts([(22, 38), (46, 38)], "k")
    P.puts([(46, 52), (22, 52)], "i")
    # corredor
    P.puts([(13, 34), (19, 34)], "r")
    P.puts([(25, 30), (31, 30), (37, 30), (43, 30)], "A")
    P.puts([(26, 34), (42, 34)], "i")
    # gabinetes
    P.put(16, 18, "A")
    P.put(10, 24, "r")
    P.put(22, 24, "m")
    P.put(34, 24, "l")
    P.put(40, 18, "m")
    P.put(46, 26, "l")
    # arquivo
    P.puts([(62, 22), (62, 30), (62, 36)], "r")
    P.put(56, 18, "k")
    P.put(52, 21, "r")
    # canil
    P.put(62, 47, "q")
    # pátio
    P.puts([(20, 2), (29, 2), (38, 2)], "B")
    P.puts([(14, 5), (14, 12), (44, 12), (44, 4)], "B")
    P.puts([(22, 15), (35, 15)], "B")
    # material apreendido
    P.put(7, 2, "k")
    P.put(0, 10, "r")
    # gabinete do director
    P.put(53, 2, "k")
    P.puts([(48, 2), (58, 2)], "i")
    P.puts([(62, 6), (62, 9)], "i")
    P.put(44, 14, "q")
    P.put(50, 18, "J")

    # --- jogadores (o primeiro é o local)
    P.puts([(3, 47), (3, 51)], "E")
    P.puts([(7, 47), (7, 51)], "W")

    # --- objetos, itens e inimigos
    items = {
        # rua
        ",": [(5, 49)], "#": [(1, 45), (9, 45)],
        # portaria
        "R": [(19, 43)], "e": [(19, 52), (15, 44)],
        "-": [(16, 47)], "@": [(11, 41)], ";": [(21, 53)], ":": [(11, 53)],
        # arrecadação
        "*": [(21, 39)], "}": [(11, 39)], "%": [(21, 35)], "'": [(19, 35), (20, 35)],
        # átrio
        "\\": [(34, 44)], "^": [(27, 39), (41, 39), (27, 49), (41, 49)],
        "/": [(30, 42), (38, 42), (30, 47), (38, 47)],
        "!": [(23, 35), (45, 35), (23, 53), (45, 53)],
    }
    for c, pts in items.items():
        P.puts(pts, c)
    P.puts([(25, 37), (43, 37), (34, 51), (29, 45)], "e")
    P.puts([(40, 45)], "d")
    P.puts([(24, 43)], ";")
    P.puts([(44, 44)], ":")
    # corredor
    P.puts([(14, 32), (20, 32), (26, 32), (32, 32), (38, 32), (44, 32)], ",")
    P.puts([(23, 31), (36, 33)], "e")
    P.put(30, 32, "d")
    # gabinete 1 (secretaria): máquinas de escrever
    P.puts([(12, 20), (15, 20), (18, 20)], "<")
    P.puts([(14, 25), (19, 25)], "-")
    P.puts([(16, 23)], ",")
    P.puts([(12, 28)], "e")
    P.puts([(21, 19)], "$")
    P.puts([(20, 28)], ";")
    # gabinete 2 (sala de espera)
    P.puts([(24, 20), (32, 20)], "~")
    P.puts([(26, 26)], "e")
    P.puts([(31, 25)], "d")
    P.puts([(23, 29), (33, 29)], "!")
    P.puts([(28, 23)], ",")
    # gabinete 3 (director-adjunto)
    P.puts([(40, 22)], "~")
    P.puts([(40, 24)], "/")
    P.puts([(35, 19), (45, 19)], "$")
    P.puts([(37, 27), (43, 27)], "R")
    P.puts([(44, 21)], "\"")
    P.puts([(35, 28)], "}")
    # cofre
    P.puts([(48, 20), (50, 20)], "*")
    P.puts([(48, 22), (49, 22), (50, 22)], "(")
    P.puts([(51, 23)], ")")
    # arquivo: estantes de processos em filas
    for x in (49, 52, 55, 58):
        P.puts([(x, y) for y in (27, 28, 29, 34, 35, 36)], "|")
    P.puts([(58, 20), (60, 20)], "<")
    P.puts([(55, 22)], "-")
    P.puts([(54, 31), (58, 31), (50, 38), (60, 25)], "e")
    P.puts([(61, 38)], "R")
    P.puts([(54, 25), (54, 33)], ",")
    P.puts([(61, 19)], "\"")
    P.puts([(47, 39)], ";")
    P.puts([(61, 33)], ")")
    # canil
    P.puts([(50, 44), (56, 48), (60, 52), (52, 51)], "d")
    P.puts([(61, 41), (47, 53)], "%")
    P.puts([(55, 42), (49, 49)], "=")
    P.puts([(54, 47)], ",")
    P.puts([(61, 53)], ":")
    # celas
    P.puts([(2, 20)], "=")
    P.puts([(2, 22)], "&")
    P.puts([(2, 28)], ":")
    P.puts([(2, 26)], "&")
    P.puts([(2, 33)], "(")
    P.puts([(2, 40), (2, 42)], "&")
    P.puts([(1, 38)], "}")
    P.puts([(7, 20), (7, 28)], ",")
    P.puts([(8, 24)], "e")
    P.puts([(6, 31)], "d")
    # sala de interrogatório
    P.puts([(7, 40)], "-")
    P.puts([(7, 38)], ",")
    P.puts([(5, 42)], "R")
    P.puts([(9, 42)], "e")
    P.puts([(9, 37)], "]")
    # material apreendido
    P.puts([(2, 4), (4, 4), (2, 6)], "\"")
    P.puts([(6, 9), (7, 9), (8, 9)], ")")
    P.puts([(10, 14), (12, 14)], "(")
    P.puts([(12, 4)], "*")
    P.puts([(7, 12)], ",")
    P.puts([(2, 16), (4, 16)], "|")
    # pátio
    P.puts([(29, 8)], "+")
    P.puts([(16, 4), (42, 4), (16, 13), (42, 13)], ".")
    P.puts([(22, 8), (36, 8)], "!")
    P.puts([(25, 4), (33, 4), (25, 13), (33, 13)], "=")
    P.puts([(20, 6), (38, 11), (24, 11)], "e")
    P.puts([(34, 6), (18, 10)], "d")
    P.puts([(40, 7)], "R")
    P.puts([(21, 13)], ";")
    P.puts([(37, 13)], ":")
    # gabinete do director
    P.puts([(53, 5)], "~")
    P.puts([(53, 4)], "@")
    P.puts([(49, 8), (57, 8)], "/")
    P.puts([(45, 3), (61, 3)], "#")
    P.puts([(46, 16), (54, 16)], "$")
    P.puts([(48, 5), (58, 5), (50, 14)], "R")
    P.puts([(52, 10), (60, 10)], "e")
    P.puts([(55, 13)], "d")
    P.puts([(45, 10)], ";")
    P.puts([(61, 11)], "}")
    # sala da saída
    P.puts([(59, 15)], ",")
    return P


def missao2():
    """Defesa do Forte. Chega-se de elevador (sul) e retoma-se a portaria,
    o pátio, o corpo da guarda, a caserna e a capela; os invasores entram
    pela gruta do mar, onde fica a saída."""
    P = Plan(64, 60)
    P.room(26, 52, 38, 59, "f")  # elevador (início)
    P.room(20, 44, 44, 52, "f")  # portaria do forte
    P.room(20, 20, 44, 44, "f")  # pátio
    P.room(20, 4, 44, 20, "f")  # capela
    P.room(26, 0, 38, 4, "f")  # sacristia (segredo)
    P.room(44, 0, 63, 20, "j")  # caserna
    P.room(44, 20, 63, 34, "F")  # corpo da guarda
    P.room(44, 34, 63, 52, "f")  # refeitório e arrecadação
    P.room(0, 30, 20, 52, "F")  # pavilhão prisional
    # gruta do mar: rocha maciça com uma passagem escavada
    P.solid(0, 0, 19, 29, "O")
    for x0, y0, x1, y1 in (
        (14, 10, 19, 14),  # entrada (vem da capela)
        (9, 8, 13, 16),  # túnel
        (3, 2, 10, 7),  # gruta alta
        (2, 8, 5, 18),  # poço
        (2, 19, 12, 21),  # passagem de baixo
        (11, 19, 17, 27),  # gruta funda
        (3, 24, 8, 28),  # porta do mar (saída)
        (10, 26, 10, 26),
    ):
        P.floor(x0, y0, x1, y1)
    P.solid(6, 4, 7, 5, "O")  # pedra no meio da gruta alta
    P.solid(14, 23, 15, 24, "O")

    # pavilhão prisional: celas a oeste, corredor, isolamento
    for y in range(31, 52):
        P.put(6, y, "F")
    for y in (36, 41, 46):
        for x in range(1, 6):
            P.put(x, y, "F")
    for y in (32, 34, 37, 39, 42, 44, 47, 48, 50, 51):
        P.put(6, y, "b")
    P.puts([(6, 44), (6, 50)], "c")
    P.puts([(6, 33), (6, 38), (6, 43), (6, 49)], "v")
    # isolamento ("segredo") a nordeste do pavilhão, atrás de parede falsa
    for x in range(14, 20):
        P.put(x, 36, "F")
    for y in range(31, 36):
        P.put(14, y, "F")
    P.put(14, 33, "x")

    # --- portas
    P.put(32, 52, "h")  # elevador -> portaria
    P.put(32, 44, "h")  # portaria -> pátio
    P.put(20, 48, "v")  # portaria -> pavilhão
    P.put(44, 48, "v")  # portaria -> refeitório
    P.put(44, 26, "v")  # pátio -> corpo da guarda
    P.put(54, 34, "h")  # corpo da guarda -> refeitório
    P.put(54, 20, "h")  # corpo da guarda -> caserna
    P.put(44, 10, "v")  # caserna -> capela
    P.put(20, 12, "v")  # capela -> gruta
    P.put(32, 20, "w")  # segredo: pátio -> capela
    P.put(32, 4, "t")  # segredo: capela -> sacristia
    # saída: porta e alavanca na porta do mar
    P.put(9, 26, "u")
    P.put(2, 26, "n")
    # elevador: parede do fundo é a porta do elevador
    P.puts([(x, 59) for x in range(27, 38)], "p")

    # --- decoração das paredes
    P.puts([(24, 4), (40, 4)], "i")
    P.put(28, 4, "J")
    P.put(36, 4, "J")
    P.puts([(20, 7), (44, 16)], "q")
    P.puts([(20, 17), (44, 6)], "i")
    P.puts([(24, 20), (40, 20)], "k")
    P.puts([(26, 20), (38, 20)], "i")
    P.puts([(20, 25), (20, 39), (44, 31), (44, 39)], "i")
    P.puts([(26, 44), (38, 44)], "q")
    P.puts([(20, 51), (44, 45)], "r")
    P.puts([(24, 52), (40, 52)], "J")
    P.put(32, 0, "k")
    P.puts([(50, 0), (57, 0)], "A")
    P.puts([(63, 5), (63, 14)], "q")
    P.put(53, 20, "l")
    P.puts([(63, 24), (63, 30)], "r")
    P.put(48, 34, "q")
    P.puts([(63, 40), (63, 47)], "i")
    P.puts([(10, 52), (16, 52)], "r")
    P.put(10, 30, "q")
    P.put(19, 36, "k")

    # --- jogador
    P.put(32, 57, "N")

    # --- objetos, itens e inimigos
    def many(c, pts):
        P.puts(pts, c)

    # elevador
    many(",", [(32, 55)])
    many(":", [(28, 54)])
    # portaria
    many("-", [(32, 48)])
    many("@", [(29, 48)])
    many("#", [(21, 45), (43, 45)])
    many(",", [(26, 48), (38, 48)])
    many("R", [(24, 46), (40, 46), (35, 50)])
    many("e", [(28, 50)])
    many(";", [(42, 51)])
    # pátio
    many("^", [(26, 26), (38, 26), (26, 38), (38, 38)])
    many("\\", [(32, 32)])
    many("?", [(23, 23)])
    many("'", [(41, 22), (42, 22), (42, 23)])
    many("%", [(22, 42), (23, 42)])
    many("L", [(28, 30), (36, 34)])
    many("R", [(40, 41), (24, 33)])
    many("d", [(32, 24), (30, 40)])
    many(",", [(32, 28), (32, 36), (26, 32), (38, 32)])
    many(":", [(42, 42)])
    many(";", [(22, 30)])
    # corpo da guarda
    many("~", [(50, 24), (57, 24)])
    many("-", [(54, 29)])
    many("@", [(62, 21)])
    many("R", [(47, 22), (60, 23), (49, 31), (59, 31)])
    many("e", [(54, 26)])
    many(",", [(50, 27), (58, 27)])
    many("}", [(62, 33)])
    many(";", [(45, 33)])
    # caserna
    for x in (47, 51, 55, 59):
        many("~", [(x, 4)])
        many("-", [(x, 15)])
    many("#", [(45, 1), (62, 1)])
    many("%", [(62, 9), (62, 10)])
    many("'", [(45, 18)])
    many("L", [(48, 8), (56, 7), (60, 12), (50, 12), (54, 17)])
    many("d", [(58, 16)])
    many(",", [(49, 10), (57, 10)])
    many("]", [(46, 9)])
    many(";", [(61, 18)])
    # capela
    many("^", [(25, 8), (25, 12), (25, 16), (39, 8), (39, 12), (39, 16)])
    many("/", [(32, 9), (32, 15)])
    many("!", [(30, 5), (34, 5)])
    many("#", [(28, 6), (36, 6)])
    many("L", [(30, 11), (36, 14), (22, 6)])
    many("e", [(42, 18)])
    many(")", [(32, 7)])
    # sacristia
    many("*", [(29, 2), (35, 2)])
    many("(", [(31, 1), (32, 1), (33, 1)])
    many(")", [(27, 3)])
    many("}", [(37, 3)])
    # refeitório e arrecadação
    for y in (38, 42, 46):
        many("~", [(49, y), (55, y)])
    many("'", [(62, 35), (62, 36), (61, 35)])
    many("%", [(62, 51)])
    many("2", [(61, 50)])
    many(":", [(52, 40), (58, 44)])
    many("}", [(45, 51)])
    many(";", [(60, 37), (46, 36)])
    many("L", [(52, 49), (59, 41)])
    many("R", [(46, 44)])
    many("e", [(57, 48)])
    many(",", [(52, 44), (58, 38)])
    # pavilhão prisional
    many("&", [(2, 32), (2, 38), (2, 43), (4, 48)])
    many("=", [(4, 34), (4, 39)])
    many(":", [(2, 34)])
    many("(", [(2, 44)])
    many("}", [(2, 50)])
    many(",", [(10, 33), (10, 39), (10, 45), (10, 50)])
    many("-", [(16, 44)])
    many("@", [(18, 43)])
    many("e", [(9, 36), (17, 48)])
    many("R", [(12, 41)])
    many("d", [(8, 46)])
    many(";", [(19, 51)])
    # isolamento
    many("*", [(17, 32)])
    many(")", [(18, 34)])
    many("|", [(16, 34)])
    many("&", [(15, 31)])
    # gruta
    many("d", [(6, 4), (11, 13)])
    many("L", [(4, 14), (15, 25)])
    many("e", [(9, 20)])
    many("(", [(4, 7), (12, 25)])
    many(":", [(2, 17)])
    many(";", [(16, 21)])
    many("=", [(17, 11)])
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
        "Q_DAMAGE 7",
        "Q_AMMO_USAGE 1",
        "Q_NAME Pistola-metralhadora do invasor",
        f"Q_USE_SOUND {S}/rifle.mp3",
        "Q_USE_SOUND_VOLUME 0.4",
        "Q_DESCRIPTION Pistola-metralhadora de tambor do soldado invasor",
        "Q_USE_DELAY 0.35",
        f"Q_SCREEN_SPRITE {T}/items/rifle/screen/0.bmp",
        f"Q_SCREEN_USE_SPRITE {T}/items/rifle/screen/2.bmp:{T}/items/rifle/screen/3.bmp",
        f"Q_ICON_SPRITE {T}/items/rifle/icon.bmp",
    ],
    "X": [
        "X_TYPE WEAPON",
        f"X_CANT_USE_SOUND {S}/empty.mp3",
        "X_DAMAGE 15",
        "X_AMMO_USAGE 1",
        "X_NAME Pistola do comissario",
        f"X_USE_SOUND {S}/sentinel-fire.mp3",
        "X_USE_SOUND_VOLUME 0.5",
        "X_DESCRIPTION Pistola do comissario invasor",
        "X_USE_DELAY 1.1",
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

NEW_IDS = "LRQXqrABFOJp^&<T"


def defs_for(variant, e_lines):
    out = []
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


def rewrite(path, grid, variant):
    lines = open(path, encoding="utf-8").read().split("\n")
    while lines and not lines[-1].strip():
        lines.pop()
    # cabeçalho = tudo até à última linha vazia (como no parser em C)
    last = max(i for i, l in enumerate(lines) if not l.strip())
    head = lines[: last + 1]
    e_lines = block_for(head, "e")
    # tirar definições que este script gera (para poder correr várias vezes)
    head = [l for l in head if not any(l.startswith(c + "_") for c in NEW_IDS)]
    head = [l for l in head if not l.startswith(story.PREFIXES)]
    # textos
    for i, l in enumerate(head):
        m = re.match(r"^(.)_(NAME|DESCRIPTION) ", l)
        if m and m.group(1) in STRINGS:
            name, desc = STRINGS[m.group(1)]
            head[i] = f"{m.group(1)}_{m.group(2)} {name if m.group(2) == 'NAME' else desc}"
    # inserir antes das cores F/C do chão e teto
    idx = next(i for i, l in enumerate(head) if re.match(r"^F\s+\d", l))
    head = (head[:idx] + defs_for(variant, e_lines) + [""]
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
    # objetos duros não podem tapar passagens de 1 célula
    count = {}
    for c in used:
        count[c] = sum(r.count(c) for r in grid)
    enemies = {c: count[c] for c in used if ids.get(c) == "CHARACTER"}
    return errs, (W, H), enemies, len(players)


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
            rewrite(os.path.join(MAPS, name), grid, variant)
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
