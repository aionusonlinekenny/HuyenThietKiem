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
        /// Note: Game engine calls g_GetPackPath() first which removes leading slash!
        /// </summary>
        /// <param name="fileName">Filename with path (e.g., "\maps\场景地图\城市\成都\成都.wor")</param>
        /// <returns>Hash ID used in pak file index</returns>
        public static uint CalculateFileId(string fileName)
        {
            // Match game engine g_GetPackPath() behavior: remove leading slash/backslash
            // From KFilePath.cpp: if (lpFileName[0] == '\\') g_StrCpy(lpPathName, lpFileName + 1);
            while (fileName.StartsWith("\\") || fileName.StartsWith("/"))
            {
                fileName = fileName.Substring(1);
            }

            // Convert to ANSI bytes using GB2312 encoding (same as game)
            byte[] ansiBytes = Encoding.GetEncoding("GB2312").GetBytes(fileName);
            return CalculateFileIdFromBytes(ansiBytes);
        }

        /// <summary>
        /// Calculate hash from raw ANSI bytes
        /// Exact port of g_FileName2Id algorithm from KPakList.cpp
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

                // Convert A-Z to a-z (match game engine FileNameToId)
                // From KPakList.cpp: if(*ptr >= 'A' && *ptr <= 'Z') ... (*ptr + 'a' - 'A')
                if (c >= (byte)'A' && c <= (byte)'Z')
                    c = (byte)(c + 32); // 'a' - 'A' = 32

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
