/* -------------------------------------------------------------------------
//	ÎÄ¼þÃû		£º	jx_path_finder.cpp
//	´´½¨Õß		£º	liupeng
//	´´½¨Ê±¼ä	£º	2007-5-17
//	¹¦ÄÜÃèÊö	£º	·â×°×Ô¶¯Ñ°Â·Àà
//
// -----------------------------------------------------------------------*/

#include "KCore.h"
#ifndef _SERVER

#include <math.h>
#include "KRegion.h"
#include "KJXpathfinder.h"
#include "Scene\ScenePlaceMapC.h"
#endif

#ifndef _SERVER
KJXPathFinder g_JXPathFinder;

#define USE_MAX_OBSTACLE 0

KJXPathFinder::KJXPathFinder(BOOL bFactor) : m_nMapWidth(0), m_nMapHeight(0), m_sCloseFlag(cs_CloseFlagStart),
m_sOpenFlag(cs_OpenFlagStart), m_map(NULL),
m_bHasFactor(bFactor)
{
}
KJXPathFinder::~KJXPathFinder()
{
	ClearMap();
}

BOOL KJXPathFinder::CreateMap(INT w, INT h)
{
	ClearMap();
	//ASSERT((sizeof(PathNode) % 4) == 0);

	m_map = new PathNode * [w];
	memset(m_map, 0, sizeof(PathNode*) * w);
	if (!m_map)
		return FALSE;

	for (INT i = 0; i < w; i++)
	{
		m_map[i] = new PathNode[h];
		if (!m_map[i])
		{
			ClearMap();
			return FALSE;
		}
		memset(m_map[i], 0, sizeof(PathNode) * h);
	}

	m_nMapWidth = w;
	m_nMapHeight = h;

	m_OpenTable.Reset(w * h);
	return TRUE;
}

VOID KJXPathFinder::ClearMap()
{
	for (INT i = 0; i < m_nMapWidth; i++)
		delete[] m_map[i];
	delete[] m_map;
	m_map = NULL;
	m_nMapWidth = 0;
	m_nMapHeight = 0;

	m_sCloseFlag = cs_CloseFlagStart;
	m_sOpenFlag = cs_OpenFlagStart;

	for (INT a = 0; a < m_OpenTable.GetSize(); a++)
		delete m_OpenTable.GetNode(a);

	m_OpenTable.Reset(0);
}

BOOL KJXPathFinder::Init(RECT* rc, KScenePlaceMapC* mapper)
{
	m_Stand.x = 0;
	m_Stand.y = 0;
	m_bReady = FALSE;
	//g_DebugLog("LOAD KJXPathFinder::Init %d - %d - %d - %d || %d", rc->left,  rc->top,  rc->right,  rc->bottom,mapper);
	if (!LoadMap(rc, mapper))
		return FALSE;

	m_target.x = m_target.y = 0;
	m_bReady = TRUE;
	return TRUE;
}

