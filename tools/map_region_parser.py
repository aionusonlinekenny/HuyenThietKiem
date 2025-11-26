#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Map Region Cell Parser Tool
Công cụ phân tích và chuyển đổi tọa độ Map/Region/Cell
"""

class MapCoordinateConverter:
    """Chuyển đổi giữa các hệ tọa độ trong game"""

    # Constants
    REGION_GRID_WIDTH = 16      # cells per region (width)
    REGION_GRID_HEIGHT = 32     # cells per region (height)
    LOGIC_CELL_WIDTH = 32       # pixels per cell (width)
    LOGIC_CELL_HEIGHT = 32      # pixels per cell (height)
    REGION_PIXEL_WIDTH = 512    # pixels per region (width) = 16 * 32
    REGION_PIXEL_HEIGHT = 1024  # pixels per region (height) = 32 * 32

    @staticmethod
    def world_to_region_cell(world_x, world_y):
        """
        Chuyển đổi tọa độ World (MPS) sang Region/Cell

        Args:
            world_x: Tọa độ X trong world (pixels)
            world_y: Tọa độ Y trong world (pixels)

        Returns:
            tuple: (region_x, region_y, cell_x, cell_y)
        """
        region_x = world_x // MapCoordinateConverter.REGION_PIXEL_WIDTH
        region_y = world_y // MapCoordinateConverter.REGION_PIXEL_HEIGHT

        cell_x = (world_x % MapCoordinateConverter.REGION_PIXEL_WIDTH) // MapCoordinateConverter.LOGIC_CELL_WIDTH
        cell_y = (world_y % MapCoordinateConverter.REGION_PIXEL_HEIGHT) // MapCoordinateConverter.LOGIC_CELL_HEIGHT

        return region_x, region_y, cell_x, cell_y

    @staticmethod
    def region_cell_to_world(region_x, region_y, cell_x, cell_y):
        """
        Chuyển đổi Region/Cell sang tọa độ World (MPS)

        Args:
            region_x: Region X coordinate
            region_y: Region Y coordinate
            cell_x: Cell X (0-15)
            cell_y: Cell Y (0-31)

        Returns:
            tuple: (world_x, world_y)
        """
        world_x = (region_x * MapCoordinateConverter.REGION_GRID_WIDTH + cell_x) * MapCoordinateConverter.LOGIC_CELL_WIDTH
        world_y = (region_y * MapCoordinateConverter.REGION_GRID_HEIGHT + cell_y) * MapCoordinateConverter.LOGIC_CELL_HEIGHT

        return world_x, world_y

    @staticmethod
    def make_region_id(region_x, region_y):
        """
        Tạo RegionID từ RegionX, RegionY (MAKELPARAM format)

        Args:
            region_x: Region X coordinate
            region_y: Region Y coordinate

        Returns:
            int: RegionID = RegionX | (RegionY << 16)
        """
        return region_x | (region_y << 16)

    @staticmethod
    def parse_region_id(region_id):
        """
        Phân tích RegionID thành RegionX, RegionY

        Args:
            region_id: RegionID (packed format)

        Returns:
            tuple: (region_x, region_y)
        """
        region_x = region_id & 0xFFFF  # LOWORD
        region_y = (region_id >> 16) & 0xFFFF  # HIWORD

        return region_x, region_y


class TrapFileParser:
    """Đọc và phân tích file Trap mapping"""

    @staticmethod
    def parse_trap_file(filepath):
        """
        Đọc file Trap mapping

        Args:
            filepath: Đường dẫn đến file Trap (ví dụ: Trap/11.txt)

        Returns:
            list: Danh sách các trap entries
        """
        traps = []

        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                lines = f.readlines()

                # Skip header
                for line in lines[1:]:
                    line = line.strip()
                    if not line:
                        continue

                    parts = line.split('\t')
                    if len(parts) >= 6:
                        trap = {
                            'MapId': int(parts[0]),
                            'RegionId': int(parts[1]),
                            'CellX': int(parts[2]),
                            'CellY': int(parts[3]),
                            'ScriptFile': parts[4],
                            'IsLoad': int(parts[5])
                        }

                        # Parse RegionID to RegionX, RegionY
                        region_x, region_y = MapCoordinateConverter.parse_region_id(trap['RegionId'])
                        trap['RegionX'] = region_x
                        trap['RegionY'] = region_y

                        # Calculate World coordinates
                        world_x, world_y = MapCoordinateConverter.region_cell_to_world(
                            region_x, region_y, trap['CellX'], trap['CellY']
                        )
                        trap['WorldX'] = world_x
                        trap['WorldY'] = world_y

                        traps.append(trap)

        except FileNotFoundError:
            print(f"❌ Không tìm thấy file: {filepath}")
        except Exception as e:
            print(f"❌ Lỗi khi đọc file: {e}")

        return traps

    @staticmethod
    def generate_trap_mapping(map_id, trap_data, output_file):
        """
        Tạo file Trap mapping từ dữ liệu

        Args:
            map_id: Map ID
            trap_data: List of trap dictionaries với keys: RegionId/RegionX/RegionY, CellX, CellY, ScriptFile
            output_file: Đường dẫn file output
        """
        try:
            with open(output_file, 'w', encoding='utf-8') as f:
                # Write header
                f.write("MapId\tRegionId\tCellX\tCellY\tScriptFile\tIsLoad\n")

                # Write trap entries
                for trap in trap_data:
                    region_id = trap.get('RegionId')
                    if region_id is None and 'RegionX' in trap and 'RegionY' in trap:
                        region_id = MapCoordinateConverter.make_region_id(
                            trap['RegionX'], trap['RegionY']
                        )

                    cell_x = trap.get('CellX', 0)
                    cell_y = trap.get('CellY', 0)
                    script_file = trap.get('ScriptFile', '')
                    is_load = trap.get('IsLoad', 1)

                    f.write(f"{map_id}\t{region_id}\t{cell_x}\t{cell_y}\t{script_file}\t{is_load}\n")

            print(f"✅ Đã tạo file: {output_file}")

        except Exception as e:
            print(f"❌ Lỗi khi tạo file: {e}")


def main():
    """Demo usage"""
    converter = MapCoordinateConverter()

    print("=" * 70)
    print("MAP REGION CELL COORDINATE CONVERTER")
    print("=" * 70)

    # Example 1: World to Region/Cell
    print("\n📍 Ví dụ 1: Chuyển đổi World coordinates sang Region/Cell")
    world_x, world_y = 11871, 832
    region_x, region_y, cell_x, cell_y = converter.world_to_region_cell(world_x, world_y)
    print(f"   World({world_x}, {world_y}) → Region({region_x}, {region_y}), Cell({cell_x}, {cell_y})")

    region_id = converter.make_region_id(region_x, region_y)
    print(f"   RegionID = {region_id}")

    # Example 2: Region/Cell to World
    print("\n📍 Ví dụ 2: Chuyển đổi Region/Cell sang World coordinates")
    region_x, region_y = 723, 0
    cell_x, cell_y = 15, 26
    world_x, world_y = converter.region_cell_to_world(region_x, region_y, cell_x, cell_y)
    print(f"   Region({region_x}, {region_y}), Cell({cell_x}, {cell_y}) → World({world_x}, {world_y})")

    # Example 3: Parse RegionID
    print("\n📍 Ví dụ 3: Phân tích RegionID")
    region_id = 92
    region_x, region_y = converter.parse_region_id(region_id)
    print(f"   RegionID {region_id} → RegionX={region_x}, RegionY={region_y}")

    region_id = 143
    region_x, region_y = converter.parse_region_id(region_id)
    print(f"   RegionID {region_id} → RegionX={region_x}, RegionY={region_y}")

    # Example 4: Parse trap file
    print("\n📍 Ví dụ 4: Đọc file Trap mapping")
    trap_file = "Bin/Server/library/maps/Trap/11.txt"
    traps = TrapFileParser.parse_trap_file(trap_file)

    if traps:
        print(f"   Đã đọc {len(traps)} traps từ file")
        print(f"\n   5 traps đầu tiên:")
        for i, trap in enumerate(traps[:5]):
            print(f"   [{i+1}] RegionID={trap['RegionId']} ({trap['RegionX']}, {trap['RegionY']}), "
                  f"Cell({trap['CellX']}, {trap['CellY']}), "
                  f"World({trap['WorldX']}, {trap['WorldY']}) → {trap['ScriptFile']}")

    print("\n" + "=" * 70)
    print("💡 Sử dụng:")
    print("   from map_region_parser import MapCoordinateConverter, TrapFileParser")
    print("   converter = MapCoordinateConverter()")
    print("   region_x, region_y, cell_x, cell_y = converter.world_to_region_cell(world_x, world_y)")
    print("=" * 70)


if __name__ == "__main__":
    main()
