// SkySimulDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SkySimul.h"
#include "SkySimulDlg.h"
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "msm4win32.h"
#include "eqsSystem.h"


#define PIXEL2BYTES(n)              (((n)+7)/8)


#define SCREEN_POS_X    10//7
#define SCREEN_POS_Y    10//7

#define MAX_MAG         4

BYTE  l_ScreenBuffer[128*160*2];
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
// CSkySimulDlg dialog

CSkySimulDlg::CSkySimulDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkySimulDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkySimulDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSkySimulDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkySimulDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSkySimulDlg, CDialog)
	//{{AFX_MSG_MAP(CSkySimulDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
	//}}AFX_MSG_MAP
    ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_F1, IDC_BUTTON_CLOSE, OnBtnKeypad)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkySimulDlg message handlers

BOOL CSkySimulDlg::OnInitDialog()
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
	
    _beginthread( Start_UI_Task, 5000, NULL  );
    _beginthread( Start_HS_Task, 5000, NULL  );
    SendKeyInformation((long)1);


	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSkySimulDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSkySimulDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSkySimulDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CSkySimulDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

    switch( nIDEvent)
    {
    case WIN_KEY_TIMER :
        (*pfnCallBack[0])(10);
        break;
    case WIN_GENERAL_TIMER_0:
        if( g_fRepeatTimer[1] == FALSE )
            KillTimer(WIN_GENERAL_TIMER_0);        
        (*pfnCallBack[1])(10);
        break;
    case WIN_GENERAL_TIMER_1:
        (*pfnCallBack[2])(10);
        break;
    case WIN_GENERAL_TIMER_2:
        (*pfnCallBack[3])(10);
        break;
    case WIN_GENERAL_TIMER_3:
        (*pfnCallBack[4])(10);
        break;
    case WIN_BACKLIGHT_TIMER:
        (*pfnCallBack[5])(10);
        break;
    case WIN_PACEMAKER_TIMER:
        (*pfnCallBack[6])(10);
        break;

    case WIN_BELL_TIMER:
        (*pfnCallBack[7])(10);
        break;

    case WIN_MOTER_TIMER:
        (*pfnCallBack[8])(10);
        break;
    case WIN_MOTER_OFF_TIMER:
        (*pfnCallBack[9])(10);
        break;
    default:
        break;
    }
    


}


