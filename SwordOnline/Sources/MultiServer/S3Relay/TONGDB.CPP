#include "stdafx.h"
#include "Global.h"
#include "db.h"
#include "DBTable.h"
#include "TongDB.h"

CTongDB		g_cTongDB;


//

int get_TongName(DB *db, const DBT *pkey, const DBT *pdata, DBT *ikey) 
{
	memset(ikey, 0, sizeof( DBT ));
	TMemberStruct* pMemberStruct = (TMemberStruct*)pdata->data;
	ikey->data = pMemberStruct->szTong;
	ikey->size = strlen( pMemberStruct->szTong ) + 1;
	return 0;
}


//

CTongDB::CTongDB()
{
	m_TongListCount = 0;
	memset(m_TongList, 0, sizeof(TTongList) * MAX_TONG_COUNT);
}


//

bool CTongDB::Open()
{
	m_TongTable = new ZDBTable("TongDB","TongTable");		
	if(!m_TongTable->open())
		return false;

	m_MemberTable = new ZDBTable("TongMemberDB","MemberTable");	
	m_MemberTable->addIndex(get_TongName);
	if(!m_MemberTable->open())
	{
		m_TongTable->close();
		return false;
	}
	return true;
}


//

void CTongDB::Close()
{
	m_TongTable->close();
	m_MemberTable->close();
	delete m_TongTable;
	m_TongTable = NULL; //them code 16/09/2023 by kinnox;
	delete m_MemberTable;
	m_MemberTable = NULL; //them code 16/09/2023 by kinnox;
}


//

bool CTongDB::ChangeTong(CTongControl& aTongControl)
{
	bool aResult = true;
	int aKeySize = strlen(aTongControl.m_szName) + 1;
	int aSize = sizeof(TTongStruct);
	TTongStruct aTongStruct;
	memset(&aTongStruct, 0, aSize);

	aTongStruct.btCamp = aTongControl.m_btCamp;
	aTongStruct.btLevel = aTongControl.m_btLevel;
	aTongStruct.dwMoney = aTongControl.m_dwMoney;
	strcpy(aTongStruct.szName, aTongControl.m_szName);
	strcpy(aTongStruct.szMasterTitle, aTongControl.m_szMasterTitle);
	int i = 0;
	for(i = 0; i < defTONG_MAX_DIRECTOR; ++i)
	{
		strcpy(aTongStruct.szDirectorTitle[i], aTongControl.m_szDirectorTitle[i]);
	}
	for(i = 0; i < defTONG_MAX_MANAGER; ++i)
	{
		strcpy(aTongStruct.szManagerTitle[i], aTongControl.m_szManagerTitle[i]);
	}
	
	strcpy(aTongStruct.szMaleTitle, aTongControl.m_szMaleTitle);
	strcpy(aTongStruct.szFemaleTitle, aTongControl.m_szFemaleTitle);
	

	aResult = m_TongTable->add(aTongControl.m_szName, aKeySize, (char*)(&aTongStruct), aSize);
	
	return aResult;
}


//

bool CTongDB::DelTong(char* aTongName)
{
	bool aResult = true;
	int aKeySize = strlen(aTongName) + 1;

	aResult = m_TongTable->remove(aTongName, aKeySize);
	if(!aResult) return aResult;

	while(!m_MemberTable->remove(aTongName, aKeySize, 0))
		{}
	return true;
}


//

