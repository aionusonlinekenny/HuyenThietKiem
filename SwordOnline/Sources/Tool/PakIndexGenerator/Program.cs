using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using MapTool;
using MapTool.PakFile;

namespace PakIndexGenerator
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.OutputEncoding = Encoding.UTF8;
            Console.WriteLine("═══════════════════════════════════════════════════════");
            Console.WriteLine("  PAK Index Generator - Huyền Thiết Kiếm");
            Console.WriteLine("  Generates .pak.txt index files by scanning folders");
            Console.WriteLine("═══════════════════════════════════════════════════════\n");

            if (args.Length < 2)
            {
                Console.WriteLine("Usage: PakIndexGenerator <pak-file> <scan-folder>");
                Console.WriteLine();
                Console.WriteLine("Example:");
                Console.WriteLine("  PakIndexGenerator.exe jxhangnga1.pak C:\\Game\\Bin\\Client");
                Console.WriteLine("  PakIndexGenerator.exe settings.pak C:\\Game\\Bin\\Client");
                Console.WriteLine();
                Console.WriteLine("This will:");
                Console.WriteLine("  1. Scan all files in <scan-folder>");
                Console.WriteLine("  2. Calculate hash for each file path");
                Console.WriteLine("  3. Match with hashes in <pak-file>");
                Console.WriteLine("  4. Generate <pak-file>.txt with filename mappings");
                return;
            }

            string pakFile = args[0];
            string scanFolder = args[1];

            if (!File.Exists(pakFile))
            {
                Console.WriteLine($"❌ ERROR: PAK file not found: {pakFile}");
                return;
            }

            if (!Directory.Exists(scanFolder))
            {
                Console.WriteLine($"❌ ERROR: Scan folder not found: {scanFolder}");
                return;
            }

            try
            {
                GenerateIndex(pakFile, scanFolder);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"\n❌ FATAL ERROR: {ex.Message}");
                Console.WriteLine($"Stack trace:\n{ex.StackTrace}");
            }
        }

        static void GenerateIndex(string pakFile, string scanFolder)
        {
            Console.WriteLine($"📂 PAK File: {pakFile}");
            Console.WriteLine($"📁 Scan Folder: {scanFolder}");
            Console.WriteLine();

            // Step 1: Load PAK file to get all hash IDs
            Console.WriteLine("Step 1: Loading PAK file...");
            PakFileReader pakReader = new PakFileReader(pakFile);
            var stats = pakReader.GetStatistics();
            Console.WriteLine($"  ✓ Loaded PAK: {stats.TotalFiles} files, {stats.TotalSize / 1024 / 1024:N1} MB");

            // Get all file IDs from PAK
            var pakFileIds = pakReader.GetAllFileIds();
            Console.WriteLine($"  ✓ Found {pakFileIds.Count} file hashes in PAK");
            Console.WriteLine();

            // Step 2: Scan folder recursively
            Console.WriteLine("Step 2: Scanning folder for files...");
            var allFiles = ScanFolder(scanFolder);
            Console.WriteLine($"  ✓ Found {allFiles.Count} files in folder");
            Console.WriteLine();

            // Step 3: Calculate hash for each file and match
            Console.WriteLine("Step 3: Matching file paths with PAK hashes...");
            var matches = new Dictionary<uint, string>(); // hash -> file path
            int totalChecked = 0;

            foreach (var filePath in allFiles)
            {
                totalChecked++;
                if (totalChecked % 1000 == 0)
                {
                    Console.Write($"\r  Checked: {totalChecked:N0}/{allFiles.Count:N0}...");
                }

                // Get relative path from scan folder
                string relativePath = GetRelativePath(scanFolder, filePath);

                // Normalize to backslash
                relativePath = relativePath.Replace('/', '\\');
                if (!relativePath.StartsWith("\\"))
                {
                    relativePath = "\\" + relativePath;
                }

                // Calculate hash
                uint hash = FileNameHasher.CalculateFileId(relativePath);

                // Check if this hash exists in PAK
                if (pakFileIds.Contains(hash))
                {
                    matches[hash] = relativePath;
                }
            }

            Console.WriteLine($"\r  ✓ Matched: {matches.Count}/{pakFileIds.Count} files ({(double)matches.Count / pakFileIds.Count * 100:F1}%)");
            Console.WriteLine();

            // Step 4: Generate .pak.txt file
            Console.WriteLine("Step 4: Generating .pak.txt index file...");
            string txtFile = pakFile + ".txt";
            GeneratePakTxt(txtFile, matches);
            Console.WriteLine($"  ✓ Generated: {txtFile}");
            Console.WriteLine($"  ✓ Contains {matches.Count} filename mappings");
            Console.WriteLine();

            // Summary
            Console.WriteLine("═══════════════════════════════════════════════════════");
            Console.WriteLine("✓ Index generation complete!");
            Console.WriteLine($"  Total PAK files: {pakFileIds.Count}");
            Console.WriteLine($"  Matched files: {matches.Count}");
            Console.WriteLine($"  Unmatched files: {pakFileIds.Count - matches.Count}");
            Console.WriteLine($"  Match rate: {(double)matches.Count / pakFileIds.Count * 100:F1}%");
            Console.WriteLine("═══════════════════════════════════════════════════════");
        }

        static List<string> ScanFolder(string folder)
        {
            var files = new List<string>();

            try
            {
                // Get all files recursively
                files.AddRange(Directory.GetFiles(folder, "*.*", SearchOption.AllDirectories));
            }
            catch (Exception ex)
            {
                Console.WriteLine($"  ⚠ Warning: Error scanning folder: {ex.Message}");
            }

            return files;
        }

        static string GetRelativePath(string basePath, string fullPath)
        {
            Uri baseUri = new Uri(basePath.TrimEnd('\\', '/') + "\\");
            Uri fullUri = new Uri(fullPath);
            return Uri.UnescapeDataString(baseUri.MakeRelativeUri(fullUri).ToString());
        }

        static void GeneratePakTxt(string txtFile, Dictionary<uint, string> matches)
        {
            using (StreamWriter writer = new StreamWriter(txtFile, false, Encoding.GetEncoding("GB2312")))
            {
                // Write header
                writer.WriteLine($"Total Files: {matches.Count}");
                writer.WriteLine("Index\tID\tTime\tFileName\tSize\tCompressedSize\tRatio");

                // Write each match
                int index = 0;
                foreach (var kvp in matches.OrderBy(x => x.Value))
                {
                    uint hash = kvp.Key;
                    string fileName = kvp.Value;

                    // Format: Index, ID(hex), Time, FileName, Size, CompressedSize, Ratio
                    writer.WriteLine($"{index}\t{hash:X8}\t0\t{fileName}\t0\t0\t0.00%");
                    index++;
                }
            }
        }
    }

    /// <summary>
    /// Extension to get all file IDs from PAK
    /// </summary>
    public static class PakFileReaderExtensions
    {
        // We need to add a method to PakFileReader to expose file IDs
    }
}
