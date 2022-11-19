// SKFontDlg.cpp : implementation file
//

#include <stdio.h>
#include <string.h>

#include "stdafx.h"
#include "SKFont.h"
#include "SKFontDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	MAX_IMG_COL		128
#define	MAX_IMG_ROW		256

#define	MAX_CHAR_COL	32
#define	MAX_CHAR_ROW	32

#define	FONT_WHITE		255     /* white */
#define	FONT_LGRAY		192     /* white gray */
#define	FONT_DGRAY		128     /* dark gray */
#define	FONT_BLACK		0       /* black */

#define BTN_RDO_BW       0       /* black and white */
#define BTN_RDO_4G       1       /* 4 grays */
#define BTN_RDO_256      2       /* 256 colors */

#define FONT_BORDER     64

BYTE	rgBuffer[MAX_IMG_COL*(MAX_CHAR_COL + 2)][MAX_IMG_ROW*(MAX_CHAR_ROW + 2)];

/////////////////////////////////////////////////////////////////////////////
// CSKFontDlg dialog

CSKFontDlg::CSKFontDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSKFontDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSKFontDlg)
	m_nColSize = 0;
	m_nRowSize = 0;
	m_strImagePath = _T("");
	m_strSrcPath = _T("");
	m_nCharSize = 0;
	m_nRadio = 0;
	m_nBoundaryColor = FONT_BORDER;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSKFontDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSKFontDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_wndProg);
	DDX_Text(pDX, IDC_COLSIZE, m_nColSize);
	DDX_Text(pDX, IDC_ROWSIZE, m_nRowSize);
	DDX_Text(pDX, IDC_IMAGE_PATH, m_strImagePath);
	DDX_Text(pDX, IDC_SRC_PATH, m_strSrcPath);
	DDX_Text(pDX, IDC_CHARSIZE, m_nCharSize);
	DDX_Radio(pDX, IDC_RDO_1, m_nRadio);
	DDX_Text(pDX, IDC_EDT_BCOLOR, m_nBoundaryColor);
	DDV_MinMaxInt(pDX, m_nBoundaryColor, 0, 255);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSKFontDlg, CDialog)
	//{{AFX_MSG_MAP(CSKFontDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SET_IMAGE_PATH, OnSetImagePath)
	ON_BN_CLICKED(IDC_SET_SRC_PATH, OnSetSrcPath)
	ON_BN_CLICKED(IDC_IMAGE_TO_SOURCE, OnImageToSource)
	ON_BN_CLICKED(IDC_IMAGE_TO_ANISRC, OnImageToAnisrc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSKFontDlg message handlers

BOOL CSKFontDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_nColSize     = AfxGetApp()->GetProfileInt("FontSize", "Column", 8);
	m_nRowSize     = AfxGetApp()->GetProfileInt("FontSize", "Row", 16);
	m_nRadio     = AfxGetApp()->GetProfileInt("FontSize", "BPP", 0);
	m_strImagePath = AfxGetApp()->GetProfileString("Path", "Image", NULL);
	m_strSrcPath   = AfxGetApp()->GetProfileString("Path", "Source", NULL);
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSKFontDlg::OnPaint() 
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
HCURSOR CSKFontDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSKFontDlg::OnSetImagePath() 
{
	UpdateData();

	static CFileDialog fileDlg(TRUE, "ppm", m_strImagePath, OFN_HIDEREADONLY,
		"Image Files (*.PPM)|*.ppm|All Files (*.*)|*.*||", NULL);

	// Open file dialog
	if (fileDlg.DoModal() != IDOK)
		return;

	// Get file path
	m_strImagePath = fileDlg.GetPathName();
	UpdateData(FALSE);
	AfxGetApp()->WriteProfileString("Path", "Image", m_strImagePath);
}

void CSKFontDlg::OnSetSrcPath() 
{
	UpdateData();

	static CFileDialog fileDlg(TRUE, NULL, m_strSrcPath, OFN_HIDEREADONLY,
		                       "NUI Font Data File (*.FNT)|*.FNT|NUI Bitmap Image Data File (*.NBI)|*.NBI|All Files (*.*)|*.*||", NULL);

	// Open file dialog
	if (fileDlg.DoModal() != IDOK)
		return;

	// Get file path
	m_strSrcPath = fileDlg.GetPathName();
	UpdateData(FALSE);
	AfxGetApp()->WriteProfileString("Path", "Source", m_strSrcPath);
}

BOOL CSKFontDlg::ReadHex(FILE* fp, BYTE& data)
{
	int		ch;
	int		chOld = -1;
	BOOL	fCmtOld = FALSE;	// C style comment
	BOOL	fCmt = FALSE;		// C++ style comment

	while (1) {
		ch = getc(fp);

		if (ch == EOF)
			return FALSE;

		if (fCmtOld == TRUE) {							// End of C style comment
			if (chOld == '*' && ch == '/')
				fCmtOld = FALSE;
		}
		else if (fCmt == TRUE) {						// End of C++ style comment
			if (ch == '\n')
				fCmt = FALSE;
		}
		else if (chOld == '/' && ch == '*') {			// Check C style comment
			fCmtOld = TRUE;
		}
		else if (chOld == '/' && ch == '/') {			// Check C++ style comment
			fCmt = TRUE;
		}
		else if (chOld == '0' && toupper(ch) == 'X') {	// Hexa prefix
			char	szHex[5] = "0x00";
			int		ch1 = fgetc(fp);
			if (isalnum(ch1) || (toupper(ch1) >= 'A' && toupper(ch1) <= 'F')) {
				int	ch2 = fgetc(fp);
				if (isalnum(ch2) || (toupper(ch2) >= 'A' && toupper(ch2) <= 'F')) {
					szHex[2] = (char)ch1;
					szHex[3] = (char)ch2;
					sscanf(szHex, "%x", &data);
					return TRUE;
				}
			}  // Inserted by MHKim
		}

		chOld = ch;
	}
}

// insert for A7 Project start
BOOL CSKFontDlg::ReadWordHex(FILE* fp, WORD& data)
{
	int		ch;
	int		chOld = -1;
	BOOL	fCmtOld = FALSE;	// C style comment
	BOOL	fCmt = FALSE;		// C++ style comment

	while (1) {
		ch = getc(fp);

		if (ch == EOF)
			return FALSE;

		if (fCmtOld == TRUE) {							// End of C style comment
			if (chOld == '*' && ch == '/')
				fCmtOld = FALSE;
		}
		else if (fCmt == TRUE) {						// End of C++ style comment
			if (ch == '\n')
				fCmt = FALSE;
		}
		else if (chOld == '/' && ch == '*') {			// Check C style comment
			fCmtOld = TRUE;
		}
		else if (chOld == '/' && ch == '/') {			// Check C++ style comment
			fCmt = TRUE;
		}
		else if (chOld == '0' && toupper(ch) == 'X') {	// Hexa prefix
			char	szHex[7] = "0x0000";
			int		ch1 = fgetc(fp);
			if (isalnum(ch1) || (toupper(ch1) >= 'A' && toupper(ch1) <= 'F')) {
				int	ch2 = fgetc(fp);
				if (isalnum(ch2) || (toupper(ch2) >= 'A' && toupper(ch2) <= 'F')) {
					int ch3 = fgetc(fp);
					if (isalnum(ch3) || (toupper(ch3) >= 'A' && toupper(ch3) <= 'F')) {
						int ch4 = fgetc(fp);
						if (isalnum(ch4) || (toupper(ch4) >= 'A' && toupper(ch4) <= 'F')) {
							szHex[2] = (char)ch1;
							szHex[3] = (char)ch2;
							szHex[4] = (char)ch3;
							szHex[5] = (char)ch4;
							sscanf(szHex, "%x", &data);
							return TRUE;
						}
					}
				}
			}

		}

		chOld = ch;
	}
}
// insert for A7 Project end
  
BOOL CSKFontDlg::IsFontBorder(int nRGB)
{
    return (nRGB == m_nBoundaryColor);
}

#define TRANSPARENT_COLOR               0x01 /* dark blue */
#define IS_TRANSPARENT_COLOR(r, g, b)   (r == 0xFF && g == 0xFF && b == 0xFF)
        
void CSKFontDlg::OnImageToSource() // 폰트를 코드로
{
	UpdateData();

	FILE*	fpImage = fopen(m_strImagePath, "r");
	if (fpImage == NULL) {
		AfxMessageBox("파일을 열 수 없습니다.");
		return;
	}

	FILE*	fpSrc = fopen(m_strSrcPath, "r");
	if (fpSrc != NULL) {
		fclose(fpSrc);
		int nRet = AfxMessageBox("데이터 파일이 이미 있습니다. 덮어 쓸까요?", MB_YESNO);
		if (nRet == IDNO)
			return;
	}

	fpSrc = fopen(m_strSrcPath, "w");
	if (fpSrc == NULL) {
		fclose(fpImage);
		AfxMessageBox("파일을 생성할 수 없습니다.");
		return;
	}

	// Get image size
	int	nTemp;
	int	nWidth, nHeight;
	int	i, j;

	while ((nTemp = getc(fpImage)) != '#');
	while ((nTemp = getc(fpImage)) != '\n');

	fscanf(fpImage, "%d %d %d", &nWidth, &nHeight, &nTemp);

	if (nWidth < 8 || nHeight < 8) {
		fclose(fpSrc);
		fclose(fpImage);
		AfxMessageBox("올바른 이미지가 아닙니다.");
		return;
	}

	// Read image data
	for (i = 0; i < nHeight; i++) {

		m_wndProg.SetPos(50*i/(nHeight - 1));
		UpdateData(FALSE);

		for (j = 0; j < nWidth; j++) {
			int	nR, nG, nB;
			fscanf(fpImage, "%d %d %d", &nR, &nG, &nB);
            /* border */
            if((nR == FONT_BORDER) && (nG == FONT_BORDER) && (nB == FONT_BORDER))
                rgBuffer[j][i] = nR;
            else if(m_nRadio == 2) /* 256 colors */
            {
                if(IS_TRANSPARENT_COLOR(nR, nG, nB))
                    rgBuffer[j][i] = TRANSPARENT_COLOR;
                else
                    rgBuffer[j][i] = MakeRGB332(nR, nG, nB);
            }
			else 
                rgBuffer[j][i] = nR;
		}
	}

	// Get font size
	for (i = 0; i <= MAX_CHAR_COL; i++)
		if (IsFontBorder(rgBuffer[i + 1][1]))
			break;
	m_nColSize = i;

	for (i = 0; i <= MAX_CHAR_ROW; i++)
		if (IsFontBorder(rgBuffer[1][i + 1]))
			break;
	m_nRowSize = i;

    /* write the font size */
    //fprintf(fpSrc, "0x%02X, 0x%02X, \n", m_nColSize, m_nRowSize);

	UpdateData(FALSE);

	int		nAssume = (nWidth/(m_nColSize + 2))*(nHeight/(m_nRowSize + 2));

	int nColTotalFont = (nWidth)/(2 + m_nColSize);

	// Write source
	m_nCharSize = 0;
    /* get nBitsPerPixel */
    int anBitsPerPixel[] = { 1, 2, 8 };
    int nBitsPerPixel = anBitsPerPixel[m_nRadio];
	int	nHexaPerChar = ((m_nColSize - 1)/8 + 1)*m_nRowSize*nBitsPerPixel;		// Number of hexa data per char
	int	x, y;

    if(nBitsPerPixel == 8)
    {
        nHexaPerChar = m_nColSize*m_nRowSize;
    }

    //if(nBitsPerPixel == 8) /* 256 */
    //{
	//	fclose(fpSrc);
	//	fclose(fpImage);
	//	AfxMessageBox("256 color FONT는 지원하지 않습니다.");
	//	return;
    //}

	while (1) {

		m_wndProg.SetPos(50 + 50*m_nCharSize/nAssume);
		UpdateData(FALSE);

		x = (m_nCharSize%nColTotalFont)*(m_nColSize + 2) + 1;
		y = (m_nCharSize/nColTotalFont)*(m_nRowSize + 2) + 1;

		if (y > nHeight || !IsFontBorder(rgBuffer[x - 1][y - 1]))
			break;

        if(nBitsPerPixel == 1) /* BLACK AND WHITE */
        {
		    int	col = 0;
		    int	row = 0;

		    for (i = 0; i < nHexaPerChar; i++) {
			    int	nData = 0;
                int bMask[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
			    for (j = 0; j + col < m_nColSize; j++) {
                    if(j != 0 && !(j % 8))
                    {
        			    fprintf(fpSrc, "0x%02X, ", nData);
                        nData = 0;
                        i++;
                        col += 8;
                        j = 0;
                    }
				    if (rgBuffer[x + col + j][y + row] == 0)
					    nData |= bMask[j%8];
			    }
                col = 0;

			    fprintf(fpSrc, "0x%02X, ", nData);
            
			    if (++row == m_nRowSize) {
				    row = 0;
				    col += 8;
			    }

			    if (i%8 == 7 || i == nHexaPerChar - 1)
				    fprintf(fpSrc, "\n");
		    }

		    m_nCharSize++;
		    UpdateData(FALSE);
        }
        else if(nBitsPerPixel == 2) /* 4grays */
        {
		    int	col = 0;
		    int	row = 0;

		    for (i = 0; i < nHexaPerChar; i++) 
            {
                int nData = 0;
                int	nBlackMask = 0xC0;
                int nDGrayMask = 0x80;
                int nWGrayMask = 0x40;

			    for (j = 0; j + col < m_nColSize; j++) 
                {
                    if(j != 0 && !(j % 4))
                    {
        			    fprintf(fpSrc, "0x%02X, ", nData);
                        nData = 0;
                        i++;
                        col += 4;
                        j = 0;
                        nBlackMask = 0xC0;
                        nDGrayMask = 0x80;
                        nWGrayMask = 0x40;
                    }
				    switch(rgBuffer[x + col + j][y + row])
                    {
                    case FONT_BLACK :
					    nData |= nBlackMask;
                        break;
                    case FONT_DGRAY :
                        nData |= nDGrayMask;
                        break;
                    case FONT_LGRAY :
                        nData |= nWGrayMask;
                        break;
                    }
				    nBlackMask >>= 2;
				    nDGrayMask >>= 2;
				    nWGrayMask >>= 2;

			    }
                col = 0;

			    fprintf(fpSrc, "0x%02X, ", nData);
            
			    if (++row == m_nRowSize) {
				    row = 0;
				    col += 8;
			    }

			    if (i%8 == 7 || i == nHexaPerChar - 1)
				    fprintf(fpSrc, "\n");
		    }

		    m_nCharSize++;
		    UpdateData(FALSE);
        }
        else if(nBitsPerPixel == 8) /* 256 */
        {
		    int	col = 0;
		    int	row = 0;

            for(row = 0; row < m_nRowSize; row++)
            //for (i = 0; i < nHexaPerChar; i++) 
            {
			    for (col = 0; col < m_nColSize; col++) 
                {
			        int nData = rgBuffer[x + col][y + row];
		            fprintf(fpSrc, "0x%02X, ", nData);

                }
			    //if (++row == m_nRowSize) {
				//    row = 0;
			    // }
			    fprintf(fpSrc, "\n");
            }

            m_nCharSize++;
		    UpdateData(FALSE);
        }
	}

	fclose(fpSrc);
	fclose(fpImage);
	
	MessageBox("저장이 완료되었습니다.", "코드 변환", MB_ICONINFORMATION);

	m_wndProg.SetPos(0);
	UpdateData(FALSE);

	AfxGetApp()->WriteProfileInt("FontSize", "Column", m_nColSize);
	AfxGetApp()->WriteProfileInt("FontSize", "Row", m_nRowSize);
	AfxGetApp()->WriteProfileInt("FontSize", "BPP", m_nRadio);
	AfxGetApp()->WriteProfileString("Path", "Image", m_strImagePath);
	AfxGetApp()->WriteProfileString("Path", "Source", m_strSrcPath);
}

BYTE CSKFontDlg::MakeRGB332(int nR, int nG, int nB)
{
    BYTE bRGB332;

    bRGB332 = ((nR/32) << 5) | ((nG/32) << 2) | (nB/64);
    return bRGB332;
}

void CSKFontDlg::OnImageToAnisrc() 
{
	UpdateData();

	FILE*	fpImage = fopen(m_strImagePath, "r");
	if (fpImage == NULL) {
		AfxMessageBox("파일을 열 수 없습니다.");
		return;
	}

	FILE*	fpSrc = fopen(m_strSrcPath, "r");
	if (fpSrc != NULL) {
		fclose(fpSrc);
		int nRet = AfxMessageBox("데이터 파일이 이미 있습니다. 덮어 쓸까요?", MB_YESNO);
		if (nRet == IDNO)
			return;
	}

	fpSrc = fopen(m_strSrcPath, "w");
	if (fpSrc == NULL) {
		fclose(fpImage);
		AfxMessageBox("파일을 생성할 수 없습니다.");
		return;
	}

	// Get image size
	int	nTemp;
	int	nWidth, nHeight;	// Image size
	int	i, j, k;

	while ((nTemp = getc(fpImage)) != '#');
	while ((nTemp = getc(fpImage)) != '\n');

	fscanf(fpImage, "%d %d %d", &nWidth, &nHeight, &nTemp);

	if (nWidth < 8 || nHeight < 8) {
		fclose(fpSrc);
		fclose(fpImage);
		AfxMessageBox("올바른 이미지가 아닙니다.");
		return;
	}

	// Read image data
	for (i = 0; i < nHeight; i++) {

		m_wndProg.SetPos(50*i/(nHeight - 1));
		UpdateData(FALSE);

		for (j = 0; j < nWidth; j++) {
			int	nR, nG, nB;
			fscanf(fpImage, "%d %d %d", &nR, &nG, &nB);
            if(m_nRadio == 2) /* 256 colors */
                rgBuffer[j][i] = MakeRGB332(nR, nG, nB);
			else
                rgBuffer[j][i] = nR;
		}
	}

	// Write source
	for (i = 0; i < nHeight; i++) {

		for (j = 0; j < nWidth; ) {
            
            if(m_nRadio == BTN_RDO_BW)
            {
			    int	nData = 0;
			    int	nMask = 0x80;

			    for (k = 0; k < 8; k++) {
				    if (rgBuffer[j + k][i] == FONT_BLACK) /* black */
					    nData |= nMask;
				    nMask >>= 1;
			    }
                j+=8;
			    fprintf(fpSrc, "0x%02X, ", nData);
            }
            else if(m_nRadio == BTN_RDO_4G)
            {
			    int	nData = 0;
                int	nBlackMask = 0xC0;
                int nDGrayMask = 0x80;
                int nWGrayMask = 0x40;

                for (k = 0; k < 4; k++) {
                    if(j+k >= nWidth)
                        break;
				    switch(rgBuffer[j + k][i])
                    {
                    case FONT_BLACK :
					    nData |= nBlackMask;
                        break;
                    case FONT_DGRAY :
                        nData |= nDGrayMask;
                        break;
                    case FONT_LGRAY :
                        nData |= nWGrayMask;
                        break;
                    }
				    nBlackMask >>= 2;
				    nDGrayMask >>= 2;
				    nWGrayMask >>= 2;
			    }
                j+=4;
			    fprintf(fpSrc, "0x%02X, ", nData);
            }
            else if(m_nRadio == BTN_RDO_256)
            {
			    int nData = rgBuffer[j][i];
			    fprintf(fpSrc, "0x%02X, ", nData);
                j++;
            }
		}
		fprintf(fpSrc, "\n");

		m_wndProg.SetPos(50 + 50*i/(nHeight - 1));
	}

	fclose(fpSrc);
	fclose(fpImage);

	// Show Image size
	AfxGetApp()->WriteProfileInt("FontSize", "Column", nWidth);		
	AfxGetApp()->WriteProfileInt("FontSize", "Row", nHeight);
	AfxGetApp()->WriteProfileInt("FontSize", "BPP", m_nRadio);
	AfxGetApp()->WriteProfileString("Path", "Image", m_strImagePath);
	AfxGetApp()->WriteProfileString("Path", "Source", m_strSrcPath);

	MessageBox("저장이 완료되었습니다.", "코드 변환", MB_ICONINFORMATION);

	m_wndProg.SetPos(0);
	UpdateData(FALSE);
}

void CSKFontDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