void CSkySimulDlg::DrawEQUSBuffer()
{
	CClientDC dc(this); // device context for painting

    /* main lcd */
    BYTE* pBuffer = GetLCDBuffer();
    COLORREF Color;
    BYTE bColor;
    BYTE bMask[] = { 0xC0, 0x30, 0x0C, 0x03 };
    INT nBufSize;

    CDC MemDC;
    MemDC.CreateCompatibleDC(&dc);
    CBitmap BmpDC;
    BmpDC.CreateCompatibleBitmap(&dc, SKY_SCREEN_WIDTH, SKY_SCREEN_HEIGHT);
    CBitmap* pOldBmp = MemDC.SelectObject(&BmpDC);

	memcpy( l_ScreenBuffer, pBuffer, 128*160*2);

   // UpdateData();
#ifdef FEATURE_EQS_COLOR_4GRAY
//    if(FALSE)//TRUE) //m_nRadioColors == 0) /* 4gray */
    {
        COLORREF paColorTable[3][4] = {
            { RGB(0, 0, 0), RGB(128, 128, 128), RGB(192, 192, 192), RGB(255, 255, 255) },
            { RGB(123, 140, 90), RGB(181, 193, 157), RGB(86, 100, 58), RGB(0, 0, 0) },
            { RGB(0, 0, 0), RGB(128, 128, 128), RGB(192, 192, 192), RGB(255, 255, 255) }
        };

        /* get buffer size */
        nBufSize = SKY_SCREEN_BUFFER_WIDTH/8 * SKY_SCREEN_BUFFER_HEIGHT * 2;
        INT nMaxBit[] = { 6,4,2,0 };
        for(int i = 0; i < nBufSize; i++, pBuffer++)
        {
            for(int j = 0; j < 4; j++)
            {
                bColor = *pBuffer & bMask[j];
                bColor >>= nMaxBit[j];

                POINT Point;
                Point.x = (i*4 + j) % SKY_SCREEN_BUFFER_WIDTH;
                Point.y = (i*4 + j) / SKY_SCREEN_BUFFER_WIDTH;
                //dc.SetPixel(Point, Color);
                if(Point.x > SKY_SCREEN_WIDTH)
                    break;
                MemDC.SetPixel(Point, paColorTable[1][bColor]);
            }
        }
    }
#elif defined FEATURE_EQS_COLOR_256
//    else /* 256 colors */
    {
        int nR, nG, nB; /* RGB format is 3,3,2 */
        /* get buffer size */
        nBufSize = SKY_SCREEN_BUFFER_WIDTH * SKY_SCREEN_BUFFER_HEIGHT;
        for(int i = 0; i < nBufSize; i++, pBuffer++)
        {
            bColor = *pBuffer;
            GetRGB332(bColor, nR, nG, nB);
            Color = RGB(nR, nG, nB);
            POINT Point;
            Point.x = (i) % SKY_SCREEN_BUFFER_WIDTH;
            Point.y = (i) / SKY_SCREEN_BUFFER_WIDTH;
            //dc.SetPixel(Point, Color);
            if(Point.x > SKY_SCREEN_WIDTH)
                continue;
            MemDC.SetPixel(Point, Color);
        }
    }
#elif defined FEATURE_EQS_COLOR_64K
    {
        int nR, nG, nB; /* RGB format is 3,3,2 */
        WORD wColor;

        /* get buffer size */
        for(int y = 0; y < SKY_SCREEN_BUFFER_HEIGHT; y++)
        {
            for(int x = 0; x < SKY_SCREEN_BUFFER_WIDTH; x++)
            {
                wColor = *pBuffer++;
                wColor += (*pBuffer++ << 8);
                GetRGB565(wColor, nR, nG, nB);
                Color = RGB(nR, nG, nB);
                POINT Point;
                Point.x = x;
                Point.y = y;
                //dc.SetPixel(Point, Color);
                if(Point.x > SKY_SCREEN_WIDTH)
                    continue;
                MemDC.SetPixel(Point, Color);
            }
        }
    }

#endif

    INT m_nMagnify = 1;
    dc.StretchBlt(SCREEN_POS_X, 
                  SCREEN_POS_Y, 
                  SKY_SCREEN_WIDTH*(m_nMagnify), 
                  SKY_SCREEN_HEIGHT*(m_nMagnify), 
                  &MemDC, 0, 0, SKY_SCREEN_WIDTH, SKY_SCREEN_HEIGHT, SRCCOPY);
    m_nMagnify = 2;
    dc.StretchBlt(SCREEN_POS_X+(SKY_SCREEN_WIDTH*MAX_MAG-SKY_SCREEN_WIDTH*(m_nMagnify))/2+5, 
                  SCREEN_POS_Y,//+120+3, 
                  //SCREEN_POS_Y+(SCREEN_HEIGHT*MAX_MAG-SCREEN_HEIGHT*(m_nMagnify))/2, 
                  SKY_SCREEN_WIDTH*(m_nMagnify), 
                  SKY_SCREEN_HEIGHT*(m_nMagnify), 
                  &MemDC, 0, 0, SKY_SCREEN_WIDTH, SKY_SCREEN_HEIGHT, SRCCOPY);
    
    MemDC.SelectObject(pOldBmp);
    MemDC.DeleteDC();
    BmpDC.DeleteObject();

    /* ********************************************************************** */
    /* ********************************************************************** */
    /* ********************************************************************** */
    /* ********************************************************************** */
    /* ********************************************************************** */
    /* sub lcd */
    pBuffer = GetSubLCDBuffer();

    CDC MemDCSub;
    MemDCSub.CreateCompatibleDC(&dc);
    CBitmap BmpDCSub;
    BmpDCSub.CreateCompatibleBitmap(&dc, SKY_SUB_SCREEN_WIDTH, SKY_SUB_SCREEN_HEIGHT);
    CBitmap* pOldBmpSub = MemDCSub.SelectObject(&BmpDCSub);

   // UpdateData();
#ifdef FEATURE_EQS_COLOR_4GRAY
//    if(FALSE)//TRUE) //m_nRadioColors == 0) /* 4gray */
    {
        COLORREF paColorTable[] = { RGB(123, 140, 90), RGB(0, 0, 0) };
        /* get buffer size */
        nBufSize = SKY_SUB_SCREEN_BUFFER_WIDTH/8 * SKY_SUB_SCREEN_BUFFER_HEIGHT;
        INT nMaxBit[] = { 7,6,5,4,3,2,1,0 };
        BYTE bMaskBW[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

        pBuffer += 12*3; /* skip 0, 1, 2 line */
        for(int i = 12*3; i < nBufSize; i++, pBuffer++)
        {
            for(int j = 0; j < 8; j++)
            {
                bColor = *pBuffer & bMaskBW[j];
                bColor >>= nMaxBit[j];

                POINT Point;
                Point.x = (i*8 + j) % SKY_SUB_SCREEN_BUFFER_WIDTH;
                Point.y = (i*8 + j) / SKY_SUB_SCREEN_BUFFER_WIDTH;
                //dc.SetPixel(Point, Color);
                if(Point.x > SKY_SUB_SCREEN_WIDTH)
                    break;
                MemDCSub.SetPixel(Point, paColorTable[bColor]);
            }
        }
    }
    m_nMagnify = 1;
    int nSubHeight = SKY_SUB_SCREEN_HEIGHT-3;
    dc.StretchBlt(SCREEN_POS_X, 
                  SCREEN_POS_Y+130, 
                  SKY_SUB_SCREEN_WIDTH*(m_nMagnify), 
                  nSubHeight*(m_nMagnify), 
                  &MemDCSub, 0, 3, SKY_SUB_SCREEN_WIDTH, nSubHeight, SRCCOPY);
    m_nMagnify = 2;
    dc.StretchBlt(SCREEN_POS_X+(SKY_SCREEN_WIDTH*MAX_MAG-SKY_SCREEN_WIDTH*(m_nMagnify))/2+5, 
                  SCREEN_POS_Y+260,//+120+3, 
                  //SCREEN_POS_Y+(SCREEN_HEIGHT*MAX_MAG-SCREEN_HEIGHT*(m_nMagnify))/2, 
                  SKY_SUB_SCREEN_WIDTH*(m_nMagnify), 
                  nSubHeight*(m_nMagnify), 
                  &MemDCSub, 0, 3, SKY_SUB_SCREEN_WIDTH, nSubHeight, SRCCOPY);

#elif defined FEATURE_EQS_COLOR_256
//    else /* 256 colors */
    {
        COLORREF paColorTable[] = { RGB(123, 140, 90), RGB(0, 0, 0) };
        /* get buffer size */
        nBufSize = SKY_SUB_SCREEN_BUFFER_WIDTH/8 * SKY_SUB_SCREEN_BUFFER_HEIGHT;
        INT nMaxBit[] = { 7,6,5,4,3,2,1,0 };
        BYTE bMaskBW[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

        //pBuffer += 12*3; /* skip 0, 1, 2 line */
        for(int i = 0/*12*3*/; i < nBufSize; i++, pBuffer++)
        {
            for(int j = 0; j < 8; j++)
            {
                bColor = *pBuffer & bMaskBW[j];
                bColor >>= nMaxBit[j];

                POINT Point;
                Point.x = (i*8 + j) % SKY_SUB_SCREEN_BUFFER_WIDTH;
                Point.y = (i*8 + j) / SKY_SUB_SCREEN_BUFFER_WIDTH;
                //dc.SetPixel(Point, Color);
                if(Point.x > SKY_SUB_SCREEN_WIDTH)
                    break;
                MemDCSub.SetPixel(Point, paColorTable[bColor]);
            }
        }
    }
    m_nMagnify = 1;
    int nSubHeight = SKY_SUB_SCREEN_HEIGHT;
    dc.StretchBlt(SCREEN_POS_X, 
                  SCREEN_POS_Y+130, 
                  SKY_SUB_SCREEN_WIDTH*(m_nMagnify), 
                  nSubHeight*(m_nMagnify), 
                  &MemDCSub, 0, 0, SKY_SUB_SCREEN_WIDTH, nSubHeight, SRCCOPY);
    m_nMagnify = 2;
    dc.StretchBlt(SCREEN_POS_X+(SKY_SCREEN_WIDTH*MAX_MAG-SKY_SCREEN_WIDTH*(m_nMagnify))/2+5, 
                  SCREEN_POS_Y+260,//+120+3, 
                  //SCREEN_POS_Y+(SCREEN_HEIGHT*MAX_MAG-SCREEN_HEIGHT*(m_nMagnify))/2, 
                  SKY_SUB_SCREEN_WIDTH*(m_nMagnify), 
                  nSubHeight*(m_nMagnify), 
                  &MemDCSub, 0, 0, SKY_SUB_SCREEN_WIDTH, nSubHeight, SRCCOPY);
#elif defined FEATURE_EQS_COLOR_64K
    {
        COLORREF paColorTable[] = { RGB(123, 140, 90), RGB(0, 0, 0) };
        /* get buffer size */
        nBufSize = SKY_SUB_SCREEN_BUFFER_WIDTH/8 * SKY_SUB_SCREEN_BUFFER_HEIGHT;
        INT nMaxBit[] = { 7,6,5,4,3,2,1,0 };
        BYTE bMaskBW[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

        //pBuffer += 12*3; /* skip 0, 1, 2 line */
        for(int i = 0/*12*3*/; i < nBufSize; i++, pBuffer++)
        {
            for(int j = 0; j < 8; j++)
            {
                bColor = *pBuffer & bMaskBW[j];
                bColor >>= nMaxBit[j];

                POINT Point;
                Point.x = (i*8 + j) % SKY_SUB_SCREEN_BUFFER_WIDTH;
                Point.y = (i*8 + j) / SKY_SUB_SCREEN_BUFFER_WIDTH;
                //dc.SetPixel(Point, Color);
                if(Point.x > SKY_SUB_SCREEN_WIDTH)
                    break;
                MemDCSub.SetPixel(Point, paColorTable[bColor]);
            }
        }
    }
    m_nMagnify = 1;
    int nSubHeight = SKY_SUB_SCREEN_HEIGHT;
    dc.StretchBlt(SCREEN_POS_X, 
                  SCREEN_POS_Y+170, 
                  SKY_SUB_SCREEN_WIDTH*(m_nMagnify), 
                  nSubHeight*(m_nMagnify), 
                  &MemDCSub, 0, 0, SKY_SUB_SCREEN_WIDTH, nSubHeight, SRCCOPY);
    m_nMagnify = 2;
    dc.StretchBlt(SCREEN_POS_X+(SKY_SCREEN_WIDTH*MAX_MAG-SKY_SCREEN_WIDTH*(m_nMagnify))/2+5, 
                  SCREEN_POS_Y+340,//+120+3, 
                  //SCREEN_POS_Y+(SCREEN_HEIGHT*MAX_MAG-SCREEN_HEIGHT*(m_nMagnify))/2, 
                  SKY_SUB_SCREEN_WIDTH*(m_nMagnify), 
                  nSubHeight*(m_nMagnify), 
                  &MemDCSub, 0, 0, SKY_SUB_SCREEN_WIDTH, nSubHeight, SRCCOPY);
#endif

    
    MemDCSub.SelectObject(pOldBmpSub);
    MemDCSub.DeleteDC();
    BmpDCSub.DeleteObject();
}



void CSkySimulDlg::DrawEQUSBufferRect(int xleft, int  ytop, int xright, int ybottom)
{
	//DrawEQUSBuffer();
	//return ;
	BYTE * pSrc;
	BYTE * pDest;
	CClientDC dc(this); // device context for painting

    /* main lcd */
    BYTE* pBuffer = GetLCDBuffer();
    COLORREF Color;
    BYTE bColor;
    BYTE bMask[] = { 0xC0, 0x30, 0x0C, 0x03 };
    INT nBufSize;
    CDC MemDC;

    MemDC.CreateCompatibleDC(&dc);
    CBitmap BmpDC;
    BmpDC.CreateCompatibleBitmap(&dc, SKY_SCREEN_WIDTH, SKY_SCREEN_HEIGHT);
    CBitmap* pOldBmp = MemDC.SelectObject(&BmpDC);

   // UpdateData();
#ifdef FEATURE_EQS_COLOR_4GRAY
	return ;

#elif defined FEATURE_EQS_COLOR_256
    return ;
#elif defined FEATURE_EQS_COLOR_64K
    {
        int y, nR, nG, nB; /* RGB format is 3,3,2 */
        WORD wColor;

        for(y = 0; y < 160; y++)
        {
			pSrc = l_ScreenBuffer + SKY_SCREEN_BUFFER_WIDTH * 2 * y ;
			
            for(int x = 0; x < 128; x++)
            {
                wColor = *pSrc++;
                wColor += (*pSrc++ << 8);
                GetRGB565(wColor, nR, nG, nB);
                Color = RGB(nR, nG, nB);
                POINT Point;
                Point.x = x;
                Point.y = y;
                //dc.SetPixel(Point, Color);
                if(Point.x > SKY_SCREEN_WIDTH)
                    continue;
                MemDC.SetPixel(Point, Color);
            }
        }
		
        /* get buffer size */
        for(y = ytop; y < ybottom; y++)
        {
			pSrc = pBuffer + SKY_SCREEN_BUFFER_WIDTH * 2 * y + 2*xleft;
			pDest = l_ScreenBuffer + SKY_SCREEN_BUFFER_WIDTH * 2 * y + 2*xleft;
			
            for(int x = xleft; x <= xright; x++)
            {
                wColor = *pSrc;
				*pDest++ = *pSrc++;
                wColor += (*pSrc << 8);
				*pDest++ =  *pSrc++ ;
				
                GetRGB565(wColor, nR, nG, nB);
                Color = RGB(nR, nG, nB);
                POINT Point;
                Point.x = x;
                Point.y = y;
                //dc.SetPixel(Point, Color);
                if(Point.x > SKY_SCREEN_WIDTH)
                    continue;
                MemDC.SetPixel(Point, Color);
            }
        }
    }

#endif

    INT m_nMagnify = 1;
    dc.StretchBlt(SCREEN_POS_X, 
                  SCREEN_POS_Y, 
                  SKY_SCREEN_WIDTH*(m_nMagnify), 
                  SKY_SCREEN_HEIGHT*(m_nMagnify), 
                  &MemDC, 0, 0, SKY_SCREEN_WIDTH, SKY_SCREEN_HEIGHT, SRCCOPY);
    m_nMagnify = 2;
    dc.StretchBlt(SCREEN_POS_X+(SKY_SCREEN_WIDTH*MAX_MAG-SKY_SCREEN_WIDTH*(m_nMagnify))/2+5, 
                  SCREEN_POS_Y,//+120+3, 
                  //SCREEN_POS_Y+(SCREEN_HEIGHT*MAX_MAG-SCREEN_HEIGHT*(m_nMagnify))/2, 
                  SKY_SCREEN_WIDTH*(m_nMagnify), 
                  SKY_SCREEN_HEIGHT*(m_nMagnify), 
                  &MemDC, 0, 0, SKY_SCREEN_WIDTH, SKY_SCREEN_HEIGHT, SRCCOPY);
    
    MemDC.SelectObject(pOldBmp);
    MemDC.DeleteDC();
    BmpDC.DeleteObject();

    /* ********************************************************************** */
    /* ********************************************************************** */
    /* ********************************************************************** */
    /* ********************************************************************** */
    /* ********************************************************************** */
    /* sub lcd */
    pBuffer = GetSubLCDBuffer();

    CDC MemDCSub;
    MemDCSub.CreateCompatibleDC(&dc);
    CBitmap BmpDCSub;
    BmpDCSub.CreateCompatibleBitmap(&dc, SKY_SUB_SCREEN_WIDTH, SKY_SUB_SCREEN_HEIGHT);
    CBitmap* pOldBmpSub = MemDCSub.SelectObject(&BmpDCSub);

   // UpdateData();
#ifdef FEATURE_EQS_COLOR_4GRAY
//    if(FALSE)//TRUE) //m_nRadioColors == 0) /* 4gray */
    {
        COLORREF paColorTable[] = { RGB(123, 140, 90), RGB(0, 0, 0) };
        /* get buffer size */
        nBufSize = SKY_SUB_SCREEN_BUFFER_WIDTH/8 * SKY_SUB_SCREEN_BUFFER_HEIGHT;
        INT nMaxBit[] = { 7,6,5,4,3,2,1,0 };
        BYTE bMaskBW[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

        pBuffer += 12*3; /* skip 0, 1, 2 line */
        for(int i = 12*3; i < nBufSize; i++, pBuffer++)
        {
            for(int j = 0; j < 8; j++)
            {
                bColor = *pBuffer & bMaskBW[j];
                bColor >>= nMaxBit[j];

                POINT Point;
                Point.x = (i*8 + j) % SKY_SUB_SCREEN_BUFFER_WIDTH;
                Point.y = (i*8 + j) / SKY_SUB_SCREEN_BUFFER_WIDTH;
                //dc.SetPixel(Point, Color);
                if(Point.x > SKY_SUB_SCREEN_WIDTH)
                    break;
                MemDCSub.SetPixel(Point, paColorTable[bColor]);
            }
        }
    }
    m_nMagnify = 1;
    int nSubHeight = SKY_SUB_SCREEN_HEIGHT-3;
    dc.StretchBlt(SCREEN_POS_X, 
                  SCREEN_POS_Y+130, 
                  SKY_SUB_SCREEN_WIDTH*(m_nMagnify), 
                  nSubHeight*(m_nMagnify), 
                  &MemDCSub, 0, 3, SKY_SUB_SCREEN_WIDTH, nSubHeight, SRCCOPY);
    m_nMagnify = 2;
    dc.StretchBlt(SCREEN_POS_X+(SKY_SCREEN_WIDTH*MAX_MAG-SKY_SCREEN_WIDTH*(m_nMagnify))/2+5, 
                  SCREEN_POS_Y+260,//+120+3, 
                  //SCREEN_POS_Y+(SCREEN_HEIGHT*MAX_MAG-SCREEN_HEIGHT*(m_nMagnify))/2, 
                  SKY_SUB_SCREEN_WIDTH*(m_nMagnify), 
                  nSubHeight*(m_nMagnify), 
                  &MemDCSub, 0, 3, SKY_SUB_SCREEN_WIDTH, nSubHeight, SRCCOPY);

#elif defined FEATURE_EQS_COLOR_256
//    else /* 256 colors */
    {
        COLORREF paColorTable[] = { RGB(123, 140, 90), RGB(0, 0, 0) };
        /* get buffer size */
        nBufSize = SKY_SUB_SCREEN_BUFFER_WIDTH/8 * SKY_SUB_SCREEN_BUFFER_HEIGHT;
        INT nMaxBit[] = { 7,6,5,4,3,2,1,0 };
        BYTE bMaskBW[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

        //pBuffer += 12*3; /* skip 0, 1, 2 line */
        for(int i = 0/*12*3*/; i < nBufSize; i++, pBuffer++)
        {
            for(int j = 0; j < 8; j++)
            {
                bColor = *pBuffer & bMaskBW[j];
                bColor >>= nMaxBit[j];

                POINT Point;
                Point.x = (i*8 + j) % SKY_SUB_SCREEN_BUFFER_WIDTH;
                Point.y = (i*8 + j) / SKY_SUB_SCREEN_BUFFER_WIDTH;
                //dc.SetPixel(Point, Color);
                if(Point.x > SKY_SUB_SCREEN_WIDTH)
                    break;
                MemDCSub.SetPixel(Point, paColorTable[bColor]);
            }
        }
    }
    m_nMagnify = 1;
    int nSubHeight = SKY_SUB_SCREEN_HEIGHT;
    dc.StretchBlt(SCREEN_POS_X, 
                  SCREEN_POS_Y+130, 
                  SKY_SUB_SCREEN_WIDTH*(m_nMagnify), 
                  nSubHeight*(m_nMagnify), 
                  &MemDCSub, 0, 0, SKY_SUB_SCREEN_WIDTH, nSubHeight, SRCCOPY);
    m_nMagnify = 2;
    dc.StretchBlt(SCREEN_POS_X+(SKY_SCREEN_WIDTH*MAX_MAG-SKY_SCREEN_WIDTH*(m_nMagnify))/2+5, 
                  SCREEN_POS_Y+260,//+120+3, 
                  //SCREEN_POS_Y+(SCREEN_HEIGHT*MAX_MAG-SCREEN_HEIGHT*(m_nMagnify))/2, 
                  SKY_SUB_SCREEN_WIDTH*(m_nMagnify), 
                  nSubHeight*(m_nMagnify), 
                  &MemDCSub, 0, 0, SKY_SUB_SCREEN_WIDTH, nSubHeight, SRCCOPY);
#elif defined FEATURE_EQS_COLOR_64K
    {
        COLORREF paColorTable[] = { RGB(123, 140, 90), RGB(0, 0, 0) };
        /* get buffer size */
        nBufSize = SKY_SUB_SCREEN_BUFFER_WIDTH/8 * SKY_SUB_SCREEN_BUFFER_HEIGHT;
        INT nMaxBit[] = { 7,6,5,4,3,2,1,0 };
        BYTE bMaskBW[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

        //pBuffer += 12*3; /* skip 0, 1, 2 line */
        for(int i = 0/*12*3*/; i < nBufSize; i++, pBuffer++)
        {
            for(int j = 0; j < 8; j++)
            {
                bColor = *pBuffer & bMaskBW[j];
                bColor >>= nMaxBit[j];

                POINT Point;
                Point.x = (i*8 + j) % SKY_SUB_SCREEN_BUFFER_WIDTH;
                Point.y = (i*8 + j) / SKY_SUB_SCREEN_BUFFER_WIDTH;
                //dc.SetPixel(Point, Color);
                if(Point.x > SKY_SUB_SCREEN_WIDTH)
                    break;
                MemDCSub.SetPixel(Point, paColorTable[bColor]);
            }
        }
    }
    m_nMagnify = 1;
    int nSubHeight = SKY_SUB_SCREEN_HEIGHT;
    dc.StretchBlt(SCREEN_POS_X, 
                  SCREEN_POS_Y+170, 
                  SKY_SUB_SCREEN_WIDTH*(m_nMagnify), 
                  nSubHeight*(m_nMagnify), 
                  &MemDCSub, 0, 0, SKY_SUB_SCREEN_WIDTH, nSubHeight, SRCCOPY);
    m_nMagnify = 2;
    dc.StretchBlt(SCREEN_POS_X+(SKY_SCREEN_WIDTH*MAX_MAG-SKY_SCREEN_WIDTH*(m_nMagnify))/2+5, 
                  SCREEN_POS_Y+340,//+120+3, 
                  //SCREEN_POS_Y+(SCREEN_HEIGHT*MAX_MAG-SCREEN_HEIGHT*(m_nMagnify))/2, 
                  SKY_SUB_SCREEN_WIDTH*(m_nMagnify), 
                  nSubHeight*(m_nMagnify), 
                  &MemDCSub, 0, 0, SKY_SUB_SCREEN_WIDTH, nSubHeight, SRCCOPY);
#endif

    
    MemDCSub.SelectObject(pOldBmpSub);
    MemDCSub.DeleteDC();
    BmpDCSub.DeleteObject();
}



void CSkySimulDlg::GetRGB332(BYTE bRGB332, int &nR, int &nG, int &nB)
{
    nR = ((bRGB332 & 0xE0) >> 5) * 32;
    nG = ((bRGB332 & 0x1C) >> 2) * 32;
    nB = (bRGB332 & 0x03) * 64;
    /* adjust color .. just for display in the true color system */
    nR += 16;
    nG += 16;
    nB += 32;
}

void CSkySimulDlg::GetRGB565(WORD wRGB565, int &nR, int &nG, int &nB)
{
    nR = ((wRGB565 & 0xF800) >> 11) * 8;
    nG = ((wRGB565 & 0x07E0) >> 5) * 4;
    nB = ((wRGB565 & 0x001F) & 0x1F) * 8;
    /* adjust color .. just for display in the true color system */
    //nR += 8;
    //nG += 4;
    //nB += 8;
}

#define sNONE_EVENT              -1
#define sOFFSET_KEY_EVENT        EQS_OFFSET_MAX_EVENT+0       /*   0 ~  99 */
#define sOFFSET_SMS_EVENT        EQS_OFFSET_MAX_EVENT+100     /* 100 ~ 199 */
#define sOFFSET_CALL_EVENT       EQS_OFFSET_MAX_EVENT+200     /* 200 ~ 299 */
#define sOFFSET_PH_EVENT         EQS_OFFSET_MAX_EVENT+300     /* 300 ~ 399 */
#define sOFFSET_TM_EVENT         EQS_OFFSET_MAX_EVENT+400     /* 400 ~ 499 */
#define sOFFSET_SND_EVENT        EQS_OFFSET_MAX_EVENT+500     /* 500 ~ 599 */
#define sOFFSET_SYS_EVENT        EQS_OFFSET_MAX_EVENT+600     /* 600 ~ 699 */
#define sOFFSET_OTASP_EVENT      EQS_OFFSET_MAX_EVENT+700     /* 700 ~ 799 */
#define sOFFSET_INBAND_EVENT     EQS_OFFSET_MAX_EVENT+800     /* 800 ~ 899 */


void CSkySimulDlg::OnBtnKeypad(UINT nID)
{
enum tagSKY_EVENT_E 
{
#undef DEFINE_EVENT
#undef DEFINE_EVENT_CATEGORY
#define DEFINE_EVENT_CATEGORY(nID, nCategory) SE_##nID = nCategory,
#define DEFINE_EVENT(nID) SE_##nID,
#include "eqs_evnt.tbl"

    SE_KEY_EXT = sOFFSET_KEY_EVENT, 
                                /* No more keycodes available */
    SE_KEY_PHONE_OPEN,          /* flip or folder is open */
    SE_KEY_PHONE_CLOSE,         /* flip or folder is close */
    SE_KEY_SPEAKER,             /* speaker phone on/off key */
    SE_KEY_LOCK,                /* lock on/off key */
    SE_KEY_POWER,               /* power key -- on or off ? */
    SE_KEY_POWER_OFF,           /* power off key */
    SE_KEY_MANNERS_LONG,        /* manners mode on/off key */
    SE_KEY_EAR_JACK_ON,         /* ear jack is placed */
    SE_KEY_EAR_JACK_OFF,        /* ear jack is off */
    SE_KEY_EAR_CLICK,           /* ear jack button is clicked */
    SE_KEY_EAR_DOUBLE_CLICK,    /* ear jack button is double-clicked */
    SE_KEY_HANDS_FREE_ON,       /* Phone was placed in hands-free cradle */
    SE_KEY_HANDS_FREE_OFF,      /* Phone was lifed from hands-free cradle */
    SE_KEY_RELEASE,             /* a key is released */
    SE_KEY_CHARGING_ON,         /* charging on */
    SE_KEY_CHARGING_OFF,        /* charging on */
    SE_KEY_EXT_POWER_ON,        /* external power is placed in */
    SE_KEY_EXT_POWER_OFF,       /* external power is detached */

    /*========================================================================
    ** SMS events
    */
#ifdef FEATURE_UASMS
    SE_SMS = sOFFSET_SMS_EVENT,
    SE_SMS_MESSAGE,             /* SMS message received */
    SE_SMS_STATUS_MESSAGE,      /* SMS status message received */
#endif /* FEATURE_UASMS */

    /*========================================================================
    ** Call managing events
    */
    SE_CALL = sOFFSET_CALL_EVENT,
    SE_CALL_INCOMING,             /* incoming call */
    SE_CALL_ORIGINATED,           /* a call is originated */
    SE_CALL_CONNECT,              /* call connected */
    SE_CALL_ENDED,                /* The call ended  */
    SE_CALL_PRIVACY_CHANGED,      /* the voice privacy is changed */
    SE_CALL_SO_CHANGED,           /* the service option is changed */
    SE_CALL_CALLER_ID,            /* Caller ID could be call waiting */
    SE_CALL_FLASHED,              /* a flash is sent */
    SE_CALL_COMMAND_ERROR,        /* some error happened to call commands */
    SE_CALL_EXT_DISPLAY,          /* display exit display infomation */
    /* newly added for EQUS engine */
    SE_CALL_RELEASED,          /* UI_RELEAE_F call is released */

    /*========================================================================
    ** phone events 
    */
    SE_PH = sOFFSET_PH_EVENT,
    SE_PH_INUSE_STATE,          /* inuse state is changed */
    SE_PH_SERVICE_STATE,        /* service state is changed */
    SE_PH_OPERATING_MODE,       /* operating mode is changed */
    SE_PH_CMDA_LOCK_MODE,       /* CDMA lock mode is changed */
    SE_PH_PREFERED_MODE,        /* prefered mode is changed */
    SE_PH_PREFERED_SYSTEM,      /* prefered system is changed */
    SE_PH_ANSWER_VOICE,         /* answer voice as data setting is changed*/
    SE_PH_RESTRICTION_LEVEL,    /* restriction level is changed */
    SE_PH_CURRENT_NAM,          /* current NAM is changed */
    SE_PH_NAM_SELECT,           /* NAM selection is changed */
    SE_PH_ROAM_STATUS,          /* roaming status is changed */
    SE_PH_INFO_AVAIL,           /* Phone information is now available */
    SE_PH_COMMAND_ERROR,        /* some error happend to phone commands */
    /* newly added for EQUS engine */
    SE_PH_MAINTREQ,             /* Maintenance requested */
    SE_PH_RSSI,                 /* new RSSI level */

    /*========================================================================
    ** sound events 
    */
    SE_SND = sOFFSET_SND_EVENT,
    SE_SND_MULTI_TONE_DONE,     /* multi-tone done playing */
    SE_SND_MUTE,                /* speaker mute */
    SE_SND_ABBR_ALERT,          /* abbreviated alert */
    SE_SND_DTMF,

    /*========================================================================
    ** system events 
    */
    SE_SYS = sOFFSET_SYS_EVENT,
    SE_SYS_OFFLINE,             /* go offline */
    SE_SYS_INFO,                /* information came in from base station */
    SE_SYS_TO_IDLE,             /* back out to idle state */
    SE_SYS_BAD_BATT,            /* Battery used up, powering down: girin. */
    SE_SYS_LOW_BATT,            /* Battery almost runs short, low batt. girin */
    
#ifdef FEATURE_OTASP
    /*========================================================================
    ** otasp events 
    */
    SE_OTASP = sOFFSET_OTASP_EVENT,
    SE_OTASP_END_ACTIVATION,    /* End otasp activation call */
    SE_OTASP_STATUS,            /* Sent otasp activation ending status */
#endif

    /*========================================================================
    ** inband events 
    */
    SE_INBAND = sOFFSET_INBAND_EVENT,
    SE_INBAND_FORWARD_BURST_DTMF,
    
    /*========================================================================
    ** inband events 
    */
    SE_ERROR                        /* always last */
};
typedef enum   tagSKY_EVENT_E           SKY_EVENT_E;

    SKY_EVENT_E Event;
    long index;
    SKY_EVENT_E EventWave[] = {
         SE_KEY_F1 ,
         SE_KEY_F2 ,
         SE_KEY_UP ,
         SE_KEY_LEFT,
         SE_KEY_CENTER ,
         SE_KEY_RIGHT,
         SE_KEY_SEND,  //SEND
         SE_KEY_DOWN,
         SE_KEY_POWER, //POWER
         SE_KEY_CLEAR,                 
         SE_KEY_1,
         SE_KEY_2,
         SE_KEY_3,
         SE_KEY_4, 
         SE_KEY_5, 
         SE_KEY_6, 
         SE_KEY_7, 
         SE_KEY_8, 
         SE_KEY_9,  
         SE_KEY_STAR,  
         SE_KEY_0,  
         SE_KEY_POUND, 
         SE_KEY_RECORD,   //RECORD
         SE_KEY_PLAY,  //PLAHY
         SE_KEY_AUTO_ANSWER,  //MANNERS
         SE_KEY_END
    };
   index = nID - IDC_BUTTON_F1;
   Event = EventWave[index];   
   WIN_ProcessKey((long)Event);
      
}