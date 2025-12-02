using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using MapTool.MapData;

namespace MapTool.Export
{
    /// <summary>
    /// Export trap/object data to file format compatible with game server
    /// </summary>
    public class TrapExporter
    {
        private List<TrapEntry> _entries;

        public TrapExporter()
        {
            _entries = new List<TrapEntry>();
        }

        /// <summary>
        /// Add a trap entry
        /// </summary>
        public void AddEntry(TrapEntry entry)
        {
            _entries.Add(entry);
        }

        /// <summary>
        /// Add entry from coordinates with map config for local RegionID calculation
        /// </summary>
        public void AddEntry(int mapId, MapCoordinate coord, MapConfig mapConfig, string scriptFile = null, int isLoad = 1)
        {
            // Calculate LOCAL RegionID (relative to map rect) for trap file format
            int localRegionID = RegionData.MakeLocalRegionID(
                coord.RegionX, coord.RegionY,
                mapConfig.RegionLeft, mapConfig.RegionTop, mapConfig.RegionWidth);

            TrapEntry entry = new TrapEntry
            {
                MapId = mapId,
                RegionId = localRegionID,  // Use LOCAL RegionID for trap files
                CellX = coord.CellX,
                CellY = coord.CellY,
                ScriptFile = scriptFile ?? $@"\script\maps\trap\{mapId}\1.lua",
                IsLoad = isLoad
            };

            _entries.Add(entry);
        }

        /// <summary>
        /// Add entry from coordinates (legacy - for backwards compatibility)
        /// </summary>
        [Obsolete("Use AddEntry with MapConfig parameter to ensure correct local RegionID")]
        public void AddEntry(int mapId, MapCoordinate coord, string scriptFile = null, int isLoad = 1)
        {
            TrapEntry entry = new TrapEntry
            {
                MapId = mapId,
                RegionId = coord.RegionID,  // WARNING: This is GLOBAL RegionID, may be wrong!
                CellX = coord.CellX,
                CellY = coord.CellY,
                ScriptFile = scriptFile ?? $@"\script\maps\trap\{mapId}\1.lua",
                IsLoad = isLoad
            };

            _entries.Add(entry);
        }

        /// <summary>
        /// Clear all entries
        /// </summary>
        public void Clear()
        {
            _entries.Clear();
        }

        /// <summary>
        /// Remove last entry
        /// </summary>
        public bool RemoveLast()
        {
            if (_entries.Count > 0)
            {
                _entries.RemoveAt(_entries.Count - 1);
                return true;
            }
            return false;
        }

        /// <summary>
        /// Get all entries
        /// </summary>
        public List<TrapEntry> GetEntries()
        {
            return new List<TrapEntry>(_entries);
        }

        /// <summary>
        /// Export to file (defaults to Trap format)
        /// </summary>
        public void ExportToFile(string filePath)
        {
            ExportToTrapFile(filePath);
        }

        /// <summary>
        /// Export to Trap format file (tab-separated)
        /// </summary>
        public void ExportToTrapFile(string filePath)
        {
            using (StreamWriter writer = new StreamWriter(filePath, false, Encoding.GetEncoding("Windows-1252")))
            {
                // Write header
                writer.WriteLine("MapId\tRegionId\tCellX\tCellY\tScriptFile\tIsLoad");

                // Write entries
                foreach (var entry in _entries)
                {
                    writer.WriteLine(entry.ToString());
                }
            }
        }

        /// <summary>
        /// Export to Object format file
        /// </summary>
        public void ExportToObjectFile(string filePath, int objId = 1, int direction = 0, int state = 0)
        {
            using (StreamWriter writer = new StreamWriter(filePath, false, Encoding.GetEncoding("Windows-1252")))
            {
                // Write header
                writer.WriteLine("ObjID\tMapID\tPosX\tPosY\tDir\tState\tScriptFile\tIsLoad");

                // Write entries
                foreach (var entry in _entries)
                {
                    // Convert region/cell to world coordinates
                    RegionData.ParseRegionID(entry.RegionId, out int regionX, out int regionY);
                    CoordinateConverter.RegionCellToWorld(regionX, regionY, entry.CellX, entry.CellY,
                                                         out int worldX, out int worldY);

                    writer.WriteLine($"{objId}\t{entry.MapId}\t{worldX}\t{worldY}\t{direction}\t{state}\t{entry.ScriptFile}\t{entry.IsLoad}");
                }
            }
        }

        /// <summary>
        /// Import from Trap file
        /// </summary>
        public void ImportFromTrapFile(string filePath)
        {
            if (!File.Exists(filePath))
            {
                throw new FileNotFoundException($"File not found: {filePath}");
            }

            _entries.Clear();
            string[] lines = File.ReadAllLines(filePath, Encoding.GetEncoding("Windows-1252"));

            for (int i = 1; i < lines.Length; i++) // Skip header
            {
                string line = lines[i].Trim();
                if (string.IsNullOrEmpty(line))
                    continue;

                string[] parts = line.Split('\t');
                if (parts.Length >= 6)
                {
                    TrapEntry entry = new TrapEntry
                    {
                        MapId = int.Parse(parts[0]),
                        RegionId = int.Parse(parts[1]),
                        CellX = int.Parse(parts[2]),
                        CellY = int.Parse(parts[3]),
                        ScriptFile = parts[4],
                        IsLoad = int.Parse(parts[5])
                    };

                    _entries.Add(entry);
                }
            }
        }

        /// <summary>
        /// Get statistics about entries
        /// </summary>
        public string GetStatistics()
        {
            if (_entries.Count == 0)
                return "No entries";

            Dictionary<int, int> regionCounts = new Dictionary<int, int>();
            HashSet<string> scripts = new HashSet<string>();

            foreach (var entry in _entries)
            {
                if (!regionCounts.ContainsKey(entry.RegionId))
                    regionCounts[entry.RegionId] = 0;
                regionCounts[entry.RegionId]++;

                scripts.Add(entry.ScriptFile);
            }

            StringBuilder sb = new StringBuilder();
            sb.AppendLine($"Total Entries: {_entries.Count}");
            sb.AppendLine($"Unique Regions: {regionCounts.Count}");
            sb.AppendLine($"Unique Scripts: {scripts.Count}");

            return sb.ToString();
        }

        /// <summary>
        /// Generate Lua script template
        /// </summary>
        public string GenerateLuaScript(int mapId)
        {
            StringBuilder sb = new StringBuilder();
            sb.AppendLine(@"Include(""\script\maps\libtrap.lua"");");
            sb.AppendLine();
            sb.AppendLine("function main()");
            sb.AppendLine($"    local nMapId = {mapId};");
            sb.AppendLine("    local nLevel = GetLevel();");
            sb.AppendLine();
            sb.AppendLine("    -- Teleport to another location");
            sb.AppendLine("    -- SetPos(worldX, worldY);");
            sb.AppendLine();
            sb.AppendLine("    -- Add skill/buff for 3 seconds");
            sb.AppendLine("    -- AddSkillTrap();");
            sb.AppendLine();
            sb.AppendLine("    -- Check fight state");
            sb.AppendLine("    -- CheckFightState(1);");
            sb.AppendLine("end;");

            return sb.ToString();
        }
    }
}
