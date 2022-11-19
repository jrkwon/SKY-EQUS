// ControllerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Graphics.h"
#include "ControllerDlg.h"
#include "GraphicsDlg.h"
#include "testgrph.h"
#include "testmult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControllerDlg dialog


CControllerDlg::CControllerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CControllerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CControllerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CControllerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CControllerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CControllerDlg, CDialog)
	//{{AFX_MSG_MAP(CControllerDlg)
	ON_BN_CLICKED(IDC_BTN_256IMAGE, OnBtn256image)
	ON_BN_CLICKED(IDC_BTN_4GIMAGE, OnBtn4gimage)
	ON_BN_CLICKED(IDC_BTN_PRIMITIVE, OnBtnPrimitive)
	ON_BN_CLICKED(IDC_BTN_TEXTOUT, OnBtnTextout)
	ON_BN_CLICKED(IDC_BTN_DC, OnBtnDc)
	ON_BN_CLICKED(IDC_BTN_GETPUTIMAGE, OnBtnGetputimage)
	ON_BN_CLICKED(IDC_BTN_MULTILANG, OnBtnMultiLang)
	ON_BN_CLICKED(IDC_BTN_OPEN_WINDOW, OnBtnOpenWindow)
	ON_BN_CLICKED(IDC_BTN_CLOSE_WINDOW, OnBtnCloseWindow)
	ON_BN_CLICKED(IDC_BTN_FORMATTER, OnBtnFormatter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControllerDlg message handlers

void CControllerDlg::OnBtn256image() 
{
	// TODO: Add your control notification handler code here
    CWnd* pWnd = GetParent();
    ((CGraphicsDlg*)pWnd)->OnBtnCls();
    NUIGraphicsDemo256Image();
    ((CGraphicsDlg*)pWnd)->DrawNUIGraphicBuffer();
}

void CControllerDlg::OnBtn4gimage() 
{
	// TODO: Add your control notification handler code here
    CWnd* pWnd = GetParent();
    ((CGraphicsDlg*)pWnd)->OnBtnCls();
    NUIGraphicsDemo4GImage();
    ((CGraphicsDlg*)pWnd)->DrawNUIGraphicBuffer();
}

void CControllerDlg::OnBtnPrimitive() 
{
	// TODO: Add your control notification handler code here
    CWnd* pWnd = GetParent();
    ((CGraphicsDlg*)pWnd)->OnBtnCls();
    NUIGraphicsDemoPrimitive();
    ((CGraphicsDlg*)pWnd)->DrawNUIGraphicBuffer();
}

void CControllerDlg::OnBtnTextout() 
{
	// TODO: Add your control notification handler code here
    CWnd* pWnd = GetParent();
    ((CGraphicsDlg*)pWnd)->OnBtnCls();
    NUIGraphicsDemoTextOut();
    ((CGraphicsDlg*)pWnd)->DrawNUIGraphicBuffer();
}

void CControllerDlg::OnBtnDc() 
{
	// TODO: Add your control notification handler code here
    CWnd* pWnd = GetParent();
    ((CGraphicsDlg*)pWnd)->OnBtnCls();
    NUIGraphicsDemoDC();
    ((CGraphicsDlg*)pWnd)->DrawNUIGraphicBuffer();
}

void CControllerDlg::OnBtnGetputimage() 
{
	// TODO: Add your control notification handler code here
    CWnd* pWnd = GetParent();
    ((CGraphicsDlg*)pWnd)->OnBtnCls();
    NUIGraphicsDemoGetPutImage();
    ((CGraphicsDlg*)pWnd)->DrawNUIGraphicBuffer();
}

void CControllerDlg::OnBtnMultiLang() 
{
	// TODO: Add your control notification handler code here
    CWnd* pWnd = GetParent();
    ((CGraphicsDlg*)pWnd)->OnBtnCls();
    TestMultiLang();
    ((CGraphicsDlg*)pWnd)->DrawNUIGraphicBuffer();
}

void CControllerDlg::OnBtnOpenWindow() 
{
	// TODO: Add your control notification handler code here
    CWnd* pWnd = GetParent();
    ((CGraphicsDlg*)pWnd)->OnBtnCls();
    TestOpenWindow();
    ((CGraphicsDlg*)pWnd)->DrawNUIGraphicBuffer();
}

void CControllerDlg::OnBtnCloseWindow() 
{
	// TODO: Add your control notification handler code here
    CWnd* pWnd = GetParent();
    ((CGraphicsDlg*)pWnd)->OnBtnCls();
    TestCloseWindow();
    ((CGraphicsDlg*)pWnd)->DrawNUIGraphicBuffer();
}

void CControllerDlg::OnBtnFormatter() 
{
	// TODO: Add your control notification handler code here
    CWnd* pWnd = GetParent();
    ((CGraphicsDlg*)pWnd)->OnBtnCls();
    TestFormatter();
    ((CGraphicsDlg*)pWnd)->DrawNUIGraphicBuffer();
}
