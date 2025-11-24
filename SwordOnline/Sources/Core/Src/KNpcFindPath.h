
#ifndef KNPCFINDPATH_H
#define KNPCFINDPATH_H

// Forward declaration
class KPathfinder;

class KNpcFindPath
{
public:
	int				m_NpcIdx;
	int				m_nDestX;
	int				m_nDestY;
	int				m_nFindTimer;
	int				m_nMaxTimeLong;
	int				m_nFindState;
	int				m_nPathSide;
	int				m_nFindTimes;

	// NEW: A* Pathfinding integration
	KPathfinder*	m_pPathfinder;        // A* pathfinder instance
	BOOL			m_bUseAStar;          // Enable/disable A* (default: TRUE)
	int				m_nLastPathTime;      // Last time path was calculated
	int				m_nLastPathX;         // Last path start X
	int				m_nLastPathY;         // Last path start Y

public:
	KNpcFindPath();
	~KNpcFindPath();  // NEW: Need destructor to clean up pathfinder

	void			Init(int nNpc);
	void			SetUseAStar(BOOL bUse) { m_bUseAStar = bUse; }  // NEW: Toggle A*

	int				GetDir(int nXpos,int nYpos, int nDir, int nDestX, int nDestY, int nMoveSpeed, int *pnGetDir, int* pnStopOK);		//
	int				Dir64To8(int nDir);
	int				Dir8To64(int nDir);
	BOOL			CheckDistance(int x1, int y1, int x2, int y2, int nDistance);
	int				CheckBarrier(int nChangeX, int nChangeY);
	INT				RealCheckBarrier(int nChangeX, int nChangeY)
	{
		return CheckBarrier(nChangeX << 10, nChangeY << 10);
	}

private:
	// NEW: A* pathfinding helper
	int				GetDirAStar(int nXpos, int nYpos, int nDestX, int nDestY, int *pnGetDir, int *pnStopOK);
};
#endif