/*****************************************************************************************
//	界面// --储物箱界面
//	Copyright : Kingsoft 2003
//	Author	:   Kinnox
//	CreateTime:	08.06.2021
*****************************************************************************************/

#pragma once
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndText.h"
#include "../elem/WndButton.h"

class KUiReWard : protected KWndShowAnimate
{
public:
	static KUiReWard* OpenWindow(const char* pszTitle, const char* pszString1, const char* pszString2, const char* pszString3);										
	static void			 CloseWindow(bool bDestroy);
	static KUiReWard*	GetIfVisible();		
private:

	void	Initialize();										
	void	LoadScheme(const char* pScheme);	
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	ShowButton();
private:
	static 	KUiReWard* m_pSelf;
private:
	KWndButton		BtnReWard1, BtnReWard2, BtnReWard3, m_CloseBtn;
	KWndText80		m_Notice;
	char			szString1[32], szString2[32], szString3[32];
	char			szFunc1[32], szFunc2[32], szFunc3[32];
	BYTE			m_Btn1, m_Btn2, m_Btn3;
	char			szStringMoney[64], szStringExp[64], szStringStone[64], szStringRandom[64], szStringEquip[64], 
						szStringMedicine[64], szStringGold[64], szStringViolet[64],szStringRoll[64];	
	char			szImgMoney[64], szImgExp[64], szImgStone[64], szImgRandom[64], szImgEquip[64], szImgMedicine[64], szImgGold[64], szImgViolet[64],szImgRoll[64];	
};