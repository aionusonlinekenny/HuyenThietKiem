using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace MapTool.NPC
{
    /// <summary>
    /// Maps NPC ID to NPC resource type using Npcs.txt from server
    /// Format: Line number = NPC ID, Column 12 = NpcResType (e.g., "enemy003", "ani001")
    /// </summary>
    public class NpcIdMapper
    {
        private Dictionary<int, NpcInfo> _npcDatabase;

        public class NpcInfo
        {
            public int NpcId { get; set; }
            public string NpcName { get; set; }
            public string NpcResType { get; set; }  // Column 12 - e.g., "enemy003"
            public string Kind { get; set; }
            public string Camp { get; set; }
            public string Series { get; set; }
        }

        public NpcIdMapper()
        {
            _npcDatabase = new Dictionary<int, NpcInfo>();
        }

        /// <summary>
        /// Load Npcs.txt from server Settings folder
        /// </summary>
        public void LoadNpcDatabase(string serverPath)
        {
            string npcsFile = Path.Combine(serverPath, "Settings", "Npcs.txt");
            DebugLogger.Log($"         Loading NPC database from: {npcsFile}");

            if (!File.Exists(npcsFile))
            {
                DebugLogger.Log($"         ✗ ERROR: Npcs.txt not found at: {npcsFile}");
                throw new FileNotFoundException($"Npcs.txt not found at: {npcsFile}");
            }

            _npcDatabase.Clear();

            // Read with Windows-1252 encoding (ANSI - compatible with TCVN3 Vietnamese)
            Encoding encoding = Encoding.GetEncoding("Windows-1252");
            string[] lines = File.ReadAllLines(npcsFile, encoding);

            DebugLogger.Log($"         Total lines in file: {lines.Length}");

            // Line 1 is header, Line 2+ are NPC data
            // lines[0] = line 1 (header)
            // lines[1] = line 2 → NPC ID 1
            // lines[2] = line 3 → NPC ID 2
            // lines[49] = line 50 → NPC ID 49
            int loadedCount = 0;
            for (int i = 1; i < lines.Length; i++)
            {
                string line = lines[i].Trim();
                if (string.IsNullOrEmpty(line))
                    continue;

                string[] columns = line.Split('\t');
                if (columns.Length < 12)
                    continue;  // Need at least 12 columns for NpcResType

                int npcId = i; // lines[1] = line 2 = ID 1, lines[49] = line 50 = ID 49

                NpcInfo info = new NpcInfo
                {
                    NpcId = npcId,
                    NpcName = columns[0].Trim(),  // Column 1 (0-indexed = 0) is Name
                    Kind = columns.Length > 1 ? columns[1] : "",
                    Camp = columns.Length > 2 ? columns[2] : "",
                    Series = columns.Length > 3 ? columns[3] : "",
                    NpcResType = columns[11].Trim()  // Column 12 (0-indexed = 11)
                };

                _npcDatabase[npcId] = info;
                loadedCount++;

                // Log first 5 and last 5 NPCs for verification
                if (loadedCount <= 5 || i >= lines.Length - 5)
                {
                    DebugLogger.Log($"         [Line {i + 1}] ID={npcId}, Name='{info.NpcName}', ResType='{info.NpcResType}'");
                }
                else if (loadedCount == 6)
                {
                    DebugLogger.Log($"         ... ({lines.Length - 10} more NPCs) ...");
                }
            }

            DebugLogger.Log($"         ✓ Loaded {_npcDatabase.Count} NPCs from Npcs.txt");
        }

        /// <summary>
        /// Get NPC info by ID
        /// </summary>
        public NpcInfo GetNpcInfo(int npcId)
        {
            if (_npcDatabase.TryGetValue(npcId, out NpcInfo info))
            {
                return info;
            }
            return null;
        }

        /// <summary>
        /// Get NpcResType by ID (this is what we need to load SPR files)
        /// </summary>
        public string GetNpcResType(int npcId)
        {
            DebugLogger.Log($"            → GetNpcResType: Looking up ID {npcId} in database");
            NpcInfo info = GetNpcInfo(npcId);
            if (info == null)
            {
                DebugLogger.Log($"               ✗ ERROR: NPC ID {npcId} not found in database");
                DebugLogger.Log($"               Database contains {_npcDatabase.Count} NPCs");
                if (_npcDatabase.Count > 0)
                {
                    // Show a few sample IDs for debugging
                    int minId = int.MaxValue;
                    int maxId = int.MinValue;
                    foreach (int id in _npcDatabase.Keys)
                    {
                        if (id < minId) minId = id;
                        if (id > maxId) maxId = id;
                    }
                    DebugLogger.Log($"               ID range: {minId} to {maxId}");
                }
                return null;
            }
            DebugLogger.Log($"               ✓ Found: ID={info.NpcId}, Name='{info.NpcName}', ResType='{info.NpcResType}'");
            return info?.NpcResType;
        }

        /// <summary>
        /// Get NPC name by ID
        /// </summary>
        public string GetNpcName(int npcId)
        {
            NpcInfo info = GetNpcInfo(npcId);
            return info?.NpcName;
        }

        /// <summary>
        /// Find NPC by name (partial match, case-insensitive)
        /// Returns the first matching NPC ID, or null if not found
        /// </summary>
        public int? FindNpcByName(string searchName)
        {
            if (string.IsNullOrWhiteSpace(searchName))
                return null;

            searchName = searchName.Trim();
            DebugLogger.Log($"      [FindNpcByName] Searching for: '{searchName}'");
            DebugLogger.Log($"         Database contains {_npcDatabase.Count} NPCs");

            // Try exact match first
            foreach (var kvp in _npcDatabase)
            {
                if (kvp.Value.NpcName != null && kvp.Value.NpcName.Equals(searchName, StringComparison.OrdinalIgnoreCase))
                {
                    DebugLogger.Log($"         ✓ Exact match found: ID={kvp.Key}, Name='{kvp.Value.NpcName}'");
                    return kvp.Key;
                }
            }

            DebugLogger.Log($"         No exact match, trying partial match...");

            // Try partial match (contains)
            int matchCount = 0;
            foreach (var kvp in _npcDatabase)
            {
                if (kvp.Value.NpcName != null && kvp.Value.NpcName.IndexOf(searchName, StringComparison.OrdinalIgnoreCase) >= 0)
                {
                    DebugLogger.Log($"         ✓ Partial match found: ID={kvp.Key}, Name='{kvp.Value.NpcName}'");
                    return kvp.Key;
                }
                // Show first few NPC names for debugging
                if (matchCount < 5 && !string.IsNullOrEmpty(kvp.Value.NpcName))
                {
                    DebugLogger.Log($"         Sample NPC: ID={kvp.Key}, Name='{kvp.Value.NpcName}'");
                    matchCount++;
                }
            }

            DebugLogger.Log($"         ✗ No match found for '{searchName}'");
            return null;
        }

        /// <summary>
        /// Check if NPC ID exists
        /// </summary>
        public bool HasNpc(int npcId)
        {
            return _npcDatabase.ContainsKey(npcId);
        }

        /// <summary>
        /// Get all NPC IDs
        /// </summary>
        public IEnumerable<int> GetAllNpcIds()
        {
            return _npcDatabase.Keys;
        }

        /// <summary>
        /// Get count of loaded NPCs
        /// </summary>
        public int Count => _npcDatabase.Count;
    }
}