bool CTongDB::SearchTong(char* aTongName, CTongControl& aTongControl)
{
	bool aResult = true;
	int aSize;
	int aKeySize = strlen(aTongName) + 1;
	char* aBuffer = m_TongTable->search(aTongName, aKeySize, aSize);
	if(!aBuffer) 
		return false;

	TTongStruct* pTongStruct = (TTongStruct*)aBuffer;

	aTongControl.m_btCamp		= pTongStruct->btCamp;
	aTongControl.m_btLevel		= pTongStruct->btLevel;
	aTongControl.m_dwMoney		= pTongStruct->dwMoney;
	strcpy(aTongControl.m_szName, pTongStruct->szName);
	strcpy(aTongControl.m_szMasterTitle, pTongStruct->szMasterTitle);
	int i;
	for(i = 0; i < defTONG_MAX_DIRECTOR; ++i)
	{
		strcpy(aTongControl.m_szDirectorTitle[i], pTongStruct->szDirectorTitle[i]);
	}
	for(i = 0; i < defTONG_MAX_MANAGER; ++i)
	{
		strcpy(aTongControl.m_szManagerTitle[i], pTongStruct->szManagerTitle[i]);
	}
	
	strcpy(aTongControl.m_szMaleTitle, pTongStruct->szMaleTitle);
	strcpy(aTongControl.m_szFemaleTitle, pTongStruct->szFemaleTitle);
	

	int aDirectorNum = 0;
	int aManagerNum = 0;
	int aMemberNum = 0;

	aBuffer = m_MemberTable->search(aTongName, aKeySize, aSize, 0);
	while(aBuffer)
	{
		TMemberStruct* pMemberStruct = (TMemberStruct*)aBuffer;
		switch(pMemberStruct->MemberClass)
		{
		case enumTONG_FIGURE_MASTER:
			strcpy(aTongControl.m_szMasterName, pMemberStruct->szName);
			aTongControl.m_dwMasterID		= g_String2Id(pMemberStruct->szName);
			aTongControl.m_bMasterGender	= pMemberStruct->bGender;
			break;
			
		case enumTONG_FIGURE_DIRECTOR:
			if (pMemberStruct->nTitleIndex < 0 || pMemberStruct->nTitleIndex >= defTONG_MAX_DIRECTOR)
				break;
			strcpy(aTongControl.m_szDirectorName[pMemberStruct->nTitleIndex], pMemberStruct->szName);
			aTongControl.m_dwDirectorID[pMemberStruct->nTitleIndex] = g_String2Id(pMemberStruct->szName);
			aTongControl.m_bDirectorGender[pMemberStruct->nTitleIndex] = pMemberStruct->bGender;
			aDirectorNum++;
			break;
			
		case enumTONG_FIGURE_MANAGER:
			if (pMemberStruct->nTitleIndex < 0 || pMemberStruct->nTitleIndex >= defTONG_MAX_MANAGER)
				break;
			strcpy(aTongControl.m_szManagerName[pMemberStruct->nTitleIndex], pMemberStruct->szName);
			aTongControl.m_dwManagerID[pMemberStruct->nTitleIndex] = g_String2Id(pMemberStruct->szName);
			aTongControl.m_bManagerGender[pMemberStruct->nTitleIndex] = pMemberStruct->bGender;
			aManagerNum++;
			break;
			
		case enumTONG_FIGURE_MEMBER:
			aTongControl.AddMember(pMemberStruct->szName, pMemberStruct->bGender);
			aMemberNum++;
			break;
			
		}
		aBuffer = m_MemberTable->next(aSize);
	}
	aTongControl.m_nDirectorNum = aDirectorNum;
	aTongControl.m_nManagerNum = aManagerNum;
	aTongControl.m_nMemberNum = aMemberNum;

	delete [] aBuffer;
	aBuffer = NULL; //them code 16/09/2023 by kinnox;
	return aResult;
}


//

int CTongDB::GetTongCount()
{
	int aCount = 0;
	int aTongSize;
	int aTongKeySize;
	char aTongKeyBuffer[defTONG_NAME_MAX_LENGTH + 1];
	char aTongBuffer[sizeof(TTongStruct)];
	bool aResult = m_TongTable->GetRecordEx(aTongBuffer, aTongSize, aTongKeyBuffer, aTongKeySize, ZDBTable::cpFirst);
	while(aResult)
	{	
		aCount++;
		aResult = m_TongTable->GetRecordEx(aTongBuffer, aTongSize, aTongKeyBuffer, aTongKeySize,ZDBTable::cpNext);
	}

	return aCount;
}



//

int CTongDB::GetTongList(TTongList* aTongList, int aMaxNum)
{
		int aCount = 0;
	int aTongSize;
	int aTongKeySize;
	char aTongKeyBuffer[defTONG_NAME_MAX_LENGTH + 1];
	char aTongBuffer[sizeof(TTongStruct)];
	bool aResult = m_TongTable->GetRecordEx(aTongBuffer, aTongSize, aTongKeyBuffer, aTongKeySize,ZDBTable::cpFirst);
	while(aResult)
	{
		TTongStruct* pTong = (TTongStruct*)aTongBuffer;
		strcpy(aTongList[aCount].szName, pTong->szName);

		int aMemCount = 0;
		int aMemSize;
		char* aMemBuffer;
		aMemBuffer = m_MemberTable->search(aTongKeyBuffer, aTongKeySize, aMemSize, 0);
		while(aMemBuffer)
		{
			TMemberStruct* pMember = (TMemberStruct*)aMemBuffer;
			if(pMember->MemberClass == enumTONG_FIGURE_MEMBER)
					aMemCount++;
			delete [] aMemBuffer;
			aMemBuffer = m_MemberTable->next(aMemSize);
		}

		aTongList[aCount].MemberCount = aMemCount;

		aCount++;

		if( aCount>= aMaxNum ) 
			break;
		aResult = m_TongTable->GetRecordEx(aTongBuffer, aTongSize, aTongKeyBuffer, aTongKeySize,ZDBTable::cpNext);
	}

	return aCount;
}


//

bool CTongDB::ChangeMember(TMemberStruct aMember)
{
	return m_MemberTable->add(aMember.szName, strlen(aMember.szName) + 1, (char*)(&aMember), sizeof(TMemberStruct));
}


//

bool CTongDB::DelMember(char* aMemberName)
{
	int aKeySize = strlen(aMemberName) + 1;

	return m_MemberTable->remove(aMemberName, aKeySize);
}