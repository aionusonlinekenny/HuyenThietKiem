#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Quick script để phân tích map trap data
"""

import sys
import os

# Add tools directory to path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'tools'))

from map_region_parser import MapCoordinateConverter, TrapFileParser


def analyze_map(map_id):
    """Phân tích một map cụ thể"""
    trap_file = f"Bin/Server/library/maps/Trap/{map_id}.txt"

    if not os.path.exists(trap_file):
        print(f"❌ Không tìm thấy file: {trap_file}")
        return

    traps = TrapFileParser.parse_trap_file(trap_file)

    if not traps:
        print(f"❌ Không có trap nào trong map {map_id}")
        return

    # Thống kê
    regions = {}
    scripts = {}

    for trap in traps:
        region_id = trap['RegionId']
        script = trap['ScriptFile']

        if region_id not in regions:
            regions[region_id] = []
        regions[region_id].append(trap)

        if script not in scripts:
            scripts[script] = []
        scripts[script].append(trap)

    print(f"\n{'='*70}")
    print(f"📊 PHÂN TÍCH MAP {map_id}")
    print(f"{'='*70}")
    print(f"Tổng số trap:     {len(traps)}")
    print(f"Số Region:        {len(regions)}")
    print(f"Số Script khác:   {len(scripts)}")

    # Liệt kê các Region
    print(f"\n📍 DANH SÁCH REGIONS:")
    converter = MapCoordinateConverter()
    for region_id in sorted(regions.keys()):
        region_x, region_y = converter.parse_region_id(region_id)
        region_traps = regions[region_id]
        print(f"   RegionID {region_id:4d} (X={region_x:3d}, Y={region_y:3d}) → {len(region_traps):3d} traps")

    # Liệt kê các Script
    print(f"\n📜 DANH SÁCH SCRIPTS:")
    for script in sorted(scripts.keys()):
        script_traps = scripts[script]
        print(f"   {script:50s} → {len(script_traps):3d} traps")

    # Show some examples
    print(f"\n📋 VÍ DỤ 10 TRAP ĐẦU TIÊN:")
    print(f"   {'MapId':<6} {'RegionID':<8} {'CellX':<6} {'CellY':<6} {'WorldX':<8} {'WorldY':<8} {'ScriptFile'}")
    print(f"   {'-'*90}")
    for i, trap in enumerate(traps[:10]):
        print(f"   {trap['MapId']:<6} {trap['RegionId']:<8} {trap['CellX']:<6} {trap['CellY']:<6} "
              f"{trap['WorldX']:<8} {trap['WorldY']:<8} {trap['ScriptFile']}")

    print(f"{'='*70}\n")


def show_coordinates_help():
    """Hiển thị hướng dẫn về tọa độ"""
    print(f"\n{'='*70}")
    print(f"📐 HƯỚNG DẪN TỌA ĐỘ")
    print(f"{'='*70}")
    print(f"""
Hệ thống tọa độ trong game:

1. REGION (Vùng)
   - Là một lưới ô 16x32 cells
   - RegionID = RegionX | (RegionY << 16)
   - Ví dụ: RegionID=92 → RegionX=92, RegionY=0

2. CELL (Ô)
   - Mỗi Region chia thành 16x32 cells
   - CellX: 0-15 (ngang)
   - CellY: 0-31 (dọc)
   - Mỗi cell = 32x32 pixels

3. WORLD COORDINATES (Tọa độ thế giới)
   - Tọa độ tuyệt đối trong game
   - WorldX = (RegionX * 16 + CellX) * 32
   - WorldY = (RegionY * 32 + CellY) * 32

CÔNG THỨC CHUYỂN ĐỔI:

▶ World → Region/Cell:
   RegionX = WorldX / 512
   RegionY = WorldY / 1024
   CellX = (WorldX % 512) / 32
   CellY = (WorldY % 1024) / 32

▶ Region/Cell → World:
   WorldX = (RegionX * 16 + CellX) * 32
   WorldY = (RegionY * 32 + CellY) * 32

VÍ DỤ:
   World(47328, 640) → Region(92, 0), Cell(7, 20)
   Region(92, 0), Cell(7, 20) → World(47328, 640)
""")
    print(f"{'='*70}\n")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Sử dụng:")
        print("  python3 analyze_map.py <map_id>        - Phân tích map")
        print("  python3 analyze_map.py help            - Hiển thị hướng dẫn tọa độ")
        print("\nVí dụ:")
        print("  python3 analyze_map.py 11")
        sys.exit(1)

    arg = sys.argv[1]

    if arg.lower() == 'help':
        show_coordinates_help()
    elif arg.isdigit():
        analyze_map(int(arg))
    else:
        print(f"❌ Tham số không hợp lệ: {arg}")
