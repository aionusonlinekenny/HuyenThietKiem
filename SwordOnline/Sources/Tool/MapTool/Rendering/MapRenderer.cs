using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.IO;
using MapTool.MapData;
using MapTool.NPC;
using MapTool.SPR;
using MapRegionData = MapTool.MapData.RegionData;

namespace MapTool.Rendering
{
    /// <summary>
    /// Trap marker with world coordinates for rendering
    /// </summary>
    public class TrapMarker
    {
        public int WorldX { get; set; }
        public int WorldY { get; set; }
        public string ScriptFile { get; set; }
    }

    /// <summary>
    /// Renders map regions and cells to a Graphics surface
    /// </summary>
    public class MapRenderer
    {
        private Dictionary<int, MapRegionData> _loadedRegions;
        private int _cellSize = 16; // Render size per cell (pixels on screen)
        private int _viewOffsetX = 0;
        private int _viewOffsetY = 0;
        private float _zoom = 1.0f;

        // Map background image (24.jpg)
        private Image _mapImage = null;
        private int _mapImageOffsetX = 0;
        private int _mapImageOffsetY = 0;

        // NPC markers
        private List<NpcEntry> _npcMarkers = new List<NpcEntry>();
        private int _hoveredNpcIndex = -1;
        private int _selectedNpcIndex = -1;

        // Trap markers (from loaded trap file)
        private List<TrapMarker> _trapMarkers = new List<TrapMarker>();
        private int _hoveredTrapIndex = -1;
        private int _selectedTrapIndex = -1;
        private List<int> _selectedTrapIndices = new List<int>();

        // Selection rectangle
        private Rectangle? _selectionRectangle = null;

        // NPC sprite loader
        private NpcLoader _npcLoader = null;

        // Colors - Simple overlay visualization
        private Color _gridColor = Color.FromArgb(80, 100, 100, 120);  // Subtle grid
        private Color _regionBorderColor = Color.FromArgb(150, 80, 120, 255);  // Semi-transparent border
        private Color _walkableCellColor = Color.FromArgb(60, 0, 255, 0);       // Semi-transparent GREEN for walkable areas
        private Color _trapColor = Color.FromArgb(200, 255, 0, 0);              // RED for traps
        private Color _npcColor = Color.FromArgb(200, 200, 0, 255);             // PURPLE for NPCs
        private Color _objectColor = Color.FromArgb(200, 255, 255, 0);          // YELLOW for objects
        private Color _selectedCellColor = Color.FromArgb(200, 0, 255, 0);      // Bright green selection
        private Color _backgroundColor = Color.FromArgb(255, 140, 180, 140);     // Green background like terrain

        public int CellSize
        {
            get => _cellSize;
            set => _cellSize = Math.Max(4, Math.Min(64, value));
        }

        public float Zoom
        {
            get => _zoom;
            set => _zoom = Math.Max(0.1f, Math.Min(4.0f, value));
        }

        public int ViewOffsetX { get => _viewOffsetX; set => _viewOffsetX = value; }
        public int ViewOffsetY { get => _viewOffsetY; set => _viewOffsetY = value; }

        public MapRenderer()
        {
            _loadedRegions = new Dictionary<int, MapRegionData>();
        }

        public void AddRegion(MapRegionData region)
        {
            _loadedRegions[region.RegionID] = region;
        }

        public void ClearRegions()
        {
            _loadedRegions.Clear();
        }

        /// <summary>
        /// Set NPC markers to display on map
        /// </summary>
        public void SetNpcMarkers(List<NpcEntry> npcs)
        {
            _npcMarkers = npcs ?? new List<NpcEntry>();

            // Debug logging
            DebugLogger.Log($"[MapRenderer] SetNpcMarkers: {_npcMarkers.Count} NPCs");
            foreach (var npc in _npcMarkers)
            {
                int mapX = npc.PosX / MapConstants.MAP_SCALE_H;
                int mapY = npc.PosY / MapConstants.MAP_SCALE_V;
                DebugLogger.Log($"   NPC {npc.NpcID}: World({npc.PosX},{npc.PosY}) → Map({mapX},{mapY})");
            }
        }

        /// <summary>
        /// Set NPC loader for loading NPC sprites
        /// </summary>
        public void SetNpcLoader(NpcLoader npcLoader)
        {
            _npcLoader = npcLoader;
        }

        /// <summary>
        /// Clear NPC markers
        /// </summary>
        public void ClearNpcMarkers()
        {
            _npcMarkers.Clear();
        }

