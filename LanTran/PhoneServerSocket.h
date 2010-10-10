#if !defined(AFX_PHONESERVERSOCKET_H__556443AE_9566_44CD_8F09_998C29CDA9BA__INCLUDED_)
#define AFX_PHONESERVERSOCKET_H__556443AE_9566_44CD_8F09_998C29CDA9BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PhoneServerSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CPhoneServerSocket command target
#include "afxsock.h"
class CPhoneDlg;
class CPhoneServerSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CPhoneServerSocket();
	virtual ~CPhoneServerSocket();

// Overrides
public:
	CPhoneDlg  * m_dlg;
	UINT m_uPort;
	BOOL Init(UINT port, CPhoneDlg* dlg);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhoneServerSocket)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CPhoneServerSocket)
    public:
	virtual void OnAccept(int nErrorCode);		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHONESERVERSOCKET_H__556443AE_9566_44CD_8F09_998C29CDA9BA__INCLUDED_)
