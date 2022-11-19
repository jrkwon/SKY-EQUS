// SKFontDlg.h : header file
//

#if !defined(AFX_SKFONTDLG_H__A6259EC7_F41C_11D1_8937_00600840863B__INCLUDED_)
#define AFX_SKFONTDLG_H__A6259EC7_F41C_11D1_8937_00600840863B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CSKFontDlg dialog

class CSKFontDlg : public CDialog
{
// Construction
public:
	CSKFontDlg(CWnd* pParent = NULL);	// standard constructor

	BOOL ReadHex(FILE* fp, BYTE& data);
	BOOL ReadWordHex(FILE* fp, WORD& data);
    BOOL IsFontBorder(int nRGB);
    BYTE MakeRGB332(int nR, int nG, int nB);


// Dialog Data
	//{{AFX_DATA(CSKFontDlg)
	enum { IDD = IDD_SKFONT_DIALOG };
	CProgressCtrl	m_wndProg;
	int		m_nColSize;
	int		m_nRowSize;
	CString	m_strImagePath;
	CString	m_strSrcPath;
	int		m_nCharSize;
	DWORD	m_lMaxImgCol;
	int		m_nRadio;
	int		m_nBoundaryColor;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSKFontDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSKFontDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSetImagePath();
	afx_msg void OnSetSrcPath();
	afx_msg void OnImageToSource();
	afx_msg void OnImageToAnisrc();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKFONTDLG_H__A6259EC7_F41C_11D1_8937_00600840863B__INCLUDED_)
