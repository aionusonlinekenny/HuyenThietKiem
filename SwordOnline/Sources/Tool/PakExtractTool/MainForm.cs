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
        private List<string> _lastGeneratedPaths; // Store paths from last generation
        private TreeView treeViewFiles;
        private ListView listViewDetails;
        private Button btnOpenPak;
        private Button btnGenerateIndex;
        private Button btnExportPaths;
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

            btnExportPaths = new Button
            {
                Text = "💾 Export Paths",
                Location = new Point(280, 10),
                Size = new Size(130, 30),
                Enabled = false,
                Font = new Font("Segoe UI", 9F, FontStyle.Regular),
                BackColor = Color.FromArgb(200, 230, 255)
            };
            btnExportPaths.Click += BtnExportPaths_Click;

            btnExtractSelected = new Button
            {
                Text = "📤 Extract Selected",
                Location = new Point(420, 10),
                Size = new Size(140, 30),
                Enabled = false,
                Font = new Font("Segoe UI", 9F, FontStyle.Regular)
            };
            btnExtractSelected.Click += BtnExtractSelected_Click;

            btnExtractAll = new Button
            {
                Text = "📦 Extract All",
                Location = new Point(570, 10),
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

            topPanel.Controls.AddRange(new Control[] { btnOpenPak, btnGenerateIndex, btnExportPaths, btnExtractSelected, btnExtractAll, lblPakInfo });

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

                    // Store paths for export functionality
                    _lastGeneratedPaths = allFiles;

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

                    // Enable export button now that paths are generated
                    btnExportPaths.Enabled = true;

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

        /// <summary>
        /// Parse INI files with Count/Path/enumeration format
        /// Example: ChatPics.ini has Count=911, Path=\spr\Ui3\聊天, 0=01.spr, 1=02.spr, etc.
        /// </summary>
        private void ParseIniFileForPaths(string filePath, HashSet<string> paths)
        {
            try
            {
                string basePath = null;
                int count = 0;

                var lines = File.ReadAllLines(filePath, Encoding.GetEncoding("GBK"));

                foreach (var line in lines)
                {
                    var trimmed = line.Trim();

                    // Parse Count=
                    if (trimmed.StartsWith("Count=", StringComparison.OrdinalIgnoreCase))
                    {
                        int.TryParse(trimmed.Substring(6), out count);
                    }
                    // Parse Path=
                    else if (trimmed.StartsWith("Path=", StringComparison.OrdinalIgnoreCase))
                    {
                        basePath = trimmed.Substring(5).Trim();
                    }
                    // Parse numeric entries like 0=01.spr, 1=02.spr
                    else if (trimmed.Contains("=") && !trimmed.StartsWith("["))
                    {
                        var parts = trimmed.Split('=');
                        if (parts.Length == 2 && int.TryParse(parts[0], out _))
                        {
                            string fileName = parts[1].Trim();
                            if (!string.IsNullOrEmpty(basePath) && !string.IsNullOrEmpty(fileName))
                            {
                                string fullPath = basePath + "\\" + fileName;
                                fullPath = fullPath.Replace('/', '\\');
                                if (!fullPath.StartsWith("\\")) fullPath = "\\" + fullPath;
                                fullPath = LowercaseAsciiOnly(fullPath);
                                paths.Add(fullPath);
                            }
                        }
                    }
                }

                // If we found Count but no enumerated entries, generate them
                if (count > 0 && !string.IsNullOrEmpty(basePath))
                {
                    for (int i = 0; i <= count; i++)
                    {
                        // Try common patterns: 01.spr, 001.spr, icon_01.spr, etc.
                        var patterns = new[] {
                            $"{i:D2}.spr",      // 01.spr, 02.spr
                            $"{i:D3}.spr",      // 001.spr, 002.spr
                            $"{i:D4}.spr",      // 0001.spr, 0002.spr
                            $"icon_{i:D2}.spr", // icon_01.spr
                            $"pic_{i:D2}.spr",  // pic_01.spr
                        };

                        foreach (var pattern in patterns)
                        {
                            string fullPath = basePath + "\\" + pattern;
                            fullPath = fullPath.Replace('/', '\\');
                            if (!fullPath.StartsWith("\\")) fullPath = "\\" + fullPath;
                            fullPath = LowercaseAsciiOnly(fullPath);
                            paths.Add(fullPath);
                        }
                    }
                }
            }
            catch
            {
                // Skip files that can't be parsed
            }
        }

        /// <summary>
        /// Parse Settings TXT files with tabular format containing sprite paths
        /// Example: NpcNormalRes.txt has columns with sprite filenames
        /// </summary>
        private void ParseSettingsTxtForPaths(string filePath, HashSet<string> paths)
        {
            try
            {
                // Check if this is Skills.txt or Missles.txt - they have special column structure
                string fileName = Path.GetFileName(filePath).ToLower();

                if (fileName == "skills.txt")
                {
                    ParseSkillsTxt(filePath, paths);
                    return;
                }
                else if (fileName == "missles.txt")
                {
                    ParseMisslesTxt(filePath, paths);
                    return;
                }

                // Generic parsing for other settings files
                var lines = File.ReadAllLines(filePath, Encoding.GetEncoding("GBK"));

                foreach (var line in lines)
                {
                    // Skip comments and empty lines
                    if (string.IsNullOrWhiteSpace(line) || line.TrimStart().StartsWith("//") || line.TrimStart().StartsWith("#"))
                        continue;

                    // Split by tab or multiple spaces
                    var fields = line.Split(new[] { '\t', ' ' }, StringSplitOptions.RemoveEmptyEntries);

                    foreach (var field in fields)
                    {
                        var trimmed = field.Trim();

                        // Check if field looks like a sprite path or filename
                        if (trimmed.Contains(".spr") || trimmed.Contains(".ini") || trimmed.Contains(".txt"))
                        {
                            string path = trimmed;

                            // If it's just a filename, try to infer the directory from the settings file path
                            if (!path.Contains("\\") && !path.Contains("/"))
                            {
                                // For NpcRes files, sprites are usually in \spr\npcres\...
                                if (filePath.Contains("NpcRes") || filePath.Contains("npcres"))
                                {
                                    // Try to extract NPC ID or name from the filename
                                    // Format: enemy003_st.spr -> \spr\npcres\enemy\enemy003\enemy003_st.spr
                                    var match = System.Text.RegularExpressions.Regex.Match(trimmed, @"([a-z]+)(\d+)_");
                                    if (match.Success)
                                    {
                                        string npcType = match.Groups[1].Value;  // enemy, npc, boss, etc.
                                        string npcNum = match.Groups[2].Value;   // 003
                                        string npcId = npcType + npcNum;         // enemy003
                                        path = $"\\spr\\npcres\\{npcType}\\{npcId}\\{trimmed}";
                                    }
                                    else
                                    {
                                        path = $"\\spr\\npcres\\{trimmed}";
                                    }
                                }
                                else if (filePath.Contains("Item") || filePath.Contains("item"))
                                {
                                    path = $"\\spr\\item\\{trimmed}";
                                }
                                else if (filePath.Contains("Skill") || filePath.Contains("skill"))
                                {
                                    path = $"\\spr\\skill\\{trimmed}";
                                }
                                else
                                {
                                    path = $"\\spr\\{trimmed}";
                                }
                            }

                            path = path.Replace('/', '\\');
                            if (!path.StartsWith("\\")) path = "\\" + path;
                            path = LowercaseAsciiOnly(path);
                            paths.Add(path);

                            // For NPC sprites, also generate action variations
                            if (path.Contains("\\npcres\\") && path.EndsWith(".spr"))
                            {
                                // Generate common action suffixes
                                var baseName = path.Substring(0, path.LastIndexOf("_"));
                                var actions = new[] { "_st.spr", "_wlk.spr", "_run.spr", "_bat.spr", "_die.spr",
                                                     "_at.spr", "_pst.spr", "_sit.spr", "_magic.spr" };
                                foreach (var action in actions)
                                {
                                    paths.Add(baseName + action);
                                }

                                // Generate shadow variants (add 'b' before .spr)
                                foreach (var action in actions)
                                {
                                    paths.Add(baseName + action.Replace(".spr", "b.spr"));
                                }
                            }
                        }
                    }
                }
            }
            catch
            {
                // Skip files that can't be parsed
            }
        }

        /// <summary>
        /// Parse Skills.txt with column structure
        /// Column 6: SkillIcon - skill icon sprite paths
        /// Column 7: PreCastSpr - pre-cast effect sprites
        /// </summary>
        private void ParseSkillsTxt(string filePath, HashSet<string> paths)
        {
            try
            {
                var lines = File.ReadAllLines(filePath, Encoding.GetEncoding("GBK"));
                bool headerSkipped = false;

                foreach (var line in lines)
                {
                    // Skip header row
                    if (!headerSkipped)
                    {
                        headerSkipped = true;
                        continue;
                    }

                    // Skip comments and empty lines
                    if (string.IsNullOrWhiteSpace(line) || line.TrimStart().StartsWith("//"))
                        continue;

                    // Split by tab
                    var fields = line.Split('\t');

                    // Column 6: SkillIcon (0-indexed = index 5)
                    if (fields.Length > 5 && !string.IsNullOrWhiteSpace(fields[5]))
                    {
                        string iconPath = fields[5].Trim();
                        if (iconPath.Contains(".spr") || iconPath.Contains(".tga"))
                        {
                            iconPath = iconPath.Replace('/', '\\');
                            if (!iconPath.StartsWith("\\")) iconPath = "\\" + iconPath;
                            iconPath = LowercaseAsciiOnly(iconPath);
                            paths.Add(iconPath);
                        }
                    }

                    // Column 7: PreCastSpr (0-indexed = index 6)
                    if (fields.Length > 6 && !string.IsNullOrWhiteSpace(fields[6]))
                    {
                        string preCastPath = fields[6].Trim();
                        if (preCastPath.Contains(".spr"))
                        {
                            preCastPath = preCastPath.Replace('/', '\\');
                            if (!preCastPath.StartsWith("\\")) preCastPath = "\\" + preCastPath;
                            preCastPath = LowercaseAsciiOnly(preCastPath);
                            paths.Add(preCastPath);
                        }
                    }

                    // Also check other columns for .spr or .wav paths
                    for (int i = 0; i < fields.Length; i++)
                    {
                        string field = fields[i].Trim();
                        if ((field.Contains(".spr") || field.Contains(".wav") || field.Contains(".lua")) && field.Contains("\\"))
                        {
                            string resourcePath = field.Replace('/', '\\');
                            if (!resourcePath.StartsWith("\\")) resourcePath = "\\" + resourcePath;
                            resourcePath = LowercaseAsciiOnly(resourcePath);
                            paths.Add(resourcePath);
                        }
                    }
                }

                DebugLogger.Log($"   Parsed Skills.txt: extracted skill icon and effect paths");
            }
            catch (Exception ex)
            {
                DebugLogger.Log($"   ⚠ Error parsing Skills.txt: {ex.Message}");
            }
        }

        /// <summary>
        /// Parse Missles.txt with column structure
        /// Contains AnimFile1-4, AnimFileB1-4 columns with skill effect sprites
        /// </summary>
        private void ParseMisslesTxt(string filePath, HashSet<string> paths)
        {
            try
            {
                var lines = File.ReadAllLines(filePath, Encoding.GetEncoding("GBK"));
                bool headerSkipped = false;

                foreach (var line in lines)
                {
                    // Skip header row
                    if (!headerSkipped)
                    {
                        headerSkipped = true;
                        continue;
                    }

                    // Skip comments and empty lines
                    if (string.IsNullOrWhiteSpace(line) || line.TrimStart().StartsWith("//"))
                        continue;

                    // Split by tab
                    var fields = line.Split('\t');

                    // Parse all fields for sprite paths
                    foreach (var field in fields)
                    {
                        string trimmed = field.Trim();
                        if (trimmed.Contains(".spr") || trimmed.Contains(".wav"))
                        {
                            string resourcePath = trimmed.Replace('/', '\\');
                            if (!resourcePath.StartsWith("\\")) resourcePath = "\\" + resourcePath;
                            resourcePath = LowercaseAsciiOnly(resourcePath);
                            paths.Add(resourcePath);
                        }
                    }
                }

                DebugLogger.Log($"   Parsed Missles.txt: extracted projectile and effect paths");
            }
            catch (Exception ex)
            {
                DebugLogger.Log($"   ⚠ Error parsing Missles.txt: {ex.Message}");
            }
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
                // Use .+? (non-greedy) to match any character including Chinese/Vietnamese
                var pathPatterns = new[]
                {
                    // Backslash paths
                    @"\\[Ss]pr\\.+?\.spr",                  // \Spr\...\file.spr
                    @"\\[Ss]ettings\\.+?\.(ini|txt)",       // \Settings\...\file.ini
                    @"\\[Mm]aps\\.+?\.(dat|wor)",           // \Maps\...\file.dat
                    @"\\[Uu]i\\.+?\.(jpg|bmp|tga|spr|png)", // \Ui\...\file.jpg
                    @"\\[^\\\s]+\\.+?\.(spr|ini|txt|dat|wor|jpg|bmp|tga|png)", // Generic

                    // Forward slash paths (Unix style)
                    @"/[Ss]pr/.+?\.spr",                    // /Spr/.../file.spr
                    @"/[Ss]ettings/.+?\.(ini|txt)",         // /Settings/.../file.ini
                    @"/[Mm]aps/.+?\.(dat|wor)",             // /Maps/.../file.dat
                    @"/[Uu]i/.+?\.(jpg|bmp|tga|spr|png)",   // /Ui/.../file.jpg
                    @"/[^/\s]+/.+?\.(spr|ini|txt|dat|wor|jpg|bmp|tga|png)", // Generic

                    // Without leading slash
                    @"[Ss]pr\\.+?\.spr(?=\s|""|'|$)",       // Spr\...\file.spr
                    @"[Ss]ettings\\.+?\.(ini|txt)(?=\s|""|'|$)", // Settings\...\file.ini
                    @"[Mm]aps\\.+?\.(dat|wor)(?=\s|""|'|$)", // Maps\...\file.dat
                };

                for (int i = 0; i < allFiles.Count; i++)
                {
                    string filePath = allFiles[i];

                    try
                    {
                        string fileExt = Path.GetExtension(filePath).ToLower();

                        // Use specialized parsers for INI and Settings TXT files
                        if (fileExt == ".ini")
                        {
                            ParseIniFileForPaths(filePath, paths);
                        }

                        if (fileExt == ".txt" && (filePath.Contains("Settings") || filePath.Contains("settings") ||
                                                   filePath.Contains("NpcRes") || filePath.Contains("npcres") ||
                                                   filePath.Contains("Item") || filePath.Contains("item") ||
                                                   filePath.Contains("Skill") || filePath.Contains("skill")))
                        {
                            ParseSettingsTxtForPaths(filePath, paths);
                        }

                        // Also do generic regex extraction for all files
                        string content = null;

                        // Try multiple encodings to handle Chinese and Vietnamese characters
                        // Priority: GBK first (most compatible for Chinese), then others
                        var encodings = new List<Encoding>();

                        try { encodings.Add(Encoding.GetEncoding("GBK")); } catch { }          // GBK (Chinese - superset of GB2312)
                        try { encodings.Add(Encoding.GetEncoding("windows-1258")); } catch { } // Vietnamese ANSI
                        try { encodings.Add(Encoding.GetEncoding(1258)); } catch { }           // Vietnamese codepage
                        encodings.Add(Encoding.UTF8);                                          // UTF-8
                        encodings.Add(Encoding.Default);                                       // System default
                        encodings.Add(Encoding.ASCII);                                         // ASCII fallback

                        foreach (var encoding in encodings)
                        {
                            try
                            {
                                content = File.ReadAllText(filePath, encoding);
                                // Successfully read, use this encoding
                                break;
                            }
                            catch
                            {
                                // Try next encoding
                            }
                        }

                        if (content == null)
                            continue; // Skip this file

                        // Extract all path references using regex
                        foreach (var pattern in pathPatterns)
                        {
                            var matches = System.Text.RegularExpressions.Regex.Matches(content, pattern);
                            foreach (System.Text.RegularExpressions.Match match in matches)
                            {
                                string path = match.Value.Trim();

                                // Normalize path: convert forward slash to backslash
                                path = path.Replace('/', '\\');

                                // ENSURE leading backslash (match game engine szPackName[0] = '\\')
                                // From KPakList::FindElemFile(): szPackName[0] = '\\'; ... FileNameToId(szPackName);
                                if (!path.StartsWith("\\"))
                                    path = "\\" + path;

                                // Lowercase ONLY A-Z → a-z (match game engine g_StrLower)
                                path = LowercaseAsciiOnly(path);

                                // Remove trailing whitespace/quotes that might be captured
                                path = path.TrimEnd(' ', '\t', '"', '\'');

                                // Add to set if valid
                                if (path.Length > 3 && path.Contains('.'))  // Min: \a.b
                                {
                                    paths.Add(path);
                                }
                            }
                        }

                        // Extract sprintf/printf format strings and generate paths
                        ExtractAndGenerateFormattedPaths(content, paths);
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

                DebugLogger.Log($"   ✓ Extracted {paths.Count:N0} unique path references from source code (including generated paths)");
            }
            catch (Exception ex)
            {
                DebugLogger.Log($"⚠ Warning scanning folder: {ex.Message}");
            }

            // Generate additional calculated paths based on client logic patterns
            int beforeCalculated = paths.Count;
            GenerateCalculatedPaths(paths, worker);
            DebugLogger.Log($"   ✓ Generated {paths.Count - beforeCalculated:N0} additional calculated paths");

            return paths.ToList();
        }

        /// <summary>
        /// Generate paths based on CLIENT LOGIC patterns (not just extraction)
        /// This calculates paths the same way the client generates them at runtime
        /// </summary>
        private void GenerateCalculatedPaths(HashSet<string> paths, System.ComponentModel.BackgroundWorker worker)
        {
            DebugLogger.Log($"📐 Calculating paths based on client patterns...");

            // 1. Player character sprites - based on series × gender × equipment
            // From client code: sprintf(pszName, "%s_%s_%s_%d.spr", pszPrefix, pszAttribute, pszGender, nIndex)
            GeneratePlayerSprites(paths);

            // 2. Numbered UI sprite sequences (icons, pics, effects)
            GenerateNumberedSpriteSequences(paths);

            // 3. Equipment and item sprites with standard naming
            GenerateEquipmentSprites(paths);

            // 4. Map-related sprites and overlays
            GenerateMapRelatedSprites(paths);
        }

        /// <summary>
        /// Generate player character sprites: series × gender × body parts
        /// Example: \spr\players\金_男_1.spr (Metal-Male-1)
        /// </summary>
        private void GeneratePlayerSprites(HashSet<string> paths)
        {
            // 5 Series (五行): Metal, Wood, Water, Fire, Earth
            var series = new[] { "金", "木", "水", "火", "土" };
            var seriesEn = new[] { "metal", "wood", "water", "fire", "earth" };

            // 2 Genders: Male, Female
            var genders = new[] { "남", "녀" };  // Korean: nam (male), nyeo (female)
            var gendersEn = new[] { "male", "female" };
            var gendersCn = new[] { "男", "女" };  // Chinese

            // Body parts and equipment slots
            var bodyParts = new[] { "body", "head", "hand", "foot", "weapon", "horse" };

            // Generate combinations
            for (int s = 0; s < series.Length; s++)
            {
                for (int g = 0; g < genders.Length; g++)
                {
                    // Korean naming: 금_남_1.spr, 금_녀_1.spr
                    for (int i = 1; i <= 20; i++)
                    {
                        string path = $"\\spr\\players\\{series[s]}_{genders[g]}_{i}.spr";
                        paths.Add(LowercaseAsciiOnly(path));
                    }

                    // English naming: metal_male_1.spr
                    for (int i = 1; i <= 20; i++)
                    {
                        string path = $"\\spr\\players\\{seriesEn[s]}_{gendersEn[g]}_{i}.spr";
                        paths.Add(LowercaseAsciiOnly(path));
                    }

                    // Chinese naming: 金_男_1.spr
                    for (int i = 1; i <= 20; i++)
                    {
                        string path = $"\\spr\\players\\{series[s]}_{gendersCn[g]}_{i}.spr";
                        paths.Add(LowercaseAsciiOnly(path));
                    }

                    // Body part variations
                    foreach (var part in bodyParts)
                    {
                        for (int i = 1; i <= 10; i++)
                        {
                            string path = $"\\spr\\players\\{seriesEn[s]}_{gendersEn[g]}_{part}_{i}.spr";
                            paths.Add(LowercaseAsciiOnly(path));
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Generate numbered sprite sequences: icon001-icon999, pic001-pic999, etc.
        /// Common pattern in UI and effects
        /// </summary>
        private void GenerateNumberedSpriteSequences(HashSet<string> paths)
        {
            // Common base paths and prefixes
            var sequences = new[]
            {
                ("\\spr\\ui\\icon", "icon", 1, 500),           // UI icons
                ("\\spr\\ui\\pic", "pic", 1, 200),             // UI pictures
                ("\\spr\\ui\\button", "button", 1, 100),       // Buttons
                ("\\spr\\ui\\frame", "frame", 1, 50),          // Frames
                ("\\spr\\skill\\icon", "icon", 1, 200),        // Skill icons
                ("\\spr\\item\\icon", "icon", 1, 500),         // Item icons
                ("\\spr\\effect", "effect", 1, 300),           // Effects
                ("\\spr\\ui3\\common", "common", 1, 100),      // UI3 common
            };

            foreach (var (basePath, prefix, start, end) in sequences)
            {
                for (int i = start; i <= end; i++)
                {
                    // Try multiple number formats
                    paths.Add(LowercaseAsciiOnly($"{basePath}\\{prefix}{i}.spr"));         // icon1.spr
                    paths.Add(LowercaseAsciiOnly($"{basePath}\\{prefix}{i:D2}.spr"));      // icon01.spr
                    paths.Add(LowercaseAsciiOnly($"{basePath}\\{prefix}{i:D3}.spr"));      // icon001.spr
                    paths.Add(LowercaseAsciiOnly($"{basePath}\\{prefix}{i:D4}.spr"));      // icon0001.spr
                    paths.Add(LowercaseAsciiOnly($"{basePath}\\{prefix}_{i}.spr"));        // icon_1.spr
                    paths.Add(LowercaseAsciiOnly($"{basePath}\\{prefix}_{i:D2}.spr"));     // icon_01.spr
                    paths.Add(LowercaseAsciiOnly($"{basePath}\\{prefix}_{i:D3}.spr"));     // icon_001.spr
                    paths.Add(LowercaseAsciiOnly($"{basePath}\\{i:D2}.spr"));              // 01.spr
                    paths.Add(LowercaseAsciiOnly($"{basePath}\\{i:D3}.spr"));              // 001.spr
                }
            }
        }

        /// <summary>
        /// Generate equipment sprites with standard naming conventions
        /// Based on item type and ID
        /// </summary>
        private void GenerateEquipmentSprites(HashSet<string> paths)
        {
            var equipTypes = new[]
            {
                "armor", "helmet", "weapon", "melee", "range", "ring", "amulet", "boots", "belt",
                "shield", "horse", "cuff", "pendant", "mask"
            };

            var equipTypesShort = new[] { "ar", "he", "wp", "ml", "rg", "ri", "am", "bo", "be", "sh", "ho", "cu", "pe", "ma" };

            foreach (var type in equipTypes)
            {
                for (int i = 1; i <= 200; i++)
                {
                    // Standard formats
                    paths.Add(LowercaseAsciiOnly($"\\spr\\item\\equip\\{type}\\obj_{type}{i:D2}.spr"));
                    paths.Add(LowercaseAsciiOnly($"\\spr\\item\\equip\\{type}\\obj_{type}{i:D3}.spr"));
                    paths.Add(LowercaseAsciiOnly($"\\spr\\item\\equip\\{type}\\obj_{type}_{i:D2}.spr"));
                    paths.Add(LowercaseAsciiOnly($"\\spr\\item\\equip\\{type}\\{type}{i:D2}.spr"));
                    paths.Add(LowercaseAsciiOnly($"\\spr\\item\\equip\\{type}\\{type}_{i:D2}.spr"));
                }
            }

            // Short form equipment codes
            for (int t = 0; t < equipTypesShort.Length; t++)
            {
                for (int i = 1; i <= 200; i++)
                {
                    paths.Add(LowercaseAsciiOnly($"\\spr\\item\\equip\\obj_{equipTypesShort[t]}_{i:D2}.spr"));
                    paths.Add(LowercaseAsciiOnly($"\\spr\\item\\equip\\obj_{equipTypesShort[t]}{i:D3}.spr"));
                }
            }
        }

        /// <summary>
        /// Generate map-related sprites (minimap icons, overlays, etc.)
        /// </summary>
        private void GenerateMapRelatedSprites(HashSet<string> paths)
        {
            // Minimap icons
            for (int i = 1; i <= 100; i++)
            {
                paths.Add(LowercaseAsciiOnly($"\\spr\\maps\\minimap\\icon{i:D2}.spr"));
                paths.Add(LowercaseAsciiOnly($"\\spr\\maps\\minimap\\icon_{i:D2}.spr"));
                paths.Add(LowercaseAsciiOnly($"\\spr\\ui\\map\\icon{i:D2}.spr"));
            }

            // Map overlays and markers
            var mapElements = new[] { "marker", "flag", "arrow", "dot", "line", "circle" };
            foreach (var elem in mapElements)
            {
                for (int i = 1; i <= 20; i++)
                {
                    paths.Add(LowercaseAsciiOnly($"\\spr\\maps\\{elem}{i}.spr"));
                    paths.Add(LowercaseAsciiOnly($"\\spr\\maps\\{elem}_{i}.spr"));
                    paths.Add(LowercaseAsciiOnly($"\\spr\\ui\\map\\{elem}{i}.spr"));
                }
            }
        }

        private void ExtractAndGenerateFormattedPaths(string content, HashSet<string> paths)
        {
            // Regex to find sprintf/wsprintf/Format calls with format strings
            var formatPatterns = new[]
            {
                @"[ws]?printf\s*\([^,]+,\s*""([^""]+)""",  // sprintf/wsprintf(buf, "format", ...)
                @"Format\s*\(\s*""([^""]+)""",             // Format("format", ...)
                @"""(\\[^\s""]+\\[^""]*%\d*[ds][^""]*\.[a-z]{2,4})""", // Direct format strings with path structure
            };

            foreach (var pattern in formatPatterns)
            {
                var matches = System.Text.RegularExpressions.Regex.Matches(content, pattern);
                foreach (System.Text.RegularExpressions.Match match in matches)
                {
                    if (match.Groups.Count < 2) continue;
                    string formatString = match.Groups[1].Value;

                    // Check if this looks like a file path (contains backslash and extension)
                    if (!formatString.Contains('\\') || !formatString.Contains('.'))
                        continue;

                    // Generate paths from format string
                    GeneratePathsFromFormat(formatString, paths);
                }
            }
        }

        private void GeneratePathsFromFormat(string formatString, HashSet<string> paths)
        {
            // Detect format specifiers: %d, %02d, %03d, %04d, etc.
            var formatMatch = System.Text.RegularExpressions.Regex.Match(formatString, @"%0?(\d*)d");

            if (!formatMatch.Success)
                return; // No numeric format specifier

            int width = 0;
            if (formatMatch.Groups[1].Success && !string.IsNullOrEmpty(formatMatch.Groups[1].Value))
            {
                int.TryParse(formatMatch.Groups[1].Value, out width);
            }

            // Determine range based on width
            int maxValue = 99; // Default for %d
            if (width == 2) maxValue = 99;   // %02d → 00-99
            else if (width == 3) maxValue = 999;  // %03d → 000-999
            else if (width == 4) maxValue = 9999; // %04d → 0000-9999

            // Limit to reasonable range to avoid too many combinations
            maxValue = Math.Min(maxValue, 999);

            // Generate paths
            string formatSpec = formatMatch.Value; // e.g., %03d

            for (int i = 0; i <= maxValue; i++)
            {
                string path = formatString.Replace(formatSpec, i.ToString(new string('0', width)));

                // Normalize path: convert forward slash to backslash
                path = path.Replace('/', '\\');

                // ENSURE leading backslash (match game engine szPackName[0] = '\\')
                if (!path.StartsWith("\\"))
                    path = "\\" + path;

                // Lowercase ONLY A-Z → a-z (match game engine g_StrLower)
                path = LowercaseAsciiOnly(path);

                // Add if valid
                if (path.Length > 3 && path.Contains('.'))
                {
                    paths.Add(path);
                }
            }
        }

        /// <summary>
        /// Lowercase ONLY ASCII A-Z characters to a-z, preserving all other characters.
        /// Matches the game engine's g_StrLower() function behavior.
        /// Chinese, Vietnamese, and other non-ASCII characters are preserved.
        /// </summary>
        private static string LowercaseAsciiOnly(string input)
        {
            if (string.IsNullOrEmpty(input))
                return input;

            char[] chars = input.ToCharArray();
            for (int i = 0; i < chars.Length; i++)
            {
                // Only convert A-Z to a-z (ASCII 65-90 → 97-122)
                if (chars[i] >= 'A' && chars[i] <= 'Z')
                {
                    chars[i] = (char)(chars[i] + 32); // 'a' - 'A' = 32
                }
            }
            return new string(chars);
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
            using (StreamWriter writer = new StreamWriter(txtFile, false, Encoding.GetEncoding("GBK")))
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

                    // Paths already have leading backslash from extraction
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

        private void BtnExportPaths_Click(object sender, EventArgs e)
        {
            if (_lastGeneratedPaths == null || _lastGeneratedPaths.Count == 0)
            {
                MessageBox.Show("No paths generated yet. Please run 'Generate Index' first.",
                    "No Paths Available", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            using (var dialog = new SaveFileDialog())
            {
                dialog.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
                dialog.FileName = $"generated_paths_{DateTime.Now:yyyyMMdd_HHmmss}.txt";
                dialog.Title = "Export Generated Paths";

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        // Write all paths to file with UTF-8 encoding (supports Chinese characters)
                        File.WriteAllLines(dialog.FileName, _lastGeneratedPaths, Encoding.UTF8);

                        DebugLogger.Log($"💾 Exported {_lastGeneratedPaths.Count:N0} paths to: {dialog.FileName}");

                        MessageBox.Show(
                            $"Successfully exported {_lastGeneratedPaths.Count:N0} paths to:\n\n{dialog.FileName}",
                            "Export Complete", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    catch (Exception ex)
                    {
                        DebugLogger.Log($"❌ Error exporting paths: {ex.Message}");
                        MessageBox.Show($"Error exporting paths:\n\n{ex.Message}",
                            "Export Failed", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
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
