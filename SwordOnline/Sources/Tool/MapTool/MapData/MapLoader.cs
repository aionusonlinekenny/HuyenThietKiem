using System;
using System.IO;
using System.Collections.Generic;

namespace MapTool.MapData
{
    /// <summary>
    /// Auto-loads complete map data from game folder
    /// Workflow: GameFolder + MapID → Auto load all regions
    /// </summary>
    public class MapLoader
    {
        private string _gameFolder;
        private bool _isServerMode;
        private MapListParser _mapListParser;

        public MapLoader(string gameFolder, bool isServerMode = true)
        {
            _gameFolder = gameFolder;
            _isServerMode = isServerMode;
            _mapListParser = new MapListParser(gameFolder);
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

            string mapFolderPath = _mapListParser.GetMapFolderPath(mapId);
            string regionSuffix = _isServerMode ? "Region_S.dat" : "Region_C.dat";

            int loadedCount = 0;
            for (int y = config.RegionTop; y <= config.RegionBottom; y++)
            {
                for (int x = config.RegionLeft; x <= config.RegionRight; x++)
                {
                    // Build region file path: <mapFolder>\v_YYY\XXX_Region_S.dat
                    string regionPath = Path.Combine(
                        mapFolderPath,
                        $"v_{y:D3}",
                        $"{x:D3}_{regionSuffix}"
                    );

                    if (File.Exists(regionPath))
                    {
                        try
                        {
                            RegionData regionData = MapFileParser.LoadRegionData(regionPath, x, y);
                            mapData.Regions[regionData.RegionID] = regionData;
                            loadedCount++;
                        }
                        catch (Exception ex)
                        {
                            // Log error but continue loading other regions
                            Console.WriteLine($"Failed to load region ({x},{y}): {ex.Message}");
                        }
                    }
                }
            }

            mapData.LoadedRegionCount = loadedCount;
            return mapData;
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
