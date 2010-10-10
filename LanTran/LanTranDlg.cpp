// LanTranDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LanTran.h"
#include "LanTranDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLanTranDlg dialog
vector<ClientInfo> g_AcceptClient; //ȫ�ֱ�������������ͻ�����Ϣ

bool GetSelectedIpAddr(CString &s, LPVOID lparam)   //��ȡѡ�е�IP��ַ��   ����lparam�в��У�  ���õ�ǿ������������
{
	//CLanTranDlg *pLanTranDlg = (CLanTranDlg *)lparam;   �Ǹ������Ǹ�������õ�
    //CLanTranDlg *pLanTranDlg = new CLanTranDlg;
	CLanTranDlg *pLanTranDlg;
	pLanTranDlg = (CLanTranDlg *)(AfxGetApp()->m_pMainWnd);

	bool bChoosed  = false;
	int	nListIndex = -1;
	int	nListCount = pLanTranDlg->m_ClientInfoList.GetItemCount();
	for(int	i=0; i<nListCount; i++)
	{
		if( pLanTranDlg->m_ClientInfoList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		{
			bChoosed = true;		//���Ѿ�ѡ����
			nListIndex = i;		    //�Ѿ�ѡ�������š�
			break;                  //����ֻ��һ��ѡ��һ��
		}
	}
	if(!bChoosed)		//�û���û��ѡ��
	{
		AfxMessageBox("�㻹û��ѡ���͵�Ŀ���û��أ���ѡ��������·��͡�");
		return	false;
	}
	s = pLanTranDlg->m_ClientInfoList.GetItemText(nListIndex, 2);
	return	true;
}

BOOL isAnti;
/////////////////////////////////////////////////////////////////////////////
CLanTranDlg::CLanTranDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLanTranDlg::IDD, pParent)
{
	m_nOpenState = 0;
	//{{AFX_DATA_INIT(CLanTranDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLanTranDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLanTranDlg)
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	DDX_Control(pDX, IDC_LIST2, m_ClientInfoList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLanTranDlg, CDialog)
	//{{AFX_MSG_MAP(CLanTranDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON_HIDE, OnButtonHide)
	ON_MESSAGE(WM_SHOWTASK, OnShowTask)    //���Զ������Ϣ�������������巽�����в�ͬ
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLanTranDlg message handlers

BOOL CLanTranDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
/*----------------------Tab Control��ʵ��-----------------------------------------*/
	m_ImageList.Create(16, 16, ILC_COLOR | ILC_MASK, 5, 0);
	m_ImageList.Add(AfxGetApp() -> LoadIcon(IDI_ICON1));                   //���ⲿ����ʱ�ǵñ���reload
    m_ImageList.Add(AfxGetApp() -> LoadIcon(IDI_ICON2));
	m_ImageList.Add(AfxGetApp() -> LoadIcon(IDI_ICON3));
	m_ImageList.Add(AfxGetApp() -> LoadIcon(IDI_ICON4));
	m_ImageList.Add(AfxGetApp() -> LoadIcon(IDI_ICON5));
	
	m_Tab.SetImageList(&m_ImageList);
	m_Tab.InsertItem(0, "���ٴ���", 0);                 //��α��Щ��
	m_Tab.InsertItem(1, "���ܴ���", 1);
	m_Tab.InsertItem(2, "����ɱ��", 2);
	m_Tab.InsertItem(3, "��������", 3);
	m_Tab.InsertItem(4, "��������", 4);
	
	m_pFileTranDlg = new CFileTranDlg;
	m_pFileTranDlg -> Create(IDD_DIALOG_FILETRAN, GetDlgItem(IDC_STATIC_DLG));
	m_pSafeTranDlg = new CSafeTranDlg;
	m_pSafeTranDlg -> Create(IDD_DIALOG_SAFETRAN, GetDlgItem(IDC_STATIC_DLG));
	m_pAntiVirusDlg = new CAntiVirusDlg;
	m_pAntiVirusDlg -> Create(IDD_DIALOG_ANTIVIRUS, GetDlgItem(IDC_STATIC_DLG));
	m_pChatDlg = new CChatDlg;
	m_pChatDlg -> Create(IDD_DIALOG_CHAT, GetDlgItem(IDC_STATIC_DLG));
	m_pPhoneDlg = new CPhoneDlg;
	m_pPhoneDlg->Create(IDD_DIALOG_PHONE, GetDlgItem(IDC_STATIC_DLG));
	
	DoTab(0);
/*----------------------Tab Control��ʵ��-----------------------------------------*/

/*----------------------List Control��ʵ��----------------------------------------*/
	m_ClientInfoList.SetExtendedStyle(
		m_ClientInfoList.GetExtendedStyle()    //������
		|LVS_EX_FLATSB            //��ƽ��������
		|LVS_EX_FULLROWSELECT    //��������ѡ��
		|LVS_EX_HEADERDRAGDROP   //���������ק
		|LVS_EX_ONECLICKACTIVATE //������ʾ
		|LVS_EX_GRIDLINES        //����������
		);
	
	m_ClientInfoList.InsertColumn(0, "�û���",   LVCFMT_LEFT, 120, 0);
	m_ClientInfoList.InsertColumn(1, "�������", LVCFMT_LEFT, 130, 1);
	m_ClientInfoList.InsertColumn(2, "IP��ַ",   LVCFMT_LEFT, 150, 2);
	
/*----------------------List Control��ʵ��----------------------------------------*/

	StartLanShow();
	AddAllClientInfo();			// add the current Client Info in g_AcceptClient

	isAnti = TRUE;  //Ĭ�Ϲ���
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLanTranDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLanTranDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
//===================================================================================================================	
	if(!m_pSafeTranDlg->path.IsEmpty())
	{
		CRect rc;
		m_pSafeTranDlg->m_picture.GetClientRect(&rc);
		CDC *pDC=m_pSafeTranDlg->m_picture.GetDC();
		HBITMAP hbmp=(HBITMAP)::LoadImage(AfxGetInstanceHandle(),m_pSafeTranDlg->path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_DEFAULTCOLOR|LR_DEFAULTSIZE);
		BITMAP bm;
		GetObject(hbmp,sizeof(bm),&bm);
		CDC memdc;
		memdc.CreateCompatibleDC(pDC);
		memdc.SelectObject(hbmp);
		pDC->StretchBlt(0,0,rc.Width(),rc.Height(),&memdc,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
	}
//===============================================================================================================================
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLanTranDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLanTranDlg::SetDlgState(CWnd *pWnd, BOOL bShow)
{
	pWnd -> EnableWindow(bShow);
	if (bShow)
	{
		pWnd -> ShowWindow(SW_SHOW);
		pWnd -> CenterWindow();
	}
	else
	{
		pWnd -> ShowWindow(SW_HIDE);
	}
}

void CLanTranDlg::DoTab(int nSel)
{
	if (nSel > 4)           //ȷ��nSelֵ���ܳ�����Χ
	{
		nSel = 4;
	}
	if (nSel < 0)
	{
		nSel = 0;
	}
	BOOL bTab[5];
	bTab[0] = bTab[1] = bTab[2] = bTab[3] = bTab[4] = FALSE;
	bTab[nSel] = TRUE;
	
	SetDlgState(m_pFileTranDlg,  bTab[0]);
	SetDlgState(m_pSafeTranDlg,  bTab[1]);
	SetDlgState(m_pAntiVirusDlg, bTab[2]);
	SetDlgState(m_pChatDlg,      bTab[3]);
	SetDlgState(m_pPhoneDlg,     bTab[4]);
}

void CLanTranDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nSelect = m_Tab.GetCurSel();
	if (nSelect >= 0)
	{
		DoTab(nSelect);
	}
	
	*pResult = 0;
}

void CLanTranDlg::OnButtonHide() 
{
	// TODO: Add your control notification handler code here
	ToTray();	
}

void CLanTranDlg::ToTray()
{
	NOTIFYICONDATA nid;
    nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd   = this->m_hWnd;
	nid.uID    = IDR_MAINFRAME; 
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP ;
	nid.uCallbackMessage = WM_SHOWTASK;                  //�Զ������Ϣ���� 
	nid.hIcon  = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
	strcpy(nid.szTip, "LanTran");         //��Ϣ��ʾ��
	Shell_NotifyIcon(NIM_ADD, &nid);                     //�����������ͼ�� 
	m_nOpenState = 1;                                    //����״̬�ɵ�ϵͳ����
	ShowWindow(SW_HIDE);                                 //����������
}

LRESULT CLanTranDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	if(wParam != IDR_MAINFRAME)
	{
		return 1;
	}
	switch(lParam)
	{ 
	case WM_RBUTTONUP:                          //�Ҽ�����ʱ������ݲ˵�������ֻ��һ�����رա�
		{ 
			LPPOINT lpoint = new tagPOINT;
			::GetCursorPos(lpoint);             //�õ����λ�� 
			CMenu menu;
			menu.CreatePopupMenu();             //����һ������ʽ�˵�,���Ӳ˵���رա������������ϢWM_DESTROY�������ڣ������أ��������������
			menu.AppendMenu(MF_STRING,WM_DESTROY,"�ر�"); //ȷ������ʽ�˵���λ�� 
			menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this); //��Դ���� 
			HMENU hmenu=menu.Detach();
			menu.DestroyMenu(); 
			delete lpoint;
		}
		break; 
	case WM_LBUTTONDBLCLK://˫������Ĵ���
		{
			this->ShowWindow(SW_SHOW);//�򵥵���ʾ���������¶� 
			DeleteTray();
		}
		break;
	default: 
		break;
	}
	return 0;
}

