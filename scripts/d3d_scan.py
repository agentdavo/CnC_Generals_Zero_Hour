#!/usr/bin/env python3
import re
import pathlib

root = pathlib.Path(__file__).resolve().parent.parent
paths = list(root.joinpath('src').rglob('*')) + list(root.joinpath('include').rglob('*'))
pattern = re.compile(r'\b(D3D\w+)\b')
results = {}
for path in paths:
    if path.suffix.lower() in {'.c', '.cpp', '.cc', '.h', '.hpp', '.inl'}:
        try:
            text = path.read_text(errors='ignore')
        except Exception:
            continue
        tokens = sorted(set(pattern.findall(text)))
        if tokens:
            results[str(path.relative_to(root))] = tokens

print('# DirectX Usage Report\n')
print('This report lists files referencing DirectX tokens (D3D*) in the src/ and include/ directories.\n')

for file in sorted(results):
    print(f'### {file}')
    for token in results[file]:
        print(f'- {token}')
    print()