BOOL KJXPathFinder::LoadMap(RECT* rc, KScenePlaceMapC* mapper)
{
	//g_DebugLog("LOAD KJXPathFinder MAP: %d - %d - %d - %d", rc->left,  rc->top,  rc->right,  rc->bottom);
	m_LeftTop.left = rc->left * 16;
	m_LeftTop.top = rc->top * 32;
	m_LeftTop.right = rc->right * 16;
	m_LeftTop.bottom = rc->bottom * 32;

	INT w = (rc->right - rc->left + 1) * RegionWidth;
	INT h = (rc->bottom - rc->top + 1) * RegionHeight;

	if (!CreateMap(w, h))
		return FALSE;
	//g_DebugLog("LOAD KJXPathFinder MAP : CREATMAP w: %d - h: %d",w,h);
	//ASSERT(w * h == nSize);
	for (INT y = 0; y < h; y++)
	{
		for (INT x = 0; x < w; x++)
		{
			BYTE* lpbtObstacle = mapper->GetbtBarrier(rc->left + x, rc->top + y);
			if (!lpbtObstacle)
				continue;
			int count = 0;
			for (int i = 0; i < REGION_GRID_HEIGHT; i++)
			{//512 points
				for (int j = 0; j < REGION_GRID_WIDTH; j++)
				{
					INT obstacle = lpbtObstacle[i * REGION_GRID_WIDTH + j];

					m_map[x * REGION_GRID_WIDTH + j][y * REGION_GRID_HEIGHT + i].factor = 1.0f;
					if (obstacle == 0 || (2 <= obstacle && obstacle <= 5)) { //Not documenting complete obstacles?
						m_map[x * REGION_GRID_WIDTH + j][y * REGION_GRID_HEIGHT + i].canwoalk = walkable;  //record obstacles
						count++;
					}

					//g_DebugLog("m_map x = %d, y = %d %d", x * REGION_GRID_WIDTH + j, y * REGION_GRID_HEIGHT + i, obstacle); 
				}
			}
			//g_DebugLog("count Finder %d", count);
		}
	}
	if (m_bHasFactor)
		CalcFactors();
	return TRUE;
}

// ¼ÆËãµãµÄÈ¨Öµ£¬¸ù¾ÝµãÀëÕÏ°­µãµÄ¾àÀë¾ö¶¨ËûµÄÈ¨Öµ¡£

// Îª¼ÆËãÈ¨Öµ£¬ÊÇ¿¼ÂÇÃ¿¸öÕÏ°­µãÈ¥¼ÆËãÖÜÎ§µÄ·ÇÕÏ°­µãµÄÖµ£¬ÕâÑù±ÈÖð¸öÖ±½Ó¼ÆËã·ÇÕÏ°­µã
// µÄÈ¨ÖµÓÐÐ§ÂÊµÄ¶à¡£

// »ù±¾¼ÙÉè¾ÍÊÇÈç¹ûÒ»¸öÕÏ°­µãÔÚµ±Ç°ÕÏ°­µãµÄ×ó±ß£¬ÄÇÃ´¾Í²»ÐèÒª¼ÆËãµ±Ç°ÕÏ°­µã
// ¶ÔËü×ó±ßµÄµãµÄÓ°Ïì£¬ÒòÎªËü×ó±ßµÄÒ»¸öÕÏ°­µã¶ÔËüÃÇµÄÓ°Ïì¿Ï¶¨Òª´óÓÚµ±Ç°ÕÏ°­µã
// ÆäËû¿ÉÒÔ½øÐÐ¼ÓËÙ¼ÆËãµÄÇé¿ö¾ÍÒÀ´ËÀàÍÆÁË¡£×îÖÕµÄÐ§¹û¾ÍÊÇÐèÒª¼ÆËãµÄ¾ÍÊÇ¿¿½ü±ß½çµÄ
// ÕÏ°­µãÐèÒª¼ÆËã¡£Ö»ÊÇÕâÑù×öÈÔÈ»ÓÐÈßÓàµÄÇé¿ö³öÏÖ¡£

// Ìá¸ßÐ§ÂÊµÄ×ö·¨ÊÇ²é¿´Ò»¸öÕÏ°­µãÖÜÎ§µÄ8¸öµã£¬Õë¶Ô¸÷ÖÖ²»Í¬µÄ×éºÏÇé¿ö½øÐÐ²Ã¼ôÐèÒª
// ¼ÆËãµÄµã£¬Ô­ÔòÉÏ¿ÉÒÔ½øÐÐ¸ü¼ÓÏ¸ÖÂµÄ²Ã¼ô£¬µ«ÊÇÈç¹û¿¼ÂÇ¸ü´óµÄ·¶Î§¾Í»áÊ¹¸´ÔÓÐÔ´ó´ó
// Ôö¼Ó

