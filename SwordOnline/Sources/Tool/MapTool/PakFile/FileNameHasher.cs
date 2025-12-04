using System;
using System.Text;

namespace MapTool.PakFile
{
    /// <summary>
    /// File name hash calculator matching game engine's g_FileName2Id function
    /// Port from: Engine/Src/KFilePath.cpp
    /// </summary>
    public static class FileNameHasher
    {
        /// <summary>
        /// Calculate hash ID for a filename (ANSI encoding required)
        /// Matches: ENGINE_API DWORD g_FileName2Id(LPSTR lpFileName)
        /// </summary>
        /// <param name="fileName">Filename with path (e.g., "\maps\场景地图\城市\成都\成都.wor")</param>
        /// <returns>Hash ID used in pak file index</returns>
        public static uint CalculateFileId(string fileName)
        {
            // Convert to ANSI bytes using GBK encoding (superset of GB2312, supports more Chinese characters)
            // GBK supports both Simplified Chinese (GB2312) AND Traditional Chinese characters
            byte[] ansiBytes = Encoding.GetEncoding("GBK").GetBytes(fileName);
            return CalculateFileIdFromBytes(ansiBytes);
        }

        /// <summary>
        /// Calculate hash ID with specific encoding
        /// Useful for trying different encodings to find match
        /// </summary>
        /// <param name="fileName">Filename with path</param>
        /// <param name="encodingName">Encoding name (e.g., "GBK", "GB2312", "Big5")</param>
        /// <returns>Hash ID</returns>
        public static uint CalculateFileIdWithEncoding(string fileName, string encodingName)
        {
            try
            {
                byte[] ansiBytes = Encoding.GetEncoding(encodingName).GetBytes(fileName);
                return CalculateFileIdFromBytes(ansiBytes);
            }
            catch
            {
                // Fallback to GBK if encoding not available
                return CalculateFileId(fileName);
            }
        }

        /// <summary>
        /// Calculate hash from raw ANSI bytes
        /// Exact port of g_FileName2Id algorithm
        /// </summary>
        public static uint CalculateFileIdFromBytes(byte[] ansiBytes)
        {
            uint id = 0;

            for (int i = 0; i < ansiBytes.Length; i++)
            {
                byte c = ansiBytes[i];

                // Convert forward slash to backslash (Unix path normalization)
                if (c == (byte)'/')
                    c = (byte)'\\';

                // CRITICAL: Convert uppercase A-Z to lowercase DURING hash calculation
                // This matches the C++ FileNameToId behavior
                if (c >= 'A' && c <= 'Z')
                    c = (byte)(c + ('a' - 'A'));  // Convert to lowercase

                // Hash algorithm from KFilePath.cpp:
                // Id = (Id + (i + 1) * c) % 0x8000000b * 0xffffffef;
                id = (id + (uint)(i + 1) * c) % 0x8000000b * 0xffffffef;
            }

            // XOR with magic constant
            return id ^ 0x12345678;
        }

        /// <summary>
        /// Normalize path separators to backslash (Windows style)
        /// Game uses backslash in pak file paths
        /// </summary>
        public static string NormalizePath(string path)
        {
            return path.Replace('/', '\\');
        }

        /// <summary>
        /// Convert filename to lowercase and normalize for comparison
        /// Note: Game does NOT lowercase, but normalizes slashes
        /// </summary>
        public static string NormalizeFileName(string fileName)
        {
            // Only normalize slashes, do NOT change case
            // Chinese characters are case-insensitive anyway
            return NormalizePath(fileName);
        }
    }
}
