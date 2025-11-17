#include "KCore.h"
#include "KNpc.h"
#include "KNpcSet.h"
#include "KSubWorld.h"
#include "KMath.h"
#include "KObj.h"
#include "KPlayer.h"
#include "KNpcAI.h"

// flying add here, to use math lib
#include <math.h>
extern int GetRandomNumber(int nMin, int nMax);

#define		MAX_FOLLOW_DISTANCE		100
#define		FOLLOW_WALK_DISTANCE	48

KNpcAI NpcAI;

KNpcAI::KNpcAI()
{
	m_nIndex = 0;
	m_bActivate = TRUE;
}

void KNpcAI::Activate(int nIndex)
{
	m_nIndex = nIndex;	
	if (Npc[m_nIndex].IsPlayer())
	{
		ProcessPlayer();
		return;
	}
#ifdef _SERVER
	if (Npc[m_nIndex].m_CurrentLifeMax == 0)
		return;
	int nCurTime = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_dwCurrentTime;
	if (/*Npc[m_nIndex].m_nPeopleIdx ||*/Npc[m_nIndex].m_NextAITime <= nCurTime)
	{
		Npc[m_nIndex].m_NextAITime = nCurTime + Npc[m_nIndex].m_AIMAXTime;
		switch(Npc[m_nIndex].m_AiMode)
		{
		case 1:
			ProcessAIType01();
			break;
		case 2:
			ProcessAIType02();
			break;
		case 3:
			ProcessAIType03();
			break;
		case 4:
			ProcessAIType04();
			break;
		case 5:
			ProcessAIType05();
			break;
		case 6:
			ProcessAIType06();
			break;
		case 7:
			ProcessAIType07();
			break;
		case 8:
			ProcessAIType08(); // Follow owner player
			break;
/*		case 9:
			ProcessAIType9();
			break;
		case 10:
			ProcessAIType10();
			break;*/
		default:
			break;
		}
	}
#else
	if (Npc[m_nIndex].m_Kind > 3 && Npc[m_nIndex].m_AiMode > 10)
	{
		if (CanShowNpc())
		{
			if (GetRandomNumber(0, 1))	
			{
				Npc[m_nIndex].m_AiParam[5] = 0;
				Npc[m_nIndex].m_AiParam[4] = 5;
				return;
			}
			if (!KeepActiveRange())
				ProcessShowNpc();
		}
	}
#endif
}

// flying add these functions
// Run at client.
#ifndef _SERVER
// �����л���Ч����NPC
int KNpcAI::ProcessShowNpc()
{
    int nResult  = false;
    int nRetCode = false;

	switch (Npc[m_nIndex].m_AiMode)
	{
	// ������
	case 11:
		nRetCode = ShowNpcType11();
        if (!nRetCode)
            goto Exit0;
		break;
	// ������
	case 12:
		nRetCode = ShowNpcType12();
        if (!nRetCode)
            goto Exit0;
		break;
	// ������
	case 13:
		nRetCode = ShowNpcType13();
        if (!nRetCode)
            goto Exit0;
		break;
	// ������
	case 14:
		nRetCode = ShowNpcType14();
        if (!nRetCode)
            goto Exit0;
		break;
	// ��Ȯ��
	case 15:
		nRetCode = ShowNpcType15();
        if (!nRetCode)
            goto Exit0;
		break;
	// ������
	case 16:
		nRetCode = ShowNpcType16();
        if (!nRetCode)
            goto Exit0;
		break;
	// ������
	case 17:
		nRetCode = ShowNpcType17();
        if (!nRetCode)
            goto Exit0;
		break;
	default:
		break;
	}

    nResult = true;
Exit0:
	return nResult;
}

// ������
int KNpcAI::ShowNpcType11()
{
    int nResult = false;
    int nRetCode = false;

	KNpc& aNpc = Npc[m_nIndex];
	// Go the distance between P1 to P2	
	int nDistance = 0;
	int nDesX = 0;
	int nDesY = 0;
	int nCurX = 0;
	int nCurY = 0;
	int nOffX = 0;
	int nOffY = 0;
	int nOffsetDir = 0;
	
	// Ч����ǿ ��������߶�
	aNpc.m_Height = GetRandomNumber(aNpc.m_AiParam[6] - 4, aNpc.m_AiParam[6]);

	aNpc.GetMpsPos(&nCurX, &nCurY);

	// �����½Ƕ� �� ����
	if (aNpc.m_AiParam[3] > 0)
		nOffsetDir = GetRandomNumber(aNpc.m_AiParam[3], aNpc.m_AiParam[2]);
	else
		nOffsetDir = aNpc.m_AiParam[2];
	
    if (GetRandomNumber(0, 1))
		nOffsetDir = -nOffsetDir;
	
    nDistance = GetRandomNumber(aNpc.m_AiParam[0] - aNpc.m_AiParam[1], aNpc.m_AiParam[0]);

	if (aNpc.m_CurrentWalkSpeed > 0)
	{
		aNpc.m_AiParam[4] = (int)(nDistance / aNpc.m_CurrentWalkSpeed);		//
		aNpc.m_AiParam[5] = 0;
	}
	//if (KeepActiveShowRange())
	//	aNpc.m_Dir += 32;
	aNpc.m_Dir += nOffsetDir;
	if (aNpc.m_Dir < 0)
		aNpc.m_Dir += 64;
	else
		aNpc.m_Dir %= 64;
	
    // �������Ǻ�������ƫ�Ƶ�X��Y��ֵ
	nRetCode = GetNpcMoveOffset(aNpc.m_Dir, nDistance, &nOffX, &nOffY);
    if (!nRetCode)
        goto Exit0;

	// ��ȡĿ������
	nDesX = nCurX + nOffX;
	nDesY = nCurY + nOffY;
	aNpc.SendCommand(do_walk, nDesX, nDesY);	

    nResult = true;
Exit0:
	return nResult;
}

// ������
// done
int KNpcAI::ShowNpcType12()
{
    int nResult = false;
    int nRetCode = false;

	// Go the distance between P1 to P2	
	int nDistance = 0;
	int nDesX = 0;
	int nDesY = 0;
	int nCurX = 0;
	int nCurY = 0;
	int nOffX = 0;
	int nOffY = 0;
	int nOffsetDir = 0;
	KNpc& aNpc = Npc[m_nIndex];

	// Ч����ǿ ��������߶�
	aNpc.m_Height = GetRandomNumber(aNpc.m_AiParam[6] - 4, aNpc.m_AiParam[6]);

	aNpc.GetMpsPos(&nCurX, &nCurY);
		
	// �����½Ƕ� �� ����
	if (aNpc.m_AiParam[3] > 0)
		nOffsetDir = GetRandomNumber(aNpc.m_AiParam[3], aNpc.m_AiParam[2]);
	else
		nOffsetDir = aNpc.m_AiParam[2];
	if (GetRandomNumber(0, 1))
		nOffsetDir = -nOffsetDir;
	nDistance = GetRandomNumber(aNpc.m_AiParam[0] - aNpc.m_AiParam[1], aNpc.m_AiParam[0]);

	// ȡ���˶������ʱ�䣬�����ڲ�������
	if (aNpc.m_CurrentWalkSpeed > 0)
	{
		aNpc.m_AiParam[4] = (int)(nDistance / aNpc.m_CurrentWalkSpeed);		//
		aNpc.m_AiParam[5] = 0;
	}
	else
	{
		aNpc.m_AiParam[4] = 0;
		aNpc.m_AiParam[5] = 0;
	}
	aNpc.m_Dir += nOffsetDir;
	if (aNpc.m_Dir < 0)
		aNpc.m_Dir += 64;
	else
		aNpc.m_Dir %= 64;

	// �������Ǻ�������ƫ�Ƶ�X��Y��ֵ
	nRetCode = GetNpcMoveOffset(aNpc.m_Dir, nDistance, &nOffX, &nOffY);
	if (!nRetCode)
		goto Exit0;
	// ��ȡĿ������
	nDesX = nCurX + nOffX;
	nDesY = nCurY + nOffY;
	aNpc.SendCommand(do_walk, nDesX, nDesY);

	nResult = true;
Exit0:
	return nResult;
}

