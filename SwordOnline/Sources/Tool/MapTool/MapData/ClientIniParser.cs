using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace MapTool.MapData
{
    /// <summary>
    /// Parser for Client .ini files (background area configuration)
    /// Client .ini format:
    /// [Main]
    /// Count=3
    /// [0]
    /// AreaLeft=176
    /// AreaTop=100
    /// AreaRight=185
    /// AreaBottom=115
    /// Image=\游戏资源\background\青城山.jpg
    /// </summary>
    public class ClientIniParser
    {
        public class AreaSection
        {
            public int AreaLeft { get; set; }
            public int AreaTop { get; set; }
            public int AreaRight { get; set; }
            public int AreaBottom { get; set; }
            public int PicCenterPointX { get; set; }
            public int PicCenterPointY { get; set; }
            public string ImagePath { get; set; }
        }

        /// <summary>
        /// Parse Client .ini file to extract map config and area sections
        /// </summary>
        public static MapConfig ParseClientIniFile(string iniFilePath, string mapName = "Unknown")
        {
            if (!File.Exists(iniFilePath))
            {
                throw new FileNotFoundException($"Client .ini file not found: {iniFilePath}");
            }

            // Read file with GB2312 encoding (Chinese)
            string[] lines = File.ReadAllLines(iniFilePath, Encoding.GetEncoding("GB2312"));

            return ParseClientIniContent(lines, mapName);
        }

        /// <summary>
        /// Parse Client .ini from byte array (for PAK support)
        /// </summary>
        public static MapConfig ParseClientIniFromBytes(byte[] data, string mapName = "Unknown")
        {
            if (data == null || data.Length == 0)
            {
                throw new ArgumentException("Client .ini data is null or empty");
            }

            // Decode with GB2312 encoding
            string content = Encoding.GetEncoding("GB2312").GetString(data);
            string[] lines = content.Split(new[] { "\r\n", "\r", "\n" }, StringSplitOptions.None);

            return ParseClientIniContent(lines, mapName);
        }

        /// <summary>
        /// Parse Client .ini content (shared logic)
        /// </summary>
        private static MapConfig ParseClientIniContent(string[] lines, string mapName)
        {
            DebugLogger.LogSeparator();
            DebugLogger.Log($"📋 PARSING CLIENT .INI FILE");
            DebugLogger.Log($"   Map Name: {mapName}");
            DebugLogger.Log($"   Total Lines: {lines.Length}");

            List<AreaSection> areas = new List<AreaSection>();
            AreaSection currentArea = null;
            int sectionCount = 0;

            for (int i = 0; i < lines.Length; i++)
            {
                string line = lines[i].Trim();

                // Skip empty lines and comments
                if (string.IsNullOrEmpty(line) || line.StartsWith(";") || line.StartsWith("#"))
                    continue;

                // Check for section headers
                if (line.StartsWith("[") && line.EndsWith("]"))
                {
                    string sectionName = line.Substring(1, line.Length - 2);

                    // Save previous area
                    if (currentArea != null)
                    {
                        areas.Add(currentArea);
                    }

                    // Start new area section (ignore [Main])
                    if (sectionName != "Main" && int.TryParse(sectionName, out _))
                    {
                        currentArea = new AreaSection();
                        sectionCount++;
                    }
                    else
                    {
                        currentArea = null;
                    }

                    continue;
                }

                // Parse key=value pairs
                int equalsIndex = line.IndexOf('=');
                if (equalsIndex > 0 && currentArea != null)
                {
                    string key = line.Substring(0, equalsIndex).Trim();
                    string value = line.Substring(equalsIndex + 1).Trim();

                    switch (key)
                    {
                        case "AreaLeft":
                            if (int.TryParse(value, out int left))
                                currentArea.AreaLeft = left;
                            break;
                        case "AreaTop":
                            if (int.TryParse(value, out int top))
                                currentArea.AreaTop = top;
                            break;
                        case "AreaRight":
                            if (int.TryParse(value, out int right))
                                currentArea.AreaRight = right;
                            break;
                        case "AreaBottom":
                            if (int.TryParse(value, out int bottom))
                                currentArea.AreaBottom = bottom;
                            break;
                        case "PicCenterPointX":
                            if (int.TryParse(value, out int centerX))
                                currentArea.PicCenterPointX = centerX;
                            break;
                        case "PicCenterPointY":
                            if (int.TryParse(value, out int centerY))
                                currentArea.PicCenterPointY = centerY;
                            break;
                        case "Image":
                            currentArea.ImagePath = value;
                            break;
                    }
                }
            }

            // Add last area
            if (currentArea != null)
            {
                areas.Add(currentArea);
            }

            if (areas.Count == 0)
            {
                throw new Exception($"No area sections found in Client .ini file");
            }

            DebugLogger.Log($"   Total area sections: {areas.Count}");

            // Calculate overall bounds from all areas
            int minX = int.MaxValue;
            int minY = int.MaxValue;
            int maxX = int.MinValue;
            int maxY = int.MinValue;

            foreach (var area in areas)
            {
                minX = Math.Min(minX, area.AreaLeft);
                minY = Math.Min(minY, area.AreaTop);
                maxX = Math.Max(maxX, area.AreaRight);
                maxY = Math.Max(maxY, area.AreaBottom);

                DebugLogger.Log($"      Area [{areas.IndexOf(area)}]: X=[{area.AreaLeft}, {area.AreaRight}], Y=[{area.AreaTop}, {area.AreaBottom}]");
                DebugLogger.Log($"               Image: {area.ImagePath}");
            }

            DebugLogger.Log($"   Overall region bounds: X=[{minX}, {maxX}], Y=[{minY}, {maxY}]");

            // Create MapConfig with first area's image path
            MapConfig config = new MapConfig
            {
                MapName = mapName,
                RegionLeft = minX,
                RegionTop = minY,
                RegionRight = maxX,
                RegionBottom = maxY,
                IsIndoor = false,
                // Store first area's background image path
                BackgroundImagePath = areas.Count > 0 ? areas[0].ImagePath : null
            };

            DebugLogger.Log($"✓ Parsed Client .ini successfully");
            DebugLogger.Log($"   Region Width: {config.RegionWidth}");
            DebugLogger.Log($"   Region Height: {config.RegionHeight}");
            DebugLogger.Log($"   Background Image: {config.BackgroundImagePath}");
            DebugLogger.LogSeparator();

            return config;
        }
    }
}
