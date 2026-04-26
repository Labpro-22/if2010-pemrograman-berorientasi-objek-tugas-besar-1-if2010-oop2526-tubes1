import os

CONFIG_DIR = "config"

ACTIONS_MASTER = [
    ("GO", "Petak_Mulai", "go"),
    ("PEN", "Penjara", "jail"),
    ("BBP", "Bebas_Parkir", "free_parking"),
    ("PPJ", "Petak_Pergi_ke_Penjara", "go_to_jail"),
    ("DNU1", "Dana_Umum", "COMMUNITY"),
    ("DNU2", "Dana_Umum", "COMMUNITY"),
    ("DNU3", "Dana_Umum", "COMMUNITY"),
    ("DNU4", "Dana_Umum", "COMMUNITY"),
    ("KSP1", "Kesempatan", "CHANCE"),
    ("KSP2", "Kesempatan", "CHANCE"),
    ("KSP3", "Kesempatan", "CHANCE"),
    ("KSP4", "Kesempatan", "CHANCE"),
    ("KSP5", "Kesempatan", "CHANCE"),
    ("FES1", "Festival", "FESTIVAL"),
    ("FES2", "Festival", "FESTIVAL"),
    ("FES3", "Festival", "FESTIVAL"),
    ("FES4", "Festival", "FESTIVAL"),
    ("PPH1", "Pajak_Penghasilan", "PAJAK"),
    ("PPH2", "Pajak_Penghasilan", "PAJAK"),
    ("PBM1", "Pajak_Barang_Mewah", "TAX_PBM"),
    ("PBM2", "Pajak_Barang_Mewah", "TAX_PBM"),
]

