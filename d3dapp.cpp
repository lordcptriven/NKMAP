//-----------------------------------------------------------------------------
// File: D3DApp.cpp
//
// Desc: Application class for the Direct3D samples framework library.
//
// Copyright (c) 1998-1999 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#define STRICT
#include "StdAfx.h"
#include <windows.h>
#include <pbt.h>
#include <mmsystem.h>
#include <stdio.h>
#include <tchar.h>
#include "D3DApp.h"




//-----------------------------------------------------------------------------
// Internal function prototypes and variables
//-----------------------------------------------------------------------------
enum APPMSGTYPE { MSG_NONE, MSGERR_APPMUSTEXIT, MSGWARN_SWITCHEDTOSOFTWARE };

static CD3DApplication* g_pD3DApp;


//-----------------------------------------------------------------------------
// Timer command codes
//
// NK_TIMER_RESET           : reset the timer
// NK_TIMER_START           : start the timer
// NK_TIMER_STOP            : stop the timer
// NK_TIMER_GETABSOLUTETIME : get the absolute system time
// NK_TIMER_GETAPPTIME      : get the current time
// NK_TIMER_GETELAPSEDTIME  : get the time that elapsed between
//                            NK_TIMER_GETELAPSEDTIME calls
//-----------------------------------------------------------------------------
enum NK_TIMER_COMMAND { NK_TIMER_RESET, NK_TIMER_START, NK_TIMER_STOP,
						NK_TIMER_GETABSOLUTETIME, NK_TIMER_GETAPPTIME,
						NK_TIMER_GETELAPSEDTIME };

// Function for the timer
FLOAT	Timer( NK_TIMER_COMMAND command );


//-----------------------------------------------------------------------------
// Name: CD3DApplication()
// Desc: 
//-----------------------------------------------------------------------------
CD3DApplication::CD3DApplication()
{
    m_pFramework   = NULL;
    m_hWnd         = NULL;
    m_pDD          = NULL;
    m_pD3D         = NULL;
    m_pd3dDevice   = NULL;

    m_pddsRenderTarget     = NULL;

    m_bActive         = FALSE;
    m_bReady          = FALSE;
    m_bFrameMoving    = TRUE;
	
	m_fFPS = 0.0f;
	m_fElapsedTime = 0.0f;

    m_strWindowTitle  = _T("Direct3D Application");
    m_bAppUseZBuffer  = FALSE;
    m_bShowStats      = FALSE;
    m_fnConfirmDevice = NULL;

    g_pD3DApp = this;

	Pause( TRUE );
}




