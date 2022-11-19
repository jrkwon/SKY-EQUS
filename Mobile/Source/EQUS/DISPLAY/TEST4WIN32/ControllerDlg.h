#if !defined(AFX_CONTROLLERDLG_H__339B9F67_481A_49BA_81E9_2DE604495973__INCLUDED_)
#define AFX_CONTROLLERDLG_H__339B9F67_481A_49BA_81E9_2DE604495973__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ControllerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CControllerDlg dialog

class CControllerDlg : public CDialog
{
// Construction
public:
	CControllerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CControllerDlg)
	enum { IDD = IDD_CONTROLLER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControllerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CControllerDlg)
	afx_msg void OnBtn256image();
	afx_msg void OnBtn4gimage();
	afx_msg void OnBtnPrimitive();
	afx_msg void OnBtnTextout();
	afx_msg void OnBtnDc();
	afx_msg void OnBtnGetputimage();
    afx_msg void OnBtnMultiLang();
	afx_msg void OnBtnOpenWindow();
    afx_msg void OnBtnCloseWindow();
    afx_msg void OnBtnFormatter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLLERDLG_H__339B9F67_481A_49BA_81E9_2DE604495973__INCLUDED_)
