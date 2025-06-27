import os
import re
import sys
import subprocess
from PIL import ImageColor
from time import sleep

def grayN_to_hex(gray_name):
    m = re.match(r'gray(\d+)', gray_name.lower())
    if m:
        n = int(m.group(1))
        if 0 <= n <= 100:
            val = round(255 * n / 100)
            return '{:02X}{:02X}{:02X}'.format(val, val, val)
    return None

def force_hex_color(color_value):
    original = color_value.strip()

    if original.lower() == 'none':
        return '#00000000'

    hex_match = re.match(r'^#([0-9a-fA-F]+)$', original)
    if hex_match:
        hex_val = hex_match.group(1).upper()

        if len(hex_val) == 3:
            r, g, b = hex_val
            rgb = r * 2 + g * 2 + b * 2
            return f'#FF{rgb}'

        elif len(hex_val) == 4:
            r, g, b, a = hex_val
            rgb = r * 2 + g * 2 + b * 2
            aa = a * 2
            return f'#{aa}{rgb}'

        elif len(hex_val) == 6:
            return f'#FF{hex_val}'

        elif len(hex_val) == 8:
            rrggbb = hex_val[:6]
            aa = hex_val[6:]
            return f'#{aa}{rrggbb}'

        elif len(hex_val) == 10:
            return f'#{hex_val}'

    hex_gray = grayN_to_hex(original)
    if hex_gray:
        return f'#FF{hex_gray}'

    try:
        r, g, b = ImageColor.getrgb(original)
        return '#FF{:02X}{:02X}{:02X}'.format(r, g, b)
    except Exception:
        return original

def clean_xpm_colors(xpm_path):
    with open(xpm_path, 'r') as f:
        content = f.read()

    # Encontra a posição de "/* pixels */"
    pixels_match = re.search(r'/\*\s*pixels\s*\*/', content)
    if not pixels_match:
        print(f"[aviso] Não encontrado '/* pixels */' em {xpm_path}")
        return

    # Divide o conteúdo antes e depois de "/* pixels */"
    before_pixels = content[:pixels_match.start()]
    after_pixels = content[pixels_match.start():]

    # Regex para encontrar todas as linhas com "c #" na parte antes de "/* pixels */"
    color_pattern = r'([^"]*"\s*[^"]*\s*c\s+)([^",\s]+)([^"]*"[^"]*)'
    
    def replace_color(match):
        prefix = match.group(1)  # parte antes da cor
        original_color = match.group(2)  # cor original
        suffix = match.group(3)  # parte depois da cor
        
        hex_color = force_hex_color(original_color)
        return f"{prefix}{hex_color}{suffix}"

    # Aplica a substituição apenas na parte antes de "/* pixels */"
    new_before_pixels = re.sub(color_pattern, replace_color, before_pixels)
    
    # Reconstrói o conteúdo completo
    new_content = new_before_pixels + after_pixels

    with open(xpm_path, 'w') as f:
        f.write(new_content)

    print(f'✔ Corrigido: {xpm_path}')

def converter_pngs_para_xpm(diretorio):
    """Converte todos os arquivos PNG para XPM usando mogrify"""
    try:
        orignal_dir = os.getcwd()
        # Mudamos para o diretório especificado
        os.chdir(diretorio)
        
        # Executa o comando mogrify para converter PNG para XMP
        resultado = subprocess.run(["mogrify", "-format", "xpm", "*.png"],
                                      capture_output=True, text=True)
        # Retorna ao diretório original
        os.chdir(orignal_dir)
        if resultado.returncode == 0:
            print(f'✔ Conversão PNG->XPM concluída em {diretorio}')
        else:
            print(f'✘ Erro na conversão: {resultado.stderr}')
            
    except Exception as e:
        print(f'✘ Erro ao executar mogrify: {e}')

def processar_xpms_recursivamente(diretorio):
    """Processa arquivos XPM no diretório especificado"""
    for root, _, files in os.walk(diretorio):
        for file in files:
            if file.endswith('.xpm'):
                caminho_completo = os.path.join(root, file)
                try:
                    print(f'🧼 A limpar {caminho_completo}...')
                    clean_xpm_colors(caminho_completo)
                except Exception as e:
                    print(f'✘ Erro em {caminho_completo}: {e}')

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Uso: python png_to_xpm.py <diretorio>")
        sys.exit(1)
    
    diretorio = sys.argv[1]
    
    if not os.path.exists(diretorio):
        print(f"✘ Diretório não encontrado: {diretorio}")
        sys.exit(1)
    
    if not os.path.isdir(diretorio):
        print(f"✘ O caminho especificado não é um diretório: {diretorio}")
        sys.exit(1)
    
    print(f"🚀 Processando diretório: {diretorio}")
    
    # Primeiro converte PNG para XPM
    converter_pngs_para_xpm(diretorio)
    # Depois limpa os arquivos XPM
    processar_xpms_recursivamente(diretorio)
    
    print("✨ Processo completo!")