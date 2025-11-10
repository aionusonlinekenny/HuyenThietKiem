// DBBackup.cpp: implementation of the CDBBackup class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DBTABLE.H"
#include "DBBackup.h"
#include "DBDumpLoad.h"
#include <FSTREAM>
#include <direct.h>
#include "Macro.h"
#include "CRC32.h"
#include <stdio.h>
#include <io.h>
#include <stdarg.h>
using namespace std;

#define IS_OUTPUT_LOG true

#define ROLE_FILTER_FILE "RoleFilter.ini"
#define ROLE_FILTER_COUNT 300

extern HWND hListOutput;
extern void AddOutputString(HWND hListCtrl, char* aStr);
static char DBPath[32] = {0};
static char DBName[32] = {0};
static ZDBTable* RunTable = NULL;
static bool IsBackupWorking = false;
static bool IsThreadWorking = false;
static bool IsSuspended = false;
static WORD BackupTime = 0;
static bool IsTimeToBackup = true;
static CDBBackup::TStatData oldGameStatData;
static CDBBackup::TStatData newGameStatData;
static TGAME_STAT_DATA SendStatData;

// =================== Helper: CRC32 patch cho ban ghi ===================
static void PatchRecordCRC32(BYTE* pData, DWORD dwLenInclCRC)
{
    if (!pData || dwLenInclCRC < 4) return;
    DWORD crc = 0;
    crc = CRC32(crc, pData, dwLenInclCRC - 4);
    // little-endian
    pData[dwLenInclCRC - 4] = (BYTE)(crc & 0xFF);
    pData[dwLenInclCRC - 3] = (BYTE)((crc >> 8) & 0xFF);
    pData[dwLenInclCRC - 2] = (BYTE)((crc >> 16) & 0xFF);
    pData[dwLenInclCRC - 1] = (BYTE)((crc >> 24) & 0xFF);
}

// ======================================================================

int CDBBackup::GetIndexByName(char* aName, TRoleList* aList, int aListSize)
{
	if(aList[0].Name[0] == '\0') return -1;
	for(int i=0;i<aListSize;++i)
	{
		if(aList[i].Name[0] == '\0') return -1;
		if (strcmp(aName, aList[i].Name) == 0)
			return i;
	}
	return -1;
}

CDBBackup::TRoleList* CDBBackup::GetMin(
					TRoleList* const aRoleList,
					const int n,
					const StatType aType)
{
	TRoleList* tmpData;
	tmpData = &aRoleList[0];
	int i = 0;
	while(true)
	{
		for(i = 0; i < n; ++i)
		{
			if(aRoleList[i].Name[0] == '\0')
				return &aRoleList[i];
			int aSourse, aDest;
			switch(aType)
			{
			case stMoney:
				aDest = tmpData->Money;
				aSourse = aRoleList[i].Money;
				break;
			case stLevel:
				aDest = tmpData->Level;
				aSourse = aRoleList[i].Level;
				break;
			case stKiller:
				aDest = tmpData->KillNum;
				aSourse = aRoleList[i].KillNum;
				break;
			}
			if(aDest > aSourse)
			{
				tmpData = &aRoleList[i];
				break;
			}
		}
		if(i==n)break;
	}
	return tmpData;
}


void CDBBackup::ListSort(
					TRoleList* const aRoleList,
					const int n,
					const StatType aType)
{
	int i,j;
	for(i=0;i<n;++i)
	{
		TRoleList* tmpData = &aRoleList[i];
		for(j=i+1;j<n;++j)
		{
			__int64 aSourse, aDest;
			switch(aType)
			{
			case stMoney:
				aDest = tmpData->Money;
				aSourse = aRoleList[j].Money;
				break;
			case stLevel:
				aDest = (__int64)tmpData->Level * 0xffffffff + (__int64)tmpData->FightExp;
				aSourse = (__int64)aRoleList[j].Level * 0xffffffff + (__int64)aRoleList[j].FightExp;
				break;
			case stKiller:
				aDest = tmpData->KillNum;
				aSourse = aRoleList[j].KillNum;
				break;
			}
			if( aSourse > aDest )
					swap(*tmpData, aRoleList[j]);
		}
	}
}


void CDBBackup::RoleDataCopy(TRoleList* Desc, TRoleData* Source)
{
	strcpy(Desc->Name, Source->BaseInfo.szName);
	Desc->Sect = Source->BaseInfo.nSect;
	Desc->Money = Source->BaseInfo.imoney + Source->BaseInfo.isavemoney;
	Desc->Level = Source->BaseInfo.ifightlevel + (200 * Source->BaseInfo.btTransLife);		//
	Desc->TransLife = Source->BaseInfo.btTransLife;		
	Desc->KillNum = Source->BaseInfo.nKillPeopleNumber;
	Desc->FightExp = Source->BaseInfo.fightexp; 
}

