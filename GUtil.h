#ifndef __GUTIL_H__
#define __GUTIL_H__

class CNkCharacter;

float DistPlToPl(CNkCharacter *pCha1, CNkCharacter *pCha2);		// 플레이어간에 거리를 리턴한다.
float DistPlToPl(float from_x, float from_z, float to_x, float to_z);
float DistPlToPlAbs(float from_x, float from_z, float to_x, float to_z);
void  GetDirectionElement(float radDir, float &x, float &z);
#endif