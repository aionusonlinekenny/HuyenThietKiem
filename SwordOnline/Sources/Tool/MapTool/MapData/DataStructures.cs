using System;

namespace MapTool.MapData
{
    /// <summary>
    /// Constants for map coordinate system
    /// </summary>
    public static class MapConstants
    {
        // Cell dimensions
        public const int LOGIC_CELL_WIDTH = 32;
        public const int LOGIC_CELL_HEIGHT = 32;

        // Region dimensions (in cells)
        public const int REGION_GRID_WIDTH = 16;
        public const int REGION_GRID_HEIGHT = 32;

        // Region dimensions (in pixels)
        public const int REGION_PIXEL_WIDTH = 512;   // 16 * 32
        public const int REGION_PIXEL_HEIGHT = 1024; // 32 * 32

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
            return x | (y << 16);
        }

        public static void ParseRegionID(int regionID, out int x, out int y)
        {
            x = regionID & 0xFFFF;
            y = (regionID >> 16) & 0xFFFF;
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
    }
}
