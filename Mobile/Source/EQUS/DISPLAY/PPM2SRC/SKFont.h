// SKFont.h : main header file for the SKFONT application
//

#if !defined(AFX_SKFONT_H__A6259EC5_F41C_11D1_8937_00600840863B__INCLUDED_)
#define AFX_SKFONT_H__A6259EC5_F41C_11D1_8937_00600840863B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSKFontApp:
// See SKFont.cpp for the implementation of this class
//

class CSKFontApp : public CWinApp
{
public:
	CSKFontApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSKFontApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSKFontApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKFONT_H__A6259EC5_F41C_11D1_8937_00600840863B__INCLUDED_)
