# Textos da história e mensagens de rádio, por mapa (pt-PT). O maps.py
# escreve-os no cabeçalho de cada mapa da campanha como chaves simples, que
# o jogo lê (ver src/utils/story/story.h). Para mudar um texto ou onde uma
# mensagem dispara, basta editar aqui e correr scripts/oms_art/maps.py.
#
# Gatilhos do rádio: ("at", carácter_do_mapa) dispara ao chegar ao primeiro
# sítio com esse carácter; ("see", "e") ao ver o primeiro inimigo desse
# tipo; ("health", 30) com a vida a 30% ou menos; ("start",) ao começar.
#
# Ficção: a invasão é de um exército estrangeiro inventado; não há pessoas
# reais. O comandante é o Major Andrade; o jogador, o Sargento Silva.

S = "assets/oms3d/story"
M = "assets/oms3d/sounds/music"


def sound(key, name, vol="0.4"):
    """Música de um ecrã da história, em ciclo."""
    return [f"{key}_SOUND {M}/{name}.mp3", f"{key}_SOUND_VOLUME {vol}", f"{key}_SOUND_LOOP TRUE"]

INTRO = [
    ("intro-1", "1968. Uma noite de lua cheia.|Uma frota estrangeira, sem bandeira conhecida, aproxima-se da costa portuguesa."),
    ("intro-2", "Paraquedistas inimigos caem sobre a cidade.|As comunicações cortam-se uma a uma."),
    ("intro-3", "No Quartel-General, o Major Andrade reúne os últimos homens.|\"Eles já estão cá dentro.\""),
    ("intro-4", "Sargento Silva: aguente o Quartel-General e depois o Forte.|Até à madrugada não há reforços."),
]

BRIEF1 = ("DEFESA DO QUARTEL-GENERAL",
          "Silva, o inimigo entrou pela portaria.|Limpe o átrio e os gabinetes, um a um.|"
          "A saída para o Forte é pelo gabinete do comando.")
BRIEF2 = ("DEFESA DO FORTE",
          "O Forte é a última porta da cidade.|O inimigo desembarca na gruta do mar.|"
          "Feche-lhes a passagem, Sargento. A madrugada está perto.")
BRIEF_MEGA = ("NÍVEL BÓNUS", "Munições sem fim e vida de ferro.|Divirta-se, Sargento. Ninguém está a contar.")

ENDING = [
    ("ending-1", "Madrugada. Os barcos inimigos fogem para o mar alto.|A bandeira continua no alto do Forte."),
    ("ending-2", "O Sargento Silva recebe a Cruz de Guerra.|\"Missão cumprida, Silva. Vá descansar.\"|FIM"),
]

RADIO1 = [
    (("start",), "Silva, aqui Andrade. Ouve-me? Avance com cuidado."),
    (("see", "e"), "Atiradores à vista! Procure abrigo e não pare."),
    (("see", "d"), "Cães de ataque! Não os deixe chegar perto!"),
    (("see", "R"), "Um comissário! Abata-o primeiro, Silva!"),
    (("at", "]"), "O armeiro! Pegue na FBP e nas munições."),
    (("at", "u"), "Reforços a caminho, aguente!"),
    (("health", 30), "Está ferido, Silva! Procure um estojo de socorros!"),
]
RADIO2 = [
    (("start",), "Bem-vindo ao Forte, Sargento. A portaria é sua."),
    (("see", "L"), "Soldados com pistolas-metralhadoras! Fique abrigado!"),
    (("see", "R"), "Comissário à vista! Sem ele, eles desorganizam-se."),
    (("at", "2"), "O armeiro do Forte! A G3 está aí, use-a."),
    (("at", "u"), "A gruta do mar é já ali. Feche-lhes a entrada!"),
    (("health", 30), "Silva, está a perder sangue! Socorros, já!"),
]


def find(grid, ch):
    """Primeira posição (x, y) de ch na grelha, no referencial do jogo (a
    primeira linha da grelha é y = 1, como o parser em C a conta)."""
    for y, row in enumerate(grid.split("\n")):
        x = row.find(ch)
        if x >= 0:
            return x, y + 1
    return None


def radio_lines(radio, grid):
    out = [f"RADIO_PORTRAIT_SPRITE {S}/radio-major.bmp"]
    n = 0
    for trig, text in radio:
        line = None
        if trig[0] == "at":
            pos = find(grid, trig[1])
            if not pos:
                continue
            line = f"RADIO_{n + 1}_AT {pos[0]} {pos[1]}"
        elif trig[0] == "see":
            if not find(grid, trig[1]):
                continue
            line = f"RADIO_{n + 1}_SEE {trig[1]}"
        elif trig[0] == "health":
            line = f"RADIO_{n + 1}_HEALTH {trig[1]}"
        n += 1
        out.append(f"RADIO_{n}_TEXT {text}")
        if line:
            out.append(line)
    return out


def lines_for(name, variant, grid):
    """Chaves da história para um mapa da campanha."""
    out = [f"STATS_SPRITE {S}/stats.bmp"] + sound("BRIEFING", "deus-patria-rei")
    if name.startswith("missao1") and variant != "mega":
        out.insert(0, "LEVEL_NAME Defesa do Quartel-General")
        for i, (img, text) in enumerate(INTRO, 1):
            out += [f"INTRO_{i}_SPRITE {S}/{img}.bmp", f"INTRO_{i}_TEXT {text}"]
        out += sound("INTRO", "hino-mocidade")
        out += [f"BRIEFING_SPRITE {S}/briefing-1.bmp", f"BRIEFING_TITLE {BRIEF1[0]}",
                f"BRIEFING_TEXT {BRIEF1[1]}"]
        out += radio_lines(RADIO1, grid)
    elif variant == "mega":
        out.insert(0, "LEVEL_NAME Nível bónus")
        out += [f"BRIEFING_SPRITE {S}/briefing-1.bmp", f"BRIEFING_TITLE {BRIEF_MEGA[0]}",
                f"BRIEFING_TEXT {BRIEF_MEGA[1]}"]
    else:
        out.insert(0, "LEVEL_NAME Defesa do Forte")
        out += [f"BRIEFING_SPRITE {S}/briefing-2.bmp", f"BRIEFING_TITLE {BRIEF2[0]}",
                f"BRIEFING_TEXT {BRIEF2[1]}"]
        for i, (img, text) in enumerate(ENDING, 1):
            out += [f"ENDING_{i}_SPRITE {S}/{img}.bmp", f"ENDING_{i}_TEXT {text}"]
        out += sound("ENDING", "a-portuguesa", "0.5")
        out += radio_lines(RADIO2, grid)
    return out


PREFIXES = ("LEVEL_NAME", "INTRO_", "BRIEFING_", "STATS_SPRITE", "ENDING_", "RADIO_")
# (os ficheiros de música vêm de scripts/oms_art/music, copiados para M)