char* CDBBackup::LoadFilterName()
{
	char* aRoleList = new char[ROLE_FILTER_COUNT * 20];
	char aBuffer[20];
	char aAppPath[MAX_PATH] = {0};
	getcwd(aAppPath,MAX_PATH);
	strcat(aAppPath,"\\");
	strcat(aAppPath,ROLE_FILTER_FILE);

	fstream aFile(aAppPath,ios::in);
	if(!aFile.is_open())
	{
		SAFE_DELETE_ARRAY(aRoleList);
		return NULL;
	}
	for(int i=0;i<ROLE_FILTER_COUNT;++i)
	{
		aFile.getline(aBuffer, 20);
		if(!aBuffer[0])break;
		int aLen = strlen(aBuffer);
		strcpy(&aRoleList[i * 20],aBuffer);
		aRoleList[i *20 + 19] = '\0';
	}
	aFile.close();

	return aRoleList;
}

bool CDBBackup::IsRoleFilter(char* aName, char* aList)
{
	if(!aList) return false;
	for(int i=0;i<ROLE_FILTER_COUNT;++i)
	{
		if(aList[i * 20] == '\0') return false;
		if (strcmp(aName, &aList[i * 20]) == 0)
			return true;
	}
	return false;
}

CDBBackup::CDBBackup(char* aPath, char* aName, ZDBTable* aRunTable)
{
	memset(DBPath,0,32);
	memset(DBName,0,32);
	strcpy(DBPath, aPath);
	strcpy(DBName, aName);
	BackupTime = 0;
	m_hThread = NULL;
	m_hManualThread = NULL;
	RunTable = aRunTable;
	
	memset(&oldGameStatData, 0, sizeof(TStatData));
	memset(&newGameStatData, 0, sizeof(TStatData));
	memset(&SendStatData, 0, sizeof(TGAME_STAT_DATA));
	m_BackupLock = 0; // them: khoa chong chay trung backup (auto/manual)
}

// Kenny them
bool CDBBackup::TryEnterBackup() {
    // 0 -> 1, neu tra ve 0 tuc la chiem duoc lock
    return (InterlockedExchange((LPLONG)&m_BackupLock, 1) == 0);
}

void CDBBackup::LeaveBackup() {
    InterlockedExchange((LPLONG)&m_BackupLock, 0);
}

//////////////////////////////////////////////////////////////////////
bool CDBBackup::Open(int aTime)
{
	aTime = aTime % 24;
	if((DBPath[0] == '\0')||(DBName[0] == '\0'))
		return false;
	
	BackupTime = aTime;
	DWORD dwThreadId, dwThrdParam = 1;
	m_hThread = CreateThread(
		NULL,                        // no security attributes 
        0,                           // use default stack size  
        TimerThreadFunc,             // thread function 
        &dwThrdParam,                // argument to thread function 
        0,                           // use default creation flags 
        &dwThreadId);                // returns the thread identifier 
	if(!m_hThread)
	{
		BackupTime = 0;
		return false;
	}
	
	char aAppPath[MAX_PATH] = {0};
	getcwd(aAppPath,MAX_PATH);
	strcat(aAppPath,"\\StatData.dat");
	memset(&newGameStatData,0,sizeof(CDBBackup::TStatData));
	FILE* aStatFile = fopen(aAppPath,"rb");
	if(aStatFile)
	{		
		int a = fread(&newGameStatData, 1, sizeof(CDBBackup::TStatData), aStatFile);
		fclose(aStatFile);
	}
	MakeSendStatData();
	
	return true;
}

bool CDBBackup::Close()
{
	BackupTime = 0;
	if(m_hManualThread) CloseHandle( m_hManualThread );
	return (CloseHandle( m_hThread ) == TRUE);

}

bool CDBBackup::Suspend()
{
	if(!m_hThread) return false;	
	if(!IsThreadWorking) return false;	
	if(IsBackupWorking) return false;	
	if(IsSuspended) return false;
	if(SuspendThread(m_hThread) == -1)return false;
	IsSuspended = true;
	return true;
}

bool CDBBackup::Resume()
{
	if(!m_hThread) return false;	
	if(!IsSuspended) return false;	
	if(!IsThreadWorking) return false;	

	if(ResumeThread(m_hThread) == -1)return false;
	IsSuspended = false;
	return true;
}

TGAME_STAT_DATA CDBBackup::GetSendStatData()
{
	return SendStatData;
}

bool CDBBackup::IsWorking()
{
	return IsThreadWorking;
}

bool CDBBackup::IsBackuping()
{//备份是否在工作
	return IsBackupWorking;
}

