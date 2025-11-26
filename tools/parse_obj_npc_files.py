#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Parse Object và NPC files để lấy tọa độ World và convert sang Region/Cell
"""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from map_region_parser import MapCoordinateConverter


def parse_obj_file(map_id):
    """Parse file Object để lấy tọa độ"""
    script_dir = os.path.dirname(os.path.abspath(__file__))
    base_dir = os.path.dirname(script_dir)
    obj_file = os.path.join(base_dir, f"Bin/Server/library/maps/Obj/{map_id}.txt")

    if not os.path.exists(obj_file):
        print(f"❌ Không tìm thấy file Object cho map {map_id}")
        return []

    converter = MapCoordinateConverter()
    objects = []

    with open(obj_file, 'r', encoding='utf-8') as f:
        lines = f.readlines()

        for i, line in enumerate(lines[1:], start=2):  # Skip header
            line = line.strip()
            if not line:
                continue

            parts = line.split('\t')
            if len(parts) >= 8:
                try:
                    obj_id = int(parts[0])
                    map_id_check = int(parts[1])
                    world_x = int(parts[2])
                    world_y = int(parts[3])
                    direction = int(parts[4])
                    state = int(parts[5])
                    script_file = parts[6]
                    is_load = int(parts[7])

                    # Convert to Region/Cell
                    region_x, region_y, cell_x, cell_y = converter.world_to_region_cell(world_x, world_y)
                    region_id = converter.make_region_id(region_x, region_y)

                    obj = {
                        'ObjID': obj_id,
                        'MapID': map_id_check,
                        'WorldX': world_x,
                        'WorldY': world_y,
                        'RegionX': region_x,
                        'RegionY': region_y,
                        'RegionID': region_id,
                        'CellX': cell_x,
                        'CellY': cell_y,
                        'Direction': direction,
                        'State': state,
                        'ScriptFile': script_file,
                        'IsLoad': is_load
                    }
                    objects.append(obj)
                except (ValueError, IndexError) as e:
                    print(f"⚠️  Lỗi parse dòng {i}: {e}")
                    continue

    return objects


def parse_npc_file(map_id):
    """Parse file NPC để lấy tọa độ"""
    script_dir = os.path.dirname(os.path.abspath(__file__))
    base_dir = os.path.dirname(script_dir)
    npc_file = os.path.join(base_dir, f"Bin/Server/library/maps/Npc/{map_id}.txt")

    if not os.path.exists(npc_file):
        print(f"❌ Không tìm thấy file NPC cho map {map_id}")
        return []

    converter = MapCoordinateConverter()
    npcs = []

    with open(npc_file, 'r', encoding='utf-8', errors='ignore') as f:
        lines = f.readlines()

        for i, line in enumerate(lines[1:], start=2):  # Skip header
            line = line.strip()
            if not line:
                continue

            parts = line.split('\t')
            if len(parts) >= 10:
                try:
                    npc_id = int(parts[0])
                    map_id_check = int(parts[1])
                    world_x = int(parts[2])
                    world_y = int(parts[3])
                    # Other fields...

                    # Convert to Region/Cell
                    region_x, region_y, cell_x, cell_y = converter.world_to_region_cell(world_x, world_y)
                    region_id = converter.make_region_id(region_x, region_y)

                    npc = {
                        'NpcID': npc_id,
                        'MapID': map_id_check,
                        'WorldX': world_x,
                        'WorldY': world_y,
                        'RegionX': region_x,
                        'RegionY': region_y,
                        'RegionID': region_id,
                        'CellX': cell_x,
                        'CellY': cell_y
                    }
                    npcs.append(npc)
                except (ValueError, IndexError) as e:
                    continue

    return npcs


def analyze_map_coordinates(map_id):
    """Phân tích tọa độ từ tất cả nguồn có sẵn cho map"""
    print(f"\n{'='*70}")
    print(f"📍 PHÂN TÍCH TỌA ĐỘ CHO MAP {map_id}")
    print(f"{'='*70}\n")

    # Try Object file
    objects = parse_obj_file(map_id)
    if objects:
        print(f"✅ Tìm thấy {len(objects)} objects trong file Obj/{map_id}.txt\n")

        # Thống kê regions
        regions = {}
        for obj in objects:
            region_id = obj['RegionID']
            if region_id not in regions:
                regions[region_id] = []
            regions[region_id].append(obj)

        print(f"📊 THỐNG KÊ REGIONS:")
        print(f"{'RegionID':<12} {'RegionX':<10} {'RegionY':<10} {'Số Objects'}")
        print(f"{'-'*70}")
        for region_id in sorted(regions.keys()):
            region_x, region_y = MapCoordinateConverter.parse_region_id(region_id)
            count = len(regions[region_id])
            print(f"{region_id:<12} {region_x:<10} {region_y:<10} {count}")

        # Hiển thị một số objects mẫu
        print(f"\n📋 10 OBJECTS ĐẦU TIÊN (với tọa độ chi tiết):")
        print(f"{'ObjID':<8} {'WorldX':<10} {'WorldY':<10} {'RegionID':<10} {'CellX':<7} {'CellY':<7} {'Script'}")
        print(f"{'-'*70}")
        for obj in objects[:10]:
            script_short = obj['ScriptFile'].split('/')[-1] if '/' in obj['ScriptFile'] else obj['ScriptFile']
            print(f"{obj['ObjID']:<8} {obj['WorldX']:<10} {obj['WorldY']:<10} "
                  f"{obj['RegionID']:<10} {obj['CellX']:<7} {obj['CellY']:<7} {script_short}")

        # Range
        min_x = min(obj['WorldX'] for obj in objects)
        max_x = max(obj['WorldX'] for obj in objects)
        min_y = min(obj['WorldY'] for obj in objects)
        max_y = max(obj['WorldY'] for obj in objects)

        print(f"\n🗺️  PHẠM VI TỌA ĐỘ:")
        print(f"   WorldX: {min_x:,} → {max_x:,} (khoảng: {max_x - min_x:,})")
        print(f"   WorldY: {min_y:,} → {max_y:,} (khoảng: {max_y - min_y:,})")

        # Convert to region range
        min_rx, min_ry, _, _ = MapCoordinateConverter.world_to_region_cell(min_x, min_y)
        max_rx, max_ry, _, _ = MapCoordinateConverter.world_to_region_cell(max_x, max_y)

        print(f"\n   RegionX: {min_rx} → {max_rx} (tổng: {max_rx - min_rx + 1} regions)")
        print(f"   RegionY: {min_ry} → {max_ry} (tổng: {max_ry - min_ry + 1} regions)")

    else:
        # Try NPC file
        npcs = parse_npc_file(map_id)
        if npcs:
            print(f"✅ Tìm thấy {len(npcs)} NPCs trong file Npc/{map_id}.txt\n")

            # Similar statistics for NPCs
            regions = {}
            for npc in npcs:
                region_id = npc['RegionID']
                if region_id not in regions:
                    regions[region_id] = []
                regions[region_id].append(npc)

            print(f"📊 THỐNG KÊ REGIONS:")
            for region_id in sorted(regions.keys()):
                region_x, region_y = MapCoordinateConverter.parse_region_id(region_id)
                count = len(regions[region_id])
                print(f"   RegionID {region_id} (X={region_x}, Y={region_y}): {count} NPCs")
        else:
            print(f"❌ Không tìm thấy file Object hoặc NPC cho map {map_id}")
            print(f"\n💡 Map {map_id} có thể:")
            print(f"   - Không có objects/NPCs được định nghĩa")
            print(f"   - Sử dụng spawn động trong scripts")
            print(f"   - Là map đặc biệt (dungeon, instance, etc.)")

    print(f"\n{'='*70}\n")


def export_to_trap_format(map_id, output_file):
    """Export tọa độ từ Object/NPC sang format Trap"""
    objects = parse_obj_file(map_id)

    if not objects and not npcs:
        print(f"❌ Không có dữ liệu để export")
        return

    with open(output_file, 'w', encoding='utf-8') as f:
        f.write("MapId\tRegionId\tCellX\tCellY\tScriptFile\tIsLoad\n")

        if objects:
            for obj in objects:
                f.write(f"{obj['MapID']}\t{obj['RegionID']}\t{obj['CellX']}\t{obj['CellY']}\t"
                        f"{obj['ScriptFile']}\t{obj['IsLoad']}\n")

    print(f"✅ Đã export {len(objects)} entries sang {output_file}")


if __name__ == "__main__":
    if len(sys.argv) > 1:
        map_id = int(sys.argv[1])
        analyze_map_coordinates(map_id)

        if len(sys.argv) > 2 and sys.argv[2] == "export":
            output_file = f"map_{map_id}_coordinates.txt"
            export_to_trap_format(map_id, output_file)
    else:
        print("Sử dụng:")
        print("  python3 parse_obj_npc_files.py <map_id>          - Phân tích tọa độ")
        print("  python3 parse_obj_npc_files.py <map_id> export   - Export sang file")
        print("\nVí dụ:")
        print("  python3 parse_obj_npc_files.py 20")
        print("  python3 parse_obj_npc_files.py 1 export")