// ¿ªÊ¼µÄÊ±ºòËùÓÐµÄµãµÄfactor¶¼Ò»ÑùÎª×îÐ¡Öµ1£¬µ±·¢ÏÖÒ»¸öÕÏ°­µã¶ÔÄ³¸öµãÓÐ×÷ÓÃµÄÊ±ºò
// Èç¹û·¢ÏÖ¶ÔËüÓ°Ïì´óÓÚfactor¾ÍÓÃ¼ÆËãËùµÃµÄÖµÌæ»»Ëü¡£

// ÕÏ°­µãµÄÓ°Ïì°ë¾¶
CONST INT	cnFactorRadius = 15;

// dx£¬dyÎªµãµÄ¾àÀë°ë¾¶,ÎªÌá¸ßËÙ¶È²ÉÓÃ²é±íµÄ·½Ê½À´¼ÆËã
inline FLOAT CalcFactor(INT dx, INT dy)
{
	// ²ÉÓÃ¼òµ¥¼ÆËãµÄ·½Ê½
	_ASSERT(abs(dx) < cnFactorRadius && abs(dy) < cnFactorRadius);
	_ASSERT(abs(dx) + abs(dy) > 0);
	FLOAT dis = sqrt((FLOAT)(dx * dx + dy * dy));
	_ASSERT(dis >= 1.0f);
	if ((FLOAT)cnFactorRadius - dis <= 0)
		return 1.0f;
	FLOAT res = 2.0f * ((FLOAT)cnFactorRadius - dis) / (FLOAT)cnFactorRadius + 1.0f;
	return res;
}