PROPERTIES_MASTER = [
    ("GRT", "GARUT", "STREET", "COKLAT", 60, 40, 20, 50, 2, 10, 30, 90, 160, 250),
    ("TSK", "TASIKMALAYA", "STREET", "COKLAT", 60, 40, 50, 50, 4, 20, 60, 180, 320, 450),
    ("SMD", "SAMARINDA", "STREET", "COKLAT", 80, 50, 30, 60, 4, 20, 60, 180, 320, 500),
    ("BGR", "BOGOR", "STREET", "BIRU_MUDA", 100, 80, 20, 50, 6, 30, 90, 270, 400, 550),
    ("DPK", "DEPOK", "STREET", "BIRU_MUDA", 100, 80, 20, 50, 6, 30, 90, 270, 400, 550),
    ("BKS", "BEKASI", "STREET", "BIRU_MUDA", 120, 90, 20, 50, 8, 40, 100, 300, 450, 600),
    ("MDN", "MEDAN_UTARA", "STREET", "BIRU_MUDA", 120, 85, 25, 55, 8, 40, 100, 300, 450, 650),
    ("SRA", "SORONG", "STREET", "BIRU_MUDA", 110, 75, 20, 45, 7, 35, 110, 330, 480, 620),
    ("MGL", "MAGELANG", "STREET", "MERAH_MUDA", 140, 100, 100, 100, 10, 50, 150, 450, 625, 750),
    ("SOL", "SOLO", "STREET", "MERAH_MUDA", 140, 100, 100, 100, 10, 50, 150, 450, 625, 750),
    ("YOG", "YOGYAKARTA", "STREET", "MERAH_MUDA", 160, 120, 100, 100, 12, 60, 180, 500, 700, 900),
    ("TTO", "TERNATE", "STREET", "MERAH_MUDA", 150, 95, 100, 100, 11, 55, 160, 470, 650, 780),
    ("PWT", "PURWOKERTO", "STREET", "MERAH_MUDA", 155, 110, 105, 105, 13, 65, 195, 480, 680, 820),
    ("MAL", "MALANG", "STREET", "ORANGE", 180, 135, 100, 100, 14, 70, 200, 550, 750, 950),
    ("SMG", "SEMARANG", "STREET", "ORANGE", 180, 140, 100, 100, 14, 70, 200, 550, 750, 950),
    ("SBY", "SURABAYA", "STREET", "ORANGE", 200, 150, 100, 100, 16, 80, 220, 600, 800, 1000),
    ("PTK", "PONTIANAK", "STREET", "ORANGE", 190, 140, 100, 100, 15, 75, 210, 560, 760, 960),
    ("BWI", "BANYUWANGI", "STREET", "ORANGE", 185, 130, 100, 100, 13, 65, 190, 540, 740, 940),
    ("MKS", "MAKASSAR", "STREET", "MERAH", 220, 175, 150, 150, 18, 90, 250, 700, 875, 1050),
    ("BLP", "BALIKPAPAN", "STREET", "MERAH", 220, 175, 150, 150, 18, 90, 250, 700, 875, 1050),
    ("MND", "MANADO", "STREET", "MERAH", 240, 190, 150, 150, 20, 100, 300, 750, 925, 1100),
    ("AMQ", "AMBON", "STREET", "MERAH", 250, 195, 150, 150, 21, 105, 310, 775, 950, 1120),
    ("PLB", "PALEMBANG", "STREET", "KUNING", 260, 200, 150, 150, 22, 110, 330, 800, 975, 1150),
    ("PKB", "PEKANBARU", "STREET", "KUNING", 260, 210, 150, 150, 22, 110, 330, 800, 975, 1150),
    ("MED", "MEDAN", "STREET", "KUNING", 280, 225, 150, 150, 24, 120, 360, 850, 1025, 1200),
    ("BNA", "BANDA_ACEH", "STREET", "KUNING", 270, 215, 150, 150, 23, 115, 350, 825, 1000, 1180),
    ("BDG", "BANDUNG", "STREET", "HIJAU", 300, 250, 200, 200, 26, 130, 390, 900, 1100, 1275),
    ("DEN", "DENPASAR", "STREET", "HIJAU", 300, 260, 200, 200, 26, 130, 390, 900, 1100, 1275),
    ("MTR", "MATARAM", "STREET", "HIJAU", 320, 280, 200, 200, 28, 150, 450, 1000, 1200, 1400),
    ("PDG", "PADANG", "STREET", "HIJAU", 310, 270, 200, 200, 27, 140, 420, 950, 1150, 1350),
    ("JKT", "JAKARTA", "STREET", "BIRU_TUA", 350, 300, 200, 200, 35, 175, 500, 1100, 1300, 1500),
    ("BPG", "BRYAN_GANTENG", "STREET", "BIRU_TUA", 400, 350, 200, 200, 50, 200, 600, 1400, 1700, 2000),
    ("LPG", "LAMPUNG", "STREET", "BIRU_TUA", 370, 320, 200, 200, 38, 185, 520, 1150, 1350, 1550),
    ("GBR", "STASIUN_GAMBIR", "RAILROAD", "DEFAULT", 200, 100),
    ("STB", "STASIUN_BANDUNG", "RAILROAD", "DEFAULT", 200, 100),
    ("TUG", "STASIUN_TUGU", "RAILROAD", "DEFAULT", 200, 100),
    ("GUB", "STASIUN_GUBENG", "RAILROAD", "DEFAULT", 200, 100),
    ("PLN", "PLN", "UTILITY", "ABU_ABU", 150, 75),
    ("PAM", "PAM", "UTILITY", "ABU_ABU", 150, 75),
    ("BTM", "BATAM", "STREET", "COKLAT", 70, 45, 25, 55, 5, 25, 75, 225, 350, 480),
    ("CRB", "CIREBON", "STREET", "BIRU_MUDA", 110, 85, 22, 52, 7, 35, 105, 315, 460, 620),
]

