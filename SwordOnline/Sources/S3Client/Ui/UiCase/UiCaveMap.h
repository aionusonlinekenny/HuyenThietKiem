#pragma once

#include "../Elem/WndImage.h"

class KUiCaveMap : protected KWndImage
{
public:
	static KUiCaveMap* OpenWindow();		
	static void			CloseWindow();		
	static KUiCaveMap*	GetIfVisible();
	void				LoadScheme(const char* pScheme);	

private:
	KUiCaveMap() {}
	~KUiCaveMap() {}
	void	Initialize();
	void	Breathe();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	UpdateData();
private:
	static KUiCaveMap* m_pSelf;
	KWndImage			m_Sign;
};