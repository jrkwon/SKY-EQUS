// GraphicsDlg.h : header file
//

#if !defined(AFX_GRAPHICSDLG_H__CDF0E66D_7FA6_4935_BBF4_80695F5625C5__INCLUDED_)
#define AFX_GRAPHICSDLG_H__CDF0E66D_7FA6_4935_BBF4_80695F5625C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ControllerDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CGraphicsDlg dialog

class CGraphicsDlg : public CDialog
{
// Construction
public:
	void DrawNUIGraphicBuffer();
	CGraphicsDlg(CWnd* pParent = NULL);	// standard constructor
    void GetRGB332(BYTE bRGB332, int& nR, int& nG, int &nB);
    void ButtonEnable(BOOL bEnable);
    void EnableOpenClose();

// Dialog Data
	//{{AFX_DATA(CGraphicsDlg)
	enum { IDD = IDD_GRAPHICS_DIALOG };
	CButton	m_btnColor;
	CButton	m_btnMagnify;
	CButton	m_btnRefresh;
	CButton	m_btnCls;
	CButton	m_btnOpen;
	CButton	m_btnClose;
	int		m_nRadioColors;
	int		m_nMagnify;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphicsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
    
    BOOL m_bOpen;
    //BOOL m_bDemoStart;
    CControllerDlg m_dlgController;
// make public
public:
	// Generated message map functions
	//{{AFX_MSG(CGraphicsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnBtnCls();
	afx_msg void OnBtnOpen();
	afx_msg void OnBtnClose();
	afx_msg void OnBtnRefresh();
	afx_msg void OnRdoMag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHICSDLG_H__CDF0E66D_7FA6_4935_BBF4_80695F5625C5__INCLUDED_)
