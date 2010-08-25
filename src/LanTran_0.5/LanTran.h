// LanTran.h : main header file for the LANTRAN application
//

#if !defined(AFX_LANTRAN_H__E0687C92_B49B_4027_BA4F_62D468AB2BFB__INCLUDED_)
#define AFX_LANTRAN_H__E0687C92_B49B_4027_BA4F_62D468AB2BFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLanTranApp:
// See LanTran.cpp for the implementation of this class
//

class CLanTranApp : public CWinApp
{
public:
	CLanTranApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLanTranApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLanTranApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LANTRAN_H__E0687C92_B49B_4027_BA4F_62D468AB2BFB__INCLUDED_)
