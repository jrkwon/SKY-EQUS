// SkySimulDlg.h : header file
//

#if !defined(AFX_SKYSIMULDLG_H__BC349C11_8935_4495_BAE9_476F7369DE64__INCLUDED_)
#define AFX_SKYSIMULDLG_H__BC349C11_8935_4495_BAE9_476F7369DE64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSkySimulDlg dialog

class CSkySimulDlg : public CDialog
{
// Construction
public:
	void GetRGB332(BYTE bRGB332, int& nR, int& nG, int &nB);
    void GetRGB565(WORD wRGB565, int &nR, int &nG, int &nB);
	void DrawEQUSBuffer();
	void DrawEQUSBufferRect(int xleft, int  ytop, int xright, int ybottom);

	CSkySimulDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSkySimulDlg)
	enum { IDD = IDD_SKYSIMUL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkySimulDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSkySimulDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
    afx_msg void OnBtnKeypad(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKYSIMULDLG_H__BC349C11_8935_4495_BAE9_476F7369DE64__INCLUDED_)
