import os
import sys

CONFIG_DIR = "config"

def get_unique_codes(filepath):
    codes = set()
    with open(filepath) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("ID"):
                continue
            parts = line.split()
            if len(parts) >= 2:
                codes.add(parts[1])
    return codes

def get_board_codes(filepath):
    codes = []
    with open(filepath) as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            codes.append(line.split()[0])
    return codes

for size in [20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60]:
    size_dir = os.path.join(CONFIG_DIR, str(size))
    board_path = os.path.join(size_dir, "board.txt")
    prop_path = os.path.join(size_dir, "property.txt")
    aksi_path = os.path.join(size_dir, "aksi.txt")

    if not os.path.exists(board_path):
        continue

    board_codes = get_board_codes(board_path)
    prop_codes = get_unique_codes(prop_path) if os.path.exists(prop_path) else set()
    aksi_codes = get_unique_codes(aksi_path) if os.path.exists(aksi_path) else set()
    defined = prop_codes | aksi_codes

    missing = [c for c in board_codes if c not in defined]
    extra = [c for c in defined if c not in board_codes]

    print(f"Board {size}: {len(board_codes)} tiles")
    print(f"  Defined tiles: {len(defined)} ({len(prop_codes)} prop + {len(aksi_codes)} aksi)")
    if missing:
        print(f"  MISSING in prop/aksi: {missing}")
    if len(board_codes) != size:
        print(f"  WRONG COUNT: expected {size}, got {len(board_codes)}")
    if extra:
        print(f"  UNUSED defined: {extra}")
    if not missing and len(board_codes) == size:
        print("  OK")
    print()
