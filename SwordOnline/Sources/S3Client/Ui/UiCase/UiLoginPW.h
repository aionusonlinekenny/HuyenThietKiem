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

class KUiLoginPW : protected KWndShowAnimate
{
public:
	static KUiLoginPW* OpenWindow();		//打开窗口，返回唯一的一个类对象实例
	static KUiLoginPW* GetIfVisible();		//如果窗口正被显示，则返回实例指针
	static void			CloseWindow();		//关闭窗口
	static void			LoadScheme(const char* pScheme);//载入界面方案
private:
	KUiLoginPW() {}
	~KUiLoginPW() {}
	void	Initialize();							//初始化
	void 	OnOk();
	int 	OnCheckInput();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//窗口函数
private:
	static KUiLoginPW*		m_pSelf;
	enum WAIT_OTHER_WND_OPER_PARAM
	{
		WAIT_PW_INPUT,
	};
private:
	KWndText32			m_Input;
	KWndButton			m_InputBtn;
	KWndButton			m_ConfirmBtn;
	KWndButton			m_CloseBtn;
	int					m_nPW1;


};