BOARD_LAYOUTS = {
    20: [
        "GO", "GRT", "DNU1", "TSK", "PPH1",
        "PEN", "GBR", "BGR", "FES1", "BKS",
        "BBP", "MGL", "SOL", "DNU2", "MAL",
        "PPJ", "SBY", "PLB", "PKB", "KSP1",
    ],
    24: [
        "GO", "GRT", "DNU1", "TSK", "PPH1", "GBR",
        "PEN", "BGR", "FES1", "DPK", "BKS", "MGL",
        "BBP", "SOL", "MAL", "DNU2", "SMG", "SBY",
        "PPJ", "MKS", "KSP1", "BLP", "STB", "KSP2",
    ],
    28: [
        "GO", "GRT", "DNU1", "TSK", "PPH1", "GBR", "BGR",
        "PEN", "FES1", "DPK", "BKS", "MGL", "SOL", "YOG",
        "BBP", "MAL", "DNU2", "SMG", "SBY", "FES2", "MKS",
        "PPJ", "KSP1", "BLP", "STB", "PLB", "PKB", "KSP2",
    ],
    32: [
        "GO", "GRT", "DNU1", "TSK", "PPH1", "GBR", "BGR", "FES1",
        "PEN", "DPK", "BKS", "MGL", "SOL", "YOG", "STB", "MAL",
        "BBP", "DNU2", "SMG", "SBY", "FES2", "MKS", "KSP1", "BLP",
        "PPJ", "MND", "TUG", "PLB", "PLN", "PKB", "BDG", "DEN",
    ],
    36: [
        "GO", "GRT", "DNU1", "TSK", "PPH1", "GBR", "BGR", "FES1", "DPK",
        "PEN", "BKS", "MGL", "SOL", "YOG", "STB", "MAL", "DNU2", "SMG",
        "BBP", "SBY", "FES2", "MKS", "KSP1", "BLP", "MND", "TUG", "PLB",
        "PPJ", "PKB", "MED", "PPH2", "BDG", "DEN", "KSP2", "JKT", "BPG",
    ],
    40: [
        "GO", "GRT", "DNU1", "TSK", "PPH1", "GBR", "BGR", "FES1", "DPK", "BKS",
        "PEN", "MGL", "PLN", "SOL", "YOG", "STB", "MAL", "DNU2", "SMG", "SBY",
        "BBP", "MKS", "KSP1", "BLP", "MND", "TUG", "PLB", "PKB", "PAM", "MED",
        "PPJ", "BDG", "DEN", "FES2", "MTR", "GUB", "KSP2", "JKT", "PBM1", "BPG",
    ],
    44: [
        "GO", "GRT", "DNU1", "TSK", "PPH1", "GBR", "BGR", "FES1", "DPK", "BKS", "MGL",
        "PEN", "PLN", "SOL", "YOG", "STB", "MAL", "DNU2", "SMG", "SBY", "FES2", "MKS",
        "BBP", "KSP1", "BLP", "MND", "TUG", "PLB", "PKB", "PAM", "MED", "BDG",
        "DEN", "PPJ", "FES3", "MTR", "GUB", "KSP2", "JKT", "PBM1", "BPG", "SMD",
        "DNU3", "MDN",
    ],
    48: [
        "GO", "GRT", "DNU1", "TSK", "PPH1", "GBR", "BGR", "FES1", "DPK", "BKS", "MGL", "PLN",
        "PEN", "SOL", "YOG", "STB", "MAL", "DNU2", "SMG", "SBY", "FES2", "MKS", "KSP1", "BLP",
        "BBP", "MND", "TUG", "PLB", "PKB", "PAM", "MED", "DNU3", "BDG", "DEN", "FES3", "MTR",
        "PPJ", "GUB", "KSP2", "JKT", "PBM1", "BPG", "SMD", "MDN", "DNU4", "PTK", "KSP3", "BNA",
    ],
    52: [
        "GO", "GRT", "DNU1", "TSK", "PPH1", "GBR", "BGR", "FES1", "DPK", "BKS", "MGL", "PLN", "SOL",
        "PEN", "YOG", "STB", "MAL", "DNU2", "SMG", "SBY", "FES2", "MKS", "KSP1", "BLP", "MND", "TUG",
        "BBP", "PLB", "PKB", "PAM", "MED", "DNU3", "BDG", "DEN", "FES3", "MTR", "GUB", "KSP2", "JKT", "PPJ",
        "PBM1", "BPG", "SMD", "MDN", "DNU4", "PTK", "KSP3", "BNA", "PPH2", "PDG", "KSP4", "LPG",
    ],
    56: [
        "GO", "GRT", "DNU1", "TSK", "PPH1", "GBR", "BGR", "FES1", "DPK", "BKS", "MGL", "PLN", "SOL", "YOG",
        "PEN", "STB", "MAL", "DNU2", "SMG", "SBY", "FES2", "MKS", "KSP1", "BLP", "MND", "TUG", "PLB", "PKB",
        "BBP", "PAM", "MED", "DNU3", "BDG", "DEN", "FES3", "MTR", "GUB", "KSP2", "JKT", "PBM1", "BPG", "SMD",
        "PPJ", "MDN", "DNU4", "PTK", "KSP3", "BNA", "PPH2", "PDG", "KSP4", "LPG", "TTO", "AMQ", "CRB", "PBM2",
    ],
    60: [
        "GO", "GRT", "DNU1", "TSK", "PPH1", "GBR", "BGR", "FES1", "DPK", "BKS", "MGL", "PLN", "SOL", "YOG", "STB",
        "PEN", "MAL", "DNU2", "SMG", "SBY", "FES2", "MKS", "KSP1", "BLP", "MND", "TUG", "PLB", "PKB", "PAM", "MED",
        "BBP", "DNU3", "BDG", "DEN", "FES3", "MTR", "GUB", "KSP2", "JKT", "PBM1", "BPG", "SMD", "MDN", "DNU4", "PTK",
        "PPJ", "KSP3", "BNA", "PPH2", "PDG", "KSP4", "LPG", "TTO", "AMQ", "FES4", "BWI", "KSP5", "SRA", "PBM2", "CRB",
    ],
}

