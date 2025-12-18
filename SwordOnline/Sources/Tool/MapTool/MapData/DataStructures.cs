using System;

namespace MapTool.MapData
{
    /// <summary>
    /// Constants for map coordinate system
    /// </summary>
    public static class MapConstants
    {
        // Cell dimensions (logic/scene coordinates)
        public const int LOGIC_CELL_WIDTH = 32;
        public const int LOGIC_CELL_HEIGHT = 32;

        // Region dimensions (in cells)
        public const int REGION_GRID_WIDTH = 16;
        public const int REGION_GRID_HEIGHT = 32;

        // Region dimensions (in logic pixels) - for game data
        public const int REGION_PIXEL_WIDTH = 512;   // 16 * 32
        public const int REGION_PIXEL_HEIGHT = 1024; // 32 * 32

        // Map image rendering (24.jpg coordinates) - from client MAP_SCALE_H/V
        // Client uses scale factors: world / 16 (H) and world / 32 (V)
        // MapPixelX = (RegionX * REGION_PIXEL_WIDTH) / MAP_SCALE_H = (RegionX * 512) / 16 = RegionX * 32
        // MapPixelY = (RegionY * REGION_PIXEL_HEIGHT) / MAP_SCALE_V = (RegionY * 1024) / 32 = RegionY * 32
        // Result: 1 region = 32x32 pixels on 24.jpg (confirmed from client MAP_A_REGION_NUM_MAP_PIXEL_H/V)
        public const int MAP_SCALE_H = 16;
        public const int MAP_SCALE_V = 32;
        public const int MAP_REGION_PIXEL_WIDTH = 32;   // REGION_PIXEL_WIDTH / MAP_SCALE_H = 512 / 16 = 32
        public const int MAP_REGION_PIXEL_HEIGHT = 32;  // REGION_PIXEL_HEIGHT / MAP_SCALE_V = 1024 / 32 = 32

        // File names
        public const string REGION_CLIENT_FILE = "Region_C.dat";
        public const string REGION_SERVER_FILE = "Region_S.dat";

        // Element file indices in combined file
        public const int ELEM_OBSTACLE = 0;
        public const int ELEM_TRAP = 1;
        public const int ELEM_NPC = 2;
        public const int ELEM_OBJECT = 3;
        public const int ELEM_GROUND = 4;
        public const int ELEM_BUILDIN = 5;
        public const int ELEM_FILE_COUNT = 6;
    }

    /// <summary>
    /// Combined file section header
    /// </summary>
    [Serializable]
    public struct KCombinFileSection
    {
        public uint Offset;  // File offset in bytes
        public uint Length;  // Data length in bytes
    }

    /// <summary>
    /// Region data container
    /// </summary>
    public class RegionData
    {
        public int RegionX { get; set; }
        public int RegionY { get; set; }
        public int RegionID { get; set; }

        // Obstacle grid: 16x32 longs
        public long[,] Obstacles { get; set; }

        // Trap grid: 16x32 DWORDs
        public uint[,] Traps { get; set; }

        // Metadata
        public KCombinFileSection[] FileSections { get; set; }
        public string FilePath { get; set; }
        public bool IsLoaded { get; set; }

        public RegionData(int regionX, int regionY)
        {
            RegionX = regionX;
            RegionY = regionY;
            RegionID = MakeRegionID(regionX, regionY);

            Obstacles = new long[MapConstants.REGION_GRID_WIDTH, MapConstants.REGION_GRID_HEIGHT];
            Traps = new uint[MapConstants.REGION_GRID_WIDTH, MapConstants.REGION_GRID_HEIGHT];
            FileSections = new KCombinFileSection[MapConstants.ELEM_FILE_COUNT];
            IsLoaded = false;
        }

        public static int MakeRegionID(int x, int y)
        {
            // GLOBAL formula: Y * 256 + X
            // Used for dictionary keys and internal representation
            // NOT for trap export files!
            return y * 256 + x;
        }

        public static int MakeLocalRegionID(int x, int y, int minX, int minY, int width)
        {
            // LOCAL formula: (Y - minY) * width + (X - minX)
            // Used for trap export files that are relative to map rect
            return (y - minY) * width + (x - minX);
        }

        public int GetLocalRegionID(int minX, int minY, int width)
        {
            // Calculate local RegionID for this region based on map rect
            return MakeLocalRegionID(RegionX, RegionY, minX, minY, width);
        }

        public static void ParseRegionID(int regionID, out int x, out int y)
        {
            // Parse using Y * 256 + X formula
            x = regionID % 256;
            y = regionID / 256;
        }

        public static void ParseLocalRegionID(int localRegionID, int minX, int minY, int width, out int x, out int y)
        {
            // Parse LOCAL RegionID back to region coordinates
            // LOCAL formula: (y - minY) * width + (x - minX) = localRegionID
            // Inverse:
            int localY = localRegionID / width;
            int localX = localRegionID % width;
            x = localX + minX;
            y = localY + minY;
        }
    }

    /// <summary>
    /// Map configuration from .wor file
    /// </summary>
    public class MapConfig
    {
        public string MapName { get; set; }
        public string MapPath { get; set; }
        public bool IsIndoor { get; set; }

        // Region boundaries
        public int RegionLeft { get; set; }
        public int RegionTop { get; set; }
        public int RegionRight { get; set; }
        public int RegionBottom { get; set; }

        public int RegionWidth => RegionRight - RegionLeft + 1;
        public int RegionHeight => RegionBottom - RegionTop + 1;
        public int TotalRegions => RegionWidth * RegionHeight;
    }

    /// <summary>
    /// Coordinate point in different systems
    /// </summary>
    public struct MapCoordinate
    {
        public int WorldX;
        public int WorldY;
        public int RegionX;
        public int RegionY;
        public int RegionID;
        public int CellX;
        public int CellY;
        public int OffsetX;
        public int OffsetY;

        public override string ToString()
        {
            return $"World({WorldX}, {WorldY}) → Region({RegionX}, {RegionY})[ID:{RegionID}], Cell({CellX}, {CellY}), Offset({OffsetX}, {OffsetY})";
        }
    }

    /// <summary>
    /// Trap entry for export
    /// </summary>
    public class TrapEntry
    {
        public int MapId { get; set; }
        public int RegionId { get; set; }
        public int CellX { get; set; }
        public int CellY { get; set; }
        public string ScriptFile { get; set; }
        public int IsLoad { get; set; }

        public TrapEntry()
        {
            IsLoad = 1;
            ScriptFile = @"\script\maps\trap\1\1.lua";
        }

        public override string ToString()
        {
            return $"{MapId}\t{RegionId}\t{CellX}\t{CellY}\t{ScriptFile}\t{IsLoad}";
        }

        /// <summary>
        /// Convert RegionId + Cell to World coordinates
        /// </summary>
        public void GetWorldCoordinates(MapConfig mapConfig, out int worldX, out int worldY)
        {
            // Convert local RegionID back to region X,Y
            RegionData.ParseLocalRegionID(RegionId, mapConfig.RegionLeft, mapConfig.RegionTop,
                mapConfig.RegionWidth, out int regionX, out int regionY);

            // Convert region + cell to world
            CoordinateConverter.RegionCellToWorld(regionX, regionY, CellX, CellY, out worldX, out worldY);
        }
    }
}