// ������
// done
int KNpcAI::ShowNpcType13()
{
	int nResult  = false;
	int nRetCode = false;
	// Go the distance between P1 to P2	
	int nDistance = 0;
	int nDesX = 0;
	int nDesY = 0;
	int nCurX = 0;
	int nCurY = 0;
	int nOffX = 0;
	int nOffY = 0;
	int nOffsetDir = 0;
	int nIndex = 0;
	KNpc& aNpc = Npc[m_nIndex];

	aNpc.GetMpsPos(&nCurX, &nCurY);

	// �����½Ƕ� �� ����
	if (aNpc.m_AiParam[3] > 0)
		nOffsetDir = GetRandomNumber(aNpc.m_AiParam[3], aNpc.m_AiParam[2]);
	else
		nOffsetDir = aNpc.m_AiParam[2];
	if (GetRandomNumber(0, 1))
		nOffsetDir = -nOffsetDir;
	nDistance = GetRandomNumber(aNpc.m_AiParam[0] - aNpc.m_AiParam[1], aNpc.m_AiParam[0]);

	// ȡ���˶������ʱ�䣬�����ڲ�������
	if (aNpc.m_CurrentWalkSpeed > 0)
	{
		aNpc.m_AiParam[4] = (int)(nDistance / aNpc.m_CurrentWalkSpeed);		//
		aNpc.m_AiParam[5] = 0;
	}
	else
	{
		aNpc.m_AiParam[4] = 0;
		aNpc.m_AiParam[5] = 0;
	}
	//if (KeepActiveShowRange())
	//	aNpc.m_Dir += 32;
	// ���������
	nIndex = IsPlayerCome();
	if (nIndex > 0)
	{
		// do flee
		DoShowFlee(nIndex);
		goto Exit0;
	}
	// �������Ǻ�������ƫ�Ƶ�X��Y��ֵ
	nRetCode = GetNpcMoveOffset(aNpc.m_Dir, nDistance, &nOffX, &nOffY);
	// ��ȡĿ������
	nDesX = nCurX + nOffX;
	nDesY = nCurY + nOffY;
	aNpc.SendCommand(do_walk, nDesX, nDesY);

	nResult = true;
Exit0:
	return nResult;
}

// ������
// done
int KNpcAI::ShowNpcType14()
{
	int nResult  = false;
	int nRetCode = false;

	int nDistance = 0;
	int nDesX = 0;
	int nDesY = 0;
	int nCurX = 0;
	int nCurY = 0;
	int nOffX = 0;
	int nOffY = 0;
	int nRandom = 0;
	int nOffsetDir = 0;
	KNpc& aNpc = Npc[m_nIndex];

	nRandom = GetRandomNumber(1, 10);
	// ��ͷ����
	if (nRandom < 4)
		nDistance = -nDistance;
	// �໷���
	else if (nRandom < 7)
	{
		aNpc.SendCommand(do_stand);
		goto Exit0;
	}
	aNpc.GetMpsPos(&nCurX, &nCurY);
	// �����½Ƕ� �� ����
	if (aNpc.m_AiParam[3] > 0)
		nOffsetDir = GetRandomNumber(aNpc.m_AiParam[3], aNpc.m_AiParam[2]);
	else
		nOffsetDir = aNpc.m_AiParam[2];
	if (GetRandomNumber(0, 1))
		nOffsetDir = -nOffsetDir;
	nDistance = GetRandomNumber(aNpc.m_AiParam[0] - aNpc.m_AiParam[1], aNpc.m_AiParam[0]);
	// ȡ���˶������ʱ�䣬�����ڲ�������
	if (aNpc.m_CurrentWalkSpeed > 0)
	{
		aNpc.m_AiParam[4] = (int)(nDistance / aNpc.m_CurrentWalkSpeed);		//
		aNpc.m_AiParam[5] = 0;
	}
	else
	{
		aNpc.m_AiParam[4] = 0;
		aNpc.m_AiParam[5] = 0;
	}

	aNpc.m_Dir += nOffsetDir;
	if (aNpc.m_Dir < 0)
		aNpc.m_Dir += 64;
	else
		aNpc.m_Dir %= 64;
	// �������Ǻ�������ƫ�Ƶ�X��Y��ֵ
	nRetCode = GetNpcMoveOffset(aNpc.m_Dir, nDistance, &nOffX, &nOffY);
	if (!nRetCode)
		goto Exit0;
	// ��ȡĿ������
	nDesX = nCurX + nOffX;
	nDesY = nCurY + nOffY;
	aNpc.SendCommand(do_walk, nDesX, nDesY);

	nResult = true;
Exit0:
	return nResult;
}

// ��Ȯ��
int KNpcAI::ShowNpcType15()
{
	int nResult  = false;
	int nRetCode = false;
	// Go the distance between P1 to P2	
	int nDistance = 0;
	int nDesX = 0;
	int nDesY = 0;
	int nCurX = 0;
	int nCurY = 0;
	int nOffX = 0;
	int nOffY = 0;
	int nOffsetDir = 0;
	int nIndex = 0;
	KNpc& aNpc = Npc[m_nIndex];

	aNpc.GetMpsPos(&nCurX, &nCurY);

	// �����½Ƕ� �� ����
	if (aNpc.m_AiParam[3] > 0)
		nOffsetDir = GetRandomNumber(aNpc.m_AiParam[3], aNpc.m_AiParam[2]);
	else
		nOffsetDir = aNpc.m_AiParam[2];
	if (GetRandomNumber(0, 1))
		nOffsetDir = -nOffsetDir;
	nDistance = GetRandomNumber(aNpc.m_AiParam[0] - aNpc.m_AiParam[1], aNpc.m_AiParam[0]);

	// ȡ���˶������ʱ�䣬�����ڲ�������
	if (aNpc.m_CurrentWalkSpeed > 0)
	{
		aNpc.m_AiParam[4] = (int)(nDistance / aNpc.m_CurrentWalkSpeed);		//
		aNpc.m_AiParam[5] = 0;
	}
	else
	{
		aNpc.m_AiParam[4] = 0;
		aNpc.m_AiParam[5] = 0;
	}
	//if (KeepActiveShowRange())
	//	aNpc.m_Dir += 32;
	// ���������
	nIndex = IsPlayerCome();
	if (nIndex > 0)
	{
		// do flee
		DoShowFlee(nIndex);
		goto Exit0;
	}
	// �������Ǻ�������ƫ�Ƶ�X��Y��ֵ
	nRetCode = GetNpcMoveOffset(aNpc.m_Dir, nDistance, &nOffX, &nOffY);
	// ��ȡĿ������
	nDesX = nCurX + nOffX;
	nDesY = nCurY + nOffY;
	aNpc.SendCommand(do_walk, nDesX, nDesY);

	nResult = true;
Exit0:
	return nResult;
}

// ������
int KNpcAI::ShowNpcType16()
{
	int nResult  = false;
	int nRetCode = false;

	// Go the distance between P1 to P2
	register int nDistance = 0;
	int nDesX = 0;
	int nDesY = 0;
	int nCurX = 0;
	int nCurY = 0;
	int nOffX = 0;
	int nOffY = 0;
	int nOffsetDir = 0;
	int nIndex = 0;
	KNpc& aNpc = Npc[m_nIndex];

	aNpc.GetMpsPos(&nCurX, &nCurY);

	// �����½Ƕ� �� ����
	if (aNpc.m_AiParam[3] > 0)
		nOffsetDir = GetRandomNumber(aNpc.m_AiParam[3], aNpc.m_AiParam[2]);
	else
		nOffsetDir = aNpc.m_AiParam[2];
	if (GetRandomNumber(0, 1))
		nOffsetDir = -nOffsetDir;
	nDistance = GetRandomNumber(aNpc.m_AiParam[0] - aNpc.m_AiParam[1], aNpc.m_AiParam[0]);
	// ���������
	nIndex = IsPlayerCome();
	if (nIndex > 0)
	{
		// do flee
		nRetCode = DoShowFlee(nIndex);
		if (!nRetCode)
			goto Exit0;		
		goto Exit1;
	}

	// �������
	if (aNpc.m_CurrentWalkSpeed > 0)
	{
		aNpc.m_AiParam[4] = (int)(nDistance / aNpc.m_CurrentWalkSpeed);		//
		aNpc.m_AiParam[5] = 0;
	}
	else
	{
		aNpc.m_AiParam[4] = 0;
		aNpc.m_AiParam[5] = 0;
	}

	// �����½Ƕ�
	//if (KeepActiveShowRange())
	//	aNpc.m_Dir += 32;
	aNpc.m_Dir += GetRandomNumber(0, 6);
	aNpc.m_Dir %= 64;
	// �������Ǻ�������ƫ�Ƶ�X��Y��ֵ
	nRetCode = GetNpcMoveOffset(aNpc.m_Dir, nDistance, &nOffX, &nOffY);
	if (!nRetCode)
		goto Exit0;
	// ��ȡĿ������
	nDesX = nCurX + nOffX;
	nDesY = nCurY + nOffY;
	aNpc.SendCommand(do_walk, nDesX, nDesY);

Exit1:	
	nResult = true;
Exit0:
	return nResult;
}

