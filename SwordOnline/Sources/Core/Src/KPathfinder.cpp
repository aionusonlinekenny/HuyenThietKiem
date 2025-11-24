//===============================================================
// KPathfinder.cpp - A* Pathfinding Implementation
// Author: Claude AI Assistant
// Date: 2025-11-24
//===============================================================

#include "KCore.h"
#include "KPathfinder.h"
#include "KSubWorld.h"
#include "KNpc.h"
#include <cmath>
#include <algorithm>

//===============================================================
// Constructor / Destructor
//===============================================================

KPathfinder::KPathfinder()
{
	m_nNpcIdx = 0;
	m_nSubWorldIndex = 0;
	m_nCacheIndex = 0;
	m_nCurrentWaypoint = 0;
	m_nNodesSearched = 0;
	m_nPathsFound = 0;
	m_nCacheHits = 0;
	m_bInitialized = false;  // Lazy initialization

	// Clear cache
	for (int i = 0; i < PATHFINDER_CACHE_SIZE; i++)
	{
		m_PathCache[i].valid = false;
	}
}

KPathfinder::~KPathfinder()
{
	// Free node pool
	for (size_t i = 0; i < m_NodePool.size(); i++)
	{
		delete m_NodePool[i];
	}
	m_NodePool.clear();
	m_bInitialized = false;
}

//===============================================================
// Initialization
//===============================================================

void KPathfinder::InitializeNodePool()
{
	if (m_bInitialized)
		return;  // Already initialized

	// Pre-allocate node pool (lazy initialization to save memory)
	m_NodePool.reserve(PATHFINDER_MAX_NODES);
	for (int i = 0; i < PATHFINDER_MAX_NODES; i++)
	{
		m_NodePool.push_back(new AStarPathNode());
	}

	m_bInitialized = true;
}

void KPathfinder::Init(int nNpcIdx)
{
	m_nNpcIdx = nNpcIdx;
	if (nNpcIdx > 0 && nNpcIdx < MAX_NPC)
	{
		m_nSubWorldIndex = Npc[nNpcIdx].m_SubWorldIndex;
	}
	Clear();
}

void KPathfinder::Clear()
{
	m_CurrentPath.clear();
	m_nCurrentWaypoint = 0;
	ResetNodePool();

	// Clear open set
	while (!m_OpenSet.empty())
		m_OpenSet.pop();
	m_OpenSetList.clear();
}

//===============================================================
// Main Pathfinding Function
//===============================================================

bool KPathfinder::FindPath(int startX, int startY, int goalX, int goalY)
{
	// Lazy initialization on first use
	if (!m_bInitialized)
	{
		InitializeNodePool();
	}

	// Convert to grid coordinates
	PathPoint gridStart = WorldToGrid(startX, startY);
	PathPoint gridGoal = WorldToGrid(goalX, goalY);

	// Check cache first
	CachedPath* cached = FindCachedPath(gridStart, gridGoal);
	if (cached && IsCacheValid(*cached))
	{
		m_CurrentPath = cached->waypoints;
		m_nCurrentWaypoint = 0;
		m_nCacheHits++;
		return true;
	}

	// Run A* algorithm
	Clear();
	bool found = AStar(gridStart, gridGoal);

	if (found)
	{
		// Smooth path
		SmoothPath();

		// Cache the path
		CachePath(gridStart, gridGoal, m_CurrentPath);

		m_nPathsFound++;
		m_nCurrentWaypoint = 0;
		return true;
	}

	return false;
}

//===============================================================
// Core A* Algorithm
//===============================================================

bool KPathfinder::AStar(const PathPoint& start, const PathPoint& goal)
{
	// Early exit if goal is unwalkable
	if (!IsWalkable(goal.x, goal.y))
		return false;

	// Initialize start node
	AStarPathNode* startNode = AllocateNode();
	if (!startNode)
		return false;

	startNode->point = start;
	startNode->gCost = 0;
	startNode->hCost = Heuristic(start, goal);
	startNode->fCost = startNode->hCost;
	startNode->parent = NULL;
	startNode->inOpenSet = true;

	m_OpenSet.push(startNode);
	m_OpenSetList.push_back(startNode);

	int nodesSearched = 0;

	// Main A* loop
	while (!m_OpenSet.empty() && nodesSearched < PATHFINDER_MAX_NODES)
	{
		// Get node with lowest fCost
		AStarPathNode* current = m_OpenSet.top();
		m_OpenSet.pop();

		// Skip if already processed
		if (current->inClosedSet)
			continue;

		// Mark as processed
		current->inOpenSet = false;
		current->inClosedSet = true;
		nodesSearched++;

		// Check if reached goal
		if (current->point == goal)
		{
			ReconstructPath(current);
			m_nNodesSearched += nodesSearched;
			return true;
		}

		// Get neighbors
		std::vector<PathPoint> neighbors;
		GetNeighbors(current->point, neighbors);

		// Process each neighbor
		for (size_t i = 0; i < neighbors.size(); i++)
		{
			const PathPoint& neighbor = neighbors[i];

			// Check if walkable
			if (!IsWalkable(neighbor.x, neighbor.y))
				continue;

			// Calculate costs
			float moveCost = Distance(current->point, neighbor);
			float newGCost = current->gCost + moveCost;

			// Try to find existing node for this neighbor
			AStarPathNode* neighborNode = NULL;
			for (size_t j = 0; j < m_OpenSetList.size(); j++)
			{
				if (m_OpenSetList[j]->point == neighbor)
				{
					neighborNode = m_OpenSetList[j];
					break;
				}
			}

			// Allocate new node if needed
			if (!neighborNode)
			{
				neighborNode = AllocateNode();
				if (!neighborNode)
					continue;

				neighborNode->point = neighbor;
				neighborNode->gCost = 999999;  // Infinity
				m_OpenSetList.push_back(neighborNode);
			}

			// Skip if already processed
			if (neighborNode->inClosedSet)
				continue;

			// Update if we found a better path
			if (newGCost < neighborNode->gCost)
			{
				neighborNode->parent = current;
				neighborNode->gCost = newGCost;
				neighborNode->hCost = Heuristic(neighbor, goal);
				neighborNode->fCost = neighborNode->gCost + neighborNode->hCost;

				// Add to open set if not already there
				if (!neighborNode->inOpenSet)
				{
					neighborNode->inOpenSet = true;
					m_OpenSet.push(neighborNode);
				}
			}
		}
	}

	m_nNodesSearched += nodesSearched;
	return false;  // No path found
}

