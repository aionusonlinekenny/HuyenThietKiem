using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using MapTool.MapData;
using MapTool.Rendering;
using MapTool.Export;

namespace MapTool
{
    public partial class MainForm : Form
    {
        private MapRenderer _renderer;
        private TrapExporter _exporter;
        private MapConfig _currentMapConfig;
        private MapCoordinate? _selectedCoordinate;
        private Point _lastMousePosition;
        private bool _isPanning;
        private int _currentMapId;

        public MainForm()
        {
            InitializeComponent();
            InitializeCustomComponents();
        }

        private void InitializeCustomComponents()
        {
            _renderer = new MapRenderer();
            _exporter = new TrapExporter();
            _currentMapId = 1;

            // Setup double buffering for map panel
            typeof(Panel).InvokeMember("DoubleBuffered",
                System.Reflection.BindingFlags.SetProperty |
                System.Reflection.BindingFlags.Instance |
                System.Reflection.BindingFlags.NonPublic,
                null, mapPanel, new object[] { true });
        }

        #region UI Event Handlers

        private void btnLoadMap_Click(object sender, EventArgs e)
        {
            using (FolderBrowserDialog dialog = new FolderBrowserDialog())
            {
                dialog.Description = "Select map directory (containing .wor file)";

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    LoadMap(dialog.SelectedPath);
                }
            }
        }

