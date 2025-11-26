#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Scan region files từ thư mục Maps để tìm ra map nào có dữ liệu region
"""

import os
import glob
import struct


def scan_all_region_files():
    """Scan tất cả file region trong thư mục maps"""
    script_dir = os.path.dirname(os.path.abspath(__file__))
    base_dir = os.path.dirname(script_dir)
    maps_dir = os.path.join(base_dir, "Bin/Server/maps")

    # Find all region files
    region_files = []
    for root, dirs, files in os.walk(maps_dir):
        for file in files:
            if '_region_' in file and file.endswith('.dat'):
                filepath = os.path.join(root, file)
                region_files.append(filepath)

    # Extract map IDs
    map_data = {}
    for filepath in region_files:
        filename = os.path.basename(filepath)
        try:
            map_id = int(filename.split('_')[0])

            if map_id not in map_data:
                map_data[map_id] = {
                    'region_c': 0,
                    'region_s': 0,
                    'files': []
                }

            if '_region_c' in filename:
                map_data[map_id]['region_c'] += 1
            elif '_region_s' in filename:
                map_data[map_id]['region_s'] += 1

            map_data[map_id]['files'].append(filepath)
        except (ValueError, IndexError):
            continue

    return map_data


def analyze_region_file(filepath):
    """Phân tích cơ bản file region"""
    file_size = os.path.getsize(filepath)

    # Đọc một số byte đầu để xem cấu trúc
    try:
        with open(filepath, 'rb') as f:
            header = f.read(64)
            return {
                'size': file_size,
                'header_preview': header[:16].hex() if len(header) >= 16 else None
            }
    except:
        return {'size': file_size, 'header_preview': None}


def check_map_regions(map_id):
    """Kiểm tra xem map có region files không"""
    script_dir = os.path.dirname(os.path.abspath(__file__))
    base_dir = os.path.dirname(script_dir)
    maps_dir = os.path.join(base_dir, "Bin/Server/maps")

    # Find region files for this map
    pattern = f"**/{map_id:03d}_region_*.dat"
    region_files = glob.glob(os.path.join(maps_dir, pattern), recursive=True)

    if not region_files:
        print(f"\n❌ Map {map_id} KHÔNG có file region .dat trong thư mục maps/")
        print(f"\n💡 Giải thích:")
        print(f"   - File region .dat chứa dữ liệu collision và obstacle cho map")
        print(f"   - Chỉ các map từ 74 trở lên mới có file region trong thư mục này")
        print(f"   - Map {map_id} có thể là:")
        print(f"     • Map cũ không sử dụng hệ thống region files")
        print(f"     • Map động được tạo runtime")
        print(f"     • Map sử dụng data từ client (*.map files)")

        # Check if defined in MapListDef.ini
        maplist_file = os.path.join(base_dir, "Bin/Server/Settings/MapListDef.ini")
        if os.path.exists(maplist_file):
            with open(maplist_file, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
                if f"\n{map_id}=" in content or f"\n{map_id}_name=" in content:
                    print(f"\n✅ Map {map_id} được định nghĩa trong MapListDef.ini")

                    # Extract info
                    for line in content.split('\n'):
                        if line.startswith(f"{map_id}_name="):
                            name = line.split('=')[1].strip()
                            print(f"   Tên: {name}")
                        elif line.startswith(f"{map_id}_MapType="):
                            map_type = line.split('=')[1].strip()
                            print(f"   Loại: {map_type}")
                else:
                    print(f"\n❌ Map {map_id} KHÔNG được định nghĩa trong MapListDef.ini")

        return False

    print(f"\n✅ Map {map_id} có {len(region_files)} file region:")
    for filepath in sorted(region_files):
        rel_path = os.path.relpath(filepath, maps_dir)
        file_size = os.path.getsize(filepath)
        print(f"   {rel_path} ({file_size:,} bytes)")

    return True


def list_available_region_maps():
    """Liệt kê tất cả maps có region files"""
    map_data = scan_all_region_files()

    print(f"\n{'='*70}")
    print(f"📋 DANH SÁCH CÁC MAP CÓ REGION FILES")
    print(f"{'='*70}")
    print(f"Tổng số: {len(map_data)} maps\n")

    print(f"{'MapID':<8} {'Region_C':<12} {'Region_S':<12} {'Total Files'}")
    print(f"{'-'*70}")

    for map_id in sorted(map_data.keys()):
        data = map_data[map_id]
        total = len(data['files'])
        print(f"{map_id:<8} {data['region_c']:<12} {data['region_s']:<12} {total}")

    print(f"\n{'='*70}")
    print(f"\n📊 THỐNG KÊ:")

    min_map = min(map_data.keys())
    max_map = max(map_data.keys())
    print(f"   Map ID nhỏ nhất có region: {min_map}")
    print(f"   Map ID lớn nhất có region: {max_map}")

    # Find gaps
    all_map_ids = set(map_data.keys())
    full_range = set(range(min_map, max_map + 1))
    missing = full_range - all_map_ids

    if missing:
        print(f"\n   Các map KHÔNG có region files (trong khoảng {min_map}-{max_map}):")
        missing_list = sorted(list(missing))
        for i in range(0, len(missing_list), 10):
            chunk = missing_list[i:i+10]
            print(f"   {', '.join(str(m) for m in chunk)}")

    print(f"\n{'='*70}\n")

    return map_data


def suggest_alternatives_for_map(map_id):
    """Gợi ý các cách thay thế để lấy tọa độ cho map không có region files"""
    print(f"\n{'='*70}")
    print(f"💡 CÁC CÁCH LẤY TỌA ĐỘ CHO MAP {map_id}")
    print(f"{'='*70}")

    print(f"""