//===============================================================
// Heuristic Functions
//===============================================================

float KPathfinder::Heuristic(const PathPoint& a, const PathPoint& b) const
{
	// Manhattan distance (faster, good for grid)
	int dx = abs(a.x - b.x);
	int dy = abs(a.y - b.y);
	return (float)(dx + dy);
}

float KPathfinder::Distance(const PathPoint& a, const PathPoint& b) const
{
	// Euclidean distance for actual movement cost
	int dx = a.x - b.x;
	int dy = a.y - b.y;

	// Diagonal movement costs more
	if (dx != 0 && dy != 0)
		return 1.414f;  // sqrt(2)
	else
		return 1.0f;
}

//===============================================================
// Neighbor Generation (8-directional)
//===============================================================

void KPathfinder::GetNeighbors(const PathPoint& point, std::vector<PathPoint>& neighbors) const
{
	neighbors.clear();
	neighbors.reserve(8);

	// 8 directions: N, NE, E, SE, S, SW, W, NW
	static const int dx[] = { 0,  1,  1,  1,  0, -1, -1, -1 };
	static const int dy[] = {-1, -1,  0,  1,  1,  1,  0, -1 };

	for (int i = 0; i < 8; i++)
	{
		PathPoint neighbor(point.x + dx[i], point.y + dy[i]);
		neighbors.push_back(neighbor);
	}
}

//===============================================================
// Walkability Check
//===============================================================

bool KPathfinder::IsWalkable(int x, int y) const
{
	// Convert grid to world coordinates
	PathPoint worldPos = GridToWorld(PathPoint(x, y));

#ifdef _SERVER
	int barrier = SubWorld[m_nSubWorldIndex].TestBarrier(worldPos.x, worldPos.y);
#else
	int barrier = SubWorld[0].TestBarrier(worldPos.x, worldPos.y);
#endif

	return (barrier == 0);
}

//===============================================================
// Path Reconstruction
//===============================================================

void KPathfinder::ReconstructPath(AStarPathNode* goalNode)
{
	m_CurrentPath.clear();

	AStarPathNode* current = goalNode;
	while (current != NULL)
	{
		// Convert grid back to world coordinates
		PathPoint worldPos = GridToWorld(current->point);
		m_CurrentPath.push_back(worldPos);
		current = current->parent;
	}

	// Reverse path (was goal->start, now start->goal)
	std::reverse(m_CurrentPath.begin(), m_CurrentPath.end());
}

//===============================================================
// Path Smoothing (String Pulling Algorithm)
//===============================================================

void KPathfinder::SmoothPath()
{
	if (m_CurrentPath.size() <= 2)
		return;  // Nothing to smooth

	std::vector<PathPoint> smoothed;
	smoothed.push_back(m_CurrentPath[0]);  // Start point

	int current = 0;
	while (current < (int)m_CurrentPath.size() - 1)
	{
		// Try to find furthest visible point
		int furthest = current + 1;
		for (int i = (int)m_CurrentPath.size() - 1; i > current + 1; i--)
		{
			PathPoint gridCurrent = WorldToGrid(m_CurrentPath[current].x, m_CurrentPath[current].y);
			PathPoint gridTarget = WorldToGrid(m_CurrentPath[i].x, m_CurrentPath[i].y);

			if (HasLineOfSight(gridCurrent, gridTarget))
			{
				furthest = i;
				break;
			}
		}

		smoothed.push_back(m_CurrentPath[furthest]);
		current = furthest;
	}

	m_CurrentPath = smoothed;
}

//===============================================================
// Line of Sight Check (Bresenham's Line Algorithm)
//===============================================================

