//===============================================================
// KPathfinder.h - A* Pathfinding System for Sword Online
// Author: Claude AI Assistant
// Date: 2025-11-24
// Description: Intelligent pathfinding using A* algorithm
//              Replaces simple wall-following with true pathfinding
//===============================================================

#ifndef KPATHFINDER_H
#define KPATHFINDER_H

#include <vector>
#include <queue>

//===============================================================
// Forward declarations
//===============================================================
class KSubWorld;

//===============================================================
// Constants
//===============================================================
#define PATHFINDER_MAX_NODES       500   // Max nodes to search (reduced to save memory)
#define PATHFINDER_GRID_SIZE       32    // Grid cell size (pixels)
#define PATHFINDER_MAX_PATH_LENGTH 100   // Max waypoints in path
#define PATHFINDER_CACHE_SIZE      15    // Number of cached paths (reduced)
#define PATHFINDER_CACHE_TIMEOUT   5000  // Cache timeout (ms)

//===============================================================
// Structures
//===============================================================

// 2D point structure
struct PathPoint
{
	int x;
	int y;

	PathPoint() : x(0), y(0) {}
	PathPoint(int _x, int _y) : x(_x), y(_y) {}

	bool operator==(const PathPoint& other) const
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(const PathPoint& other) const
	{
		return !(*this == other);
	}
};

// A* Node for pathfinding (renamed to avoid conflict with KJXPathFinder::PathNode)
struct AStarPathNode
{
	PathPoint point;      // Grid position
	float gCost;          // Distance from start
	float hCost;          // Heuristic to goal
	float fCost;          // Total cost (g + h)
	AStarPathNode* parent;     // Parent node for path reconstruction
	bool inOpenSet;       // Is in open set?
	bool inClosedSet;     // Is in closed set?

	AStarPathNode() : gCost(0), hCost(0), fCost(0), parent(NULL),
	             inOpenSet(false), inClosedSet(false) {}

	AStarPathNode(const PathPoint& p) : point(p), gCost(0), hCost(0), fCost(0),
	                                parent(NULL), inOpenSet(false), inClosedSet(false) {}
};

// Comparator for priority queue (min-heap by fCost)
struct AStarPathNodeComparator
{
	bool operator()(const AStarPathNode* a, const AStarPathNode* b) const
	{
		return a->fCost > b->fCost;  // Min-heap
	}
};

// Cached path entry
struct CachedPath
{
	PathPoint start;
	PathPoint goal;
	std::vector<PathPoint> waypoints;
	unsigned int timestamp;  // GetTickCount() when cached
	bool valid;

	CachedPath() : timestamp(0), valid(false) {}
};

//===============================================================
// KPathfinder Class - A* Pathfinding System
//===============================================================
class KPathfinder
{
private:
	// Map reference
	int m_nNpcIdx;           // NPC using this pathfinder
	int m_nSubWorldIndex;    // SubWorld index

	// A* data structures
	std::vector<AStarPathNode*> m_NodePool;     // Pre-allocated node pool
	std::priority_queue<AStarPathNode*, std::vector<AStarPathNode*>, AStarPathNodeComparator> m_OpenSet;
	std::vector<AStarPathNode*> m_OpenSetList;  // For cleanup

	// Path cache
	CachedPath m_PathCache[PATHFINDER_CACHE_SIZE];
	int m_nCacheIndex;  // Circular buffer index

	// Current path
	std::vector<PathPoint> m_CurrentPath;
	int m_nCurrentWaypoint;  // Index of current waypoint

	// Statistics
	int m_nNodesSearched;
	int m_nPathsFound;
	int m_nCacheHits;

	// Initialization flag
	bool m_bInitialized;  // Node pool allocated?

public:
	KPathfinder();
	~KPathfinder();

	// Initialization
	void Init(int nNpcIdx);
	void Clear();

	// Main pathfinding function
	// Returns: true if path found, false otherwise
	bool FindPath(int startX, int startY, int goalX, int goalY);

	// Get current path
	const std::vector<PathPoint>& GetPath() const { return m_CurrentPath; }
	int GetPathLength() const { return (int)m_CurrentPath.size(); }

	// Get next waypoint to move towards
	// Returns: true if waypoint available, false if reached goal
	bool GetNextWaypoint(int currentX, int currentY, int* outX, int* outY);

	// Check if path is still valid (no new obstacles)
	bool IsPathValid() const;

	// Statistics
	void GetStatistics(int* pNodesSearched, int* pPathsFound, int* pCacheHits) const
	{
		if (pNodesSearched) *pNodesSearched = m_nNodesSearched;
		if (pPathsFound) *pPathsFound = m_nPathsFound;
		if (pCacheHits) *pCacheHits = m_nCacheHits;
	}

private:
	// Core A* algorithm
	bool AStar(const PathPoint& start, const PathPoint& goal);

	// Heuristic function (Manhattan distance)
	float Heuristic(const PathPoint& a, const PathPoint& b) const;

	// Get distance between two points
	float Distance(const PathPoint& a, const PathPoint& b) const;

	// Get neighbors of a point (8-directional)
	void GetNeighbors(const PathPoint& point, std::vector<PathPoint>& neighbors) const;

	// Check if a point is walkable (no obstacle)
	bool IsWalkable(int x, int y) const;

	// Reconstruct path from goal node
	void ReconstructPath(AStarPathNode* goalNode);

	// Path smoothing using string pulling
	void SmoothPath();

	// Check line of sight between two points
	bool HasLineOfSight(const PathPoint& a, const PathPoint& b) const;

	// Path cache functions
	CachedPath* FindCachedPath(const PathPoint& start, const PathPoint& goal);
	void CachePath(const PathPoint& start, const PathPoint& goal, const std::vector<PathPoint>& path);
	bool IsCacheValid(const CachedPath& cache) const;

	// Node pool management
	void InitializeNodePool();  // Lazy initialization
	AStarPathNode* AllocateNode();
	void ResetNodePool();

	// Coordinate conversion
	PathPoint WorldToGrid(int worldX, int worldY) const;
	PathPoint GridToWorld(const PathPoint& grid) const;
};

#endif // KPATHFINDER_H