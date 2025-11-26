using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using MapTool.MapData;

namespace MapTool.Rendering
{
    /// <summary>
    /// Renders map regions and cells to a Graphics surface
    /// </summary>
    public class MapRenderer
    {
        private Dictionary<int, RegionData> _loadedRegions;
        private int _cellSize = 16; // Render size per cell (pixels on screen)
        private int _viewOffsetX = 0;
        private int _viewOffsetY = 0;
        private float _zoom = 1.0f;

        // Colors
        private Color _gridColor = Color.FromArgb(100, 128, 128, 128);
        private Color _regionBorderColor = Color.FromArgb(200, 0, 0, 255);
        private Color _obstacleColor = Color.FromArgb(120, 255, 0, 0);
        private Color _trapColor = Color.FromArgb(120, 255, 255, 0);
        private Color _selectedCellColor = Color.FromArgb(150, 0, 255, 0);
        private Color _backgroundcolor = Color.FromArgb(255, 32, 32, 32);

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
            _loadedRegions = new Dictionary<int, RegionData>();
        }

        public void AddRegion(RegionData region)
        {
            _loadedRegions[region.RegionID] = region;
        }

        public void ClearRegions()
        {
            _loadedRegions.Clear();
        }

        /// <summary>
        /// Render map to graphics surface
        /// </summary>
        public void Render(Graphics g, int width, int height, MapCoordinate? selectedCoord = null)
        {
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.Clear(_backgroundcolor);

            // Apply zoom transform
            g.ScaleTransform(_zoom, _zoom);

            // Calculate visible region range
            int startWorldX = _viewOffsetX;
            int startWorldY = _viewOffsetY;
            int endWorldX = startWorldX + (int)(width / _zoom);
            int endWorldY = startWorldY + (int)(height / _zoom);

            // Draw loaded regions
            foreach (var region in _loadedRegions.Values)
            {
                if (!region.IsLoaded)
                    continue;

                // Calculate region bounds in world coordinates
                int regionWorldX = region.RegionX * MapConstants.REGION_PIXEL_WIDTH;
                int regionWorldY = region.RegionY * MapConstants.REGION_PIXEL_HEIGHT;

                // Skip if region is not in view
                if (regionWorldX + MapConstants.REGION_PIXEL_WIDTH < startWorldX ||
                    regionWorldX > endWorldX ||
                    regionWorldY + MapConstants.REGION_PIXEL_HEIGHT < startWorldY ||
                    regionWorldY > endWorldY)
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
        private void RenderRegion(Graphics g, RegionData region, MapCoordinate? selectedCoord)
        {
            int regionWorldX = region.RegionX * MapConstants.REGION_PIXEL_WIDTH;
            int regionWorldY = region.RegionY * MapConstants.REGION_PIXEL_HEIGHT;

            // Draw cells
            for (int cy = 0; cy < MapConstants.REGION_GRID_HEIGHT; cy++)
            {
                for (int cx = 0; cx < MapConstants.REGION_GRID_WIDTH; cx++)
                {
                    int cellWorldX = regionWorldX + cx * MapConstants.LOGIC_CELL_WIDTH;
                    int cellWorldY = regionWorldY + cy * MapConstants.LOGIC_CELL_HEIGHT;

                    int screenX = cellWorldX - _viewOffsetX;
                    int screenY = cellWorldY - _viewOffsetY;

                    Rectangle cellRect = new Rectangle(screenX, screenY, _cellSize, _cellSize);

                    // Draw obstacle
                    if (region.Obstacles[cx, cy] != 0)
                    {
                        using (SolidBrush brush = new SolidBrush(_obstacleColor))
                        {
                            g.FillRectangle(brush, cellRect);
                        }
                    }

                    // Draw trap
                    if (region.Traps[cx, cy] != 0)
                    {
                        using (SolidBrush brush = new SolidBrush(_trapColor))
                        {
                            g.FillRectangle(brush, cellRect);
                        }
                    }

                    // Highlight selected cell
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

                    // Draw grid
                    using (Pen pen = new Pen(_gridColor))
                    {
                        g.DrawRectangle(pen, cellRect);
                    }
                }
            }

            // Draw region border
            Rectangle regionRect = new Rectangle(
                regionWorldX - _viewOffsetX,
                regionWorldY - _viewOffsetY,
                MapConstants.REGION_GRID_WIDTH * _cellSize,
                MapConstants.REGION_GRID_HEIGHT * _cellSize);

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