bool CDBBackup::ManualBackup()
{
    // Neu muon bat buoc co timer: mo 2 dong duoi
    // if (!m_hThread)        return false;
    // if (!IsThreadWorking)  return false;

    // Bao dam he DB da init
    if (!RunTable) return false;

    // Chong chay trung
    if (!TryEnterBackup()) {
        // TODO: Log "ManualBackup: busy"
        return false;
    }

    // Dong handle cu neu con
    if (m_hManualThread) {
        CloseHandle(m_hManualThread);
        m_hManualThread = NULL;
    }

    DWORD tid = 0;
    // Truyen this vao thread
    m_hManualThread = CreateThread(
        NULL, 0, &CDBBackup::ManualThreadFunc, this, 0, &tid
    );
    if (!m_hManualThread) {
        LeaveBackup();
        BackupTime = 0;
        return false;
    }

    return true;
}

DWORD WINAPI CDBBackup::TimerThreadFunc( LPVOID lpParam )
{
	IsThreadWorking = true;
	while(true)
	{	
		/*
		SYSTEMTIME aSysTime;
		GetLocalTime(&aSysTime);
		if(aSysTime.wHour == BackupTime)
		{	
			if(IsTimeToBackup) {
                if (TryEnterBackup()) {
                    Backup();
                    LeaveBackup();
                }
            }
			IsTimeToBackup = false;
		}
		else
		{		
			IsTimeToBackup = true;
		}
		*/
		Sleep(1000 * 60 * 30);
		//Sleep(BackupTime);
	}
	IsThreadWorking = false;
	return 0;
}

DWORD WINAPI CDBBackup::ManualThreadFunc( LPVOID lpParam )
{
    // Sua: lay pThis tu lpParam, goi Backup() va tha lock
    CDBBackup* pThis = (CDBBackup*)lpParam;
    if (!pThis) return 0;
    pThis->Backup();
    pThis->LeaveBackup();
    return 0;
}



static void LogUIAndFile(std::fstream &log, const char* fmt, ...)
{
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    _vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
    buf[sizeof(buf) - 1] = '\0';
    va_end(ap);

    // ghi file
    if (log.is_open())
        log << buf << std::endl;

    // day ra UI listbox
    if (hListOutput)
        AddOutputString(hListOutput, buf);
}

