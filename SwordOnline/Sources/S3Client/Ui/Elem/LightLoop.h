#ifndef LIGHTLOOP_H_
#define LIGHTLOOP_H_
#pragma once


#include "../../Represent/iRepresent/KRepresentUnit.h"

enum eTypeItem
{
	blue_item = 0,
	purple_item,
	gold_item,
	num_item,
};

void DrawLight(int &zPos, unsigned char &zDir, unsigned char &zTimeDelay, int nX, int nY, int nWidth, int nHeight, int zSizeLine = 52, int nType = blue_item);
void DrawBorder(BOOL &bCountFrame, unsigned short &nFrame, unsigned long &ulTimeDelay, int nX, int nY, int nWidth, int nHeight, int nType = blue_item);
#endif
