/*****************************************************************************************
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-25
// --
	提供对图形资源引用的维护操作。
*****************************************************************************************/
#pragma once
#include "../../../Represent/iRepresent/KRepresentUnit.h"

//======================================
//		图形对象的引用结构
//======================================
struct KUiImageRef : public KRUImage
{
	int			nNumFrames;		//图形对象的图形帧数目
	int			nInterval;		//帧间隔
	int			nFlipTime;		//当前帧切换出来的时间
};

#define	IDF_LEFT_TO_RIGHT	0
#define IDF_RIGHT_TO_LEFT	1
#define IDF_TOP_TO_BOTTOM	2
#define IDF_BOTTOM_TO_TOP	3

//======================================
//		图形对象的引用结构
//======================================
struct KUiImagePartRef : public KRUImagePart
{
	int			Width;			//图横宽
	int			Height;			//图纵宽
	int			nDivideFashion;	//分割方式,取值为IDF_*之一
};

void	IR_InitUiImageRef(KUiImageRef& Img);
void	IR_InitUiImagePartRef(KUiImagePartRef& Img);
void	IR_UpdateTime();
int		IR_NextFrame(KUiImageRef& Img);
void	IR_GetReferenceSpot(KUiImageRef& Img, int& h, int& v);
void	IR_UpdateImagePart(KUiImagePartRef& Img, int nPartValue, int nFullValue);
int		IR_IsTimePassed(unsigned int uInterval, unsigned int& uLastTimer);
unsigned int IR_GetRemainTime(unsigned int uInterval, unsigned int uLastTimer);
unsigned int IR_GetCurrentTime();
int		IR_GetCurFrame(KUiImageRef& Img);
int		IR_GetNumFrames(KUiImageRef& Img);

