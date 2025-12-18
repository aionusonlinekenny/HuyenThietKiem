
#ifndef KDInput_H
#define KDInput_H
class ENGINE_API KDirectInput
{
private:
	LPDIRECTINPUT8			m_pDirectInput;
	LPDIRECTINPUTDEVICE8	m_pMouseDevice;
	LPDIRECTINPUTDEVICE8	m_pKeyboardDevice;
private:
	BOOL	CreateDirectInput();
	BOOL	CreateMouseDevice();
	BOOL	CreateKeyboardDevice();
public:
	KDirectInput();
	~KDirectInput();
	BOOL	Init();
	void	Exit();
	BOOL	GetMouseState(PINT dx, PINT dy, PBYTE lb, PBYTE rb);
	BOOL	GetKeyboardState(PBYTE KeyBuffer);
	BOOL	SetMouseBehavior(BOOL bExclusive);
};
extern ENGINE_API KDirectInput* g_pDirectInput;
#endif
