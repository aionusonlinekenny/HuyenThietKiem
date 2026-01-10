
#pragma once

#if !defined _CHATITEMINFO
#define _CHATITEMINFO

#include "../Elem/WndButton.h"
#include "../Elem/WndEdit.h"
#include "../elem/WndPureTextBtn.h"
#include "../../../core/src/GameDataDef.h"

class KUiChatItemInfo : public KWndWindow
{
public:

	static KUiChatItemInfo* OpenWindow(const char* pItem, int nLen, const char* plszIconImage, int nIconWith, int nIconHeight);
	static void		CloseWindow();
	static KUiChatItemInfo*	GetIfVisible();
	void			LoadScheme(const char* pScheme);
	
private:

	KUiChatItemInfo();
	~KUiChatItemInfo() {}
	void		PaintWindow();
	int			WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void		Initialize();
	void		Show();
	void		Hide();
	void		Breathe();
	void		UpdateData(const char* lpszIconImage);
	int			CloseWindowSpace(unsigned int uKey);
private:

	static KUiChatItemInfo* m_pSelf;

private:

	KWndImage				m_IconImage;

private:

	unsigned int			m_nBgColor;
	unsigned int			m_nPicColor;
	unsigned short			m_nBgAlpha;
	unsigned short			m_nRectColor;
	char					pItemDesc[GOD_MAX_OBJ_TITLE_LEN];
	short					pItemLen;
	short					nMaxLineLen;
	short					nNumLine;
	KRUImagePart			m_Image;
	short					m_nImgWidth;	        
	short					m_nImgHeight;
	short					m_nIconWidth;	        
	short					m_nIconHeight;	
	KRURect					Rect;
};

#endif