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

class KUiChaPW : protected KWndShowAnimate
{
public:
	static KUiChaPW* OpenWindow();		//打开窗口，返回唯一的一个类对象实例
	static KUiChaPW* GetIfVisible();		//如果窗口正被显示，则返回实例指针
	static void			CloseWindow();		//关闭窗口
	static void			LoadScheme(const char* pScheme);//载入界面方案
private:
	KUiChaPW() {}
	~KUiChaPW() {}
	void	Initialize();							//初始化
	void 	OnOk();
	int 	OnCheckInput();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//窗口函数
private:
	static KUiChaPW*		m_pSelf;
	enum WAIT_OTHER_WND_OPER_PARAM
	{
		WAIT_PW_OLDPASS,
		WAIT_PW_INPUT,
		WAIT_PW_REINPUT,
	};
private:
	
	KWndText32			m_InputOld;
	KWndText32			m_Input;
	KWndText32			m_ReInput;
	KWndButton			m_InputOldBtn;
	KWndButton			m_InputBtn;
	KWndButton			m_ReInputBtn;
	KWndButton			m_ConfirmBtn;
	KWndButton			m_CloseBtn;
	int					m_nPW1;
	int					m_nPW2;
	int					m_nPWOld;

};