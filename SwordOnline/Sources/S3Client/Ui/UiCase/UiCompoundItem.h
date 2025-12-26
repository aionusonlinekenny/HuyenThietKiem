/*****************************************************************************************
//	����--����
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

    void Initialize();                                //��ʼ��
    void LoadScheme(const char *pScheme);            //������淽��

    void UpdateItem(KUiDraggedObject *pItem, int bAdd);

private:
    int WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
    void PaintWindow();                                //���ƴ���
private:
    KWndObjectBox m_Box1;
    KWndObjectBox m_Box2;
    KWndObjectBox m_Box3;
    KWndText256 m_Guide;

    KWndScrollBar m_ListScroll;                      //�б��Ĺ�����
    //KWndButton		m_ListBtn;
    KWndButton m_Compound;
    KWndButton m_Cancle;

    KCanGetNumImage2
            m_TrembleEffect1;                   //�ϳ��е���Ч
    KCanGetNumImage2
            m_TrembleEffect2;                   //�ϳ��е���Ч
    KCanGetNumImage2
            m_TrembleEffect3;                   //�ϳ��е���Ч
};

class KUiEnchaseTim : public KWndPage {
public:
    KUiEnchaseTim();

    void Initialize();                                //��ʼ��
    void LoadScheme(const char *pScheme);            //������淽��

    void UpdateItem(KUiDraggedObject *pItem, int bAdd);

    void CleanItem();

private:
    int WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
    void PaintWindow();                                //���ƴ���
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

    KWndScrollBar m_ListScroll;                      //�б��Ĺ�����
    KWndButton m_ListBtn;
    KWndButton m_Distill;
    KWndButton m_Cancle;

    KCanGetNumImage2
            m_TrembleEffect1;                   //�ϳ��е���Ч
    KCanGetNumImage2
            m_TrembleEffect2;                   //�ϳ��е���Ч
    KCanGetNumImage2
            m_TrembleEffect3;                   //�ϳ��е���Ч
};

class KUiForge : public KWndPage {
public:
    KUiForge();

    void Initialize();                                //��ʼ��
    void LoadScheme(const char *pScheme);            //������淽��

    void UpdateItem(KUiDraggedObject *pItem, int bAdd);

    void CleanItem();

private:
    int WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
    void PaintWindow();                                //���ƴ���
private:
    KWndObjectBox m_BigBox;
    KWndObjectBox m_SmallBox;
    KWndMessageListBox m_Guide;
    KWndText32 m_Pos1;
    KWndText32 m_Pos2;

    KWndScrollBar m_ListScroll;                      //�б��Ĺ�����
    KWndButton m_ListBtn;
    KWndButton m_ForgeBtn;
    KWndButton m_Cancle;

    KCanGetNumImage2
            m_TrembleEffect1;                   //�ϳ��е���Ч
};

class KUiDistill : public KWndPage {
public:
    KUiDistill();

    void Initialize();                                //��ʼ��
    void LoadScheme(const char *pScheme);            //������淽��

    void UpdateItem(KUiDraggedObject *pItem, int bAdd);

    void CleanItem();

private:
    int WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
    void PaintWindow();                                //���ƴ���
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

    KWndScrollBar m_ListScroll;                      //�б��Ĺ�����
    KWndButton m_ListBtn;
    KWndButton m_Distill;
    KWndButton m_Cancle;

    KCanGetNumImage2
            m_TrembleEffect1;                   //�ϳ��е���Ч
    KCanGetNumImage2
            m_TrembleEffect2;                   //�ϳ��е���Ч
    KCanGetNumImage2
            m_TrembleEffect3;                   //�ϳ��е���Ч
};

class KUiCompound : public KWndPage {
public:
    KUiCompound();

    void Initialize();                                //��ʼ��
    void LoadScheme(const char *pScheme);            //������淽��

    void UpdateItem(KUiDraggedObject *pItem, int bAdd);

    void SetPosText(int i = 1);

    void CleanItem();

private:
    int WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
    void PaintWindow();

    virtual void Breathe();

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

    KWndScrollBar m_ListScroll;                      //�б��Ĺ�����
    KWndButton m_ListBtn;
    KWndButton m_Compound;
    KWndButton m_Cancle;

    KCanGetNumImage2
            m_TrembleEffect1;                   //�ϳ��е���Ч
    KCanGetNumImage2
            m_TrembleEffect2;                   //�ϳ��е���Ч
    KCanGetNumImage2
            m_TrembleEffect3;                   //�ϳ��е���Ч
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
    static KUiComItem *OpenWindow();                            //�򿪴��ڣ�����Ψһ��һ�������ʵ��
    static KUiComItem *GetIfVisible();                        //�������������ʾ���򷵻�ʵ��ָ��
    static void CloseWindow(bool bDestory = TRUE);            //�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
    static void LoadScheme(const char *pScheme);        //������淽��
    static void ShowWindow(int nNum = 0);

    void UpdateItem(KUiObjAtRegion *pItem, int bAdd);

    int GetWindowsNum();

    void CleanItem();

    void ComItem(unsigned int pItem, int nWindowNum, int nNum);

private:
    KUiComItem() {}

    ~KUiComItem() {}

    void Initialize();                            //��ʼ��
    int WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���

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
    int m_nStatus;                         //��ǰ����������״̬
    int m_nNum;
};
