// CamViewWin.h : main header file for the CAMVIEWWIN application
//

#if !defined(AFX_CAMVIEWWIN_H__45DC9B1A_D370_457B_A420_00FA461E9C11__INCLUDED_)
#define AFX_CAMVIEWWIN_H__45DC9B1A_D370_457B_A420_00FA461E9C11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCamViewWinApp:
// See CamViewWin.cpp for the implementation of this class
//

class CCamViewWinApp : public CWinApp
{
public:
	CCamViewWinApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamViewWinApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCamViewWinApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMVIEWWIN_H__45DC9B1A_D370_457B_A420_00FA461E9C11__INCLUDED_)
