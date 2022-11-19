// GraphicsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Graphics.h"
#include "GraphicsDlg.h"
#include "testgrph.h"
#include "ControllerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* NUI screen definition */
#define SCREEN_POS_X    7
#define SCREEN_POS_Y    7
#define MAX_MAG         4

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphicsDlg dialog

CGraphicsDlg::CGraphicsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGraphicsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGraphicsDlg)
	m_nRadioColors = 0;
	m_nMagnify = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_bOpen = FALSE;
}

void CGraphicsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGraphicsDlg)
	DDX_Control(pDX, IDC_RDO_COLOR, m_btnColor);
	DDX_Control(pDX, IDC_RDO_MAG, m_btnMagnify);
	DDX_Control(pDX, IDC_BTN_REFRESH, m_btnRefresh);
	DDX_Control(pDX, IDC_BTN_CLS, m_btnCls);
	DDX_Control(pDX, IDC_BTN_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
	DDX_Radio(pDX, IDC_RDO_COLOR, m_nRadioColors);
	DDX_Radio(pDX, IDC_RDO_MAG, m_nMagnify);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGraphicsDlg, CDialog)
	//{{AFX_MSG_MAP(CGraphicsDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_CLS, OnBtnCls)
	ON_BN_CLICKED(IDC_BTN_OPEN, OnBtnOpen)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_BN_CLICKED(IDC_BTN_REFRESH, OnBtnRefresh)
	ON_BN_CLICKED(IDC_RDO_MAG, OnRdoMag)
	ON_BN_CLICKED(IDC_RDO_MAG3, OnRdoMag)
	ON_BN_CLICKED(IDC_RDO_MAG2, OnRdoMag)
	ON_BN_CLICKED(IDC_RDO_MAG4, OnRdoMag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphicsDlg message handlers

BOOL CGraphicsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
    m_btnOpen.EnableWindow();
    m_btnClose.EnableWindow(FALSE);
    ButtonEnable(FALSE);

    //m_dlgController.Create(IDD_CONTROLLER);
    //m_dlgController.ShowWindow(SW_HIDE);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGraphicsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGraphicsDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
        if(m_bOpen)
            DrawNUIGraphicBuffer();

        CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGraphicsDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGraphicsDlg::DrawNUIGraphicBuffer()
{
	CClientDC dc(this); // device context for painting
    //CDC* pDC = GetDC();
    BYTE* pBuffer = GetLCDBuffer();
    COLORREF Color;
    BYTE bColor;
    BYTE bMask[] = { 0xC0, 0x30, 0x0C, 0x03 };
    INT nBufSize;

    CDC MemDC;
    MemDC.CreateCompatibleDC(&dc);
    CBitmap BmpDC;
    BmpDC.CreateCompatibleBitmap(&dc, SCREEN_WIDTH, SCREEN_HEIGHT);
    CBitmap* pOldBmp = MemDC.SelectObject(&BmpDC);

    UpdateData();
    if(m_nRadioColors == 0) /* 4gray */
    {
        COLORREF paColorTable[3][4] = {
            { RGB(0, 0, 0), RGB(128, 128, 128), RGB(192, 192, 192), RGB(255, 255, 255) },
            { RGB(123, 140, 90), RGB(181, 193, 157), RGB(86, 100, 58), RGB(0, 0, 0) },
            { RGB(0, 0, 0), RGB(128, 128, 128), RGB(192, 192, 192), RGB(255, 255, 255) }
        };
        /* get buffer size */
        nBufSize = SCREEN_WIDTH/8 * SCREEN_HEIGHT * 2;
        INT nMaxBit[] = { 6,4,2,0 };
        for(int i = 0; i < nBufSize; i++, pBuffer++)
        {
            for(int j = 0; j < 4; j++)
            {
                bColor = *pBuffer & bMask[j];
                bColor >>= nMaxBit[j];

                POINT Point;
                Point.x = (i*4 + j) % SCREEN_WIDTH;
                Point.y = (i*4 + j) / SCREEN_WIDTH;
                //dc.SetPixel(Point, Color);
                MemDC.SetPixel(Point, paColorTable[1][bColor]);
            }
        }
    }
    else /* 256 colors */
    {
        int nR, nG, nB; /* RGB format is 3,3,2 */
        /* get buffer size */
        nBufSize = SCREEN_WIDTH * SCREEN_HEIGHT;
        for(int i = 0; i < nBufSize; i++, pBuffer++)
        {
            bColor = *pBuffer;
            GetRGB332(bColor, nR, nG, nB);
            Color = RGB(nR, nG, nB);
            POINT Point;
            Point.x = (i) % SCREEN_WIDTH;
            Point.y = (i) / SCREEN_WIDTH;
            //dc.SetPixel(Point, Color);
            MemDC.SetPixel(Point, Color);
        }
    }
    /* clear the area */
    CRect rect(SCREEN_POS_X, SCREEN_POS_Y, 
                 SCREEN_WIDTH*MAX_MAG+SCREEN_POS_X, 
                 SCREEN_POS_Y+SCREEN_HEIGHT*MAX_MAG);
 	CBrush br;
	br.CreateStockObject(DKGRAY_BRUSH);
    dc.FillRect(rect, &br);
    //dc.Rectangle(SCREEN_POS_X, SCREEN_POS_Y, 
    //             SCREEN_WIDTH*MAX_MAG+SCREEN_POS_X, 
    //             SCREEN_POS_Y+SCREEN_HEIGHT*MAX_MAG);

    //Copy the unmodified bitmap to the temporary bitmap
    dc.StretchBlt(SCREEN_POS_X+(SCREEN_WIDTH*MAX_MAG-SCREEN_WIDTH*(m_nMagnify+1))/2, 
                  SCREEN_POS_Y+(SCREEN_HEIGHT*MAX_MAG-SCREEN_HEIGHT*(m_nMagnify+1))/2, 
                  SCREEN_WIDTH*(m_nMagnify+1), 
                  SCREEN_HEIGHT*(m_nMagnify+1), 
                  &MemDC, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SRCCOPY);
    
    MemDC.SelectObject(pOldBmp);
    MemDC.DeleteDC();
}

void CGraphicsDlg::GetRGB332(BYTE bRGB332, int& nR, int& nG, int &nB)
{
    nR = ((bRGB332 & 0xE0) >> 5) * 32;
    nG = ((bRGB332 & 0x1C) >> 2) * 32;
    nB = (bRGB332 & 0x03) * 64;
}

void CGraphicsDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
    if(m_bOpen)
        CloseNUIGraphics();

    //m_dlgController.DestroyWindow();
	CDialog::OnClose();
}

