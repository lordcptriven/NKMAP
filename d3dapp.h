//-----------------------------------------------------------------------------
// File: D3DApp.h
//
// Desc: Application class for the Direct3D samples framework library.
//
// Copyright (c) 1998-1999 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef  D3DAPP_H
#define  D3DAPP_H
#define  D3D_OVERLOADS
#include <d3d.h>
#include "D3DFrame.h"
#include "D3DEnum.h"
#include "D3DUtil.h"
#include "D3DRes.h"




//-----------------------------------------------------------------------------
// Name: class CD3DApplication
// Desc:
//-----------------------------------------------------------------------------
class CD3DApplication
{
protected:
    // Internal variables and member functions
    CD3DFramework7* m_pFramework;
    BOOL            m_bActive;
    BOOL            m_bReady;
	BOOL            m_bFrameMoving;
	FLOAT			m_fTime;					// Current time in seconds
	FLOAT			m_fFPS;						// Instanteous frame rate
	FLOAT			m_fElapsedTime;				// Time elapsed since last frame

    HRESULT Initialize3DEnvironment();
    HRESULT Change3DEnvironment();

    VOID    Cleanup3DEnvironment();
    VOID    DisplayFrameworkError( HRESULT, DWORD );

protected:
    HWND                 m_hWnd;
    D3DEnum_DeviceInfo*  m_pDeviceInfo;
    LPDIRECTDRAW7        m_pDD;
    LPDIRECT3D7          m_pD3D;
    LPDIRECT3DDEVICE7    m_pd3dDevice;
    LPDIRECTDRAWSURFACE7 m_pddsRenderTarget;
    DDSURFACEDESC2       m_ddsdRenderTarget;

    // Overridable variables for the app
    TCHAR*               m_strWindowTitle;
    BOOL                 m_bAppUseZBuffer;
    BOOL                 m_bShowStats;
    HRESULT              (*m_fnConfirmDevice)(DDCAPS*, D3DDEVICEDESC7*);

	DWORD				 m_dwClientWidth;				// Width of a client screen
	DWORD				 m_dwClientHeight;				// Height of a client screen

    // Overridable functions for the 3D scene created by the app
    virtual HRESULT OneTimeSceneInit()     { return S_OK; }
    virtual HRESULT InitDeviceObjects()    { return S_OK; }
    virtual HRESULT DeleteDeviceObjects()  { return S_OK; }
    virtual HRESULT Render()               { return S_OK; }
    virtual HRESULT FrameMove()            { return S_OK; }
    virtual HRESULT RestoreSurfaces()      { return S_OK; }
    virtual HRESULT FinalCleanup()         { return S_OK; }

    // Overridable power management (APM) functions
    virtual LRESULT OnQuerySuspend( DWORD dwFlags );
    virtual LRESULT OnResumeSuspend( DWORD dwData );

    // Miscellaneous functions
    VOID    OutputText( DWORD x, DWORD y, TCHAR* str );

public:
    // Functions to create, run, pause, and clean up the application
    virtual HRESULT Create( HWND hWnd );
    virtual LRESULT MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );
    virtual VOID    Pause( BOOL bPause );
	HRESULT Render3DEnvironment();

    // Class constructor
    CD3DApplication();
};




#endif // D3DAPP_H


