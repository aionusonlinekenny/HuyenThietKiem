using System;
using System.IO;
using System.Text;

namespace MapTool.MapData
{
    /// <summary>
    /// Parser for map files (.wor, Region_C.dat, Region_S.dat)
    /// Based on KSubWorld::LoadMap and KRegion::LoadObject
    /// </summary>
    public class MapFileParser
    {
        /// <summary>
        /// Load map configuration from .wor file
        /// </summary>
        public static MapConfig LoadMapConfig(string worFilePath)
        {
            if (!File.Exists(worFilePath))
            {
                throw new FileNotFoundException($"Map config file not found: {worFilePath}");
            }

            MapConfig config = new MapConfig();
            config.MapPath = Path.GetDirectoryName(worFilePath);
            config.MapName = Path.GetFileNameWithoutExtension(worFilePath);

            // Parse INI file
            string[] lines = File.ReadAllLines(worFilePath, Encoding.GetEncoding("GB2312"));

            ParseWorContent(config, lines);
            return config;
        }

        /// <summary>
        /// Load map configuration from byte array (for pak file support)
        /// </summary>
        public static MapConfig LoadMapConfigFromBytes(byte[] worBytes, string mapName = "Unknown")
        {
            MapConfig config = new MapConfig();
            config.MapName = mapName;

            // Parse INI file from bytes
            string content = Encoding.GetEncoding("GB2312").GetString(worBytes);
            string[] lines = content.Split(new[] { "\r\n", "\r", "\n" }, StringSplitOptions.None);

            ParseWorContent(config, lines);
            return config;
        }

        /// <summary>
        /// Parse .wor file content (shared logic)
        /// </summary>
        private static void ParseWorContent(MapConfig config, string[] lines)
        {
            string currentSection = "";

            foreach (string line in lines)
            {
                string trimmed = line.Trim();

                // Skip comments and empty lines
                if (trimmed.StartsWith(";") || trimmed.StartsWith("#") || string.IsNullOrEmpty(trimmed))
                    continue;

                // Section header
                if (trimmed.StartsWith("[") && trimmed.EndsWith("]"))
                {
                    currentSection = trimmed.Substring(1, trimmed.Length - 2);
                    continue;
                }

                // Key=Value pair
                string[] parts = trimmed.Split(new[] { '=' }, 2);
                if (parts.Length != 2)
                    continue;

                string key = parts[0].Trim().ToLower();
                string value = parts[1].Trim();

                // Parse values based on key
                switch (key)
                {
                    case "isindoor":
                        config.IsIndoor = value == "1" || value.ToLower() == "true";
                        break;

                    case "rect":
                        // Format: rect=left,top,right,bottom
                        string[] coords = value.Split(',');
                        if (coords.Length == 4)
                        {
                            config.RegionLeft = int.Parse(coords[0].Trim());
                            config.RegionTop = int.Parse(coords[1].Trim());
                            config.RegionRight = int.Parse(coords[2].Trim());
                            config.RegionBottom = int.Parse(coords[3].Trim());

                            // DEBUG: Log parsed rect to file
                            DebugLogger.LogSeparator();
                            DebugLogger.Log($"🔍 PARSED RECT FROM .WOR FILE");
                            DebugLogger.Log($"   rect = {config.RegionLeft},{config.RegionTop},{config.RegionRight},{config.RegionBottom}");
                            DebugLogger.Log($"   Grid size: {config.RegionRight - config.RegionLeft + 1} x {config.RegionBottom - config.RegionTop + 1} regions");
                            DebugLogger.Log($"   First region coords: ({config.RegionLeft}, {config.RegionTop})");

                            // Calculate what RegionID SHOULD be for first region using Y*256+X formula
                            int firstRegionID = config.RegionTop * 256 + config.RegionLeft;
                            DebugLogger.Log($"   First RegionID (Y*256+X): {config.RegionTop}*256 + {config.RegionLeft} = {firstRegionID}");
                            DebugLogger.LogSeparator();
                        }
                        break;
                }
            }
        }

