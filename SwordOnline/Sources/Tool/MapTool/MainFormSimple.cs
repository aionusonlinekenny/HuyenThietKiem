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
            _renderer = new MapRenderer();
            _exporter = new TrapExporter();

            // Set default game folder if exists
            string defaultFolder = @"D:\HuyenThietKiem\Bin\Server";
            if (Directory.Exists(defaultFolder))
            {
                txtGameFolder.Text = defaultFolder;
            }
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
                _mapLoader = new MapLoader(_gameFolder, _isServerMode);

                // Auto-load complete map
                _currentMap = _mapLoader.LoadMap(mapId);

                // Update UI
                lblMapInfo.Text = $"Map: {_currentMap.MapName} (ID: {_currentMap.MapId})\n" +
                                  $"Folder: {_currentMap.MapFolder}\n" +
                                  $"Type: {_currentMap.MapType}\n" +
                                  $"Region Grid: {_currentMap.RegionWidth}x{_currentMap.RegionHeight}\n" +
                                  $"Map Size: {_currentMap.GetMapPixelWidth()}x{_currentMap.GetMapPixelHeight()} pixels\n" +
                                  $"Loaded: {_currentMap.LoadedRegionCount}/{_currentMap.RegionWidth * _currentMap.RegionHeight} regions";

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
                    Console.WriteLine($"⚠ No map image data available");
                    _renderer.ClearMapImage();
                    lblStatus.Text = $"Map loaded (no image). {_currentMap.LoadedRegionCount} regions.";
                }

                // Load regions into renderer
                _renderer.ClearRegions();
                foreach (var region in _currentMap.Regions.Values)
                {
                    _renderer.AddRegion(region);
                }

                // Reset view
                _renderer.ViewOffsetX = 0;
                _renderer.ViewOffsetY = 0;
                _renderer.Zoom = 1.0f;

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

        // Map panel paint
        private void mapPanel_Paint(object sender, PaintEventArgs e)
        {
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
            if (_isPanning)
            {
                int deltaX = _lastMousePosition.X - e.X;
                int deltaY = _lastMousePosition.Y - e.Y;

                _renderer.Pan(deltaX, deltaY);
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
            _renderer.Zoom = Math.Min(4.0f, _renderer.Zoom * 1.2f);
            mapPanel.Invalidate();
        }

        private void btnZoomOut_Click(object sender, EventArgs e)
        {
            _renderer.Zoom = Math.Max(0.1f, _renderer.Zoom / 1.2f);
            mapPanel.Invalidate();
        }

        // Export buttons
        private void btnExport_Click(object sender, EventArgs e)
        {
            // Export ALL cells from ALL loaded regions
            ExportAllCellsToTxt();
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
                    Console.WriteLine($"📝 Exporting all cells to: {dialog.FileName}");

                    int totalCells = 0;

                    using (StreamWriter writer = new StreamWriter(dialog.FileName, false, System.Text.Encoding.UTF8))
                    {
                        // Write header
                        writer.WriteLine("MapId\tRegionId\tCellX\tCellY\tScriptFile\tIsLoad");

                        // Loop through all loaded regions
                        foreach (var region in _currentMap.Regions.Values)
                        {
                            if (!region.IsLoaded)
                                continue;

                            // Use the correct RegionID (x | (y << 16)) from the region data
                            int regionId = region.RegionID;

                            // Loop through all cells in region (16x32)
                            for (int cellY = 0; cellY < MapConstants.REGION_GRID_HEIGHT; cellY++)
                            {
                                for (int cellX = 0; cellX < MapConstants.REGION_GRID_WIDTH; cellX++)
                                {
                                    // Write cell data
                                    // Format: MapId	RegionId	CellX	CellY	ScriptFile	IsLoad
                                    writer.WriteLine($"{_currentMap.MapId}\t{regionId}\t{cellX}\t{cellY}\t\t1");
                                    totalCells++;
                                }
                            }
                        }
                    }

                    Console.WriteLine($"✓ Exported {totalCells} cells to {dialog.FileName}");
                    MessageBox.Show($"Exported {totalCells} cells to:\n{dialog.FileName}",
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
