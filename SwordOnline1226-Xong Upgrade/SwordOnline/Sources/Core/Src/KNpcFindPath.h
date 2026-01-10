
#ifndef KNPCFINDPATH_H
#define KNPCFINDPATH_H

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
public:
	KNpcFindPath();
	void			Init(int nNpc);
	int				GetDir(int nXpos,int nYpos, int nDir, int nDestX, int nDestY, int nMoveSpeed, int *pnGetDir, int* pnStopOK);		//
	int				Dir64To8(int nDir);
	int				Dir8To64(int nDir);
	BOOL			CheckDistance(int x1, int y1, int x2, int y2, int nDistance);
	int				CheckBarrier(int nChangeX, int nChangeY);
	INT				RealCheckBarrier(int nChangeX, int nChangeY)
	{
		return CheckBarrier(nChangeX << 10, nChangeY << 10);
	}
};
#endif
