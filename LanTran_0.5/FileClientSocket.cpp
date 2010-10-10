// FileClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "LanTran.h"
#include "FileClientSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileClientSocket

CFileClientSocket::CFileClientSocket(CFileTranDlg *pDlg)
: m_pDlg(pDlg)
{
}

CFileClientSocket::~CFileClientSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CFileClientSocket, CSocket)
	//{{AFX_MSG_MAP(CFileClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CFileClientSocket member functions

void CFileClientSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_pDlg->SendFileData();	
}
