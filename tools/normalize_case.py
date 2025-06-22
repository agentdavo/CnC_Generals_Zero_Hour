import os
import re
import sys
import subprocess

PATTERN = re.compile(r'#include\s+"([^"]+)"')


def rename_and_fix(path_root):
    for root, dirs, files in os.walk(path_root):
        if '.git' in root.split(os.sep):
            continue
        for f in files:
            old_path = os.path.join(root, f)
            lower_name = f.lower()
            if f != lower_name:
                new_path = os.path.join(root, lower_name)
                subprocess.run(["git", "mv", old_path, new_path], check=False)

    for root, dirs, files in os.walk(path_root):
        if '.git' in root.split(os.sep):
            continue
        for f in files:
            if f.endswith(('.cpp', '.h', '.hpp', '.c')):
                file_path = os.path.join(root, f)
                with open(file_path, 'r') as fh:
                    data = fh.read()
                new_data = PATTERN.sub(lambda m: f'#include "{m.group(1).lower()}"', data)
                if new_data != data:
                    with open(file_path, 'w') as fh:
                        fh.write(new_data)
                    subprocess.run(["git", "add", file_path], check=False)

if __name__ == "__main__":
    root = sys.argv[1] if len(sys.argv) > 1 else '.'
    rename_and_fix(root)