// ������
int KNpcAI::ShowNpcType17()
{
	int nResult  = false;
	int nRetCode = false;

	// Go the distance between P1 to P2
	int nDistance = 0;
	int nDesX = 0;
	int nDesY = 0;
	int nCurX = 0;
	int nCurY = 0;
	int nOffX = 0;
	int nOffY = 0;
	int nOffsetDir = 0;
	KNpc& aNpc = Npc[m_nIndex];

	// Ч����ǿ ��������߶�
	aNpc.m_Height = GetRandomNumber(aNpc.m_AiParam[6] - 4, aNpc.m_AiParam[6]);

	aNpc.GetMpsPos(&nCurX, &nCurY);
		
	// �����½Ƕ� �� ����
	if (aNpc.m_AiParam[3] > 0)
		nOffsetDir = GetRandomNumber(aNpc.m_AiParam[3], aNpc.m_AiParam[2]);
	else
		nOffsetDir = aNpc.m_AiParam[2];
	if (GetRandomNumber(0, 1))
		nOffsetDir = -nOffsetDir;
	nDistance = GetRandomNumber(aNpc.m_AiParam[0] - aNpc.m_AiParam[1], aNpc.m_AiParam[0]);	

	// ȡ���˶������ʱ�䣬�����ڲ�������
	if (aNpc.m_CurrentWalkSpeed > 0)
	{
		aNpc.m_AiParam[4] = (int)(nDistance / aNpc.m_CurrentWalkSpeed);		//
		aNpc.m_AiParam[5] = 0;
	}
	else
	{
		aNpc.m_AiParam[4] = 0;
		aNpc.m_AiParam[5] = 0;
	}
	if (KeepActiveRange())
	{
		//aNpc.SendCommand(do_walk, aNpc.m_OriginX, aNpc.m_OriginY);
		goto Exit0;
		//aNpc.m_Dir += 32;
	}
	aNpc.m_Dir += nOffsetDir;
	//aNpc.m_Dir += GetRandomNumber(32, 64);
	if (aNpc.m_Dir < 0)
		aNpc.m_Dir += 64;
	else
		aNpc.m_Dir %= 64;
	// �������Ǻ�������ƫ�Ƶ�X��Y��ֵ
	nRetCode = GetNpcMoveOffset(aNpc.m_Dir, nDistance, &nOffX, &nOffY);
	if (!nRetCode)
		goto Exit0;
	// ��ȡĿ������
	nDesX = nCurX + nOffX;
	nDesY = nCurY + nOffY;
	aNpc.SendCommand(do_walk, nDesX, nDesY);
	
	nResult = true;
Exit0:
	return nResult;
}
#endif

// --
//
// --
void KNpcAI::ProcessPlayer()
{
#ifdef _SERVER
	TriggerObjectTrap();
	TriggerMapTrap();
#else
	int i = Npc[m_nIndex].m_nPeopleIdx;
	if (i > 0)
	{
		FollowPeople(i);
	}
	i = Npc[m_nIndex].m_nObjectIdx;
	if (i > 0)
	{
		FollowObject(i);
	}
#endif
}

#ifndef _SERVER
// --
//
// --
void KNpcAI::FollowObject(int nIdx)
{
	int nX1, nY1, nX2, nY2;
	Npc[m_nIndex].GetMpsPos(&nX1, &nY1);
	Object[nIdx].GetMpsPos(&nX2, &nY2);

	if ((nX1 - nX2) * (nX1 - nX2) + (nY1 - nY2) * (nY1 - nY2) < PLAYER_PICKUP_CLIENT_DISTANCE * PLAYER_PICKUP_CLIENT_DISTANCE)
	{
//#ifndef _SERVER
		Player[CLIENT_PLAYER_INDEX].CheckObject(nIdx);
//#endif
	}
}
#endif

#ifndef _SERVER
// --
//
// --
void KNpcAI::FollowPeople(int nIdx)
{	
	
	if ( Npc[nIdx].m_HideState.nTime > 0)
	{
		Npc[m_nIndex].m_nPeopleIdx = 0;
		return;
	}
	
	if (!Npc[nIdx].IsAlive())
	{
		Npc[m_nIndex].m_nPeopleIdx = 0;
		return;
	}

	int distance = NpcSet.GetDistance(nIdx, m_nIndex);
	int	nRelation = NpcSet.GetRelation(m_nIndex, nIdx);

	if ((Npc[nIdx].m_Kind == kind_dialoger))
	{
		if (distance <= Npc[nIdx].m_DialogRadius)
		{
			int x, y;
			SubWorld[Npc[m_nIndex].m_SubWorldIndex].Map2Mps(Npc[m_nIndex].m_RegionIndex, Npc[m_nIndex].m_MapX, Npc[m_nIndex].m_MapY, Npc[m_nIndex].m_OffX, Npc[m_nIndex].m_OffY, &x, &y);
			Npc[m_nIndex].SendCommand(do_walk, x,y);
			
			SendClientCmdWalk(x, y);
			Player[CLIENT_PLAYER_INDEX].DialogNpc(nIdx);
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;
			Npc[nIdx].TurnTo(Player[CLIENT_PLAYER_INDEX].m_nIndex);
			return;
		}
	}

	if (nRelation == relation_enemy)
	{
		Npc[m_nIndex].SendClientPos2Server();
		if (distance <= Npc[m_nIndex].m_CurrentAttackRadius)
		{
			Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, -1, nIdx);
			Npc[m_nIndex].SendClientPos2Server();	
			// Send to Server
			SendClientCmdSkill(Npc[m_nIndex].m_ActiveSkillID, -1, Npc[nIdx].m_dwID);
		}
		// ��׷
		else
		{
			int nDesX, nDesY;
			Npc[nIdx].GetMpsPos(&nDesX, &nDesY);

			Npc[nIdx].SendClientPos2Server();
			// modify by spe 2003/06/13
			if (Player[CLIENT_PLAYER_INDEX].m_RunStatus)
			{
				Npc[m_nIndex].SendClientPos2Server();
				Npc[m_nIndex].SendCommand(do_run, nDesX, nDesY);			
				SendClientCmdRun(nDesX, nDesY);
			}
			else
			{
				Npc[m_nIndex].SendClientPos2Server();
				Npc[m_nIndex].SendCommand(do_walk, nDesX, nDesY);
				SendClientCmdWalk(nDesX, nDesY);
			}
		}
		return;
	}
	// ����
	if (Npc[nIdx].m_Kind == kind_player)
	{
		// flow
		int nDesX, nDesY;
		Npc[nIdx].GetMpsPos(&nDesX, &nDesY);
		Npc[nIdx].SendClientPos2Server();
		// walk
		if (!Player[CLIENT_PLAYER_INDEX].m_RunStatus){
			if (distance >= FOLLOW_WALK_DISTANCE){
				Npc[m_nIndex].SendClientPos2Server();
				Npc[m_nIndex].SendCommand(do_walk, nDesX, nDesY);
				SendClientCmdWalk(nDesX, nDesY);
			}
		}
		// run
		else {
			if (distance >= MAX_FOLLOW_DISTANCE){
				Npc[m_nIndex].SendClientPos2Server();
				Npc[m_nIndex].SendCommand(do_run, nDesX, nDesY);			
				SendClientCmdRun(nDesX, nDesY);
			}
		}
	}
	return;
}
#endif

void KNpcAI::TriggerMapTrap()
{
	Npc[m_nIndex].CheckTrap();
}

void KNpcAI::TriggerObjectTrap()
{
	return;
}

