#include "KWin32.h"
#include "../../Represent/iRepresent/iRepresentShell.h"

#include "LightLoop.h"
#include <math.h>

extern iRepresentShell*	g_pRepresentShell;



#define  MINFRAMEITEMBACK 30
#define  MAXFRAMEITEMBACK 120
#define  TOTALFRAMEITEMBACK  4

#define  FRAMEDELAYITEMBACK 2 // 4 //speed vong sang hoang kim by kinnox;
#define	 FRAMEGOITEMBACK 1

unsigned int l_BlueBorder[] =
{	
	0x0a00002d,
	0x0a000032,
	0x0a000037,
	0x0a00003c,
	0x0a000041,
	0x0a000046,
	0x0a00004b,
	0x0a000050,
	0x0a000055,
	0x0a00005a,
	0x0a00005f,
	0x0a000064,
	0x0a000069,
};	

unsigned int l_PurpleBorder[] =
{	
	0x0a2d002d,
	0x0a320032,
	0x0a370037,
	0x0a3c003c,
	0x0a410041,
	0x0a460046,
	0x0a4b004b,
	0x0a500050,
	0x0a550055,
	0x0a5a005a,
	0x0a5f005f,
	0x0a640064,
	0x0a690069,
};

unsigned int l_GoldBorder[] =
{	

	// 0x0a40311c,
	// 0x0a40311c,
	// 0x0a4a3920,
	// 0x0a4a3920,
	0x0a5a4527,
	0x0a5a4527,
	0x0a644d2c,
	0x0a644d2c,
	0x0a6e5530,
	0x0a6e5530,
	0x0a755a33,
	0x0a755a33,
	
	0x0a75643d,
	0x0a75643d,
	0x0a75643d,
	0x0a75643d,
	
	0x0a755a33,
	0x0a755a33,
	0x0a755a33,
	0x0a755a33,
	0x0a755a33,
	0x0a6e5530,
	0x0a6e5530,
	0x0a644d2c,
	0x0a644d2c,
	0x0a5a4527,
	0x0a5a4527,
	// 0x0a4a3920,
	// 0x0a4a3920,
	// 0x0a40311c,
	// 0x0a40311c,

	// 0x0a40311c,
	// 0x0a40311c,
	// 0x0a4a3920,
	// 0x0a4a3920,
	0x0a5a4527,
	0x0a5a4527,
	0x0a644d2c,
	0x0a644d2c,
	0x0a6e5530,
	0x0a6e5530,
	0x0a755a33,
	0x0a755a33,
	0x0a75643d,
	0x0a75643d,
	0x0a75643d,
	0x0a75643d,
	0x0a755a33,
	0x0a755a33,
	0x0a755a33,
	0x0a755a33,
	0x0a6e5530,
	0x0a6e5530,
	0x0a644d2c,
	0x0a644d2c,
	0x0a5a4527,
	0x0a5a4527,
	// 0x0a4a3920,
	// 0x0a4a3920,
	// 0x0a40311c,
	// 0x0a40311c,
		
};


/*
void _SetTypeItemColor(int nType, unsigned char &A, unsigned char &R, unsigned char &G, unsigned char &B, unsigned char ki = 0)
{
	unsigned char zColor = 255;

	
	zColor -= ki;

	switch(nType)
	{
		case blue_item:						// xanh
			A = B = zColor;
			R = G = 0;
			break;
		case purple_item:					//tim
			A = R = B = zColor;
			G = 0;
			break;
		case gold_item:						//vang
			A = 255 - ki;
			R = 255 - ki;
			G = 180 - ki;
			B = 0;
			break;
		default:
			break;
	}
	zColor = 0;
}*/

int vsColors[][4][3] = {
	{{0, 0, 0}, {0, 0, 0},{0, 0, 0}, {0, 0, 0}},
	{{100, 78, 107}, {66, 48, 74},{189, 40, 255}, {49, 24, 66}},
	{{80, 62, 0}, {82, 74, 42}, {243, 194, 70}, {97, 85, 47}},// vang dong vng 2
	// {{80, 62, 0}, {82, 74, 42}, {255, 239, 101}, {97, 85, 47}}, // vang nhat vng 1
	{{107, 103, 107}, {74, 73, 74},{247, 243, 247}, {74, 77, 74}},
};

int rgbColor[3] = {0,0,0};

