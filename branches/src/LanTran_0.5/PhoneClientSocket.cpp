// PhoneClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "lantran.h"
#include "PhoneClientSocket.h"
#include "PhoneDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhoneClientSocket

CPhoneClientSocket::CPhoneClientSocket()
{
	m_aSessionIn=NULL;
	m_aSessionOut=NULL;
	m_sfSocketFile=NULL;
	m_bInit=false;
	m_bClose=false;
}

CPhoneClientSocket::~CPhoneClientSocket()
{
	if(m_aSessionIn)
		delete m_aSessionIn;
	if(m_aSessionOut)
		delete m_aSessionOut;
	if(m_sfSocketFile)
		delete m_sfSocketFile;
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CPhoneClientSocket, CSocket)
	//{{AFX_MSG_MAP(CPhoneClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CPhoneClientSocket member functions

void CPhoneClientSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CSocket::OnReceive(nErrorCode);
	do
	{
		CPhoneMessg temp;
		temp.Serialize(*m_aSessionIn);
		m_dlg->m_sMsgList+=temp.m_strText;
		m_dlg->SetDlgItemText(IDC_SHOWTEXT,m_dlg->m_sMsgList);
		
		if(temp.m_tag==1&&m_dlg->m_willchating==FALSE)
			//如果有声音过来并且本机的声音设备已经准备好了则首先在本机发出声音
		{	
			memcpy(m_dlg->m_sound.m_cBufferOut,temp.m_buffer,MAX_BUFFER_SIZE);
		}
		int linenum=((CEdit *)(m_dlg->GetDlgItem(IDC_SHOWTEXT)))->GetLineCount();
		((CEdit *)(m_dlg->GetDlgItem(IDC_SHOWTEXT)))->LineScroll(linenum);
		if(!m_dlg->m_bClient)
		{
			for(POSITION pos=m_dlg->m_connectionList.GetHeadPosition();pos!=NULL;)
			{
				CPhoneClientSocket * t = (CPhoneClientSocket*)m_dlg->m_connectionList.GetNext(pos);
				if(t->m_hSocket!=this->m_hSocket)
				{
					t->SendMessage(&temp);
				}
			}
		}
	}
	while (!m_aSessionIn->IsBufferEmpty());
}

void CPhoneClientSocket::Init(CPhoneDlg * dlg)
{
	m_sfSocketFile= new CSocketFile(this);
	m_aSessionIn=new CArchive(m_sfSocketFile,CArchive::load);
	m_aSessionOut=new CArchive(m_sfSocketFile,CArchive::store);
	m_bClose=false;
	this->m_dlg=dlg;
}
BOOL CPhoneClientSocket::SendMessage(CPhoneMessg * msg)
{
	if (m_aSessionOut != NULL)
	{
		msg->Serialize(*m_aSessionOut);
		m_aSessionOut->Flush();
		return TRUE;
	}
	else
	{
		//对方关闭了连接
		m_bClose=true;
		CloseSocket();
		m_dlg->CloseSessionSocket();
		return FALSE;
	}
}


void CPhoneClientSocket::CloseSocket()
{
	if(m_aSessionIn)
	{
		delete m_aSessionIn;
		m_aSessionIn=NULL;
	}
	if(m_aSessionOut)
	{
		delete m_aSessionOut;
		m_aSessionOut=NULL;
	}
	if(m_sfSocketFile)
	{
		delete m_aSessionOut;
		m_sfSocketFile=NULL;
	}
	Close();
	m_bInit=false;
	m_bClose=true;
}

void CPhoneClientSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_bClose=true;
	CloseSocket();
	m_dlg->CloseSessionSocket();	
	CSocket::OnClose(nErrorCode);
}

int CPhoneClientSocket::GetLocalHostName(CString &sHostName)	//获得本地计算机名称
{
	char szHostName[256];
	int nRetCode;
	nRetCode=gethostname(szHostName,sizeof(szHostName));
	if(nRetCode!=0)
	{
		//产生错误
		sHostName=_T("没有取得");
		return GetLastError();
	}
	sHostName=szHostName;
	return 0;
}

int CPhoneClientSocket::GetIpAddress(const CString &sHostName, CString &sIpAddress)//获得本地IP
{
	struct hostent FAR * lpHostEnt=gethostbyname(sHostName);
	if(lpHostEnt==NULL)
	{
		//产生错误
		sIpAddress=_T("");
		return GetLastError();
	}
	//获取IP
	LPSTR lpAddr=lpHostEnt->h_addr_list[0];
	if(lpAddr)
	{
		struct in_addr inAddr;
		memmove(&inAddr,lpAddr,4);
		//转换为标准格式
		sIpAddress=inet_ntoa(inAddr);
		if(sIpAddress.IsEmpty())
			sIpAddress=_T("没有取得");
	}
	return 0;
}
int CPhoneClientSocket::GetIpAddress(const CString &sHostName, BYTE &f0,BYTE &f1,BYTE &f2,BYTE &f3)//获得本地IP
{
	struct hostent FAR * lpHostEnt=gethostbyname(sHostName);
	if(lpHostEnt==NULL)
	{
		//产生错误
		f0=f1=f2=f3=0;
		return GetLastError();
	}
	//获取IP
	LPSTR lpAddr=lpHostEnt->h_addr_list[0];
	if(lpAddr)
	{
		struct in_addr inAddr;
		memmove(&inAddr,lpAddr,4);
		f0=inAddr.S_un.S_un_b.s_b1;
		f1=inAddr.S_un.S_un_b.s_b2;
		f2=inAddr.S_un.S_un_b.s_b3;
		f3=inAddr.S_un.S_un_b.s_b4;
	}
	return 0;
}

CString CPhoneClientSocket::ErrorReason(int tag)
{
	CString result;
//======================================================================
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		tag,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL);
	result = (UCHAR *)lpMsgBuf;
	LocalFree( lpMsgBuf );
//====================================================================================
	return result;
}

