#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Trap Finder Tool - Tìm kiếm và dò thông tin Region/Cell
"""

import os
import sys
from map_region_parser import MapCoordinateConverter, TrapFileParser


class TrapFinder:
    """Tool để tìm kiếm và phân tích trap data"""

    def __init__(self, base_path="Bin/Server/library/maps/Trap"):
        self.base_path = base_path
        self.converter = MapCoordinateConverter()

    def list_available_maps(self):
        """Liệt kê tất cả các map có file trap"""
        if not os.path.exists(self.base_path):
            print(f"❌ Không tìm thấy thư mục: {self.base_path}")
            return []

        maps = []
        for filename in os.listdir(self.base_path):
            if filename.endswith('.txt'):
                map_id = filename.replace('.txt', '')
                if map_id.isdigit():
                    maps.append(int(map_id))

        return sorted(maps)

    def analyze_map(self, map_id):
        """Phân tích chi tiết một map"""
        trap_file = os.path.join(self.base_path, f"{map_id}.txt")

        if not os.path.exists(trap_file):
            print(f"❌ Không tìm thấy file trap cho map {map_id}")
            return

        traps = TrapFileParser.parse_trap_file(trap_file)

        if not traps:
            print(f"❌ Không có trap nào trong map {map_id}")
            return

        # Thống kê
        regions = set()
        scripts = set()
        cell_coords = set()

        for trap in traps:
            regions.add(trap['RegionId'])
            scripts.add(trap['ScriptFile'])
            cell_coords.add((trap['CellX'], trap['CellY']))

        print(f"\n{'='*70}")
        print(f"📊 PHÂN TÍCH MAP {map_id}")
        print(f"{'='*70}")
        print(f"Tổng số trap:     {len(traps)}")
        print(f"Số Region:        {len(regions)}")
        print(f"Số Script khác:   {len(scripts)}")
        print(f"Số Cell khác:     {len(cell_coords)}")

        # Liệt kê các Region
        print(f"\n📍 DANH SÁCH REGIONS:")
        for region_id in sorted(regions):
            region_x, region_y = self.converter.parse_region_id(region_id)
            region_traps = [t for t in traps if t['RegionId'] == region_id]
            print(f"   RegionID {region_id:4d} (X={region_x:3d}, Y={region_y:3d}) → {len(region_traps):3d} traps")

        # Liệt kê các Script
        print(f"\n📜 DANH SÁCH SCRIPTS:")
        for script in sorted(scripts):
            script_traps = [t for t in traps if t['ScriptFile'] == script]
            print(f"   {script:50s} → {len(script_traps):3d} traps")

        # Range của coordinates
        min_world_x = min(t['WorldX'] for t in traps)
        max_world_x = max(t['WorldX'] for t in traps)
        min_world_y = min(t['WorldY'] for t in traps)
        max_world_y = max(t['WorldY'] for t in traps)

        print(f"\n🗺️  PHẠM VI TỌA ĐỘ WORLD:")
        print(f"   X: {min_world_x} → {max_world_x} (range: {max_world_x - min_world_x})")
        print(f"   Y: {min_world_y} → {max_world_y} (range: {max_world_y - min_world_y})")

        print(f"{'='*70}\n")

    def find_by_world_coords(self, map_id, world_x, world_y):
        """Tìm trap tại tọa độ world"""
        trap_file = os.path.join(self.base_path, f"{map_id}.txt")

        if not os.path.exists(trap_file):
            print(f"❌ Không tìm thấy file trap cho map {map_id}")
            return

        # Convert to region/cell
        region_x, region_y, cell_x, cell_y = self.converter.world_to_region_cell(world_x, world_y)
        region_id = self.converter.make_region_id(region_x, region_y)

        print(f"\n{'='*70}")
        print(f"🔍 TÌM TRAP TẠI TỌA ĐỘ WORLD ({world_x}, {world_y})")
        print(f"{'='*70}")
        print(f"Region:  ({region_x}, {region_y}) → RegionID = {region_id}")
        print(f"Cell:    ({cell_x}, {cell_y})")

        # Find matching trap
        traps = TrapFileParser.parse_trap_file(trap_file)
        matching_traps = [
            t for t in traps
            if t['RegionId'] == region_id and t['CellX'] == cell_x and t['CellY'] == cell_y
        ]

        if matching_traps:
            print(f"\n✅ Tìm thấy {len(matching_traps)} trap:")
            for trap in matching_traps:
                print(f"   Script: {trap['ScriptFile']}")
                print(f"   IsLoad: {trap['IsLoad']}")
        else:
            print(f"\n❌ Không tìm thấy trap tại vị trí này")

        # Show nearby traps
        nearby_traps = [
            t for t in traps
            if t['RegionId'] == region_id
            and abs(t['CellX'] - cell_x) <= 2
            and abs(t['CellY'] - cell_y) <= 2
        ]

        if nearby_traps:
            print(f"\n📍 Các trap gần đó (trong cùng Region, ±2 cells):")
            for trap in nearby_traps[:10]:
                dist = abs(trap['CellX'] - cell_x) + abs(trap['CellY'] - cell_y)
                print(f"   Cell({trap['CellX']:2d}, {trap['CellY']:2d}) [khoảng cách: {dist}] → {trap['ScriptFile']}")

        print(f"{'='*70}\n")

    def generate_area_mapping(self, map_id, region_id, output_file):
        """
        Tạo file mapping cho một Region cụ thể
        """
        trap_file = os.path.join(self.base_path, f"{map_id}.txt")

        if not os.path.exists(trap_file):
            print(f"❌ Không tìm thấy file trap cho map {map_id}")
            return

        traps = TrapFileParser.parse_trap_file(trap_file)
        region_traps = [t for t in traps if t['RegionId'] == region_id]

        if not region_traps:
            print(f"❌ Không có trap nào trong Region {region_id}")
            return

        # Create visual map
        grid = [[' ' for _ in range(self.converter.REGION_GRID_WIDTH)] for _ in range(self.converter.REGION_GRID_HEIGHT)]

        for trap in region_traps:
            cell_x = trap['CellX']
            cell_y = trap['CellY']
            if 0 <= cell_y < self.converter.REGION_GRID_HEIGHT and 0 <= cell_x < self.converter.REGION_GRID_WIDTH:
                grid[cell_y][cell_x] = 'X'

        # Write to file
        with open(output_file, 'w', encoding='utf-8') as f:
            region_x, region_y = self.converter.parse_region_id(region_id)
            f.write(f"Map {map_id} - Region {region_id} (X={region_x}, Y={region_y})\n")
            f.write(f"Total traps: {len(region_traps)}\n")
            f.write("=" * 80 + "\n\n")

            # Write grid
            f.write("    ")
            for x in range(self.converter.REGION_GRID_WIDTH):
                f.write(f"{x:2d} ")
            f.write("\n")

            for y in range(self.converter.REGION_GRID_HEIGHT):
                f.write(f"{y:2d}: ")
                for x in range(self.converter.REGION_GRID_WIDTH):
                    f.write(f" {grid[y][x]} ")
                f.write("\n")

            f.write("\n" + "=" * 80 + "\n\n")
            f.write("Trap Details:\n")
            f.write("-" * 80 + "\n")

            for trap in sorted(region_traps, key=lambda t: (t['CellY'], t['CellX'])):
                f.write(f"Cell({trap['CellX']:2d}, {trap['CellY']:2d}) → World({trap['WorldX']:6d}, {trap['WorldY']:6d}) → {trap['ScriptFile']}\n")

        print(f"✅ Đã tạo file mapping: {output_file}")


def interactive_menu():
    """Menu tương tác"""
    finder = TrapFinder()

    while True:
        print("\n" + "="*70)
        print("🛠️  TRAP FINDER TOOL")
        print("="*70)
        print("1. Liệt kê tất cả maps có trap")
        print("2. Phân tích một map cụ thể")
        print("3. Tìm trap theo tọa độ World (X, Y)")
        print("4. Tạo file mapping cho một Region")
        print("5. Chuyển đổi World → Region/Cell")
        print("6. Chuyển đổi Region/Cell → World")
        print("0. Thoát")
        print("="*70)

        choice = input("\nChọn chức năng (0-6): ").strip()

        if choice == '0':
            print("👋 Tạm biệt!")
            break

        elif choice == '1':
            maps = finder.list_available_maps()
            if maps:
                print(f"\n📋 Có {len(maps)} maps với trap data:")
                for i, map_id in enumerate(maps, 1):
                    print(f"   {i:3d}. Map {map_id}")
            else:
                print("❌ Không tìm thấy map nào")

        elif choice == '2':
            map_id = input("Nhập Map ID: ").strip()
            if map_id.isdigit():
                finder.analyze_map(int(map_id))
            else:
                print("❌ Map ID không hợp lệ")

        elif choice == '3':
            map_id = input("Nhập Map ID: ").strip()
            world_x = input("Nhập World X: ").strip()
            world_y = input("Nhập World Y: ").strip()

            if map_id.isdigit() and world_x.isdigit() and world_y.isdigit():
                finder.find_by_world_coords(int(map_id), int(world_x), int(world_y))
            else:
                print("❌ Tham số không hợp lệ")

        elif choice == '4':
            map_id = input("Nhập Map ID: ").strip()
            region_id = input("Nhập Region ID: ").strip()
            output_file = input("Nhập tên file output (ví dụ: region_map.txt): ").strip()

            if map_id.isdigit() and region_id.isdigit() and output_file:
                finder.generate_area_mapping(int(map_id), int(region_id), output_file)
            else:
                print("❌ Tham số không hợp lệ")

        elif choice == '5':
            world_x = input("Nhập World X: ").strip()
            world_y = input("Nhập World Y: ").strip()

            if world_x.isdigit() and world_y.isdigit():
                region_x, region_y, cell_x, cell_y = finder.converter.world_to_region_cell(
                    int(world_x), int(world_y)
                )
                region_id = finder.converter.make_region_id(region_x, region_y)

                print(f"\n✅ Kết quả chuyển đổi:")
                print(f"   World({world_x}, {world_y})")
                print(f"   → Region({region_x}, {region_y})")
                print(f"   → RegionID = {region_id}")
                print(f"   → Cell({cell_x}, {cell_y})")
            else:
                print("❌ Tham số không hợp lệ")

        elif choice == '6':
            region_x = input("Nhập Region X: ").strip()
            region_y = input("Nhập Region Y: ").strip()
            cell_x = input("Nhập Cell X (0-15): ").strip()
            cell_y = input("Nhập Cell Y (0-31): ").strip()

            if all(x.isdigit() for x in [region_x, region_y, cell_x, cell_y]):
                world_x, world_y = finder.converter.region_cell_to_world(
                    int(region_x), int(region_y), int(cell_x), int(cell_y)
                )
                region_id = finder.converter.make_region_id(int(region_x), int(region_y))

                print(f"\n✅ Kết quả chuyển đổi:")
                print(f"   Region({region_x}, {region_y}) → RegionID = {region_id}")
                print(f"   Cell({cell_x}, {cell_y})")
                print(f"   → World({world_x}, {world_y})")
            else:
                print("❌ Tham số không hợp lệ")

        else:
            print("❌ Lựa chọn không hợp lệ")


if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "-i":
        interactive_menu()
    else:
        print("Sử dụng: python3 trap_finder.py -i (để chạy menu tương tác)")
        print("\nHoặc import vào Python:")
        print("  from trap_finder import TrapFinder")
        print("  finder = TrapFinder()")
        print("  finder.analyze_map(11)")
