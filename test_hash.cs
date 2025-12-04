using System;
using System.Text;

class TestHash
{
    static uint CalculateFileId(string path, string encodingName)
    {
        byte[] bytes = Encoding.GetEncoding(encodingName).GetBytes(path);
        uint id = 0;

        for (int i = 0; i < bytes.Length; i++)
        {
            byte c = bytes[i];
            if (c == (byte)'/') c = (byte)'\\';

            // CRITICAL: Convert uppercase A-Z to lowercase DURING hash calculation
            if (c >= 'A' && c <= 'Z')
                c = (byte)(c + ('a' - 'A'));

            id = (id + (uint)(i + 1) * c) % 0x8000000b * 0xffffffef;
        }

        return id ^ 0x12345678;
    }

    static void Main()
    {
        // Test known good path from user's log
        string path1 = "\\spr\\obj\\money\\obj_money_05_fall.spr";
        uint hash1 = CalculateFileId(path1, "iso-8859-1");
        Console.WriteLine($"Path: {path1}");
        Console.WriteLine($"Hash: 0x{hash1:X8}");
        Console.WriteLine($"Expected: 0x5BD62620");
        Console.WriteLine($"Match: {hash1 == 0x5BD62620}");
        Console.WriteLine();

        // Test garbled path from user's log
        string path2 = "\\spr\\skill\\¶ëáÒ\\mag_em_02_ËÄÏóÍ¬¹é.spr";
        uint hash2_iso = CalculateFileId(path2, "iso-8859-1");
        uint hash2_gbk = CalculateFileId(path2, "GBK");
        Console.WriteLine($"Path: {path2}");
        Console.WriteLine($"Hash [iso-8859-1]: 0x{hash2_iso:X8}");
        Console.WriteLine($"Hash [GBK]:        0x{hash2_gbk:X8}");
        Console.WriteLine($"Expected [iso-8859-1]: 0x3B3D7B90 (from user log)");
        Console.WriteLine($"Match: {hash2_iso == 0x3B3D7B90}");
    }
}
