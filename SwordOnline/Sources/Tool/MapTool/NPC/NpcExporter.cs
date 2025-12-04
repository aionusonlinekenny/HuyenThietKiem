using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace MapTool.NPC
{
    /// <summary>
    /// Export NPC data to file format compatible with game server
    /// Format: NpcID | MapID | PosX | PosY | ScriptFile | Name | Level | IsLoad
    /// </summary>
    public class NpcExporter
    {
        private List<NpcEntry> _entries;

        public NpcExporter()
        {
            _entries = new List<NpcEntry>();
        }

        /// <summary>
        /// Add NPC entry
        /// </summary>
        public void AddEntry(NpcEntry entry)
        {
            _entries.Add(entry);
        }

        /// <summary>
        /// Add entry from parameters
        /// </summary>
        public void AddEntry(int npcId, int mapId, int posX, int posY, string scriptFile, string name, int level = 1, int isLoad = 1)
        {
            NpcEntry entry = new NpcEntry
            {
                NpcID = npcId,
                MapID = mapId,
                PosX = posX,
                PosY = posY,
                ScriptFile = scriptFile ?? "",
                Name = name ?? "",
                Level = level,
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
        public List<NpcEntry> GetEntries()
        {
            return new List<NpcEntry>(_entries);
        }

        /// <summary>
        /// Get entries for specific map
        /// </summary>
        public List<NpcEntry> GetEntriesForMap(int mapId)
        {
            return _entries.FindAll(e => e.MapID == mapId);
        }

        /// <summary>
        /// Remove entry at index
        /// </summary>
        public bool RemoveAt(int index)
        {
            if (index >= 0 && index < _entries.Count)
            {
                _entries.RemoveAt(index);
                return true;
            }
            return false;
        }

        /// <summary>
        /// Update entry at index
        /// </summary>
        public bool UpdateEntry(int index, NpcEntry entry)
        {
            if (index >= 0 && index < _entries.Count)
            {
                _entries[index] = entry;
                return true;
            }
            return false;
        }

        /// <summary>
        /// Export to file
        /// </summary>
        public void ExportToFile(string filePath)
        {
            // Use Windows-1252 (ANSI) encoding for Vietnamese TCVN3 characters
            Encoding encoding = Encoding.GetEncoding("Windows-1252");
            using (StreamWriter writer = new StreamWriter(filePath, false, encoding))
            {
                // Write header
                writer.WriteLine("NpcID\tMapID\tPosX\tPosY\tScriptFile\tName\tLevel\tIsLoad");

                // Write entries
                foreach (var entry in _entries)
                {
                    writer.WriteLine(entry.ToString());
                }
            }
        }

        /// <summary>
        /// Import from file (auto-detects Normal/Dialoger/Object format)
        /// </summary>
        public void ImportFromFile(string filePath)
        {
            if (!File.Exists(filePath))
            {
                throw new FileNotFoundException($"File not found: {filePath}");
            }

            _entries.Clear();
            // Use Windows-1252 (ANSI) encoding for Vietnamese TCVN3 characters
            Encoding encoding = Encoding.GetEncoding("Windows-1252");
            string[] lines = File.ReadAllLines(filePath, encoding);

            if (lines.Length < 2)
                return;

            // Detect format from header
            string header = lines[0].ToLower();
            bool isObjectFormat = header.Contains("objid") && header.Contains("dir") && header.Contains("state");

            for (int i = 1; i < lines.Length; i++) // Skip header
            {
                string line = lines[i].Trim();
                if (string.IsNullOrEmpty(line))
                    continue;

                string[] parts = line.Split('\t');

                if (isObjectFormat)
                {
                    // Object format: ObjID MapID PosX PosY Dir State ScriptFile IsLoad (8 fields)
                    if (parts.Length >= 8)
                    {
                        ObjectEntry objEntry = new ObjectEntry
                        {
                            ObjID = int.Parse(parts[0]),
                            MapID = int.Parse(parts[1]),
                            PosX = int.Parse(parts[2]),
                            PosY = int.Parse(parts[3]),
                            Dir = int.Parse(parts[4]),
                            State = int.Parse(parts[5]),
                            ScriptFile = parts[6],
                            IsLoad = int.Parse(parts[7])
                        };

                        // Convert to NpcEntry for display
                        _entries.Add(objEntry.ToNpcEntry());
                    }
                }
                else
                {
                    // NPC format: Support two variants
                    // Normal NPC: NpcID MapID PosX PosY ScriptFile Name Level IsLoad (8 fields)
                    // Dialoger: NpcID MapID PosX PosY ScriptFile Name IsLoad (7 fields, no Level)
                    if (parts.Length >= 7)
                    {
                        NpcEntry entry = new NpcEntry
                        {
                            NpcID = int.Parse(parts[0]),
                            MapID = int.Parse(parts[1]),
                            PosX = int.Parse(parts[2]),
                            PosY = int.Parse(parts[3]),
                            ScriptFile = parts[4],
                            Name = parts[5],
                            Level = parts.Length >= 8 ? int.Parse(parts[6]) : 1,  // Default level 1 for Dialoger
                            IsLoad = int.Parse(parts[parts.Length - 1])  // Last field is always IsLoad
                        };

                        _entries.Add(entry);
                    }
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

            Dictionary<int, int> mapCounts = new Dictionary<int, int>();
            Dictionary<int, int> npcCounts = new Dictionary<int, int>();

            foreach (var entry in _entries)
            {
                if (!mapCounts.ContainsKey(entry.MapID))
                    mapCounts[entry.MapID] = 0;
                mapCounts[entry.MapID]++;

                if (!npcCounts.ContainsKey(entry.NpcID))
                    npcCounts[entry.NpcID] = 0;
                npcCounts[entry.NpcID]++;
            }

            StringBuilder sb = new StringBuilder();
            sb.AppendLine($"Total Entries: {_entries.Count}");
            sb.AppendLine($"Unique Maps: {mapCounts.Count}");
            sb.AppendLine($"Unique NPC IDs: {npcCounts.Count}");

            return sb.ToString();
        }
    }
}
