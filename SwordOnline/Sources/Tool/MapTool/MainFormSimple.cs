using System;
using System.Drawing;
using System.Windows.Forms;
using System.IO;
using MapTool.MapData;
using MapTool.Rendering;
using MapTool.Export;

namespace MapTool
{
    /// <summary>
    /// Simple Map Tool - Auto-load workflow
    /// Browse game folder → Enter Map ID → Load → Done!
    /// </summary>
    public partial class MainFormSimple : Form
    {
        private string _gameFolder;
        private bool _isServerMode = true;
        private MapLoader _mapLoader;
        private CompleteMapData _currentMap;
        private MapRenderer _renderer;
        private TrapExporter _exporter;
        private MapCoordinate? _selectedCoordinate;

        // Panning state
        private bool _isPanning;
        private Point _lastMousePosition;

        public MainFormSimple()
        {
            InitializeComponent();

            // Initialize debug logger FIRST
            DebugLogger.Initialize();
            DebugLogger.Log("=== MapTool Started ===");
            DebugLogger.Log($"Executable: {System.Reflection.Assembly.GetExecutingAssembly().Location}");
            DebugLogger.Log($"Working Directory: {Environment.CurrentDirectory}");
            DebugLogger.Log($"Log file: {DebugLogger.GetLogFilePath()}");
            DebugLogger.LogSeparator();

            _renderer = new MapRenderer();
            _exporter = new TrapExporter();

            // Set default game folder if exists
            string defaultFolder = @"D:\HuyenThietKiem\Bin\Server";
            if (Directory.Exists(defaultFolder))
            {
                txtGameFolder.Text = defaultFolder;
                DebugLogger.Log($"Default folder set: {defaultFolder}");
            }

            // Show log file path in status
            lblStatus.Text = $"Ready. Log: {Path.GetFileName(DebugLogger.GetLogFilePath())}";
        }

        // Browse game folder
        private void btnBrowseFolder_Click(object sender, EventArgs e)
        {
            using (FolderBrowserDialog dialog = new FolderBrowserDialog())
            {
                dialog.Description = "Select game folder (Bin/Server or Bin/Client)";
                if (!string.IsNullOrEmpty(txtGameFolder.Text))
                {
                    dialog.SelectedPath = txtGameFolder.Text;
                }

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    txtGameFolder.Text = dialog.SelectedPath;
                    _gameFolder = dialog.SelectedPath;

                    // Update mode based on folder name
                    if (dialog.SelectedPath.ToLower().Contains("server"))
                    {
                        rdoServer.Checked = true;
                        _isServerMode = true;
                    }
                    else if (dialog.SelectedPath.ToLower().Contains("client"))
                    {
                        rdoClient.Checked = true;
                        _isServerMode = false;
                    }
                }
            }
        }

        // Mode changed
        private void rdoServer_CheckedChanged(object sender, EventArgs e)
        {
            _isServerMode = rdoServer.Checked;
        }