VOID KJXPathFinder::CalcFactors()
{
	// ÏÈ²ÉÓÃ¼òµ¥µÄ·½Ê½¼ÆËã
	// ÏÈ¼ÆËã±ß½çµÄÓ°Ïì
	{
		for (INT a = 0; a < cnFactorRadius - 1; a++)
		{
			FLOAT factor = CalcFactor(a + 1, 0);
			// ×ó±ßºÍÓÒ±ß
			INT i = 0;
			for (i = 0; i < m_nMapHeight; i++)
			{
				if (m_map[a][i].canwoalk && factor > m_map[a][i].factor)
					m_map[a][i].factor = factor;
				if (m_map[m_nMapWidth - a - 1][i].canwoalk && factor > m_map[m_nMapWidth - a - 1][i].factor)
					m_map[m_nMapWidth - a - 1][i].factor = factor;
			}
			// ÉÏ±ßºÍÏÂ±ß
			for (i = 0; i < m_nMapWidth; i++)
			{
				if (m_map[i][a].canwoalk && factor > m_map[i][a].factor)
					m_map[i][a].factor = factor;
				if (m_map[i][m_nMapHeight - a - 1].canwoalk && factor > m_map[i][m_nMapHeight - a - 1].factor)
					m_map[i][m_nMapHeight - a - 1].factor = factor;
			}
		}
	}

	// Ã¿¸öÕÏ°­µã¼ÆËãËüµÄ¸²¸Ç·¶Î§
	for (INT i = 0; i < m_nMapWidth; i++)
	{
		for (INT j = 0; j < m_nMapHeight; j++)
		{
			if (!m_map[i][j].canwoalk)
			{
				// ²Ã¼ôµôÉÏÏÂ×óÓÒ¶¼ÓÐÕÏ°­µãµÄÕÏ°­µã
				BOOL bCanCalc = FALSE;
				if (i - 1 >= 0 && m_map[i - 1][j].canwoalk)		// ×ó
					bCanCalc = TRUE;
				else if (i + 1 < m_nMapWidth && m_map[i + 1][j].canwoalk) // ÓÒ
					bCanCalc = TRUE;
				else if (j - 1 >= 0 && m_map[i][j - 1].canwoalk)		// ÉÏ
					bCanCalc = TRUE;
				else if (j + 1 < m_nMapHeight && m_map[i][j + 1].canwoalk) // ÏÂ
					bCanCalc = TRUE;
				if (bCanCalc)
				{
					// ÔÚÒ»¸öÕý·½ÐÎ±ß½çÖÐÑ°ÕÒ×÷ÓÃµã
					INT x = i - cnFactorRadius + 1;
					INT y = j - cnFactorRadius + 1;

					for (INT a = x; a < x + cnFactorRadius * 2 - 1; a++)
						for (INT b = y; b < y + cnFactorRadius * 2 - 1; b++)
							if (a >= 0 && a < m_nMapWidth && b >= 0 && b < m_nMapHeight)
							{
								if (m_map[a][b].canwoalk)
								{
									FLOAT factor = CalcFactor(a - i, b - j);
									if (factor > m_map[a][b].factor)
										m_map[a][b].factor = factor;
								}
							}
				}
			}
		}
	}
}
void KJXPathFinder::FillNote(int x, int y, int z)
{
	int nowx = x / 32 - m_LeftTop.left;
	int nowy = y / 32 - m_LeftTop.top;

	m_map[nowx][nowy].canwoalk = unwalkable;
}
BOOL KJXPathFinder::FindPath(int OldX, int OldY, int nXpos, int nYpos)
{
	FindPathNode start, target;
	start.x = OldX / 32 - m_LeftTop.left;
	start.y = OldY / 32 - m_LeftTop.top;

	target.x = nXpos / 32 - m_LeftTop.left;
	target.y = nYpos / 32 - m_LeftTop.top;

	m_target.x = m_target.x = -1;
	return FindPath(start, target);
}
void KJXPathFinder::GetPath(int OldX, int OldY, std::vector<FindPathNode>& PathIts)
{
	int nowx = OldX / 32 - m_LeftTop.left;
	int nowy = OldY / 32 - m_LeftTop.top;

	INT x = nowx, y = nowy;
	INT nLen = 0;
	while (!(x == m_target.x && y == m_target.y))
	{
		nLen++;
		INT tempx = m_map[x][y].parent_x;
		INT tempy = m_map[x][y].parent_y;
		x = tempx;
		y = tempy;
	}
	if (nLen)
	{
		x = nowx;
		y = nowy;

		bool data = false;
		while (true)
		{
			if (data)
			{
				FindPathNode get;
				get.x = (x + m_LeftTop.left) * 32;
				get.y = (y + m_LeftTop.top) * 32;
				PathIts.push_back(get);
			}
			else
			{
				data = true;
			}

			if (x == m_target.x && y == m_target.y)
				break;

			INT tempx = m_map[x][y].parent_x;
			INT tempy = m_map[x][y].parent_y;
			x = tempx;
			y = tempy;
		}
	}
}
BOOL KJXPathFinder::FindPath(FindPathNode& start, FindPathNode& target, BOOL bFactor)
{
	if (!m_bReady)
		return FALSE;

	if (target.x == m_target.x && target.y == m_target.y)
	{
		// The target point is the same, find the next point directly
		FindPathNode next;
		return GetNextStep(start, next) != emKNEXTSTEP_RESULT_NOANYWAY;
	}
	else
	{
		if (target.x < 0 || target.x >= m_nMapWidth || target.y < 0 || target.y >= m_nMapHeight)
			return emKNEXTSTEP_RESULT_NOANYWAY;

		m_target.x = target.x;
		m_target.y = target.y;
		m_bHasFactor = bFactor;

		// Erase previous information
		ResetMap();
		// Put the information of the first point into the open table
		WORD hcost = 10 * (abs(start.x - target.x) + abs(start.y - target.y));
		AddOpenNode(m_target.x, m_target.y, hcost, hcost, -1, -1);

		// find the next
		FindPathNode next;
		return GetNextStep(start, next) != emKNEXTSTEP_RESULT_NOANYWAY;
	}
	return TRUE;
}
KE_NEXTSTEP_RESULT
KJXPathFinder::GetNextStep(CONST FindPathNode& now, FindPathNode& nextstep, INT steplen)
{
	if (!m_bReady)
		return emKNEXTSTEP_RESULT_NOANYWAY;
	FindPathNode tmpnow;
	tmpnow.x = now.x / 32 - m_LeftTop.left;
	tmpnow.y = now.y / 32 - m_LeftTop.top;

	_ASSERT(steplen > 0);
	KE_NEXTSTEP_RESULT  res = GetNextStep(tmpnow, nextstep);
	if (res == emKNEXTSTEP_RESULT_NOANYWAY || res == emKNEXTSTEP_RESULT_ARRIVAL)
		return res;
	else
	{
		// res == emKNEXTSTEP_RESULT_SUCCESS
		_ASSERT(m_map[tmpnow.x][tmpnow.y].pathflag == m_sCloseFlag ||
			m_map[tmpnow.x][tmpnow.y].pathflag == m_sOpenFlag);
		INT x = tmpnow.x, y = tmpnow.y;
		while (!(x == m_target.x && y == m_target.y))
		{
			INT tempx = m_map[x][y].parent_x;
			INT tempy = m_map[x][y].parent_y;
			x = tempx;
			y = tempy;
			steplen--;
			if (steplen == 0)
			{
				nextstep.x = (x + m_LeftTop.left) * 32;
				nextstep.y = (y + m_LeftTop.top) * 32;
				return res;
			}
		}
		nextstep.x = m_target.x;
		nextstep.y = m_target.y;
		return res;
	}
}
KE_NEXTSTEP_RESULT
KJXPathFinder::GetNextStep(CONST FindPathNode& now, FindPathNode& nextstep)
{
	if (!m_bReady)
		return emKNEXTSTEP_RESULT_NOANYWAY;
	// Determine whether a search is required
	if (now.x < 0 || now.x >= m_nMapWidth || now.y < 0 || now.y >= m_nMapHeight)
		return emKNEXTSTEP_RESULT_NOANYWAY;

	if (m_map[m_target.x][m_target.y].canwoalk != walkable)
		return emKNEXTSTEP_RESULT_NOANYWAY;
	if (m_map[now.x][now.y].canwoalk != walkable)
		return emKNEXTSTEP_RESULT_NOANYWAY;

	if (now.x == m_target.x && now.y == m_target.y)
		return emKNEXTSTEP_RESULT_ARRIVAL;

	// Already in the open table or close table
	if ((m_map[now.x][now.y].pathflag == m_sOpenFlag) ||
		(m_map[now.x][now.y].pathflag == m_sCloseFlag))
	{
		nextstep.x = m_map[now.x][now.y].parent_x;
		nextstep.y = m_map[now.x][now.y].parent_y;
		return emKNEXTSTEP_RESULT_SUCCESS;
	}

	//need to search
	// Correct the cost value in the open table so that its value is based on the new target.
	FixOpenTable(now);
	while (!m_OpenTable.empty())
	{
		OpenNode* pNode = m_OpenTable.PopNode();
		AddCloseNode(pNode, now.x, now.y);
		delete pNode;

		if ((m_map[now.x][now.y].pathflag == m_sOpenFlag))
		{
			nextstep.x = m_map[now.x][now.y].parent_x;
			nextstep.y = m_map[now.x][now.y].parent_y;
			return emKNEXTSTEP_RESULT_SUCCESS;
		}
	}
	return emKNEXTSTEP_RESULT_NOANYWAY;
}