void CGraphicsDlg::OnBtnCls() 
{
	// TODO: Add your control notification handler code here
    if(!m_bOpen)
        return;

    ClearNUIScreen();	
    DrawNUIGraphicBuffer();
    //m_bDemoStart = FALSE;
}

void CGraphicsDlg::ButtonEnable(BOOL bEnable)
{
    m_btnRefresh.EnableWindow(bEnable);
	//m_btnDemo.EnableWindow(bEnable);
	m_btnCls.EnableWindow(bEnable);
	//m_btnOpen.EnableWindow(bEnable);
	//m_btnClose.EnableWindow(bEnable);
	m_btnMagnify.EnableWindow(bEnable);
    CWnd* pWnd;
    pWnd = GetDlgItem(IDC_RDO_MAG2);
    pWnd->EnableWindow(bEnable);
    pWnd = GetDlgItem(IDC_RDO_MAG3);
    pWnd->EnableWindow(bEnable);
    pWnd = GetDlgItem(IDC_RDO_MAG4);
    pWnd->EnableWindow(bEnable);
}

void CGraphicsDlg::EnableOpenClose()
{
    m_btnOpen.EnableWindow(!m_bOpen);
    m_btnClose.EnableWindow(m_bOpen);
	m_btnColor.EnableWindow(!m_bOpen);

    CWnd* pWnd;
    pWnd = GetDlgItem(IDC_RDO_COLOR2);
    pWnd->EnableWindow(!m_bOpen);
}

void CGraphicsDlg::OnBtnOpen() 
{
	// TODO: Add your control notification handler code here
    if(m_bOpen)
        return;

    m_bOpen = TRUE;
    UpdateData();
    OpenNUIGraphics(m_nRadioColors);
    //m_bDemoStart = FALSE;

    EnableOpenClose();

    //m_btnOpen.EnableWindow(!m_bOpen);
    //m_btnClose.EnableWindow(m_bOpen);
	//m_btnColor.EnableWindow(!m_bOpen);

    ButtonEnable(TRUE);

    m_dlgController.Create(IDD_CONTROLLER);
    //m_dlgController.ShowWindow(SW_SHOW);
}

void CGraphicsDlg::OnBtnClose() 
{
	// TODO: Add your control notification handler code here
    if(!m_bOpen)
        return;

    OnBtnCls();

    CloseNUIGraphics();
    m_bOpen = FALSE;
    //m_bDemoStart = FALSE;

    ButtonEnable(FALSE);

    //m_btnOpen.EnableWindow(!m_bOpen);
    //m_btnClose.EnableWindow(m_bOpen);
	//m_btnColor.EnableWindow(!m_bOpen);
    EnableOpenClose();

    //m_dlgController.ShowWindow(SW_HIDE);
    m_dlgController.DestroyWindow();
}

void CGraphicsDlg::OnBtnRefresh() 
{
	// TODO: Add your control notification handler code here
    if(!m_bOpen)// || !m_bDemoStart)
        return;

    UpdateData();
    DrawNUIGraphicBuffer();
}

void CGraphicsDlg::OnRdoMag() 
{
	// TODO: Add your control notification handler code here
    DrawNUIGraphicBuffer();
}


