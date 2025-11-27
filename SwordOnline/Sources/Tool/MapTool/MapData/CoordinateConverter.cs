using System;

namespace MapTool.MapData
{
    /// <summary>
    /// Coordinate conversion between World, Region, Cell systems
    /// Based on KSubWorld::Mps2Map and Map2Mps
    /// </summary>
    public static class CoordinateConverter
    {
        /// <summary>
        /// Convert World coordinates to Region/Cell coordinates
        /// </summary>
        public static MapCoordinate WorldToRegionCell(int worldX, int worldY)
        {
            MapCoordinate coord = new MapCoordinate();
            coord.WorldX = worldX;
            coord.WorldY = worldY;

            // Calculate Region coordinates
            coord.RegionX = worldX / MapConstants.REGION_PIXEL_WIDTH;
            coord.RegionY = worldY / MapConstants.REGION_PIXEL_HEIGHT;
            coord.RegionID = RegionData.MakeRegionID(coord.RegionX, coord.RegionY);

            // Calculate Cell coordinates within region
            int localX = worldX % MapConstants.REGION_PIXEL_WIDTH;
            int localY = worldY % MapConstants.REGION_PIXEL_HEIGHT;

            coord.CellX = localX / MapConstants.LOGIC_CELL_WIDTH;
            coord.CellY = localY / MapConstants.LOGIC_CELL_HEIGHT;

            // Calculate offset within cell
            coord.OffsetX = localX % MapConstants.LOGIC_CELL_WIDTH;
            coord.OffsetY = localY % MapConstants.LOGIC_CELL_HEIGHT;

            return coord;
        }

        /// <summary>
        /// Convert Region/Cell coordinates to World coordinates
        /// </summary>
        public static void RegionCellToWorld(int regionX, int regionY, int cellX, int cellY,
                                             out int worldX, out int worldY)
        {
            worldX = (regionX * MapConstants.REGION_GRID_WIDTH + cellX) * MapConstants.LOGIC_CELL_WIDTH;
            worldY = (regionY * MapConstants.REGION_GRID_HEIGHT + cellY) * MapConstants.LOGIC_CELL_HEIGHT;
        }

        /// <summary>
        /// Convert Region/Cell with offset to World coordinates
        /// </summary>
        public static void RegionCellToWorld(int regionX, int regionY, int cellX, int cellY,
                                             int offsetX, int offsetY, out int worldX, out int worldY)
        {
            RegionCellToWorld(regionX, regionY, cellX, cellY, out worldX, out worldY);
            worldX += offsetX;
            worldY += offsetY;
        }

        /// <summary>
        /// Convert screen coordinates to world coordinates based on view settings
        /// </summary>
        public static void ScreenToWorld(int screenX, int screenY, int viewWorldX, int viewWorldY,
                                        out int worldX, out int worldY)
        {
            worldX = viewWorldX + screenX;
            worldY = viewWorldY + screenY;
        }

        /// <summary>
        /// Convert world coordinates to screen coordinates
        /// </summary>
        public static void WorldToScreen(int worldX, int worldY, int viewWorldX, int viewWorldY,
                                        out int screenX, out int screenY)
        {
            screenX = worldX - viewWorldX;
            screenY = worldY - viewWorldY;
        }

        /// <summary>
        /// Get region file path
        /// </summary>
        public static string GetRegionFilePath(string mapPath, int regionX, int regionY, bool isServer = false)
        {
            string fileName = isServer ? MapConstants.REGION_SERVER_FILE : MapConstants.REGION_CLIENT_FILE;
            return $@"{mapPath}\v_{regionY:D3}\{regionX:D3}_{fileName}";
        }

        /// <summary>
        /// Validate coordinates are within region bounds
        /// </summary>
        public static bool IsValidCell(int cellX, int cellY)
        {
            return cellX >= 0 && cellX < MapConstants.REGION_GRID_WIDTH &&
                   cellY >= 0 && cellY < MapConstants.REGION_GRID_HEIGHT;
        }

        /// <summary>
        /// Clamp cell coordinates to valid range
        /// </summary>
        public static void ClampCell(ref int cellX, ref int cellY)
        {
            cellX = Math.Max(0, Math.Min(MapConstants.REGION_GRID_WIDTH - 1, cellX));
            cellY = Math.Max(0, Math.Min(MapConstants.REGION_GRID_HEIGHT - 1, cellY));
        }

        /// <summary>
        /// Make Region ID from coordinates (wrapper for RegionData.MakeRegionID)
        /// </summary>
        public static int MakeRegionID(int regionX, int regionY)
        {
            return RegionData.MakeRegionID(regionX, regionY);
        }
    }
}