VOID KJXPathFinder::ClearOpenTable()
{
	for (INT a = 0; a < m_OpenTable.GetSize(); a++)
		delete m_OpenTable.GetNode(a);
	m_OpenTable.Reset(0);
}
VOID KJXPathFinder::ResetMap()
{
	ClearOpenTable();
	m_OpenTable.Reset(m_nMapHeight * m_nMapWidth);
	for (INT y = 0; y < m_nMapHeight; y++)
		for (INT x = 0; x < m_nMapWidth; x++)
		{
			PathNode& node = m_map[x][y];
			node.gconst = 0;
			node.parent_x = 0;
			node.parent_y = 0;
			node.pathflag = 0;
		}
}

VOID KJXPathFinder::AddOpenNode(INT x, INT y, WORD fcost, WORD hcost, INT px, INT py)
{
	// The node put into the open table must be a node that is neither in the open table nor in the close table
	// ASSERT(m_map[x][y].pathflag == 0 && m_map[x][y].canwoalk == walkable);
	if (m_map[x][y].pathflag != 0 || m_map[x][y].canwoalk != walkable)
	{
		return;
	}
	OpenNode* pNode = new OpenNode;
	pNode->x = x;
	pNode->y = y;
	pNode->fcost = fcost;
	pNode->hcost = hcost;
	m_OpenTable.PushNode(pNode);

	m_map[x][y].gconst = fcost - hcost;
	m_map[x][y].parent_x = px;
	m_map[x][y].parent_y = py;
	m_map[x][y].pathflag = (BYTE)m_sOpenFlag;
}

