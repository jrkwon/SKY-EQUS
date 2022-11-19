// SkySimul.h : main header file for the SKYSIMUL application
//

#if !defined(AFX_SKYSIMUL_H__F2091749_F361_488F_8D62_8F8BB727946F__INCLUDED_)
#define AFX_SKYSIMUL_H__F2091749_F361_488F_8D62_8F8BB727946F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSkySimulApp:
// See SkySimul.cpp for the implementation of this class
//

class CSkySimulApp : public CWinApp
{
public:
	CSkySimulApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkySimulApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSkySimulApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKYSIMUL_H__F2091749_F361_488F_8D62_8F8BB727946F__INCLUDED_)
