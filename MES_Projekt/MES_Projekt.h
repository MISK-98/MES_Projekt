// MES_Projekt.h : main header file for the MES_Projekt application ----- wszystko jest ok
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMESProjektApp:
// See MES_Projekt.cpp for the implementation of this class
//

class CMESProjektApp : public CWinApp
{
public:
	CMESProjektApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMESProjektApp theApp;