int* get_VS_ColorRGB(int* fromColor, int* toColor, int maxFrame, int step)
{
	float stepRBG[3] = {0,0,0};
	stepRBG[0] = (float)(fromColor[0] - toColor[0]) / maxFrame;
	stepRBG[1] = (float)(fromColor[1] - toColor[1]) / maxFrame;
	stepRBG[2] = (float)(fromColor[2] - toColor[2]) / maxFrame;
	rgbColor[0] = (int)floor(fromColor[0] - (stepRBG[0] * step));
	rgbColor[1] = (int)floor(fromColor[1] - (stepRBG[1] * step));
	rgbColor[2] = (int)floor(fromColor[2] - (stepRBG[2] * step));
	return rgbColor;
}
//
void _Draw_Light(KRUPoint *zData,int zSizeLine,int &zPos,unsigned char &zDir,int nX,int nY,int nXE,int nYE,int nType)
{	
	int i, nTemp, nTemp1;
	nTemp = zSizeLine;


	for(i = 0;i < zSizeLine; i++)
	{
		// _SetTypeItemColor(nType, zData[i].Color.Color_b.a, zData[i].Color.Color_b.r,
			// zData[i].Color.Color_b.g, zData[i].Color.Color_b.b, i * nTempColor);
		if 	(nType == blue_item) 
		{
			nType = 0;
		}
		else if(nType == purple_item)
		{
			nType = 1;
		}
		else if(nType == gold_item)
		{
			nType = 2;
		}
		else
		{
			nType = 3;
		}
			
		nTemp1 = i;
		if(nTemp1 >= nTemp)
			nTemp1-=nTemp;
		int* getVSColor = get_VS_ColorRGB(vsColors[nType][2], vsColors[nType][3], nTemp, nTemp1);
		zData[i].Color.Color_b.r = getVSColor[0];
		zData[i].Color.Color_b.g = getVSColor[1];
		zData[i].Color.Color_b.b = getVSColor[2];
	}
	switch(zDir)
	{
		case  0:
			for(i = 0;i < zSizeLine;i++)
			{	
				nTemp = zPos - i;
				if(nTemp >= nX)			//dir 0
				{
					zData[i].oPosition.nX = nTemp;
					zData[i].oPosition.nY = nY;	
				}
				else 
				{
					nTemp1 = nY + nX - nTemp;	//pos end
					if(nTemp1 <= nYE)			//chua dat den nHeight cua dir 3
					{	
						zData[i].oPosition.nX = nX;
						zData[i].oPosition.nY = nTemp1;
					}
					else						//truong hop cuoi,khong qua phan nua rect
					{
						zData[i].oPosition.nX = nX + nTemp1 - nYE;
						zData[i].oPosition.nY = nYE;
							
					}

				}
			}
			break;
		case  1:
			for(i = 0;i < zSizeLine;i++)
			{
				nTemp = zPos - i;
				if(nTemp >= nY)			//dir 1
				{
					zData[i].oPosition.nY = nTemp;
					zData[i].oPosition.nX = nXE;	
				}
				else 
				{
					nTemp1 = nXE - (nY - nTemp);		//pos end
					if(nTemp1 >= nX)					//chua dat den nHeight cua dir 3
					{
						zData[i].oPosition.nX = nTemp1;
						zData[i].oPosition.nY = nY;
					}
					else								//truong hop cuoi,khong qua phan nua rect
					{
						zData[i].oPosition.nY = nY + nX - nTemp1;
						zData[i].oPosition.nX = nX;

					}

				}
			}
			break;
		case  2:
			for(i = 0;i < zSizeLine;i++)
			{
				nTemp = zPos + i;
				if(nTemp <= nXE)			//dir 2
				{
					zData[i].oPosition.nX = nTemp;
					zData[i].oPosition.nY = nYE;	
				}
				else 
				{
					nTemp1 = nYE - (nTemp - nXE);	//pos end
					if(nTemp1 >= nY)
					{
						zData[i].oPosition.nX = nXE;
						zData[i].oPosition.nY = nTemp1;
					}
					else							//truong hop cuoi,khong qua phan nua rect
					{
						zData[i].oPosition.nX = nXE - (nY - nTemp1);
						zData[i].oPosition.nY = nY;

					}

				}
			}
			break;
		case  3:
			for(i = 0;i < zSizeLine;i++)
			{	
				nTemp = zPos + i;
				if(nTemp <= nYE)		//dir 2
				{
					zData[i].oPosition.nX = nX;
					zData[i].oPosition.nY = nTemp;	
				}
				else 
				{
					nTemp1 = nX + (nTemp - nYE);//pos end
					if(nTemp1 <= nXE)
					{
						zData[i].oPosition.nX = nTemp1;
						zData[i].oPosition.nY = nYE;
					}
					else//truong hop cuoi,khong qua phan nua rect
					{
						zData[i].oPosition.nY = nYE - (nTemp1 - nXE);
						zData[i].oPosition.nX = nXE;

					}

				}
			}
			break;

		}
	g_pRepresentShell->DrawPrimitives(zSizeLine, zData, RU_T_POINT, true);
}

