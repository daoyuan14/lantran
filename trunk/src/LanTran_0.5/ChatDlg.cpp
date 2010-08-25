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
		AfxMessageBox("�������׽��ִ���ʧ��.");
		pChatDlg->m_SendChat.EnableWindow(false);       //��ʼ����ʧ���ˣ���Ȼ��������������
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
		AfxMessageBox("���������������׽��ְ�ʧ�ܡ�");
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
		if(SOCKET_ERROR==errCode)   //SOCKET_ERRORд��������е����
		{
			//�����Ӵ������
			continue;
		}
        /*--------------������������---------------------*/
		pChatDlg->GetDlgItemText(IDC_EDIT_RECEIVE, strChatOld);   //�õ����е�ֵ   ������ô�ã�

		strChatOld += "\r\n";
		strChatOld += strChat;
		pChatDlg->SetDlgItemText(IDC_EDIT_RECEIVE, strChatOld);   //���ó��µ�
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
	
	sprintf(strSend, "%s\t%s:\r\n%s", m_localUserName, strTime, strTemp);   //������Ҫ���͵���Ϣ������send��

	int nError = sendto(m_sockChat, strSend, strlen(strSend)+1, 0, (SOCKADDR*)&addrDest, sizeof(SOCKADDR));
	if(SOCKET_ERROR==nError)
	{
		MessageBox("����������Ϣʧ�ܡ�\t");
		return;
	}

	/*-----------------��Ϊ�Լ���ߵ���ʾ��ϢҲҪ��-----------------*/
	CString	strOld;
	GetDlgItemText(IDC_EDIT_RECEIVE, strOld);
	strOld += "\r\n";
	strOld += strSend;
	SetDlgItemText(IDC_EDIT_RECEIVE, strOld);
	SetDlgItemText(IDC_EDIT_SANDCHAT, "");	 //���  Edit  �ؼ�����
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
	if ( GetUserName(m_localUserName, &nameLen)==0 )     //nameLenǰ�Ӹ�&
	{
		strcpy(m_localUserName, "Anony");
	}

	AfxBeginThread(ChatReceive, this);    //��ʵ���ǰ�this������������ȥ
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChatDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if ( closesocket(m_sockChat) != 0 )
		AfxMessageBox("Close Socket m_pChatDlg->m_sockChat - Failed!");

	// �ǲ��ǽ���ֻ��AfxBeginThread����û����ֹ�̵߳�?
	// ��cao������û�á�����
	AfxEndThread(0, TRUE);
}