        /// <summary>
        /// Set hovered NPC index for highlighting
        /// </summary>
        public void SetHoveredNpcIndex(int index)
        {
            _hoveredNpcIndex = index;
        }

        /// <summary>
        /// Set selected NPC index for highlighting
        /// </summary>
        public void SetSelectedNpcIndex(int index)
        {
            _selectedNpcIndex = index;
        }

        /// <summary>
        /// Find NPC marker at screen position (returns -1 if not found)
        /// </summary>
        public int FindNpcMarkerAtScreenPosition(int screenX, int screenY)
        {
            if (_npcMarkers == null || _npcMarkers.Count == 0)
                return -1;

            int markerSize = 8;
            int hitRadius = markerSize / 2 + 3; // Slightly larger hit area

            for (int i = 0; i < _npcMarkers.Count; i++)
            {
                var npc = _npcMarkers[i];

                // Convert NPC world coords to map coords
                int mapX = npc.PosX / MapConstants.MAP_SCALE_H;
                int mapY = npc.PosY / MapConstants.MAP_SCALE_V;

                // Convert to screen coords (including zoom)
                int markerScreenX = (int)((mapX - _viewOffsetX) * _zoom);
                int markerScreenY = (int)((mapY - _viewOffsetY) * _zoom);

                // Check if mouse is within hit radius
                int dx = screenX - markerScreenX;
                int dy = screenY - markerScreenY;
                int distanceSquared = dx * dx + dy * dy;

                if (distanceSquared <= hitRadius * hitRadius)
                {
                    return i;
                }
            }

            return -1;
        }

        /// <summary>
        /// Get NPC marker by index
        /// </summary>
        public NpcEntry GetNpcMarker(int index)
        {
            if (index >= 0 && index < _npcMarkers.Count)
                return _npcMarkers[index];
            return null;
        }

        /// <summary>
        /// Update NPC marker position
        /// </summary>
        public void UpdateNpcMarkerPosition(int index, int worldX, int worldY)
        {
            if (index >= 0 && index < _npcMarkers.Count)
            {
                _npcMarkers[index].PosX = worldX;
                _npcMarkers[index].PosY = worldY;
            }
        }

        /// <summary>
        /// Set Trap markers to display on map
        /// </summary>
        public void SetTrapMarkers(List<TrapMarker> traps)
        {
            _trapMarkers = traps ?? new List<TrapMarker>();

            // Debug logging
            DebugLogger.Log($"[MapRenderer] SetTrapMarkers: {_trapMarkers.Count} Traps");
        }

        /// <summary>
        /// Clear Trap markers
        /// </summary>
        public void ClearTrapMarkers()
        {
            _trapMarkers.Clear();
        }

        /// <summary>
        /// Set hovered trap index for highlighting
        /// </summary>
        public void SetHoveredTrapIndex(int index)
        {
            _hoveredTrapIndex = index;
        }

        /// <summary>
        /// Set selected trap index for highlighting
        /// </summary>
        public void SetSelectedTrapIndex(int index)
        {
            _selectedTrapIndex = index;
        }

        /// <summary>
        /// Find trap marker at screen position (returns -1 if not found)
        /// </summary>
        public int FindTrapMarkerAtScreenPosition(int screenX, int screenY)
        {
            if (_trapMarkers == null || _trapMarkers.Count == 0)
                return -1;

            int markerSize = 6;
            int hitRadius = markerSize / 2 + 2; // Slightly larger hit area

            for (int i = 0; i < _trapMarkers.Count; i++)
            {
                var trap = _trapMarkers[i];

                // Convert trap world coords to map coords
                int mapX = trap.WorldX / MapConstants.MAP_SCALE_H;
                int mapY = trap.WorldY / MapConstants.MAP_SCALE_V;

                // Convert to screen coords (including zoom)
                int markerScreenX = (int)((mapX - _viewOffsetX) * _zoom);
                int markerScreenY = (int)((mapY - _viewOffsetY) * _zoom);

                // Check if mouse is within hit radius
                int dx = screenX - markerScreenX;
                int dy = screenY - markerScreenY;
                int distanceSquared = dx * dx + dy * dy;

                if (distanceSquared <= hitRadius * hitRadius)
                {
                    return i;
                }
            }

            return -1;
        }

        /// <summary>
        /// Get trap marker by index
        /// </summary>
        public TrapMarker GetTrapMarker(int index)
        {
            if (index >= 0 && index < _trapMarkers.Count)
                return _trapMarkers[index];
            return null;
        }

