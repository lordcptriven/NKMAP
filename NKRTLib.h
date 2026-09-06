//-----------------------------------------------------------------------------
// File : NKRTLib.h
// Desc : Real-time library for the NAKO Application
//-----------------------------------------------------------------------------

#ifndef _NKRTLIB_H_
#define _NKRTLIB_H_


#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <tchar.h>
#include <math.h>
#include <ddraw.h>
#include <d3d.h>
#include <d3dxcore.h>
#include <d3dxmath.h>

#pragma warning ( disable : 4786 )

#include <vector>
#include <list>
#include <map>
#include <algorithm>

using namespace std;


//-----------------------------------------------------------------------------
// Miscellaneous helper macros
//-----------------------------------------------------------------------------
#define	SAFE_DELETE(p)			{ if(p) { delete (p);		(p)=NULL; } }
#define	SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);		(p)=NULL; } }
#define	SAFE_RELEASE(p)			{ if(p) { (p)->Release();	(p)=NULL; } }


#endif // _NKRTLIB_H_