bool KPathfinder::HasLineOfSight(const PathPoint& a, const PathPoint& b) const
{
	int x0 = a.x;
	int y0 = a.y;
	int x1 = b.x;
	int y1 = b.y;

	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while (true)
	{
		// Check if current point is walkable
		if (!IsWalkable(x0, y0))
			return false;

		if (x0 == x1 && y0 == y1)
			break;

		int e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}

	return true;
}

//===============================================================
// Get Next Waypoint
//===============================================================

bool KPathfinder::GetNextWaypoint(int currentX, int currentY, int* outX, int* outY)
{
	if (m_CurrentPath.empty())
		return false;

	if (m_nCurrentWaypoint >= (int)m_CurrentPath.size())
		return false;  // Reached end of path

	// Check if close enough to current waypoint
	const PathPoint& waypoint = m_CurrentPath[m_nCurrentWaypoint];
	int dx = currentX - waypoint.x;
	int dy = currentY - waypoint.y;
	int distSq = dx * dx + dy * dy;

	// If close enough (within ~32 pixels), move to next waypoint
	if (distSq < (PATHFINDER_GRID_SIZE * PATHFINDER_GRID_SIZE))
	{
		m_nCurrentWaypoint++;
		if (m_nCurrentWaypoint >= (int)m_CurrentPath.size())
			return false;  // Reached goal
	}

	// Return current target waypoint
	*outX = m_CurrentPath[m_nCurrentWaypoint].x;
	*outY = m_CurrentPath[m_nCurrentWaypoint].y;
	return true;
}

//===============================================================
// Path Validation
//===============================================================

bool KPathfinder::IsPathValid() const
{
	// Check if all waypoints are still walkable
	for (size_t i = 0; i < m_CurrentPath.size(); i++)
	{
		PathPoint grid = WorldToGrid(m_CurrentPath[i].x, m_CurrentPath[i].y);
		if (!IsWalkable(grid.x, grid.y))
			return false;
	}
	return true;
}

//===============================================================
// Path Caching
//===============================================================

CachedPath* KPathfinder::FindCachedPath(const PathPoint& start, const PathPoint& goal)
{
	for (int i = 0; i < PATHFINDER_CACHE_SIZE; i++)
	{
		if (m_PathCache[i].valid &&
		    m_PathCache[i].start == start &&
		    m_PathCache[i].goal == goal)
		{
			return &m_PathCache[i];
		}
	}
	return NULL;
}

void KPathfinder::CachePath(const PathPoint& start, const PathPoint& goal, const std::vector<PathPoint>& path)
{
	CachedPath& cache = m_PathCache[m_nCacheIndex];
	cache.start = start;
	cache.goal = goal;
	cache.waypoints = path;
	cache.timestamp = GetTickCount();
	cache.valid = true;

	m_nCacheIndex = (m_nCacheIndex + 1) % PATHFINDER_CACHE_SIZE;
}

bool KPathfinder::IsCacheValid(const CachedPath& cache) const
{
	// Check timeout
	unsigned int elapsed = GetTickCount() - cache.timestamp;
	if (elapsed > PATHFINDER_CACHE_TIMEOUT)
		return false;

	// Check if path still valid (no new obstacles)
	for (size_t i = 0; i < cache.waypoints.size(); i++)
	{
		PathPoint grid = WorldToGrid(cache.waypoints[i].x, cache.waypoints[i].y);
		if (!IsWalkable(grid.x, grid.y))
			return false;
	}

	return true;
}

//===============================================================
// Node Pool Management
//===============================================================

AStarPathNode* KPathfinder::AllocateNode()
{
	for (size_t i = 0; i < m_NodePool.size(); i++)
	{
		AStarPathNode* node = m_NodePool[i];
		if (!node->inOpenSet && !node->inClosedSet)
		{
			// Reset node
			node->gCost = 0;
			node->hCost = 0;
			node->fCost = 0;
			node->parent = NULL;
			node->inOpenSet = false;
			node->inClosedSet = false;
			return node;
		}
	}
	return NULL;  // Pool exhausted
}

void KPathfinder::ResetNodePool()
{
	if (!m_bInitialized)
		return;  // Not initialized yet, nothing to reset

	for (size_t i = 0; i < m_NodePool.size(); i++)
	{
		m_NodePool[i]->inOpenSet = false;
		m_NodePool[i]->inClosedSet = false;
		m_NodePool[i]->parent = NULL;
	}
}

//===============================================================
// Coordinate Conversion
//===============================================================

PathPoint KPathfinder::WorldToGrid(int worldX, int worldY) const
{
	// Convert world coordinates (pixels) to grid coordinates
	return PathPoint(worldX / PATHFINDER_GRID_SIZE, worldY / PATHFINDER_GRID_SIZE);
}

PathPoint KPathfinder::GridToWorld(const PathPoint& grid) const
{
	// Convert grid coordinates to world coordinates (center of cell)
	return PathPoint(
		grid.x * PATHFINDER_GRID_SIZE + PATHFINDER_GRID_SIZE / 2,
		grid.y * PATHFINDER_GRID_SIZE + PATHFINDER_GRID_SIZE / 2
	);
}