//-----------------------------------------------------------------------------
// Name: Create()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Create( HWND hWnd )
{
    HRESULT hr;

    // Enumerate available D3D devices. The callback is used so the app can
    // confirm/reject each enumerated device depending on its capabilities.
    if( FAILED( hr = D3DEnum_EnumerateDevices( m_fnConfirmDevice ) ) )
    {
        DisplayFrameworkError( hr, MSGERR_APPMUSTEXIT );
        return hr;
    }

    // Select a device. Ask for a hardware device that renders in a window.
    if( FAILED( hr = D3DEnum_SelectDefaultDevice( &m_pDeviceInfo ) ) )
    {
        DisplayFrameworkError( hr, MSGERR_APPMUSTEXIT );
        return hr;
    }

    // Initialize the app's custom scene stuff
    if( FAILED( hr = OneTimeSceneInit() ) )
    {
        DisplayFrameworkError( hr, MSGERR_APPMUSTEXIT );
        return hr;
    }

    // Create a new CD3DFramework class. This class does all of our D3D
    // initialization and manages the common D3D objects.
    if( NULL == ( m_pFramework = new CD3DFramework7() ) )
    {
        DisplayFrameworkError( E_OUTOFMEMORY, MSGERR_APPMUSTEXIT );
        return E_OUTOFMEMORY;
    }

    // Set the render window
	m_hWnd = hWnd;

    // Initialize the 3D environment for the app
    if( FAILED( hr = Initialize3DEnvironment() ) )
    {
        DisplayFrameworkError( hr, MSGERR_APPMUSTEXIT );
        Cleanup3DEnvironment();
        return E_FAIL;
    }

    // Initialize the application timer
    Timer( NK_TIMER_START );

    // The app is ready to go
    m_bReady = TRUE;

	Pause( FALSE );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Message handling function.
//-----------------------------------------------------------------------------
LRESULT CD3DApplication::MsgProc( UINT uMsg, WPARAM wParam,
                                  LPARAM lParam )
{
    HRESULT hr;

    switch( uMsg )
    {
        case WM_PAINT:
            // Handle paint messages when the app is not ready
            if( m_pFramework && !m_bReady )
            {
                if( m_pDeviceInfo->bWindowed )
                    m_pFramework->ShowFrame();
                else
                    m_pFramework->FlipToGDISurface( TRUE );
            }
            break;

        case WM_MOVE:
            // If in windowed mode, move the Framework's window
            if( m_pFramework && m_bActive && m_bReady && m_pDeviceInfo->bWindowed )
                m_pFramework->Move( (SHORT)LOWORD(lParam), (SHORT)HIWORD(lParam) );
            break;

        case WM_SIZE:
            // Check to see if we are losing our window...
            if( SIZE_MAXHIDE==wParam || SIZE_MINIMIZED==wParam )
                m_bActive = FALSE;
            else
                m_bActive = TRUE;

            // A new window size will require a new backbuffer
            // size, so the 3D structures must be changed accordingly.
            if( m_bActive && m_bReady && m_pDeviceInfo->bWindowed )
            {
				Pause( TRUE );

                if( FAILED( hr = Change3DEnvironment() ) )
                    return 0;

				Pause( FALSE );
            }
            break;

        case WM_ENTERMENULOOP:
            // Pause the app when menus are displayed
            Pause( TRUE );
            break;

        case WM_EXITMENULOOP:
            Pause( FALSE );
            break;

        case WM_ENTERSIZEMOVE:
            // Halt frame movement while the app is sizing or moving
            Pause( TRUE );
            break;

        case WM_EXITSIZEMOVE:
            Pause( FALSE );
            break;

        case WM_POWERBROADCAST:
            switch( wParam )
            {
                case PBT_APMQUERYSUSPEND:
                    // At this point, the app should save any data for open
                    // network connections, files, etc.., and prepare to go into
                    // a suspended mode.
                    return OnQuerySuspend( (DWORD)lParam );

                case PBT_APMRESUMESUSPEND:
                    // At this point, the app should recover any data, network
                    // connections, files, etc.., and resume running from when
                    // the app was suspended.
                    return OnResumeSuspend( (DWORD)lParam );
            }
            break;

        case WM_DESTROY:
            Cleanup3DEnvironment();
            PostQuitMessage(0);
            return 0;
    }

	return 0;
}
            



//-----------------------------------------------------------------------------
// Name: Initialize3DEnvironment()
// Desc: Initializes the sample framework, then calls the app-specific function
//       to initialize device specific objects. This code is structured to
//       handled any errors that may occur duing initialization
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Initialize3DEnvironment()
{
    HRESULT hr;
    DWORD   dwFrameworkFlags = 0L;
    dwFrameworkFlags |= ( !m_pDeviceInfo->bWindowed ? D3DFW_FULLSCREEN : 0L );
    dwFrameworkFlags |= (  m_pDeviceInfo->bStereo   ? D3DFW_STEREO     : 0L );
    dwFrameworkFlags |= (  m_bAppUseZBuffer         ? D3DFW_ZBUFFER    : 0L );

    // Initialize the D3D framework
    if( SUCCEEDED( hr = m_pFramework->Initialize( m_hWnd,
                     m_pDeviceInfo->pDriverGUID, m_pDeviceInfo->pDeviceGUID,
                     &m_pDeviceInfo->ddsdFullscreenMode, dwFrameworkFlags ) ) )
    {
        m_pDD        = m_pFramework->GetDirectDraw();
        m_pD3D       = m_pFramework->GetDirect3D();
        m_pd3dDevice = m_pFramework->GetD3DDevice();

        m_pddsRenderTarget     = m_pFramework->GetRenderSurface();

        m_ddsdRenderTarget.dwSize = sizeof(m_ddsdRenderTarget);
        m_pddsRenderTarget->GetSurfaceDesc( &m_ddsdRenderTarget );

		m_dwClientWidth = m_ddsdRenderTarget.dwWidth;
		m_dwClientHeight = m_ddsdRenderTarget.dwHeight;

        // Let the app run its startup code which creates the 3d scene.
        if( SUCCEEDED( hr = InitDeviceObjects() ) )
            return S_OK;
        else
        {
            DeleteDeviceObjects();
            m_pFramework->DestroyObjects();
        }
    }

    // If we get here, the first initialization passed failed. If that was with a
    // hardware device, try again using a software rasterizer instead.
    if( m_pDeviceInfo->bHardware )
    {
        // Try again with a software rasterizer
        DisplayFrameworkError( hr, MSGWARN_SWITCHEDTOSOFTWARE );
        D3DEnum_SelectDefaultDevice( &m_pDeviceInfo, D3DENUM_SOFTWAREONLY );
        return Initialize3DEnvironment();
    }

    return hr;
}




//-----------------------------------------------------------------------------
// Name: Change3DEnvironment()
// Desc: Handles driver, device, and/or mode changes for the app.
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Change3DEnvironment()
{
    HRESULT hr;
    static BOOL  bOldWindowedState = TRUE;
    static DWORD dwSavedStyle;
    static RECT  rcSaved;

    // Release all scene objects that will be re-created for the new device
    DeleteDeviceObjects();

    // Release framework objects, so a new device can be created
    if( FAILED( hr = m_pFramework->DestroyObjects() ) )
    {
        DisplayFrameworkError( hr, MSGERR_APPMUSTEXIT );
        SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
        return hr;
    }

    // Check if going from fullscreen to windowed mode, or vice versa.
    if( bOldWindowedState != m_pDeviceInfo->bWindowed )
    {
        if( m_pDeviceInfo->bWindowed )
        {
            // Coming from fullscreen mode, so restore window properties
            SetWindowLong( m_hWnd, GWL_STYLE, dwSavedStyle );
            SetWindowPos( m_hWnd, HWND_NOTOPMOST, rcSaved.left, rcSaved.top,
                          ( rcSaved.right - rcSaved.left ), 
                          ( rcSaved.bottom - rcSaved.top ), SWP_SHOWWINDOW );
        }
        else
        {
            // Going to fullscreen mode, save/set window properties as needed
            dwSavedStyle = GetWindowLong( m_hWnd, GWL_STYLE );
            GetWindowRect( m_hWnd, &rcSaved );
            SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE );
        }

        bOldWindowedState = m_pDeviceInfo->bWindowed;
    }

    // Inform the framework class of the driver change. It will internally
    // re-create valid surfaces, a d3ddevice, etc.
    if( FAILED( hr = Initialize3DEnvironment() ) )
    {
        DisplayFrameworkError( hr, MSGERR_APPMUSTEXIT );
        SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
        return hr;
    }

    // If the app is paused, trigger the rendering of the current frame
    if( FALSE == m_bFrameMoving )
    {
        Timer( NK_TIMER_START );
		Timer( NK_TIMER_STOP );
    }
    
    return S_OK;
}