        private void btnLoadWor_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog dialog = new OpenFileDialog())
            {
                dialog.Filter = "World Files (*.wor)|*.wor|All Files (*.*)|*.*";
                dialog.Title = "Select .wor file";

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    string mapPath = Path.GetDirectoryName(dialog.FileName);
                    LoadMap(mapPath);
                }
            }
        }

        private void btnLoadRegion_Click(object sender, EventArgs e)
        {
            if (_currentMapConfig == null)
            {
                MessageBox.Show("Please load a map first!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            if (!int.TryParse(txtRegionX.Text, out int regionX) ||
                !int.TryParse(txtRegionY.Text, out int regionY))
            {
                MessageBox.Show("Invalid region coordinates!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            LoadRegion(regionX, regionY);
        }

        private void btnExport_Click(object sender, EventArgs e)
        {
            if (_exporter.GetEntries().Count == 0)
            {
                MessageBox.Show("No entries to export!", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            using (SaveFileDialog dialog = new SaveFileDialog())
            {
                dialog.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
                dialog.DefaultExt = "txt";
                dialog.FileName = $"{_currentMapId}.txt";

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        _exporter.ExportToTrapFile(dialog.FileName);
                        MessageBox.Show($"Exported {_exporter.GetEntries().Count} entries successfully!",
                                      "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show($"Export failed: {ex.Message}", "Error",
                                      MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("Clear all entries?", "Confirm",
                              MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
            {
                _exporter.Clear();
                UpdateEntryList();
                UpdateStatistics();
            }
        }

        private void btnRemoveLast_Click(object sender, EventArgs e)
        {
            if (_exporter.RemoveLast())
            {
                UpdateEntryList();
                UpdateStatistics();
            }
        }

        private void txtMapId_TextChanged(object sender, EventArgs e)
        {
            int.TryParse(txtMapId.Text, out _currentMapId);
        }

        private void btnZoomIn_Click(object sender, EventArgs e)
        {
            _renderer.Zoom *= 1.2f;
            mapPanel.Invalidate();
        }

        private void btnZoomOut_Click(object sender, EventArgs e)
        {
            _renderer.Zoom /= 1.2f;
            mapPanel.Invalidate();
        }

        private void mapPanel_Paint(object sender, PaintEventArgs e)
        {
            _renderer.Render(e.Graphics, mapPanel.Width, mapPanel.Height, _selectedCoordinate);
        }

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

        private void mapPanel_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                _isPanning = false;
                mapPanel.Cursor = Cursors.Default;
            }
        }

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
            else
            {
                // Show coordinate under mouse
                MapCoordinate coord = _renderer.ScreenToMapCoordinate(e.X, e.Y);
                lblStatus.Text = $"World: ({coord.WorldX}, {coord.WorldY}) | Region: ({coord.RegionX}, {coord.RegionY}) | Cell: ({coord.CellX}, {coord.CellY})";
            }
        }

        private void mapPanel_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left && _selectedCoordinate.HasValue)
            {
                // Add entry
                string scriptFile = txtScriptFile.Text;
                if (string.IsNullOrWhiteSpace(scriptFile))
                {
                    scriptFile = $@"\script\maps\trap\{_currentMapId}\1.lua";
                }

                _exporter.AddEntry(_currentMapId, _selectedCoordinate.Value, _currentMapConfig, scriptFile);
                UpdateEntryList();
                UpdateStatistics();

                lblStatus.Text = "Entry added";
            }
        }

        #endregion

        #region Map Loading

        private void LoadMap(string mapPath)
        {
            try
            {
                // Find .wor file
                string[] worFiles = Directory.GetFiles(mapPath, "*.wor");
                if (worFiles.Length == 0)
                {
                    MessageBox.Show("No .wor file found in directory!", "Error",
                                  MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                _currentMapConfig = MapFileParser.LoadMapConfig(worFiles[0]);

                // Update UI
                lblMapInfo.Text = $"Map: {_currentMapConfig.MapName}\n" +
                                 $"Path: {_currentMapConfig.MapPath}\n" +
                                 $"Indoor: {_currentMapConfig.IsIndoor}\n" +
                                 $"Region Bounds: ({_currentMapConfig.RegionLeft}, {_currentMapConfig.RegionTop}) to " +
                                 $"({_currentMapConfig.RegionRight}, {_currentMapConfig.RegionBottom})\n" +
                                 $"Total Regions: {_currentMapConfig.TotalRegions}";

                // Get available regions
                var regions = MapFileParser.GetAvailableRegions(_currentMapConfig.MapPath, chkUseServer.Checked);

                lstRegions.Items.Clear();
                foreach (var (x, y) in regions)
                {
                    lstRegions.Items.Add($"Region ({x}, {y})");
                }

                lblStatus.Text = $"Map loaded: {_currentMapConfig.MapName} ({regions.Count} regions available)";
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Failed to load map: {ex.Message}", "Error",
                              MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void LoadRegion(int regionX, int regionY)
        {
            try
            {
                string filePath = CoordinateConverter.GetRegionFilePath(_currentMapConfig.MapPath,
                                                                        regionX, regionY,
                                                                        chkUseServer.Checked);

                RegionData region = MapFileParser.LoadRegion(filePath, regionX, regionY);
                _renderer.AddRegion(region);

                mapPanel.Invalidate();
                lblStatus.Text = $"Region ({regionX}, {regionY}) loaded";
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Failed to load region: {ex.Message}", "Error",
                              MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void lstRegions_DoubleClick(object sender, EventArgs e)
        {
            if (lstRegions.SelectedItem != null)
            {
                string item = lstRegions.SelectedItem.ToString();
                // Parse "Region (x, y)"
                int start = item.IndexOf('(') + 1;
                int comma = item.IndexOf(',');
                int end = item.IndexOf(')');

                if (int.TryParse(item.Substring(start, comma - start).Trim(), out int x) &&
                    int.TryParse(item.Substring(comma + 1, end - comma - 1).Trim(), out int y))
                {
                    LoadRegion(x, y);
                }
            }
        }

        #endregion

        #region UI Updates

        private void UpdateCoordinateDisplay()
        {
            if (!_selectedCoordinate.HasValue)
                return;

            var coord = _selectedCoordinate.Value;
            txtWorldX.Text = coord.WorldX.ToString();
            txtWorldY.Text = coord.WorldY.ToString();
            txtRegionX.Text = coord.RegionX.ToString();
            txtRegionY.Text = coord.RegionY.ToString();
            txtRegionID.Text = coord.RegionID.ToString();
            txtCellX.Text = coord.CellX.ToString();
            txtCellY.Text = coord.CellY.ToString();
        }

        private void UpdateEntryList()
        {
            lstEntries.Items.Clear();
            foreach (var entry in _exporter.GetEntries())
            {
                lstEntries.Items.Add($"Map {entry.MapId} | Region {entry.RegionId} | Cell ({entry.CellX}, {entry.CellY})");
            }
        }

        private void UpdateStatistics()
        {
            lblStats.Text = _exporter.GetStatistics();
        }

        #endregion
    }
}