        // Load map button clicked
        private void btnLoadMap_Click(object sender, EventArgs e)
        {
            // Validate inputs
            if (string.IsNullOrEmpty(txtGameFolder.Text))
            {
                MessageBox.Show("Please select game folder first!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            if (!int.TryParse(txtMapId.Text, out int mapId))
            {
                MessageBox.Show("Please enter valid Map ID!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Load map
            try
            {
                Cursor = Cursors.WaitCursor;
                lblStatus.Text = "Loading map...";
                Application.DoEvents();

                _gameFolder = txtGameFolder.Text;

                // Log load attempt
                DebugLogger.LogSeparator();
                DebugLogger.Log($"📂 LOADING MAP");
                DebugLogger.Log($"   Map ID: {mapId}");
                DebugLogger.Log($"   Game Folder: {_gameFolder}");
                DebugLogger.Log($"   Mode: {(_isServerMode ? "Server" : "Client")}");
                DebugLogger.LogSeparator();

                _mapLoader = new MapLoader(_gameFolder, _isServerMode);

                // Auto-load complete map
                _currentMap = _mapLoader.LoadMap(mapId);

                // Log loaded map info
                DebugLogger.Log($"✓ Map loaded successfully!");
                DebugLogger.Log($"   Name: {_currentMap.MapName}");
                DebugLogger.Log($"   Folder: {_currentMap.MapFolder}");
                DebugLogger.Log($"   Regions loaded: {_currentMap.LoadedRegionCount}");
                if (_currentMap.Regions.Count > 0)
                {
                    var firstRegion = _currentMap.Regions.Values.GetEnumerator();
                    firstRegion.MoveNext();
                    var region = firstRegion.Current;
                    DebugLogger.Log($"   First region: ({region.RegionX}, {region.RegionY}) RegionID={region.RegionID}");
                }
                DebugLogger.LogSeparator();

                // Update UI
                lblMapInfo.Text = $"Map: {_currentMap.MapName} (ID: {_currentMap.MapId})\n" +
                                  $"Folder: {_currentMap.MapFolder}\n" +
                                  $"Type: {_currentMap.MapType}\n" +
                                  $"Region Grid: {_currentMap.RegionWidth}x{_currentMap.RegionHeight}\n" +
                                  $"Map Size: {_currentMap.GetMapPixelWidth()}x{_currentMap.GetMapPixelHeight()} pixels\n" +
                                  $"Loaded: {_currentMap.LoadedRegionCount}/{_currentMap.RegionWidth * _currentMap.RegionHeight} regions";

                // Load regions into renderer FIRST
                _renderer.ClearRegions();
                foreach (var region in _currentMap.Regions.Values)
                {
                    _renderer.AddRegion(region);
                }

                // Load map image if available
                if (_currentMap.MapImageData != null)
                {
                    Console.WriteLine($"🎨 Setting map image to renderer ({_currentMap.MapImageData.Length} bytes)");
                    Console.WriteLine($"🎨 Map image offset: ({_currentMap.MapImageOffsetX}, {_currentMap.MapImageOffsetY})");
                    _renderer.SetMapImage(_currentMap.MapImageData, _currentMap.MapImageOffsetX, _currentMap.MapImageOffsetY);

                    lblStatus.Text = $"Map loaded with image! {_currentMap.LoadedRegionCount} regions.";
                }
                else
                {
                    Console.WriteLine($"ℹ No map image (24.jpg) - this is normal for most maps");
                    Console.WriteLine($"  Tool will render region grid and obstacles/traps");
                    _renderer.ClearMapImage();

                    lblStatus.Text = $"Map loaded (no background image). {_currentMap.LoadedRegionCount} regions.";
                }

                _renderer.Zoom = 1.0f;

                // Update scroll area size based on map size and zoom
                UpdateScrollAreaSize();

                // Set initial scroll position to show map image or first region
                // AutoScrollPosition uses NEGATIVE values and SCREEN pixels (with zoom)
                int initialViewX = 0;
                int initialViewY = 0;

                if (_currentMap.MapImageData != null)
                {
                    // Scroll to map image position
                    initialViewX = _currentMap.MapImageOffsetX;
                    initialViewY = _currentMap.MapImageOffsetY;
                    Console.WriteLine($"📍 Initial view set to map image position: ({initialViewX}, {initialViewY})");
                }
                else if (_currentMap.Regions.Count > 0)
                {
                    // Scroll to first loaded region
                    var firstRegion = _currentMap.Regions.Values.GetEnumerator();
                    firstRegion.MoveNext();
                    var region = firstRegion.Current;
                    initialViewX = region.RegionX * MapConstants.MAP_REGION_PIXEL_WIDTH;
                    initialViewY = region.RegionY * MapConstants.MAP_REGION_PIXEL_HEIGHT;
                    Console.WriteLine($"📍 Initial view set to first region: ({region.RegionX}, {region.RegionY})");
                }
                else
                {
                    // Scroll to region grid start
                    initialViewX = _currentMap.Config.RegionLeft * MapConstants.MAP_REGION_PIXEL_WIDTH;
                    initialViewY = _currentMap.Config.RegionTop * MapConstants.MAP_REGION_PIXEL_HEIGHT;
                }

                // Convert MAP pixels to SCREEN pixels (apply zoom) and set scroll position
                // AutoScrollPosition uses NEGATIVE coordinates
                mapPanel.AutoScrollPosition = new Point(
                    (int)(initialViewX * _renderer.Zoom),
                    (int)(initialViewY * _renderer.Zoom)
                );

                mapPanel.Invalidate();

                // Don't auto-export - user will use Export button
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Failed to load map:\n{ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                lblStatus.Text = "Load failed!";
            }
            finally
            {
                Cursor = Cursors.Default;
            }
        }

        // Update scroll area size based on map and zoom level
        private void UpdateScrollAreaSize()
        {
            if (_currentMap == null)
            {
                mapPanel.AutoScrollMinSize = new Size(0, 0);
                return;
            }

            // Calculate the farthest extent of all content (regions and map image)
            // Scroll area must be large enough to SCROLL TO any content position
            // Not just contain the content size!
            int maxX = 0;
            int maxY = 0;

            // Include all loaded regions
            foreach (var region in _currentMap.Regions.Values)
            {
                int regionMapX = region.RegionX * MapConstants.MAP_REGION_PIXEL_WIDTH;
                int regionMapY = region.RegionY * MapConstants.MAP_REGION_PIXEL_HEIGHT;

                maxX = Math.Max(maxX, regionMapX + MapConstants.MAP_REGION_PIXEL_WIDTH);
                maxY = Math.Max(maxY, regionMapY + MapConstants.MAP_REGION_PIXEL_HEIGHT);
            }

            // Include map image if available
            if (_currentMap.MapImageData != null && _renderer != null)
            {
                var imageInfo = _renderer.GetMapImageBounds();
                if (imageInfo.HasValue)
                {
                    maxX = Math.Max(maxX, imageInfo.Value.X + imageInfo.Value.Width);
                    maxY = Math.Max(maxY, imageInfo.Value.Y + imageInfo.Value.Height);
                }
            }

            // Scroll area = entire virtual canvas from (0,0) to (maxX, maxY)
            // AutoScrollPosition can be 0 to (ScrollArea - Viewport)
            // So if content is at position X, ScrollArea must be >= X + Viewport
            int scrollWidth = (int)(maxX * _renderer.Zoom) + mapPanel.Width + 1000;
            int scrollHeight = (int)(maxY * _renderer.Zoom) + mapPanel.Height + 1000;

            mapPanel.AutoScrollMinSize = new Size(scrollWidth, scrollHeight);

            Console.WriteLine($"📏 Content max extent: MAP (0,0) to ({maxX},{maxY})");
            Console.WriteLine($"📏 Scroll area: {scrollWidth}x{scrollHeight} SCREEN pixels (zoom: {_renderer.Zoom:F2})");
        }

        // Map panel paint
        private void mapPanel_Paint(object sender, PaintEventArgs e)
        {
            // Sync renderer view offset with panel's auto scroll position
            // AutoScrollPosition is in SCREEN pixels (with zoom applied)
            // ViewOffset is in MAP pixels (without zoom)
            // Need to convert: MAP pixels = SCREEN pixels / zoom
            if (mapPanel.AutoScroll)
            {
                _renderer.ViewOffsetX = (int)(-mapPanel.AutoScrollPosition.X / _renderer.Zoom);
                _renderer.ViewOffsetY = (int)(-mapPanel.AutoScrollPosition.Y / _renderer.Zoom);
            }

            _renderer.Render(e.Graphics, mapPanel.Width, mapPanel.Height, _selectedCoordinate);
        }

        // Map panel mouse down
        private void mapPanel_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                _isPanning = true;
                _lastMousePosition = e.Location;
                mapPanel.Cursor = Cursors.SizeAll;
            }
            else if (e.Button == MouseButtons.Left)
            {
                // Select cell
                _selectedCoordinate = _renderer.ScreenToMapCoordinate(e.X, e.Y);
                UpdateCoordinateDisplay();
                mapPanel.Invalidate();
            }
        }

        // Map panel mouse up
        private void mapPanel_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                _isPanning = false;
                mapPanel.Cursor = Cursors.Default;
            }
        }

        // Map panel mouse move
        private void mapPanel_MouseMove(object sender, MouseEventArgs e)
        {
            if (_isPanning && mapPanel.AutoScroll)
            {
                int deltaX = _lastMousePosition.X - e.X;
                int deltaY = _lastMousePosition.Y - e.Y;

                // Update auto scroll position instead of manual panning
                Point currentScroll = mapPanel.AutoScrollPosition;
                mapPanel.AutoScrollPosition = new Point(
                    -currentScroll.X + deltaX,
                    -currentScroll.Y + deltaY
                );

                _lastMousePosition = e.Location;
                mapPanel.Invalidate();
            }
            else if (_currentMap != null)
            {
                // Show coordinate under mouse
                MapCoordinate coord = _renderer.ScreenToMapCoordinate(e.X, e.Y);
                lblStatus.Text = $"World: ({coord.WorldX}, {coord.WorldY}) | Region: ({coord.RegionX}, {coord.RegionY}) | Cell: ({coord.CellX}, {coord.CellY})";
            }
        }

        // Map panel double-click to add trap entry
        private void mapPanel_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left && _selectedCoordinate.HasValue && _currentMap != null)
            {
                // Add to trap list
                string scriptFile = txtScriptFile.Text;
                if (string.IsNullOrWhiteSpace(scriptFile))
                {
                    scriptFile = $@"\script\maps\trap\{_currentMap.MapId}\1.lua";
                }

                _exporter.AddEntry(_currentMap.MapId, _selectedCoordinate.Value, scriptFile);
                UpdateTrapList();

                lblStatus.Text = $"Added trap entry at ({_selectedCoordinate.Value.CellX}, {_selectedCoordinate.Value.CellY})";
            }
        }

        // Update coordinate display
        private void UpdateCoordinateDisplay()
        {
            if (!_selectedCoordinate.HasValue)
            {
                txtWorldX.Text = "";
                txtWorldY.Text = "";
                txtRegionX.Text = "";
                txtRegionY.Text = "";
                txtRegionID.Text = "";
                txtCellX.Text = "";
                txtCellY.Text = "";
                return;
            }

            MapCoordinate coord = _selectedCoordinate.Value;
            txtWorldX.Text = coord.WorldX.ToString();
            txtWorldY.Text = coord.WorldY.ToString();
            txtRegionX.Text = coord.RegionX.ToString();
            txtRegionY.Text = coord.RegionY.ToString();
            txtRegionID.Text = coord.RegionID.ToString();
            txtCellX.Text = coord.CellX.ToString();
            txtCellY.Text = coord.CellY.ToString();
        }

        // Update trap entry list
        private void UpdateTrapList()
        {
            lstEntries.Items.Clear();
            foreach (var entry in _exporter.GetEntries())
            {
                lstEntries.Items.Add(entry);
            }
        }

        // Zoom buttons
        private void btnZoomIn_Click(object sender, EventArgs e)
        {
            ZoomMap(1.2f);
        }

        private void btnZoomOut_Click(object sender, EventArgs e)
        {
            ZoomMap(1.0f / 1.2f);
        }

        // Zoom map while keeping center point stable
        private void ZoomMap(float zoomFactor)
        {
            if (_currentMap == null)
                return;

            float oldZoom = _renderer.Zoom;
            float newZoom = Math.Max(0.1f, Math.Min(4.0f, oldZoom * zoomFactor));

            if (Math.Abs(newZoom - oldZoom) < 0.001f)
                return; // No change

            // Get current scroll position (note: AutoScrollPosition returns negative values)
            int oldScrollX = -mapPanel.AutoScrollPosition.X;  // Convert to positive
            int oldScrollY = -mapPanel.AutoScrollPosition.Y;

            // Calculate center point of viewport in MAP coordinates
            // ViewOffset (MAP coords) = ScrollPosition (SCREEN coords) / zoom
            // Center MAP = ViewOffset + (viewportCenter / zoom)
            int viewportCenterX = mapPanel.Width / 2;
            int viewportCenterY = mapPanel.Height / 2;

            float centerMapX = (oldScrollX / oldZoom) + (viewportCenterX / oldZoom);
            float centerMapY = (oldScrollY / oldZoom) + (viewportCenterY / oldZoom);

            // Update zoom level
            _renderer.Zoom = newZoom;

            // Recalculate scroll area with new zoom
            UpdateScrollAreaSize();

            // Calculate new scroll position to keep same MAP center point
            // centerMapX = (newScrollX / newZoom) + (viewportCenterX / newZoom)
            // centerMapX * newZoom = newScrollX + viewportCenterX
            // newScrollX = (centerMapX * newZoom) - viewportCenterX
            int newScrollX = (int)((centerMapX * newZoom) - viewportCenterX);
            int newScrollY = (int)((centerMapY * newZoom) - viewportCenterY);

            // Clamp to valid range (0 to ScrollAreaSize - ViewportSize)
            newScrollX = Math.Max(0, Math.Min(newScrollX, mapPanel.AutoScrollMinSize.Width - mapPanel.Width));
            newScrollY = Math.Max(0, Math.Min(newScrollY, mapPanel.AutoScrollMinSize.Height - mapPanel.Height));

            // Set scroll position (must use positive values when setting)
            mapPanel.AutoScrollPosition = new Point(newScrollX, newScrollY);

            mapPanel.Invalidate();
            lblStatus.Text = $"Zoom: {_renderer.Zoom:P0}";
        }

        // Export buttons
        private void btnExport_Click(object sender, EventArgs e)
        {
            // Export trap entries that user added to the list (via double-click)
            if (_exporter.GetEntries().Count == 0)
            {
                MessageBox.Show("No trap entries to export!\n\nDouble-click on the map to add trap entries to the list first.",
                    "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            // Ask user for save location
            using (SaveFileDialog dialog = new SaveFileDialog())
            {
                dialog.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
                dialog.FileName = _currentMap != null ? $"{_currentMap.MapId}.txt" : "traps.txt";
                dialog.DefaultExt = "txt";
                dialog.Title = "Export Trap Entries";

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        _exporter.ExportToFile(dialog.FileName);

                        string stats = _exporter.GetStatistics();
                        MessageBox.Show($"Exported successfully!\n\nFile: {dialog.FileName}\n\n{stats}",
                            "Export Complete", MessageBoxButtons.OK, MessageBoxIcon.Information);

                        lblStatus.Text = $"Exported {_exporter.GetEntries().Count} trap entries to {Path.GetFileName(dialog.FileName)}";
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show($"Failed to export:\n{ex.Message}", "Error",
                            MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            _exporter.Clear();
            UpdateTrapList();
        }

        private void btnRemoveLast_Click(object sender, EventArgs e)
        {
            _exporter.RemoveLast();
            UpdateTrapList();
        }

        // Export all cells from all loaded regions
        private void ExportAllCellsToTxt()
        {
            if (_currentMap == null || _currentMap.Regions == null || _currentMap.Regions.Count == 0)
            {
                MessageBox.Show("No map loaded! Please load a map first.", "Warning",
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            // Ask user for save location
            using (SaveFileDialog dialog = new SaveFileDialog())
            {
                dialog.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
                dialog.FileName = $"{_currentMap.MapId}.txt";
                dialog.DefaultExt = "txt";
                dialog.Title = "Export All Cells to Txt";

                if (dialog.ShowDialog() != DialogResult.OK)
                    return;

                try
                {
                    Cursor = Cursors.WaitCursor;

                    // Log export start
                    DebugLogger.LogSeparator();
                    DebugLogger.Log($"📝 EXPORTING TO TXT");
                    DebugLogger.Log($"   Output file: {dialog.FileName}");
                    DebugLogger.Log($"   Map ID: {_currentMap.MapId}");
                    DebugLogger.Log($"   Regions to export: {_currentMap.Regions.Count}");

                    int totalCells = 0;
                    int regionCount = 0;

                    using (StreamWriter writer = new StreamWriter(dialog.FileName, false, System.Text.Encoding.UTF8))
                    {
                        // Write header - MapId RegionId CellX CellY ScriptFile IsLoad format
                        writer.WriteLine("MapId\tRegionId\tCellX\tCellY\tScriptFile\tIsLoad");

                        // Loop through all loaded regions
                        foreach (var region in _currentMap.Regions.Values)
                        {
                            if (!region.IsLoaded)
                                continue;

                            // Use RegionID from region data (calculated by Y*256+X formula)
                            int regionId = region.RegionID;

                            // Log first few regions to debug
                            if (regionCount < 5)
                            {
                                DebugLogger.Log($"   Region #{regionCount + 1}: ({region.RegionX}, {region.RegionY}) → RegionID = {regionId}");
                                DebugLogger.Log($"      Formula check: {region.RegionY} * 256 + {region.RegionX} = {region.RegionY * 256 + region.RegionX}");
                            }

                            // Loop through all cells in region (16x32)
                            for (int cellY = 0; cellY < MapConstants.REGION_GRID_HEIGHT; cellY++)
                            {
                                for (int cellX = 0; cellX < MapConstants.REGION_GRID_WIDTH; cellX++)
                                {
                                    // Write cell data in correct format
                                    writer.WriteLine($"{_currentMap.MapId}\t{regionId}\t{cellX}\t{cellY}\t\t1");
                                    totalCells++;
                                }
                            }

                            regionCount++;
                        }
                    }

                    DebugLogger.Log($"✓ Export completed!");
                    DebugLogger.Log($"   Total cells: {totalCells}");
                    DebugLogger.Log($"   Total regions: {regionCount}");
                    DebugLogger.LogSeparator();

                    MessageBox.Show($"Exported {totalCells} cells from {regionCount} regions to:\n{dialog.FileName}\n\nCheck log file for details!",
                        "Export Complete", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"❌ Failed to export: {ex.Message}");
                    MessageBox.Show($"Failed to export:\n{ex.Message}",
                        "Export Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                finally
                {
                    Cursor = Cursors.Default;
                }
            }
        }
    }
}