1. TỪ FILE TRAP (nếu map có trap):
   - File: Bin/Server/library/maps/Trap/{map_id}.txt
   - Chứa: MapId, RegionId, CellX, CellY, ScriptFile
   - Sử dụng: python3 analyze_map.py {map_id}

2. TỪ FILE OBJECT (nếu map có objects):
   - File: Bin/Server/library/maps/Obj/{map_id}.txt
   - Chứa: ObjID, MapID, PosX, PosY (World coordinates)
   - Có thể convert: World → Region/Cell

3. TỪ FILE NPC:
   - File: Bin/Server/library/maps/Npc/{map_id}.txt
   - Chứa tọa độ spawn NPC (World coordinates)

4. TỪ CLIENT MAP FILES:
   - File client: *.map hoặc *.smap
   - Chứa toàn bộ map data bao gồm collision
   - Cần tool để parse (phức tạp hơn)

5. TỪ DATABASE (nếu có):
   - Kiểm tra table maps, regions, cells
   - Có thể có tọa độ được lưu trong DB

6. TỪ SCRIPT FILES:
   - Tìm trong script/maps/ xem có định nghĩa tọa độ không
   - Các function SetPos(), teleport thường có tọa độ cụ thể

7. TỰ TẠO TRAP/REGION DATA:
   - Nếu map không có dữ liệu, có thể tự tạo
   - Cần xác định kích thước map và chia region/cell
""")

    # Check what's available
    script_dir = os.path.dirname(os.path.abspath(__file__))
    base_dir = os.path.dirname(script_dir)

    print(f"\n📁 KIỂM TRA DỮ LIỆU CÓ SẴN CHO MAP {map_id}:")

    # Check trap file
    trap_file = os.path.join(base_dir, f"Bin/Server/library/maps/Trap/{map_id}.txt")
    if os.path.exists(trap_file):
        with open(trap_file, 'r', encoding='utf-8') as f:
            lines = len(f.readlines())
        print(f"   ✅ Trap file: {trap_file} ({lines} dòng)")
    else:
        print(f"   ❌ Trap file: Không tồn tại")

    # Check obj file
    obj_file = os.path.join(base_dir, f"Bin/Server/library/maps/Obj/{map_id}.txt")
    if os.path.exists(obj_file):
        with open(obj_file, 'r', encoding='utf-8') as f:
            lines = len(f.readlines())
        print(f"   ✅ Object file: {obj_file} ({lines} dòng)")
    else:
        print(f"   ❌ Object file: Không tồn tại")

    # Check npc file
    npc_file = os.path.join(base_dir, f"Bin/Server/library/maps/Npc/{map_id}.txt")
    if os.path.exists(npc_file):
        with open(npc_file, 'r', encoding='utf-8') as f:
            lines = len(f.readlines())
        print(f"   ✅ NPC file: {npc_file} ({lines} dòng)")
    else:
        print(f"   ❌ NPC file: Không tồn tại")

    print(f"\n{'='*70}\n")


if __name__ == "__main__":
    import sys

    if len(sys.argv) > 1:
        arg = sys.argv[1]

        if arg == "list":
            list_available_region_maps()
        elif arg.isdigit():
            map_id = int(arg)
            has_regions = check_map_regions(map_id)

            if not has_regions:
                suggest_alternatives_for_map(map_id)
        else:
            print("Sử dụng:")
            print("  python3 scan_region_files.py list        - Liệt kê tất cả maps có region")
            print("  python3 scan_region_files.py <map_id>    - Kiểm tra map cụ thể")
    else:
        list_available_region_maps()
