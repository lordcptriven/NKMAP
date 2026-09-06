//-----------------------------------------------------------------------------
// File : NKVertexType.inl
// Desc :
//-----------------------------------------------------------------------------


//- NKVERTEXTL --------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : NKVERTEXTL()
// Desc : constructor
//-----------------------------------------------------------------------------
inline NKVERTEXTL::NKVERTEXTL()
{
}


//-----------------------------------------------------------------------------
// Name : NKVERTEXTL()
// Desc : constructor
//-----------------------------------------------------------------------------
inline NKVERTEXTL::NKVERTEXTL( const D3DXVECTOR4& Pos, DWORD dwColor, float fu, float fv )
{
	p = Pos;	c = dwColor;	u = fu;	v = fv;
}


//-----------------------------------------------------------------------------
// Name : NKVERTEXTL()
// Desc : constructor
//-----------------------------------------------------------------------------
inline NKVERTEXTL::NKVERTEXTL( float fx, float fy, float fz, float frhw, DWORD dwColor, float fu, float fv )
{
	x = fx;		y = fy;		z = fz;		rhw = frhw;		c = dwColor;	u = fu;		v = fv;
}


//-----------------------------------------------------------------------------
// Name : Set()
// Desc :
//-----------------------------------------------------------------------------
inline void NKVERTEXTL::Set( const D3DXVECTOR4& Pos, DWORD dwColor, float fu, float fv )
{
	p = Pos;	c = dwColor;	u = fu;	v = fv;
}


//-----------------------------------------------------------------------------
// Name : Set()
// Desc :
//-----------------------------------------------------------------------------
inline void NKVERTEXTL::Set( float fx, float fy, float fz, float frhw, DWORD dwColor, float fu, float fv )
{
	x = fx;		y = fy;		z = fz;		rhw = frhw;		c = dwColor;	u = fu;		v = fv;
}



//- NKVERTEXL ---------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : NKVERTEXL()
// Desc : constructor
//-----------------------------------------------------------------------------
inline NKVERTEXL::NKVERTEXL()
{
}


//-----------------------------------------------------------------------------
// Name : NKVERTEXL()
// Desc : constructor
//-----------------------------------------------------------------------------
inline NKVERTEXL::NKVERTEXL( const D3DXVECTOR3& Pos,  DWORD dwColor )
{
	p = Pos;	c = dwColor;
}


//-----------------------------------------------------------------------------
// Name : NKVERTEXL()
// Desc : constructor
//-----------------------------------------------------------------------------
inline NKVERTEXL::NKVERTEXL( float fx, float fy, float fz, DWORD dwColor )
{
	x = fx;		y = fy;		z = fz;		c = dwColor;
}


//-----------------------------------------------------------------------------
// Name : Set()
// Desc :
//-----------------------------------------------------------------------------
inline void NKVERTEXL::Set( const D3DXVECTOR3& Pos, DWORD dwColor )
{
	p = Pos;	c = dwColor;
}


//-----------------------------------------------------------------------------
// Name : Set()
// Desc :
//-----------------------------------------------------------------------------
inline void NKVERTEXL::Set( float fx, float fy, float fz, DWORD dwColor )
{
	x = fx;		y = fy;		z = fz;		c = dwColor;
}



//- NKVERTEXLT --------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : NKVERTEXLT()
// Desc : constructor
//-----------------------------------------------------------------------------
inline NKVERTEXLT::NKVERTEXLT()
{
}


//-----------------------------------------------------------------------------
// Name : NKVERTEXLT()
// Desc : constructor
//-----------------------------------------------------------------------------
inline NKVERTEXLT::NKVERTEXLT( const D3DXVECTOR3& Pos,  DWORD dwColor, float fu, float fv )
{
	p = Pos;	c = dwColor;	u = fu;		v = fv;
}


//-----------------------------------------------------------------------------
// Name : NKVERTEXLT()
// Desc : constructor
//-----------------------------------------------------------------------------
inline NKVERTEXLT::NKVERTEXLT( float fx, float fy, float fz, DWORD dwColor, float fu, float fv )
{
	x = fx;		y = fy;		z = fz;		c = dwColor;	u = fu;		v = fv;
}


//-----------------------------------------------------------------------------
// Name : Set()
// Desc :
//-----------------------------------------------------------------------------
inline void NKVERTEXLT::Set( const D3DXVECTOR3& Pos, DWORD dwColor, float fu, float fv )
{
	p = Pos;	c = dwColor;	u = fu;		v = fv;
}


