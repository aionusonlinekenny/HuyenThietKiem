using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.IO;
using MapTool.MapData;
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

        // Colors
        private Color _gridColor = Color.FromArgb(100, 128, 128, 128);
        private Color _regionBorderColor = Color.FromArgb(200, 0, 0, 255);
        private Color _obstacleColor = Color.FromArgb(180, 255, 0, 0);      // Red for obstacles
        private Color _trapColor = Color.FromArgb(180, 255, 255, 0);         // Yellow for traps
        private Color _walkableCellColor = Color.FromArgb(255, 60, 60, 60);  // Dark gray for walkable cells
        private Color _selectedCellColor = Color.FromArgb(150, 0, 255, 0);
        private Color _backgroundColor = Color.FromArgb(255, 20, 20, 20);     // Very dark background

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

            // Draw coordinate info
            g.ResetTransform();
            DrawCoordinateInfo(g, width, height, selectedCoord);
        }

        /// <summary>
        /// Render a single region
        /// </summary>
        private void RenderRegion(Graphics g, MapRegionData region, MapCoordinate? selectedCoord)
        {
            // Use MAP coordinates (same as 24.jpg) for rendering
            // 1 region = 128x128 pixels on screen (matching image scale)
            int regionMapX = region.RegionX * MapConstants.MAP_REGION_PIXEL_WIDTH;
            int regionMapY = region.RegionY * MapConstants.MAP_REGION_PIXEL_HEIGHT;

            // Calculate cell size on map (divide region size by cell count)
            int cellMapWidth = MapConstants.MAP_REGION_PIXEL_WIDTH / MapConstants.REGION_GRID_WIDTH;   // 128/16 = 8
            int cellMapHeight = MapConstants.MAP_REGION_PIXEL_HEIGHT / MapConstants.REGION_GRID_HEIGHT; // 128/32 = 4

            // Draw cells
            for (int cy = 0; cy < MapConstants.REGION_GRID_HEIGHT; cy++)
            {
                for (int cx = 0; cx < MapConstants.REGION_GRID_WIDTH; cx++)
                {
                    int cellMapX = regionMapX + cx * cellMapWidth;
                    int cellMapY = regionMapY + cy * cellMapHeight;

                    int screenX = cellMapX - _viewOffsetX;
                    int screenY = cellMapY - _viewOffsetY;

                    // Use MAP scale for rendering
                    Rectangle cellRect = new Rectangle(screenX, screenY,
                        cellMapWidth, cellMapHeight);

                    // Determine cell color and whether to draw
                    bool shouldDraw = true;
                    Color cellColor = _walkableCellColor; // Default: walkable (dark gray)

                    if (region.Obstacles[cx, cy] != 0)
                    {
                        cellColor = _obstacleColor; // Red for obstacles
                    }
                    else if (region.Traps[cx, cy] != 0)
                    {
                        cellColor = _trapColor; // Yellow for traps
                    }
                    else if (_mapImage != null)
                    {
                        // If we have map image, don't draw empty walkable cells
                        // (they would cover the image!)
                        shouldDraw = false;
                    }

                    // Fill cell with color (only if needed)
                    if (shouldDraw)
                    {
                        using (SolidBrush brush = new SolidBrush(cellColor))
                        {
                            g.FillRectangle(brush, cellRect);
                        }
                    }

                    // Highlight selected cell (draw on top)
                    if (selectedCoord.HasValue &&
                        selectedCoord.Value.RegionID == region.RegionID &&
                        selectedCoord.Value.CellX == cx &&
                        selectedCoord.Value.CellY == cy)
                    {
                        using (SolidBrush brush = new SolidBrush(_selectedCellColor))
                        {
                            g.FillRectangle(brush, cellRect);
                        }
                        using (Pen pen = new Pen(Color.Lime, 2))
                        {
                            g.DrawRectangle(pen, cellRect);
                        }
                    }

                    // Draw grid (draw last so it's visible)
                    using (Pen pen = new Pen(_gridColor))
                    {
                        g.DrawRectangle(pen, cellRect);
                    }
                }
            }

            // Draw region border
            Rectangle regionRect = new Rectangle(
                regionMapX - _viewOffsetX,
                regionMapY - _viewOffsetY,
                MapConstants.MAP_REGION_PIXEL_WIDTH,
                MapConstants.MAP_REGION_PIXEL_HEIGHT);

            using (Pen pen = new Pen(_regionBorderColor, 2))
            {
                g.DrawRectangle(pen, regionRect);
            }

            // Draw region label
            using (Font font = new Font("Arial", 10, FontStyle.Bold))
            using (SolidBrush brush = new SolidBrush(Color.White))
            {
                string label = $"R({region.RegionX},{region.RegionY})";
                g.DrawString(label, font, brush, regionRect.X + 5, regionRect.Y + 5);
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
            // Adjust for zoom
            int worldX = (int)(screenX / _zoom) + _viewOffsetX;
            int worldY = (int)(screenY / _zoom) + _viewOffsetY;

            return CoordinateConverter.WorldToRegionCell(worldX, worldY);
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