int KNpcAI::GetNearestNpc(int nRelation)
{
	int nRangeX = Npc[m_nIndex].m_VisionRadius;
	int	nRangeY = nRangeX;
	int	nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
	int	nRegion = Npc[m_nIndex].m_RegionIndex;
	int	nMapX = Npc[m_nIndex].m_MapX;
	int	nMapY = Npc[m_nIndex].m_MapY;
	int	nRet;
	int	nRMx, nRMy, nSearchRegion;

	nRangeX = nRangeX / SubWorld[nSubWorld].m_nCellWidth;
	nRangeY = nRangeY / SubWorld[nSubWorld].m_nCellHeight;	

	for (int i = 0; i < nRangeX; i++)
	{
		for (int j = 0; j < nRangeY; j++)
		{
			if ((i * i + j * j) > nRangeX * nRangeX)
				continue;

			nRMx = nMapX + i;
			nRMy = nMapY + j;
			nSearchRegion = nRegion;
			if (nRMx < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[2];
				nRMx += SubWorld[nSubWorld].m_nRegionWidth;
			}
			else if (nRMx >= SubWorld[nSubWorld].m_nRegionWidth)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[6];
				nRMx -= SubWorld[nSubWorld].m_nRegionWidth;
			}
			if (nSearchRegion == -1)
				continue;
			if (nRMy < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[4];
				nRMy += SubWorld[nSubWorld].m_nRegionHeight;
			}
			else if (nRMy >= SubWorld[nSubWorld].m_nRegionHeight)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[0];
				nRMy -= SubWorld[nSubWorld].m_nRegionHeight;
			}
			if (nSearchRegion == -1)
				continue;
			
			nRet = SubWorld[nSubWorld].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, m_nIndex, nRelation);
			
			if (Npc[nRet].m_HideState.nTime > 0)
				nRet = 0;
			
			if (nRet > 0)
				return nRet;
			
			nRMx = nMapX - i;
			nRMy = nMapY + j;
			nSearchRegion = nRegion;
			if (nRMx < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[2];
				nRMx += SubWorld[nSubWorld].m_nRegionWidth;
			}
			else if (nRMx >= SubWorld[nSubWorld].m_nRegionWidth)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[6];
				nRMx -= SubWorld[nSubWorld].m_nRegionWidth;
			}
			if (nSearchRegion == -1)
				continue;
			if (nRMy < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[4];
				nRMy += SubWorld[nSubWorld].m_nRegionHeight;
			}
			else if (nRMy >= SubWorld[nSubWorld].m_nRegionHeight)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[0];
				nRMy -= SubWorld[nSubWorld].m_nRegionHeight;
			}
			if (nSearchRegion == -1)
				continue;
			
			nRet = SubWorld[nSubWorld].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, m_nIndex, nRelation);
			
			if (Npc[nRet].m_HideState.nTime > 0)
				nRet = 0;
			
			if (nRet > 0)
				return nRet;

			nRMx = nMapX - i;
			nRMy = nMapY - j;
			nSearchRegion = nRegion;
			if (nRMx < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[2];
				nRMx += SubWorld[nSubWorld].m_nRegionWidth;
			}
			else if (nRMx >= SubWorld[nSubWorld].m_nRegionWidth)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[6];
				nRMx -= SubWorld[nSubWorld].m_nRegionWidth;
			}
			if (nSearchRegion == -1)
				continue;
			if (nRMy < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[4];
				nRMy += SubWorld[nSubWorld].m_nRegionHeight;
			}
			else if (nRMy >= SubWorld[nSubWorld].m_nRegionHeight)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[0];
				nRMy -= SubWorld[nSubWorld].m_nRegionHeight;
			}
			if (nSearchRegion == -1)
				continue;
			
			nRet = SubWorld[nSubWorld].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, m_nIndex, nRelation);
			
			if (Npc[nRet].m_HideState.nTime > 0)
				nRet = 0;
			
			if (nRet > 0)
				return nRet;

			nRMx = nMapX + i;
			nRMy = nMapY - j;
			nSearchRegion = nRegion;			
			if (nRMx < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[2];
				nRMx += SubWorld[nSubWorld].m_nRegionWidth;
			}
			else if (nRMx >= SubWorld[nSubWorld].m_nRegionWidth)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[6];
				nRMx -= SubWorld[nSubWorld].m_nRegionWidth;
			}
			if (nSearchRegion == -1)
				continue;
			if (nRMy < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[4];
				nRMy += SubWorld[nSubWorld].m_nRegionHeight;
			}
			else if (nRMy >= SubWorld[nSubWorld].m_nRegionHeight)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[0];
				nRMy -= SubWorld[nSubWorld].m_nRegionHeight;
			}
			if (nSearchRegion == -1)
				continue;
			nRet = SubWorld[nSubWorld].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, m_nIndex, nRelation);
			
			if (Npc[nRet].m_HideState.nTime > 0)
				nRet = 0;
			
			if (nRet > 0)
				return nRet;
		}
	}
	return 0;
}

#ifndef _SERVER
// flying add this
// ������ĳ��NPC��������
int KNpcAI::IsPlayerCome()
{
	int nResult = 0;
	int nPlayer = 0;
	int X1 = 0;
	int Y1 = 0;
	int X2 = 0;
	int Y2 = 0;
	int nDistance = 0;

	nPlayer = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	nDistance = NpcSet.GetDistance(nPlayer, m_nIndex);
	// �����ĵ������
	if (nDistance < Npc[m_nIndex].m_VisionRadius)
	{
		// �ֱ����ߺ���
		if (Player[CLIENT_PLAYER_INDEX].m_RunStatus ||
			Npc[m_nIndex].m_CurrentVisionRadius > nDistance * 4)
		{
			nResult = nPlayer;
		}
	}
	return nResult;
}
#endif

int KNpcAI::GetNpcNumber(int nRelation)
{
	int nRangeX = Npc[m_nIndex].m_VisionRadius;
	int	nRangeY = nRangeX;
	int	nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
	int	nRegion = Npc[m_nIndex].m_RegionIndex;
	int	nMapX = Npc[m_nIndex].m_MapX;
	int	nMapY = Npc[m_nIndex].m_MapY;
	int	nRet = 0;
	int	nRMx, nRMy, nSearchRegion;

	nRangeX = nRangeX / SubWorld[nSubWorld].m_nCellWidth;
	nRangeY = nRangeY / SubWorld[nSubWorld].m_nCellHeight;

	// �����Ұ��Χ�ڵĸ������NPC
	for (int i = -nRangeX; i < nRangeX; i++)
	{
		for (int j = -nRangeY; j < nRangeY; j++)
		{
			// ȥ���߽Ǽ������ӣ���֤��Ұ����Բ��
			if ((i * i + j * j) > nRangeX * nRangeX)
				continue;

			// ȷ��Ŀ�����ʵ�ʵ�REGION������ȷ��
			nRMx = nMapX + i;
			nRMy = nMapY + j;
			nSearchRegion = nRegion;
			if (nRMx < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[2];
				nRMx += SubWorld[nSubWorld].m_nRegionWidth;
			}
			else if (nRMx >= SubWorld[nSubWorld].m_nRegionWidth)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[6];
				nRMx -= SubWorld[nSubWorld].m_nRegionWidth;
			}
			if (nSearchRegion == -1)
				continue;
			if (nRMy < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[4];
				nRMy += SubWorld[nSubWorld].m_nRegionHeight;
			}
			else if (nRMy >= SubWorld[nSubWorld].m_nRegionHeight)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[0];
				nRMy -= SubWorld[nSubWorld].m_nRegionHeight;
			}
			if (nSearchRegion == -1)
				continue;
			// ��REGION��NPC�б��в�������������NPC			
			int nNpcIdx = SubWorld[nSubWorld].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, m_nIndex, nRelation);
			if (nNpcIdx > 0)
				nRet++;
		}
	}
	return nRet;
}

void KNpcAI::KeepAttackRange(int nEnemy, int nRange)
{
	int nX1, nY1, nX2, nY2, nDir, nWantX, nWantY;

	Npc[m_nIndex].GetMpsPos(&nX1, &nY1);
	Npc[nEnemy].GetMpsPos(&nX2, &nY2);
	nDir = g_GetDirIndex(nX1, nY1, nX2, nY2);

	nWantX = nX2 - ((nRange * g_DirCos(nDir, 64)) >> 10);
	nWantY = nY2 - ((nRange * g_DirSin(nDir, 64)) >> 10);

	Npc[m_nIndex].SendCommand(do_walk, nWantX, nWantY);
}

void KNpcAI::FollowAttack(int i)
{	
	if ( Npc[i].m_RegionIndex < 0 )
		return;
	
	if ( Npc[i].m_HideState.nTime > 0)
		return;
	
	int distance = NpcSet.GetDistance(m_nIndex, i);

#define	MINI_ATTACK_RANGE	32

	if (distance <= MINI_ATTACK_RANGE)
	{
		KeepAttackRange(i, MINI_ATTACK_RANGE);
		return;
	}
	// Attack Enemy
	if (distance <= Npc[m_nIndex].m_CurrentAttackRadius && InEyeshot(i))
	{
		Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, -1, i);
		return;
	}

	// Move to Enemy
	int x, y;
	Npc[i].GetMpsPos(&x, &y);

	Npc[m_nIndex].SendCommand(do_walk, x, y);
}

