using System;
using System.IO;
using System.Text;
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
        private static string _logFilePath;

        static void Main(string[] args)
        {
            // Initialize log file FIRST
            InitializeLog();

            try
            {
                RunExtraction(args);
            }
            catch (Exception ex)
            {
                LogError("FATAL EXCEPTION", ex);
                Console.WriteLine();
                Console.WriteLine("❌ FATAL ERROR! See log file for details:");
                Console.WriteLine($"   {_logFilePath}");
            }
            finally
            {
                Console.WriteLine();
                Console.WriteLine("═══════════════════════════════════════════════════════");
                Console.WriteLine($"📄 Log file saved: {_logFilePath}");
                Console.WriteLine("═══════════════════════════════════════════════════════");
                Console.WriteLine();
                Console.WriteLine("Press any key to exit...");
                Console.ReadKey();
            }
        }

        private static void InitializeLog()
        {
            try
            {
                string exeDir = AppDomain.CurrentDomain.BaseDirectory;
                string timestamp = DateTime.Now.ToString("yyyyMMdd_HHmmss");
                _logFilePath = Path.Combine(exeDir, $"PakExtract_Log_{timestamp}.log");

                Log("═══════════════════════════════════════════════════════");
                Log("  PAK EXTRACT TOOL - Huyền Thiết Kiếm");
                Log("═══════════════════════════════════════════════════════");
                Log($"Started: {DateTime.Now}");
                Log($"Executable: {System.Reflection.Assembly.GetExecutingAssembly().Location}");
                Log($"Working Directory: {Environment.CurrentDirectory}");
                Log($"Log File: {_logFilePath}");
                Log("");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"⚠ Warning: Could not create log file: {ex.Message}");
                _logFilePath = "ERROR_CREATING_LOG.txt";
            }
        }

        private static void Log(string message)
        {
            try
            {
                string timestamp = DateTime.Now.ToString("HH:mm:ss.fff");
                string logLine = $"[{timestamp}] {message}\n";
                File.AppendAllText(_logFilePath, logLine, Encoding.UTF8);
                Console.WriteLine(message);
            }
            catch
            {
                Console.WriteLine(message);
            }
        }

        private static void LogError(string context, Exception ex)
        {
            Log($"❌ ERROR: {context}");
            Log($"   Message: {ex.Message}");
            Log($"   Type: {ex.GetType().FullName}");
            Log($"   Stack Trace:");
            foreach (string line in ex.StackTrace.Split('\n'))
            {
                Log($"     {line.Trim()}");
            }
            if (ex.InnerException != null)
            {
                Log($"   Inner Exception: {ex.InnerException.Message}");
            }
        }

        private static void RunExtraction(string[] args)
        {
            Console.OutputEncoding = System.Text.Encoding.UTF8;
            Log("═══════════════════════════════════════════════════════");
            Log("  PAK EXTRACT TOOL - Huyền Thiết Kiếm");
            Log("═══════════════════════════════════════════════════════");
            Log("");

            // Parse arguments
            string pakFile;
            string outputFolder;

            Log($"Arguments: {args.Length} provided");
            for (int i = 0; i < args.Length; i++)
            {
                Log($"  args[{i}] = {args[i]}");
            }
            Log("");

            if (args.Length >= 2)
            {
                pakFile = args[0];
                outputFolder = args[1];
                Log($"Using command line arguments");
            }
            else if (args.Length == 1)
            {
                pakFile = args[0];
                // Default output folder: same location as pak file, but named 'extracted'
                string pakDir = Path.GetDirectoryName(Path.GetFullPath(pakFile));
                outputFolder = Path.Combine(pakDir, "..", "maps");
                Log($"Using single argument with default output folder");
            }
            else
            {
                // Interactive mode
                Log("Interactive mode - no arguments provided");
                Console.Write("Enter pak file path (or press Enter for default): ");
                pakFile = Console.ReadLine();
                Log($"User input pak file: '{pakFile}'");

                if (string.IsNullOrWhiteSpace(pakFile))
                {
                    // Try to find maps.pak automatically
                    Log("Searching for maps.pak in common locations...");
                    string[] possiblePaths = new[]
                    {
                        "pak/maps.pak",
                        "Bin/Server/pak/maps.pak",
                        "../pak/maps.pak",
                        "../Bin/Server/pak/maps.pak",
                        "../../Bin/Server/pak/maps.pak"
                    };

                    foreach (string path in possiblePaths)
                    {
                        Log($"  Checking: {path}");
                        if (File.Exists(path))
                        {
                            pakFile = path;
                            Log($"✓ Found: {Path.GetFullPath(pakFile)}");
                            break;
                        }
                    }

                    if (string.IsNullOrWhiteSpace(pakFile))
                    {
                        Log("❌ Could not find maps.pak automatically.");
                        Log("\nUsage: PakExtractTool <pakfile> [output_folder]");
                        Log("Example: PakExtractTool Bin/Server/pak/maps.pak Bin/Server/maps");
                        throw new FileNotFoundException("maps.pak not found in any common location");
                    }
                }

                Console.Write("\nEnter output folder (or press Enter for default 'Bin/Server/maps'): ");
                outputFolder = Console.ReadLine();
                Log($"User input output folder: '{outputFolder}'");

                if (string.IsNullOrWhiteSpace(outputFolder))
                {
                    outputFolder = "Bin/Server/maps";
                    Log($"Using default output folder: {outputFolder}");
                }
            }

            // Validate inputs
            Log("");
            Log("Validating inputs...");
            if (!File.Exists(pakFile))
            {
                Log($"❌ Error: Pak file not found: {pakFile}");
                throw new FileNotFoundException($"Pak file not found: {pakFile}");
            }

            // Create output folder
            Log($"Creating output directory: {outputFolder}");
            Directory.CreateDirectory(outputFolder);

            Log("");
            Log($"📦 Pak File: {Path.GetFullPath(pakFile)}");
            Log($"📁 Output:   {Path.GetFullPath(outputFolder)}");
            Log("");

            try
            {
                // Open pak file
                Log("Opening pak file...");
                using (PakFileReader reader = new PakFileReader(pakFile))
                {
                    var stats = reader.GetStatistics();
                    Log($"✓ Pak opened successfully");
                    Log($"  Total files: {stats.TotalFiles}");
                    Log($"  Compressed: {stats.CompressedFiles}");
                    Log($"  Uncompressed: {stats.UncompressedFiles}");
                    Log($"  Files with names: {stats.TotalFiles - stats.FilesWithoutNames}");
                    Log($"  Unknown files: {stats.FilesWithoutNames}");
                    Log("");

                    // Extract all files
                    Log("Extracting files...");
                    Log("─────────────────────────────────────────────────────");

                    int extractedCount = 0;
                    int errorCount = 0;
                    int skippedCount = 0;

                    var allFiles = reader.GetAllFileNames();
                    Log($"Found {allFiles.Count} files with known names");
                    Log("");

                    foreach (var filename in allFiles)
                    {
                        try
                        {
                            // Read file from pak
                            byte[] data = reader.ReadFile(filename);

                            if (data == null)
                            {
                                Log($"⚠ Skipped (null data): {filename}");
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
                                Log($"✓ Extracted {extractedCount}/{allFiles.Count} files...");
                            }
                        }
                        catch (NotImplementedException ex)
                        {
                            // UCL compression not implemented
                            Log($"⚠ Skipped (compressed): {filename}");
                            Log($"  Reason: {ex.Message}");
                            skippedCount++;
                        }
                        catch (Exception ex)
                        {
                            Log($"❌ Error extracting {filename}: {ex.Message}");
                            errorCount++;
                        }
                    }

                    Log("─────────────────────────────────────────────────────");
                    Log("");
                    Log("📊 EXTRACTION SUMMARY:");
                    Log($"  ✓ Extracted:  {extractedCount} files");
                    Log($"  ⚠ Skipped:    {skippedCount} files (compressed/null)");
                    Log($"  ❌ Errors:     {errorCount} files");
                    Log($"  📁 Output:     {Path.GetFullPath(outputFolder)}");
                    Log("");

                    if (skippedCount > 0)
                    {
                        Log("⚠ NOTE: Some files are compressed and cannot be extracted");
                        Log("  without UCL decompression library (ucl.dll).");
                        Log("  You may need to use the original unpack.exe tool for those files.");
                        Log("");
                    }

                    if (extractedCount > 0)
                    {
                        Log("✓ SUCCESS! Files extracted successfully.");
                        Log($"  You can now load maps from: {Path.GetFullPath(outputFolder)}");
                    }
                    else
                    {
                        Log("⚠ WARNING: No files were extracted!");
                        Log("  All files might be compressed or there might be an issue.");
                    }
                }
            }
            catch (Exception ex)
            {
                LogError("Exception during extraction", ex);
                throw; // Re-throw to be caught by outer try-catch
            }
        }
    }
}
