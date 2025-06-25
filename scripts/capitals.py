import os

def has_capital(name):
    return any(c.isupper() for c in name)

def get_capital_folders(path, output_file):
    with open(output_file, 'w', encoding='utf-8') as f:
        for root, dirs, _ in os.walk(path):
            for dir_name in dirs:
                if has_capital(dir_name):
                    relative_path = os.path.relpath(os.path.join(root, dir_name), path)
                    f.write(relative_path + '\n')

if __name__ == '__main__':
    current_dir = os.getcwd()
    output_file = 'capital_folders.txt'
    get_capital_folders(current_dir, output_file)
    print(f"Folder names with capital letters have been written to {output_file}")