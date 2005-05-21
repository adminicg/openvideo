// CamViewWinDlg.h : header file
//

#if !defined(AFX_CAMVIEWWINDLG_H__8C4A2C92_58C6_4E94_A6DE_EC3DDB09A755__INCLUDED_)
#define AFX_CAMVIEWWINDLG_H__8C4A2C92_58C6_4E94_A6DE_EC3DDB09A755__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../vidcapture/VidCapture.h"


/////////////////////////////////////////////////////////////////////////////
// CCamViewWinDlg dialog

class CCamViewWinDlg : public CDialog
{
// Construction
public:
	CCamViewWinDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCamViewWinDlg)
	enum { IDD = IDD_CAMVIEWWIN_DIALOG };
	CListBox	m_VideoModes;
	CListBox	m_Cameras;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamViewWinDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	CVVidCapture* vidCap;
	int numDevices;
	int selDevice, selMode;
	CString selDeviceName;

	void initVidCap();
	void formatModeString(CVVidCapture::VIDCAP_MODE& nModeInfo, int idx, char* str);

	CVVidCapture::VIDCAP_MODE modeInfos[256];
	int numModes;

	// Generated message map functions
	//{{AFX_MSG(CCamViewWinDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTest();
	afx_msg void OnSelchangeCameras();
	afx_msg void OnSelchangeVideoModes();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMVIEWWINDLG_H__8C4A2C92_58C6_4E94_A6DE_EC3DDB09A755__INCLUDED_)
