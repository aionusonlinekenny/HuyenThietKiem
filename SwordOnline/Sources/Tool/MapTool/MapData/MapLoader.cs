using System;
using System.IO;
using System.Collections.Generic;
using MapTool.PakFile;

namespace MapTool.MapData
{
    /// <summary>
    /// Auto-loads complete map data from game folder
    /// Workflow: GameFolder + MapID → Auto load all regions
    /// Supports both .pak files and disk files
    /// </summary>
    public class MapLoader : IDisposable
    {
        private string _gameFolder;
        private bool _isServerMode;
        private MapListParser _mapListParser;
        private PakFileReader _pakReader;

        public MapLoader(string gameFolder, bool isServerMode = true)
        {
            _gameFolder = gameFolder;
            _isServerMode = isServerMode;
            _mapListParser = new MapListParser(gameFolder);

            // Try to open maps.pak
            TryOpenPakFile();
        }

        private void TryOpenPakFile()
        {
            string pakPath = Path.Combine(_gameFolder, "pak", "maps.pak");
            if (File.Exists(pakPath))
            {
                try
                {
                    _pakReader = new PakFileReader(pakPath);
                    Console.WriteLine($"✓ Opened pak file: {pakPath}");

                    // Show pak statistics
                    var stats = _pakReader.GetStatistics();
                    Console.WriteLine($"✓ Pak contains {stats.TotalFiles} files");
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"⚠ Warning: Could not open pak file: {ex.Message}");
                    _pakReader = null;
                }
            }
            else
            {
                Console.WriteLine($"ℹ No pak file found, will read from disk");
            }
        }

        /// <summary>
        /// Check if file exists (pak or disk)
        /// </summary>
        private bool FileExists(string relativePath)
        {
            // Try pak first
            if (_pakReader != null && _pakReader.FileExists(relativePath))
            {
                return true;
            }

            // Try disk
            string diskPath = Path.Combine(_gameFolder, relativePath.TrimStart('\\', '/'));
            return File.Exists(diskPath);
        }

        /// <summary>
        /// Read file (pak or disk)
        /// </summary>
        private byte[] ReadFileBytes(string relativePath)
        {
            // Try pak first
            if (_pakReader != null)
            {
                try
                {
                    byte[] data = _pakReader.ReadFile(relativePath);
                    if (data != null)
                    {
                        return data;
                    }
                }
                catch (NotImplementedException ex)
                {
                    // UCL decompression not implemented
                    throw new Exception(
                        $"❌ Cannot read compressed file from pak:\n{relativePath}\n\n" +
                        "═══════════════════════════════════════\n" +
                        ex.Message + "\n" +
                        "═══════════════════════════════════════\n\n" +
                        "📝 Recommended Solution:\n" +
                        "1. Extract maps.pak using UnpackTool\n" +
                        "2. Place extracted files in Bin/Server/maps/\n" +
                        "3. Reload the map\n\n" +
                        "Alternative: Implement UCL decompression (PInvoke to ucl.dll)");
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"⚠ Failed to read from pak: {ex.Message}");
                }
            }

            // Fallback to disk
            string diskPath = Path.Combine(_gameFolder, relativePath.TrimStart('\\', '/'));
            if (File.Exists(diskPath))
            {
                return File.ReadAllBytes(diskPath);
            }