void CLanTranDlg::DeleteTray()
{
	NOTIFYICONDATA nid; 
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA); 
	nid.hWnd   = this -> m_hWnd;
	nid.uID    = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP ;
	nid.uCallbackMessage = WM_SHOWTASK;                //�Զ������Ϣ���� 
	nid.hIcon  = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)); 
	strcpy(nid.szTip, "LanTran");                      //��Ϣ��ʾ��Ϊ���ƻ��������ѡ� 
	Shell_NotifyIcon(NIM_DELETE, &nid);                //��������ɾ��ͼ��
}

void CLanTranDlg::OnDestroy() 
{
	CDialog::OnDestroy();	
	// TODO: Add your message handler code here
	SendLanShowData(EXIT);
//	delete this;һ��Ҫ��

/*----------ɾ�����ļ����׽��֡�������m_pFileTranDlg������ǰ��-------------*/
	m_pFileTranDlg->DeleteFileSock();
/*----------ɾ�����ļ����׽���-------------*/

	if (m_pFileTranDlg)
	{
		delete m_pFileTranDlg;
	}
	if (m_pSafeTranDlg)
	{
		delete m_pSafeTranDlg;
	}
	if (m_pAntiVirusDlg)
	{
		delete m_pAntiVirusDlg;
	}

	// ע�͵�������ξͲ���Debug Assertion Failed��
	// ��������Ϊm_pChatDlg�к����׽���ûɾ����
	if (m_pChatDlg)
	{
		delete m_pChatDlg;
	}

	if (m_pPhoneDlg)
	{
		delete m_pPhoneDlg;
	}
	
	if(m_nOpenState == 1)
	{
		DeleteTray();
	}
}