        /// <summary>
        /// Update trap marker position
        /// </summary>
        public void UpdateTrapMarkerPosition(int index, int worldX, int worldY)
        {
            if (index >= 0 && index < _trapMarkers.Count)
            {
                _trapMarkers[index].WorldX = worldX;
                _trapMarkers[index].WorldY = worldY;
            }
        }

        /// <summary>
        /// Convert trap entries to trap markers with world coordinates
        /// </summary>
        public static List<TrapMarker> ConvertTrapEntriesToMarkers(List<MapData.TrapEntry> entries, MapConfig config)
        {
            List<TrapMarker> markers = new List<TrapMarker>();

            foreach (var entry in entries)
            {
                // Convert RegionId + Cell to World coordinates
                entry.GetWorldCoordinates(config, out int worldX, out int worldY);

                markers.Add(new TrapMarker
                {
                    WorldX = worldX,
                    WorldY = worldY,
                    ScriptFile = entry.ScriptFile
                });
            }

            return markers;
        }

        /// <summary>
        /// Set selection rectangle for block selection
        /// </summary>
        public void SetSelectionRectangle(Rectangle? rect)
        {
            _selectionRectangle = rect;
        }

        /// <summary>
        /// Find all trap markers within screen rectangle
        /// </summary>
        public List<int> FindTrapMarkersInRectangle(Rectangle screenRect)
        {
            List<int> indices = new List<int>();

            if (_trapMarkers == null || _trapMarkers.Count == 0)
                return indices;

            for (int i = 0; i < _trapMarkers.Count; i++)
            {
                var trap = _trapMarkers[i];

                // Convert trap world coords to screen coords
                int mapX = trap.WorldX / MapConstants.MAP_SCALE_H;
                int mapY = trap.WorldY / MapConstants.MAP_SCALE_V;
                int screenX = (int)((mapX - _viewOffsetX) * _zoom);
                int screenY = (int)((mapY - _viewOffsetY) * _zoom);

                // Check if trap marker is inside selection rectangle
                if (screenRect.Contains(screenX, screenY))
                {
                    indices.Add(i);
                }
            }

            return indices;
        }

        /// <summary>
        /// Set multiple selected trap indices
        /// </summary>
        public void SetSelectedTrapIndices(List<int> indices)
        {
            _selectedTrapIndices = new List<int>(indices);
            _selectedTrapIndex = -1; // Clear single selection
        }

        /// <summary>
        /// Get selected trap indices
        /// </summary>
        public List<int> GetSelectedTrapIndices()
        {
            return new List<int>(_selectedTrapIndices);
        }

        /// <summary>
        /// Clear all trap selections
        /// </summary>
        public void ClearTrapSelection()
        {
            _selectedTrapIndices.Clear();
            _selectedTrapIndex = -1;
        }

        /// <summary>
        /// Update multiple trap marker positions (for block drag)
        /// </summary>
        public void UpdateTrapMarkerPositions(List<int> indices, int deltaWorldX, int deltaWorldY)
        {
            foreach (int index in indices)
            {
                if (index >= 0 && index < _trapMarkers.Count)
                {
                    _trapMarkers[index].WorldX += deltaWorldX;
                    _trapMarkers[index].WorldY += deltaWorldY;
                }
            }
        }

        /// <summary>
        /// Set map background image from byte array
        /// </summary>
        public void SetMapImage(byte[] imageData, int offsetX = 0, int offsetY = 0)
        {
            if (_mapImage != null)
            {
                _mapImage.Dispose();
                _mapImage = null;
            }

            if (imageData != null && imageData.Length > 0)
            {
                try
                {
                    using (MemoryStream ms = new MemoryStream(imageData))
                    {
                        // Load image from stream and CLONE it (MemoryStream will be disposed)
                        Image tempImage = Image.FromStream(ms);
                        _mapImage = new Bitmap(tempImage);
                        tempImage.Dispose();

                        _mapImageOffsetX = offsetX;
                        _mapImageOffsetY = offsetY;

                        Console.WriteLine($"✓ Map image loaded: {_mapImage.Width}x{_mapImage.Height} pixels");
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"⚠ Failed to load map image: {ex.Message}");
                }
            }
        }

        /// <summary>
        /// Clear map background image
        /// </summary>
        public void ClearMapImage()
        {
            if (_mapImage != null)
            {
                _mapImage.Dispose();
                _mapImage = null;
            }
        }