void CDBBackup::Backup()
{
    IsBackupWorking = true;

    char aLogPath[MAX_PATH] = {0};
    getcwd(aLogPath, MAX_PATH);
    strcat(aLogPath, "\\Backup.log");

    fstream aLogFile(aLogPath, ios::out); // giu hanh vi cu: ghi de moi lan
    // fstream aLogFile(aLogPath, ios::out | ios::app); // neu muon ghi tiep, dung dong nay

    SYSTEMTIME aSysTime;
    GetSystemTime(&aSysTime);

    LogUIAndFile(aLogFile, "[Backup] Bat dau backup...");

    string aBackupDir;
    char aIntStr[16];
    itoa(aSysTime.wYear, aIntStr, 10);
    aBackupDir = aIntStr;
    itoa(aSysTime.wMonth, aIntStr, 10);
    aBackupDir += aIntStr;
    itoa(aSysTime.wDay, aIntStr, 10);
    aBackupDir += aIntStr;
    aBackupDir += "_";
    itoa(aSysTime.wHour, aIntStr, 10);
    aBackupDir += aIntStr;
    itoa(aSysTime.wMinute, aIntStr, 10);
    aBackupDir += aIntStr;
    itoa(aSysTime.wSecond, aIntStr, 10);
    aBackupDir += aIntStr;
    itoa(aSysTime.wMilliseconds, aIntStr, 10);
    aBackupDir += aIntStr;

    aLogFile << "Backup Log File Open Time:" << aSysTime.wYear << "."
             << aSysTime.wMonth << "." << aSysTime.wDay << " "
             << aSysTime.wHour << ":" << aSysTime.wMinute << ":" << aSysTime.wSecond << endl;

    // ==== Tao thu muc dich: .\<DBPath>\YYYYMMDD_HHMMSSmmm\ ====
    char baseDir[MAX_PATH] = {0};
    getcwd(baseDir, MAX_PATH);
    strcat(baseDir, "\\");
    strcat(baseDir, DBPath);
#ifdef WIN32
    mkdir(baseDir); // .\DBPath
#else
    mkdir(baseDir, 0);
#endif
    strcat(baseDir, "\\");
    strcat(baseDir, aBackupDir.c_str());
#ifdef WIN32
    mkdir(baseDir); // .\DBPath\YYYYMMDD_HHMMSSmmm
#else
    mkdir(baseDir, 0);
#endif
    LogUIAndFile(aLogFile, "[Backup] Thu muc dich: %s", baseDir);

    // ==== File .txt thong ke: .\<DBPath>\...\<DBName>.txt ====
    char aDBSFullPath[MAX_PATH] = {0};
    strcpy(aDBSFullPath, baseDir);
    strcat(aDBSFullPath, "\\");
    strcat(aDBSFullPath, DBName);
    strcat(aDBSFullPath, ".txt");
    fstream aDBSOutput(aDBSFullPath, ios::out);
    aDBSOutput << "账号名\t角色名\t门派\t等级\t金钱" << endl;

    int    aDBSPlayerCount = 0;
    int    aDBSSectPlayerCount[12] = {0};
    double aDBSMoneyCount = 0;
    double aDBSSectMoneyCount[12] = {0};
    int    aDBSLevelPlayerCount[200] = {0};

    // ==== File .bak dump: .\<DBPath>\...\<DBName>.bak ====
    char aBakFullPath[MAX_PATH] = {0};
    strcpy(aBakFullPath, baseDir);
    strcat(aBakFullPath, "\\");
    strcat(aBakFullPath, DBName);
    strcat(aBakFullPath, ".bak");

    CDBDump DBDump;
    if (!DBDump.Open(aBakFullPath))
    {
        LogUIAndFile(aLogFile, "[Backup][ERROR] Khong mo duoc file dump: %s", aBakFullPath);
        IsBackupWorking = false;
        return;
    }
    LogUIAndFile(aLogFile, "[Backup] Mo file dump: %s", aBakFullPath);

    if (!RunTable)
    {
        aLogFile << "RunTable NULL Error." << endl;
        LogUIAndFile(aLogFile, "[Backup][ERROR] RunTable = NULL");
        IsBackupWorking = false;
        return;
    }

    char*    RoleFilterList = LoadFilterName();
    TStatData aStatData;
    memset(&aStatData, 0, sizeof(TStatData));

    ZCursor *cursor = RunTable->first();
    aLogFile << "RunTable cursor Opened. Backup begin." << endl;
    LogUIAndFile(aLogFile, "[Backup] RunTable cursor opened. Bat dau dump...");

    while (cursor)
    {
        if (!DBDump.AddData(cursor->key, cursor->key_size, cursor->data, cursor->size))
        {
            aLogFile << "Role[" << cursor->key << "] Dump Error." << endl;
        }

        TRoleList*  tmpData;
        TRoleData*  pRoleData = (TRoleData*)cursor->data;

        char aDBSSect[32] = {0};
        int  aDBSSectIndex = (int)pRoleData->BaseInfo.nSect;
        switch (aDBSSectIndex)
        {
        case 0: strcpy(aDBSSect, "少林派"); break;
        case 1: strcpy(aDBSSect, "天王帮"); break;
        case 2: strcpy(aDBSSect, "唐门"); break;
        case 3: strcpy(aDBSSect, "五毒教"); break;
        case 4: strcpy(aDBSSect, "峨嵋派"); break;
        case 5: strcpy(aDBSSect, "翠烟门"); break;
        case 6: strcpy(aDBSSect, "丐帮"); break;
        case 7: strcpy(aDBSSect, "天忍教"); break;
        case 8: strcpy(aDBSSect, "武当派"); break;
        case 9: strcpy(aDBSSect, "昆仑派"); break;
        default:
            if (pRoleData->BaseInfo.ijoincount == 0)
            { strcpy(aDBSSect, "新手"); break; }
            else
            { strcpy(aDBSSect, "出师"); break; }
        }
        if (aDBSSectIndex == 255)
        {
            if (pRoleData->BaseInfo.ijoincount == 0)
            {
                ++aDBSSectPlayerCount[10];
                aDBSSectMoneyCount[10] += pRoleData->BaseInfo.isavemoney + pRoleData->BaseInfo.imoney;
            }
            else
            {
                ++aDBSSectPlayerCount[11];
                aDBSSectMoneyCount[11] += pRoleData->BaseInfo.isavemoney + pRoleData->BaseInfo.imoney;
            }
        }
        else
        {
            ++aDBSSectPlayerCount[aDBSSectIndex];
            aDBSSectMoneyCount[aDBSSectIndex] += pRoleData->BaseInfo.isavemoney + pRoleData->BaseInfo.imoney;
        }
        aDBSMoneyCount += pRoleData->BaseInfo.isavemoney + pRoleData->BaseInfo.imoney;
        ++aDBSPlayerCount;

        if (((pRoleData->BaseInfo.ifightlevel > 0)) && (pRoleData->BaseInfo.ifightlevel < 200))
            aDBSLevelPlayerCount[pRoleData->BaseInfo.ifightlevel]++;
        else
        {
            aDBSOutput << "***级别问题*** ";
            aDBSLevelPlayerCount[0]++;
        }

        aDBSOutput << pRoleData->BaseInfo.caccname << "\t"
                   << pRoleData->BaseInfo.szName << "\t"
                   << aDBSSect << "\t"
                   << pRoleData->BaseInfo.ifightlevel << "\t"
                   << pRoleData->BaseInfo.btTransLife << "\t"
                   << pRoleData->BaseInfo.isavemoney + pRoleData->BaseInfo.imoney << endl;

        if (IsRoleFilter(pRoleData->BaseInfo.szName, RoleFilterList))
        {
            if (!RunTable->next(cursor))
                break;
            continue;
        }

        tmpData = GetMin(aStatData.MoneyStat, MONEYSTATNUM, stMoney);
        if (tmpData->Money < pRoleData->BaseInfo.imoney + pRoleData->BaseInfo.isavemoney)
        {
            RoleDataCopy(tmpData, pRoleData);
        }

        tmpData = GetMin(aStatData.LevelStat, LEVELSTATNUM, stLevel);
        if (tmpData->Level < pRoleData->BaseInfo.ifightlevel + (200 * pRoleData->BaseInfo.btTransLife))
        {
            RoleDataCopy(tmpData, pRoleData);
        }

        tmpData = GetMin(aStatData.KillerStat, KILLERSTATNUM, stKiller);
        if (tmpData->KillNum < pRoleData->BaseInfo.nKillPeopleNumber)
        {
            RoleDataCopy(tmpData, pRoleData);
        }

        if ((pRoleData->BaseInfo.nSect < 10) && (pRoleData->BaseInfo.nSect >= 0))
        {
            tmpData = GetMin(aStatData.MoneyStatBySect[pRoleData->BaseInfo.nSect + 1], SECTMAXSTATNUM, stMoney);
        }
        else
        {
            tmpData = GetMin(aStatData.MoneyStatBySect[0], SECTMAXSTATNUM, stMoney);
        }

        if (tmpData->Money < pRoleData->BaseInfo.imoney + pRoleData->BaseInfo.isavemoney)
        {
            RoleDataCopy(tmpData, pRoleData);
        }

        if ((pRoleData->BaseInfo.nSect < 10) && (pRoleData->BaseInfo.nSect >= 0))
        {
            tmpData = GetMin(aStatData.LevelStatBySect[pRoleData->BaseInfo.nSect + 1], SECTMAXSTATNUM, stMoney);
        }
        else
        {
            tmpData = GetMin(aStatData.LevelStatBySect[0], SECTMAXSTATNUM, stMoney);
        }

        if (tmpData->Level < pRoleData->BaseInfo.ifightlevel + (200 * pRoleData->BaseInfo.btTransLife))
        {
            RoleDataCopy(tmpData, pRoleData);
        }

        if ((pRoleData->BaseInfo.nSect < 10) && (pRoleData->BaseInfo.nSect >= 0))
        {
            ++aStatData.SectPlayerNum[pRoleData->BaseInfo.nSect + 1];
        }
        else
        {
            ++aStatData.SectPlayerNum[0];
        }

        if (!RunTable->next(cursor)) break;
    }

    DBDump.Close();
    aLogFile << "DB Dump Finished." << endl;
    aLogFile << "RunTable cursor closed." << endl;
    LogUIAndFile(aLogFile, "[Backup] Dump xong. Tong so player: %d; Tong tien: %.0f", aDBSPlayerCount, aDBSMoneyCount);

    aDBSOutput << "==记录结束==" << endl << endl;
    aDBSOutput << "==统计==" << endl;
    aDBSOutput << "总人数：" << aDBSPlayerCount << endl;

    int i, j;
    for (i = 0; i < 12; ++i)
    {
        char aDBSSect[32] = {0};
        switch (i)
        {
        case 0:  strcpy(aDBSSect, "少林派"); break;
        case 1:  strcpy(aDBSSect, "天王帮"); break;
        case 2:  strcpy(aDBSSect, "唐门"); break;
        case 3:  strcpy(aDBSSect, "五毒教"); break;
        case 4:  strcpy(aDBSSect, "峨嵋派"); break;
        case 5:  strcpy(aDBSSect, "翠烟门"); break;
        case 6:  strcpy(aDBSSect, "丐帮"); break;
        case 7:  strcpy(aDBSSect, "天忍教"); break;
        case 8:  strcpy(aDBSSect, "武当派"); break;
        case 9:  strcpy(aDBSSect, "昆仑派"); break;
        case 10: strcpy(aDBSSect, "新手"); break;
        case 11: strcpy(aDBSSect, "出师"); break;
        }
        aDBSOutput << aDBSSect << "人数：" << aDBSSectPlayerCount[i] << endl;
    }
    aDBSOutput << "" << endl;
    aDBSOutput << "总金钱数：" << aDBSMoneyCount << endl;
    for (i = 0; i < 12; ++i)
    {
        char aDBSSect[32] = {0};
        switch (i)
        {
        case 0:  strcpy(aDBSSect, "少林派"); break;
        case 1:  strcpy(aDBSSect, "天王帮"); break;
        case 2:  strcpy(aDBSSect, "唐门"); break;
        case 3:  strcpy(aDBSSect, "五毒教"); break;
        case 4:  strcpy(aDBSSect, "峨嵋派"); break;
        case 5:  strcpy(aDBSSect, "翠烟门"); break;
        case 6:  strcpy(aDBSSect, "丐帮"); break;
        case 7:  strcpy(aDBSSect, "天忍教"); break;
        case 8:  strcpy(aDBSSect, "武当派"); break;
        case 9:  strcpy(aDBSSect, "昆仑派"); break;
        case 10: strcpy(aDBSSect, "新手"); break;
        case 11: strcpy(aDBSSect, "出师"); break;
        }
        aDBSOutput << aDBSSect << "金钱数：" << aDBSSectMoneyCount[i] << endl;
    }
    aDBSOutput << "" << endl;
    aDBSOutput << "角色级别分布[1-200级]：" << endl;
    for (i = 1; i < 200; ++i)
        if (aDBSLevelPlayerCount[i] != 0)
            aDBSOutput << i << "级人数：" << aDBSLevelPlayerCount[i] << endl;
    aDBSOutput << "级别异常人数：" << aDBSLevelPlayerCount[0] << endl;

    for (i = 0; i < MONEYSTATNUM; ++i)
    {
        if ((aStatData.MoneyStat[i].Sect <= 10) && (aStatData.MoneyStat[i].Sect >= 1))
        {
            ++aStatData.SectMoneyMost[aStatData.MoneyStat[i].Sect + 1];
        }
        else
        {
            ++aStatData.SectMoneyMost[0];
        }
    }

    for (i = 0; i < LEVELSTATNUM; ++i)
    {
        if ((aStatData.LevelStat[i].Sect <= 10) && (aStatData.LevelStat[i].Sect >= 1))
        {
            ++aStatData.SectLevelMost[aStatData.LevelStat[i].Sect + 1];
        }
        else
        {
            ++aStatData.SectLevelMost[0];
        }
    }

    ListSort(aStatData.MoneyStat, MONEYSTATNUM, stMoney);
    ListSort(aStatData.LevelStat, LEVELSTATNUM, stLevel);
    ListSort(aStatData.KillerStat, KILLERSTATNUM, stKiller);
    for (i = 0; i < 11; ++i)
    {
        ListSort(aStatData.MoneyStatBySect[i], SECTMAXSTATNUM, stMoney);
    }
    for (i = 0; i < 11; ++i)
    {
        ListSort(aStatData.LevelStatBySect[i], SECTMAXSTATNUM, stLevel);
    }

    SAFE_DELETE_ARRAY(RoleFilterList);

    // ===== xu ly StatData.dat =====
    char aAppPath[MAX_PATH] = {0};
    getcwd(aAppPath, MAX_PATH);
    strcat(aAppPath, "\\StatData.dat");

    memset(&oldGameStatData, 0, sizeof(CDBBackup::TStatData));
    FILE* aStatFile = fopen(aAppPath, "rb");
    if (aStatFile)
    {
        int a = fread(&oldGameStatData, 1, sizeof(CDBBackup::TStatData), aStatFile);
        fclose(aStatFile);
    }

    newGameStatData = aStatData;

    for (i = 0; i < 10; ++i)
    {
        int aIndex;
        aIndex = CDBBackup::GetIndexByName(
            newGameStatData.LevelStat[i].Name, oldGameStatData.LevelStat, LEVELSTATNUM);
        if (aIndex != -1)
        {
            if (i < aIndex)      newGameStatData.LevelStat[i].Sort = 1;
            else if (i > aIndex) newGameStatData.LevelStat[i].Sort = 255;
            else                 newGameStatData.LevelStat[i].Sort = 0;
        }
        else newGameStatData.LevelStat[i].Sort = 1;

        aIndex = CDBBackup::GetIndexByName(
            newGameStatData.MoneyStat[i].Name, oldGameStatData.MoneyStat, MONEYSTATNUM);
        if (aIndex != -1)
        {
            if (i < aIndex)      newGameStatData.MoneyStat[i].Sort = 1;
            else if (i > aIndex) newGameStatData.MoneyStat[i].Sort = 255;
            else                 newGameStatData.MoneyStat[i].Sort = 0;
        }
        else newGameStatData.MoneyStat[i].Sort = 1;

        aIndex = CDBBackup::GetIndexByName(
            newGameStatData.KillerStat[i].Name, oldGameStatData.KillerStat, KILLERSTATNUM);
        if (aIndex != -1)
        {
            if (i < aIndex)      newGameStatData.KillerStat[i].Sort = 1;
            else if (i > aIndex) newGameStatData.KillerStat[i].Sort = 255;
            else                 newGameStatData.KillerStat[i].Sort = 0;
        }
        else newGameStatData.KillerStat[i].Sort = 1;

        for (j = 0; j < 11; ++j)
        {
            aIndex = CDBBackup::GetIndexByName(
                newGameStatData.LevelStatBySect[j][i].Name, oldGameStatData.LevelStatBySect[j], SECTMAXSTATNUM);
            if (aIndex != -1)
            {
                if (i < aIndex)      newGameStatData.LevelStatBySect[j][i].Sort = 1;
                else if (i > aIndex) newGameStatData.LevelStatBySect[j][i].Sort = 255;
                else                 newGameStatData.LevelStatBySect[j][i].Sort = 0;
            }
            else newGameStatData.LevelStatBySect[j][i].Sort = 1;

            aIndex = CDBBackup::GetIndexByName(
                newGameStatData.MoneyStatBySect[j][i].Name, oldGameStatData.MoneyStatBySect[j], SECTMAXSTATNUM);
            if (aIndex != -1)
            {
                if (i < aIndex)      newGameStatData.MoneyStatBySect[j][i].Sort = 1;
                else if (i > aIndex) newGameStatData.MoneyStatBySect[j][i].Sort = 255;
                else                 newGameStatData.MoneyStatBySect[j][i].Sort = 0;
            }
            else newGameStatData.MoneyStatBySect[j][i].Sort = 1;
        }
    }

    aStatFile = fopen(aAppPath, "wb");
    if (aStatFile)
    {
        int a = fwrite(&newGameStatData, 1, sizeof(CDBBackup::TStatData), aStatFile);
        fclose(aStatFile);
        LogUIAndFile(aLogFile, "[Backup] Cap nhat va luu StatData.dat thanh cong");
    }
    else
    {
        LogUIAndFile(aLogFile, "[Backup][ERROR] Khong mo duoc StatData.dat de ghi");
    }

    MakeSendStatData();

    if (IS_OUTPUT_LOG) {
        aLogFile << "DB Statistic Log:" << endl;
        aLogFile << "// --Level// --" << endl;
        for (i = 0; i < 10; ++i)
        {
            aLogFile << SendStatData.LevelStat[i].Name << "\t"
                     << SendStatData.LevelStat[i].nValue << "\t"
                     << (int)SendStatData.LevelStat[i].bySort << "\t"
                     << (int)SendStatData.LevelStat[i].TransLife << endl;
        }
        aLogFile << "// --Money// --" << endl;
        for (i = 0; i < 10; ++i)
        {
            aLogFile << SendStatData.MoneyStat[i].Name << "\t"
                     << SendStatData.MoneyStat[i].nValue << "\t"
                     << (int)SendStatData.MoneyStat[i].bySort << endl;
        }
        aLogFile << "// --Killer// --" << endl;
        for (i = 0; i < 10; ++i)
        {
            aLogFile << SendStatData.KillerStat[i].Name << "\t"
                     << SendStatData.KillerStat[i].nValue << "\t"
                     << (int)SendStatData.KillerStat[i].bySort << endl;
        }
        for (i = 0; i < 11; ++i)
        {
            aLogFile << "// --Sect " << i << " Level// --" << endl;
            for (j = 0; j < 10; ++j)
            {
                aLogFile << SendStatData.LevelStatBySect[i][j].Name << "\t"
                         << SendStatData.LevelStatBySect[i][j].nValue << "\t"
                         << (int)SendStatData.LevelStatBySect[i][j].bySort << "\t"
                         << (int)SendStatData.LevelStat[i].TransLife << endl;
            }
            aLogFile << "-Sect " << i << " Money// ---" << endl;
            for (j = 0; j < 10; ++j)
            {
                aLogFile << SendStatData.MoneyStatBySect[i][j].Name << "\t"
                         << SendStatData.MoneyStatBySect[i][j].nValue << "\t"
                         << (int)SendStatData.MoneyStatBySect[i][j].bySort << endl;
            }
        }
        aLogFile << "各个门派的玩家数" << endl;
        for (i = 0; i < 11; ++i)
        {
            aLogFile << "Sect " << i << " ：" << SendStatData.SectPlayerNum[i] << endl;
        }
        aLogFile << "财富排名前 " << MONEYSTATNUM << " 玩家中各门派所占比例数" << endl;
        for (i = 0; i < 11; ++i)
        {
            aLogFile << "Sect " << i << " ：" << SendStatData.SectMoneyMost[i] << endl;
        }
        aLogFile << "级别排名前 " << MONEYSTATNUM << " 玩家中各门派所占比例数" << endl;
        for (i = 0; i < 11; ++i)
        {
            aLogFile << "Sect " << i << " ：" << SendStatData.SectLevelMost[i] << endl;
        }
    }

    // Flush/close thong ke text (de chac chan day xuong disk som)
    aDBSOutput.flush();
    // aDBSOutput.close(); // optional

    LogUIAndFile(aLogFile, "[Backup] Hoan tat backup. Xem log tai Backup.log");

    IsBackupWorking = false;
}