//-----------------------------------------------------------------------------
// Name: Render3DEnvironment()
// Desc: Draws the scene.
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Render3DEnvironment()
{
    HRESULT hr;

    // Check the cooperative level before rendering
    if( FAILED( hr = m_pDD->TestCooperativeLevel() ) )
    {
        switch( hr )
        {
            case DDERR_EXCLUSIVEMODEALREADYSET:
            case DDERR_NOEXCLUSIVEMODE:
                // Do nothing because some other app has exclusive mode
                return S_OK;

            case DDERR_WRONGMODE:
                // The display mode changed on us. Resize accordingly
                if( m_pDeviceInfo->bWindowed )
                    return Change3DEnvironment();
                break;
        }
        return hr;
    }

    // Get the app's time, in seconds. Skip rendering if no time elapsed
	FLOAT	fAppTime = Timer( NK_TIMER_GETAPPTIME );
	FLOAT	fElapsedAppTime = Timer( NK_TIMER_GETELAPSEDTIME );
    if( ( 0.0f == fElapsedAppTime ) && m_bFrameMoving )
        return S_OK;

    // FrameMove (animate) the scene
    if( m_bFrameMoving )
    {
		// Store the time for the app
		m_fTime = fAppTime;
		m_fElapsedTime = fElapsedAppTime;

		// Frame move the scene
        if( FAILED( hr = FrameMove() ) )
            return hr;
    }

    // Render the scene as normal
    if( FAILED( hr = Render() ) )
        return hr;

    // Keep track of the frame count
    static FLOAT	fLastTime = 0.0f;
    static DWORD	dwFrames = 0;
    FLOAT	fTime = Timer( NK_TIMER_GETABSOLUTETIME );
    ++dwFrames;

    // Update the frame rate once per second
    if( fTime - fLastTime > 1.0f )
    {
        m_fFPS = dwFrames / (fTime - fLastTime);
        fLastTime = fTime;
        dwFrames  = 0L;
    }

    // Show the frame on the primary surface.
    if( FAILED( hr = m_pFramework->ShowFrame() ) )
    {
        if( DDERR_SURFACELOST != hr )
            return hr;

        m_pFramework->RestoreSurfaces();
        RestoreSurfaces();
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Cleanup3DEnvironment()
// Desc: Cleanup scene objects
//-----------------------------------------------------------------------------
VOID CD3DApplication::Cleanup3DEnvironment()
{
    m_bActive = FALSE;
    m_bReady  = FALSE;

    if( m_pFramework )
    {
        DeleteDeviceObjects();
        SAFE_DELETE( m_pFramework );

        FinalCleanup();
    }

    D3DEnum_FreeResources();
}




//-----------------------------------------------------------------------------
// Name: Pause()
// Desc: Called in to toggle the pause state of the app. This function
//       brings the GDI surface to the front of the display, so drawing
//       output like message boxes and menus may be displayed.
//-----------------------------------------------------------------------------
VOID CD3DApplication::Pause( BOOL bPause )
{
	static DWORD	dwAppPausedCount = 0;

	dwAppPausedCount += (bPause ? +1 : -1 );
	m_bReady = (dwAppPausedCount ? false : true );

	// Handle the first pause request (of many, nestable pause requests)
	if( bPause && ( 1 == dwAppPausedCount ) )
	{
		// Get a surface for the GDI
		if( m_pFramework )
			m_pFramework->FlipToGDISurface( TRUE );

		// Stop the scene from animating
		if( m_bFrameMoving )
			Timer( NK_TIMER_STOP );
	}

	if( 0 == dwAppPausedCount )
	{
		// Restart the timers
		if( m_bFrameMoving )
			Timer( NK_TIMER_START );
	}
}




//-----------------------------------------------------------------------------
// Name: OnQuerySuspend()
// Desc: Called when the app receives a PBT_APMQUERYSUSPEND message, meaning
//       the computer is about to be suspended. At this point, the app should
//       save any data for open network connections, files, etc.., and prepare
//       to go into a suspended mode.
//-----------------------------------------------------------------------------
LRESULT CD3DApplication::OnQuerySuspend( DWORD dwFlags )
{
    Pause( TRUE );

    return TRUE;
}




//-----------------------------------------------------------------------------
// Name: OnResumeSuspend()
// Desc: Called when the app receives a PBT_APMRESUMESUSPEND message, meaning
//       the computer has just resumed from a suspended state. At this point, 
//       the app should recover any data, network connections, files, etc..,
//       and resume running from when the app was suspended.
//-----------------------------------------------------------------------------
LRESULT CD3DApplication::OnResumeSuspend( DWORD dwData )
{
    Pause( FALSE );

    return TRUE;
}



//-----------------------------------------------------------------------------
// Name: OutputText()
// Desc: Draws text on the window.
//-----------------------------------------------------------------------------
VOID CD3DApplication::OutputText( DWORD x, DWORD y, TCHAR* str )
{
    HDC hDC;

    // Get a DC for the surface. Then, write out the buffer
    if( m_pddsRenderTarget )
    {
        if( SUCCEEDED( m_pddsRenderTarget->GetDC(&hDC) ) )
        {
            SetTextColor( hDC, RGB(255,255,0) );
            SetBkMode( hDC, TRANSPARENT );
            ExtTextOut( hDC, x, y, 0, NULL, str, lstrlen(str), NULL );
            m_pddsRenderTarget->ReleaseDC(hDC);
        }
    }
}




//-----------------------------------------------------------------------------
// Name: DisplayFrameworkError()
// Desc: Displays error messages in a message box
//-----------------------------------------------------------------------------
VOID CD3DApplication::DisplayFrameworkError( HRESULT hr, DWORD dwType )
{
    TCHAR strMsg[512];

    switch( hr )
    {
        case D3DENUMERR_NODIRECTDRAW:
            lstrcpy( strMsg, _T("Could not create DirectDraw!") );
            break;
        case D3DENUMERR_NOCOMPATIBLEDEVICES:
            lstrcpy( strMsg, _T("Could not find any compatible Direct3D\n"
                     "devices.") );
            break;
        case D3DENUMERR_SUGGESTREFRAST:
            lstrcpy( strMsg, _T("Could not find any compatible devices.\n\n"
                     "Try enabling the reference rasterizer using\n"
                     "EnableRefRast.reg.") );
            break;
        case D3DENUMERR_ENUMERATIONFAILED:
            lstrcpy( strMsg, _T("Enumeration failed. Your system may be in an\n"
                     "unstable state and need to be rebooted") );
            break;
        case D3DFWERR_INITIALIZATIONFAILED:
            lstrcpy( strMsg, _T("Generic initialization error.\n\nEnable "
                     "debug output for detailed information.") );
            break;
        case D3DFWERR_NODIRECTDRAW:
            lstrcpy( strMsg, _T("No DirectDraw") );
            break;
        case D3DFWERR_NODIRECT3D:
            lstrcpy( strMsg, _T("No Direct3D") );
            break;
        case D3DFWERR_INVALIDMODE:
            lstrcpy( strMsg, _T("This sample requires a 16-bit (or higher) "
                                "display mode\nto run in a window.\n\nPlease "
                                "switch your desktop settings accordingly.") );
            break;
        case D3DFWERR_COULDNTSETCOOPLEVEL:
            lstrcpy( strMsg, _T("Could not set Cooperative Level") );
            break;
        case D3DFWERR_NO3DDEVICE:
            lstrcpy( strMsg, _T("Could not create the Direct3DDevice object.") );
            
            if( MSGWARN_SWITCHEDTOSOFTWARE == dwType )
                lstrcat( strMsg, _T("\nThe 3D hardware chipset may not support"
                                    "\nrendering in the current display mode.") );
            break;
        case D3DFWERR_NOZBUFFER:
            lstrcpy( strMsg, _T("No ZBuffer") );
            break;
        case D3DFWERR_INVALIDZBUFFERDEPTH:
            lstrcpy( strMsg, _T("Invalid Z-buffer depth. Try switching modes\n"
                     "from 16- to 32-bit (or vice versa)") );
            break;
        case D3DFWERR_NOVIEWPORT:
            lstrcpy( strMsg, _T("No Viewport") );
            break;
        case D3DFWERR_NOPRIMARY:
            lstrcpy( strMsg, _T("No primary") );
            break;
        case D3DFWERR_NOCLIPPER:
            lstrcpy( strMsg, _T("No Clipper") );
            break;
        case D3DFWERR_BADDISPLAYMODE:
            lstrcpy( strMsg, _T("Bad display mode") );
            break;
        case D3DFWERR_NOBACKBUFFER:
            lstrcpy( strMsg, _T("No backbuffer") );
            break;
        case D3DFWERR_NONZEROREFCOUNT:
            lstrcpy( strMsg, _T("A DDraw object has a non-zero reference\n"
                     "count (meaning it was not properly cleaned up)." ) );
            break;
        case D3DFWERR_NORENDERTARGET:
            lstrcpy( strMsg, _T("No render target") );
            break;
        case E_OUTOFMEMORY:
            lstrcpy( strMsg, _T("Not enough memory!") );
            break;
        case DDERR_OUTOFVIDEOMEMORY:
            lstrcpy( strMsg, _T("There was insufficient video memory "
                     "to use the\nhardware device.") );
            break;
        default:
            lstrcpy( strMsg, _T("Generic application error.\n\nEnable "
                     "debug output for detailed information.") );
    }

    if( MSGERR_APPMUSTEXIT == dwType )
    {
        lstrcat( strMsg, _T("\n\nThis sample will now exit.") );
        MessageBox( NULL, strMsg, m_strWindowTitle, MB_ICONERROR|MB_OK );
    }
    else
    {
        if( MSGWARN_SWITCHEDTOSOFTWARE == dwType )
            lstrcat( strMsg, _T("\n\nSwitching to software rasterizer.") );
        MessageBox( NULL, strMsg, m_strWindowTitle, MB_ICONWARNING|MB_OK );
    }
}



//-----------------------------------------------------------------------------
// Name : Timer()
// Desc : Performs timer operations
//-----------------------------------------------------------------------------
FLOAT Timer( NK_TIMER_COMMAND command )
{
	static BOOL ms_bTimerInitialized = FALSE;
	static BOOL ms_bUsingQPF = FALSE;
	static BOOL ms_bTimerStopped = TRUE;
	static LONGLONG ms_llQPFTicksPerSec = 0;

	// Initialize the timer
	if( FALSE == ms_bTimerInitialized )
	{
		ms_bTimerInitialized = TRUE;

		// Use QueryPerformanceFrequency() to get frequency of timer.  If QPF is
		// not supported, we will timeGetTime() which returns milliseconds.
		LARGE_INTEGER qwTicksPerSec;
		ms_bUsingQPF = QueryPerformanceFrequency( &qwTicksPerSec );
		if( ms_bUsingQPF )
			ms_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
	}

	if( ms_bUsingQPF )
	{
		static LONGLONG ms_llStopTime = 0;
		static LONGLONG ms_llLastElapsedTime = 0;
		static LONGLONG ms_llBaseTime = 0;
		double fTime;
		double fElapsedTime;
		LARGE_INTEGER qwTime;
        
		// Get either the current time or the stop time, depending
		// on whether we're stopped and what command was sent
		if( ms_llStopTime != 0 && command != NK_TIMER_START && command != NK_TIMER_GETABSOLUTETIME)
			qwTime.QuadPart = ms_llStopTime;
		else
			QueryPerformanceCounter( &qwTime );

		// Return the elapsed time
		if( command == NK_TIMER_GETELAPSEDTIME )
		{
			fElapsedTime = (double) ( qwTime.QuadPart - ms_llLastElapsedTime ) / (double) ms_llQPFTicksPerSec;
			ms_llLastElapsedTime = qwTime.QuadPart;
			return (FLOAT) fElapsedTime;
		}
    
		// Return the current time
		if( command == NK_TIMER_GETAPPTIME )
		{
			double fAppTime = (double) ( qwTime.QuadPart - ms_llBaseTime ) / (double) ms_llQPFTicksPerSec;
			return (FLOAT) fAppTime;
		}

		// Get the absolute system time
		if( command == NK_TIMER_GETABSOLUTETIME )
		{
			fTime = qwTime.QuadPart / (double) ms_llQPFTicksPerSec;
			return (FLOAT) fTime;
		}
    
		// Reset the timer
		if( command == NK_TIMER_RESET )
		{
			ms_llBaseTime = qwTime.QuadPart;
			ms_llLastElapsedTime = qwTime.QuadPart;
			ms_llStopTime = 0;
			ms_bTimerStopped = FALSE;
			return 0.0f;
		}
    
		// Start the timer
		if( command == NK_TIMER_START )
		{
			if( ms_bTimerStopped )
				ms_llBaseTime += qwTime.QuadPart - ms_llStopTime;
			ms_llStopTime = 0;
			ms_llLastElapsedTime = qwTime.QuadPart;
			ms_bTimerStopped = FALSE;
			return 0.0f;
		}
    
		// Stop the timer
		if( command == NK_TIMER_STOP )
		{
			ms_llStopTime = qwTime.QuadPart;
			ms_llLastElapsedTime = qwTime.QuadPart;
			ms_bTimerStopped = TRUE;
			return 0.0f;
		}
    
        return -1.0f; // Invalid command specified
	}
	else
	{
		// Get the time using timeGetTime()
		static double ms_fLastElapsedTime = 0.0;
		static double ms_fBaseTime = 0.0;
		static double ms_fStopTime = 0.0;
		double fTime;
		double fElapsedTime;
        
		// Get either the current time or the stop time, depending
		// on whether we're stopped and what command was sent
		if( ms_fStopTime != 0.0 && command != NK_TIMER_START && command != NK_TIMER_GETABSOLUTETIME)
			fTime = ms_fStopTime;
		else
			fTime = timeGetTime() * 0.001;
    
		// Return the elapsed time
		if( command == NK_TIMER_GETELAPSEDTIME )
		{   
			fElapsedTime = (double) (fTime - ms_fLastElapsedTime);
			ms_fLastElapsedTime = fTime;
			return (FLOAT) fElapsedTime;
		}
    
		// Return the current time
		if( command == NK_TIMER_GETAPPTIME )
		{
			return (FLOAT) (fTime - ms_fBaseTime);
		}
    
		// Get the absolute system time
		if( command == NK_TIMER_GETABSOLUTETIME )
		{
			return (FLOAT) fTime;
		}

		// Reset the timer
		if( command == NK_TIMER_RESET )
		{
			ms_fBaseTime = fTime;
			ms_fLastElapsedTime = fTime;
			ms_fStopTime = 0;
			ms_bTimerStopped = FALSE;
			return 0.0f;
		}
    
		// Start the timer
		if( command == NK_TIMER_START )
		{
			if( ms_bTimerStopped )
				ms_fBaseTime += fTime - ms_fStopTime;
			ms_fStopTime = 0.0f;
			ms_fLastElapsedTime = fTime;
			ms_bTimerStopped = FALSE;
			return 0.0f;
		}
    
		// Stop the timer
		if( command == NK_TIMER_STOP )
		{
			ms_fStopTime = fTime;
			ms_fLastElapsedTime = fTime;
			ms_bTimerStopped = TRUE;
			return 0.0f;
		}
    
		return -1.0f; // Invalid command specified
	}
}