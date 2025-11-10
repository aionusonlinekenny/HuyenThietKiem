/*******************************************************************************
File        : UiTrembleItem.h
Creator     : AlexKing || AWJX
create data : 01.05.2014
Chuc nang	: Kham nam trang bi xanh
********************************************************************************/
#pragma once
#include "../elem/wndbutton.h"
#include "../elem/wndtext.h"
#include "../elem/WndObjContainer.h"
#include "../elem/wndlabeledbutton.h"
#include "../elem/wndimage.h"
#include "../../../core/src/gamedatadef.h"
#include "../Elem/WndShowAnimate.h"

#define _ITEM_TREMBLE_COUNT 9

struct KUiObjAtRegion;

class KUiTrembleItem : public KWndShowAnimate
{
public:
	static        KUiTrembleItem* OpenWindow();      //打开窗口
	static        KUiTrembleItem* GetIfVisible();    //如果窗口正被显示，则返回实例指针
	static void   CloseWindow(bool bDestory = TRUE); //关闭窗口，同时可以选则是否删除对象实例
	static void   LoadScheme(const char* pScheme);   //载入界面方案
	void		  UpdateData();
	void		  UpdateItem(KUiObjAtRegion* pItem, int bAdd);	
private:
	static        KUiTrembleItem *m_pSelf;
private:
	KUiTrembleItem() {}
	~KUiTrembleItem() {}
	void          Initialize();
	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	virtual void  Breathe();
	void		  OnItemPickDrop(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);
	void		  StartEffect();
	void		  StopEffect();
	BOOL		  IsEffect();
	void		  OnOk();	
	void		  OnCancel();	
	void		  UpdatePickPut(bool bLock);
	BOOL		  EnoughItemPickDrop(KWndWindow* pWnd, int nIndex);	
	BOOL		  EnoughItemToGo();		
	void 		  PecentSuccess(BYTE btType);
private:
	KWndObjectBox		 m_TrembleBox[_ITEM_TREMBLE_COUNT];
	KWndLabeledButton    m_Confirm;                         //合成按钮
	KWndLabeledButton    m_Cancel;                          //关闭按钮
	KCanGetNumImage	  		 m_TrembleEffect;
	unsigned int		 m_EffectTime; //thoi gian trinh dien hieu ung
	char				 ArrayReturnInfo[8][128];	
	KWndText80			m_TextPecent;
	
	enum STRING_NOTE_EVENT
	{
		ISP_DO_EVENT		= 0x100,
	};		
};