void CDBBackup::MakeSendStatData()
{
	int i,j;
	memset(&SendStatData,0,sizeof(TGAME_STAT_DATA));
	for(i=0;i<10;++i)
	{
		strcpy(SendStatData.LevelStat[i].Name, newGameStatData.LevelStat[i].Name);
		SendStatData.LevelStat[i].nValue = newGameStatData.LevelStat[i].Level;
		SendStatData.LevelStat[i].bySort = newGameStatData.LevelStat[i].Sort;
		SendStatData.LevelStat[i].TransLife = newGameStatData.LevelStat[i].TransLife;	

		strcpy(SendStatData.MoneyStat[i].Name, newGameStatData.MoneyStat[i].Name);
		SendStatData.MoneyStat[i].nValue = newGameStatData.MoneyStat[i].Money;
		SendStatData.MoneyStat[i].bySort = newGameStatData.MoneyStat[i].Sort;

		strcpy(SendStatData.KillerStat[i].Name, newGameStatData.KillerStat[i].Name);
		SendStatData.KillerStat[i].nValue = newGameStatData.KillerStat[i].KillNum;
		SendStatData.KillerStat[i].bySort = newGameStatData.KillerStat[i].Sort;
		
		for(j=0;j<11;++j)
		{
			strcpy(SendStatData.LevelStatBySect[j][i].Name, newGameStatData.LevelStatBySect[j][i].Name);
			SendStatData.LevelStatBySect[j][i].nValue = newGameStatData.LevelStatBySect[j][i].Level;
			SendStatData.LevelStatBySect[j][i].bySort = newGameStatData.LevelStatBySect[j][i].Sort;
			SendStatData.LevelStatBySect[j][i].TransLife = newGameStatData.LevelStatBySect[j][i].TransLife; 

			strcpy(SendStatData.MoneyStatBySect[j][i].Name, newGameStatData.MoneyStatBySect[j][i].Name);
			SendStatData.MoneyStatBySect[j][i].nValue = newGameStatData.MoneyStatBySect[j][i].Money;
			SendStatData.MoneyStatBySect[j][i].bySort = newGameStatData.MoneyStatBySect[j][i].Sort;

			SendStatData.SectLevelMost[j] = newGameStatData.SectLevelMost[j];
			SendStatData.SectMoneyMost[j] = newGameStatData.SectMoneyMost[j];
			SendStatData.SectPlayerNum[j] = newGameStatData.SectPlayerNum[j];
		}
	}
}

