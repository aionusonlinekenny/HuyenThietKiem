#include "KCore.h"
#include "../../Represent/iRepresent/iRepresentShell.h"

iRepresentShell*	g_pRepresent = 0;

unsigned int	l_Time = 0;


unsigned int IR_GetCurrentTime()
{
	return l_Time ;
}


void IR_UpdateTime()
{
	l_Time = timeGetTime();
}


void IR_NextFrame(int& nFrame, int nTotalFrame, unsigned int uInterval, unsigned int& uFlipTime)
{
	if (nTotalFrame > 1 && uInterval)
	{
		while ((l_Time - uFlipTime) >= uInterval)
		{
			uFlipTime += uInterval;
			if ((++nFrame) >= nTotalFrame)
			{
				uFlipTime = l_Time;
				nFrame = 0;
				break;
			}
		}
	}
}
