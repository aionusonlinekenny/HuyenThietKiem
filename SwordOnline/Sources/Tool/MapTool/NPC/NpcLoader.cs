using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using MapTool.Utils;

namespace MapTool.NPC
{
    /// <summary>
    /// NPC resource loader
    /// Loads NPC data from client Settings files
    /// </summary>
    public class NpcLoader
    {
        private string _clientPath;
        private string _serverPath;
        private NpcIdMapper _idMapper;
        private Dictionary<string, NpcResKind> _resKindCache;
        private Dictionary<string, NpcSpriteRes> _spriteResCache;
        private Dictionary<string, NpcSpriteInfo> _spriteInfoCache;

        public NpcLoader(string clientPath, string serverPath = null)
        {
            _clientPath = clientPath;
            _serverPath = serverPath;
            _idMapper = serverPath != null ? new NpcIdMapper() : null;
            _resKindCache = new Dictionary<string, NpcResKind>(StringComparer.OrdinalIgnoreCase);
            _spriteResCache = new Dictionary<string, NpcSpriteRes>(StringComparer.OrdinalIgnoreCase);
            _spriteInfoCache = new Dictionary<string, NpcSpriteInfo>(StringComparer.OrdinalIgnoreCase);
        }

        /// <summary>
        /// Load all NPC mapping files
        /// </summary>
        public void LoadMappingFiles()
        {
            LoadNpcResKind();
            LoadNpcSpriteRes();
            LoadNpcSpriteInfo();

            // Load NPC ID database if server path is available
            if (_idMapper != null && !string.IsNullOrEmpty(_serverPath))
            {
                _idMapper.LoadNpcDatabase(_serverPath);
            }
        }

        /// <summary>
        /// Load NpcResKind.txt
        /// Format: CharacterName | CharacterType | empty | empty | ResFilePath
        /// </summary>
        private void LoadNpcResKind()
        {
            string filePath = Path.Combine(_clientPath, "Settings", "NpcRes", "NpcResKind.txt");
            if (!File.Exists(filePath))
            {
                throw new FileNotFoundException($"NpcResKind.txt not found: {filePath}");
            }

            _resKindCache.Clear();

            string[] lines = File.ReadAllLines(filePath, Encoding.GetEncoding("GB2312"));
            foreach (string line in lines)
            {
                string trimmed = line.Trim();
                if (string.IsNullOrEmpty(trimmed) || trimmed.StartsWith("#") || trimmed.StartsWith("//"))
                    continue;

                string[] parts = trimmed.Split('\t');
                if (parts.Length >= 5)
                {
                    string charName = parts[0].Trim();
                    if (string.IsNullOrEmpty(charName))
                        continue;

                    NpcResKind resKind = new NpcResKind
                    {
                        CharacterName = charName,
                        CharacterType = parts[1].Trim(),
                        ResFilePath = parts[4].Trim()
                    };

                    _resKindCache[charName] = resKind;
                }
            }
        }

        /// <summary>
        /// Load NpcNormalRes.txt
        /// Format: NpcList | FightStand | NormalStand1 | ... | Run
        /// </summary>
        private void LoadNpcSpriteRes()
        {
            string filePath = Path.Combine(_clientPath, "Settings", "NpcRes", "NpcNormalRes.txt");
            if (!File.Exists(filePath))
            {
                throw new FileNotFoundException($"NpcNormalRes.txt not found: {filePath}");
            }

            _spriteResCache.Clear();

            string[] lines = File.ReadAllLines(filePath, Encoding.GetEncoding("GB2312"));
            foreach (string line in lines)
            {
                string trimmed = line.Trim();
                if (string.IsNullOrEmpty(trimmed) || trimmed.StartsWith("#") || trimmed.StartsWith("//"))
                    continue;

                string[] parts = trimmed.Split('\t');
                if (parts.Length >= 11)
                {
                    string npcName = parts[0].Trim();
                    if (string.IsNullOrEmpty(npcName) || npcName.Equals("NpcList", StringComparison.OrdinalIgnoreCase))
                        continue;

                    NpcSpriteRes spriteRes = new NpcSpriteRes
                    {
                        NpcName = npcName,
                        FightStand = parts[1].Trim(),
                        NormalStand1 = parts[2].Trim(),
                        NormalWalk = parts[3].Trim(),
                        Attack1 = parts[4].Trim(),
                        Attack2 = parts[5].Trim(),
                        Attack3 = parts[6].Trim(),
                        CastSkill = parts[7].Trim(),
                        Hurt = parts[8].Trim(),
                        Die = parts[9].Trim(),
                        Run = parts[10].Trim()
                    };

                    _spriteResCache[npcName] = spriteRes;
                }
            }
        }

