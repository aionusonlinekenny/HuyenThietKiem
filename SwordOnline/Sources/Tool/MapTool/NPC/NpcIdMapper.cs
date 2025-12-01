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
            if (!File.Exists(npcsFile))
            {
                throw new FileNotFoundException($"Npcs.txt not found at: {npcsFile}");
            }

            _npcDatabase.Clear();

            // Read with GB2312 encoding (Chinese encoding)
            Encoding gb2312 = Encoding.GetEncoding("GB2312");
            string[] lines = File.ReadAllLines(npcsFile, gb2312);

            // Line 1 is header, Line 2+ are NPC data
            // Line 2 = ID 1, Line 3 = ID 2, etc. (line index - 1 = NPC ID)
            for (int i = 1; i < lines.Length; i++)
            {
                string line = lines[i].Trim();
                if (string.IsNullOrEmpty(line))
                    continue;

                string[] columns = line.Split('\t');
                if (columns.Length < 12)
                    continue;  // Need at least 12 columns for NpcResType

                int npcId = i - 1; // Line 2 = ID 1, Line 3 = ID 2

                NpcInfo info = new NpcInfo
                {
                    NpcId = npcId,
                    NpcName = columns.Length > 1 ? columns[1] : "",
                    Kind = columns.Length > 2 ? columns[2] : "",
                    Camp = columns.Length > 3 ? columns[3] : "",
                    Series = columns.Length > 4 ? columns[4] : "",
                    NpcResType = columns[11].Trim()  // Column 12 (0-indexed = 11)
                };

                _npcDatabase[npcId] = info;
            }

            Console.WriteLine($"Loaded {_npcDatabase.Count} NPCs from Npcs.txt");
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
            NpcInfo info = GetNpcInfo(npcId);
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
