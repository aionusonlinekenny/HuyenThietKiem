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
            // Try multiple possible pak file locations
            string[] possiblePaths = new[]
            {
                Path.Combine(_gameFolder, "pak", "maps.pak"),              // Server: Bin/Server/pak/maps.pak
                Path.Combine(_gameFolder, "..", "pak", "maps.pak"),         // Bin/Client/../pak/maps.pak (if exists)
                Path.Combine(_gameFolder, "..", "Server", "pak", "maps.pak"), // Client: Bin/Client/../Server/pak/maps.pak
                Path.Combine(_gameFolder, "maps.pak"),                     // Direct: Bin/maps.pak
            };

            foreach (string pakPath in possiblePaths)
            {
                if (File.Exists(pakPath))
                {
                    try
                    {
                        _pakReader = new PakFileReader(pakPath);
                        Console.WriteLine($"✓ Opened pak file: {pakPath}");

                        // Show pak statistics
                        var stats = _pakReader.GetStatistics();
                        Console.WriteLine($"✓ Pak contains {stats.TotalFiles} files");
                        return; // Success!
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine($"⚠ Warning: Could not open pak file {pakPath}: {ex.Message}");
                        _pakReader = null;
                    }
                }
            }

            Console.WriteLine($"ℹ No pak file found at any location, will read from disk");
            Console.WriteLine($"  Tried paths:");
            foreach (string path in possiblePaths)
            {
                Console.WriteLine($"    - {path}");
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
            MapConfig config;
            string worRelativePath = _mapListParser.GetMapWorRelativePath(mapId);

            // Try to read from pak first, then disk
            if (FileExists(worRelativePath))
            {
                byte[] worBytes = ReadFileBytes(worRelativePath);
                if (worBytes != null)
                {
                    Console.WriteLine($"✓ Loaded .wor from pak: {worRelativePath}");
                    config = MapFileParser.LoadMapConfigFromBytes(worBytes, mapEntry.Name);
                }
                else
                {
                    throw new FileNotFoundException($".wor file not found: {worRelativePath}");
                }
            }
            else
            {
                // Fallback to disk
                string worPath = _mapListParser.GetMapWorPath(mapId);
                if (File.Exists(worPath))
                {
                    Console.WriteLine($"✓ Loaded .wor from disk: {worPath}");
                    config = MapFileParser.LoadMapConfig(worPath);
                }
                else
                {
                    throw new FileNotFoundException($".wor file not found in pak ({worRelativePath}) or disk ({worPath})");
                }
            }

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
            int attemptedCount = 0;
            List<string> existingRegions = new List<string>();
            List<string> missingRegions = new List<string>();

            DebugLogger.LogSeparator();
            DebugLogger.Log($"🔍 SCANNING FOR REGION FILES");
            DebugLogger.Log($"   Looking for regions from ({config.RegionLeft},{config.RegionTop}) to ({config.RegionRight},{config.RegionBottom})");

            for (int y = config.RegionTop; y <= config.RegionBottom; y++)
            {
                for (int x = config.RegionLeft; x <= config.RegionRight; x++)
                {
                    attemptedCount++;

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

                                // Log first 5 regions found
                                if (loadedCount <= 5)
                                {
                                    int regionId = y * 256 + x;
                                    DebugLogger.Log($"   ✓ Found region ({x:3d},{y:3d}) → RegionID={regionId,5} | File: {regionRelativePath}");
                                    existingRegions.Add($"({x},{y})");
                                }
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
                            DebugLogger.Log($"   ⚠ Error loading ({x},{y}): {ex.Message}");
                        }
                    }
                    else
                    {
                        // Log first 5 missing regions
                        if (missingRegions.Count < 5)
                        {
                            DebugLogger.Log($"   ✗ Missing region ({x:3d},{y:3d}) | Expected: {regionRelativePath}");
                            missingRegions.Add($"({x},{y})");
                        }
                    }
                }
            }

            DebugLogger.LogSeparator();
            DebugLogger.Log($"📊 REGION SCAN SUMMARY");
            DebugLogger.Log($"   Attempted: {attemptedCount} regions");
            DebugLogger.Log($"   Loaded: {loadedCount} regions");
            DebugLogger.Log($"   Missing: {attemptedCount - loadedCount} regions");
            if (missingRegions.Count > 0)
            {
                DebugLogger.Log($"   First missing: {string.Join(", ", missingRegions)}");
            }
            if (existingRegions.Count > 0)
            {
                DebugLogger.Log($"   First existing: {string.Join(", ", existingRegions)}");
            }

            // Check for data mismatch - warn if most regions are missing
            if (loadedCount == 0)
            {
                DebugLogger.LogSeparator();
                DebugLogger.Log($"❌ CRITICAL: NO REGIONS LOADED!");
                DebugLogger.Log($"   The .wor file says regions should exist at ({config.RegionLeft},{config.RegionTop}) to ({config.RegionRight},{config.RegionBottom})");
                DebugLogger.Log($"   But NO region files were found at these coordinates!");
                DebugLogger.Log($"");
                DebugLogger.Log($"   Possible causes:");
                DebugLogger.Log($"   1. Map files not extracted from pak");
                DebugLogger.Log($"   2. Wrong game folder selected");
                DebugLogger.Log($"   3. .wor file doesn't match actual region files (different version)");
                DebugLogger.LogSeparator();
            }
            else if ((double)loadedCount / attemptedCount < 0.1) // Less than 10% loaded
            {
                DebugLogger.LogSeparator();
                DebugLogger.Log($"⚠ WARNING: DATA MISMATCH DETECTED!");
                DebugLogger.Log($"   Only {loadedCount}/{attemptedCount} regions loaded ({(double)loadedCount / attemptedCount * 100:F1}%)");
                DebugLogger.Log($"   .wor expects: ({config.RegionLeft},{config.RegionTop}) to ({config.RegionRight},{config.RegionBottom})");
                DebugLogger.Log($"   Actual regions: Starting around {existingRegions[0]}");
                DebugLogger.Log($"");
                DebugLogger.Log($"   This means your .wor file doesn't match your region files!");
                DebugLogger.Log($"   The exported RegionIDs will be WRONG for trap placement.");
                DebugLogger.Log($"");
                DebugLogger.Log($"   📝 SOLUTION:");
                DebugLogger.Log($"   Compare with Bin/Server/library/maps/Trap/{mapId}.txt to see correct RegionIDs");
                DebugLogger.Log($"   You may need to extract correct map files from original game data");
                DebugLogger.LogSeparator();
            }

            DebugLogger.LogSeparator();

            mapData.LoadedRegionCount = loadedCount;

            // Step 5: Try to load map image (24.jpg)
            // NEW: Simple and correct image path construction with encoding fallback
            // FolderPath format: "西北南区\成都\成都"
            // Image path: "\maps\西北南区\成都\成都24.jpg"

            string mapImageRelativePath = $"\\maps\\{mapEntry.FolderPath}24.jpg";

            DebugLogger.Log($"🖼️  LOADING MAP IMAGE");
            DebugLogger.Log($"   Map ID: {mapId}");
            DebugLogger.Log($"   Folder Path: {mapEntry.FolderPath}");
            DebugLogger.Log($"   Image Path: {mapImageRelativePath}");

            // Try disk first (preferred for user-uploaded images)
            string diskPath = Path.Combine(_gameFolder, mapImageRelativePath.TrimStart('\\', '/'));
            DebugLogger.Log($"   Disk Path: {diskPath}");
            DebugLogger.Log($"   Disk Exists: {File.Exists(diskPath)}");

            // DEBUG: List folders to help diagnose encoding/path issues
            if (!File.Exists(diskPath))
            {
                try
                {
                    string mapsDir = Path.Combine(_gameFolder, "maps");
                    DebugLogger.Log($"   📁 Checking maps directory: {mapsDir}");
                    DebugLogger.Log($"   📁 Directory exists: {Directory.Exists(mapsDir)}");

                    if (Directory.Exists(mapsDir))
                    {
                        var folders = Directory.GetDirectories(mapsDir);
                        DebugLogger.Log($"   📁 Found {folders.Length} folders");

                        // Show first few folders to help debug
                        int showCount = Math.Min(10, folders.Length);
                        for (int i = 0; i < showCount; i++)
                        {
                            string folderName = Path.GetFileName(folders[i]);
                            DebugLogger.Log($"      [{i+1}] {folderName}");
                        }

                        // Check if expected folder exists
                        string expectedFolder = Path.Combine(mapsDir, mapEntry.FolderPath.Split('\\')[0]);
                        DebugLogger.Log($"   📁 Expected first folder: {mapEntry.FolderPath.Split('\\')[0]}");
                        DebugLogger.Log($"   📁 Expected folder exists: {Directory.Exists(expectedFolder)}");
                    }
                }
                catch (Exception ex)
                {
                    DebugLogger.Log($"   ⚠ Failed to list directories: {ex.Message}");
                }
            }

            try
            {
                // PRIORITY 1: Try disk first (user-uploaded files)
                if (File.Exists(diskPath))
                {
                    DebugLogger.Log($"✓ Loading image from DISK");
                    mapData.MapImageData = File.ReadAllBytes(diskPath);
                    mapData.MapImagePath = mapImageRelativePath;

                    // Calculate image offset based on region boundaries
                    // 24.jpg uses MAP coordinates (not logic coordinates!)
                    // Client scale: 1 region = 128x128 pixels on 24.jpg
                    mapData.MapImageOffsetX = config.RegionLeft * MapConstants.MAP_REGION_PIXEL_WIDTH;
                    mapData.MapImageOffsetY = config.RegionTop * MapConstants.MAP_REGION_PIXEL_HEIGHT;

                    DebugLogger.Log($"✓ Loaded map image from disk: {diskPath}");
                    DebugLogger.Log($"   Size: {mapData.MapImageData.Length:N0} bytes");
                    DebugLogger.Log($"   Offset: ({mapData.MapImageOffsetX}, {mapData.MapImageOffsetY}) pixels");
                }
                // PRIORITY 2: Try pak file
                else if (_pakReader != null && FileExists(mapImageRelativePath))
                {
                    DebugLogger.Log($"✓ Loading image from PAK");
                    mapData.MapImageData = ReadFileBytes(mapImageRelativePath);
                    if (mapData.MapImageData != null)
                    {
                        mapData.MapImagePath = mapImageRelativePath;
                        mapData.MapImageOffsetX = config.RegionLeft * MapConstants.MAP_REGION_PIXEL_WIDTH;
                        mapData.MapImageOffsetY = config.RegionTop * MapConstants.MAP_REGION_PIXEL_HEIGHT;

                        DebugLogger.Log($"✓ Loaded map image from pak: {mapImageRelativePath}");
                        DebugLogger.Log($"   Size: {mapData.MapImageData.Length:N0} bytes");
                        DebugLogger.Log($"   Offset: ({mapData.MapImageOffsetX}, {mapData.MapImageOffsetY}) pixels");
                    }
                    else
                    {
                        DebugLogger.Log($"⚠ Image data is null after reading from pak");
                    }
                }
                else
                {
                    DebugLogger.Log($"ℹ️  No map image found");
                    DebugLogger.Log($"   This is normal - not all maps have 24.jpg files");
                    DebugLogger.Log($"   Tool will render using obstacle data instead");
                }
            }
            catch (Exception ex)
            {
                DebugLogger.Log($"⚠ Failed to load map image: {ex.Message}");
                DebugLogger.Log($"   Stack trace: {ex.StackTrace}");
            }

            DebugLogger.LogSeparator();

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

        // Map image (24.jpg file)
        public byte[] MapImageData { get; set; }
        public string MapImagePath { get; set; }
        public int MapImageOffsetX { get; set; }
        public int MapImageOffsetY { get; set; }

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