        /// <summary>
        /// Load NpcNormalSprInfo.txt
        /// Format: NpcList | FightStand | NormalStand1 | ...
        /// Values: "width,directions,interval"
        /// </summary>
        private void LoadNpcSpriteInfo()
        {
            string filePath = Path.Combine(_clientPath, "Settings", "NpcRes", "NpcNormalSprInfo.txt");
            if (!File.Exists(filePath))
            {
                throw new FileNotFoundException($"NpcNormalSprInfo.txt not found: {filePath}");
            }

            _spriteInfoCache.Clear();

            string[] lines = File.ReadAllLines(filePath, Encoding.GetEncoding("GB2312"));
            foreach (string line in lines)
            {
                string trimmed = line.Trim();
                if (string.IsNullOrEmpty(trimmed) || trimmed.StartsWith("#") || trimmed.StartsWith("//"))
                    continue;

                string[] parts = trimmed.Split('\t');
                if (parts.Length >= 11)
                {
                    string npcName = parts[0].Trim();
                    if (string.IsNullOrEmpty(npcName) || npcName.Equals("NpcList", StringComparison.OrdinalIgnoreCase))
                        continue;

                    NpcSpriteInfo spriteInfo = new NpcSpriteInfo
                    {
                        NpcName = npcName,
                        FightStand = SpriteFrameInfo.Parse(parts[1]),
                        NormalStand1 = SpriteFrameInfo.Parse(parts[2]),
                        NormalWalk = SpriteFrameInfo.Parse(parts[3]),
                        Attack1 = SpriteFrameInfo.Parse(parts[4]),
                        Attack2 = SpriteFrameInfo.Parse(parts[5]),
                        Attack3 = SpriteFrameInfo.Parse(parts[6]),
                        CastSkill = SpriteFrameInfo.Parse(parts[7]),
                        Hurt = SpriteFrameInfo.Parse(parts[8]),
                        Die = SpriteFrameInfo.Parse(parts[9]),
                        Run = SpriteFrameInfo.Parse(parts[10])
                    };

                    _spriteInfoCache[npcName] = spriteInfo;
                }
            }
        }

        /// <summary>
        /// Get NPC resource by NPC ID (from Npcs.txt)
        /// </summary>
        public NpcResource GetNpcResourceById(int npcId)
        {
            DebugLogger.Log($"      → GetNpcResourceById: Looking up NPC ID {npcId}");

            if (_idMapper == null)
            {
                DebugLogger.Log($"         ✗ ERROR: NPC ID mapper is null (not initialized)");
                throw new InvalidOperationException("NPC ID mapper not initialized. Need server path.");
            }

            DebugLogger.Log($"         NPC database count: {_idMapper.Count} NPCs loaded");

            // Get NpcResType from ID (e.g., ID 49 → "enemy003")
            string npcResType = _idMapper.GetNpcResType(npcId);
            DebugLogger.Log($"         NpcResType for ID {npcId}: '{npcResType ?? "(null)"}'");

            if (string.IsNullOrEmpty(npcResType))
            {
                DebugLogger.Log($"         ✗ ERROR: NpcResType is null or empty for ID {npcId}");
                return null;
            }

            // Get resource using NpcResType
            DebugLogger.Log($"         Calling GetNpcResource('{npcResType}')...");
            NpcResource resource = GetNpcResource(npcResType);

            if (resource == null)
            {
                DebugLogger.Log($"         ✗ ERROR: GetNpcResource returned null for '{npcResType}'");
                return null;
            }

            DebugLogger.Log($"         ✓ NpcResource found for '{npcResType}'");

            resource.NpcID = npcId;
            // Also set the display name from Npcs.txt
            string npcName = _idMapper.GetNpcName(npcId);
            DebugLogger.Log($"         NPC name from Npcs.txt: '{npcName ?? "(null)"}'");
            if (!string.IsNullOrEmpty(npcName))
            {
                resource.NpcName = $"{npcName} (ID: {npcId})";
                DebugLogger.Log($"         Final resource name: '{resource.NpcName}'");
            }

            return resource;
        }

        /// <summary>
        /// Get NPC resource by NPC name (ResType like "enemy003")
        /// </summary>
        public NpcResource GetNpcResource(string npcName)
        {
            if (string.IsNullOrEmpty(npcName))
                return null;

            if (!_resKindCache.ContainsKey(npcName))
                return null;

            NpcResource resource = new NpcResource
            {
                NpcName = npcName,
                ResKind = _resKindCache[npcName],
                SpriteRes = _spriteResCache.ContainsKey(npcName) ? _spriteResCache[npcName] : null,
                SpriteInfo = _spriteInfoCache.ContainsKey(npcName) ? _spriteInfoCache[npcName] : null
            };

            return resource;
        }

        /// <summary>
        /// Get NPC info by ID from Npcs.txt
        /// </summary>
        public NpcIdMapper.NpcInfo GetNpcInfo(int npcId)
        {
            if (_idMapper == null)
                return null;

            return _idMapper.GetNpcInfo(npcId);
        }

        /// <summary>
        /// Get all loaded NPC names
        /// </summary>
        public List<string> GetAllNpcNames()
        {
            return new List<string>(_resKindCache.Keys);
        }

        /// <summary>
        /// Get full SPR file path on disk by NPC name
        /// </summary>
        public string GetSprFilePath(string npcName, NpcAction action)
        {
            NpcResource resource = GetNpcResource(npcName);
            if (resource == null)
                return null;

            string relativePath = resource.GetSprFilePath(action);
            if (string.IsNullOrEmpty(relativePath))
                return null;

            // Convert to full path
            string fullPath = Path.Combine(_clientPath, relativePath);
            return fullPath;
        }

        /// <summary>
        /// Get full SPR file path on disk by NPC ID
        /// </summary>
        public string GetSprFilePathById(int npcId, NpcAction action)
        {
            NpcResource resource = GetNpcResourceById(npcId);
            if (resource == null)
                return null;

            string relativePath = resource.GetSprFilePath(action);
            if (string.IsNullOrEmpty(relativePath))
                return null;

            // Convert to full path
            string fullPath = Path.Combine(_clientPath, relativePath);
            return fullPath;
        }
    }
}
