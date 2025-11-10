#include "kcore.h"
#include <iostream>
#include <fstream>
#include "knpc.h"
#include "kplayer.h"
#include "knpcfindpath.h"
#include "kautoai.h"
#include <math.h>
#include "KMath.h"

using namespace std;

#ifndef _SERVER
class KSelfNpcFindPath	:	public KNpcFindPath
{
public:
	KSelfNpcFindPath() {m_NpcIdx = Player[CLIENT_PLAYER_INDEX].m_nIndex;}
};

#define AI_BLACK_COUNT			2
#define	AI_TIMES_ADDBLACK		15
#define	AI_TIMES_BLACK_CLOSE	5
#define AI_TIME_BLACK_LAST		(1000*3)

//////////// -- KAutoMove- ////////////
INT KAutoMove::AutoMove()
{
	if (m_CurDir < 0)
		m_CurDir = g_Random(8); //m_CurDir = KSysService:Rand() % 8;
	INT nDis;
	if ((nDis = FindPathAndMove(m_CurDir)) <= 0)
	{
		m_CurDir = (m_CurDir + 4) & 0x7;
		return FindPathAndMove(m_CurDir);
	}
	return nDis;
}

INT KAutoMove::AutoMoveTo( INT nX, INT nY)
{
	INT nOrgX, nOrgY;
	INT nPlayer = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	KNpc *pNpc = &Npc[nPlayer];
	pNpc->GetMpsPos(&nOrgX,&nOrgY);
	INT nDis = g_GetDisSquare(nOrgX, nOrgY, nX, nY);
	if (nDis <= 200 * 200)
	{
		pNpc->ClientGotoPos(nX,nY);
		return -2;
	}
	INT nDir = g_GetDirIndex(nOrgX, nOrgY, nX, nY);
	nDir = KSelfNpcFindPath().Dir64To8(nDir);
	if (m_CurDir < 0)
		m_CurDir = nDir;
	INT nCDir = (nDir + m_CurDir) / 2;
	if (abs(nDir - m_CurDir) > 4)
		nCDir = (nCDir +4) & 0x7;
	m_CurDir = nCDir;
	if ((nDis = FindPathAndMove(m_CurDir,min((INT)sqrt((DOUBLE)nDis),(INT)DIS_MOVE))) <= 0)
	{
		Reset();
		return AutoMove();
	}
	return nDis;
}

INT KAutoMove::FindPathAndMove( INT &nDir8, INT minStep /* == 1*/)
{
	if (minStep < 0)
		minStep = DIS_MOVE;
	INT nPlayer = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	KNpc *pNpc = &Npc[nPlayer];
	KSelfNpcFindPath path;
	INT nOrgX, nOrgY;
	pNpc->GetMpsPos(&nOrgX, &nOrgY);
	INT nDir64 = path.Dir8To64(nDir8 & 0x7);
	INT i, x, y;
	INT nSpeed = pNpc->m_CurrentRunSpeed;
	INT nLastDis = CheckDirectMoveDis(pNpc, nDir8, nSpeed);
	INT nMaxDis = nLastDis;
	i = 1;
	INT nGoodDir = nDir8;
	while(nMaxDis < minStep && i < 3)
	{
		INT nDis = CheckDirectMoveDis(pNpc, nDir8 + i, nSpeed);
		if (nDis > nMaxDis)
		{
			nMaxDis = nDis;
			nGoodDir = (nDir8 + i + 8) & 0x7;
		}
		i = i > 0 ? -i : 1 -i;
	}
	if (nMaxDis == 0)
	{
		return 0;
	}
	nDir8 = nGoodDir;
	nDir64 = path.Dir8To64(nDir8 & 0x7);
	x = (g_DirCos(nDir64, 64) * nMaxDis) >> 10;
	y = (g_DirSin(nDir64, 64) * nMaxDis) >> 10;
	pNpc->ClientGotoPos(nOrgX + x, nOrgY +y);
	return nMaxDis;
}

INT KAutoMove::CheckDirectMoveDis(KNpc *pNpc, INT nDir8, INT nSpeed)
{
	KSelfNpcFindPath path;
	INT nDir64 	= path.Dir8To64((nDir8 + 8) & 0x7);
	INT nDx 	= g_DirCos(nDir64, 64) * nSpeed;
	INT nDy 	= g_DirSin(nDir64, 64) * nSpeed;
	INT nDis 	= 0;
	INT i;
	for (i = 1; i <= DIS_MOVE / nSpeed; i++)
	{
		if (path.CheckBarrier(nDx * i, nDy * i) != Obstacle_NULL)
			return nDis;
		nDis += nSpeed;
	}
	return DIS_MOVE;
}

#endif //ifndef _SERVER