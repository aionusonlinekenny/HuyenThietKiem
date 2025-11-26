// MapCoord.cpp - Map Coordinate Converter Tool
// Compile: cl.exe MapCoord.cpp
// Usage: MapCoord <command> <args...>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants from game engine
#define REGION_GRID_WIDTH   16
#define REGION_GRID_HEIGHT  32
#define LOGIC_CELL_WIDTH    32
#define LOGIC_CELL_HEIGHT   32
#define REGION_PIXEL_WIDTH  512   // 16 * 32
#define REGION_PIXEL_HEIGHT 1024  // 32 * 32

// Make RegionID from X, Y (MAKELONG equivalent)
#define MAKE_REGION_ID(x, y) ((int)((unsigned short)(x) | ((unsigned int)((unsigned short)(y)) << 16)))

// Extract X, Y from RegionID (LOWORD/HIWORD)
#define REGION_ID_X(id) ((int)((unsigned short)(id)))
#define REGION_ID_Y(id) ((int)((unsigned short)(((unsigned int)(id)) >> 16)))

// Function prototypes
void WorldToRegionCell(int worldX, int worldY, int* regionX, int* regionY, int* cellX, int* cellY, int* regionID);
void RegionCellToWorld(int regionX, int regionY, int cellX, int cellY, int* worldX, int* worldY);
void PrintUsage();
void GenerateTrapEntry(int mapId, int worldX, int worldY, const char* scriptFile);
void ConvertWorldCoords(int worldX, int worldY);
void ConvertRegionCell(int regionX, int regionY, int cellX, int cellY);

// Main function
int main(int argc, char* argv[])
{
    printf("========================================\n");
    printf("Map Coordinate Converter Tool (C++)\n");
    printf("========================================\n\n");

    if (argc < 2)
    {
        PrintUsage();
        return 0;
    }

    const char* command = argv[1];

    // Command: w2r (World to Region/Cell)
    if (strcmp(command, "w2r") == 0 || strcmp(command, "-w") == 0)
    {
        if (argc < 4)
        {
            printf("Error: Missing arguments for w2r command\n");
            printf("Usage: MapCoord w2r <WorldX> <WorldY>\n");
            return 1;
        }

        int worldX = atoi(argv[2]);
        int worldY = atoi(argv[3]);

        ConvertWorldCoords(worldX, worldY);
        return 0;
    }

    // Command: r2w (Region/Cell to World)
    if (strcmp(command, "r2w") == 0 || strcmp(command, "-r") == 0)
    {
        if (argc < 6)
        {
            printf("Error: Missing arguments for r2w command\n");
            printf("Usage: MapCoord r2w <RegionX> <RegionY> <CellX> <CellY>\n");
            return 1;
        }

        int regionX = atoi(argv[2]);
        int regionY = atoi(argv[3]);
        int cellX = atoi(argv[4]);
        int cellY = atoi(argv[5]);

        ConvertRegionCell(regionX, regionY, cellX, cellY);
        return 0;
    }

    // Command: trap (Generate trap entry)
    if (strcmp(command, "trap") == 0 || strcmp(command, "-t") == 0)
    {
        if (argc < 5)
        {
            printf("Error: Missing arguments for trap command\n");
            printf("Usage: MapCoord trap <MapID> <WorldX> <WorldY> [ScriptFile]\n");
            return 1;
        }

        int mapId = atoi(argv[2]);
        int worldX = atoi(argv[3]);
        int worldY = atoi(argv[4]);
        const char* scriptFile = (argc >= 6) ? argv[5] : "\\script\\maps\\trap\\1\\1.lua";

        GenerateTrapEntry(mapId, worldX, worldY, scriptFile);
        return 0;
    }

    printf("Error: Unknown command '%s'\n\n", command);
    PrintUsage();
    return 1;
}

// Convert World coordinates to Region/Cell
void WorldToRegionCell(int worldX, int worldY, int* regionX, int* regionY, int* cellX, int* cellY, int* regionID)
{
    // Calculate Region coordinates
    *regionX = worldX / REGION_PIXEL_WIDTH;
    *regionY = worldY / REGION_PIXEL_HEIGHT;

    // Calculate Cell coordinates within region
    int localX = worldX % REGION_PIXEL_WIDTH;
    int localY = worldY % REGION_PIXEL_HEIGHT;

    *cellX = localX / LOGIC_CELL_WIDTH;
    *cellY = localY / LOGIC_CELL_HEIGHT;

    // Calculate RegionID
    *regionID = MAKE_REGION_ID(*regionX, *regionY);
}

// Convert Region/Cell to World coordinates
void RegionCellToWorld(int regionX, int regionY, int cellX, int cellY, int* worldX, int* worldY)
{
    *worldX = (regionX * REGION_GRID_WIDTH + cellX) * LOGIC_CELL_WIDTH;
    *worldY = (regionY * REGION_GRID_HEIGHT + cellY) * LOGIC_CELL_HEIGHT;
}

