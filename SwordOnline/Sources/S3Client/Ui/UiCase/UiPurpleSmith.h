/*******************************************************************************
File        : UiPurpleSmith.h
Creator     : Based on UiTrembleItem by AlexKing
Create Date : 2025-12-26
Function    : Purple Item Crafting System - Tho Ren Huyen Bi
*******************************************************************************/
#pragma once
#include "../elem/wndbutton.h"
#include "../elem/wndtext.h"
#include "../elem/WndObjContainer.h"
#include "../elem/wndlabeledbutton.h"
#include "../elem/wndimage.h"
#include "../../../core/src/gamedatadef.h"
#include "../Elem/WndShowAnimate.h"

#define _PURPLE_SMITH_SLOT_COUNT 17  // 10 blue items + 7 crystals

struct KUiObjAtRegion;

class KUiPurpleSmith : public KWndShowAnimate
{
public:
	static        KUiPurpleSmith* OpenWindow();      // Open window
	static        KUiPurpleSmith* GetIfVisible();    // Get instance if visible
	static void   CloseWindow(bool bDestory = TRUE); // Close window
	static void   LoadScheme(const char* pScheme);   // Load scheme
	void          UpdateData();
	void          UpdateItem(KUiObjAtRegion* pItem, int bAdd);

private:
	static        KUiPurpleSmith *m_pSelf;

private:
	KUiPurpleSmith() {}
	~KUiPurpleSmith() {}

	void          Initialize();
	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	virtual void  Breathe();

	void          OnItemPickDrop(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);
	void          StartEffect();
	void          StopEffect();
	BOOL          IsEffect();

	void          OnCraft();       // Execute purple item crafting
	void          OnCancel();      // Cancel and return items

	void          UpdatePickPut(bool bLock);
	BOOL          ValidateItemPickDrop(KWndWindow* pWnd, int nIndex);
	BOOL          ValidateCraftReady();
	void          UpdateSuccessRate();

private:
	KWndObjectBox      m_CraftSlot[_PURPLE_SMITH_SLOT_COUNT]; // Item slots (10 blue + 7 crystal)
	KWndLabeledButton  m_BtnCraft;              // Craft button
	KWndLabeledButton  m_BtnClose;              // Close button
	KCanGetNumImage    m_CraftEffect;           // Effect animation
	unsigned int       m_EffectTime;            // Effect duration
	int                m_nSelectedRecipe;       // Selected recipe (0-4)
	KWndText80         m_TextSuccessRate;       // Success rate display
	KWndText80         m_TextCost;              // Cost display
	KWndText80         m_BlueItemsLabel;        // Blue items label
	KWndText80         m_CrystalsLabel;         // Crystals label
	char               m_szReturnInfo[10][128]; // Error/info messages

	enum STRING_NOTE_EVENT
	{
		ISP_DO_EVENT = 0x100,
	};
};