BOOL KNpcAI::InEyeshot(int nIdx)
{
	int distance = NpcSet.GetDistance(nIdx, m_nIndex);

	return (Npc[m_nIndex].m_VisionRadius > distance);
}

void KNpcAI::CommonAction()
{
	if (Npc[m_nIndex].m_Kind == kind_dialoger)
	{
		if (Npc[m_nIndex].m_Doing != do_stand)
			Npc[m_nIndex].SendCommand(do_stand);
		return;
	}
	int	nOffX, nOffY;
	if (g_RandPercent(80))
	{
		nOffX = 0;
		nOffY = 0;
	}
	else
	{
		
		nOffX = g_Random(Npc[m_nIndex].m_CurrentActiveRadius / 2);
		nOffY = g_Random(Npc[m_nIndex].m_CurrentActiveRadius / 2);
		if (nOffX & 1)
		{
			nOffX = - nOffX;
		}
		if (nOffY & 1)
		{
			nOffY = - nOffY;
		}
	}
	Npc[m_nIndex].SendCommand(do_walk, Npc[m_nIndex].m_OriginX + nOffX, Npc[m_nIndex].m_OriginY + nOffY);
}

BOOL KNpcAI::KeepActiveRange()
{
	int x, y;
	
	Npc[m_nIndex].GetMpsPos(&x, &y);
	int	nRange = g_GetDistance(Npc[m_nIndex].m_OriginX, Npc[m_nIndex].m_OriginY, x, y);

	// ���ֳ������Χ���ѵ�ǰ���Χ��С�������ڻ��Χ��Ե���ػΡ�
	if (Npc[m_nIndex].m_ActiveRadius < nRange)
	{
		Npc[m_nIndex].m_CurrentActiveRadius = Npc[m_nIndex].m_ActiveRadius / 2;
	}

	// ���ֳ�����ǰ���Χ��������
	if (Npc[m_nIndex].m_CurrentActiveRadius < nRange)
	{
		Npc[m_nIndex].SendCommand(do_walk, Npc[m_nIndex].m_OriginX, Npc[m_nIndex].m_OriginY);
		return TRUE;
	}
	else	// �ڵ�ǰ���Χ�ڣ��ָ���ǰ���Χ��С��
	{
		Npc[m_nIndex].m_CurrentActiveRadius = Npc[m_nIndex].m_ActiveRadius;
		return FALSE;
	}
}

#ifndef _SERVER
// 15/16 AiMode NPC�����ݶ���
int KNpcAI::DoShowFlee(int nIdx)
{
	int nResult  = false;
	int nRetCode = false;
	
	int x1, y1, x2, y2;
	int nDistance = Npc[m_nIndex].m_AiParam[6];

	Npc[m_nIndex].GetMpsPos(&x1, &y1);
	//Npc[nIdx].GetMpsPos(&x2, &y2);
	Npc[m_nIndex].m_Dir = Npc[nIdx].m_Dir;
	nRetCode = GetNpcMoveOffset(Npc[m_nIndex].m_Dir, nDistance, &x2, &y2);
	if (!nRetCode)
		goto Exit0;
	Npc[m_nIndex].m_AiParam[4] = (int) nDistance / Npc[m_nIndex].m_WalkSpeed;
	Npc[m_nIndex].m_AiParam[5] = 0;
	Npc[m_nIndex].SendCommand(do_walk, x1 + x2, y1 + y2);

	nResult = true;
Exit0:
	return nResult;
}

#endif

// ����Npc[nIdx]
void KNpcAI::Flee(int nIdx)
{
	int x1, y1, x2, y2;

	Npc[m_nIndex].GetMpsPos(&x1, &y1);
	Npc[nIdx].GetMpsPos(&x2, &y2);

	x1 = x1 * 2 - x2;
	y1 = y1 * 2 - y2;

	Npc[m_nIndex].SendCommand(do_walk, x1, y1);
}

void	KNpcAI::ProcessAIType01()
{
	int *pAIParam = Npc[m_nIndex].m_AiParam;
	// �Ƿ��ѳ�����뾶
	if (KeepActiveRange())
		return;

	int nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;
	// ���ԭ��û���������˻������������̫Զ��������������
	if (nEnemyIdx <= 0 || Npc[nEnemyIdx].m_dwID <= 0 || !InEyeshot(nEnemyIdx) )
	{
		nEnemyIdx = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = nEnemyIdx;
	}

	// ��Χû�е��ˣ�һ�����ʴ���/Ѳ��
	if (nEnemyIdx <= 0)
	{
		// pAIParam[0]:Ѳ�߸���
		if (pAIParam[0] > 0 && g_RandPercent(pAIParam[0]))
		{	// Ѳ��
			CommonAction();
		}
		return;
	}

	// ������������м��ܹ�����Χ֮�⣬һ������ѡ�����/Ѳ��/����˿���
	if (KNpcSet::GetDistanceSquare(m_nIndex, nEnemyIdx) > pAIParam[MAX_AI_PARAM - 1])
	{
		int		nRand;
		nRand = g_Random(100);
		if (nRand < pAIParam[5])	// ����
			return;
		if (nRand < pAIParam[5] + pAIParam[6])	// Ѳ��
		{
			CommonAction();
			return;
		}
		FollowAttack(nEnemyIdx);	// ����˿���
		return;
	}

	// ����������ܹ�����Χ֮�ڣ�ѡ��һ�ּ��ܹ���
	int		nRand;
	nRand = g_Random(100);
	if (nRand < pAIParam[1])
	{
		if (!Npc[m_nIndex].SetActiveSkill(1))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[1] + pAIParam[2])
	{
		if (!Npc[m_nIndex].SetActiveSkill(2))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[1] + pAIParam[2] + pAIParam[3])
	{
		if (!Npc[m_nIndex].SetActiveSkill(3))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[1] + pAIParam[2] + pAIParam[3] + pAIParam[4])
	{
		if (!Npc[m_nIndex].SetActiveSkill(4))
		{
			CommonAction();
			return;
		}
	}
	else	// ����
	{
		return;
	}

	FollowAttack(nEnemyIdx);
}

void	KNpcAI::ProcessAIType02()
{
	int *pAIParam = Npc[m_nIndex].m_AiParam;
	// �Ƿ��ѳ�����뾶
	if (KeepActiveRange())
		return;

	int nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;
	// ���ԭ��û���������˻������������̫Զ��������������
	if (nEnemyIdx <= 0 || Npc[nEnemyIdx].m_dwID <= 0 || !InEyeshot(nEnemyIdx) )
	{
		nEnemyIdx = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = nEnemyIdx;
	}

	if (nEnemyIdx <= 0)
	{
		if (pAIParam[0] > 0 && g_RandPercent(pAIParam[0]))
		{
			CommonAction();
		}
		return;
	}

	if ((int)(Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax) < pAIParam[1])
	{
		if (g_RandPercent(pAIParam[2]))	
		{
			if (Npc[m_nIndex].m_AiAddLifeTime < pAIParam[9] && g_RandPercent(pAIParam[3]))
			{
				Npc[m_nIndex].SetActiveSkill(1);
				Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, -1, m_nIndex);
				Npc[m_nIndex].m_AiAddLifeTime++;
				return;
			}
			else	// ����
			{
				Flee(nEnemyIdx);
				return;
			}
		}
	}

	// ������������м��ܹ�����Χ֮�⣬һ������ѡ�����/Ѳ��/����˿���
	if (KNpcSet::GetDistanceSquare(m_nIndex, nEnemyIdx) > pAIParam[MAX_AI_PARAM - 1])
	{
		int		nRand;
		nRand = g_Random(100);
		if (nRand < pAIParam[7])	// ����
			return;
		if (nRand < pAIParam[7] + pAIParam[8])	// Ѳ��
		{
			CommonAction();
			return;
		}
		FollowAttack(nEnemyIdx);	// ����˿���
		return;
	}

	// ����������ܹ�����Χ֮�ڣ�ѡ��һ�ּ��ܹ���
	int		nRand;
	nRand = g_Random(100);
	if (nRand < pAIParam[4])
	{
		if (!Npc[m_nIndex].SetActiveSkill(2))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[4] + pAIParam[5])
	{
		if (!Npc[m_nIndex].SetActiveSkill(3))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[4] + pAIParam[5] + pAIParam[6])
	{
		if (!Npc[m_nIndex].SetActiveSkill(4))
		{
			CommonAction();
			return;
		}
	}
	else	// ����
	{
		return;
	}
	FollowAttack(nEnemyIdx);
}