// StartLanShow()�������̺߳�����ֻ�Ǹ����̵���
// ������������CreateThread()������һ���߳�
void CLanTranDlg::StartLanShow()
{
	BOOL bIni = InitLanShowSocket();		// init and bind a Listen Socket
	if ( bIni == FALSE )
		return;

	InitMyClientInfo();						// add own ip info to Global Variable: g_AcceptClient
	SendLanShowData(ENTRY);					// �㲥���͸����ߵ���Ϣ, ���߱���������

	HANDLE hThread = CreateThread(NULL, 0, LanShowThread, this, CREATE_SUSPENDED, &m_id);
	ResumeThread(hThread);					// run thread
	
	CloseHandle(hThread);
}

// use WinSock to create, bind socket
BOOL CLanTranDlg::InitLanShowSocket()
{
	m_LanShowSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if ( m_LanShowSocket == INVALID_SOCKET )
	{
		AfxMessageBox("Creating listenSocket is failed!");
		return FALSE;
	}

	GetLocalIP(m_dwLocalIP);

	m_listenAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	m_listenAddr.sin_port = htons(NET_LANSHOW_PORT);
	m_listenAddr.sin_family = AF_INET;

	int iBind = bind(m_LanShowSocket, (struct sockaddr FAR *) &m_listenAddr, sizeof(SOCKADDR_IN));
	if ( iBind == SOCKET_ERROR )
	{
		AfxMessageBox("binging is failed!");
		return FALSE ;
	}
	return TRUE;
}

