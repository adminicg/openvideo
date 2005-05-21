// CamViewWinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CamViewWin.h"
#include "CamViewWinDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CCamViewWinDlg dialog

CCamViewWinDlg::CCamViewWinDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCamViewWinDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCamViewWinDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	vidCap = NULL;
}

void CCamViewWinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCamViewWinDlg)
	DDX_Control(pDX, IDC_VIDEOMODES, m_VideoModes);
	DDX_Control(pDX, IDC_CAMERAS, m_Cameras);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCamViewWinDlg, CDialog)
	//{{AFX_MSG_MAP(CCamViewWinDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_TEST, OnTest)
	ON_LBN_SELCHANGE(IDC_CAMERAS, OnSelchangeCameras)
	ON_LBN_SELCHANGE(IDC_VIDEOMODES, OnSelchangeVideoModes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCamViewWinDlg message handlers

BOOL CCamViewWinDlg::OnInitDialog()
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

	initVidCap();


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCamViewWinDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCamViewWinDlg::OnPaint() 
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
HCURSOR CCamViewWinDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void
CCamViewWinDlg::initVidCap()
{
	vidCap = CVPlatform::GetPlatform()->AcquireVideoCapture();

	if(!CVSUCCESS(vidCap->Init()))
	{
		CVPlatform::GetPlatform()->Release(vidCap);
		MessageBox("initializing video capture object failed.", "ERROR", MB_OK);
		return;
	}


	numDevices = 0;
	selMode = -1;
	numModes = 0;
	vidCap->GetNumDevices(numDevices);


	for(int i=0; i<numDevices; i++)
	{
		CVVidCapture::VIDCAP_DEVICE deviceInfo;

		vidCap->GetDeviceInfo(i, deviceInfo);

		int id = m_Cameras.AddString(deviceInfo.DeviceString);
		m_Cameras.SetItemData(id, i);

		if(i==0)
			selDeviceName = deviceInfo.DeviceString;
	}
}


void
CCamViewWinDlg::formatModeString(CVVidCapture::VIDCAP_MODE& nModeInfo, int idx, char* str)
{
	sprintf(str, "Mode %d  -  %dx%d  %s  at %d frames/sec", idx,
															nModeInfo.XRes, nModeInfo.YRes,
															vidCap->GetFormatModeName(nModeInfo.InputFormat),
															nModeInfo.EstFrameRate);
}


void
CCamViewWinDlg::OnSelchangeCameras() 
{
	int idx = m_Cameras.GetCurSel();
	selDevice = m_Cameras.GetItemData(idx);

	selMode = -1;

	m_VideoModes.ResetContent();

	CVVidCapture::VIDCAP_DEVICE deviceInfo;

	vidCap->GetDeviceInfo(selDevice, deviceInfo);
	selDeviceName = deviceInfo.DeviceString;


	if(vidCap->IsConnected())
		vidCap->Disconnect();


	if(!CVSUCCESS(vidCap->Connect(selDevice)))
	{
		char str[256];
		sprintf(str, "Connecting to camera device\n\n         %s       \n\nfailed", selDeviceName);
		MessageBox(str, "ERROR", MB_OK);
		return;
	}


	numModes = 0;
	vidCap->GetNumSupportedModes(numModes);

	for(int curmode=0; curmode<numModes; curmode++)
	{
		CVVidCapture::VIDCAP_MODE modeInfo;

		if(CVSUCCESS(vidCap->GetModeInfo(curmode, modeInfo)))
		{
			modeInfos[curmode] = modeInfo;
			char str[256];

			formatModeString(modeInfo, curmode, str);

			int id = m_VideoModes.AddString(str);
			m_VideoModes.SetItemData(id, curmode);
		}
	}
}


void CCamViewWinDlg::OnSelchangeVideoModes() 
{
	selMode = m_VideoModes.GetItemData(m_VideoModes.GetCurSel());
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL createPreviewWindow(char* title, int width, int height);
void killPreviewWindow();
bool capCallback(CVRES status, CVImage* imagePtr, void* userParam);

const char* wndClassName = "VideoPreviewWindow";

struct PreviewWindow {
	HINSTANCE hInstance;
	HDC hDC;
	HWND hWnd;
	bool active,
		 created;
	CVVidCapture* vidCap;

	BITMAPINFO bitmapInfo;
	int width,height;

	PreviewWindow()
	{
		hInstance = NULL;
		hDC = NULL;
		hWnd = NULL;
		active = false;
		created = false;
		vidCap = NULL;
	};
} previewWindow;


void CCamViewWinDlg::OnTest() 
{
	if(previewWindow.created)
		killPreviewWindow();

	char str[256];

	formatModeString(modeInfos[selMode], selMode, str);

	createPreviewWindow(str, modeInfos[selMode].XRes, modeInfos[selMode].YRes);

	previewWindow.vidCap = vidCap;


	if(CVFAILED(vidCap->SetMode(selMode)))
	{
		MessageBox("failed to set camera mode %d", "ERROR", MB_OK);
		killPreviewWindow();
		return;
	}

	if(!CVSUCCESS(vidCap->StartImageCap(CVImage::CVIMAGE_BGRX32_FLIPPED, capCallback, NULL)))
	{
		MessageBox("failed to start capturing", "ERROR", MB_OK);
		killPreviewWindow();
		return;
	}
}




bool
capCallback(CVRES status, CVImage* imagePtr, void* userParam)
{
	::SetDIBitsToDevice(previewWindow.hDC, 0,0, previewWindow.width,previewWindow.height, 0,0, (UINT)0,
						previewWindow.bitmapInfo.bmiHeader.biHeight, imagePtr->GetRawDataPtr(), &previewWindow.bitmapInfo, DIB_RGB_COLORS);

	return true;
}


void killPreviewWindow()
{
	previewWindow.vidCap->Stop();

	if(previewWindow.hDC && !ReleaseDC(previewWindow.hWnd,previewWindow.hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.","WINDOW CLOSE ERROR",MB_OK | MB_ICONINFORMATION);
		previewWindow.hDC=NULL;										// Set DC To NULL
	}

	if(previewWindow.hWnd && !DestroyWindow(previewWindow.hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.","WINDOW CLOSE ERROR", MB_OK | MB_ICONINFORMATION);
		previewWindow.hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass(wndClassName, previewWindow.hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.","WINDOW CLOSE ERROR",MB_OK | MB_ICONINFORMATION);
		previewWindow.hInstance=NULL;									// Set hInstance To NULL
	}

	previewWindow.created = false;
}


BOOL createPreviewWindow(char* title, int width, int height)
{
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	previewWindow.hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= previewWindow.hInstance;				// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= wndClassName;							// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(previewWindow.hWnd=CreateWindowEx(dwExStyle,				// Extended Style For The Window
								wndClassName,						// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								previewWindow.hInstance,			// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		killPreviewWindow();						// Reset The Display
		MessageBox(NULL, "Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}


	if(!(previewWindow.hDC=GetDC(previewWindow.hWnd)))							// Did We Get A Device Context?
	{
		killPreviewWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	previewWindow.width = width;
	previewWindow.height = height;

    ZeroMemory( &previewWindow.bitmapInfo, sizeof(BITMAPINFO) );
    previewWindow.bitmapInfo.bmiHeader.biSize = sizeof( previewWindow.bitmapInfo.bmiHeader );
    previewWindow.bitmapInfo.bmiHeader.biWidth  = width;
    previewWindow.bitmapInfo.bmiHeader.biHeight = height;
    previewWindow.bitmapInfo.bmiHeader.biPlanes = 1;
    previewWindow.bitmapInfo.bmiHeader.biBitCount = 32;
    previewWindow.bitmapInfo.bmiHeader.biCompression = BI_RGB;
    previewWindow.bitmapInfo.bmiHeader.biSizeImage = 0;
    previewWindow.bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
    previewWindow.bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
    previewWindow.bitmapInfo.bmiHeader.biClrUsed = 0;
    previewWindow.bitmapInfo.bmiHeader.biClrImportant = 0;

	previewWindow.bitmapInfo.bmiHeader.biWidth  = width;
	previewWindow.bitmapInfo.bmiHeader.biHeight = height;


	ShowWindow(previewWindow.hWnd,SW_SHOW);			// Show The Window
	SetForegroundWindow(previewWindow.hWnd);		// Slightly Higher Priority
	SetFocus(previewWindow.hWnd);					// Sets Keyboard Focus To The Window

	previewWindow.created = true;

	return TRUE;									// Success
}


LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				previewWindow.active=TRUE;			// Program Is Active
			}
			else
			{
				previewWindow.active=FALSE;			// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
			}
			break;
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			killPreviewWindow();
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