        /// <summary>
        /// Get map image bounds in MAP coordinates (for scroll area calculation)
        /// </summary>
        public Rectangle? GetMapImageBounds()
        {
            if (_mapImage == null)
                return null;

            return new Rectangle(
                _mapImageOffsetX,
                _mapImageOffsetY,
                _mapImage.Width,
                _mapImage.Height
            );
        }

        /// <summary>
        /// Render map to graphics surface
        /// </summary>
        public void Render(Graphics g, int width, int height, MapCoordinate? selectedCoord = null)
        {
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.Clear(_backgroundColor);

            // Apply zoom transform
            g.ScaleTransform(_zoom, _zoom);

            // Calculate visible region range
            int startWorldX = _viewOffsetX;
            int startWorldY = _viewOffsetY;
            int endWorldX = startWorldX + (int)(width / _zoom);
            int endWorldY = startWorldY + (int)(height / _zoom);

            // Draw map background image if available
            if (_mapImage != null)
            {
                int imgX = _mapImageOffsetX - _viewOffsetX;
                int imgY = _mapImageOffsetY - _viewOffsetY;
                g.DrawImage(_mapImage, imgX, imgY, _mapImage.Width, _mapImage.Height);
            }

            // Draw loaded regions (overlay on top of map image)
            foreach (var region in _loadedRegions.Values)
            {
                if (!region.IsLoaded)
                    continue;

                // Calculate region bounds in MAP coordinates
                int regionMapX = region.RegionX * MapConstants.MAP_REGION_PIXEL_WIDTH;
                int regionMapY = region.RegionY * MapConstants.MAP_REGION_PIXEL_HEIGHT;

                // Skip if region is not in view
                if (regionMapX + MapConstants.MAP_REGION_PIXEL_WIDTH < startWorldX ||
                    regionMapX > endWorldX ||
                    regionMapY + MapConstants.MAP_REGION_PIXEL_HEIGHT < startWorldY ||
                    regionMapY > endWorldY)
                    continue;

                RenderRegion(g, region, selectedCoord);
            }

            // Draw NPC markers
            RenderNpcMarkers(g);

            // Draw Trap markers
            RenderTrapMarkers(g);

            // Draw selection rectangle (before ResetTransform)
            if (_selectionRectangle.HasValue)
            {
                // Selection rectangle is in screen coordinates, need to invert zoom
                Rectangle rect = _selectionRectangle.Value;
                float invZoom = 1.0f / _zoom;

                Rectangle zoomedRect = new Rectangle(
                    (int)(rect.X * invZoom),
                    (int)(rect.Y * invZoom),
                    (int)(rect.Width * invZoom),
                    (int)(rect.Height * invZoom)
                );

                using (Pen pen = new Pen(Color.FromArgb(150, 0, 255, 255), 2))
                {
                    pen.DashStyle = System.Drawing.Drawing2D.DashStyle.Dash;
                    g.DrawRectangle(pen, zoomedRect);
                }

                // Draw semi-transparent fill
                using (SolidBrush brush = new SolidBrush(Color.FromArgb(30, 0, 255, 255)))
                {
                    g.FillRectangle(brush, zoomedRect);
                }
            }

            // Draw coordinate info
            g.ResetTransform();
            DrawCoordinateInfo(g, width, height, selectedCoord);
        }

