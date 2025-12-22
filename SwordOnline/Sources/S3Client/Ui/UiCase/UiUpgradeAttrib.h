/*******************************************************************************
File        : UiUpgradeAttrib.h
Creator     : Based on UiTrembleItem by AlexKing
Create Date : 2025-12-21
Function    : Upgrade equipment magic attributes
*******************************************************************************/
#pragma once
#include "../elem/wndbutton.h"
#include "../elem/wndtext.h"
#include "../elem/WndObjContainer.h"
#include "../elem/wndlabeledbutton.h"
#include "../elem/wndimage.h"
#include "../../../core/src/gamedatadef.h"
#include "../Elem/WndShowAnimate.h"

#define _UPGRADE_ATTRIB_SLOT_COUNT 3

struct KUiObjAtRegion;

class KUiUpgradeAttrib : public KWndShowAnimate
{
public:
	static        KUiUpgradeAttrib* OpenWindow();      // Open window
	static        KUiUpgradeAttrib* GetIfVisible();    // Get instance if visible
	static void   CloseWindow(bool bDestory = TRUE);   // Close window
	static void   LoadScheme(const char* pScheme);     // Load scheme
	void          UpdateData();
	void          UpdateItem(KUiObjAtRegion* pItem, int bAdd);

private:
	static        KUiUpgradeAttrib *m_pSelf;

private:
	KUiUpgradeAttrib() {}
	~KUiUpgradeAttrib() {}

	void          Initialize();
	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	virtual void  Breathe();

	void          OnItemPickDrop(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);
	void          StartEffect();
	void          StopEffect();
	BOOL          IsEffect();

	void          OnUpgrade();     // Execute upgrade
	void          OnCancel();      // Cancel and return items

	void          UpdatePickPut(bool bLock);
	BOOL          ValidateItemPickDrop(KWndWindow* pWnd, int nIndex);
	BOOL          ValidateUpgradeReady();

private:
	KWndObjectBox      m_UpgradeSlot[_UPGRADE_ATTRIB_SLOT_COUNT]; // Item slots
	KWndLabeledButton  m_BtnUpgrade;            // Upgrade button
	KWndLabeledButton  m_BtnClose;              // Close button
	KCanGetNumImage    m_UpgradeEffect;         // Effect animation
	unsigned int       m_EffectTime;            // Effect duration
	int                m_nSelectedAttrib;       // Selected attribute index (0-5)
	KWndText80         m_TextPercent;           // Success rate display
	char               m_szReturnInfo[8][128];  // Error messages

	enum STRING_NOTE_EVENT
	{
		ISP_DO_EVENT = 0x100,
	};
};