//-----------------------------------------------------------------------------
// Name : Set()
// Desc :
//-----------------------------------------------------------------------------
inline void NKVERTEXLT::Set( float fx, float fy, float fz, DWORD dwColor, float fu, float fv )
{
	x = fx;		y = fy;		z = fz;		c = dwColor;	u = fu;		v = fv;
}



//- NKVERTEXT2 -------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : NKVERTEXLT2()
// Desc : constructor
//-----------------------------------------------------------------------------
inline NKVERTEXLT2::NKVERTEXLT2()
{
}


//-----------------------------------------------------------------------------
// Name : NKVERTEXLT2()
// Desc : constructor
//-----------------------------------------------------------------------------
inline NKVERTEXLT2::NKVERTEXLT2( const D3DXVECTOR3& Pos, DWORD dwColor,
									float fu1, float fv1, float fu2, float fv2 )
{
	p = Pos;
	c = dwColor;
	u1 = fu1;	v1 = fv1;
	u2 = fu2;	v2 = fv2;
}


//-----------------------------------------------------------------------------
// Name : NKVERTEXLT2()
// Desc : constructor
//-----------------------------------------------------------------------------
inline NKVERTEXLT2::NKVERTEXLT2( float fx, float fy, float fz, DWORD dwColor,
									float fu1, float fv1, float fu2, float fv2 )
{
	x = fx;		y = fy;		z = fz;
	c = dwColor;
	u1 = fu1;	v1 = fv1;
	u2 = fu2;	v2 = fv2;
}


//-----------------------------------------------------------------------------
// Name : Set()
// Desc :
//-----------------------------------------------------------------------------
inline void NKVERTEXLT2::Set( const D3DXVECTOR3& Pos, DWORD dwColor,
								float fu1, float fv1, float fu2, float fv2 )
{
	p = Pos;
	c = dwColor;
	u1 = fu1;	v1 = fv1;
	u2 = fu2;	v2 = fv2;
}


//-----------------------------------------------------------------------------
// Name : Set()
// Desc :
//-----------------------------------------------------------------------------
inline void NKVERTEXLT2::Set( float fx, float fy, float fz, DWORD dwColor,
								float fu1, float fv1, float fu2, float fv2 )
{
	x = fx;		y = fy;		z = fz;
	c = dwColor;
	u1 = fu1;	v1 = fv1;
	u2 = fu2;	v2 = fv2;
}



//- NKVERTEXT2 -------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : NKVERTEXT2()
// Desc : constructor
//-----------------------------------------------------------------------------
inline NKVERTEXT2::NKVERTEXT2()
{
}


//-----------------------------------------------------------------------------
// Name : NKVERTEXLT2()
// Desc : constructor
//-----------------------------------------------------------------------------
inline NKVERTEXT2::NKVERTEXT2( const D3DXVECTOR3& Pos, float fu1, float fv1, float fu2, float fv2 )
{
	p = Pos;
	u1 = fu1;	v1 = fv1;
	u2 = fu2;	v2 = fv2;
}


//-----------------------------------------------------------------------------
// Name : NKVERTEXT2()
// Desc : constructor
//-----------------------------------------------------------------------------
inline NKVERTEXT2::NKVERTEXT2( float fx, float fy, float fz, float fu1, float fv1, float fu2, float fv2 )
{
	x = fx;		y = fy;		z = fz;
	u1 = fu1;	v1 = fv1;
	u2 = fu2;	v2 = fv2;
}


//-----------------------------------------------------------------------------
// Name : Set()
// Desc :
//-----------------------------------------------------------------------------
inline void NKVERTEXT2::Set( const D3DXVECTOR3& Pos, float fu1, float fv1, float fu2, float fv2 )
{
	p = Pos;
	u1 = fu1;	v1 = fv1;
	u2 = fu2;	v2 = fv2;
}


//-----------------------------------------------------------------------------
// Name : Set()
// Desc :
//-----------------------------------------------------------------------------
inline void NKVERTEXT2::Set( float fx, float fy, float fz, float fu1, float fv1, float fu2, float fv2 )
{
	x = fx;		y = fy;		z = fz;
	u1 = fu1;	v1 = fv1;
	u2 = fu2;	v2 = fv2;
}