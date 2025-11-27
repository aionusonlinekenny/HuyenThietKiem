using System;
using System.IO;
using MapTool.PakFile;

namespace PakExtractTool
{
    /// <summary>
    /// Simple tool to extract all files from maps.pak
    /// Usage: PakExtractTool.exe <pakfile> <output_folder>
    /// Example: PakExtractTool.exe "Bin/Server/pak/maps.pak" "Bin/Server/maps"
    /// </summary>
    class Program
    {
        static void Main(string[] args)
        {
            Console.OutputEncoding = System.Text.Encoding.UTF8;
            Console.WriteLine("═══════════════════════════════════════════════════════");
            Console.WriteLine("  PAK EXTRACT TOOL - Huyền Thiết Kiếm");
            Console.WriteLine("═══════════════════════════════════════════════════════");
            Console.WriteLine();

            // Parse arguments
            string pakFile;
            string outputFolder;

            if (args.Length >= 2)
            {
                pakFile = args[0];
                outputFolder = args[1];
            }
            else if (args.Length == 1)
            {
                pakFile = args[0];
                // Default output folder: same location as pak file, but named 'extracted'
                string pakDir = Path.GetDirectoryName(Path.GetFullPath(pakFile));
                outputFolder = Path.Combine(pakDir, "..", "maps");
            }
            else
            {
                // Interactive mode
                Console.Write("Enter pak file path (or press Enter for default): ");
                pakFile = Console.ReadLine();

                if (string.IsNullOrWhiteSpace(pakFile))
                {
                    // Try to find maps.pak automatically
                    string[] possiblePaths = new[]
                    {
                        "Bin/Server/pak/maps.pak",
                        "../Bin/Server/pak/maps.pak",
                        "../../Bin/Server/pak/maps.pak",
                        "pak/maps.pak"
                    };

                    foreach (string path in possiblePaths)
                    {
                        if (File.Exists(path))
                        {
                            pakFile = path;
                            Console.WriteLine($"✓ Found: {Path.GetFullPath(pakFile)}");
                            break;
                        }
                    }

                    if (string.IsNullOrWhiteSpace(pakFile))
                    {
                        Console.WriteLine("❌ Could not find maps.pak automatically.");
                        Console.WriteLine("\nUsage: PakExtractTool <pakfile> [output_folder]");
                        Console.WriteLine("Example: PakExtractTool Bin/Server/pak/maps.pak Bin/Server/maps");
                        return;
                    }
                }

                Console.Write("\nEnter output folder (or press Enter for default 'Bin/Server/maps'): ");
                outputFolder = Console.ReadLine();

                if (string.IsNullOrWhiteSpace(outputFolder))
                {
                    outputFolder = "Bin/Server/maps";
                }
            }

            // Validate inputs
            if (!File.Exists(pakFile))
            {
                Console.WriteLine($"❌ Error: Pak file not found: {pakFile}");
                return;
            }

            // Create output folder
            Directory.CreateDirectory(outputFolder);

            Console.WriteLine($"📦 Pak File: {Path.GetFullPath(pakFile)}");
            Console.WriteLine($"📁 Output:   {Path.GetFullPath(outputFolder)}");
            Console.WriteLine();

            try
            {
                // Open pak file
                Console.WriteLine("Opening pak file...");
                using (PakFileReader reader = new PakFileReader(pakFile))
                {
                    var stats = reader.GetStatistics();
                    Console.WriteLine($"✓ Pak opened successfully");
                    Console.WriteLine($"  Total files: {stats.TotalFiles}");
                    Console.WriteLine($"  Compressed: {stats.CompressedFiles}");
                    Console.WriteLine($"  Uncompressed: {stats.UncompressedFiles}");
                    Console.WriteLine($"  Unknown: {stats.FilesWithoutNames}");
                    Console.WriteLine();

                    // Extract all files
                    Console.WriteLine("Extracting files...");
                    Console.WriteLine("─────────────────────────────────────────────────────");

                    int extractedCount = 0;
                    int errorCount = 0;
                    int skippedCount = 0;

                    var allFiles = reader.GetAllFileNames();

                    foreach (var filename in allFiles)
                    {
                        try
                        {
                            // Read file from pak
                            byte[] data = reader.ReadFile(filename);

                            if (data == null)
                            {
                                Console.WriteLine($"⚠ Skipped (null data): {filename}");
                                skippedCount++;
                                continue;
                            }

                            // Build output path
                            string relativePath = filename.TrimStart('\\', '/');
                            string outputPath = Path.Combine(outputFolder, relativePath);

                            // Create directory if needed
                            string outputDir = Path.GetDirectoryName(outputPath);
                            if (!string.IsNullOrEmpty(outputDir))
                            {
                                Directory.CreateDirectory(outputDir);
                            }

                            // Write file
                            File.WriteAllBytes(outputPath, data);

                            extractedCount++;

                            // Show progress every 10 files
                            if (extractedCount % 10 == 0)
                            {
                                Console.WriteLine($"✓ Extracted {extractedCount}/{allFiles.Count} files...");
                            }
                        }
                        catch (NotImplementedException ex)
                        {
                            // UCL compression not implemented
                            Console.WriteLine($"⚠ Skipped (compressed): {filename}");
                            Console.WriteLine($"  Reason: {ex.Message}");
                            skippedCount++;
                        }
                        catch (Exception ex)
                        {
                            Console.WriteLine($"❌ Error extracting {filename}: {ex.Message}");
                            errorCount++;
                        }
                    }

                    Console.WriteLine("─────────────────────────────────────────────────────");
                    Console.WriteLine();
                    Console.WriteLine("📊 EXTRACTION SUMMARY:");
                    Console.WriteLine($"  ✓ Extracted:  {extractedCount} files");
                    Console.WriteLine($"  ⚠ Skipped:    {skippedCount} files (compressed/null)");
                    Console.WriteLine($"  ❌ Errors:     {errorCount} files");
                    Console.WriteLine($"  📁 Output:     {Path.GetFullPath(outputFolder)}");
                    Console.WriteLine();

                    if (skippedCount > 0)
                    {
                        Console.WriteLine("⚠ NOTE: Some files are compressed and cannot be extracted");
                        Console.WriteLine("  without UCL decompression library (ucl.dll).");
                        Console.WriteLine("  You may need to use the original unpack.exe tool for those files.");
                    }

                    if (extractedCount > 0)
                    {
                        Console.WriteLine("✓ SUCCESS! Files extracted successfully.");
                        Console.WriteLine($"  You can now load maps from: {Path.GetFullPath(outputFolder)}");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"❌ FATAL ERROR: {ex.Message}");
                Console.WriteLine($"Stack trace: {ex.StackTrace}");
            }

            Console.WriteLine();
            Console.WriteLine("Press any key to exit...");
            Console.ReadKey();
        }
    }
}