        /// <summary>
        /// Render NPC markers at their world positions
        /// </summary>
        private void RenderNpcMarkers(Graphics g)
        {
            if (_npcMarkers == null || _npcMarkers.Count == 0)
                return;

            for (int i = 0; i < _npcMarkers.Count; i++)
            {
                var npc = _npcMarkers[i];
                bool isHovered = (i == _hoveredNpcIndex);
                bool isSelected = (i == _selectedNpcIndex);

                // PosX/PosY from Npc_Load.txt are in WORLD coordinates (logic coordinates)
                // Need to convert to MAP coordinates (24.jpg pixel coordinates)
                // WorldX → MapX: divide by MAP_SCALE_H (16)
                // WorldY → MapY: divide by MAP_SCALE_V (32)
                int mapX = npc.PosX / MapConstants.MAP_SCALE_H;
                int mapY = npc.PosY / MapConstants.MAP_SCALE_V;

                // Convert MAP coordinates to screen coordinates
                int screenX = mapX - _viewOffsetX;
                int screenY = mapY - _viewOffsetY;

                // Draw NPC marker as a circle (8 pixel diameter normally, 10 if hovered/selected)
                int markerSize = (isHovered || isSelected) ? 10 : 8;
                Rectangle markerRect = new Rectangle(
                    screenX - markerSize / 2,
                    screenY - markerSize / 2,
                    markerSize,
                    markerSize
                );

                // Choose color based on state
                Color fillColor = _npcColor;  // Purple
                Color borderColor = Color.White;
                int borderWidth = 1;

                if (isSelected)
                {
                    fillColor = Color.HotPink;  // Hot pink for selected
                    borderColor = Color.Lime;    // Bright green border
                    borderWidth = 3;
                }
                else if (isHovered)
                {
                    fillColor = Color.Magenta;  // Magenta for hovered
                    borderWidth = 2;
                }

                // Draw filled circle with border
                using (SolidBrush brush = new SolidBrush(fillColor))
                using (Pen pen = new Pen(borderColor, borderWidth))
                {
                    g.FillEllipse(brush, markerRect);
                    g.DrawEllipse(pen, markerRect);
                }

                // Draw tooltip for hovered NPC
                if (isHovered)
                {
                    const int spriteSize = 64; // Fixed sprite display size
                    const int padding = 4;
                    const int columnSpacing = 8;

                    // Load NPC sprite if available
                    Bitmap npcSprite = null;
                    if (_npcLoader != null)
                    {
                        try
                        {
                            DebugLogger.Log($"[Tooltip] Loading sprite for NPC ID {npc.NpcID}...");
                            SpriteData spriteData = _npcLoader.LoadSpriteById(npc.NpcID, NpcAction.NormalStand);
                            if (spriteData != null && spriteData.FrameCount > 0)
                            {
                                // Get first frame (direction 0)
                                DebugLogger.Log($"[Tooltip] Sprite loaded: {spriteData.FrameCount} frames, converting frame 0 to bitmap");
                                npcSprite = SpriteLoader.FrameToBitmap(spriteData, 0);
                                DebugLogger.Log($"[Tooltip] ✓ Bitmap created: {npcSprite.Width}x{npcSprite.Height}");
                            }
                            else
                            {
                                DebugLogger.Log($"[Tooltip] ⚠ No sprite data or frames for NPC ID {npc.NpcID}");
                            }
                        }
                        catch (Exception ex)
                        {
                            DebugLogger.Log($"[Tooltip] ✗ Failed to load NPC sprite for ID {npc.NpcID}: {ex.Message}");
                            DebugLogger.Log($"[Tooltip]    Stack: {ex.StackTrace}");
                        }
                    }
                    else
                    {
                        DebugLogger.Log($"[Tooltip] ⚠ NpcLoader is null - cannot load sprite");
                    }

                    string tooltipText = $"NPC #{i + 1}\nID: {npc.NpcID}\nName: {npc.Name}\nLevel: {npc.Level}\nWorld: ({npc.PosX}, {npc.PosY})";

                    using (Font font = new Font(".VnArial", 8))
                    using (SolidBrush textBrush = new SolidBrush(Color.White))
                    using (SolidBrush bgBrush = new SolidBrush(Color.FromArgb(220, 0, 0, 0)))
                    {
                        // Measure text size for right column
                        string[] lines = tooltipText.Split('\n');
                        float maxTextWidth = 0;
                        float totalTextHeight = 0;
                        foreach (string line in lines)
                        {
                            SizeF lineSize = g.MeasureString(line, font);
                            maxTextWidth = Math.Max(maxTextWidth, lineSize.Width);
                            totalTextHeight += lineSize.Height;
                        }

                        // Calculate tooltip dimensions
                        // Left column: sprite (if available), Right column: text
                        int leftColumnWidth = (npcSprite != null) ? spriteSize : 0;
                        int rightColumnWidth = (int)maxTextWidth;
                        int tooltipContentWidth = leftColumnWidth + (leftColumnWidth > 0 ? columnSpacing : 0) + rightColumnWidth;
                        int tooltipContentHeight = (int)Math.Max(npcSprite != null ? spriteSize : 0, totalTextHeight);

                        // Position tooltip above and to the right of marker
                        int tooltipX = screenX + markerSize / 2 + 5;
                        int tooltipY = screenY - tooltipContentHeight - padding * 2 - 5;

                        Rectangle tooltipBg = new Rectangle(
                            tooltipX - padding,
                            tooltipY - padding,
                            tooltipContentWidth + padding * 2,
                            tooltipContentHeight + padding * 2
                        );

                        // Draw background and border
                        g.FillRectangle(bgBrush, tooltipBg);
                        g.DrawRectangle(new Pen(Color.Magenta, 2), tooltipBg);

                        // Draw sprite in left column (if available)
                        if (npcSprite != null)
                        {
                            Rectangle spriteRect = new Rectangle(
                                tooltipX,
                                tooltipY + (tooltipContentHeight - spriteSize) / 2,
                                spriteSize,
                                spriteSize
                            );
                            g.DrawImage(npcSprite, spriteRect);

                            // Draw separator line
                            int separatorX = tooltipX + spriteSize + columnSpacing / 2;
                            g.DrawLine(new Pen(Color.FromArgb(100, 255, 255, 255), 1),
                                separatorX, tooltipY,
                                separatorX, tooltipY + tooltipContentHeight);
                        }

                        // Draw text in right column
                        int textStartX = tooltipX + leftColumnWidth + (leftColumnWidth > 0 ? columnSpacing : 0);
                        float currentY = tooltipY + (tooltipContentHeight - totalTextHeight) / 2;
                        foreach (string line in lines)
                        {
                            g.DrawString(line, font, textBrush, textStartX, currentY);
                            currentY += g.MeasureString(line, font).Height;
                        }
                    }

                    // Dispose sprite bitmap
                    npcSprite?.Dispose();
                }
                else if (!isSelected)
                {
                    // Draw NPC ID text next to marker (smaller font) - only when not hovered/selected
                    using (Font font = new Font(".VnArial", 6))
                    using (SolidBrush textBrush = new SolidBrush(Color.White))
                    using (SolidBrush bgBrush = new SolidBrush(Color.FromArgb(180, 0, 0, 0)))
                    {
                        string npcText = $"NPC {npc.NpcID}";
                        SizeF textSize = g.MeasureString(npcText, font);
                        Rectangle textBg = new Rectangle(
                            screenX + markerSize / 2 + 2,
                            screenY - (int)textSize.Height / 2,
                            (int)textSize.Width + 4,
                            (int)textSize.Height
                        );
                        g.FillRectangle(bgBrush, textBg);
                        g.DrawString(npcText, font, textBrush, screenX + markerSize / 2 + 4, screenY - textSize.Height / 2);
                    }
                }
            }
        }