static int DLLMAXSIZELINE[16] = {
	50,2*50,3*50,4*50,5*50,6*50,7*50,8*50,
	9*50,10*50,11*50,12*50,13*50,14*50,15*50,16*50
};

void _SwapPos(int &zPos, unsigned char &zDir, int nX, int nY, int nWidth, int nHeight)
{	
		if (zDir == 0)
		{
			zDir = 2;
			zPos = 2 * nX + nWidth - zPos;
		}
		else if (zDir == 1)
		{
			zDir = 3;
			zPos = 2 * nY + nHeight - zPos;
		}
		else if (zDir == 2)
		{
			zDir = 0;
			zPos = 2 * nX + nWidth - zPos;
		}
		else if(zDir == 3)
		{
			zDir = 1;
			zPos = 2 * nY + nHeight - zPos;
		}
	
}

void DrawLight(int &zPos, unsigned char &zDir, unsigned char &zTimeDelay, int nX, int nY, int nWidth, int nHeight, int zSizeLine, int nType)
{	
	zTimeDelay += FRAMEGOITEMBACK;

	if(zTimeDelay >= FRAMEDELAYITEMBACK)
	{
		zTimeDelay -= FRAMEDELAYITEMBACK;
		if(zDir <= 1)
			zPos++;
		else
			zPos--;
	}

	if(zDir == 0)
	{
			if(zPos < nX)
				zPos = nX;
			else if(zPos >= nX + nWidth)
			{	
				zPos= nY + 1;
				zDir = 1;
			}
	}
	else if(zDir == 1)
	{
		if(zPos < nY)
			zPos = nY;
		else if(zPos >= nY + nHeight)
		{	
			zPos = nX + nWidth - 1;
			zDir = 2;
		}
	}
	else if(zDir == 2)
	{
		if(zPos > nX + nWidth)
			zPos = nX + nWidth;
		else if(zPos <= nX)
		{	
			zPos = nY + nHeight - 1;
			zDir = 3;
		}
	}
	else if(zDir == 3)
	{
		if(zPos > nY + nHeight)
			zPos = nY + nHeight;
		else if(zPos <= nY)
		{	
			zPos = nX + 1;
			zDir = 0;
		}
	}
	

	int nTempSize = nWidth + nHeight;
	if(zSizeLine > nTempSize || zSizeLine < 0)
		return;
	else if(zSizeLine <= DLLMAXSIZELINE[0])
	{
		KRUPoint nTemp[85];
		_Draw_Light(nTemp, DLLMAXSIZELINE[0], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
		_Draw_Light(nTemp, DLLMAXSIZELINE[0], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);

	}
	else if(zSizeLine <= DLLMAXSIZELINE[1])
	{
		KRUPoint nTemp[2*50];
		_Draw_Light(nTemp, DLLMAXSIZELINE[1], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
		_Draw_Light(nTemp, DLLMAXSIZELINE[1], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
	}
	else if(zSizeLine <= DLLMAXSIZELINE[2])
	{
		KRUPoint nTemp[3*50];
		_Draw_Light(nTemp, DLLMAXSIZELINE[2], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
		_Draw_Light(nTemp, DLLMAXSIZELINE[2], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);

	}
	else if(zSizeLine <= DLLMAXSIZELINE[3])
	{
		KRUPoint nTemp[4*50];
		_Draw_Light(nTemp, DLLMAXSIZELINE[3], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
		_Draw_Light(nTemp, DLLMAXSIZELINE[3], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
	}
	else if(zSizeLine <= DLLMAXSIZELINE[4])
	{
		KRUPoint nTemp[5*50];
		_Draw_Light(nTemp, DLLMAXSIZELINE[4], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
		_Draw_Light(nTemp, DLLMAXSIZELINE[4], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
	}
	else if(zSizeLine <= DLLMAXSIZELINE[5])
	{
		KRUPoint nTemp[6*50];
		_Draw_Light(nTemp, DLLMAXSIZELINE[5], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
		_Draw_Light(nTemp, DLLMAXSIZELINE[5], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);


	}
	else if(zSizeLine <= DLLMAXSIZELINE[6])
	{
		KRUPoint nTemp[7*50];
		_Draw_Light(nTemp, DLLMAXSIZELINE[6], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
		_Draw_Light(nTemp, DLLMAXSIZELINE[6], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
	}
	else if(zSizeLine <= DLLMAXSIZELINE[7])
	{
		KRUPoint nTemp[8*50];
		_Draw_Light(nTemp, DLLMAXSIZELINE[7], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
		_Draw_Light(nTemp, DLLMAXSIZELINE[7], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);

	}
	else if(zSizeLine <= DLLMAXSIZELINE[8])
	{
		KRUPoint nTemp[9*50];
		_Draw_Light(nTemp, DLLMAXSIZELINE[8], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
		_Draw_Light(nTemp, DLLMAXSIZELINE[8], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);

	}
	else if(zSizeLine <= DLLMAXSIZELINE[9])
	{
		KRUPoint nTemp[10*50];
		_Draw_Light(nTemp, DLLMAXSIZELINE[9], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
		_Draw_Light(nTemp, DLLMAXSIZELINE[9], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
	}
	else if(zSizeLine <= DLLMAXSIZELINE[10])
	{
		KRUPoint nTemp[11*50];
		_Draw_Light(nTemp, DLLMAXSIZELINE[10], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
		_Draw_Light(nTemp, DLLMAXSIZELINE[10], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);

	}
	else if(zSizeLine <= DLLMAXSIZELINE[11])
	{
		KRUPoint nTemp[12*50];
		_Draw_Light(nTemp, DLLMAXSIZELINE[11], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
		_Draw_Light(nTemp, DLLMAXSIZELINE[11], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);

	}
	else if(zSizeLine <= DLLMAXSIZELINE[12])
	{
		KRUPoint nTemp[13*50];
		_Draw_Light(nTemp, DLLMAXSIZELINE[12], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
		_Draw_Light(nTemp, DLLMAXSIZELINE[12], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);

	}
	else if(zSizeLine <= DLLMAXSIZELINE[13])
	{
		KRUPoint nTemp[14*50];
		_Draw_Light(nTemp, DLLMAXSIZELINE[13], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
		_Draw_Light(nTemp, DLLMAXSIZELINE[13], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
	}
	else if(zSizeLine <= DLLMAXSIZELINE[14])
	{
		KRUPoint nTemp[15*50];
		_Draw_Light(nTemp, DLLMAXSIZELINE[14], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
		_Draw_Light(nTemp, DLLMAXSIZELINE[14], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);

	}
	else if(zSizeLine <= DLLMAXSIZELINE[15])
	{
		KRUPoint nTemp[16*50];
		_Draw_Light(nTemp, DLLMAXSIZELINE[15], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);
		_Draw_Light(nTemp, DLLMAXSIZELINE[15], zPos, zDir, nX, nY, nX + nWidth, nY + nHeight,nType);
		_SwapPos(zPos, zDir, nX, nY, nWidth, nHeight);

	}

}

void DrawBorder(BOOL &bCountFrame, unsigned short &nFrame, unsigned long &ulTimeDelay, int nX, int nY, int nWidth, int nHeight, int nType /*= GoldItem*/)
{	
	KRURect		Rect;
	switch(nType)
	{
		case blue_item:
			Rect.Color.Color_dw = l_BlueBorder[nFrame];
			break;
		case purple_item:
			Rect.Color.Color_dw = l_PurpleBorder[nFrame];
			break;
		case gold_item:
			Rect.Color.Color_dw = l_GoldBorder[nFrame];
			break;
		default:
			return;
			break;
	}

	if(GetTickCount() -  ulTimeDelay >= 60)
	{	
		ulTimeDelay = GetTickCount();
		if(bCountFrame == 0)
		{
			nFrame++;
			if(nFrame >= 12)
				bCountFrame = 1;
		}
		else
		{
			nFrame--;
			if(nFrame <= 0)
				bCountFrame = 0;
		}
		
		switch(nType)
		{
			case blue_item:
				Rect.Color.Color_dw = l_BlueBorder[nFrame];
				break;
			case purple_item:
				Rect.Color.Color_dw = l_PurpleBorder[nFrame];
				break;
			case gold_item:
				Rect.Color.Color_dw = l_GoldBorder[nFrame];
				break;
			default:
				return;
				break;
		}
	}
	for(int i = 0; i < 2; i++)
	{	
		Rect.oPosition.nX = nX + i - 1;
		Rect.oPosition.nY = nY + i - 1;
		Rect.oEndPos.nX = nX + nWidth - i;
		Rect.oEndPos.nY = nY + nHeight - i;
		Rect.oEndPos.nZ = Rect.oPosition.nZ = 0;
		g_pRepresentShell->DrawPrimitives(1, &Rect, RU_T_RECT, true);
		
	}	
}