void CLanTranDlg::GetLocalIP(DWORD &dwLocalIP)
{
	struct in_addr in;
	struct hostent *thisHost;

	if ( gethostname(m_hostName, sizeof(m_hostName)) == SOCKET_ERROR )
	{
		AfxMessageBox("can not get local name!");
		return;
	}

	thisHost = gethostbyname(m_hostName);
	if ( thisHost == NULL )
	{
		return ;
	}

	int i = 0;
	while( thisHost->h_addr_list[i] )                //�Ǹ��ṹ�帳ֵ�𣿲��Ǻܶ�
	{
		in.S_un.S_addr = *(unsigned long *) thisHost->h_addr_list[i];
		i++;
	}

	dwLocalIP = in.S_un.S_addr;                     //��Ϊ�����ã�����m_dwLocalIPҲ�ı���
	sprintf(m_localIPAddr,"%d.%d.%d.%d"             //m_localIPAddr��"%d.%d.%d.%d"��������ʽ
		                 ,dwLocalIP&0x000000ff
		                 ,dwLocalIP>>8&0x0000000ff
		                 ,dwLocalIP>>16&0x000000ff
		                 ,dwLocalIP>>24&0x000000ff
				          );
}

void CLanTranDlg::InitMyClientInfo()                // �ѱ�����IP����Ϣ���ӵ�g_AcceptClient��
{
	ClientInfo LocalInfo;						// ���ڱ��汾����Ϣ�����ݽṹ
	strcpy(LocalInfo.m_ClientIPAddr, m_localIPAddr);
	strcpy(LocalInfo.m_ComputerName, m_hostName);

	DWORD dwNameLen = sizeof ( m_TLocalUserName );
	GetUserName(m_TLocalUserName, &dwNameLen);
	strcpy(LocalInfo.m_UserName, m_TLocalUserName);

	g_AcceptClient.push_back(LocalInfo);            // ���Լ�����Ϣ����g_AcceptClient��
}

// ���Լ���ip info������, �����վ����������������û�����Ϣ
DWORD WINAPI CLanTranDlg::LanShowThread(LPVOID lParam)
{
	CLanTranDlg *pLanTranDlg = (CLanTranDlg *)lParam;		// ��Ϊ���Ǿ�̬������
	FD_SET fdR;
	struct timeval timeouts;

	while( true )
	{
		timeouts.tv_sec  = 1;         //Time value, in seconds. 
		timeouts.tv_usec = 0;         //Time value, in microseconds.
		FD_ZERO(&fdR);
		FD_SET(pLanTranDlg -> m_LanShowSocket, &fdR);     // ����pAcceptClient -> m_listenSocket�൱���ļ������������ã�Ϊ����
		switch( select ( pLanTranDlg->m_LanShowSocket + 1, &fdR, NULL, NULL, &timeouts) )
		{
		case -1:
			break;
		case 0:
			{							// ��������Ϊ��iEr����ͨ��
			int iEr = WSAGetLastError();
			break;
			}
		default:
			if ( FD_ISSET(pLanTranDlg -> m_LanShowSocket, &fdR) )
			{
				pLanTranDlg -> RecvLanShowData();
				pLanTranDlg -> AddAllClientInfo();
			}
		}
	}
	return 0L;
}


void CLanTranDlg::SendLanShowData(int nState)
{
	char BroadCastIP[20];                        //ͨ����ȡ�Լ���IP���ó�����������Ӧ�е�IP
	sprintf(BroadCastIP,"%d.%d.%d.255"           //Write formatted data to a string.
		               ,m_dwLocalIP & 0x000000ff
					   ,m_dwLocalIP >> 8 & 0x0000000ff
					   ,m_dwLocalIP >> 16& 0x000000ff
						);

	SOCKADDR_IN AddrSendToClient;
	AddrSendToClient.sin_addr.S_un.S_addr = inet_addr(BroadCastIP);
	AddrSendToClient.sin_family = AF_INET;
	AddrSendToClient.sin_port = htons(NET_LANSHOW_PORT);

	LanShowData *SendingInfo = new LanShowData;       //�洢������������Ϣ
	memset(SendingInfo, 0, sizeof(LanShowData));     //��SendingInfo����ֵΪ0

	SendingInfo->nState = nState;

	strcpy(SendingInfo->m_name, m_TLocalUserName);       //�û���
	strcpy(SendingInfo->m_iPAddr, m_localIPAddr);
	struct hostent *thisHost = gethostbyname(m_hostName);   //������
	if (thisHost != NULL)
	{
		strcpy(SendingInfo->m_computerName, thisHost->h_name);
	}

	int iSend = sendto(m_LanShowSocket,(char FAR *)SendingInfo,sizeof(LanShowData),0,(struct sockaddr FAR *)&AddrSendToClient,sizeof(SOCKADDR_IN));    //���������ڵ������û������Լ���SendingInfo
	int iError = WSAGetLastError();

	delete SendingInfo;       //��Ȼ������ġ�
	if ( iSend == SOCKET_ERROR )
	{
		AfxMessageBox("sending initial info is failed!");
		return ;
	}

}

