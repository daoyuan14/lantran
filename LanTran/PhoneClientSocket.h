#if !defined(AFX_PHONECLIENTSOCKET_H__24462203_3C8C_4FCC_BDBD_48EF4467BCF5__INCLUDED_)
#define AFX_PHONECLIENTSOCKET_H__24462203_3C8C_4FCC_BDBD_48EF4467BCF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PhoneClientSocket.h : header file
//

#include "afxsock.h"
#include "PhoneMessg.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneClientSocket command target
class CPhoneDlg;
class CPhoneClientSocket : public CSocket
{
// Attributes
public:
	CArchive* m_aSessionIn;
	CArchive* m_aSessionOut;
	CSocketFile* m_sfSocketFile;
	CPhoneDlg * m_dlg;
	bool m_bInit;
	bool m_bClose;
public:
	void Init(	CPhoneDlg * dlg);
	BOOL SendMessage(CPhoneMessg * msg);
	void CloseSocket();
	
public:
	static int GetLocalHostName(CString &sHostName);	//获得本地计算机名称
	static int GetIpAddress(const CString &sHostName, CString &sIpAddress);//获得本地IP
	static int GetIpAddress(const CString &sHostName, BYTE &f0,BYTE &f1,BYTE &f2,BYTE &f3);//获得本地IP
	static CString ErrorReason(int tag);				//

// Operations
public:
	CPhoneClientSocket();
	virtual ~CPhoneClientSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhoneClientSocket)
    public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CPhoneClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHONECLIENTSOCKET_H__24462203_3C8C_4FCC_BDBD_48EF4467BCF5__INCLUDED_)
