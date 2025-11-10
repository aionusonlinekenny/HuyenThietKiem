//copyright: alexking - kinnox;
#ifndef	_KAUTOAI_H_
#define	_KAUTOAI_H_

#include <vector>
#include <string>

#define DIS_MOVE 500

class AutoAi;
class KAutoMove
{
	public:
		KAutoMove() {m_CurDir = 1;};
		INT			AutoMove();
		INT			AutoMoveTo(INT nX, INT nY);
		VOID		Reset() {m_CurDir = 1;};
	protected:
		INT			FindPathAndMove(INT &nDir8, INT minStep = -1);
		inline INT CheckDirectMoveDis(KNpc *pNpc, INT nDir8, INT nSpeed);
		INT 		m_CurDir;
};

#endif