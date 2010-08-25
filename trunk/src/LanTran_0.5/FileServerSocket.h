#if !defined(AFX_FILESERVERSOCKET_H__DE7E94FD_DF72_407F_982D_C5A3E8ABDFF9__INCLUDED_)
#define AFX_FILESERVERSOCKET_H__DE7E94FD_DF72_407F_982D_C5A3E8ABDFF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileServerSocket.h : header file
//


#include "FileTranDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CFileServerSocket command target

class CFileServerSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CFileServerSocket(CFileTranDlg *pDlg);
	virtual ~CFileServerSocket();

// Overrides
public:
	CFileTranDlg *m_pDlg;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileServerSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CFileServerSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILESERVERSOCKET_H__DE7E94FD_DF72_407F_982D_C5A3E8ABDFF9__INCLUDED_)
