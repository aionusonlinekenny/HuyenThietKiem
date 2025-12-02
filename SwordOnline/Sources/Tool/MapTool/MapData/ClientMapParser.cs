using System;
using System.IO;
using System.Linq;
using System.Text;

namespace MapTool.MapData
{
    /// <summary>
    /// Parser for Client .map files (simplified format)
    /// Client .map format:
    /// Section 1: NPC/Object positions (WorldX WorldY Name)
    /// 0 0   <- separator
    /// Section 2: Region grid (RegionX RegionY Value)
    /// 0 0   <- end marker
    /// </summary>
    public class ClientMapParser
    {
        /// <summary>
        /// Parse Client .map file to extract map config (region bounds)
        /// </summary>
        public static MapConfig ParseClientMapFile(string mapFilePath, string mapName = "Unknown")
        {
            if (!File.Exists(mapFilePath))
            {
                throw new FileNotFoundException($"Client .map file not found: {mapFilePath}");
            }

            // Read file with Windows-1252 encoding (TCVN3 Vietnamese)
            string[] lines = File.ReadAllLines(mapFilePath, Encoding.GetEncoding("Windows-1252"));

            return ParseClientMapContent(lines, mapName);
        }

        /// <summary>
        /// Parse Client .map from byte array (for PAK support)
        /// </summary>
        public static MapConfig ParseClientMapFromBytes(byte[] data, string mapName = "Unknown")
        {
            if (data == null || data.Length == 0)
            {
                throw new ArgumentException("Client .map data is null or empty");
            }

            // Decode with Windows-1252 encoding
            string content = Encoding.GetEncoding("Windows-1252").GetString(data);
            string[] lines = content.Split(new[] { "\r\n", "\r", "\n" }, StringSplitOptions.None);

            return ParseClientMapContent(lines, mapName);
        }

        /// <summary>
        /// Parse Client .map content (shared logic)
        /// </summary>
        private static MapConfig ParseClientMapContent(string[] lines, string mapName)
        {
            DebugLogger.LogSeparator();
            DebugLogger.Log($"📋 PARSING CLIENT .MAP FILE");
            DebugLogger.Log($"   Map Name: {mapName}");
            DebugLogger.Log($"   Total Lines: {lines.Length}");

            // Find the separator line "0 0" that marks the start of region data
            int regionDataStartLine = -1;
            for (int i = 0; i < lines.Length; i++)
            {
                string trimmed = lines[i].Trim();
                if (trimmed == "0 0")
                {
                    regionDataStartLine = i + 1; // Region data starts after "0 0"
                    DebugLogger.Log($"   Region data starts at line: {regionDataStartLine + 1}");
                    break;
                }
            }

            if (regionDataStartLine == -1)
            {
                throw new Exception($"Invalid Client .map format: no region data separator found");
            }

            // Parse region grid to find bounds
            int minX = int.MaxValue;
            int minY = int.MaxValue;
            int maxX = int.MinValue;
            int maxY = int.MinValue;
            int regionCount = 0;

            for (int i = regionDataStartLine; i < lines.Length; i++)
            {
                string trimmed = lines[i].Trim();

                // End marker
                if (trimmed == "0 0")
                {
                    DebugLogger.Log($"   Region data ends at line: {i + 1}");
                    break;
                }

                // Skip empty lines
                if (string.IsNullOrEmpty(trimmed))
                    continue;

                // Parse: RegionX RegionY Value
                string[] parts = trimmed.Split(new[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
                if (parts.Length < 2)
                    continue;

                if (int.TryParse(parts[0], out int x) && int.TryParse(parts[1], out int y))
                {
                    minX = Math.Min(minX, x);
                    minY = Math.Min(minY, y);
                    maxX = Math.Max(maxX, x);
                    maxY = Math.Max(maxY, y);
                    regionCount++;
                }
            }

            if (regionCount == 0)
            {
                throw new Exception($"No region data found in Client .map file");
            }

            DebugLogger.Log($"   Total regions: {regionCount}");
            DebugLogger.Log($"   Region bounds: X=[{minX}, {maxX}], Y=[{minY}, {maxY}]");

            // Create MapConfig
            MapConfig config = new MapConfig
            {
                MapName = mapName,
                RegionLeft = minX,
                RegionTop = minY,
                RegionRight = maxX,
                RegionBottom = maxY,
                IsIndoor = false // Client maps are typically outdoor
            };

            DebugLogger.Log($"✓ Parsed Client .map successfully");
            DebugLogger.Log($"   Region Width: {config.RegionWidth}");
            DebugLogger.Log($"   Region Height: {config.RegionHeight}");
            DebugLogger.LogSeparator();

            return config;
        }
    }
}
