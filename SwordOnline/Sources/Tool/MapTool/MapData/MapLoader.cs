using System;
using System.IO;
using System.Text;
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
        /// Try to match two strings considering GB2312 encoding issues
        /// </summary>
        private bool TryMatchGB2312String(string actual, string expected)
        {
            // Strategy 1: Direct Unicode match
            if (string.Equals(actual, expected, StringComparison.OrdinalIgnoreCase))
            {
                return true;
            }

            // Strategy 2: GB2312 re-decode (Default→GB2312)
            try
            {
                byte[] nameBytes = Encoding.Default.GetBytes(actual);
                string actualGB2312 = Encoding.GetEncoding("GB2312").GetString(nameBytes);
                if (string.Equals(actualGB2312, expected, StringComparison.OrdinalIgnoreCase))
                {
                    return true;
                }
            }
            catch { }

            // Strategy 3: GB2312 re-decode (Latin-1→GB2312)
            try
            {
                byte[] nameBytes = Encoding.GetEncoding("ISO-8859-1").GetBytes(actual);
                string actualGB2312 = Encoding.GetEncoding("GB2312").GetString(nameBytes);
                if (string.Equals(actualGB2312, expected, StringComparison.OrdinalIgnoreCase))
                {
                    return true;
                }
            }
            catch { }

            // Strategy 4: Expected→GB2312→Latin-1
            try
            {
                byte[] expectedBytes = Encoding.GetEncoding("GB2312").GetBytes(expected);
                string expectedAsLatin1 = Encoding.GetEncoding("ISO-8859-1").GetString(expectedBytes);
                if (string.Equals(actual, expectedAsLatin1, StringComparison.OrdinalIgnoreCase))
                {
                    return true;
                }
            }
            catch { }

            return false;
        }

        /// <summary>
        /// Find file on disk using GB2312-aware filesystem enumeration
        /// Handles Chinese folder/file names that use GB2312 encoding
        /// </summary>
        private string FindFileWithGB2312Encoding(string relativePath)
        {
            try
            {
                string fullRelativePath = relativePath.TrimStart('\\', '/');
                string expectedFileName = Path.GetFileName(fullRelativePath);
                string expectedDirPath = Path.GetDirectoryName(fullRelativePath);

                // Split directory path into parts and navigate step by step
                string[] pathParts = expectedDirPath.Split(new[] { '\\', '/' }, StringSplitOptions.RemoveEmptyEntries);
                string currentPath = _gameFolder;

                // Navigate through directories
                for (int i = 0; i < pathParts.Length; i++)
                {
                    string expectedName = pathParts[i];
                    string foundPath = null;

                    if (!Directory.Exists(currentPath))
                        return null;

                    var subdirs = Directory.GetDirectories(currentPath);
                    foreach (var subdir in subdirs)
                    {
                        string actualName = Path.GetFileName(subdir);
                        if (TryMatchGB2312String(actualName, expectedName))
                        {
                            foundPath = subdir;
                            break;
                        }
                    }

                    if (foundPath != null)
                    {
                        currentPath = foundPath;
                    }
                    else
                    {
                        return null; // Directory not found
                    }
                }

                // Look for the file in the final directory
                if (!Directory.Exists(currentPath))
                    return null;

                var files = Directory.GetFiles(currentPath);
                foreach (var file in files)
                {
                    string actualFileName = Path.GetFileName(file);
                    if (TryMatchGB2312String(actualFileName, expectedFileName))
                    {
                        return file;
                    }
                }
            }
            catch
            {
                // Ignore errors in GB2312 enumeration
            }

            return null; // File not found
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

            // Try direct disk path first (for non-GB2312 paths)
            string diskPath = Path.Combine(_gameFolder, relativePath.TrimStart('\\', '/'));
            if (File.Exists(diskPath))
            {
                return File.ReadAllBytes(diskPath);
            }

            // Fallback: Try GB2312-aware filesystem enumeration
            string actualPath = FindFileWithGB2312Encoding(relativePath);
            if (actualPath != null && File.Exists(actualPath))
            {
                return File.ReadAllBytes(actualPath);
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
            DebugLogger.Log($"   Disk Path (Unicode): {diskPath}");
            DebugLogger.Log($"   Disk Exists (direct): {File.Exists(diskPath)}");

            // FALLBACK: Try to find the file using actual filesystem enumeration
            // This handles GB2312 encoding issues on Windows
            string actualDiskPath = null;
            if (!File.Exists(diskPath))
            {
                try
                {
                    DebugLogger.Log($"   🔍 Searching with filesystem enumeration (handles GB2312 encoding)...");

                    // The image path is: \maps\{FolderPath}24.jpg
                    // Example: \maps\西南北区\成都\成都24.jpg
                    // We need to:
                    // 1. Navigate through folder structure: maps\西南北区\成都\
                    // 2. Find file: 成都24.jpg

                    // Build the full relative path and split it into directory + filename
                    string fullRelativePath = mapImageRelativePath.TrimStart('\\', '/');
                    string expectedFileName = Path.GetFileName(fullRelativePath); // "成都24.jpg"
                    string expectedDirPath = Path.GetDirectoryName(fullRelativePath); // "maps\西南北区\成都"

                    DebugLogger.Log($"      Expected directory: {expectedDirPath}");
                    DebugLogger.Log($"      Expected filename: {expectedFileName}");

                    // Split directory path into parts and navigate step by step
                    string[] pathParts = expectedDirPath.Split(new[] { '\\', '/' }, StringSplitOptions.RemoveEmptyEntries);
                    string currentPath = _gameFolder;

                    bool pathExists = true;
                    for (int i = 0; i < pathParts.Length; i++)
                    {
                        string expectedName = pathParts[i];
                        string foundPath = null;

                        // Enumerate subdirectories and find matching one (case-insensitive, encoding-tolerant)
                        var subdirs = Directory.GetDirectories(currentPath);
                        foreach (var subdir in subdirs)
                        {
                            string actualName = Path.GetFileName(subdir);

                            // Try multiple matching strategies to handle GB2312 encoding issues
                            bool isMatch = false;

                            // Strategy 1: Direct Unicode match
                            if (string.Equals(actualName, expectedName, StringComparison.OrdinalIgnoreCase))
                            {
                                isMatch = true;
                                DebugLogger.Log($"      🔍 Match strategy: Direct Unicode");
                            }

                            // Strategy 2: GB2312 encoding conversion
                            // If folder name was created with GB2312 bytes, .NET might misinterpret it
                            // Try converting: actualName (wrong encoding) → bytes → GB2312 decode → compare
                            if (!isMatch)
                            {
                                try
                                {
                                    // The folder name might be GB2312 bytes interpreted as Latin-1/Default
                                    // Try to get the raw bytes and re-decode as GB2312
                                    byte[] nameBytes = Encoding.Default.GetBytes(actualName);
                                    string actualNameGB2312 = Encoding.GetEncoding("GB2312").GetString(nameBytes);

                                    if (string.Equals(actualNameGB2312, expectedName, StringComparison.OrdinalIgnoreCase))
                                    {
                                        isMatch = true;
                                        DebugLogger.Log($"      🔍 Match strategy: GB2312 re-decode (Default→GB2312)");
                                        DebugLogger.Log($"         Original: '{actualName}' → Decoded: '{actualNameGB2312}'");
                                    }
                                }
                                catch { /* Ignore encoding errors */ }
                            }

                            // Strategy 3: Try Latin-1 to GB2312 conversion
                            if (!isMatch)
                            {
                                try
                                {
                                    // Folder name might be GB2312 bytes interpreted as Latin-1
                                    byte[] nameBytes = Encoding.GetEncoding("ISO-8859-1").GetBytes(actualName);
                                    string actualNameGB2312 = Encoding.GetEncoding("GB2312").GetString(nameBytes);

                                    if (string.Equals(actualNameGB2312, expectedName, StringComparison.OrdinalIgnoreCase))
                                    {
                                        isMatch = true;
                                        DebugLogger.Log($"      🔍 Match strategy: GB2312 re-decode (Latin-1→GB2312)");
                                        DebugLogger.Log($"         Original: '{actualName}' → Decoded: '{actualNameGB2312}'");
                                    }
                                }
                                catch { /* Ignore encoding errors */ }
                            }

                            // Strategy 4: Convert expected name to GB2312 and back (in case it was stored incorrectly)
                            if (!isMatch)
                            {
                                try
                                {
                                    byte[] expectedBytes = Encoding.GetEncoding("GB2312").GetBytes(expectedName);
                                    string expectedAsLatin1 = Encoding.GetEncoding("ISO-8859-1").GetString(expectedBytes);

                                    if (string.Equals(actualName, expectedAsLatin1, StringComparison.OrdinalIgnoreCase))
                                    {
                                        isMatch = true;
                                        DebugLogger.Log($"      🔍 Match strategy: Expected→GB2312→Latin-1");
                                        DebugLogger.Log($"         Expected: '{expectedName}' → '{expectedAsLatin1}' = Actual: '{actualName}'");
                                    }
                                }
                                catch { /* Ignore encoding errors */ }
                            }

                            if (isMatch)
                            {
                                foundPath = subdir;
                                break;
                            }
                        }

                        if (foundPath != null)
                        {
                            currentPath = foundPath;
                            DebugLogger.Log($"      ✓ Found: {pathParts[i]} → {Path.GetFileName(foundPath)}");
                        }
                        else
                        {
                            DebugLogger.Log($"      ✗ Not found: {pathParts[i]}");
                            DebugLogger.Log($"      Available folders:");
                            int showCount = Math.Min(5, subdirs.Length);
                            for (int j = 0; j < showCount; j++)
                            {
                                DebugLogger.Log($"        [{j+1}] {Path.GetFileName(subdirs[j])}");
                            }
                            pathExists = false;
                            break;
                        }
                    }

                    // Now look for the actual file in the final directory
                    if (pathExists)
                    {
                        DebugLogger.Log($"      📁 Navigated to: {currentPath}");
                        DebugLogger.Log($"      🔍 Looking for file: {expectedFileName}");

                        // Try to find the file using enumeration (handles GB2312 filenames)
                        var files = Directory.GetFiles(currentPath);
                        foreach (var file in files)
                        {
                            string actualFileName = Path.GetFileName(file);
                            bool fileMatch = false;

                            // Try the same encoding strategies for filename
                            if (string.Equals(actualFileName, expectedFileName, StringComparison.OrdinalIgnoreCase))
                            {
                                fileMatch = true;
                                DebugLogger.Log($"      ✓ File match: Direct Unicode");
                            }
                            else
                            {
                                // Try GB2312 conversion
                                try
                                {
                                    byte[] fileNameBytes = Encoding.Default.GetBytes(actualFileName);
                                    string actualFileNameGB2312 = Encoding.GetEncoding("GB2312").GetString(fileNameBytes);

                                    if (string.Equals(actualFileNameGB2312, expectedFileName, StringComparison.OrdinalIgnoreCase))
                                    {
                                        fileMatch = true;
                                        DebugLogger.Log($"      ✓ File match: GB2312 re-decode");
                                        DebugLogger.Log($"         Original: '{actualFileName}' → Decoded: '{actualFileNameGB2312}'");
                                    }
                                }
                                catch { }
                            }

                            if (fileMatch)
                            {
                                actualDiskPath = file;
                                DebugLogger.Log($"   ✓ Found image via enumeration: {actualDiskPath}");
                                break;
                            }
                        }

                        if (actualDiskPath == null)
                        {
                            DebugLogger.Log($"   ✗ File not found in directory: {currentPath}");
                            DebugLogger.Log($"      Available files:");
                            int showCount = Math.Min(5, files.Length);
                            for (int j = 0; j < showCount; j++)
                            {
                                DebugLogger.Log($"        [{j+1}] {Path.GetFileName(files[j])}");
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    DebugLogger.Log($"   ⚠ Failed during filesystem enumeration: {ex.Message}");
                }
            }

            try
            {
                // PRIORITY 1: Try direct disk path (Unicode)
                if (File.Exists(diskPath))
                {
                    DebugLogger.Log($"✓ Loading image from DISK (direct Unicode path)");
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
                // PRIORITY 2: Try enumerated path (handles GB2312 encoding)
                else if (actualDiskPath != null && File.Exists(actualDiskPath))
                {
                    DebugLogger.Log($"✓ Loading image from DISK (via filesystem enumeration)");
                    mapData.MapImageData = File.ReadAllBytes(actualDiskPath);
                    mapData.MapImagePath = mapImageRelativePath;

                    mapData.MapImageOffsetX = config.RegionLeft * MapConstants.MAP_REGION_PIXEL_WIDTH;
                    mapData.MapImageOffsetY = config.RegionTop * MapConstants.MAP_REGION_PIXEL_HEIGHT;

                    DebugLogger.Log($"✓ Loaded map image from disk (enumerated): {actualDiskPath}");
                    DebugLogger.Log($"   Size: {mapData.MapImageData.Length:N0} bytes");
                    DebugLogger.Log($"   Offset: ({mapData.MapImageOffsetX}, {mapData.MapImageOffsetY}) pixels");
                }
                // PRIORITY 3: Try pak file
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