// Print usage information
void PrintUsage()
{
    printf("Usage:\n");
    printf("  MapCoord w2r <WorldX> <WorldY>\n");
    printf("           Convert World coordinates to Region/Cell\n\n");

    printf("  MapCoord r2w <RegionX> <RegionY> <CellX> <CellY>\n");
    printf("           Convert Region/Cell to World coordinates\n\n");

    printf("  MapCoord trap <MapID> <WorldX> <WorldY> [ScriptFile]\n");
    printf("           Generate trap file entry\n\n");

    printf("Examples:\n");
    printf("  MapCoord w2r 47328 640\n");
    printf("  MapCoord r2w 92 0 7 20\n");
    printf("  MapCoord trap 21 5000 10000 \\script\\maps\\trap\\21\\1.lua\n\n");

    printf("Constants:\n");
    printf("  Region Grid:   %d x %d cells\n", REGION_GRID_WIDTH, REGION_GRID_HEIGHT);
    printf("  Cell Size:     %d x %d pixels\n", LOGIC_CELL_WIDTH, LOGIC_CELL_HEIGHT);
    printf("  Region Size:   %d x %d pixels\n", REGION_PIXEL_WIDTH, REGION_PIXEL_HEIGHT);
}

// Convert and display World coordinates
void ConvertWorldCoords(int worldX, int worldY)
{
    int regionX, regionY, cellX, cellY, regionID;

    WorldToRegionCell(worldX, worldY, &regionX, &regionY, &cellX, &cellY, &regionID);

    printf("World Coordinates: (%d, %d)\n", worldX, worldY);
    printf("  |               \n");
    printf("  +---> Region:    (%d, %d)\n", regionX, regionY);
    printf("  +---> RegionID:  %d\n", regionID);
    printf("  +---> Cell:      (%d, %d)\n", cellX, cellY);
    printf("  +---> Offset:    (%d, %d)\n",
           worldX - (regionX * REGION_PIXEL_WIDTH + cellX * LOGIC_CELL_WIDTH),
           worldY - (regionY * REGION_PIXEL_HEIGHT + cellY * LOGIC_CELL_HEIGHT));
    printf("\n");

    printf("Trap Entry Format:\n");
    printf("  MapId\\tRegionId\\tCellX\\tCellY\\tScriptFile\\tIsLoad\n");
    printf("  <mapid>\\t%d\\t%d\\t%d\\t<script>\\t1\n", regionID, cellX, cellY);
}

// Convert and display Region/Cell coordinates
void ConvertRegionCell(int regionX, int regionY, int cellX, int cellY)
{
    int worldX, worldY;
    int regionID = MAKE_REGION_ID(regionX, regionY);

    RegionCellToWorld(regionX, regionY, cellX, cellY, &worldX, &worldY);

    printf("Region/Cell Coordinates:\n");
    printf("  Region:    (%d, %d)\n", regionX, regionY);
    printf("  RegionID:  %d\n", regionID);
    printf("  Cell:      (%d, %d)\n", cellX, cellY);
    printf("  |               \n");
    printf("  +---> World:     (%d, %d)\n", worldX, worldY);
    printf("\n");

    printf("Trap Entry Format:\n");
    printf("  <mapid>\\t%d\\t%d\\t%d\\t<script>\\t1\n", regionID, cellX, cellY);
}

// Generate trap file entry
void GenerateTrapEntry(int mapId, int worldX, int worldY, const char* scriptFile)
{
    int regionX, regionY, cellX, cellY, regionID;

    WorldToRegionCell(worldX, worldY, &regionX, &regionY, &cellX, &cellY, &regionID);

    printf("Generated Trap Entry:\n");
    printf("========================================\n");
    printf("MapId\\tRegionId\\tCellX\\tCellY\\tScriptFile\\tIsLoad\n");
    printf("%d\\t%d\\t%d\\t%d\\t%s\\t1\n", mapId, regionID, cellX, cellY, scriptFile);
    printf("========================================\n\n");

    printf("Details:\n");
    printf("  Map ID:        %d\n", mapId);
    printf("  World:         (%d, %d)\n", worldX, worldY);
    printf("  Region:        (%d, %d)\n", regionX, regionY);
    printf("  RegionID:      %d\n", regionID);
    printf("  Cell:          (%d, %d)\n", cellX, cellY);
    printf("  Script:        %s\n", scriptFile);
    printf("\n");

    printf("To add to file:\n");
    printf("  1. Open: Bin\\Server\\library\\maps\\Trap\\%d.txt\n", mapId);
    printf("  2. Add line above to the file\n");
    printf("  3. Save and restart server\n");
}
