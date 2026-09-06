# Microsoft Developer Studio Project File - Name="NKMapTool" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=NKMapTool - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NKMapTool.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NKMapTool.mak" CFG="NKMapTool - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NKMapTool - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "NKMapTool - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NKMapTool - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 dxguid.lib ddraw.lib d3dx.lib winmm.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "NKMapTool - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dxguid.lib ddraw.lib d3dx.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /incremental:no

!ENDIF 

# Begin Target

# Name "NKMapTool - Win32 Release"
# Name "NKMapTool - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CapSyong.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dapp.cpp
# End Source File
# Begin Source File

SOURCE=.\d3denum.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dframe.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dmath.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dutil.cpp
# End Source File
# Begin Source File

SOURCE=.\DecoObj.cpp
# End Source File
# Begin Source File

SOURCE=.\FileMergeMaps.cpp
# End Source File
# Begin Source File

SOURCE=.\FileNewTerrainDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\IndexedTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\LightCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFileDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MyNKApp.cpp
# End Source File
# Begin Source File

SOURCE=.\NKCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\NkCharacter.cpp
# End Source File
# Begin Source File

SOURCE=.\NKMapTool.cpp
# End Source File
# Begin Source File

SOURCE=.\NKMapTool.rc
# End Source File
# Begin Source File

SOURCE=.\NKMapToolDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\NKMapToolView.cpp
# End Source File
# Begin Source File

SOURCE=.\NKMath.cpp
# End Source File
# Begin Source File

SOURCE=.\NKObjQuadNode.cpp
# End Source File
# Begin Source File

SOURCE=.\NKTerQuadNode.cpp
# End Source File
# Begin Source File

SOURCE=.\NKTerrain.cpp
# End Source File
# Begin Source File

SOURCE=.\NKVertexType.cpp
# End Source File
# Begin Source File

SOURCE=.\PageCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ShadowRect.cpp
# End Source File
# Begin Source File

SOURCE=.\SizeCBar.cpp
# End Source File
# Begin Source File

SOURCE=.\SkyPlane.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ToolAtmosEditPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolAttrEditPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolCtrlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolLightEditPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolMapMoveDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolObjectRemoveRegion.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolObjEditPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolTexPaintListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolTexPaintPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolVertEditPage.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm"
# Begin Source File

SOURCE=.\CapSyong.h
# End Source File
# Begin Source File

SOURCE=.\ChaSta.h
# End Source File
# Begin Source File

SOURCE=.\Config.h
# End Source File
# Begin Source File

SOURCE=.\d3dapp.h
# End Source File
# Begin Source File

SOURCE=.\d3denum.h
# End Source File
# Begin Source File

SOURCE=.\d3dframe.h
# End Source File
# Begin Source File

SOURCE=.\d3dmath.h
# End Source File
# Begin Source File

SOURCE=.\d3dres.h
# End Source File
# Begin Source File

SOURCE=.\d3dutil.h
# End Source File
# Begin Source File

SOURCE=.\DecoObj.h
# End Source File
# Begin Source File

SOURCE=.\FileMergeMaps.h
# End Source File
# Begin Source File

SOURCE=.\FileNewTerrainDlg.h
# End Source File
# Begin Source File

SOURCE=.\GUtil.h
# End Source File
# Begin Source File

SOURCE=.\IndexedTexture.h
# End Source File
# Begin Source File

SOURCE=.\LightCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MyFileDialog.h
# End Source File
# Begin Source File

SOURCE=.\MyNKApp.h
# End Source File
# Begin Source File

SOURCE=.\NKCamera.h
# End Source File
# Begin Source File

SOURCE=.\NkCharacter.h
# End Source File
# Begin Source File

SOURCE=.\NKMapTool.h
# End Source File
# Begin Source File

SOURCE=.\NKMapToolDoc.h
# End Source File
# Begin Source File

SOURCE=.\NKMapToolView.h
# End Source File
# Begin Source File

SOURCE=.\NKMath.h
# End Source File
# Begin Source File

SOURCE=.\NKObjQuadNode.h
# End Source File
# Begin Source File

SOURCE=.\NKRTLib.h
# End Source File
# Begin Source File

SOURCE=.\NKTerQuadNode.h
# End Source File
# Begin Source File

SOURCE=.\NKTerrain.h
# End Source File
# Begin Source File

SOURCE=.\NKVertexType.h
# End Source File
# Begin Source File

SOURCE=.\PageCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ShadowRect.h
# End Source File
# Begin Source File

SOURCE=.\SizeCBar.h
# End Source File
# Begin Source File

SOURCE=.\SkyPlane.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ToolAtmosEditPage.h
# End Source File
# Begin Source File

SOURCE=.\ToolAttrEditPage.h
# End Source File
# Begin Source File

SOURCE=.\ToolCtrlBar.h
# End Source File
# Begin Source File

SOURCE=.\ToolLightEditPage.h
# End Source File
# Begin Source File

SOURCE=.\ToolMapMoveDlg.h
# End Source File
# Begin Source File

SOURCE=.\ToolObjectRemoveRegion.h
# End Source File
# Begin Source File

SOURCE=.\ToolObjEditPage.h
# End Source File
# Begin Source File

SOURCE=.\ToolTexPaintListBox.h
# End Source File
# Begin Source File

SOURCE=.\ToolTexPaintPage.h
# End Source File
# Begin Source File

SOURCE=.\ToolVertEditPage.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "inl"
# Begin Source File

SOURCE=.\MyNKApp.inl
# End Source File
# Begin Source File

SOURCE=.\NKCamera.inl
# End Source File
# Begin Source File

SOURCE=.\NKMath.inl
# End Source File
# Begin Source File

SOURCE=.\NKObjQuadNode.inl
# End Source File
# Begin Source File

SOURCE=.\NKTerQuadNode.inl
# End Source File
# Begin Source File

SOURCE=.\NKTerrain.inl
# End Source File
# Begin Source File

SOURCE=.\NKVertexType.inl
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\file_nop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\nako_ico.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NKMapTool.ico
# End Source File
# Begin Source File

SOURCE=.\res\NKMapTool.rc2
# End Source File
# Begin Source File

SOURCE=.\res\NKMapToolDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\tool_align.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_array.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_atm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_att.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_bru.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_era.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_era_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_era_on_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_era_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_fla.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_fla_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_hid.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_lay1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_lay1_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_lay2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_lay2_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_lay3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_lay3_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_lig.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_lig_control.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_lig_control_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_low.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_low_all.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_low_all_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_low_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_mov.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_mov_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_move.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_move_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_obj.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_pai.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_pai_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_pai_on_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_pai_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_pen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_pla.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_pla_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_rai.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_rai_all.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_rai_all_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_rai_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_rot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_rot_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_sel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_sel_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_sho.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_smo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_smo_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_tex.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_til.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_til_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_ver.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_vert.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_vert_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_wat.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_wat_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_wav.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_wav_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\UNDO_Object.cpp
# End Source File
# Begin Source File

SOURCE=.\Undo_Object.h
# End Source File
# End Target
# End Project
