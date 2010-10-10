// PhoneServerSocket.cpp : implementation file
//

#include "stdafx.h"
#include "lantran.h"
#include "PhoneServerSocket.h"
#include "PhoneDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhoneServerSocket

CPhoneServerSocket::CPhoneServerSocket()
{
}

CPhoneServerSocket::~CPhoneServerSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CPhoneServerSocket, CSocket)
	//{{AFX_MSG_MAP(CPhoneServerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CPhoneServerSocket member functions

BOOL CPhoneServerSocket::Init(UINT port, CPhoneDlg* dlg)
{
	m_uPort=port;
	m_dlg=dlg;
	if(Create(m_uPort)==FALSE)
	{
		AfxMessageBox(CPhoneClientSocket::ErrorReason(GetLastError()));
		return FALSE;
	}
	if(Listen()==FALSE)
	{
		AfxMessageBox(CPhoneClientSocket::ErrorReason(GetLastError()));
		return FALSE;
	}
	m_dlg->SetDlgItemText(IDC_SHOWTEXT,"主机建立成功!");
	return TRUE;
}

void CPhoneServerSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_dlg->ProcessPendingAccept();
	CSocket::OnAccept(nErrorCode);
}
