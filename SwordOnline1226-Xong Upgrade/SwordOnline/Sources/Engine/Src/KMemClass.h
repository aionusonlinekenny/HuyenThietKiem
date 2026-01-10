#ifndef KMemClass_H
#define KMemClass_H
class ENGINE_API KMemClass
{
private:
	PVOID		m_lpMemPtr;
	DWORD		m_lpMemLen;
public:
	KMemClass();
	~KMemClass();
	PVOID		Alloc(DWORD dwSize);
	void		Free();
	void		Zero();
	void		Fill(BYTE byFill);
	void		Fill(WORD wFill);
	void		Fill(DWORD dwFill);
	PVOID		GetMemPtr() { return m_lpMemPtr; };
	DWORD		GetMemLen() { return m_lpMemLen; };
};
#endif