def write_property_file(path, props):
    lines = ["ID KODE NAMA JENIS WARNA HARGA_LAHAN NILAI_GADAI UPG_RUMAH UPG_HT RENT_L0 RENT_L1 RENT_L2 RENT_L3 RENT_L4 RENT_L5"]
    for i, p in enumerate(props, start=1):
        if p[2] == "RAILROAD" or p[2] == "UTILITY":
            line = f"{i} {p[0]} {p[1]} {p[2]} {p[3]} {p[4]} {p[5]}"
        else:
            line = f"{i} {p[0]} {p[1]} {p[2]} {p[3]} {p[4]} {p[5]} {p[6]} {p[7]} {' '.join(str(r) for r in p[8:])}"
        lines.append(line)
    with open(path, 'w') as f:
        f.write('\n'.join(lines) + '\n')

def write_aksi_file(path, actions):
    lines = ["ID KODE NAMA JENIS_PETAK WARNA"]
    for i, a in enumerate(actions, start=1):
        line = f"{i} {a[0]} {a[1]} {a[2]} DEFAULT"
        lines.append(line)
    with open(path, 'w') as f:
        f.write('\n'.join(lines) + '\n')

def main():
    action_map = {a[0]: a for a in ACTIONS_MASTER}
    prop_map = {p[0]: p for p in PROPERTIES_MASTER}

    for size, layout in BOARD_LAYOUTS.items():
        size_dir = os.path.join(CONFIG_DIR, str(size))
        os.makedirs(size_dir, exist_ok=True)

        used_actions = []
        used_props = []
        seen_actions = set()
        seen_props = set()
        for code in layout:
            if code in action_map:
                if code not in seen_actions:
                    used_actions.append(action_map[code])
                    seen_actions.add(code)
            elif code in prop_map:
                if code not in seen_props:
                    used_props.append(prop_map[code])
                    seen_props.add(code)
            else:
                print(f"ERROR: Unknown code '{code}' in board {size}")

        with open(os.path.join(size_dir, "board.txt"), 'w') as f:
            f.write('\n'.join(layout) + '\n')

        write_property_file(os.path.join(size_dir, "property.txt"), used_props)
        write_aksi_file(os.path.join(size_dir, "aksi.txt"), used_actions)

        assert len(layout) == size, f"Board {size} has {len(layout)} tiles, expected {size}"
        assert len(set(layout)) == size, f"Board {size} has duplicates"
        corners = {0, size//4, size//2, 3*size//4}
        corner_codes = {layout[i] for i in corners}
        assert corner_codes == {"GO", "PEN", "BBP", "PPJ"}, f"Board {size} wrong corners: {corner_codes}"
        print(f"Board {size}: OK ({len(used_props)} props, {len(used_actions)} actions)")

if __name__ == "__main__":
    main()