void	KNpcAI::ProcessAIType03()
{
	int *pAIParam = Npc[m_nIndex].m_AiParam;
	// �Ƿ��ѳ�����뾶
	if (KeepActiveRange())
		return;

	int nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;
	// ���ԭ��û���������˻������������̫Զ��������������
	if (nEnemyIdx <= 0 || Npc[nEnemyIdx].m_dwID <= 0 || !InEyeshot(nEnemyIdx) )
	{
		nEnemyIdx = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = nEnemyIdx;
	}

	if (nEnemyIdx <= 0)
	{
		if (pAIParam[0] > 0 && g_RandPercent(pAIParam[0]))
		{
			CommonAction();
		}
		return;
	}

	if ((int)(Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax) < pAIParam[1])
	{
		if (g_RandPercent(pAIParam[2]))	
		{
			if (g_RandPercent(pAIParam[3]))
			{
				Npc[m_nIndex].SetActiveSkill(1);
				FollowAttack(nEnemyIdx);
				return;
			}
			else
			{
				Flee(nEnemyIdx);
				return;
			}
		}
	}

	// ������������м��ܹ�����Χ֮�⣬һ������ѡ�����/Ѳ��/����˿���
	if (KNpcSet::GetDistanceSquare(m_nIndex, nEnemyIdx) > pAIParam[MAX_AI_PARAM - 1])
	{
		int		nRand;
		nRand = g_Random(100);
		if (nRand < pAIParam[7])	// ����
			return;
		if (nRand < pAIParam[7] + pAIParam[8])	// Ѳ��
		{
			CommonAction();
			return;
		}
		FollowAttack(nEnemyIdx);	// ����˿���
		return;
	}

	// ����������ܹ�����Χ֮�ڣ�ѡ��һ�ּ��ܹ���
	int		nRand;
	nRand = g_Random(100);
	if (nRand < pAIParam[4])
	{
		if (!Npc[m_nIndex].SetActiveSkill(2))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[4] + pAIParam[5])
	{
		if (!Npc[m_nIndex].SetActiveSkill(3))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[4] + pAIParam[5] + pAIParam[6])
	{
		if (!Npc[m_nIndex].SetActiveSkill(4))
		{
			CommonAction();
			return;
		}
	}
	else	// ����
	{
		return;
	}
	FollowAttack(nEnemyIdx);
}

void	KNpcAI::ProcessAIType04()
{
	int *pAIParam = Npc[m_nIndex].m_AiParam;

	int nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;
	// �Ƿ��ܵ���������һ������ѡ�����/Ѳ��
	if (nEnemyIdx <= 0)
	{
		// pAIParam[0]:Ѳ�߸���
		if (pAIParam[0] > 0 && g_RandPercent(pAIParam[0]))
		{	// Ѳ��
			CommonAction();
		}
		return;
	}

	// �Ƿ��ѳ�����뾶
	if (KeepActiveRange())
		return;

	// ������������м��ܹ�����Χ֮�⣬һ������ѡ�����/Ѳ��/����˿���
	if (KNpcSet::GetDistanceSquare(m_nIndex, nEnemyIdx) > pAIParam[MAX_AI_PARAM - 1])
	{
		int		nRand;
		nRand = g_Random(100);
		if (nRand < pAIParam[5])	// ����
			return;
		if (nRand < pAIParam[5] + pAIParam[6])	// Ѳ��
		{
			CommonAction();
			return;
		}
		FollowAttack(nEnemyIdx);	// ����˿���
		return;
	}

	// ����������ܹ�����Χ֮�ڣ�ѡ��һ�ּ��ܹ���
	int		nRand;
	nRand = g_Random(100);
	if (nRand < pAIParam[1])
	{
		if (!Npc[m_nIndex].SetActiveSkill(1))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[1] + pAIParam[2])
	{
		if (!Npc[m_nIndex].SetActiveSkill(2))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[1] + pAIParam[2] + pAIParam[3])
	{
		if (!Npc[m_nIndex].SetActiveSkill(3))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[1] + pAIParam[2] + pAIParam[3] + pAIParam[4])
	{
		if (!Npc[m_nIndex].SetActiveSkill(4))
		{
			CommonAction();
			return;
		}
	}
	else	// ����
	{
		return;
	}
	FollowAttack(nEnemyIdx);
}

void	KNpcAI::ProcessAIType05()
{
	int *pAIParam = Npc[m_nIndex].m_AiParam;

	int nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;
	// �Ƿ��ܵ���������һ������ѡ�����/Ѳ��
	if (nEnemyIdx <= 0)
	{
		// pAIParam[0]:Ѳ�߸���
		if (pAIParam[0] > 0 && g_RandPercent(pAIParam[0]))
		{	// Ѳ��
			CommonAction();
		}
		return;
	}

	if (KeepActiveRange())
		return;

	if ((int)(Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax) < pAIParam[1])
	{
		if (g_RandPercent(pAIParam[2]))
		{
			if (Npc[m_nIndex].m_AiAddLifeTime < pAIParam[9] && g_RandPercent(pAIParam[3]))	// ʹ�ò�Ѫ����
			{
				Npc[m_nIndex].m_AiAddLifeTime++;
				Npc[m_nIndex].SetActiveSkill(1);
				Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, -1, m_nIndex);
				return;
			}
			else	// ����
			{
				Flee(nEnemyIdx);
				return;
			}
		}
	}

	// ������������м��ܹ�����Χ֮�⣬һ������ѡ�����/Ѳ��/����˿���
	if (KNpcSet::GetDistanceSquare(m_nIndex, nEnemyIdx) > pAIParam[MAX_AI_PARAM - 1])
	{
		int		nRand;
		nRand = g_Random(100);
		if (nRand < pAIParam[7])	// ����
			return;
		if (nRand < pAIParam[7] + pAIParam[8])	// Ѳ��
		{
			CommonAction();
			return;
		}
		FollowAttack(nEnemyIdx);	// ����˿���
		return;
	}

	// ����������ܹ�����Χ֮�ڣ�ѡ��һ�ּ��ܹ���
	int		nRand;
	nRand = g_Random(100);
	if (nRand < pAIParam[4])
	{
		if (!Npc[m_nIndex].SetActiveSkill(2))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[4] + pAIParam[5])
	{
		if (!Npc[m_nIndex].SetActiveSkill(3))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[4] + pAIParam[5] + pAIParam[6])
	{
		if (!Npc[m_nIndex].SetActiveSkill(4))
		{
			CommonAction();
			return;
		}
	}
	else	// ����
	{
		return;
	}
	FollowAttack(nEnemyIdx);
}

void	KNpcAI::ProcessAIType06()
{
	int *pAIParam = Npc[m_nIndex].m_AiParam;

	int nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;

	if (nEnemyIdx <= 0)
	{
		if (pAIParam[0] > 0 && g_RandPercent(pAIParam[0]))
		{
			CommonAction();
		}
		return;
	}

	if (KeepActiveRange())
		return;

	if ((int)(Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax) < pAIParam[1])
	{
		if (g_RandPercent(pAIParam[2]))
		{
			if (g_RandPercent(pAIParam[3]))
			{
				Npc[m_nIndex].SetActiveSkill(1);
				FollowAttack(nEnemyIdx);
				return;
			}
			else
			{
				Flee(nEnemyIdx);
				return;
			}
		}
	}

	if (KNpcSet::GetDistanceSquare(m_nIndex, nEnemyIdx) > pAIParam[MAX_AI_PARAM - 1])
	{
		int		nRand;
		nRand = g_Random(100);
		if (nRand < pAIParam[7])	// ����
			return;
		if (nRand < pAIParam[7] + pAIParam[8])	// Ѳ��
		{
			CommonAction();
			return;
		}
		FollowAttack(nEnemyIdx);	// ����˿���
		return;
	}

	// ����������ܹ�����Χ֮�ڣ�ѡ��һ�ּ��ܹ���
	int		nRand;
	nRand = g_Random(100);
	if (nRand < pAIParam[4])
	{
		if (!Npc[m_nIndex].SetActiveSkill(2))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[4] + pAIParam[5])
	{
		if (!Npc[m_nIndex].SetActiveSkill(3))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[4] + pAIParam[5] + pAIParam[6])
	{
		if (!Npc[m_nIndex].SetActiveSkill(4))
		{
			CommonAction();
			return;
		}
	}
	else	// ����
	{
		return;
	}
	FollowAttack(nEnemyIdx);
}

