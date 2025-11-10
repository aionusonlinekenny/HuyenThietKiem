#if !defined(AFX_TONGDB_H__8C487F07_C490_4562_AD01_77953CCCBCE2__INCLUDED_)
#define AFX_TONGDB_H__8C487F07_C490_4562_AD01_77953CCCBCE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DBTable.h"
#include "KTongControl.h"

#define MAX_TONG_COUNT 100

typedef struct
{	
	BYTE 		btCamp;	
	BYTE 		btLevel;
	DWORD		dwMoney;

	char		szName[defTONG_NAME_MAX_LENGTH + 1];
	char		szMasterTitle[defTONG_TITLE_MAX_LENGTH + 1];
	char		szDirectorTitle[defTONG_MAX_DIRECTOR][defTONG_TITLE_MAX_LENGTH + 1];
	char		szManagerTitle[defTONG_MAX_MANAGER][defTONG_TITLE_MAX_LENGTH + 1];
	
	char		szMaleTitle[defTONG_TITLE_MAX_LENGTH + 1];	
	char		szFemaleTitle[defTONG_TITLE_MAX_LENGTH + 1];
	
}TTongStruct;

typedef struct
{
	TONG_MEMBER_FIGURE		MemberClass;			
	int						nTitleIndex;
	BYTE					bGender;
	char					szName[defPLAYER_NAME_MAX_LENGTH + 1];
	char					szTong[defTONG_NAME_MAX_LENGTH + 1];
}TMemberStruct;

class CTongDB  
{
private:
	ZDBTable* m_TongTable;						
	ZDBTable* m_MemberTable;					
	
	TTongList m_TongList[MAX_TONG_COUNT];	
	int m_TongListCount;						
public:
	CTongDB();
	virtual ~CTongDB(){};

	bool Open();
	void Close();
	
	bool ChangeTong(CTongControl& aTongControl);	
	bool DelTong(char* aTongName);	
	bool SearchTong(char* aTongName, CTongControl& aTongControl);	
	int GetTongCount();							
	int GetTongList(TTongList* aTongList, int aMaxNum);	

	bool ChangeMember(TMemberStruct aMember);
	bool DelMember(char* aMemberName);
	
};

#endif // !defined(AFX_TONGDB_H__8C487F07_C490_4562_AD01_77953CCCBCE2__INCLUDED_)
