//-----------------------------------------------------------------------------
// File : NKVertexType.cpp
// Desc :
//-----------------------------------------------------------------------------

#include "StdAfx.h"
#include "NKVertexType.h"

//-----------------------------------------------------------------------------
// Static member variables
//-----------------------------------------------------------------------------
DWORD NKVERTEXTL::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
DWORD NKVERTEXL::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
DWORD NKVERTEXLT::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
DWORD NKVERTEXLT2::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2;
DWORD NKVERTEXT2::FVF = D3DFVF_XYZ | D3DFVF_TEX2;