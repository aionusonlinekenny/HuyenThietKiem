#ifndef __COREDRAWGAMEOBJ__H__
#define __COREDRAWGAMEOBJ__H__

struct KLightInfo;

void	CoreDrawGameObj(unsigned int uObjGenre, unsigned int uId, int x, int y, int Width, int Height, int nParam);
void	CoreGetGameObjLightInfo(unsigned int uObjGenre, unsigned int uId, KLightInfo *pLightInfo);

#endif //#ifndef __COREDRAWGAMEOBJ__H__