            return null;
        }

        /// <summary>
        /// Load complete map data automatically
        /// </summary>
        public CompleteMapData LoadMap(int mapId)
        {
            // Step 1: Load MapList.ini
            _mapListParser.Load();
            var mapEntry = _mapListParser.GetMapEntry(mapId);

            if (mapEntry == null)
            {
                throw new Exception($"Map ID {mapId} not found in MapList.ini");
            }

            // Step 2: Load .wor file to get region grid
            string worPath = _mapListParser.GetMapWorPath(mapId);
            if (!File.Exists(worPath))
            {
                throw new FileNotFoundException($".wor file not found: {worPath}");
            }

            MapConfig config = MapFileParser.LoadMapConfig(worPath);

            // Step 3: Calculate region grid dimensions
            int regionWidth = config.RegionRight - config.RegionLeft + 1;
            int regionHeight = config.RegionBottom - config.RegionTop + 1;
            int totalRegions = regionWidth * regionHeight;

            // Step 4: Auto-load all regions
            CompleteMapData mapData = new CompleteMapData
            {
                MapId = mapId,
                MapName = mapEntry.Name,
                MapFolder = mapEntry.FolderPath,
                MapType = mapEntry.MapType,
                Config = config,
                RegionWidth = regionWidth,
                RegionHeight = regionHeight,
                Regions = new Dictionary<int, RegionData>()
            };

            string regionSuffix = _isServerMode ? "Region_S.dat" : "Region_C.dat";

            int loadedCount = 0;
            for (int y = config.RegionTop; y <= config.RegionBottom; y++)
            {
                for (int x = config.RegionLeft; x <= config.RegionRight; x++)
                {
                    // Build region file path: \maps\<mapfolder>\v_YYY\XXX_Region_S.dat
                    string regionRelativePath = Path.Combine(
                        "maps",
                        mapEntry.FolderPath,
                        $"v_{y:D3}",
                        $"{x:D3}_{regionSuffix}"
                    ).Replace(Path.DirectorySeparatorChar, '\\'); // Normalize to backslash

                    if (FileExists(regionRelativePath))
                    {
                        try
                        {
                            byte[] regionBytes = ReadFileBytes(regionRelativePath);
                            if (regionBytes != null)
                            {
                                // Parse region data from bytes
                                RegionData regionData = ParseRegionDataFromBytes(regionBytes, x, y);
                                mapData.Regions[regionData.RegionID] = regionData;
                                loadedCount++;
                            }
                        }
                        catch (NotImplementedException)
                        {
                            // Re-throw UCL decompression error with clear message
                            throw;
                        }
                        catch (Exception ex)
                        {
                            // Log error but continue loading other regions
                            Console.WriteLine($"⚠ Failed to load region ({x},{y}): {ex.Message}");
                        }
                    }
                }
            }

            mapData.LoadedRegionCount = loadedCount;
            return mapData;
        }

        /// <summary>
        /// Parse region data from byte array
        /// </summary>
        private RegionData ParseRegionDataFromBytes(byte[] data, int regionX, int regionY)
        {
            using (MemoryStream ms = new MemoryStream(data))
            using (BinaryReader reader = new BinaryReader(ms))
            {
                return MapFileParser.LoadRegionDataFromStream(reader, regionX, regionY);
            }
        }

        /// <summary>
        /// Get available map IDs from MapList.ini
        /// </summary>
        public List<int> GetAvailableMapIds()
        {
            _mapListParser.Load();
            return _mapListParser.GetAllMapIds();
        }

        /// <summary>
        /// Get map info without loading full data
        /// </summary>
        public MapListEntry GetMapInfo(int mapId)
        {
            _mapListParser.Load();
            return _mapListParser.GetMapEntry(mapId);
        }

        public void Dispose()
        {
            if (_pakReader != null)
            {
                _pakReader.Dispose();
                _pakReader = null;
            }
        }
    }

    /// <summary>
    /// Complete map data
    /// </summary>
    public class CompleteMapData
    {
        public int MapId { get; set; }
        public string MapName { get; set; }
        public string MapFolder { get; set; }
        public string MapType { get; set; }
        public MapConfig Config { get; set; }
        public int RegionWidth { get; set; }
        public int RegionHeight { get; set; }
        public int LoadedRegionCount { get; set; }
        public Dictionary<int, RegionData> Regions { get; set; }

        /// <summary>
        /// Get total map size in pixels
        /// </summary>
        public int GetMapPixelWidth()
        {
            return RegionWidth * MapConstants.REGION_PIXEL_WIDTH;
        }

        public int GetMapPixelHeight()
        {
            return RegionHeight * MapConstants.REGION_PIXEL_HEIGHT;
        }

        /// <summary>
        /// Get region by coordinates
        /// </summary>
        public RegionData GetRegion(int regionX, int regionY)
        {
            int regionId = CoordinateConverter.MakeRegionID(regionX, regionY);
            return Regions.ContainsKey(regionId) ? Regions[regionId] : null;
        }

        public override string ToString()
        {
            return $"Map {MapId}: {MapName} ({MapType}) - {LoadedRegionCount}/{RegionWidth * RegionHeight} regions loaded";
        }
    }
}
