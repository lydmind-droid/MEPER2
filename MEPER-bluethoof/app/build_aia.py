#!/usr/bin/env python3
"""
Gera o arquivo MEPER.aia a partir dos fontes em src/.
O .aia e um ZIP com a estrutura esperada pelo MIT App Inventor.
"""

import zipfile
import os

SRC_DIR = os.path.join(os.path.dirname(__file__), "src")
OUTPUT = os.path.join(os.path.dirname(__file__), "MEPER.aia")

FILES = [
    "youngandroidproject/project.properties",
    "src/appinventor/ai_user/MEPER/Screen1.scm",
    "src/appinventor/ai_user/MEPER/Screen1.bky",
    "src/appinventor/ai_user/MEPER/ControlScreen.scm",
    "src/appinventor/ai_user/MEPER/ControlScreen.bky",
]

def build():
    with zipfile.ZipFile(OUTPUT, "w", zipfile.ZIP_DEFLATED) as zf:
        for rel in FILES:
            full = os.path.join(SRC_DIR, rel)
            if not os.path.exists(full):
                print(f"AVISO: {full} nao encontrado, pulando.")
                continue
            zf.write(full, rel)
    print(f"Gerado: {OUTPUT}")

if __name__ == "__main__":
    build()
