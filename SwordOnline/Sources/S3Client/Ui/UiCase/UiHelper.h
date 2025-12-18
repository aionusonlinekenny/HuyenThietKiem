
#pragma once
#if !defined _UIHELPER1
#define _UIHELPER1

#include "../Elem/WndImage.h"

class KUiHelper : protected KWndImage
{
public:
	static KUiHelper* OpenWindow(bool bShow);
	static void				CloseWindow(bool bDestroy);
	static KUiHelper*		GetIfVisible();
	void					LoadScheme(const char* pScheme);
	static void SelectInterfacePic(); 
	static void SelectKeyboardPic();  
	static void SelectWuxingPic();    

private:
	KUiHelper();
	virtual ~KUiHelper();
	static KUiHelper* m_pSelf;
	static int ms_nImgFrame;
	int					WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void Initialize();
	void	Breathe() {NextFrame();}
	virtual void	Show();
};


#endif