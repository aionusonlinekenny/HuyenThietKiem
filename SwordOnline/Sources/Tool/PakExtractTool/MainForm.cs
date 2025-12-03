using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MapTool;
using MapTool.PakFile;

namespace PakExtractTool
{
    public partial class MainForm : Form
    {
        private PakFileReader _currentPakReader;
        private string _currentPakPath;
        private TreeView treeViewFiles;
        private ListView listViewDetails;
        private Button btnOpenPak;
        private Button btnGenerateIndex;
        private Button btnExtractSelected;
        private Button btnExtractAll;
        private StatusStrip statusStrip;
        private ToolStripStatusLabel statusLabel;
        private SplitContainer splitContainer;
        private Label lblPakInfo;

        public MainForm()
        {
            try
            {
                // DebugLogger already initialized in Program.Main()
                DebugLogger.Log("MainForm constructor called");
                DebugLogger.Log($"Log file: {DebugLogger.GetLogFilePath()}");
                DebugLogger.LogSeparator();

                DebugLogger.Log("Initializing GUI components...");
                InitializeComponent();

                DebugLogger.Log("✓ GUI initialized successfully");
            }
            catch (Exception ex)
            {
                DebugLogger.Log($"❌ ERROR in MainForm constructor: {ex.Message}");
                DebugLogger.Log($"   Stack trace: {ex.StackTrace}");
                MessageBox.Show(
                    $"Failed to initialize PAK Extract Tool:\n\n{ex.Message}\n\nStack trace:\n{ex.StackTrace}",
                    "Initialization Error",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
                throw;
            }
        }

        private void InitializeComponent()
        {
            this.Text = "PAK Extract Tool - Huyền Thiết Kiếm";
            this.Size = new Size(1000, 700);
            this.StartPosition = FormStartPosition.CenterScreen;

            // Top panel with buttons and PAK info
            var topPanel = new Panel
            {
                Dock = DockStyle.Top,
                Height = 80,
                Padding = new Padding(10)
            };

            btnOpenPak = new Button
            {
                Text = "📂 Open PAK",
                Location = new Point(10, 10),
                Size = new Size(120, 30),
                Font = new Font("Segoe UI", 9F, FontStyle.Regular)
            };
            btnOpenPak.Click += BtnOpenPak_Click;

            btnGenerateIndex = new Button
            {
                Text = "🔧 Generate Index",
                Location = new Point(140, 10),
                Size = new Size(130, 30),
                Enabled = false,
                Font = new Font("Segoe UI", 9F, FontStyle.Regular),
                BackColor = Color.FromArgb(255, 255, 200)
            };
            btnGenerateIndex.Click += BtnGenerateIndex_Click;

            btnExtractSelected = new Button
            {
                Text = "📤 Extract Selected",
                Location = new Point(280, 10),
                Size = new Size(140, 30),
                Enabled = false,
                Font = new Font("Segoe UI", 9F, FontStyle.Regular)
            };
            btnExtractSelected.Click += BtnExtractSelected_Click;

            btnExtractAll = new Button
            {
                Text = "📦 Extract All",
                Location = new Point(430, 10),
                Size = new Size(120, 30),
                Enabled = false,
                Font = new Font("Segoe UI", 9F, FontStyle.Regular)
            };
            btnExtractAll.Click += BtnExtractAll_Click;

            lblPakInfo = new Label
            {
                Text = "No PAK file loaded. Use 'Open PAK' to load a PAK file, then 'Generate Index' if no file names shown.",
                Location = new Point(10, 45),
                Size = new Size(950, 25),
                Font = new Font("Segoe UI", 9F, FontStyle.Regular),
                ForeColor = Color.Gray
            };

            topPanel.Controls.AddRange(new Control[] { btnOpenPak, btnGenerateIndex, btnExtractSelected, btnExtractAll, lblPakInfo });

            // Split container for tree and details
            splitContainer = new SplitContainer
            {
                Dock = DockStyle.Fill,
                Orientation = Orientation.Horizontal,
                SplitterDistance = 350,
                Panel1MinSize = 200,
                Panel2MinSize = 150
            };

            // TreeView for file hierarchy
            treeViewFiles = new TreeView
            {
                Dock = DockStyle.Fill,
                Font = new Font("Segoe UI", 9F),
                HideSelection = false,
                CheckBoxes = true
            };
            treeViewFiles.AfterSelect += TreeViewFiles_AfterSelect;
            treeViewFiles.AfterCheck += TreeViewFiles_AfterCheck;

            var lblTree = new Label
            {
                Text = "Files in PAK (check to extract):",
                Dock = DockStyle.Top,
                Height = 25,
                Padding = new Padding(5),
                Font = new Font("Segoe UI", 9F, FontStyle.Bold)
            };

            splitContainer.Panel1.Controls.Add(treeViewFiles);
            splitContainer.Panel1.Controls.Add(lblTree);

            // ListView for file details
            listViewDetails = new ListView
            {
                Dock = DockStyle.Fill,
                View = View.Details,
                FullRowSelect = true,
                GridLines = true,
                Font = new Font("Segoe UI", 9F)
            };

            listViewDetails.Columns.Add("Property", 150);
            listViewDetails.Columns.Add("Value", 500);

            var lblDetails = new Label
            {
                Text = "File Details:",
                Dock = DockStyle.Top,
                Height = 25,
                Padding = new Padding(5),
                Font = new Font("Segoe UI", 9F, FontStyle.Bold)
            };

            splitContainer.Panel2.Controls.Add(listViewDetails);
            splitContainer.Panel2.Controls.Add(lblDetails);

            // Status bar
            statusStrip = new StatusStrip();
            statusLabel = new ToolStripStatusLabel("Ready");
            statusStrip.Items.Add(statusLabel);

            // Add all controls to form
            this.Controls.Add(splitContainer);
            this.Controls.Add(topPanel);
            this.Controls.Add(statusStrip);
        }

        private void BtnOpenPak_Click(object sender, EventArgs e)
        {
            try
            {
                DebugLogger.Log("📂 User clicked 'Open PAK File' button");

                using (var dialog = new OpenFileDialog())
                {
                    dialog.Filter = "PAK Files (*.pak)|*.pak|All Files (*.*)|*.*";
                    dialog.Title = "Select PAK File";

                    if (dialog.ShowDialog() == DialogResult.OK)
                    {
                        DebugLogger.Log($"   Selected file: {dialog.FileName}");
                        LoadPakFile(dialog.FileName);
                    }
                    else
                    {
                        DebugLogger.Log("   User cancelled file selection");
                    }
                }
            }
            catch (Exception ex)
            {
                DebugLogger.Log($"❌ ERROR in BtnOpenPak_Click: {ex.Message}");
                DebugLogger.Log($"   Stack trace: {ex.StackTrace}");
                MessageBox.Show($"Error opening file dialog:\n\n{ex.Message}",
                    "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void LoadPakFile(string pakPath)
        {
            try
            {
                DebugLogger.LogSeparator();
                DebugLogger.Log($"🔄 LOADING PAK FILE");
                DebugLogger.Log($"   Path: {pakPath}");
                DebugLogger.Log($"   File exists: {File.Exists(pakPath)}");
                DebugLogger.Log($"   File size: {new FileInfo(pakPath).Length:N0} bytes");

                treeViewFiles.Nodes.Clear();
                listViewDetails.Items.Clear();
                UpdateStatus("Loading PAK file...");

                // Dispose previous reader
                if (_currentPakReader != null)
                {
                    DebugLogger.Log("   Disposing previous PAK reader...");
                    _currentPakReader.Dispose();
                }

                // Load new PAK file
                DebugLogger.Log("   Creating new PakFileReader...");
                _currentPakReader = new PakFileReader(pakPath);
                _currentPakPath = pakPath;
                DebugLogger.Log("   ✓ PakFileReader created successfully");

                DebugLogger.Log("   Getting statistics...");
                var stats = _currentPakReader.GetStatistics();
                DebugLogger.Log($"   ✓ Statistics: {stats.TotalFiles} files, {stats.TotalSize:N0} bytes");

                DebugLogger.Log("   Getting all file names...");
                var allFiles = _currentPakReader.GetAllFileNames();
                DebugLogger.Log($"   ✓ Got {allFiles.Count} named files");

                // Update UI based on whether we have named files
                btnGenerateIndex.Enabled = true;

                if (allFiles.Count > 0)
                {
                    // Has index file
                    lblPakInfo.Text = $"📦 {Path.GetFileName(pakPath)} - " +
                                      $"{stats.TotalFiles:N0} files " +
                                      $"({stats.TotalSize / 1024 / 1024:N1} MB) - " +
                                      $"✓ {allFiles.Count:N0} named files";
                    lblPakInfo.ForeColor = Color.DarkGreen;

                    // Build tree structure
                    DebugLogger.Log("   Building file tree...");
                    BuildFileTree(allFiles);
                    DebugLogger.Log("   ✓ File tree built successfully");

                    // Enable extract buttons
                    btnExtractSelected.Enabled = true;
                    btnExtractAll.Enabled = true;
                }
                else
                {
                    // No index file - show warning
                    lblPakInfo.Text = $"📦 {Path.GetFileName(pakPath)} - " +
                                      $"{stats.TotalFiles:N0} files " +
                                      $"({stats.TotalSize / 1024 / 1024:N1} MB) - " +
                                      $"⚠ No .pak.txt index found! Click 'Generate Index' to create one.";
                    lblPakInfo.ForeColor = Color.DarkOrange;

                    // Highlight generate index button
                    btnGenerateIndex.BackColor = Color.FromArgb(255, 200, 100);

                    DebugLogger.Log("   ⚠ No filename index found (.pak.txt missing)");
                    DebugLogger.Log("   User needs to generate index to see file names");

                    // Don't enable extract buttons without index
                    btnExtractSelected.Enabled = false;
                    btnExtractAll.Enabled = false;
                }

                UpdateStatus($"Loaded {allFiles.Count:N0} files from {Path.GetFileName(pakPath)}");
                DebugLogger.Log($"✓ PAK file loaded successfully");
                DebugLogger.LogSeparator();
            }
            catch (Exception ex)
            {
                DebugLogger.Log($"❌ ERROR loading PAK file: {ex.Message}");
                DebugLogger.Log($"   Exception type: {ex.GetType().FullName}");
                DebugLogger.Log($"   Stack trace: {ex.StackTrace}");
                if (ex.InnerException != null)
                {
                    DebugLogger.Log($"   Inner exception: {ex.InnerException.Message}");
                    DebugLogger.Log($"   Inner stack trace: {ex.InnerException.StackTrace}");
                }
                DebugLogger.LogSeparator();

                MessageBox.Show($"Error loading PAK file:\n\n{ex.Message}\n\nCheck log file for details:\n{DebugLogger.GetLogFilePath()}",
                    "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                UpdateStatus("Error loading PAK file");
            }
        }

        private void BtnGenerateIndex_Click(object sender, EventArgs e)
        {
            try
            {
                if (_currentPakReader == null || string.IsNullOrEmpty(_currentPakPath))
                {
                    MessageBox.Show("Please open a PAK file first!", "No PAK File", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }

                DebugLogger.LogSeparator();
                DebugLogger.Log("🔧 USER CLICKED: Generate Index");

                // Ask user to select scan folder
                using (var dialog = new FolderBrowserDialog())
                {
                    dialog.Description = "Select folder to scan for matching files\n\n" +
                        "For Client PAK files: Select the Client folder\n" +
                        "For Server PAK files: Select the Server folder\n" +
                        "Not sure? Select the game's root folder (parent of Client/Server)";

                    // Try to suggest a default folder
                    string pakFolder = Path.GetDirectoryName(_currentPakPath);
                    if (!string.IsNullOrEmpty(pakFolder))
                    {
                        // Try to go up to parent folder
                        DirectoryInfo pakDir = new DirectoryInfo(pakFolder);
                        if (pakDir.Parent != null && pakDir.Parent.Parent != null)
                        {
                            dialog.SelectedPath = pakDir.Parent.Parent.FullName; // Up 2 levels
                        }
                    }

                    if (dialog.ShowDialog() != DialogResult.OK)
                    {
                        DebugLogger.Log("   User cancelled folder selection");
                        return;
                    }

                    string scanFolder = dialog.SelectedPath;
                    DebugLogger.Log($"   Scan folder: {scanFolder}");

                    // Generate index with progress dialog
                    GenerateIndexWithProgress(scanFolder);
                }
            }
            catch (Exception ex)
            {
                DebugLogger.Log($"❌ ERROR in BtnGenerateIndex_Click: {ex.Message}");
                DebugLogger.Log($"   Stack trace: {ex.StackTrace}");
                MessageBox.Show($"Error generating index:\n\n{ex.Message}",
                    "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void GenerateIndexWithProgress(string scanFolder)
        {
            // Create progress dialog
            var progressForm = new Form
            {
                Text = "Generating PAK Index...",
                Size = new Size(500, 250),
                StartPosition = FormStartPosition.CenterParent,
                FormBorderStyle = FormBorderStyle.FixedDialog,
                MaximizeBox = false,
                MinimizeBox = false
            };

            var lblStep = new Label
            {
                Text = "Initializing...",
                Location = new Point(20, 20),
                Size = new Size(460, 20),
                Font = new Font("Segoe UI", 10F, FontStyle.Bold)
            };

            var lblProgress = new Label
            {
                Text = "",
                Location = new Point(20, 50),
                Size = new Size(460, 20),
                Font = new Font("Segoe UI", 9F)
            };

            var progressBar = new ProgressBar
            {
                Location = new Point(20, 80),
                Size = new Size(460, 25),
                Style = ProgressBarStyle.Continuous
            };

            var lblStats = new Label
            {
                Text = "",
                Location = new Point(20, 115),
                Size = new Size(460, 80),
                Font = new Font("Consolas", 9F)
            };

            var btnClose = new Button
            {
                Text = "Close",
                Location = new Point(200, 170),
                Size = new Size(100, 30),
                Enabled = false
            };
            btnClose.Click += (s, e) => progressForm.Close();

            progressForm.Controls.AddRange(new Control[] { lblStep, lblProgress, progressBar, lblStats, btnClose });

            // Run generation in background
            var worker = new System.ComponentModel.BackgroundWorker
            {
                WorkerReportsProgress = true
            };

            worker.DoWork += (s, e) =>
            {
                try
                {
                    // Get all file IDs from PAK
                    worker.ReportProgress(0, new ProgressData { Step = "Step 1/4: Loading PAK file...", Progress = "" });
                    var pakFileIds = _currentPakReader.GetAllFileIds();

                    // Log some sample hashes for debugging
                    DebugLogger.Log($"📊 PAK contains {pakFileIds.Count} file hashes");
                    DebugLogger.Log($"   Sample hashes (first 10):");
                    for (int i = 0; i < Math.Min(10, pakFileIds.Count); i++)
                    {
                        DebugLogger.Log($"      0x{pakFileIds[i]:X8}");
                    }

                    worker.ReportProgress(10, new ProgressData { Step = "Step 2/4: Scanning folder for files...", Progress = $"Found {pakFileIds.Count} hashes in PAK" });
                    var allFiles = ScanFolderRecursive(scanFolder, worker);

                    DebugLogger.Log($"📂 Scanned {allFiles.Count:N0} files from: {scanFolder}");

                    // Log sample file paths for debugging
                    DebugLogger.Log($"   Sample scanned files (first 10):");
                    for (int i = 0; i < Math.Min(10, allFiles.Count); i++)
                    {
                        DebugLogger.Log($"      {allFiles[i]}");
                    }

                    worker.ReportProgress(40, new ProgressData { Step = "Step 3/4: Matching file paths with PAK hashes...", Progress = $"Scanning {allFiles.Count:N0} files..." });
                    var matches = MatchFilesWithPak(allFiles, pakFileIds, scanFolder, worker);

                    DebugLogger.Log($"🎯 Match result: {matches.Count:N0}/{pakFileIds.Count:N0} files matched");

                    worker.ReportProgress(90, new ProgressData { Step = "Step 4/4: Generating .pak.txt file...", Progress = $"Matched {matches.Count:N0}/{pakFileIds.Count:N0} files" });
                    string txtFile = _currentPakPath + ".txt";
                    GeneratePakTxtFile(txtFile, matches);

                    e.Result = new GenerateIndexResult { TotalPakFiles = pakFileIds.Count, MatchedFiles = matches.Count, TxtFile = txtFile };
                }
                catch (Exception ex)
                {
                    e.Result = ex;
                }
            };

            worker.ProgressChanged += (s, e) =>
            {
                var data = e.UserState as ProgressData;
                if (data != null)
                {
                    lblStep.Text = data.Step;
                    lblProgress.Text = data.Progress;
                }
                progressBar.Value = e.ProgressPercentage;
            };

            worker.RunWorkerCompleted += (s, e) =>
            {
                if (e.Result is Exception ex)
                {
                    lblStep.Text = "❌ Error occurred!";
                    lblProgress.Text = ex.Message;
                    lblStats.Text = $"Error: {ex.Message}\n\nSee log file for details.";
                    lblStats.ForeColor = Color.Red;
                    DebugLogger.Log($"❌ ERROR generating index: {ex.Message}");
                    DebugLogger.Log($"   Stack trace: {ex.StackTrace}");
                }
                else if (e.Result is GenerateIndexResult result)
                {
                    int totalPakFiles = result.TotalPakFiles;
                    int matchedFiles = result.MatchedFiles;
                    string txtFile = result.TxtFile;
                    double matchRate = (double)matchedFiles / totalPakFiles * 100;

                    lblStep.Text = "✓ Index generation complete!";
                    lblProgress.Text = $"Generated: {Path.GetFileName(txtFile)}";
                    lblStats.Text = $"Total PAK files:  {totalPakFiles:N0}\n" +
                                    $"Matched files:    {matchedFiles:N0}\n" +
                                    $"Unmatched files:  {(totalPakFiles - matchedFiles):N0}\n" +
                                    $"Match rate:       {matchRate:F1}%";
                    lblStats.ForeColor = matchRate > 50 ? Color.DarkGreen : Color.DarkOrange;
                    progressBar.Value = 100;

                    DebugLogger.Log($"✓ Index generation complete!");
                    DebugLogger.Log($"   Generated: {txtFile}");
                    DebugLogger.Log($"   Match rate: {matchRate:F1}%");

                    // Auto-reload PAK to show new index
                    MessageBox.Show(
                        $"Index file generated successfully!\n\n" +
                        $"Match rate: {matchRate:F1}% ({matchedFiles:N0}/{totalPakFiles:N0} files)\n\n" +
                        $"The PAK will now reload to show file names.",
                        "Success",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Information);

                    progressForm.Close();

                    // Reload PAK file
                    LoadPakFile(_currentPakPath);
                }

                btnClose.Enabled = true;
            };

            worker.RunWorkerAsync();
            progressForm.ShowDialog(this);
        }

        private List<string> ScanFolderRecursive(string folder, System.ComponentModel.BackgroundWorker worker)
        {
            var paths = new HashSet<string>();

            try
            {
                DebugLogger.Log($"📂 Scanning source files for path references...");

                // Scan text-based source files
                var sourceExtensions = new[] { "*.txt", "*.ini", "*.cpp", "*.h", "*.lua", "*.c" };
                var allFiles = new List<string>();

                foreach (var ext in sourceExtensions)
                {
                    allFiles.AddRange(Directory.GetFiles(folder, ext, SearchOption.AllDirectories));
                }

                DebugLogger.Log($"   Found {allFiles.Count:N0} source files to parse");

                // Regex patterns to find file paths
                var pathPatterns = new[]
                {
                    @"\\[Ss]pr\\[^\s""']+\.spr",           // \Spr\...\file.spr
                    @"\\[Ss]ettings\\[^\s""']+\.(ini|txt)", // \Settings\...\file.ini
                    @"\\[Mm]aps\\[^\s""']+\.(dat|wor)",    // \Maps\...\file.dat
                    @"\\[Uu]i\\[^\s""']+\.(jpg|bmp|tga|spr)", // \Ui\...\file.jpg
                    @"\\[^\s""'\\]+\\[^\s""']+\.(spr|ini|txt|dat|wor|jpg|bmp|tga)", // Generic: \folder\...\file.ext
                };

                for (int i = 0; i < allFiles.Count; i++)
                {
                    string filePath = allFiles[i];

                    try
                    {
                        // Read file content with GB2312 encoding
                        string content = File.ReadAllText(filePath, Encoding.GetEncoding("GB2312"));

                        // Extract all path references
                        foreach (var pattern in pathPatterns)
                        {
                            var matches = System.Text.RegularExpressions.Regex.Matches(content, pattern);
                            foreach (System.Text.RegularExpressions.Match match in matches)
                            {
                                string path = match.Value;
                                // Normalize path
                                path = path.Replace('/', '\\');
                                if (!path.StartsWith("\\"))
                                    path = "\\" + path;

                                paths.Add(path);
                            }
                        }
                    }
                    catch
                    {
                        // Skip files that can't be read
                    }

                    if (i % 100 == 0)
                    {
                        worker.ReportProgress(20 + (i * 20 / allFiles.Count),
                            new ProgressData { Step = "Step 2/4: Extracting path references from source files...", Progress = $"Parsed {i:N0}/{allFiles.Count:N0} files - Found {paths.Count:N0} unique paths" });
                    }
                }

                DebugLogger.Log($"   ✓ Extracted {paths.Count:N0} unique path references from source code");
            }
            catch (Exception ex)
            {
                DebugLogger.Log($"⚠ Warning scanning folder: {ex.Message}");
            }

            return paths.ToList();
        }

        private Dictionary<uint, string> MatchFilesWithPak(List<string> pathReferences, List<uint> pakFileIds, string scanFolder, System.ComponentModel.BackgroundWorker worker)
        {
            var matches = new Dictionary<uint, string>();
            var pakHashSet = new HashSet<uint>(pakFileIds);

            DebugLogger.Log($"🔍 Matching {pathReferences.Count:N0} path references with PAK hashes...");

            // Log first 10 sample paths
            if (pathReferences.Count > 0)
            {
                DebugLogger.Log($"   Sample path references (first 10):");
                for (int i = 0; i < Math.Min(10, pathReferences.Count); i++)
                {
                    DebugLogger.Log($"      {pathReferences[i]}");
                }
            }

            for (int i = 0; i < pathReferences.Count; i++)
            {
                string path = pathReferences[i];

                // Calculate hash for this path
                uint hash = FileNameHasher.CalculateFileId(path);

                // Check if this hash exists in PAK
                if (pakHashSet.Contains(hash) && !matches.ContainsKey(hash))
                {
                    matches[hash] = path;

                    if (matches.Count <= 20)
                    {
                        // Log first 20 matches for debugging
                        DebugLogger.Log($"   ✓ Match #{matches.Count}: {path} -> 0x{hash:X8}");
                    }
                }

                if (i % 1000 == 0)
                {
                    worker.ReportProgress(40 + (i * 50 / pathReferences.Count),
                        new ProgressData { Step = "Step 3/4: Matching paths with PAK...", Progress = $"Checked {i:N0}/{pathReferences.Count:N0} - Matched {matches.Count:N0}" });
                }
            }

            DebugLogger.Log($"   ✓ Final match result: {matches.Count:N0}/{pakFileIds.Count:N0} ({(double)matches.Count / pakFileIds.Count * 100:F1}%)");

            return matches;
        }

        private string GetRelativePath(string basePath, string fullPath)
        {
            Uri baseUri = new Uri(basePath.TrimEnd('\\', '/') + "\\");
            Uri fullUri = new Uri(fullPath);
            return Uri.UnescapeDataString(baseUri.MakeRelativeUri(fullUri).ToString());
        }

        private void GeneratePakTxtFile(string txtFile, Dictionary<uint, string> matches)
        {
            using (StreamWriter writer = new StreamWriter(txtFile, false, Encoding.GetEncoding("GB2312")))
            {
                // Write header line 1 - matching original format
                string pakTime = DateTime.Now.ToString("yyyy-M-d H:m:s");
                string pakTimeSave = ((uint)DateTime.Now.Ticks).ToString("x");
                writer.WriteLine($"TotalFile:{matches.Count}\tPakTime:{pakTime}\tPakTimeSave:{pakTimeSave}\tCRC:00000000");

                // Write header line 2 - column names
                writer.WriteLine("Index\tID\tTime\tFileName\tSize\tInPakSize\tComprFlag\tCRC");

                // Write each match
                int index = 0;
                foreach (var kvp in matches.OrderBy(x => x.Value))
                {
                    uint hash = kvp.Key;
                    string fileName = kvp.Value;
                    string fileTime = "2000-1-1 0:0:0";
                    writer.WriteLine($"{index}\t{hash:x}\t{fileTime}\t{fileName}\t0\t0\t0\t0");
                    index++;
                }
            }
        }

        private void BuildFileTree(List<string> allFiles)
        {
            treeViewFiles.BeginUpdate();
            treeViewFiles.Nodes.Clear();

            // Create root node
            var rootNode = new TreeNode("Root")
            {
                Tag = null
            };
            treeViewFiles.Nodes.Add(rootNode);

            // Group files by directory
            var filesByDir = new Dictionary<string, List<string>>();

            foreach (var filePath in allFiles.OrderBy(f => f))
            {
                var normalizedPath = filePath.TrimStart('\\', '/').Replace('/', '\\');
                var dir = Path.GetDirectoryName(normalizedPath) ?? "";

                if (!filesByDir.ContainsKey(dir))
                {
                    filesByDir[dir] = new List<string>();
                }
                filesByDir[dir].Add(normalizedPath);
            }

            // Build tree hierarchy
            foreach (var kvp in filesByDir.OrderBy(k => k.Key))
            {
                var dirPath = kvp.Key;
                var files = kvp.Value;

                // Find or create directory node
                TreeNode parentNode = rootNode;

                if (!string.IsNullOrEmpty(dirPath))
                {
                    var dirParts = dirPath.Split('\\');
                    foreach (var part in dirParts)
                    {
                        var existingNode = parentNode.Nodes.Cast<TreeNode>()
                            .FirstOrDefault(n => n.Text == part && n.Tag == null);

                        if (existingNode == null)
                        {
                            existingNode = new TreeNode(part)
                            {
                                Tag = null, // null = directory
                                ImageIndex = 0
                            };
                            parentNode.Nodes.Add(existingNode);
                        }
                        parentNode = existingNode;
                    }
                }

                // Add files to this directory
                foreach (var file in files)
                {
                    var fileName = Path.GetFileName(file);
                    var fileNode = new TreeNode(fileName)
                    {
                        Tag = "\\" + file, // Full path with leading backslash
                        ImageIndex = 1
                    };
                    parentNode.Nodes.Add(fileNode);
                }
            }

            rootNode.Expand();
            treeViewFiles.EndUpdate();
        }

        private void TreeViewFiles_AfterSelect(object sender, TreeViewEventArgs e)
        {
            listViewDetails.Items.Clear();

            if (e.Node.Tag == null) // Directory node
            {
                // Show directory info
                int fileCount = CountFiles(e.Node);
                AddDetail("Type", "Directory");
                AddDetail("Path", GetNodePath(e.Node));
                AddDetail("Files", fileCount.ToString("N0"));
            }
            else // File node
            {
                string filePath = e.Node.Tag.ToString();
                ShowFileDetails(filePath);
            }
        }

        private void TreeViewFiles_AfterCheck(object sender, TreeViewEventArgs e)
        {
            // Prevent recursive calls
            if (e.Action != TreeViewAction.Unknown)
            {
                CheckAllChildNodes(e.Node, e.Node.Checked);
                UpdateParentNodeCheck(e.Node);
            }

            UpdateSelectedFilesCount();
        }

        private void CheckAllChildNodes(TreeNode node, bool isChecked)
        {
            foreach (TreeNode child in node.Nodes)
            {
                child.Checked = isChecked;
                CheckAllChildNodes(child, isChecked);
            }
        }

        private void UpdateParentNodeCheck(TreeNode node)
        {
            if (node.Parent == null) return;

            bool allChecked = node.Parent.Nodes.Cast<TreeNode>().All(n => n.Checked);
            bool anyChecked = node.Parent.Nodes.Cast<TreeNode>().Any(n => n.Checked);

            if (allChecked)
            {
                node.Parent.Checked = true;
            }
            else if (!anyChecked)
            {
                node.Parent.Checked = false;
            }

            UpdateParentNodeCheck(node.Parent);
        }

        private void ShowFileDetails(string filePath)
        {
            try
            {
                var fileInfo = _currentPakReader.GetFileInfo(filePath);
                if (fileInfo != null)
                {
                    AddDetail("Type", "File");
                    AddDetail("Path", filePath);
                    AddDetail("File ID", $"0x{fileInfo.FileId:X8}");
                    AddDetail("Size", $"{fileInfo.Size:N0} bytes ({fileInfo.Size / 1024.0:N2} KB)");
                    AddDetail("Compressed Size", $"{fileInfo.CompressedSize:N0} bytes ({fileInfo.CompressedSize / 1024.0:N2} KB)");
                    AddDetail("Compression", fileInfo.IsCompressed ?
                        $"{fileInfo.CompressionMethod} ({fileInfo.CompressionRatio:F1}%)" : "None");
                }
            }
            catch (Exception ex)
            {
                AddDetail("Error", ex.Message);
            }
        }

        private void AddDetail(string property, string value)
        {
            listViewDetails.Items.Add(new ListViewItem(new[] { property, value }));
        }

        private string GetNodePath(TreeNode node)
        {
            var path = new List<string>();
            var current = node;
            while (current != null && current.Parent != null)
            {
                path.Insert(0, current.Text);
                current = current.Parent;
            }
            return string.Join("\\", path);
        }

        private int CountFiles(TreeNode node)
        {
            int count = 0;
            foreach (TreeNode child in node.Nodes)
            {
                if (child.Tag != null) // File
                {
                    count++;
                }
                else // Directory
                {
                    count += CountFiles(child);
                }
            }
            return count;
        }

        private void UpdateSelectedFilesCount()
        {
            int selectedCount = CountCheckedFiles(treeViewFiles.Nodes[0]);
            btnExtractSelected.Text = selectedCount > 0 ?
                $"📤 Extract Selected ({selectedCount})" :
                "📤 Extract Selected";
        }

        private int CountCheckedFiles(TreeNode node)
        {
            int count = 0;
            foreach (TreeNode child in node.Nodes)
            {
                if (child.Checked && child.Tag != null) // File
                {
                    count++;
                }
                count += CountCheckedFiles(child);
            }
            return count;
        }

        private void BtnExtractSelected_Click(object sender, EventArgs e)
        {
            var checkedFiles = GetCheckedFiles(treeViewFiles.Nodes[0]);
            if (checkedFiles.Count == 0)
            {
                MessageBox.Show("Please select files to extract by checking them in the tree.",
                    "No Files Selected", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            ExtractFiles(checkedFiles);
        }

        private void BtnExtractAll_Click(object sender, EventArgs e)
        {
            var allFiles = _currentPakReader.GetAllFileNames();
            ExtractFiles(allFiles);
        }

        private List<string> GetCheckedFiles(TreeNode node)
        {
            var files = new List<string>();
            foreach (TreeNode child in node.Nodes)
            {
                if (child.Checked && child.Tag != null) // File
                {
                    files.Add(child.Tag.ToString());
                }
                files.AddRange(GetCheckedFiles(child));
            }
            return files;
        }

        private void ExtractFiles(List<string> filesToExtract)
        {
            using (var dialog = new FolderBrowserDialog())
            {
                dialog.Description = "Select output folder for extracted files";
                dialog.ShowNewFolderButton = true;

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    ExtractFilesToFolder(filesToExtract, dialog.SelectedPath);
                }
            }
        }

        private void ExtractFilesToFolder(List<string> files, string outputFolder)
        {
            var progressForm = new ProgressForm();
            progressForm.Show(this);

            int extracted = 0;
            int skipped = 0;
            int errors = 0;

            try
            {
                foreach (var filePath in files)
                {
                    try
                    {
                        progressForm.UpdateProgress(extracted + skipped + errors, files.Count,
                            $"Extracting: {Path.GetFileName(filePath)}");
                        Application.DoEvents();

                        byte[] data = _currentPakReader.ReadFile(filePath);

                        if (data == null)
                        {
                            skipped++;
                            continue;
                        }

                        string relativePath = filePath.TrimStart('\\', '/');
                        string outputPath = Path.Combine(outputFolder, relativePath);
                        string outputDir = Path.GetDirectoryName(outputPath);

                        if (!string.IsNullOrEmpty(outputDir))
                        {
                            Directory.CreateDirectory(outputDir);
                        }

                        File.WriteAllBytes(outputPath, data);
                        extracted++;
                    }
                    catch (NotImplementedException)
                    {
                        skipped++;
                    }
                    catch (Exception)
                    {
                        errors++;
                    }
                }

                progressForm.Close();

                string message = $"Extraction complete!\n\n" +
                                 $"✓ Extracted: {extracted:N0} files\n" +
                                 $"⚠ Skipped: {skipped:N0} files (compressed)\n" +
                                 $"❌ Errors: {errors:N0} files\n\n" +
                                 $"Output: {outputFolder}";

                MessageBox.Show(message, "Extraction Complete",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);

                UpdateStatus($"Extracted {extracted:N0} files to {outputFolder}");
            }
            catch (Exception ex)
            {
                progressForm.Close();
                MessageBox.Show($"Error during extraction:\n\n{ex.Message}",
                    "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void UpdateStatus(string message)
        {
            statusLabel.Text = $"{DateTime.Now:HH:mm:ss} - {message}";
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            _currentPakReader?.Dispose();
            base.OnFormClosing(e);
        }
    }

    // Progress data class for BackgroundWorker
    internal class ProgressData
    {
        public string Step { get; set; }
        public string Progress { get; set; }
    }

    // Result data class for BackgroundWorker completion
    internal class GenerateIndexResult
    {
        public int TotalPakFiles { get; set; }
        public int MatchedFiles { get; set; }
        public string TxtFile { get; set; }
    }

    // Simple progress form
    public class ProgressForm : Form
    {
        private ProgressBar progressBar;
        private Label lblStatus;

        public ProgressForm()
        {
            this.Text = "Extracting Files...";
            this.Size = new Size(500, 120);
            this.StartPosition = FormStartPosition.CenterParent;
            this.FormBorderStyle = FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;

            lblStatus = new Label
            {
                Text = "Preparing...",
                Location = new Point(10, 10),
                Size = new Size(460, 20)
            };

            progressBar = new ProgressBar
            {
                Location = new Point(10, 40),
                Size = new Size(460, 25),
                Style = ProgressBarStyle.Continuous
            };

            this.Controls.Add(lblStatus);
            this.Controls.Add(progressBar);
        }

        public void UpdateProgress(int current, int total, string status)
        {
            progressBar.Maximum = total;
            progressBar.Value = Math.Min(current, total);
            lblStatus.Text = $"{current}/{total} - {status}";
        }
    }
}
