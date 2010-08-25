// FileServerSocket.cpp : implementation file
//

#include "stdafx.h"
#include "LanTran.h"
#include "FileServerSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileServerSocket

CFileServerSocket::CFileServerSocket(CFileTranDlg *pDlg)
: m_pDlg(pDlg)
{
}

CFileServerSocket::~CFileServerSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CFileServerSocket, CSocket)
	//{{AFX_MSG_MAP(CFileServerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CFileServerSocket member functions

void CFileServerSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	m_pDlg->AcceptConnection();	
}

void CFileServerSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	m_pDlg->ReceiveData();
}
