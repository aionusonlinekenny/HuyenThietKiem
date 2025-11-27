using System;
using System.IO;
using System.Text;
using System.Collections.Generic;

namespace MapTool.MapData
{
    /// <summary>
    /// Parser for MapList.ini - maps Map ID to folder paths
    /// Based on KSubWorld::LoadMap reading from \settings\MapList.ini
    /// </summary>
    public class MapListParser
    {
        private Dictionary<int, MapListEntry> _mapEntries;
        private string _gameFolder;

        public MapListParser(string gameFolder)
        {
            _gameFolder = gameFolder;
            _mapEntries = new Dictionary<int, MapListEntry>();
        }

        /// <summary>
        /// Load MapList.ini from game folder
        /// </summary>
        public void Load()
        {
            string mapListPath = Path.Combine(_gameFolder, "Settings", "MapList.ini");

            if (!File.Exists(mapListPath))
            {
                throw new FileNotFoundException($"MapList.ini not found: {mapListPath}");
            }

            _mapEntries.Clear();

            // Read with GB2312 encoding (Chinese characters)
            string[] lines = File.ReadAllLines(mapListPath, Encoding.GetEncoding("GB2312"));
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

                // Only process [List] section
                if (currentSection != "List")
                    continue;

                // Key=Value pair
                string[] parts = trimmed.Split(new[] { '=' }, 2);
                if (parts.Length != 2)
                    continue;

                string key = parts[0].Trim();
                string value = parts[1].Trim();

                // Parse map entries
                // Format examples:
                // 11=场景地图\城市\成都
                // 11_name=Thành Đô
                // 11_MapType=City

                if (int.TryParse(key, out int mapId))
                {
                    // Main map folder entry
                    if (!_mapEntries.ContainsKey(mapId))
                    {
                        _mapEntries[mapId] = new MapListEntry { MapId = mapId };
                    }
                    _mapEntries[mapId].FolderPath = value;
                }
                else if (key.EndsWith("_name"))
                {
                    // Map name
                    string mapIdStr = key.Substring(0, key.Length - 5);
                    if (int.TryParse(mapIdStr, out mapId))
                    {
                        if (!_mapEntries.ContainsKey(mapId))
                        {
                            _mapEntries[mapId] = new MapListEntry { MapId = mapId };
                        }
                        _mapEntries[mapId].Name = value;
                    }
                }
                else if (key.EndsWith("_MapType"))
                {
                    // Map type
                    string mapIdStr = key.Substring(0, key.Length - 8);
                    if (int.TryParse(mapIdStr, out mapId))
                    {
                        if (!_mapEntries.ContainsKey(mapId))
                        {
                            _mapEntries[mapId] = new MapListEntry { MapId = mapId };
                        }
                        _mapEntries[mapId].MapType = value;
                    }
                }
            }
        }

        /// <summary>
        /// Get map entry by ID
        /// </summary>
        public MapListEntry GetMapEntry(int mapId)
        {
            if (_mapEntries.ContainsKey(mapId))
            {
                return _mapEntries[mapId];
            }
            return null;
        }

        /// <summary>
        /// Get all map IDs
        /// </summary>
        public List<int> GetAllMapIds()
        {
            List<int> ids = new List<int>(_mapEntries.Keys);
            ids.Sort();
            return ids;
        }

        /// <summary>
                /// Get full path to map .wor file (for disk access)
        /// </summary>
        public string GetMapWorPath(int mapId)
        {
            var entry = GetMapEntry(mapId);
            if (entry == null || string.IsNullOrEmpty(entry.FolderPath))
            {
                return null;
            }
 
            // Extract map name from folder path
            // Example: "场景地图\城市\成都" → "成都"
            string[] parts = entry.FolderPath.Split('\\', '/');
            string mapName = parts[parts.Length - 1];
 
            // Build full path: <gameFolder>\maps\<folderPath>\<mapName>.wor
            string worPath = Path.Combine(_gameFolder, "maps", entry.FolderPath, mapName + ".wor");
            return worPath;
        }
 
        /// <summary>
        /// Get relative path to map .wor file (for pak access)
        /// </summary>
        public string GetMapWorRelativePath(int mapId)
        {
            var entry = GetMapEntry(mapId);
            if (entry == null || string.IsNullOrEmpty(entry.FolderPath))
            {
                return null;
            }
 
            // Extract map name from folder path
            // Example: "场景地图\城市\成都" → "成都"
            string[] parts = entry.FolderPath.Split('\\', '/');
            string mapName = parts[parts.Length - 1];
 
            // Build relative path: \maps\<folderPath>\<mapName>.wor
            return $"\\maps\\{entry.FolderPath}\\{mapName}.wor";
        
        }

        /// <summary>
        /// Get map folder path for loading regions
        /// </summary>
        public string GetMapFolderPath(int mapId)
        {
            var entry = GetMapEntry(mapId);
            if (entry == null || string.IsNullOrEmpty(entry.FolderPath))
            {
                return null;
            }

            return Path.Combine(_gameFolder, "maps", entry.FolderPath);
        }
    }

    /// <summary>
    /// Map entry from MapList.ini
    /// </summary>
    public class MapListEntry
    {
        public int MapId { get; set; }
        public string FolderPath { get; set; }  // e.g. "场景地图\城市\成都"
        public string Name { get; set; }         // e.g. "Thành Đô"
        public string MapType { get; set; }      // e.g. "City", "Field", "Cave"

        public override string ToString()
        {
            return $"[{MapId}] {Name ?? FolderPath} ({MapType})";
        }
    }
}