void CDBBackup::SaveStatInfo()
{
	TStatData aStatData;
	char aAppPath[MAX_PATH] = {0};
	getcwd(aAppPath,MAX_PATH);
	strcat(aAppPath,"\\StatData.dat");
	
	memset(&aStatData,0,sizeof(TStatData));
	FILE* aStatFile = fopen(aAppPath,"rb");
	if(aStatFile)
	{		
		int aRead = fread(&aStatData, sizeof(TStatData), 1, aStatFile);
		if(aRead != 1) return;
		fclose(aStatFile);
	}
	else
	{
		return;
	}
	
	int i,j;
	for(i=0;i<LEVELSTATNUM;++i)
	{
		if(aStatData.LevelStat[i].Name[0] == '\0')continue;
		ZCursor *cursor = RunTable->search(aStatData.LevelStat[i].Name, strlen(aStatData.LevelStat[i].Name) +1);

		if(!cursor)
		{
			continue;
		}

		TRoleData* pRoleData = (TRoleData*)cursor->data;
		pRoleData->BaseInfo.nWorldStat = i+1;

		// Cap nhat CRC 4 byte cuoi
		PatchRecordCRC32((BYTE*)pRoleData, pRoleData->dwDataLen);

		RunTable->add(aStatData.LevelStat[i].Name, strlen(aStatData.LevelStat[i].Name) +1, cursor->data, cursor->size);
		RunTable->closeCursor(cursor);
	}


	for(i=0;i<11;++i)
	{
		for(j=0;j<SECTMAXSTATNUM;++j)
		{
			if(aStatData.LevelStatBySect[i][j].Name[0] == '\0')continue;
			ZCursor *cursor = RunTable->search( aStatData.LevelStatBySect[i][j].Name, strlen(aStatData.LevelStatBySect[i][j].Name) +1 );

			if(!cursor)
			{
				continue;
			}
			TRoleData* pRoleData = (TRoleData*)cursor->data;
			pRoleData->BaseInfo.nSectStat = j+1;

			// Cap nhat CRC 4 byte cuoi
			PatchRecordCRC32((BYTE*)pRoleData, pRoleData->dwDataLen);

			RunTable->add(aStatData.LevelStatBySect[i][j].Name, strlen(aStatData.LevelStatBySect[i][j].Name) +1, cursor->data, cursor->size);
			RunTable->closeCursor(cursor);
		}
	}
}
