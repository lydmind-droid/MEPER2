import zipfile
import os

os.chdir(os.path.dirname(os.path.abspath(__file__)))

with zipfile.ZipFile('MEPER.aia', 'w', zipfile.ZIP_DEFLATED) as z:
    base = 'src'
    for root, dirs, files in os.walk(base):
        for f in files:
            filepath = os.path.join(root, f)
            rel = os.path.relpath(filepath, base)
            arcname = rel.replace(os.sep, '/')
            z.write(filepath, arcname)

with zipfile.ZipFile('MEPER.aia', 'r') as z:
    for name in z.namelist():
        print(name)

print('MEPER.aia created successfully')
