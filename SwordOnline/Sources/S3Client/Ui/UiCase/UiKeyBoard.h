/*****************************************************************************************
//	界面// --储物箱界面
//	Copyright : Kingsoft 2003
//	Author	:   Kinnox
//	CreateTime:	03.06.2021
*****************************************************************************************/
#pragma once

#include "../elem/WndButton.h"
#include "../elem/WndText.h"
#include "../elem/WndObjContainer.h"
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndEdit.h"
#define	MAX_NUM_KEY 6
class KUiKeyBoard : protected KWndShowAnimate
{
public:
	static KUiKeyBoard* OpenWindow(KWndWindow* pRequester, unsigned int uParam);		//打开窗口，返回唯一的一个类对象实例
	static KUiKeyBoard* GetIfVisible();		//如果窗口正被显示，则返回实例指针
	static void			CloseWindow();		//关闭窗口
	static void			LoadScheme(const char* pScheme);//载入界面方案
private:
	KUiKeyBoard();
	void	Initialize();							//初始化
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//窗口函数
	void 	OnOk();
	void 	OnCancel();
	void 	OnCheckInput();
private:
	static KUiKeyBoard*		m_pSelf;
	enum NUM_COUNT
	{
		NUMBER_0 = 0,
		NUMBER_1,
		NUMBER_2,
		NUMBER_3,
		NUMBER_4,
		NUMBER_5,
		NUMBER_6,
		NUMBER_7,
		NUMBER_8,
		NUMBER_9,
		MAX_NUMBER_KEY,
	};
private:
	KWndEdit06			m_InputEdit;
	KWndButton			m_ReInputBtn;
	KWndButton			m_Number[MAX_NUMBER_KEY];
	int					m_Value[MAX_NUMBER_KEY];
	int					m_NumFinal;
	KWndButton			m_CloseBtn;
	KWndButton			m_ConfirmBtn;
private:
	KWndWindow*			m_pRequester;
	unsigned int		m_uRequesterParam;
};