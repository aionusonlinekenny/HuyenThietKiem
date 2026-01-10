// --
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KRandom.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Random Number Generate Functions
// --
#include "KWin32.h"
#include "KRandom.h"
// --
#define IM 139968
#define IA 3877
#define IC 29573
// --
static UINT nRandomSeed = 42;
// --
//
// --
void g_RandomSeed(UINT nSeed)
{
	nRandomSeed = nSeed;
}
// --
//
// --
UINT g_Random(UINT nMax)
{
	if (nMax)
	{
		nRandomSeed = nRandomSeed * IA + IC;
		return nRandomSeed % nMax;
	}
	else
	{
		return 0;
	}
}
// --
//
// --
UINT g_GetRandomSeed()
{
	return nRandomSeed;
}