void CLanTranDlg::RecvLanShowData()
{
	SOCKADDR_IN	from;
	int addrLen = sizeof(SOCKADDR_IN);
	char RecvDataInfo[NET_LANSHOW_BUFFER];

	int iRecv = recvfrom(m_LanShowSocket, RecvDataInfo, sizeof(RecvDataInfo), 0, (struct sockaddr FAR *)&from, &addrLen);
	if ( iRecv == SOCKET_ERROR )
	{
		AfxMessageBox("Recving ClientInfo is failed!");
		return;
	}

	if ( from.sin_addr.S_un.S_addr != m_dwLocalIP )						// ��Ⱥ���������Լ�Ҳ���յ�
	{
		LanShowData *OneClientInfo = ( LanShowData *)RecvDataInfo;		// �ܹؼ�; ���͵ľ���ClientInfo *���͵ġ�

		vector<ClientInfo>::iterator ts;

		/*
		 * handle LanShowData: "EXIT"
		 */
		if (OneClientInfo->nState == EXIT)							// EXIT����������Ĵ���Ƚϲ�ͬ����������������������һЩ
		{
			for( ts = g_AcceptClient.begin(); ts != g_AcceptClient.end(); ts ++ )
			{
				if ( (strcmp(ts->m_ClientIPAddr, OneClientInfo->m_iPAddr) == 0) )    //Compare strings.=0ʱ���
				{
					g_AcceptClient.erase(ts);
					break;
				}
			}
			delete OneClientInfo;     //��ɾ�ĸ�ֵ��OneClientInfo����RecvDataInfo
			return;    //�����������ϢҲ�Ͳ����ж���
		}

		/*
		 * handle LanShowData: "ENTRY" and judge no other message
		 */
		switch (OneClientInfo->nState)       //��switchд����Ч��Щ��
		{
		case ENTRY:
			SendLanShowData(ONLINE);
			break;

		case ONLINE:			// ����ֻҪ���������Ϣ��ӽ�����ok��
			break;
			
		default:
			AfxMessageBox("�յ��������������Ϣ");
			return;				// ���ٴ����������
		}

		/*
		 * so the LanShowData must be "ENTRY" or "ANENTRY"
		 *
		 * it's time to add info to g_AcceptClient
		 * 
		 * �ȿ���g_AcceptClient���Ƿ��Ѿ�����, ����еĻ���ֱ�Ӷ�����
		 */
		for( ts = g_AcceptClient.begin(); ts != g_AcceptClient.end(); ts ++ )
		{
			if ( (strcmp(ts->m_ClientIPAddr, OneClientInfo->m_iPAddr) == 0) )     //Compare strings.=0ʱ���
			{
				delete OneClientInfo;
				return;
			}
		}

		/*
		 * ˵��g_AcceptClient�л�û��, ��Ҫ��������
		 */
		ClientInfo OneManClientInfo;
		strcpy(OneManClientInfo.m_UserName, OneClientInfo->m_name);
		strcpy(OneManClientInfo.m_ComputerName, OneClientInfo->m_computerName);
		strcpy(OneManClientInfo.m_ClientIPAddr, OneClientInfo->m_iPAddr);

		g_AcceptClient.push_back(OneManClientInfo);

		delete OneClientInfo;
	}
}

void CLanTranDlg::AddAllClientInfo()
{
	vector<ClientInfo>::iterator ts;          //terator����vector���ֿռ��ж����������ָ���һ�ֽṹ
	
	m_ClientInfoList.DeleteAllItems();           //ˢ�µ�ʱ��������б�
	
	int i = 0;
	for(ts = g_AcceptClient.begin(); ts != g_AcceptClient.end(); ts ++)
	{
		m_ClientInfoList.InsertItem(i, ts->m_UserName);
		m_ClientInfoList.SetItemText(i, 1, ts->m_ComputerName);
		m_ClientInfoList.SetItemText(i, 2, ts->m_ClientIPAddr);
		
		i++;         //i�����б��е�˳��
	}
}

void CLanTranDlg::OnButtonNew() 
{
	// TODO: Add your control notification handler code here
	SendLanShowData(ONLINE);
	Sleep(200);        //Ϊ�˷�ֹ�����
	AddAllClientInfo();
}

BOOL CLanTranDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message==WM_KEYDOWN && (int)pMsg->wParam==VK_ESCAPE)   //��ֹ��Esc���رնԻ���
	{
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
