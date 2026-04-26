import os

CONFIG_DIR = "config"

COLOR_NAMES = {
    "COKLAT": "Brown",
    "BIRU_MUDA": "Light Blue",
    "MERAH_MUDA": "Pink",
    "ORANGE": "Orange",
    "MERAH": "Red",
    "KUNING": "Yellow",
    "HIJAU": "Green",
    "BIRU_TUA": "Dark Blue",
    "DEFAULT": "Default (RR)",
    "ABU_ABU": "Gray (Utility)",
}

for size in [20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60]:
    prop_path = os.path.join(CONFIG_DIR, str(size), "property.txt")
    if not os.path.exists(prop_path):
        continue

    color_count = {}
    streets_by_color = {}
    with open(prop_path) as f:
        next(f)  # skip header
        for line in f:
            line = line.strip()
            if not line:
                continue
            parts = line.split()
            if len(parts) < 6:
                continue
            code = parts[1]
            kind = parts[3]
            color = parts[4]
            color_count[color] = color_count.get(color, 0) + 1
            if kind == "STREET":
                if color not in streets_by_color:
                    streets_by_color[color] = []
                streets_by_color[color].append(code)

    print(f"\n=== Board {size} ===")
    print(f"Total properties: {sum(color_count.values())}")
    print("Streets per color:")
    issues = []
    for color in ["COKLAT", "BIRU_MUDA", "MERAH_MUDA", "ORANGE", "MERAH", "KUNING", "HIJAU", "BIRU_TUA"]:
        streets = streets_by_color.get(color, [])
        count = len(streets)
        status = "OK" if count >= 2 else "WARNING: < 2 streets!"
        if count < 2:
            issues.append(f"{color}: only {count} street(s)")
        print(f"  {COLOR_NAMES.get(color, color)}: {count} streets {streets} {status}")

    # Non-street properties
    for color in ["DEFAULT", "ABU_ABU"]:
        count = color_count.get(color, 0)
        if count > 0:
            print(f"  {COLOR_NAMES.get(color, color)}: {count} properties")

    if issues:
        print(f"ISSUES: {', '.join(issues)}")
    else:
        print("All color groups OK (>=2 streets each)")
