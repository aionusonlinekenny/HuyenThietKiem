#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
List all maps with trap data
"""

import os
import glob

def list_all_trap_maps():
    """Liệt kê tất cả các map có trap"""
    # Find the correct base path
    script_dir = os.path.dirname(os.path.abspath(__file__))
    base_dir = os.path.dirname(script_dir)
    trap_pattern = os.path.join(base_dir, "Bin/Server/library/maps/Trap/*.txt")
    trap_files = glob.glob(trap_pattern)
    map_ids = []

    for filepath in trap_files:
        filename = os.path.basename(filepath)
        map_id = filename.replace('.txt', '')
        if map_id.isdigit():
            map_ids.append(int(map_id))

    map_ids.sort()

    print(f"\n{'='*70}")
    print(f"📋 DANH SÁCH CÁC MAP CÓ TRAP DATA")
    print(f"{'='*70}")
    print(f"Tổng số: {len(map_ids)} maps\n")

    # Group by tens
    current_group = 0
    for i, map_id in enumerate(map_ids):
        if map_id // 10 > current_group:
            current_group = map_id // 10
            print()
        print(f"{map_id:3d}", end="  ")

    print(f"\n\n{'='*70}\n")

    return map_ids

def check_map_has_trap(map_id):
    """Kiểm tra xem map có trap không"""
    script_dir = os.path.dirname(os.path.abspath(__file__))
    base_dir = os.path.dirname(script_dir)
    trap_file = os.path.join(base_dir, f"Bin/Server/library/maps/Trap/{map_id}.txt")

    if os.path.exists(trap_file):
        print(f"✅ Map {map_id} có file trap tại: {trap_file}")

        # Count traps
        with open(trap_file, 'r', encoding='utf-8') as f:
            lines = f.readlines()
            trap_count = len([l for l in lines if l.strip() and not l.startswith('MapId')])

        print(f"   Số lượng trap: {trap_count}")
        return True
    else:
        print(f"❌ Map {map_id} KHÔNG có file trap")
        print(f"   File cần tạo: {trap_file}")
        return False

def suggest_trap_creation(map_id):
    """Gợi ý cách tạo trap cho map"""
    print(f"\n{'='*70}")
    print(f"💡 CÁCH TẠO TRAP CHO MAP {map_id}")
    print(f"{'='*70}")

    trap_file = f"Bin/Server/library/maps/Trap/{map_id}.txt"

    print(f"""
1. Tạo file: {trap_file}

2. Format của file (tab-separated):

   MapId\tRegionId\tCellX\tCellY\tScriptFile\tIsLoad

3. Ví dụ nội dung:

   MapId\tRegionId\tCellX\tCellY\tScriptFile\tIsLoad
   {map_id}\t100\t5\t10\t\\script\\maps\\trap\\{map_id}\\1.lua\t1
   {map_id}\t100\t6\t10\t\\script\\maps\\trap\\{map_id}\\1.lua\t1

4. Tạo script trap tại:
   Bin/Server/script/maps/trap/{map_id}/1.lua

5. Nội dung script mẫu:

   Include("\\\\script\\\\maps\\\\libtrap.lua");

   function main()
       local nMapId = {map_id};
       local nLevel = GetLevel();

       -- Teleport đến vị trí khác
       SetPos(1000, 2000);  -- World coordinates

       -- Hoặc add skill/buff
       AddSkillTrap();
   end;

6. Load trong Begin_Head.lua:
   - Function AddTrap({map_id}) sẽ tự động load file trap
   - Hoặc thêm vào function OnJoinMap()
""")
    print(f"{'='*70}\n")

if __name__ == "__main__":
    import sys

    if len(sys.argv) > 1:
        map_id = sys.argv[1]
        if map_id == "list":
            list_all_trap_maps()
        elif map_id.isdigit():
            has_trap = check_map_has_trap(int(map_id))
            if not has_trap:
                suggest_trap_creation(int(map_id))
        else:
            print("Sử dụng:")
            print("  python3 list_trap_maps.py list           - Liệt kê tất cả maps")
            print("  python3 list_trap_maps.py <map_id>       - Kiểm tra map cụ thể")
    else:
        list_all_trap_maps()