        /// <summary>
        /// Load region data from Region_C.dat or Region_S.dat file
        /// </summary>
        public static RegionData LoadRegion(string filePath, int regionX, int regionY)
        {
            if (!File.Exists(filePath))
            {
                throw new FileNotFoundException($"Region file not found: {filePath}");
            }

            RegionData region = new RegionData(regionX, regionY);
            region.FilePath = filePath;

            using (FileStream fs = new FileStream(filePath, FileMode.Open, FileAccess.Read))
            using (BinaryReader reader = new BinaryReader(fs))
            {
                // Read header: DWORD dwMaxElemFile
                uint elemCount = reader.ReadUInt32();

                if (elemCount != MapConstants.ELEM_FILE_COUNT)
                {
                    throw new InvalidDataException($"Invalid element count: {elemCount}, expected {MapConstants.ELEM_FILE_COUNT}");
                }

                // Read file sections
                for (int i = 0; i < elemCount; i++)
                {
                    region.FileSections[i].Offset = reader.ReadUInt32();
                    region.FileSections[i].Length = reader.ReadUInt32();
                }

                // Load obstacle data (Element 0)
                if (region.FileSections[MapConstants.ELEM_OBSTACLE].Length > 0)
                {
                    fs.Seek(region.FileSections[MapConstants.ELEM_OBSTACLE].Offset, SeekOrigin.Begin);

                    for (int y = 0; y < MapConstants.REGION_GRID_HEIGHT; y++)
                    {
                        for (int x = 0; x < MapConstants.REGION_GRID_WIDTH; x++)
                        {
                            region.Obstacles[x, y] = reader.ReadInt32(); // long = 4 bytes
                        }
                    }
                }

                // Load trap data (Element 1)
                if (region.FileSections[MapConstants.ELEM_TRAP].Length > 0)
                {
                    fs.Seek(region.FileSections[MapConstants.ELEM_TRAP].Offset, SeekOrigin.Begin);

                    for (int y = 0; y < MapConstants.REGION_GRID_HEIGHT; y++)
                    {
                        for (int x = 0; x < MapConstants.REGION_GRID_WIDTH; x++)
                        {
                            region.Traps[x, y] = reader.ReadUInt32(); // DWORD = 4 bytes
                        }
                    }
                }

                // Note: NPC, Object, Ground, BuildinObj data parsing can be added here if needed
            }

            region.IsLoaded = true;
            return region;
        }

        /// <summary>
        /// Load region data from BinaryReader stream (for pak file support)
        /// </summary>
        public static RegionData LoadRegionDataFromStream(BinaryReader reader, int regionX, int regionY)
        {
            RegionData region = new RegionData(regionX, regionY);

            // Read header: DWORD dwMaxElemFile
            uint elemCount = reader.ReadUInt32();

            if (elemCount != MapConstants.ELEM_FILE_COUNT)
            {
                throw new InvalidDataException($"Invalid element count: {elemCount}, expected {MapConstants.ELEM_FILE_COUNT}");
            }

            // Read file sections
            for (int i = 0; i < elemCount; i++)
            {
                region.FileSections[i].Offset = reader.ReadUInt32();
                region.FileSections[i].Length = reader.ReadUInt32();
            }

            // Load obstacle data (Element 0)
            if (region.FileSections[MapConstants.ELEM_OBSTACLE].Length > 0)
            {
                reader.BaseStream.Seek(region.FileSections[MapConstants.ELEM_OBSTACLE].Offset, SeekOrigin.Begin);

                for (int y = 0; y < MapConstants.REGION_GRID_HEIGHT; y++)
                {
                    for (int x = 0; x < MapConstants.REGION_GRID_WIDTH; x++)
                    {
                        region.Obstacles[x, y] = reader.ReadInt32(); // long = 4 bytes
                    }
                }
            }

            // Load trap data (Element 1)
            if (region.FileSections[MapConstants.ELEM_TRAP].Length > 0)
            {
                reader.BaseStream.Seek(region.FileSections[MapConstants.ELEM_TRAP].Offset, SeekOrigin.Begin);

                for (int y = 0; y < MapConstants.REGION_GRID_HEIGHT; y++)
                {
                    for (int x = 0; x < MapConstants.REGION_GRID_WIDTH; x++)
                    {
                        region.Traps[x, y] = reader.ReadUInt32(); // DWORD = 4 bytes
                    }
                }
            }

            region.IsLoaded = true;
            return region;
        }

        /// <summary>
        /// Quick check if region file exists
        /// </summary>
        public static bool RegionFileExists(string mapPath, int regionX, int regionY, bool isServer = false)
        {
            string filePath = CoordinateConverter.GetRegionFilePath(mapPath, regionX, regionY, isServer);
            return File.Exists(filePath);
        }

        /// <summary>
        /// Get all available regions in a map
        /// </summary>
        public static System.Collections.Generic.List<(int x, int y)> GetAvailableRegions(string mapPath, bool isServer = false)
        {
            var regions = new System.Collections.Generic.List<(int x, int y)>();

            if (!Directory.Exists(mapPath))
                return regions;

            // Search all v_XXX directories
            foreach (string vDir in Directory.GetDirectories(mapPath, "v_*"))
            {
                int regionY = int.Parse(Path.GetFileName(vDir).Substring(2));

                // Search all region files in this directory
                string pattern = isServer ? "*_Region_S.dat" : "*_Region_C.dat";
                foreach (string file in Directory.GetFiles(vDir, pattern))
                {
                    string filename = Path.GetFileNameWithoutExtension(file);
                    int regionX = int.Parse(filename.Split('_')[0]);

                    regions.Add((regionX, regionY));
                }
            }

            return regions;
        }
    }
}
