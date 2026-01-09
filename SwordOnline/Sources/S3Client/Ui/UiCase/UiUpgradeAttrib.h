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

// Total slots: 1 equipment + 4 minerals + 1 lucky stone = 6
#define _UPGRADE_ATTRIB_SLOT_COUNT 6

// Slot indices
#define SLOT_EQUIPMENT  0  // Blue equipment to upgrade
#define SLOT_MINERAL1   1  // Special mineral type 1 (3 levels)
#define SLOT_MINERAL2   2  // Special mineral type 2 (3 levels)
#define SLOT_MINERAL3   3  // Special mineral type 3 (3 levels)
#define SLOT_MINERAL4   4  // Special mineral type 4 (3 levels)
#define SLOT_LUCKY_STONE 5 // Lucky stone for success boost

// Requirements
#define UPGRADE_COST_MONEY  1000000  // 100 v?n lu?ng
#define UPGRADE_COST_XU     2        // 2 xu

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

	// NEW: Success rate calculation
	int           CalculateSuccessRate();
	void          UpdateSuccessRateDisplay();
	int           GetMineralLevel(int nItemIndex);  // Get mineral level (1-3)

	// NEW: Attribute list display
	void          LoadEquipmentAttributes();         // Load attributes from equipment
	void          DisplayAttributeList();            // Display attribute list in UI
	void          OnSelectAttribute(int nSlot);      // Player clicks attribute
	void          ClearAttributeList();              // Clear when equipment removed

private:
	// Attribute info structure
	struct AttributeInfo
	{
		int nType;      // Attribute type
		int nValue;     // Current value
		int nMin;       // Min value
		int nMax;       // Max value
		int nNewValue;  // Calculated new value after upgrade
		int nActualSlot; // ACTUAL slot index in equipment (0-5)
		bool bCanUpgrade; // Can this attribute be upgraded?
	};

	AttributeInfo m_Attributes[6];  // Up to 6 magic attributes
	int           m_nAttributeCount; // Number of valid attributes

private:
	KWndObjectBox      m_UpgradeSlot[_UPGRADE_ATTRIB_SLOT_COUNT]; // Item slots
	KWndLabeledButton  m_BtnUpgrade;            // Upgrade button
	KWndLabeledButton  m_BtnClose;              // Close button
	KCanGetNumImage    m_UpgradeEffect;         // Effect animation
	unsigned int       m_EffectTime;            // Effect duration
	int                m_nSelectedAttrib;       // Selected attribute index (0-5)
	BOOL               m_bUpgradeInProgress;    // Lock flag during upgrade processing
	KWndText80         m_TextPercent;           // Success rate display
	KWndText80         m_EquipmentLabel;        // Equipment slot label
	KWndText80         m_MaterialLabel;         // Material slot label
	char               m_szReturnInfo[8][128];  // Error messages

	// NEW: Attribute selection buttons
	KWndLabeledButton  m_BtnAttrib[6];          // Attribute selection buttons (up to 6)
	KWndText80         m_TextGuide;             // Guide text below material label

	enum STRING_NOTE_EVENT
	{
		ISP_DO_EVENT = 0x100,
	};
};