#include "StdAfx.h"
#include "NkCharacter.h"
#include "GUtil.h"
#include "D3DMath.h"
#include <math.h>

float DistPlToPl(CNkCharacter *pCha1, CNkCharacter *pCha2)
{
	return sqrtf( ((pCha1->m_wx - pCha2->m_wx)*(pCha1->m_wx - pCha2->m_wx) + (pCha1->m_wz - pCha2->m_wz)*(pCha1->m_wz - pCha2->m_wz)));
}

float DistPlToPl(float from_x, float from_z, float to_x, float to_z)
{
	return sqrtf( (from_x - to_x)*(from_x - to_x) + (from_z-to_z)*(from_z-to_z) );
}

float DistPlToPlAbs(float from_x, float from_z, float to_x, float to_z)
{
	return (float) ( fabs(from_x-to_x)+fabs(from_z-to_z) );
}

void  GetDirectionElement(float radDir, float &x, float &z)
{
	float mag;			// w 는 밑변, h는 높이

	if (g_PI_DIV_2 - radDir <=g_EPSILON) {
		x = 0.0f; 
		z = 1.0f;
		return;
	} else if (g_PI - radDir <= g_EPSILON) {
		x = -1.0f;
		z = 0.0f;
		return;
	} else if (g_PI + g_PI_DIV_2 - radDir <= g_EPSILON) {
		x = 0.0f;
		z = -1.0f;
		return;
	} else if (D3DMath_IsZero(radDir)) {
		x = 1.0f;
		z = 0.0f;
		return;
	} else
		;
		
	x = 1.0f;
	z = tanf(radDir);

	mag = sqrtf(x*x+z*z);
	x = x / mag;
	z = z / mag;

	radDir = fmodf(radDir, g_2_PI);

	if (radDir < 0.0f)
		radDir += g_2_PI;

	// 어느 사분면에 있는지 알아본다.
	// 위의 계산값은 180도 이하에서의 계산값이다.
	if (radDir > 0.0f && radDir <= g_PI_DIV_2) {
		return;
	} else if (radDir > g_PI_DIV_2 && radDir < g_PI) {		// 결과는 음수, x 는 음수, z는 양수
		x = 0.0f - x;
		z = 0.0f - z;
		return;
	} else if (radDir > g_PI && radDir < (g_PI+g_PI_DIV_2)) {	// 둘다 음수가 되어야 한다.
		x = 0.0f - x;
		z = 0.0f - z;
	} else
		;
}