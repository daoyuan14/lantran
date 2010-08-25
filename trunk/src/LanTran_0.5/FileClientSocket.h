#if !defined(AFX_FILECLIENTSOCKET_H__A68E049F_8BED_479F_B029_37526DE2468E__INCLUDED_)
#define AFX_FILECLIENTSOCKET_H__A68E049F_8BED_479F_B029_37526DE2468E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileClientSocket.h : header file
//


#include "FileTranDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CFileClientSocket command target

class CFileClientSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CFileClientSocket(CFileTranDlg *pDlg);
	virtual ~CFileClientSocket();

// Overrides
public:
	CFileTranDlg *m_pDlg;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileClientSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CFileClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILECLIENTSOCKET_H__A68E049F_8BED_479F_B029_37526DE2468E__INCLUDED_)
