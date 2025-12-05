using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.IO;
using MapTool.MapData;
using MapTool.Rendering;
using MapTool.Export;
using MapTool.NPC;
using MapTool.SPR;

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

        // Trap dragging state
        private bool _isDraggingTrap = false;
        private int _draggedTrapIndex = -1;
        private Point _trapDragStartPos;

        // Block selection for traps
        private bool _isSelectingTrapBlock = false;
        private Point _selectionStartPos;
        private Point _selectionCurrentPos;
        private List<int> _selectedTrapIndices = new List<int>();
        private bool _isDraggingTrapBlock = false;
        private Point _trapBlockDragStartPos;

        // NPC dragging state
        private bool _isDraggingNpc = false;
        private int _draggedNpcIndex = -1;
        private Point _npcDragStartPos;

        // NPC functionality
        private NpcLoader _npcLoader;
        private NpcExporter _npcExporter;
        private NpcResource _currentNpcResource;
        private SpriteData _currentNpcSprite;
        private int _currentAnimationFrame = 0;
        private float _previewZoom = 1.0f; // Default 1.0x zoom (no zoom)

        public MainFormSimple()
        {
            InitializeComponent();

            // Enable double buffering for smooth rendering
            mapPanel.GetType().InvokeMember("DoubleBuffered",
                System.Reflection.BindingFlags.SetProperty | System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.NonPublic,
                null, mapPanel, new object[] { true });

            // Initialize debug logger FIRST
            DebugLogger.Initialize();
            DebugLogger.Log("=== MapTool Started ===");
            DebugLogger.Log($"Executable: {System.Reflection.Assembly.GetExecutingAssembly().Location}");
            DebugLogger.Log($"Working Directory: {Environment.CurrentDirectory}");
            DebugLogger.Log($"Log file: {DebugLogger.GetLogFilePath()}");
            DebugLogger.LogSeparator();

            _renderer = new MapRenderer();
            _exporter = new TrapExporter();
            _npcExporter = new NpcExporter();

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

                // Initialize NPC Loader early for sprite tooltips
                if (_npcLoader == null)
                {
                    try
                    {
                        DebugLogger.Log("   Initializing NPC Loader for sprite tooltips...");
                        string clientPath = Path.Combine(_gameFolder, "client");
                        string serverPath = Path.Combine(_gameFolder, "library", "npc");

                        if (Directory.Exists(clientPath) && Directory.Exists(serverPath))
                        {
                            _npcLoader = new NpcLoader(clientPath, serverPath);
                            _npcLoader.LoadMappingFiles();
                            _renderer.SetNpcLoader(_npcLoader);
                            DebugLogger.Log("   ✓ NpcLoader initialized and set in MapRenderer");
                        }
                        else
                        {
                            DebugLogger.Log($"   ⚠ NPC paths not found - sprite tooltips disabled");
                            DebugLogger.Log($"      Client: {clientPath} (exists: {Directory.Exists(clientPath)})");
                            DebugLogger.Log($"      Server: {serverPath} (exists: {Directory.Exists(serverPath)})");
                        }
                    }
                    catch (Exception ex)
                    {
                        DebugLogger.Log($"   ⚠ Failed to init NpcLoader: {ex.Message}");
                        DebugLogger.Log($"      Sprite tooltips will not work");
                    }
                }

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
                // Check for Shift+drag to start block selection (only in Trap tab)
                if (ModifierKeys == Keys.Shift && tabMain.SelectedTab == tabTrap)
                {
                    _isSelectingTrapBlock = true;
                    _selectionStartPos = e.Location;
                    _selectionCurrentPos = e.Location;
                    _renderer.ClearTrapSelection();
                    mapPanel.Cursor = Cursors.Cross;
                    DebugLogger.Log($"[Block Selection] Started at ({e.X}, {e.Y})");
                    return;
                }

                // Check if clicking on selected trap block for dragging
                int trapIndex = _renderer.FindTrapMarkerAtScreenPosition(e.X, e.Y);
                List<int> selectedIndices = _renderer.GetSelectedTrapIndices();

                if (selectedIndices.Count > 0 && selectedIndices.Contains(trapIndex))
                {
                    // Start dragging the entire block
                    _isDraggingTrapBlock = true;
                    _trapBlockDragStartPos = e.Location;
                    mapPanel.Cursor = Cursors.SizeAll;
                    DebugLogger.Log($"[Block Drag] Started dragging {selectedIndices.Count} traps");
                    return;
                }

                // Check if clicking on a trap marker
                int npcIndex = _renderer.FindNpcMarkerAtScreenPosition(e.X, e.Y);

                if (trapIndex >= 0)
                {
                    // Clicked on a trap marker
                    _isDraggingTrap = true;
                    _draggedTrapIndex = trapIndex;
                    _trapDragStartPos = e.Location;
                    _renderer.SetSelectedTrapIndex(trapIndex);
                    _renderer.SetSelectedNpcIndex(-1);

                    // Select corresponding item in Trap Entries listbox
                    if (trapIndex < lstEntries.Items.Count)
                    {
                        lstEntries.SelectedIndex = trapIndex;
                        lstEntries.TopIndex = Math.Max(0, trapIndex - 2); // Scroll to show selected item
                    }

                    mapPanel.Cursor = Cursors.Hand;
                    mapPanel.Invalidate();
                }
                else if (npcIndex >= 0)
                {
                    // Clicked on an NPC marker
                    _isDraggingNpc = true;
                    _draggedNpcIndex = npcIndex;
                    _npcDragStartPos = e.Location;
                    _renderer.SetSelectedNpcIndex(npcIndex);
                    _renderer.SetSelectedTrapIndex(-1);

                    // Select corresponding item in NPC Entries listbox
                    if (npcIndex < lstNpcEntries.Items.Count)
                    {
                        lstNpcEntries.SelectedIndex = npcIndex;
                        lstNpcEntries.TopIndex = Math.Max(0, npcIndex - 2); // Scroll to show selected item
                    }

                    // Auto-fill NPC ID in Add NPC tab for preview
                    try
                    {
                        NpcEntry selectedNpc = _renderer.GetNpcMarker(npcIndex);
                        if (selectedNpc != null)
                        {
                            // Switch to NPC tab
                            tabMain.SelectedTab = tabNPC;

                            // Fill NPC ID textbox
                            txtNpcId.Text = selectedNpc.NpcID.ToString();

                            // Trigger load preview (simulate button click)
                            btnLoadNpcPreview_Click(this, EventArgs.Empty);

                            DebugLogger.Log($"[NPC Selected] Auto-filled NPC ID: {selectedNpc.NpcID} ({selectedNpc.Name})");
                        }
                    }
                    catch (Exception ex)
                    {
                        DebugLogger.Log($"[NPC Selected] Failed to auto-fill: {ex.Message}");
                    }

                    mapPanel.Cursor = Cursors.Hand;
                    mapPanel.Invalidate();
                }
                else
                {
                    // Normal cell selection
                    _selectedCoordinate = _renderer.ScreenToMapCoordinate(e.X, e.Y);
                    UpdateCoordinateDisplay();
                    _renderer.SetSelectedTrapIndex(-1);
                    _renderer.SetSelectedNpcIndex(-1);
                    mapPanel.Invalidate();
                }
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
            else if (e.Button == MouseButtons.Left && _isSelectingTrapBlock)
            {
                // Finish block selection
                _isSelectingTrapBlock = false;

                // Find all traps in selection rectangle
                int x = Math.Min(_selectionStartPos.X, _selectionCurrentPos.X);
                int y = Math.Min(_selectionStartPos.Y, _selectionCurrentPos.Y);
                int width = Math.Abs(_selectionCurrentPos.X - _selectionStartPos.X);
                int height = Math.Abs(_selectionCurrentPos.Y - _selectionStartPos.Y);

                Rectangle selRect = new Rectangle(x, y, width, height);
                List<int> selectedIndices = _renderer.FindTrapMarkersInRectangle(selRect);

                _selectedTrapIndices = selectedIndices;
                _renderer.SetSelectedTrapIndices(selectedIndices);
                _renderer.SetSelectionRectangle(null); // Clear selection rectangle

                DebugLogger.Log($"[Block Selection] Selected {selectedIndices.Count} traps");
                lblStatus.Text = $"Selected {selectedIndices.Count} traps. Click and drag to move them.";

                mapPanel.Cursor = Cursors.Default;
                mapPanel.Invalidate();
            }
            else if (e.Button == MouseButtons.Left && _isDraggingTrapBlock)
            {
                // Finish dragging trap block
                if (_selectedTrapIndices.Count > 0 && _currentMap != null)
                {
                    // Calculate delta
                    MapCoordinate startCoord = _renderer.ScreenToMapCoordinate(_trapBlockDragStartPos.X, _trapBlockDragStartPos.Y);
                    MapCoordinate endCoord = _renderer.ScreenToMapCoordinate(e.X, e.Y);
                    int deltaWorldX = endCoord.WorldX - startCoord.WorldX;
                    int deltaWorldY = endCoord.WorldY - startCoord.WorldY;

                    // Update all selected traps
                    _renderer.UpdateTrapMarkerPositions(_selectedTrapIndices, deltaWorldX, deltaWorldY);

                    // Update all trap entries in exporter
                    var entries = _exporter.GetEntries();
                    foreach (int trapIndex in _selectedTrapIndices)
                    {
                        if (trapIndex < entries.Count)
                        {
                            TrapMarker marker = _renderer.GetTrapMarker(trapIndex);
                            if (marker != null)
                            {
                                // Convert world coords back to region/cell
                                MapCoordinate newCoord = new MapCoordinate
                                {
                                    WorldX = marker.WorldX,
                                    WorldY = marker.WorldY
                                };

                                // Calculate region/cell from world coords
                                newCoord.RegionX = marker.WorldX / MapConstants.REGION_PIXEL_WIDTH;
                                newCoord.RegionY = marker.WorldY / MapConstants.REGION_PIXEL_HEIGHT;
                                newCoord.CellX = (marker.WorldX % MapConstants.REGION_PIXEL_WIDTH) / MapConstants.LOGIC_CELL_WIDTH;
                                newCoord.CellY = (marker.WorldY % MapConstants.REGION_PIXEL_HEIGHT) / MapConstants.LOGIC_CELL_HEIGHT;

                                int localRegionID = MapData.RegionData.MakeLocalRegionID(
                                    newCoord.RegionX, newCoord.RegionY,
                                    _currentMap.Config.RegionLeft, _currentMap.Config.RegionTop, _currentMap.Config.RegionWidth);

                                entries[trapIndex].RegionId = localRegionID;
                                entries[trapIndex].CellX = newCoord.CellX;
                                entries[trapIndex].CellY = newCoord.CellY;
                            }
                        }
                    }

                    UpdateTrapList();

                    DebugLogger.Log($"[Block Drag] Moved {_selectedTrapIndices.Count} traps by ({deltaWorldX}, {deltaWorldY})");
                    lblStatus.Text = $"Moved {_selectedTrapIndices.Count} traps. Shift+drag to select new block.";
                }

                _isDraggingTrapBlock = false;
                mapPanel.Cursor = Cursors.Default;
                mapPanel.Invalidate();
            }
            else if (e.Button == MouseButtons.Left && _isDraggingTrap)
            {
                // Finish dragging trap
                if (_draggedTrapIndex >= 0 && _currentMap != null)
                {
                    // Get new world coordinates from current mouse position
                    MapCoordinate newCoord = _renderer.ScreenToMapCoordinate(e.X, e.Y);

                    // Update trap marker position
                    _renderer.UpdateTrapMarkerPosition(_draggedTrapIndex, newCoord.WorldX, newCoord.WorldY);

                    // Update trap entry in exporter
                    var entries = _exporter.GetEntries();
                    if (_draggedTrapIndex < entries.Count)
                    {
                        // Calculate new local RegionID
                        int localRegionID = MapData.RegionData.MakeLocalRegionID(
                            newCoord.RegionX, newCoord.RegionY,
                            _currentMap.Config.RegionLeft, _currentMap.Config.RegionTop, _currentMap.Config.RegionWidth);

                        entries[_draggedTrapIndex].RegionId = localRegionID;
                        entries[_draggedTrapIndex].CellX = newCoord.CellX;
                        entries[_draggedTrapIndex].CellY = newCoord.CellY;

                        // Refresh trap list display
                        UpdateTrapList();

                        // Re-select the item
                        if (_draggedTrapIndex < lstEntries.Items.Count)
                        {
                            lstEntries.SelectedIndex = _draggedTrapIndex;
                        }
                    }

                    DebugLogger.Log($"[Trap Moved] Trap #{_draggedTrapIndex + 1} moved to World({newCoord.WorldX},{newCoord.WorldY}) Region({newCoord.RegionX},{newCoord.RegionY}) Cell({newCoord.CellX},{newCoord.CellY})");
                    lblStatus.Text = $"Trap #{_draggedTrapIndex + 1} moved to World({newCoord.WorldX},{newCoord.WorldY})";
                }

                _isDraggingTrap = false;
                _draggedTrapIndex = -1;
                mapPanel.Cursor = Cursors.Default;
                mapPanel.Invalidate();
            }
            else if (e.Button == MouseButtons.Left && _isDraggingNpc)
            {
                // Finish dragging NPC
                if (_draggedNpcIndex >= 0 && _currentMap != null)
                {
                    // Get new world coordinates from current mouse position
                    MapCoordinate newCoord = _renderer.ScreenToMapCoordinate(e.X, e.Y);

                    // Update NPC marker position
                    _renderer.UpdateNpcMarkerPosition(_draggedNpcIndex, newCoord.WorldX, newCoord.WorldY);

                    // Update NPC entry in exporter
                    var allNpcs = _npcExporter.GetEntries();
                    if (_draggedNpcIndex < allNpcs.Count)
                    {
                        allNpcs[_draggedNpcIndex].PosX = newCoord.WorldX;
                        allNpcs[_draggedNpcIndex].PosY = newCoord.WorldY;

                        // Refresh NPC list display
                        UpdateNpcList();

                        // Re-select the item
                        if (_draggedNpcIndex < lstNpcEntries.Items.Count)
                        {
                            lstNpcEntries.SelectedIndex = _draggedNpcIndex;
                        }
                    }

                    DebugLogger.Log($"[NPC Moved] NPC #{_draggedNpcIndex + 1} moved to World({newCoord.WorldX},{newCoord.WorldY})");
                    lblStatus.Text = $"NPC #{_draggedNpcIndex + 1} moved to World({newCoord.WorldX},{newCoord.WorldY})";
                }

                _isDraggingNpc = false;
                _draggedNpcIndex = -1;
                mapPanel.Cursor = Cursors.Default;
                mapPanel.Invalidate();
            }
        }

        // Map panel mouse move
        private void mapPanel_MouseMove(object sender, MouseEventArgs e)
        {
            if (_isSelectingTrapBlock)
            {
                // Update selection rectangle
                _selectionCurrentPos = e.Location;

                // Create rectangle from start to current position
                int x = Math.Min(_selectionStartPos.X, _selectionCurrentPos.X);
                int y = Math.Min(_selectionStartPos.Y, _selectionCurrentPos.Y);
                int width = Math.Abs(_selectionCurrentPos.X - _selectionStartPos.X);
                int height = Math.Abs(_selectionCurrentPos.Y - _selectionStartPos.Y);

                Rectangle selRect = new Rectangle(x, y, width, height);
                _renderer.SetSelectionRectangle(selRect);

                lblStatus.Text = $"Selecting traps: {width}x{height} pixels";
                mapPanel.Invalidate();
            }
            else if (_isDraggingTrapBlock && _selectedTrapIndices.Count > 0)
            {
                // Dragging block of traps
                int deltaX = e.X - _trapBlockDragStartPos.X;
                int deltaY = e.Y - _trapBlockDragStartPos.Y;

                // Convert screen delta to world delta
                MapCoordinate startCoord = _renderer.ScreenToMapCoordinate(_trapBlockDragStartPos.X, _trapBlockDragStartPos.Y);
                MapCoordinate currentCoord = _renderer.ScreenToMapCoordinate(e.X, e.Y);
                int deltaWorldX = currentCoord.WorldX - startCoord.WorldX;
                int deltaWorldY = currentCoord.WorldY - startCoord.WorldY;

                lblStatus.Text = $"Dragging {_selectedTrapIndices.Count} traps (Δ: {deltaWorldX}, {deltaWorldY})";
                mapPanel.Invalidate();
            }
            else if (_isPanning && mapPanel.AutoScroll)
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
            else if (_isDraggingTrap && _draggedTrapIndex >= 0)
            {
                // Dragging a trap marker - update visual feedback
                MapCoordinate coord = _renderer.ScreenToMapCoordinate(e.X, e.Y);
                lblStatus.Text = $"Dragging Trap #{_draggedTrapIndex + 1} to World: ({coord.WorldX}, {coord.WorldY}) | Region: ({coord.RegionX}, {coord.RegionY}) | Cell: ({coord.CellX}, {coord.CellY})";

                // Update trap position in real-time for visual feedback
                _renderer.UpdateTrapMarkerPosition(_draggedTrapIndex, coord.WorldX, coord.WorldY);
                mapPanel.Invalidate();
            }
            else if (_isDraggingNpc && _draggedNpcIndex >= 0)
            {
                // Dragging an NPC marker - update visual feedback
                MapCoordinate coord = _renderer.ScreenToMapCoordinate(e.X, e.Y);
                lblStatus.Text = $"Dragging NPC #{_draggedNpcIndex + 1} to World: ({coord.WorldX}, {coord.WorldY})";

                // Update NPC position in real-time for visual feedback
                _renderer.UpdateNpcMarkerPosition(_draggedNpcIndex, coord.WorldX, coord.WorldY);
                mapPanel.Invalidate();
            }
            else if (_currentMap != null)
            {
                // Check if hovering over a trap or NPC marker
                int hoveredTrapIndex = _renderer.FindTrapMarkerAtScreenPosition(e.X, e.Y);
                int hoveredNpcIndex = _renderer.FindNpcMarkerAtScreenPosition(e.X, e.Y);

                _renderer.SetHoveredTrapIndex(hoveredTrapIndex);
                _renderer.SetHoveredNpcIndex(hoveredNpcIndex);

                if (hoveredTrapIndex >= 0)
                {
                    // Hovering over a trap
                    mapPanel.Cursor = Cursors.Hand;
                    lblStatus.Text = $"Trap #{hoveredTrapIndex + 1} - Click to select, Drag to move";
                    mapPanel.Invalidate(); // Redraw to show tooltip
                }
                else if (hoveredNpcIndex >= 0)
                {
                    // Hovering over an NPC
                    mapPanel.Cursor = Cursors.Hand;
                    lblStatus.Text = $"NPC #{hoveredNpcIndex + 1} - Click to select, Drag to move";
                    mapPanel.Invalidate(); // Redraw to show tooltip
                }
                else
                {
                    // Not hovering over anything
                    mapPanel.Cursor = Cursors.Default;

                    // Show coordinate under mouse
                    MapCoordinate coord = _renderer.ScreenToMapCoordinate(e.X, e.Y);
                    lblStatus.Text = $"World: ({coord.WorldX}, {coord.WorldY}) | Region: ({coord.RegionX}, {coord.RegionY}) | Cell: ({coord.CellX}, {coord.CellY})";
                    mapPanel.Invalidate(); // Redraw to clear tooltip
                }
            }
        }

        // Map panel double-click to add entry (trap or NPC based on active tab)
        private void mapPanel_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left && _selectedCoordinate.HasValue && _currentMap != null)
            {
                // Check which tab is active
                if (tabMain.SelectedTab == tabTrap)
                {
                    // Add trap entry
                    string scriptFile = txtScriptFile.Text;
                    if (string.IsNullOrWhiteSpace(scriptFile))
                    {
                        scriptFile = $@"\script\maps\trap\{_currentMap.MapId}\1.lua";
                    }

                    // Use LOCAL RegionID for trap export (relative to map rect)
                    _exporter.AddEntry(_currentMap.MapId, _selectedCoordinate.Value, _currentMap.Config, scriptFile);
                    UpdateTrapList();

                    // Update trap markers to render on map immediately
                    var trapEntries = _exporter.GetEntries();
                    var trapMarkers = MapRenderer.ConvertTrapEntriesToMarkers(trapEntries, _currentMap.Config);
                    _renderer.SetTrapMarkers(trapMarkers);
                    mapPanel.Invalidate();

                    // Calculate local RegionID for display
                    int localRegionID = RegionData.MakeLocalRegionID(
                        _selectedCoordinate.Value.RegionX, _selectedCoordinate.Value.RegionY,
                        _currentMap.Config.RegionLeft, _currentMap.Config.RegionTop, _currentMap.Config.RegionWidth);

                    lblStatus.Text = $"Added trap entry at Region({_selectedCoordinate.Value.RegionX},{_selectedCoordinate.Value.RegionY}) LocalID={localRegionID} Cell({_selectedCoordinate.Value.CellX},{_selectedCoordinate.Value.CellY})";
                    DebugLogger.Log($"[Trap Added] Region({_selectedCoordinate.Value.RegionX},{_selectedCoordinate.Value.RegionY}) GlobalID={_selectedCoordinate.Value.RegionID} LocalID={localRegionID} Cell({_selectedCoordinate.Value.CellX},{_selectedCoordinate.Value.CellY})");
                }
                else if (tabMain.SelectedTab == tabNPC)
                {
                    // Add NPC entry
                    if (_currentNpcResource == null)
                    {
                        MessageBox.Show("Please load an NPC preview first!", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        return;
                    }

                    string npcIdText = txtNpcId.Text.Trim();
                    if (string.IsNullOrEmpty(npcIdText) || !int.TryParse(npcIdText, out int npcId))
                    {
                        MessageBox.Show("Please enter a valid NPC ID!", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        return;
                    }

                    string npcLevelText = txtNpcLevel.Text.Trim();
                    if (string.IsNullOrEmpty(npcLevelText) || !int.TryParse(npcLevelText, out int npcLevel))
                    {
                        MessageBox.Show("Please enter a valid Level (number)!", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        return;
                    }

                    // Add NPC at selected position
                    // Get NPC name - fallback to NPC_{ID} if name is null, empty, or "0"
                    string npcName = (!string.IsNullOrEmpty(_currentNpcResource.NpcName) && _currentNpcResource.NpcName != "0")
                        ? _currentNpcResource.NpcName
                        : $"NPC_{npcId}";

                    _npcExporter.AddEntry(
                        npcId: npcId,
                        mapId: _currentMap.MapId,
                        posX: _selectedCoordinate.Value.WorldX,
                        posY: _selectedCoordinate.Value.WorldY,
                        scriptFile: "",  // Empty script for NPCs
                        name: npcName,
                        level: npcLevel,
                        isLoad: 1
                    );

                    UpdateNpcList();

                    // Update renderer with current map's NPCs to render on map immediately
                    var mapNpcs = _npcExporter.GetEntriesForMap(_currentMap.MapId);
                    _renderer.SetNpcMarkers(mapNpcs);
                    mapPanel.Invalidate();

                    lblStatus.Text = $"Added NPC ID {npcId} ({_currentNpcResource.NpcName}) at World({_selectedCoordinate.Value.WorldX},{_selectedCoordinate.Value.WorldY})";
                    DebugLogger.Log($"[NPC Added] ID: {npcId}, Name: {_currentNpcResource.NpcName}, Pos: World({_selectedCoordinate.Value.WorldX},{_selectedCoordinate.Value.WorldY})");

                    // Trigger map redraw to show new NPC
                    mapPanel.Invalidate();
                }
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

        // Load trap file button
        private void btnLoadTrapFile_Click(object sender, EventArgs e)
        {
            if (_currentMap == null)
            {
                MessageBox.Show("Please load a map first!", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            // Open file dialog to select trap file
            using (OpenFileDialog dialog = new OpenFileDialog())
            {
                dialog.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
                dialog.Title = "Load Trap File";

                // Try to default to library/maps/Trap folder with map ID as filename
                string trapFolder = Path.Combine(_gameFolder, "library", "maps", "Trap");
                string defaultFile = Path.Combine(trapFolder, $"{_currentMap.MapId}.txt");

                if (Directory.Exists(trapFolder))
                {
                    dialog.InitialDirectory = trapFolder;
                    if (File.Exists(defaultFile))
                    {
                        dialog.FileName = $"{_currentMap.MapId}.txt";
                    }
                }

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        Cursor = Cursors.WaitCursor;

                        // Import trap file
                        _exporter.ImportFromTrapFile(dialog.FileName);

                        // Update trap list display
                        UpdateTrapList();

                        // Convert trap entries to markers with world coordinates and update renderer
                        var trapEntries = _exporter.GetEntries();
                        var trapMarkers = MapRenderer.ConvertTrapEntriesToMarkers(trapEntries, _currentMap.Config);
                        _renderer.SetTrapMarkers(trapMarkers);

                        // Refresh map display
                        mapPanel.Invalidate();

                        MessageBox.Show($"Loaded {_exporter.GetEntries().Count} trap entries from:\n{dialog.FileName}",
                            "Load Complete", MessageBoxButtons.OK, MessageBoxIcon.Information);

                        lblStatus.Text = $"Loaded {_exporter.GetEntries().Count} trap entries from {Path.GetFileName(dialog.FileName)}";
                        DebugLogger.Log($"[Trap Import] Loaded {_exporter.GetEntries().Count} trap entries from {dialog.FileName}");
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show($"Failed to load trap file:\n{ex.Message}", "Error",
                            MessageBoxButtons.OK, MessageBoxIcon.Error);
                        DebugLogger.Log($"ERROR loading trap file: {ex}");
                    }
                    finally
                    {
                        Cursor = Cursors.Default;
                    }
                }
            }
        }

        // Extract All Regions button - export all trap cells from loaded regions
        private void btnExtractAllRegions_Click(object sender, EventArgs e)
        {
            if (_currentMap == null || _currentMap.Regions.Count == 0)
            {
                MessageBox.Show("No regions loaded!\n\nLoad a map first.",
                    "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            // Ask user for save location
            using (SaveFileDialog dialog = new SaveFileDialog())
            {
                dialog.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
                dialog.FileName = $"{_currentMap.MapId}_all_traps.txt";
                dialog.DefaultExt = "txt";
                dialog.Title = "Extract All Region Traps";

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        Cursor = Cursors.WaitCursor;

                        int trapCount = 0;
                        int regionCount = 0;
                        string scriptFile = txtScriptFile.Text;

                        DebugLogger.LogSeparator();
                        DebugLogger.Log($"📝 EXTRACTING ALL TRAPS FROM LOADED REGIONS");
                        DebugLogger.Log($"   Output file: {dialog.FileName}");
                        DebugLogger.Log($"   Map ID: {_currentMap.MapId}");
                        DebugLogger.Log($"   Total loaded regions: {_currentMap.Regions.Count}");

                        using (StreamWriter writer = new StreamWriter(dialog.FileName, false, System.Text.Encoding.GetEncoding("Windows-1252")))
                        {
                            // Write header
                            writer.WriteLine("MapId\tRegionId\tCellX\tCellY\tScriptFile\tIsLoad");

                            // Get map rect for local RegionID calculation
                            int minX = _currentMap.Config.RegionLeft;
                            int minY = _currentMap.Config.RegionTop;
                            int width = _currentMap.Config.RegionWidth;

                            // Iterate through all loaded regions
                            foreach (var region in _currentMap.Regions.Values)
                            {
                                if (!region.IsLoaded)
                                    continue;

                                // Calculate LOCAL RegionID (relative to map rect)
                                int localRegionID = region.GetLocalRegionID(minX, minY, width);
                                int regionTrapCount = 0;

                                // Scan all cells in region for traps
                                for (int cy = 0; cy < MapConstants.REGION_GRID_HEIGHT; cy++)
                                {
                                    for (int cx = 0; cx < MapConstants.REGION_GRID_WIDTH; cx++)
                                    {
                                        if (region.Traps[cx, cy] != 0)
                                        {
                                            writer.WriteLine($"{_currentMap.MapId}\t{localRegionID}\t{cx}\t{cy}\t{scriptFile}\t1");
                                            trapCount++;
                                            regionTrapCount++;
                                        }
                                    }
                                }

                                if (regionTrapCount > 0)
                                {
                                    regionCount++;
                                    if (regionCount <= 5)
                                        DebugLogger.Log($"   Region ({region.RegionX},{region.RegionY}) Global={region.RegionID} Local={localRegionID}: {regionTrapCount} traps");
                                }
                            }
                        }

                        DebugLogger.Log($"✓ Extraction completed!");
                        DebugLogger.Log($"   Total trap cells: {trapCount}");
                        DebugLogger.Log($"   Regions with traps: {regionCount}/{_currentMap.Regions.Count}");
                        DebugLogger.LogSeparator();

                        MessageBox.Show($"Extracted successfully!\n\n" +
                            $"File: {Path.GetFileName(dialog.FileName)}\n" +
                            $"Total Trap Cells: {trapCount}\n" +
                            $"Regions with Traps: {regionCount}/{_currentMap.Regions.Count}",
                            "Extract Complete", MessageBoxButtons.OK, MessageBoxIcon.Information);

                        lblStatus.Text = $"Extracted {trapCount} trap cells from {regionCount} regions";
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show($"Failed to extract traps:\n{ex.Message}",
                            "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        DebugLogger.Log($"ERROR extracting all regions: {ex}");
                    }
                    finally
                    {
                        Cursor = Cursors.Default;
                    }
                }
            }
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

                    using (StreamWriter writer = new StreamWriter(dialog.FileName, false, System.Text.Encoding.GetEncoding("Windows-1252")))
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

        // ===== NPC EVENT HANDLERS =====

        // Load NPC preview button
        private void btnLoadNpcPreview_Click(object sender, EventArgs e)
        {
            DebugLogger.Log("--------------------------------------------------------------------------------");
            DebugLogger.Log("🎯 LOADING NPC PREVIEW");

            string searchText = txtNpcId.Text.Trim();
            DebugLogger.Log($"   Search input: '{searchText}'");

            if (string.IsNullOrEmpty(searchText))
            {
                MessageBox.Show("Please enter NPC ID or Name!", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                DebugLogger.Log("   ✗ ERROR: Search text is empty");
                return;
            }

            int npcId;
            // Try parse as number first
            if (!int.TryParse(searchText, out npcId))
            {
                // Not a number, search by name
                DebugLogger.Log($"   Not a number, searching by name: '{searchText}'");
                // Need to initialize loader first to search by name
                // (will be done below, so we'll defer the search)
                npcId = -1; // Mark as "search by name"
            }
            else
            {
                DebugLogger.Log($"   Parsed as NPC ID: {npcId}");
            }

            // Get client and server paths
            string clientPath = _gameFolder;
            string serverPath = _gameFolder;

            DebugLogger.Log($"   Game folder: {_gameFolder}");
            DebugLogger.Log($"   Mode: {(_isServerMode ? "Server" : "Client")}");

            if (_isServerMode)
            {
                // If in server mode, try to find client folder
                DirectoryInfo serverDir = new DirectoryInfo(_gameFolder);
                string possibleClientPath = Path.Combine(serverDir.Parent.FullName, "Client");
                DebugLogger.Log($"   Looking for Client folder: {possibleClientPath}");
                if (Directory.Exists(possibleClientPath))
                {
                    clientPath = possibleClientPath;
                    DebugLogger.Log($"   ✓ Found Client folder: {clientPath}");
                }
                else
                {
                    MessageBox.Show("Cannot find Client folder!\n\nNPC resources are in Client folder. Please select Client folder or switch to Client mode.",
                        "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    DebugLogger.Log($"   ✗ ERROR: Client folder not found at {possibleClientPath}");
                    return;
                }
            }
            else
            {
                // If in client mode, try to find server folder
                DirectoryInfo clientDir = new DirectoryInfo(_gameFolder);
                string possibleServerPath = Path.Combine(clientDir.Parent.FullName, "Server");
                DebugLogger.Log($"   Looking for Server folder: {possibleServerPath}");
                if (Directory.Exists(possibleServerPath))
                {
                    serverPath = possibleServerPath;
                    DebugLogger.Log($"   ✓ Found Server folder: {serverPath}");
                }
                else
                {
                    MessageBox.Show("Cannot find Server folder!\n\nNpcs.txt is in Server folder. Please make sure both Client and Server folders are accessible.",
                        "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    DebugLogger.Log($"   ✗ ERROR: Server folder not found at {possibleServerPath}");
                    return;
                }
            }

            try
            {
                Cursor = Cursors.WaitCursor;
                lblStatus.Text = "Loading NPC resource...";

                // Initialize NPC loader if needed
                if (_npcLoader == null)
                {
                    DebugLogger.Log("   Initializing NPC Loader...");
                    _npcLoader = new NpcLoader(clientPath, serverPath);
                    _npcLoader.LoadMappingFiles();
                    DebugLogger.Log($"   ✓ NPC Loader initialized");
                    DebugLogger.Log($"      Client path: {clientPath}");
                    DebugLogger.Log($"      Server path: {serverPath}");

                    // Set NpcLoader in MapRenderer for sprite rendering
                    _renderer.SetNpcLoader(_npcLoader);
                    DebugLogger.Log("   ✓ NpcLoader set in MapRenderer for sprite tooltips");
                }
                else
                {
                    DebugLogger.Log("   Using existing NPC Loader instance");
                }

                // If search was by name, find the NPC ID first
                if (npcId == -1)
                {
                    DebugLogger.Log($"   Searching for NPC by name: '{searchText}'");
                    int? foundId = _npcLoader.FindNpcIdByName(searchText);
                    if (foundId == null)
                    {
                        MessageBox.Show($"NPC with name '{searchText}' not found!\n\nTry entering the NPC ID instead.",
                            "Not Found", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        lblStatus.Text = $"NPC '{searchText}' not found";
                        DebugLogger.Log($"   ✗ ERROR: No NPC found with name '{searchText}'");
                        return;
                    }
                    npcId = foundId.Value;
                    DebugLogger.Log($"   ✓ Found NPC ID {npcId} for name '{searchText}'");
                }

                // Load NPC resource by ID
                DebugLogger.Log($"   Calling GetNpcResourceById({npcId})...");
                _currentNpcResource = _npcLoader.GetNpcResourceById(npcId);
                if (_currentNpcResource == null)
                {
                    MessageBox.Show($"NPC ID {npcId} not found in Npcs.txt!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    lblStatus.Text = $"NPC ID {npcId} not found";
                    DebugLogger.Log($"   ✗ ERROR: GetNpcResourceById returned null for ID {npcId}");
                    return;
                }

                DebugLogger.Log($"   ✓ NPC resource loaded: {_currentNpcResource.NpcName}");

                // Auto-load default sprite (NormalStand)
                NpcAction action = NpcAction.NormalStand;

                // Load SPR file (from PAK or disk)
                DebugLogger.Log($"   Loading sprite for action: {action}");
                _currentNpcSprite = _npcLoader.LoadSpriteById(npcId, action);
                if (_currentNpcSprite == null)
                {
                    MessageBox.Show($"Failed to load sprite for NPC ID {npcId}\n\nAction: {action}\nNPC: {_currentNpcResource.NpcName}\n\nCheck debug log for details.",
                        "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    lblStatus.Text = $"Failed to load sprite";
                    return;
                }

                // Render first frame to preview
                _currentAnimationFrame = 0;
                _previewZoom = 1.0f; // Reset zoom to 1.0x (no zoom)
                if (_currentNpcSprite.FrameCount > 0)
                {
                    RenderNpcPreviewFrame(0);
                }

                // Update info labels - show NPC Name first, then ResType
                string npcName = _currentNpcResource.NpcName;
                string resType = _currentNpcResource.ResKind?.CharacterName ?? "Unknown";

                // Display format: "Name" (ResType) or just ResType if no name
                string displayName = (!string.IsNullOrEmpty(npcName) && npcName != "0")
                    ? $"{npcName} ({resType})"
                    : resType;

                lblNpcName.Text = $"ID {npcId}: {displayName} - {_currentNpcSprite.FrameCount} frames";
                lblFrameInfo.Text = $"Frame: 1 / {_currentNpcSprite.FrameCount}";
                lblStatus.Text = $"Loaded NPC ID {npcId}: {displayName}";

                // Enable play button if there are multiple frames
                btnPlayAnimation.Enabled = _currentNpcSprite.FrameCount > 1;

                DebugLogger.Log($"[NPC Loaded] ID: {npcId}, Name: {_currentNpcResource.NpcName}, Frames: {_currentNpcSprite.FrameCount}");
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Failed to load NPC:\n{ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                lblStatus.Text = "Failed to load NPC";
                DebugLogger.Log($"ERROR loading NPC ID {npcId}: {ex}");
            }
            finally
            {
                Cursor = Cursors.Default;
            }
        }

        // Render NPC preview frame with zoom
        private void RenderNpcPreviewFrame(int frameIndex)
        {
            if (_currentNpcSprite == null || frameIndex < 0 || frameIndex >= _currentNpcSprite.FrameCount)
                return;

            try
            {
                // Get original frame
                Bitmap originalFrame = SpriteLoader.FrameToBitmap(_currentNpcSprite, frameIndex);

                // Apply zoom
                int scaledWidth = (int)(originalFrame.Width * _previewZoom);
                int scaledHeight = (int)(originalFrame.Height * _previewZoom);

                Bitmap scaledFrame = new Bitmap(scaledWidth, scaledHeight);
                using (Graphics g = Graphics.FromImage(scaledFrame))
                {
                    g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
                    g.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.Half;
                    g.DrawImage(originalFrame, 0, 0, scaledWidth, scaledHeight);
                }

                // Dispose old image
                if (picNpcPreview.Image != null)
                {
                    var oldImage = picNpcPreview.Image;
                    picNpcPreview.Image = null;
                    oldImage.Dispose();
                }

                picNpcPreview.Image = scaledFrame;
                originalFrame.Dispose();
            }
            catch (Exception ex)
            {
                DebugLogger.Log($"ERROR rendering preview frame: {ex.Message}");
            }
        }

        // Preview zoom in button
        private void btnPreviewZoomIn_Click(object sender, EventArgs e)
        {
            _previewZoom = Math.Min(8.0f, _previewZoom * 1.5f);
            RenderNpcPreviewFrame(_currentAnimationFrame);
        }

        // Preview zoom out button
        private void btnPreviewZoomOut_Click(object sender, EventArgs e)
        {
            _previewZoom = Math.Max(0.5f, _previewZoom / 1.5f);
            RenderNpcPreviewFrame(_currentAnimationFrame);
        }

        // Play/Stop animation button
        private void btnPlayAnimation_Click(object sender, EventArgs e)
        {
            if (_currentNpcSprite == null || _currentNpcSprite.FrameCount <= 1)
                return;

            if (animationTimer.Enabled)
            {
                // Stop animation
                animationTimer.Stop();
                btnPlayAnimation.Text = "▶ Play";
            }
            else
            {
                // Start animation
                animationTimer.Start();
                btnPlayAnimation.Text = "⏸ Stop";
            }
        }

        // Animation timer tick - advance to next frame
        private void animationTimer_Tick(object sender, EventArgs e)
        {
            if (_currentNpcSprite == null || _currentNpcSprite.FrameCount <= 1)
            {
                animationTimer.Stop();
                return;
            }

            try
            {
                // Advance to next frame
                _currentAnimationFrame = (_currentAnimationFrame + 1) % _currentNpcSprite.FrameCount;

                // Render frame with zoom
                RenderNpcPreviewFrame(_currentAnimationFrame);

                // Update frame info
                lblFrameInfo.Text = $"Frame: {_currentAnimationFrame + 1} / {_currentNpcSprite.FrameCount}";
            }
            catch (Exception ex)
            {
                animationTimer.Stop();
                btnPlayAnimation.Text = "▶ Play";
                DebugLogger.Log($"ERROR in animation: {ex.Message}");
            }
        }

        // Load NPCs from server button
        private void btnLoadNpcsFromServer_Click(object sender, EventArgs e)
        {
            if (_currentMap == null)
            {
                MessageBox.Show("Please load a map first!", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            // Ask user for Npc_Load.txt file
            using (OpenFileDialog dialog = new OpenFileDialog())
            {
                dialog.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
                dialog.FileName = "Npc_Load.txt";
                dialog.Title = "Load NPCs from Server";

                // Try to default to server/library/maps folder
                string defaultPath = Path.Combine(_gameFolder, "library", "maps", "Npc_Load.txt");
                if (File.Exists(defaultPath))
                {
                    dialog.InitialDirectory = Path.GetDirectoryName(defaultPath);
                    dialog.FileName = "Npc_Load.txt";
                }

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        Cursor = Cursors.WaitCursor;

                        // Import NPCs
                        _npcExporter.ImportFromFile(dialog.FileName);

                        // Filter for current map
                        var mapNpcs = _npcExporter.GetEntriesForMap(_currentMap.MapId);

                        // Update list
                        UpdateNpcList();

                        // Update renderer with NPC markers
                        _renderer.SetNpcMarkers(mapNpcs);

                        MessageBox.Show($"Loaded {_npcExporter.GetEntries().Count} NPCs total\n" +
                                        $"{mapNpcs.Count} NPCs for map {_currentMap.MapId}",
                            "NPCs Loaded", MessageBoxButtons.OK, MessageBoxIcon.Information);

                        lblStatus.Text = $"Loaded {mapNpcs.Count} NPCs for map {_currentMap.MapId}";
                        DebugLogger.Log($"[NPC Import] Loaded {_npcExporter.GetEntries().Count} NPCs, {mapNpcs.Count} for map {_currentMap.MapId}");

                        // Trigger map redraw to show NPC positions
                        mapPanel.Invalidate();
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show($"Failed to load NPCs:\n{ex.Message}", "Error",
                            MessageBoxButtons.OK, MessageBoxIcon.Error);
                        DebugLogger.Log($"ERROR importing NPCs: {ex}");
                    }
                    finally
                    {
                        Cursor = Cursors.Default;
                    }
                }
            }
        }

        // Update NPC entry list
        private void UpdateNpcList()
        {
            lstNpcEntries.Items.Clear();

            // Show only NPCs for current map
            if (_currentMap != null)
            {
                var mapNpcs = _npcExporter.GetEntriesForMap(_currentMap.MapId);
                foreach (var entry in mapNpcs)
                {
                    lstNpcEntries.Items.Add(entry);
                }
            }
            else
            {
                foreach (var entry in _npcExporter.GetEntries())
                {
                    lstNpcEntries.Items.Add(entry);
                }
            }
        }

        // Extract NPC list button
        private void btnExtractNpcList_Click(object sender, EventArgs e)
        {
            if (_npcExporter.GetEntries().Count == 0)
            {
                MessageBox.Show("No NPC entries to export!\n\nDouble-click on the map to add NPCs to the list first.",
                    "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            // Ask user for save location
            using (SaveFileDialog dialog = new SaveFileDialog())
            {
                dialog.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
                dialog.FileName = _currentMap != null ? $"Npc_Load_{_currentMap.MapId}.txt" : "Npc_Load.txt";
                dialog.DefaultExt = "txt";
                dialog.Title = "Export NPC List";

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        _npcExporter.ExportToFile(dialog.FileName);

                        string stats = _npcExporter.GetStatistics();
                        MessageBox.Show($"Exported successfully!\n\nFile: {dialog.FileName}\n\n{stats}",
                            "Export Complete", MessageBoxButtons.OK, MessageBoxIcon.Information);

                        lblStatus.Text = $"Exported {_npcExporter.GetEntries().Count} NPCs to {Path.GetFileName(dialog.FileName)}";
                        DebugLogger.Log($"[NPC Export] {_npcExporter.GetEntries().Count} entries to {dialog.FileName}");
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show($"Failed to export:\n{ex.Message}", "Error",
                            MessageBoxButtons.OK, MessageBoxIcon.Error);
                        DebugLogger.Log($"ERROR exporting NPCs: {ex}");
                    }
                }
            }
        }

        // Remove selected NPC button
        private void btnRemoveLastNpc_Click(object sender, EventArgs e)
        {
            if (lstNpcEntries.SelectedIndex >= 0)
            {
                int selectedIndex = lstNpcEntries.SelectedIndex;
                if (_npcExporter.RemoveAt(selectedIndex))
                {
                    UpdateNpcList();

                    // Update renderer with current map's NPCs
                    if (_currentMap != null)
                    {
                        var mapNpcs = _npcExporter.GetEntriesForMap(_currentMap.MapId);
                        _renderer.SetNpcMarkers(mapNpcs);
                        mapPanel.Invalidate();
                    }

                    lblStatus.Text = $"Removed NPC entry at index {selectedIndex}";
                }
            }
            else
            {
                MessageBox.Show("Please select an NPC entry to remove!", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        // Clear all NPCs button
        private void btnClearNpcs_Click(object sender, EventArgs e)
        {
            if (_npcExporter.GetEntries().Count == 0)
            {
                MessageBox.Show("No NPC entries to clear!", "Information", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            DialogResult result = MessageBox.Show(
                $"Are you sure you want to remove all {_npcExporter.GetEntries().Count} NPC entries?",
                "Confirm Clear All",
                MessageBoxButtons.YesNo,
                MessageBoxIcon.Question);

            if (result == DialogResult.Yes)
            {
                _npcExporter.Clear();
                UpdateNpcList();

                // Update renderer with current map's NPCs (should be empty now)
                if (_currentMap != null)
                {
                    _renderer.SetNpcMarkers(new List<NpcEntry>());
                    mapPanel.Invalidate();
                }

                lblStatus.Text = "Cleared all NPC entries";
            }
        }
    }
}