        /// <summary>
        /// Render Trap markers at their world positions
        /// </summary>
        private void RenderTrapMarkers(Graphics g)
        {
            if (_trapMarkers == null || _trapMarkers.Count == 0)
                return;

            for (int i = 0; i < _trapMarkers.Count; i++)
            {
                var trap = _trapMarkers[i];
                bool isHovered = (i == _hoveredTrapIndex);
                bool isSelected = (i == _selectedTrapIndex) || _selectedTrapIndices.Contains(i);

                // Trap WorldX/WorldY are in WORLD coordinates (logic coordinates)
                // Need to convert to MAP coordinates (24.jpg pixel coordinates)
                // WorldX → MapX: divide by MAP_SCALE_H (16)
                // WorldY → MapY: divide by MAP_SCALE_V (32)
                int mapX = trap.WorldX / MapConstants.MAP_SCALE_H;
                int mapY = trap.WorldY / MapConstants.MAP_SCALE_V;

                // Convert MAP coordinates to screen coordinates
                int screenX = mapX - _viewOffsetX;
                int screenY = mapY - _viewOffsetY;

                // Draw Trap marker as a square (6x6 pixels normally, 8x8 if hovered/selected)
                int markerSize = (isHovered || isSelected) ? 8 : 6;
                Rectangle markerRect = new Rectangle(
                    screenX - markerSize / 2,
                    screenY - markerSize / 2,
                    markerSize,
                    markerSize
                );

                // Choose color based on state
                Color fillColor = Color.Gold;
                Color borderColor = Color.White;
                int borderWidth = 2;

                if (isSelected)
                {
                    fillColor = Color.Orange;  // Orange for selected
                    borderColor = Color.Lime;   // Bright green border
                    borderWidth = 3;
                }
                else if (isHovered)
                {
                    fillColor = Color.Orange;  // Orange for hovered
                    borderWidth = 2;
                }

                // Draw filled square with border
                using (SolidBrush brush = new SolidBrush(fillColor))
                using (Pen pen = new Pen(borderColor, borderWidth))
                {
                    g.FillRectangle(brush, markerRect);
                    g.DrawRectangle(pen, markerRect);
                }

                // Draw tooltip for hovered trap (BEFORE ResetTransform)
                if (isHovered)
                {
                    // Convert region/cell back from world coordinates for display
                    int regionX = trap.WorldX / MapConstants.REGION_PIXEL_WIDTH;
                    int regionY = trap.WorldY / MapConstants.REGION_PIXEL_HEIGHT;
                    int cellX = (trap.WorldX % MapConstants.REGION_PIXEL_WIDTH) / MapConstants.LOGIC_CELL_WIDTH;
                    int cellY = (trap.WorldY % MapConstants.REGION_PIXEL_HEIGHT) / MapConstants.LOGIC_CELL_HEIGHT;

                    string tooltipText = $"Trap #{i + 1}\nWorld: ({trap.WorldX}, {trap.WorldY})\nRegion: ({regionX}, {regionY})\nCell: ({cellX}, {cellY})";

                    using (Font font = new Font(".VnArial", 8))
                    using (SolidBrush textBrush = new SolidBrush(Color.White))
                    using (SolidBrush bgBrush = new SolidBrush(Color.FromArgb(220, 0, 0, 0)))
                    {
                        // Measure text size
                        string[] lines = tooltipText.Split('\n');
                        float maxWidth = 0;
                        float totalHeight = 0;
                        foreach (string line in lines)
                        {
                            SizeF lineSize = g.MeasureString(line, font);
                            maxWidth = Math.Max(maxWidth, lineSize.Width);
                            totalHeight += lineSize.Height;
                        }

                        // Position tooltip above and to the right of marker
                        int tooltipX = screenX + markerSize / 2 + 5;
                        int tooltipY = screenY - (int)totalHeight - 5;

                        Rectangle tooltipBg = new Rectangle(
                            tooltipX - 2,
                            tooltipY - 2,
                            (int)maxWidth + 4,
                            (int)totalHeight + 4
                        );

                        g.FillRectangle(bgBrush, tooltipBg);
                        g.DrawRectangle(new Pen(Color.Gold, 1), tooltipBg);

                        // Draw text lines
                        float currentY = tooltipY;
                        foreach (string line in lines)
                        {
                            g.DrawString(line, font, textBrush, tooltipX, currentY);
                            currentY += g.MeasureString(line, font).Height;
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Render a single region
        /// </summary>
        private void RenderRegion(Graphics g, MapRegionData region, MapCoordinate? selectedCoord)
        {
            // Use MAP coordinates (same as 24.jpg) for rendering
            // 1 region = 32x32 pixels on screen (matching image scale)
            int regionMapX = region.RegionX * MapConstants.MAP_REGION_PIXEL_WIDTH;
            int regionMapY = region.RegionY * MapConstants.MAP_REGION_PIXEL_HEIGHT;

            // Calculate cell size on map (divide region size by cell count)
            int cellMapWidth = MapConstants.MAP_REGION_PIXEL_WIDTH / MapConstants.REGION_GRID_WIDTH;   // 32/16 = 2
            int cellMapHeight = MapConstants.MAP_REGION_PIXEL_HEIGHT / MapConstants.REGION_GRID_HEIGHT; // 32/32 = 1

            // OPTIMIZED: Draw walkable areas and traps, skip obstacles (let map show through)
            // Obstacles = transparent (no overlay)
            // Walkable = semi-transparent green overlay
            // Traps = semi-transparent yellow overlay
            for (int cy = 0; cy < MapConstants.REGION_GRID_HEIGHT; cy++)
            {
                for (int cx = 0; cx < MapConstants.REGION_GRID_WIDTH; cx++)
                {
                    bool hasObstacle = region.Obstacles[cx, cy] != 0;
                    bool hasTrap = region.Traps[cx, cy] != 0;
                    bool isSelected = selectedCoord.HasValue &&
                                    selectedCoord.Value.RegionID == region.RegionID &&
                                    selectedCoord.Value.CellX == cx &&
                                    selectedCoord.Value.CellY == cy;

                    // Skip obstacles - they are transparent (let map image show through)
                    if (hasObstacle && !isSelected)
                        continue;

                    // Skip drawing walkable cells when no map image (would fill entire screen with green)
                    if (!hasTrap && !isSelected && _mapImage == null && !hasObstacle)
                        continue;

                    int cellMapX = regionMapX + cx * cellMapWidth;
                    int cellMapY = regionMapY + cy * cellMapHeight;

                    int screenX = cellMapX - _viewOffsetX;
                    int screenY = cellMapY - _viewOffsetY;

                    Rectangle cellRect = new Rectangle(screenX, screenY, cellMapWidth, cellMapHeight);

                    // Determine cell color
                    Color cellColor;
                    if (hasTrap)
                        cellColor = _trapColor; // Semi-transparent YELLOW
                    else if (isSelected)
                        cellColor = _selectedCellColor; // Bright GREEN
                    else
                        cellColor = _walkableCellColor; // Semi-transparent GREEN

                    // Draw cell
                    using (SolidBrush brush = new SolidBrush(cellColor))
                    {
                        g.FillRectangle(brush, cellRect);
                    }

                    // Draw border for selected cell
                    if (isSelected)
                    {
                        using (Pen pen = new Pen(Color.Lime, 2))
                        {
                            g.DrawRectangle(pen, cellRect);
                        }
                    }
                }
            }

            // Draw region border (subtle outline)
            Rectangle regionRect = new Rectangle(
                regionMapX - _viewOffsetX,
                regionMapY - _viewOffsetY,
                MapConstants.MAP_REGION_PIXEL_WIDTH,
                MapConstants.MAP_REGION_PIXEL_HEIGHT);

            using (Pen pen = new Pen(_regionBorderColor, 1))
            {
                g.DrawRectangle(pen, regionRect);
            }
        }

        /// <summary>
        /// Draw coordinate information overlay
        /// </summary>
        private void DrawCoordinateInfo(Graphics g, int width, int height, MapCoordinate? coord)
        {
            if (!coord.HasValue)
                return;

            using (Font font = new Font("Consolas", 10))
            using (SolidBrush textBrush = new SolidBrush(Color.White))
            using (SolidBrush bgBrush = new SolidBrush(Color.FromArgb(200, 0, 0, 0)))
            {
                string[] lines = new string[]
                {
                    $"World: ({coord.Value.WorldX}, {coord.Value.WorldY})",
                    $"Region: ({coord.Value.RegionX}, {coord.Value.RegionY}) [ID: {coord.Value.RegionID}]",
                    $"Cell: ({coord.Value.CellX}, {coord.Value.CellY})",
                    $"Offset: ({coord.Value.OffsetX}, {coord.Value.OffsetY})"
                };

                int lineHeight = 20;
                int padding = 10;
                int boxHeight = lines.Length * lineHeight + padding * 2;
                int boxWidth = 400;

                Rectangle infoBox = new Rectangle(10, height - boxHeight - 10, boxWidth, boxHeight);
                g.FillRectangle(bgBrush, infoBox);

                for (int i = 0; i < lines.Length; i++)
                {
                    g.DrawString(lines[i], font, textBrush, 20, height - boxHeight - 10 + padding + i * lineHeight);
                }
            }
        }

        /// <summary>
        /// Convert screen coordinates to map coordinates
        /// </summary>
        public MapCoordinate ScreenToMapCoordinate(int screenX, int screenY)
        {
            // Step 1: Convert screen pixels to MAP coordinates (24.jpg pixel coordinates)
            int mapX = (int)(screenX / _zoom) + _viewOffsetX;
            int mapY = (int)(screenY / _zoom) + _viewOffsetY;

            // Step 2: Convert MAP coordinates to WORLD/LOGIC coordinates
            // WorldX = MapX * MAP_SCALE_H (multiply by 16)
            // WorldY = MapY * MAP_SCALE_V (multiply by 32)
            int worldX = mapX * MapConstants.MAP_SCALE_H;
            int worldY = mapY * MapConstants.MAP_SCALE_V;

            // Step 3: Convert WORLD coordinates to Region/Cell
            MapCoordinate result = CoordinateConverter.WorldToRegionCell(worldX, worldY);

            // Debug logging
            DebugLogger.Log($"[ScreenToMapCoordinate] Screen({screenX},{screenY}) → Map({mapX},{mapY}) → World({worldX},{worldY}) → Region({result.RegionX},{result.RegionY}) Cell({result.CellX},{result.CellY}) ID={result.RegionID}");
            DebugLogger.Log($"  Zoom={_zoom:F2}, ViewOffset=({_viewOffsetX},{_viewOffsetY})");

            return result;
        }

        /// <summary>
        /// Pan view by offset
        /// </summary>
        public void Pan(int deltaX, int deltaY)
        {
            _viewOffsetX += deltaX;
            _viewOffsetY += deltaY;

            // Clamp to reasonable bounds
            _viewOffsetX = Math.Max(0, _viewOffsetX);
            _viewOffsetY = Math.Max(0, _viewOffsetY);
        }
    }
}