//------------------------------------------------------------------------------
//	
//	
//	
//	
//------------------------------------------------------------------------------
void	KNpcAI::ProcessAIType07()
{
	int *pAIParam = Npc[m_nIndex].m_AiParam;

	int nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;
	if (nEnemyIdx < 0 || Npc[nEnemyIdx].m_dwID <= 0)	//Khong co muc tieu
	{
		nEnemyIdx = GetNearestNpc(relation_enemy);	//tim muc tieu moi
		Npc[m_nIndex].m_nPeopleIdx = nEnemyIdx;
	}
	else
	{
		if (KNpcSet::GetDistanceSquare(m_nIndex, nEnemyIdx) > 250)
		{
			nEnemyIdx = GetNearestNpc(relation_enemy);	//tim muc tieu moi
			Npc[m_nIndex].m_nPeopleIdx = nEnemyIdx;
		}
	}

	if (nEnemyIdx < 0)
	{
		CommonAction();
		return;
	}

		// �Ƿ��ѳ�����뾶
	if (KeepActiveRange())
		return;

	int		nRand;
	nRand = g_Random(100);
	// if (KNpcSet::GetDistanceSquare(m_nIndex, nEnemyIdx) > 0)
	// {
		// FollowAttack(nEnemyIdx);
	// }
	if (nRand < 25)
	{
		Npc[m_nIndex].SetActiveSkill(1);
	}
	else if (nRand >= 25 && nRand < 50)
	{
		Npc[m_nIndex].SetActiveSkill(2);
	}
	else if (nRand >= 50 && nRand < 75)
	{
		Npc[m_nIndex].SetActiveSkill(3);
	}
	else if (nRand >= 75 && nRand <= 100)
	{
		Npc[m_nIndex].SetActiveSkill(4);
	}
}
//------------------------------------------------------------------------------
//
//	AI Type 08: Follow owner player (escort NPCs)
//
//------------------------------------------------------------------------------
void	KNpcAI::ProcessAIType08()
{
	// Check if NPC has an owner (stored in m_nParam[7] and m_nParam[8])
	// m_nParam[7] = owner player index (param 8 in Lua, 0-indexed in C++)
	// m_nParam[8] = follow mode (param 9 in Lua, 1=follow, 0=disabled)
	// NOTE: m_nParam is 0-indexed! SetNpcParam(nId, 8, value) -> m_nParam[7]
	int nOwnerPlayerIdx = Npc[m_nIndex].m_nParam[7];
	int nFollowMode = Npc[m_nIndex].m_nParam[8];

	// If no owner or follow disabled, just stand still
	if (nOwnerPlayerIdx < 0 || nOwnerPlayerIdx >= MAX_PLAYER || nFollowMode == 0)
		return;

	// Check if owner player is still valid
	if (Player[nOwnerPlayerIdx].m_nIndex <= 0)
		return;

	int nPlayerNpcIdx = Player[nOwnerPlayerIdx].m_nIndex;
	if (nPlayerNpcIdx <= 0 || nPlayerNpcIdx >= MAX_NPC)
		return;

	// Check if player is on same subworld
	if (Npc[m_nIndex].m_SubWorldIndex != Npc[nPlayerNpcIdx].m_SubWorldIndex)
		return;

	// Calculate distance to owner
	int nDistanceSquare = KNpcSet::GetDistanceSquare(m_nIndex, nPlayerNpcIdx);

	// Follow behavior:
	// - If distance > 5 tiles (160 pixels), walk to player
	// - If distance 2-5 tiles, keep following at normal pace
	// - Only stop if distance < 1.5 tiles (48 pixels) to avoid collision

	if (nDistanceSquare > 25600)  // 160*160 = 25600 (> 5 tiles)
	{
		// Far away - walk towards player
		Npc[m_nIndex].SendCommand(do_walk, Npc[nPlayerNpcIdx].m_MapX, Npc[nPlayerNpcIdx].m_MapY);
	}
	else if (nDistanceSquare > 2304)  // 48*48 = 2304 (> 1.5 tiles)
	{
		// Medium distance - keep following
		Npc[m_nIndex].SendCommand(do_walk, Npc[nPlayerNpcIdx].m_MapX, Npc[nPlayerNpcIdx].m_MapY);
	}
	// else: very close (< 1.5 tiles) - do nothing, let NPC idle naturally
}

/*
// һ��������
void KNpcAI::ProcessAIType1()
{
	int *pAIParam = Npc[m_nIndex].m_AiParam;
	// �Ƿ��ѳ�����뾶
	if (KeepActiveRange())
		return;

	if (Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax < pAIParam[0])
	{
		if (g_RandPercent(pAIParam[1]))
		{
			Npc[m_nIndex].SetActiveSkill(1);
			Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, -1, m_nIndex);
			return;
		}
	}

	int nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;
	
	if (nEnemyIdx <= 0 || Npc[nEnemyIdx].m_dwID <= 0 || !InEyeshot(nEnemyIdx) )
	{
		nEnemyIdx = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = nEnemyIdx;
	}
	
	

	if (nEnemyIdx > 0)
	{
		int		nRand;
		nRand = g_Random(100);
		if (nRand < pAIParam[2])
		{
			if (!Npc[m_nIndex].SetActiveSkill(2))
			{
				CommonAction();
				return;
			}
		}
		else if (nRand < pAIParam[2] + pAIParam[3])
		{
			if (!Npc[m_nIndex].SetActiveSkill(3))
			{
				CommonAction();
				return;
			}
		}
		else if (nRand < pAIParam[2] + pAIParam[3] + pAIParam[4])
		{
			if (!Npc[m_nIndex].SetActiveSkill(4))
			{
				CommonAction();
				return;
			}
		}

//		if (g_RandPercent(pAIParam[2]))
//		{
//			Npc[m_nIndex].SetActiveSkill(2);
//		}
//		else if (g_RandPercent(pAIParam[3]))
//		{
//			Npc[m_nIndex].SetActiveSkill(3);
//		}
//		else if (g_RandPercent(pAIParam[4]))
//		{
//			Npc[m_nIndex].SetActiveSkill(4);
//		}
		else
		{
			CommonAction();
			return;
		}
		FollowAttack(nEnemyIdx);
		return;
	}
	CommonAction();
}
*/

/*
// һ�㱻����
void KNpcAI::ProcessAIType2()
{
	int *pAIParam = Npc[m_nIndex].m_AiParam;

	if (KeepActiveRange())
		return;

	if (Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax < pAIParam[0])
	{
		if (g_RandPercent(pAIParam[1]))
		{
			Npc[m_nIndex].SetActiveSkill(1);
			Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, -1, m_nIndex);
			return;
		}
	}

	int nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;
	if (nEnemyIdx <= 0 || !InEyeshot(nEnemyIdx))
		return;

	int		nRand;
	nRand = g_Random(100);

	if (nRand < pAIParam[2])
	{
		if (!Npc[m_nIndex].SetActiveSkill(2))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[2] + pAIParam[3])
	{
		if (!Npc[m_nIndex].SetActiveSkill(3))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[2] + pAIParam[3] + pAIParam[4])
	{
		if (!Npc[m_nIndex].SetActiveSkill(4))
		{
			CommonAction();
			return;
		}
	}

//	if (g_RandPercent(pAIParam[2]))
//	{
//		Npc[m_nIndex].SetActiveSkill(2);
//	}
//	else if (g_RandPercent(pAIParam[3]))
//	{
//		Npc[m_nIndex].SetActiveSkill(3);
//	}
//	else if (g_RandPercent(pAIParam[4]))
//	{
//		Npc[m_nIndex].SetActiveSkill(4);
//	}
	else
	{
		CommonAction();
		return;
	}
	FollowAttack(nEnemyIdx);

	return;
}
*/