// ½«Ò»¸öopen±íÖÐµÄ½Úµã·ÅÈë
VOID KJXPathFinder::AddCloseNode(OpenNode* pNode, INT tx, INT ty)
{
	// ·ÅÈëclose±íµÄ±ØÐëÊÇ
	_ASSERT(m_map[pNode->x][pNode->y].pathflag == m_sOpenFlag);
	m_map[pNode->x][pNode->y].pathflag = (BYTE)m_sCloseFlag;

	// ²éÕÒºÍ¸Ãopen½ÚµãÏàÁÚµÄ½Úµã,Èç¹û²»ÊÇopen»òclose±íÖÐµÄ½Úµã,Ôò·ÅÈëopen±íÖÐ
	// Èç¹ûÊÇopen±íÖÐµÄ½Úµã,±È½ÏÐÂµÄclose½ÚµãºÍ
	for (INT y = pNode->y - 1; y <= pNode->y + 1; y++)
		for (INT x = pNode->x - 1; x <= pNode->x + 1; x++)
		{
			if (x < 0 || x >= m_nMapWidth || y < 0 || y >= m_nMapHeight)
				continue;
			if (m_map[x][y].canwoalk != walkable)
				continue;
			if (m_map[x][y].pathflag == m_sCloseFlag)
				continue;
			// ÅÐ¶ÏÊÇ·ñ¿ÉÒÔ´Óµ±Ç°µã×ßµ½À©Õ¹µã
			WORD corner = walkable;
			if (x == pNode->x - 1)
			{
				if (y == pNode->y - 1)
				{
					if (m_map[pNode->x - 1][pNode->y].canwoalk == unwalkable
						|| m_map[pNode->x][pNode->y - 1].canwoalk == unwalkable)
						corner = unwalkable;
				}
				else if (y == pNode->y + 1)
				{
					if (m_map[pNode->x][pNode->y + 1].canwoalk == unwalkable
						|| m_map[pNode->x - 1][pNode->y].canwoalk == unwalkable)
						corner = unwalkable;
				}
			}
			else if (x == pNode->x + 1)
			{
				if (y == pNode->x - 1)
				{
					if (m_map[pNode->x][pNode->y - 1].canwoalk == unwalkable
						|| m_map[pNode->x + 1][pNode->y].canwoalk == unwalkable)
						corner = unwalkable;
				}
				else if (y == pNode->x + 1)
				{
					if (m_map[pNode->x + 1][pNode->y].canwoalk == unwalkable
						|| m_map[pNode->x][pNode->y + 1].canwoalk == unwalkable)
						corner = unwalkable;
				}
			}
			if (corner != walkable)
				continue;

			// ¼ÆËãÀ©Õ¹µãµ½open½ÚµãµÄ»¨Ïú
			_ASSERT(abs(x - pNode->x) + abs(y - pNode->y) != 0);
			FLOAT addCost = 0;
			if (abs(x - pNode->x) == 1 && abs(y - pNode->y) == 1)
			{
				if (m_bHasFactor)
					addCost = 14.0f * m_map[pNode->x][pNode->y].factor;
				else
					addCost = 14.0f;
			}
			else
			{
				if (m_bHasFactor)
					addCost = 10.0f * m_map[pNode->x][pNode->y].factor;
				else
					addCost = 10.0f;
			}

			// ¼ÆËãµ±Ç°À©Õ¹µãµÄgcost
			WORD gcost = m_map[pNode->x][pNode->y].gconst + (WORD)addCost;

			if (m_map[x][y].pathflag != m_sOpenFlag)
			{
				// Ìí¼Óµ±Ç°À©Õ¹½Úµãµ½open±í
				WORD hcost = 10 * (abs(x - tx) + abs(y - ty));
				AddOpenNode(x, y, gcost + hcost, hcost, pNode->x, pNode->y);
			}
			else
			{
				// ÐÞ¸ÄÒÑ¾­ÊÇopen±íÖÐµÄ½Úµã
				if (gcost < m_map[x][y].gconst)
				{
					m_map[x][y].gconst = gcost;
					m_map[x][y].parent_x = pNode->x;
					m_map[x][y].parent_y = pNode->y;
					for (INT i = 0; i < m_OpenTable.GetSize(); i++)
					{
						OpenNode* pOpenNode = m_OpenTable.GetNode(i);
						if (pOpenNode->x == x && pOpenNode->y == y)
						{
							pOpenNode->fcost = gcost + pOpenNode->hcost;
							m_OpenTable.MakeHeap();
							break;
						}
					}
				}
			}
		}
}
// ¸ù¾ÝÐÂµÄÄ¿±êÎ»ÖÃÐÞÕýopen±íÖÐµÄÈ¡Öµ
VOID KJXPathFinder::FixOpenTable(CONST FindPathNode& now)
{
	for (INT a = 0; a < m_OpenTable.GetSize(); a++)
	{
		OpenNode* pNode = m_OpenTable.GetNode(a);
		WORD oldfcost = pNode->fcost;
		WORD oldhcost = pNode->hcost;
		_ASSERT(oldfcost - oldhcost == m_map[pNode->x][pNode->y].gconst);
		pNode->hcost = 10 * (abs(pNode->x - now.x) + abs(pNode->y - now.y));
		pNode->fcost = pNode->hcost + (oldfcost - oldhcost);
	}
	m_OpenTable.MakeHeap();
}

