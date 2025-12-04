using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.IO;
using MapTool.MapData;
using MapTool.NPC;
using MapTool.Export;
using MapRegionData = MapTool.MapData.RegionData;

namespace MapTool.Rendering
{
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

        // Trap markers (from imported trap files)
        private List<TrapEntry> _trapMarkers = new List<TrapEntry>();

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
        /// Clear NPC markers
        /// </summary>
        public void ClearNpcMarkers()
        {
            _npcMarkers.Clear();
        }

        /// <summary>
        /// Set trap markers to display on map
        /// </summary>
        public void SetTrapMarkers(List<TrapEntry> traps)
        {
            _trapMarkers = traps ?? new List<TrapEntry>();

            // Debug logging
            DebugLogger.Log($"[MapRenderer] SetTrapMarkers: {_trapMarkers.Count} traps");
            foreach (var trap in _trapMarkers)
            {
                // Convert local RegionID back to global RegionX, RegionY
                // Then calculate world coordinates for display
                DebugLogger.Log($"   Trap Map={trap.MapId} RegionID={trap.RegionId} Cell({trap.CellX},{trap.CellY})");
            }
        }

        /// <summary>
        /// Clear trap markers
        /// </summary>
        public void ClearTrapMarkers()
        {
            _trapMarkers.Clear();
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

            // Draw trap markers
            RenderTrapMarkers(g);

            // Draw NPC markers
            RenderNpcMarkers(g);

            // Draw coordinate info
            g.ResetTransform();
            DrawCoordinateInfo(g, width, height, selectedCoord);
        }

        /// <summary>
        /// Render trap markers at their cell positions
        /// </summary>
        private void RenderTrapMarkers(Graphics g)
        {
            if (_trapMarkers == null || _trapMarkers.Count == 0)
                return;

            foreach (var trap in _trapMarkers)
            {
                // Convert RegionID + Cell to World coordinates
                // Note: RegionID in trap file is LOCAL RegionID (relative to map rect)
                // We need to convert it to actual RegionX, RegionY first

                // For now, we'll assume RegionID is in format Y*256+X (global format)
                // TODO: Need map config to convert local RegionID to global RegionX/Y
                RegionData.ParseRegionID(trap.RegionId, out int regionX, out int regionY);

                // Convert Region + Cell to World coordinates
                CoordinateConverter.RegionCellToWorld(regionX, regionY, trap.CellX, trap.CellY,
                    out int worldX, out int worldY);

                // Convert World to Map coordinates (for 24.jpg pixel coordinates)
                int mapX = worldX / MapConstants.MAP_SCALE_H;
                int mapY = worldY / MapConstants.MAP_SCALE_V;

                // Convert MAP coordinates to screen coordinates
                int screenX = mapX - _viewOffsetX;
                int screenY = mapY - _viewOffsetY;

                // Draw trap marker as a small square (6 pixel)
                int markerSize = 6;
                Rectangle markerRect = new Rectangle(
                    screenX - markerSize / 2,
                    screenY - markerSize / 2,
                    markerSize,
                    markerSize
                );

                // Draw filled square with border (yellow for traps)
                Color trapMarkerColor = Color.FromArgb(220, 255, 200, 0); // Yellow-orange
                using (SolidBrush brush = new SolidBrush(trapMarkerColor))
                using (Pen pen = new Pen(Color.White, 1))
                {
                    g.FillRectangle(brush, markerRect);
                    g.DrawRectangle(pen, markerRect);
                }

                // Draw trap cell position text next to marker (very small font)
                using (Font font = new Font("Arial", 5))
                using (SolidBrush textBrush = new SolidBrush(Color.White))
                using (SolidBrush bgBrush = new SolidBrush(Color.FromArgb(180, 0, 0, 0)))
                {
                    string trapText = $"T({trap.CellX},{trap.CellY})";
                    SizeF textSize = g.MeasureString(trapText, font);
                    Rectangle textBg = new Rectangle(
                        screenX + markerSize / 2 + 2,
                        screenY - (int)textSize.Height / 2,
                        (int)textSize.Width + 4,
                        (int)textSize.Height
                    );
                    g.FillRectangle(bgBrush, textBg);
                    g.DrawString(trapText, font, textBrush, screenX + markerSize / 2 + 4, screenY - textSize.Height / 2);
                }
            }
        }

        /// <summary>
        /// Render NPC markers at their world positions
        /// </summary>
        private void RenderNpcMarkers(Graphics g)
        {
            if (_npcMarkers == null || _npcMarkers.Count == 0)
                return;

            foreach (var npc in _npcMarkers)
            {
                // PosX/PosY from Npc_Load.txt are in WORLD coordinates (logic coordinates)
                // Need to convert to MAP coordinates (24.jpg pixel coordinates)
                // WorldX → MapX: divide by MAP_SCALE_H (16)
                // WorldY → MapY: divide by MAP_SCALE_V (32)
                int mapX = npc.PosX / MapConstants.MAP_SCALE_H;
                int mapY = npc.PosY / MapConstants.MAP_SCALE_V;

                // Convert MAP coordinates to screen coordinates
                int screenX = mapX - _viewOffsetX;
                int screenY = mapY - _viewOffsetY;

                // Draw NPC marker as a circle (8 pixel diameter)
                int markerSize = 8;
                Rectangle markerRect = new Rectangle(
                    screenX - markerSize / 2,
                    screenY - markerSize / 2,
                    markerSize,
                    markerSize
                );

                // Draw filled circle with border
                using (SolidBrush brush = new SolidBrush(_npcColor))
                using (Pen pen = new Pen(Color.White, 1))
                {
                    g.FillEllipse(brush, markerRect);
                    g.DrawEllipse(pen, markerRect);
                }

                // Draw NPC ID text next to marker (smaller font)
                using (Font font = new Font("Arial", 6))
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