/*
// һ��������
void KNpcAI::ProcessAIType3()
{
	int* pAIParam = Npc[m_nIndex].m_AiParam;

	if (KeepActiveRange())
		return;

	int	nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;

	if (nEnemyIdx <= 0 || !InEyeshot(nEnemyIdx))
	{
		nEnemyIdx = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = nEnemyIdx;
	}

	if (nEnemyIdx <= 0)
	{
		CommonAction();
		return;
	}
	
	if (Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax < pAIParam[0])
	{
		if (g_RandPercent(pAIParam[1]))
		{
			Flee(nEnemyIdx);
			return;
		}
	}

	int		nRand;
	nRand = g_Random(100);

	if (nRand < pAIParam[2])
	{
		if (!Npc[m_nIndex].SetActiveSkill(1))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[2] + pAIParam[3])
	{
		if (!Npc[m_nIndex].SetActiveSkill(2))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[2] + pAIParam[3] + pAIParam[4])
	{
		if (!Npc[m_nIndex].SetActiveSkill(3))
		{
			CommonAction();
			return;
		}
	}

//	if (g_RandPercent(pAIParam[2]))
///	{
//		Npc[m_nIndex].SetActiveSkill(1);
//	}
//	else if (g_RandPercent(pAIParam[3]))
//	{
//		Npc[m_nIndex].SetActiveSkill(2);
//	}
//	else if (g_RandPercent(pAIParam[4]))
//	{
//		Npc[m_nIndex].SetActiveSkill(3);
//	}
	else
	{
		CommonAction();
		return;
	}
	FollowAttack(nEnemyIdx);
	return;
}
*/

/*
// ���ܼ�ǿ��
void KNpcAI::ProcessAIType4()
{
	int*	pAIParam = Npc[m_nIndex].m_AiParam;
	
	if (KeepActiveRange())
		return;

	int	nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;

	if (nEnemyIdx <= 0 || !InEyeshot(nEnemyIdx))
	{
		nEnemyIdx = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = nEnemyIdx;
	}

	if (nEnemyIdx <= 0)
	{
		CommonAction();
		return;
	}
	
	int nLifePercent = Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax;
	if (nLifePercent < pAIParam[0])
	{
		if (g_RandPercent(pAIParam[1]))
		{
			Flee(nEnemyIdx);
			return;
		}
	}
	if (nLifePercent < pAIParam[2])
	{
		if (g_RandPercent(pAIParam[3]))
		{
			Npc[m_nIndex].SetActiveSkill(1);
			Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, -1, m_nIndex);
			return;
		}
	}

	if (g_RandPercent(pAIParam[4]))
	{
		Npc[m_nIndex].SetActiveSkill(2);
	}
	else if (g_RandPercent(pAIParam[5]))
	{
		Npc[m_nIndex].SetActiveSkill(3);
	}
	else
	{
		CommonAction();
		return;
	}
	FollowAttack(nEnemyIdx);
	return;
}
*/

/*
//	�˶������
void KNpcAI::ProcessAIType5()
{
	int *pAIParam = Npc[m_nIndex].m_AiParam;

	if (KeepActiveRange())
		return;

	int i = Npc[m_nIndex].m_nPeopleIdx;

	if (!i || !InEyeshot(i))
	{
		i = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = i;
	}

	if (!i)
	{
		CommonAction();
		return;
	}

	int nEnemyNumber = GetNpcNumber(relation_enemy);
	if (nEnemyNumber > pAIParam[0])
	{
		if (g_RandPercent(pAIParam[1]))
		{
			Flee(i);
			return;
		}
	}

	if (g_RandPercent(pAIParam[2]))
	{
		Npc[m_nIndex].SetActiveSkill(1);
	}
	else if (nEnemyNumber <= pAIParam[3] && g_RandPercent(pAIParam[4]))
	{
		Npc[m_nIndex].SetActiveSkill(2);
	}
	else
	{
		CommonAction();
		return;
	}

	FollowAttack(i);
	return;
}
*/

/*
//	��Ⱥ�����
void KNpcAI::ProcessAIType6()
{
	int *pAIParam = Npc[m_nIndex].m_AiParam;

	if (KeepActiveRange())
		return;

	int i = Npc[m_nIndex].m_nPeopleIdx;
	if (!i || !InEyeshot(i))
	{
		i = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = i;
	}

	if (!i)
	{
		CommonAction();
		return;
	}

	int nAllyNumber = GetNpcNumber(relation_none);
	if (nAllyNumber <= pAIParam[0])
	{
		if (g_RandPercent(pAIParam[1]))
		{
			Flee(i);
			return;
		}
	}
	
	if (g_RandPercent(pAIParam[2]))
	{
		Npc[m_nIndex].SetActiveSkill(1);
	}
	else if (nAllyNumber > pAIParam[3] && g_RandPercent(pAIParam[4]))
	{
		Npc[m_nIndex].SetActiveSkill(2);
	}
	else
	{
		CommonAction();
		return;
	}

	FollowAttack(i);
	return;
}
*/

/*
// ����۶���
void KNpcAI::ProcessAIType7()
{
	int *pAIParam = Npc[m_nIndex].m_AiParam;

	if (KeepActiveRange())
		return;

	int i = Npc[m_nIndex].m_nPeopleIdx;
	if (!i || !InEyeshot(i))
	{
		i = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = i;
	}

	if (!i)
	{
		CommonAction();
		return;
	}

	int j = GetNearestNpc(relation_ally);

	if (j && Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax < pAIParam[0])
	{
		if (g_RandPercent(pAIParam[1]))
		{
			int x, y;
			Npc[j].GetMpsPos(&x, &y);
			Npc[m_nIndex].SendCommand(do_walk, x, y);
			return;
		}
	}

	if (g_RandPercent(pAIParam[2]))
	{
		Npc[m_nIndex].SetActiveSkill(1);
	}
	else if (g_RandPercent(pAIParam[3]))
	{
		Npc[m_nIndex].SetActiveSkill(2);
	}
	else if (g_RandPercent(pAIParam[4]))
	{
		Npc[m_nIndex].SetActiveSkill(3);
	}
	else
	{
		CommonAction();
		return;
	}
	FollowAttack(i);
	return;
}
*/

/*
//	����������
void KNpcAI::ProcessAIType8()
{
	int *pAIParam = Npc[m_nIndex].m_AiParam;

	if (KeepActiveRange())
		return;

	int i = Npc[m_nIndex].m_nPeopleIdx;

	if (!i || !InEyeshot(i))
	{
		i = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = i;
	}

	if (!i)
	{
		CommonAction();
		return;
	}
	
	if (g_RandPercent(pAIParam[0]))
	{
		int x, y;

		Npc[i].GetMpsPos(&x, &y);
		Npc[m_nIndex].SendCommand(do_walk, x, y);
	}
	else if (g_RandPercent(pAIParam[1]))
	{
		Npc[m_nIndex].SetActiveSkill(1);
	}
	else if (g_RandPercent(pAIParam[2]))
	{
		Npc[m_nIndex].SetActiveSkill(2);
	}
	else if (g_RandPercent(pAIParam[3]))
	{
		Npc[m_nIndex].SetActiveSkill(3);
	}
	else
	{
		CommonAction();
		return;
	}
	FollowAttack(i);
	return;
}
*/

/*
//	ԽսԽ����
void KNpcAI::ProcessAIType9()
{
	int *pAIParam = Npc[m_nIndex].m_AiParam;

	if (KeepActiveRange())
		return;

	int i = Npc[m_nIndex].m_nPeopleIdx;

	if (!i || !InEyeshot(i))
	{
		i = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = i;
	}

	if (!i)
	{
		CommonAction();
		return;
	}
	
	int nLifePercent = Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax;

	if (g_RandPercent(pAIParam[0]))
	{
		Npc[m_nIndex].SetActiveSkill(1);
	}
	else if (nLifePercent < pAIParam[1] && g_RandPercent(pAIParam[2]))
	{
		Npc[m_nIndex].SetActiveSkill(2);
	}
	else if (nLifePercent < pAIParam[3] && g_RandPercent(pAIParam[4]))
	{
		Npc[m_nIndex].SetActiveSkill(3);
	}
	else
	{
		CommonAction();
		return;
	}
	FollowAttack(i);
	return;
}
*/

/*
//	���ܲ�����
void KNpcAI::ProcessAIType10()
{
	int *pAIParam = Npc[m_nIndex].m_AiParam;

	if (KeepActiveRange())
		return;

	int i = Npc[m_nIndex].m_nPeopleIdx;

	if (!i || !InEyeshot(i))
	{
		i = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = i;
	}

	if (!i)
	{
		CommonAction();
		return;
	}
	
	int nLifePercent = Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax;

	if (nLifePercent < pAIParam[0] && g_RandPercent(pAIParam[1]))
	{
		Npc[m_nIndex].SetActiveSkill(1);
	}
	else if (nLifePercent < pAIParam[2] && g_RandPercent(pAIParam[3]))
	{
		Npc[m_nIndex].SetActiveSkill(2);
	}
	else if (nLifePercent < pAIParam[4] && g_RandPercent(pAIParam[5]))
	{
		Flee(i);
		return;
	}
	else
	{
		CommonAction();
		return;
	}

	FollowAttack(i);
	return;
}
*/
