// ChatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LanTran.h"
#include "ChatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChatDlg dialog

UINT ChatReceive( LPVOID param )
{
	CChatDlg *pChatDlg = (CChatDlg *)param;

	pChatDlg->m_sockChat = socket(AF_INET,SOCK_DGRAM,0);
	if(	INVALID_SOCKET==pChatDlg->m_sockChat )
	{
		AfxMessageBox("聊天用套接字创建失败.");
		pChatDlg->m_SendChat.EnableWindow(false);       //初始化都失败了，当然不能让它聊天了
		pChatDlg->m_Clear.EnableWindow(false);
		return -1;
	}

	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_addr.S_un.S_addr	= htonl(	INADDR_ANY	);
	serverAddr.sin_family			= AF_INET;
	serverAddr.sin_port				= htons(	CHATPORT	);
	
	int	nError;
	nError = bind(pChatDlg->m_sockChat, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR));
	if(	SOCKET_ERROR==nError	)
	{
		AfxMessageBox("服务器端聊天用套接字绑定失败。");
        pChatDlg->m_SendChat.EnableWindow(false);
		pChatDlg->m_Clear.EnableWindow(false);
		return -1;
	}
	
	pChatDlg->m_SendChat.EnableWindow(true);
	pChatDlg->m_Clear.EnableWindow(true);

	int	len = sizeof(SOCKADDR),errCode;
	char strChat[1024];

	while( true )
	{
		CString	strChatOld;
		memset(strChat, 0, 1024);
		strChatOld.Empty();

		errCode = recvfrom(pChatDlg->m_sockChat, strChat, 1024, 0, (SOCKADDR*)&serverAddr, &len);
		if(SOCKET_ERROR==errCode)   //SOCKET_ERROR写在左边是有道理的
		{
			//最好添加错误分析
			continue;
		}
        /*--------------更新聊天内容---------------------*/
		pChatDlg->GetDlgItemText(IDC_EDIT_RECEIVE, strChatOld);   //得到现有的值   可以这么得？

		strChatOld += "\r\n";
		strChatOld += strChat;
		pChatDlg->SetDlgItemText(IDC_EDIT_RECEIVE, strChatOld);   //设置成新的
		pChatDlg->GetDlgItem(IDC_EDIT_RECEIVE)->
			SendMessage(WM_VSCROLL,SB_BOTTOM);
		Sleep(55);
	}
	return 0;
}



//////////////////////////////////////////////////////////////////////////////
CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChatDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatDlg)
	DDX_Control(pDX, IDC_BUTTON_SENDCHAT, m_SendChat);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_Clear);
	DDX_Control(pDX, IDC_EDIT_SANDCHAT, m_editSendChat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialog)
	//{{AFX_MSG_MAP(CChatDlg)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_SENDCHAT, OnButtonSendChat)
	ON_EN_MAXTEXT(IDC_EDIT_RECEIVE, OnMaxtextEditReceive)
	ON_EN_MAXTEXT(IDC_EDIT_SANDCHAT, OnMaxtextEditSandChat)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatDlg message handlers

void CChatDlg::OnButtonClear() 
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_RECEIVE, "");
}

void CChatDlg::OnButtonSendChat() 
{
	// TODO: Add your control notification handler code here
	SOCKADDR_IN		addrDest;
	int				len = sizeof(SOCKADDR);
	char			strSend[1024], strTemp[800];
	CString			selectedIP;
	memset(strSend, 0, 1024);
	memset(strTemp, 0, 800);
	memset(&addrDest, 0, sizeof(SOCKADDR_IN));

	if( !GetSelectedIpAddr(selectedIP, this) )
	{
		return;
	}

	addrDest.sin_addr.S_un.S_addr	=inet_addr(	selectedIP );
	addrDest.sin_family				=AF_INET;
	addrDest.sin_port				=htons(	CHATPORT );

	GetDlgItemText(IDC_EDIT_SANDCHAT, strTemp, 800);
	CString		strTime;
	CTime		tm = CTime::GetCurrentTime();
	strTime = tm.Format("%H:%M:%S");	
	
	sprintf(strSend, "%s\t%s:\r\n%s", m_localUserName, strTime, strTemp);   //把所有要发送的信息保存在send中

	int nError = sendto(m_sockChat, strSend, strlen(strSend)+1, 0, (SOCKADDR*)&addrDest, sizeof(SOCKADDR));
	if(SOCKET_ERROR==nError)
	{
		MessageBox("发送聊天消息失败。\t");
		return;
	}

	/*-----------------因为自己这边的显示信息也要改-----------------*/
	CString	strOld;
	GetDlgItemText(IDC_EDIT_RECEIVE, strOld);
	strOld += "\r\n";
	strOld += strSend;
	SetDlgItemText(IDC_EDIT_RECEIVE, strOld);
	SetDlgItemText(IDC_EDIT_SANDCHAT, "");	 //清空  Edit  控件内容
	GetDlgItem(IDC_EDIT_RECEIVE)->
		SendMessage(WM_VSCROLL,SB_BOTTOM);
}

void CChatDlg::OnMaxtextEditReceive() 
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_RECEIVE, "");
}

void CChatDlg::OnMaxtextEditSandChat() 
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_SANDCHAT, "");	
}

BOOL CChatDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	unsigned long nameLen = sizeof(m_localUserName);
	if ( GetUserName(m_localUserName, &nameLen)==0 )     //nameLen前加个&
	{
		strcpy(m_localUserName, "Anony");
	}

	AfxBeginThread(ChatReceive, this);    //其实就是把this当作参数传进去
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChatDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if ( closesocket(m_sockChat) != 0 )
		AfxMessageBox("Close Socket m_pChatDlg->m_sockChat - Failed!");

	// 是不是仅仅只有AfxBeginThread，而没有中止线程的?
	// 我cao，还是没用。。。
	AfxEndThread(0, TRUE);
}
