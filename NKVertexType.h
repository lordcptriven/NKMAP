//-----------------------------------------------------------------------------
// File : NKVertexType.h
// Desc :
//-----------------------------------------------------------------------------

#ifndef _NKVERTEXTYPE_H_
#define _NKVERTEXTYPE_H_


#include "NKRTLib.h"


//-----------------------------------------------------------------------------
// Name : struct NKVERTEXTL
// Desc : Transformed and lit vertex
//        ( screen coordinates, diffuse color, 1 texture coordinate )
//-----------------------------------------------------------------------------
struct NKVERTEXTL
{
public:
	union {
		struct {
			D3DXVECTOR4	p;								// Transformed Position
			DWORD		c;								// Diffuse color
			float		u, v;							// Texture coordinate
		};
		struct {
			float		x, y, z, rhw;
			DWORD		c;
			float		u, v;
		};		
	};

	NKVERTEXTL();
	NKVERTEXTL( const D3DXVECTOR4& Pos, DWORD dwColor, float fu, float fv );
	NKVERTEXTL( float fx, float fy, float fz, float frhw, DWORD dwColor, float fu, float fv );

	void	Set( const D3DXVECTOR4& Pos, DWORD dwColor, float fu, float fv );
	void	Set( float fx, float fy, float fz, float frhw, DWORD dwColor, float fu, float fv );

	static DWORD	FVF;
};


//-----------------------------------------------------------------------------
// Name : struct NKVERTEXL
// Desc : Untransformed and lit vertex
//        ( position, diffuse color )
//-----------------------------------------------------------------------------
struct NKVERTEXL
{
public:
	union {
		struct {
			D3DXVECTOR3	p;
			DWORD		c;
		};
		struct {
			float		x, y, z;
			DWORD		c;
		};		
	};

	NKVERTEXL();
	NKVERTEXL( const D3DXVECTOR3& Pos,  DWORD dwColor);
	NKVERTEXL( float fx, float fy, float fz, DWORD dwColor);

	void	Set( const D3DXVECTOR3& Pos, DWORD dwColor);
	void	Set( float fx, float fy, float fz, DWORD dwColor);

	static DWORD	FVF;
};


//-----------------------------------------------------------------------------
// Name : struct NKVERTEXLT
// Desc : Untransformed and lit vertex with one texture coordinate
//        ( position, diffuse color, 1 texture coordinate )
//-----------------------------------------------------------------------------
struct NKVERTEXLT
{
public:
	union {
		struct {
			D3DXVECTOR3	p;
			DWORD		c;
			float		u, v;
		};
		struct {
			float		x, y, z;
			DWORD		c;
			float		u, v;
		};		
	};

	NKVERTEXLT();
	NKVERTEXLT( const D3DXVECTOR3& Pos,  DWORD dwColor, float fu, float fv );
	NKVERTEXLT( float fx, float fy, float fz, DWORD dwColor, float fu, float fv );

	void	Set( const D3DXVECTOR3& Pos, DWORD dwColor, float fu, float fv );
	void	Set( float fx, float fy, float fz, DWORD dwColor, float fu, float fv );

	static DWORD	FVF;
};


//-----------------------------------------------------------------------------
// Name : struct NKVERTEXLT2
// Desc : Untransformed and lit vertex with two texture coordinate
//        ( position, diffuse color, 2 texture coordinate )
//-----------------------------------------------------------------------------
struct NKVERTEXLT2
{
public:
	union {
		struct {
			D3DXVECTOR3	p;
			DWORD		c;
			float		u1, v1;
			float		u2, v2;
		};
		struct {
			float		x, y, z;
			DWORD		c;
			float		u1, v1;
			float		u2, v2;
		};		
	};

	NKVERTEXLT2();
	NKVERTEXLT2( const D3DXVECTOR3& Pos, DWORD dwColor, float fu1, float fv1, float fu2, float fv2 );
	NKVERTEXLT2( float fx, float fy, float fz, DWORD dwColor, float fu1, float fv1, float fu2, float fv2 );

	void	Set( const D3DXVECTOR3& Pos, DWORD dwColor, float fu1, float fv1, float fu2, float fv2 );
	void	Set( float fx, float fy, float fz, DWORD dwColor, float fu1, float fv1, float fu2, float fv2 );

	static DWORD	FVF;
};


//-----------------------------------------------------------------------------
// Name : struct NKVERTEXT2
// Desc : Untransformed and unlit vertex with two texture coordinate
//        ( position, 2 texture coordinate )
//-----------------------------------------------------------------------------
struct NKVERTEXT2
{
public:
	union {
		struct {
			D3DXVECTOR3	p;
			float		u1, v1;
			float		u2, v2;
		};
		struct {
			float		x, y, z;
			float		u1, v1;
			float		u2, v2;
		};		
	};

	NKVERTEXT2();
	NKVERTEXT2( const D3DXVECTOR3& Pos, float fu1, float fv1, float fu2, float fv2 );
	NKVERTEXT2( float fx, float fy, float fz, float fu1, float fv1, float fu2, float fv2 );

	void	Set( const D3DXVECTOR3& Pos, float fu1, float fv1, float fu2, float fv2 );
	void	Set( float fx, float fy, float fz, float fu1, float fv1, float fu2, float fv2 );

	static DWORD	FVF;
};


// Inline functions
#include "NKVertexType.inl"

#endif // _NKVERTEXTYPE_H_