//
INT	KJXPathFinder::GetPath(FindPathNode& now, FindPathNode*& pPath)
{
	if (!m_bReady)
		return 0;

	_ASSERT(m_map[now.x][now.y].pathflag == m_sCloseFlag ||
		m_map[now.x][now.y].pathflag == m_sOpenFlag);
	INT x = now.x, y = now.y;
	INT nLen = 0;
	while (!(x == m_target.x && y == m_target.y))
	{
		nLen++;
		INT tempx = m_map[x][y].parent_x;
		INT tempy = m_map[x][y].parent_y;
		x = tempx;
		y = tempy;
	}
	if (nLen)
	{
		x = now.x;
		y = now.y;
		pPath = new FindPathNode[nLen];
		FindPathNode* pCurNode = pPath;
		INT nTestLen = 0;
		while (!(x == m_target.x && y == m_target.y))
		{
			pCurNode->x = x;
			pCurNode->y = y;
			pCurNode++;
			nTestLen++;
			INT tempx = m_map[x][y].parent_x;
			INT tempy = m_map[x][y].parent_y;
			x = tempx;
			y = tempy;
		}
		_ASSERT(nTestLen == nLen);
	}

	if (pPath)
	{	
		delete[] pPath;
		pPath = NULL;
	}
	return nLen;
}
#endif