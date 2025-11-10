#pragma once

#include "../Elem/WndImage.h"
#include "../Elem/UiImage.h"
#include "../Elem/WndPureTextBtn.h"

#define		defMAX_UI_CITY		7
class KUiWorldMap : protected KWndImage
{
public:
	static KUiWorldMap* OpenWindow();		
	static void			CloseWindow();		
	static KUiWorldMap*	GetIfVisible();
	void				LoadScheme(const char* pScheme);	

private:
	KUiWorldMap();
	~KUiWorldMap() {}
	void	Initialize();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	Breathe();
	void	UpdateData();
private:
	static KUiWorldMap* m_pSelf;
	KWndImage			m_Country, m_City, m_Capital, m_Cave, m_Field;

	short		m_nLeft[defMAX_UI_CITY];
	short		m_nTop[defMAX_UI_CITY];
};