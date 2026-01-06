/*****************************************************************************************
//	????--????
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-28
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../elem/wndpage.h"
#include "../Elem/WndImagePart.h"
#include "../elem/wndbutton.h"
#include "../elem/wndtext.h"
#include "../elem/WndObjContainer.h"
#include "../Elem/WndMessage.h"

#include "../elem/wndlabeledbutton.h"
#include "../elem/wndscrollbar.h"
#include "../elem/wndimage.h"
#include "../elem/wndlist2.h"
#include "../Elem/WndMessageListBox.h"

#include "../../../core/src/gamedatadef.h"

struct KUiObjAtRegion;  // Forward declaration

class KCanGetNumImage2 : public KWndImage {
public:
    int GetMaxFrame();

    int GetCurrentFrame();
};

class KUiAtlas : public KWndPage {
public:
    KUiAtlas();

    void Initialize();                                //??'??
    void LoadScheme(const char *pScheme);            //?????????

    void UpdateItem(KUiDraggedObject *pItem, int bAdd);

private:
    int WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//???????
    void PaintWindow();                                //???????
private:
    KWndObjectBox m_Box1;
    KWndObjectBox m_Box2;
    KWndObjectBox m_Box3;
    KWndText256 m_Guide;

    KWndScrollBar m_ListScroll;                      //????L?????
    //KWndButton		m_ListBtn;
    KWndButton m_Compound;
    KWndButton m_Cancle;

    KCanGetNumImage2
            m_TrembleEffect1;                   //?????????
    KCanGetNumImage2
            m_TrembleEffect2;                   //?????????
    KCanGetNumImage2
            m_TrembleEffect3;                   //?????????
};

class KUiEnchaseTim : public KWndPage {
public:
    KUiEnchaseTim();

    void Initialize();                                //??'??
    void LoadScheme(const char *pScheme);            //?????????

    void UpdateItem(KUiDraggedObject *pItem, int bAdd);
    void UpdateData();  // Load items from server and show boxes

    void CleanItem();

    // Effect animation methods (added for enchase effect)
    void Breathe();          // Called every frame to update animation
    int PlayEffect();        // Returns 1 if still animating, 0 if complete
    void UpdateResult();     // Called after effect completes to send server request

private:
    int WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//???????
    void PaintWindow();                                //???????
private:
    KWndObjectBox m_BigBox;
    KWndObjectBox m_Box1;
    KWndObjectBox m_Box2;
    KWndObjectMatrix m_ItemBox;
    KWndMessageListBox m_Guide;
    KWndText32 m_Pos1;
    KWndText32 m_Pos2;
    KWndText32 m_Pos3;
    KWndText32 m_Pos4;

    KWndScrollBar m_ListScroll;                      //????L?????
    KWndButton m_ListBtn;
    KWndButton m_Distill;
    KWndButton m_Cancle;

    // Animation status (added for enchase effect)
    int m_nStatus;  // STATUS_WAITING_MATERIALS, STATUS_BEGIN_TREMBLE, STATUS_TREMBLING, etc.
    int m_EffectTime;  // Animation time counter (like Upgrade tab)

    KCanGetNumImage2
            m_TrembleEffect1;                   //?????????
    KCanGetNumImage2
            m_TrembleEffect2;                   //?????????
    KCanGetNumImage2
            m_TrembleEffect3;                   //?????????

private:
    enum THIS_INTERFACE_STATUS {
        STATUS_WAITING_MATERIALS,
        STATUS_BEGIN_TREMBLE,
        STATUS_TREMBLING,
        STATUS_CHANGING_ITEM,
        STATUS_FINISH,
    };
};

class KUiForge : public KWndPage {
public:
    KUiForge();

    void Initialize();                                //??'??
    void LoadScheme(const char *pScheme);            //?????????

    void UpdateData(); // Load items from server when tab opens
    void UpdateItem(KUiDraggedObject *pItem, int bAdd);

    void CleanItem();

    void StartEffect();  // Start crafting animation
    void StopEffect();   // Stop crafting animation
    BOOL IsEffect();     // Check if effect is running
    void Breathe();  // Animation frame update - must be public for parent to call

private:
    int WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//???????
    void PaintWindow();                                //???????
private:
    int m_EffectTime;  // Effect animation timer
private:
    KWndObjectBox m_BigBox;
    KWndObjectBox m_SmallBox;
    KWndMessageListBox m_Guide;
    KWndText32 m_Pos1;
    KWndText32 m_Pos2;

    KWndScrollBar m_ListScroll;                      //????L?????
    KWndButton m_ListBtn;
    KWndButton m_ForgeBtn;
    KWndButton m_Cancle;

    KCanGetNumImage2
            m_TrembleEffect1;                   //?????????
};

class KUiDistill : public KWndPage {
public:
    KUiDistill();

    void Initialize();                                //??'??
    void LoadScheme(const char *pScheme);            //?????????

    void UpdateItem(KUiDraggedObject *pItem, int bAdd);
    void UpdateData();  // Load items from server and show boxes

    void CleanItem();

    // Effect animation methods (added for extraction effect)
    void Breathe();          // Called every frame to update animation
    int PlayEffect();        // Returns 1 if still animating, 0 if complete
    void UpdateResult();     // Called after effect completes to send server request

private:
    int WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//???????
    void PaintWindow();                                //???????
private:
    KWndObjectBox m_BigBox;
    KWndObjectBox m_Box1;
    KWndObjectBox m_Box2;
    KWndObjectMatrix m_ItemBox;
    KWndMessageListBox m_Guide;
    KWndText32 m_Pos1;
    KWndText32 m_Pos2;
    KWndText32 m_Pos3;
    KWndText32 m_Pos4;

    KWndScrollBar m_ListScroll;                      //????L?????
    KWndButton m_ListBtn;
    KWndButton m_Distill;
    KWndButton m_Cancle;

    // Animation status (added for extraction effect)
    int m_nStatus;  // STATUS_WAITING_MATERIALS, STATUS_BEGIN_TREMBLE, STATUS_TREMBLING, etc.

    KCanGetNumImage2
            m_TrembleEffect1;                   //?????????
    KCanGetNumImage2
            m_TrembleEffect2;                   //?????????
    KCanGetNumImage2
            m_TrembleEffect3;                   //?????????

private:
    enum THIS_INTERFACE_STATUS {
        STATUS_WAITING_MATERIALS,
        STATUS_BEGIN_TREMBLE,
        STATUS_TREMBLING,
        STATUS_CHANGING_ITEM,
        STATUS_FINISH,
    };
};

class KUiCompound : public KWndPage {
public:
    KUiCompound();

    void Initialize();                                //??'??
    void LoadScheme(const char *pScheme);            //?????????

    void UpdateItem(KUiDraggedObject *pItem, int bAdd);
    void UpdateData();  // Load items from server and show boxes

    void SetPosText(int i = 1);

    void CleanItem();

    void Breathe();  // Animation frame update - must be public for parent to call

private:
    int WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//???????
    void PaintWindow();

    int PlayEffect();

    int m_nStatus;

    void UpdateResult();

private:
    KWndObjectBox m_Box1;
    KWndObjectBox m_Box2;
    KWndObjectBox m_Box3;
    KWndMessageListBox m_Guide;
    KWndText32 m_Pos1;
    KWndText32 m_Pos2;
    KWndText32 m_Pos3;

    int m_nSelect;

    KWndScrollBar m_ListScroll;                      //????L?????
    KWndButton m_ListBtn;
    KWndButton m_Compound;
    KWndButton m_Cancle;

    KCanGetNumImage2
            m_TrembleEffect1;                   //?????????
    KCanGetNumImage2
            m_TrembleEffect2;                   //?????????
    KCanGetNumImage2
            m_TrembleEffect3;                   //?????????
private:
    enum THIS_INTERFACE_STATUS {
        STATUS_WAITING_MATERIALS,
        STATUS_BEGIN_TREMBLE,
        STATUS_TREMBLING,
        STATUS_CHANGING_ITEM,
        STATUS_FINISH,
    };

};

class KUiComItem : public KWndPageSet {
public:
    static KUiComItem *OpenWindow();                            //????????????h??h??????????
    static KUiComItem *GetIfVisible();                        //??????????????????????????
    static void CloseWindow(bool bDestory = TRUE);            //?????????????????????????????
    static void LoadScheme(const char *pScheme);        //?????????
    static void ShowWindow(int nNum = 0);

    void UpdateItem(KUiObjAtRegion *pItem, int bAdd);

    int GetWindowsNum();

    void CleanItem();

    void ComItem(unsigned int pItem, int nWindowNum, int nNum);

private:
    KUiComItem() {}

    ~KUiComItem() {}

    void Initialize();                            //??'??
    int WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//???????

    virtual void Breathe();

    int PlayEffect();

private:
    static KUiComItem *m_pSelf;
private:

    enum THIS_INTERFACE_STATUS {
        STATUS_WAITING_MATERIALS,
        STATUS_BEGIN_TREMBLE,
        STATUS_TREMBLING,
        STATUS_CHANGING_ITEM,
        STATUS_FINISH,
    };

    enum THIS_WINDOWS {
        WINDOWS_COMP,
        WINDOWS_COMP2,
        WINDOWS_COMP3,
        WINDOWS_DISTill,
        WINDOWS_FORG,
        WINDOWS_ENCHASE,
        WINDOWS_ATLAS,
    };

    KUiCompound m_CompoundPad;
    KUiDistill m_DistillPad;
    KUiForge m_ForgePad;
    KUiEnchaseTim m_EnchasePad;
    KUiAtlas m_AtlasPad;

    KWndButton m_CompoundPadBtn;
    KWndButton m_DistillPadBtn;
    KWndButton m_ForgePadBtn;
    KWndButton m_EnchasePadBtn;
    KWndButton m_AtlasPadBtn;
    KWndButton m_UpCryoliteBtn;
    KWndButton m_UpPropMineBtn;
    KWndButton m_Close;
    KWndText32 m_CheTao;


private:
    int m_nStatus;                         //??j????????????
    int m_nNum;
};