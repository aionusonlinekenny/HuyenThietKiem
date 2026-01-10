
#ifndef KPlayerShop_H
#define KPlayerShop_H

#include	"KPlayerDef.h"

class KShop
{
	friend class KPlayer;
private:
#ifdef _SERVER
	BOOL	m_bIsOpen;
	char	m_szAdvText[16];
#else
	DWORD	m_dwNpcID;
	char	m_szName[16];
	PlayerItem*	m_sItem;
	DWORD*	m_dwItemID;
	BYTE	m_btUseIdx;
	BYTE	m_btMaxItem;
	int		m_nDataIndex;
#endif

public:
	KShop();
	void		Release();
#ifdef _SERVER
	void	SetState(BOOL bState)
	{
		m_bIsOpen = bState;
	}
	inline BOOL	GetState() const { return m_bIsOpen; };
	void	SetAdvText(const char* lpszText)
	{
		memcpy(m_szAdvText, lpszText, sizeof(m_szAdvText));
	}
	inline char* GetAdv() const { return (char*)m_szAdvText; };
#else
	void	ApplyViewShop(DWORD dwNpcID, int nDataIndex = 0);
	void	DeleteAll();
	void	GetData(BYTE* pMsg);
	inline BYTE	GetItemCountMax() const { return m_btMaxItem; };
	inline int GetItemIndex(int nIdx) const { return m_sItem[nIdx].nIdx; };
	inline int GetItemPosX(int nIdx) const { return m_sItem[nIdx].nX; };
	inline int GetItemPosY(int nIdx) const { return m_sItem[nIdx].nY; };
	DWORD	FindID(int nIndex);
	inline DWORD GetDestNpc() const { return m_dwNpcID; };
	inline BYTE	GetItemCount() const { return m_btUseIdx; };
	void	UpdateData();
#endif

};
#endif
