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
vector<ClientInfo> g_AcceptClient; //全局变量，用来管理客户端信息

bool GetSelectedIpAddr(CString &s, LPVOID lparam)   //获取选中的IP地址。   不用lparam行不行？  引用的强大作用体现了
{
	//CLanTranDlg *pLanTranDlg = (CLanTranDlg *)lparam;   那个参数是给别的类用的
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
			bChoosed = true;		//有已经选择的项。
			nListIndex = i;		    //已经选择的项序号。
			break;                  //所以只能一次选中一个
		}
	}
	if(!bChoosed)		//用户还没有选择。
	{
		AfxMessageBox("你还没有选择发送的目的用户呢，请选择后再重新发送。");
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
	ON_MESSAGE(WM_SHOWTASK, OnShowTask)    //我自定义的消息。但是两个定义方法略有不同
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
/*----------------------Tab Control的实现-----------------------------------------*/
	m_ImageList.Create(16, 16, ILC_COLOR | ILC_MASK, 5, 0);
	m_ImageList.Add(AfxGetApp() -> LoadIcon(IDI_ICON1));                   //从外部加入时记得保存reload
    m_ImageList.Add(AfxGetApp() -> LoadIcon(IDI_ICON2));
	m_ImageList.Add(AfxGetApp() -> LoadIcon(IDI_ICON3));
	m_ImageList.Add(AfxGetApp() -> LoadIcon(IDI_ICON4));
	m_ImageList.Add(AfxGetApp() -> LoadIcon(IDI_ICON5));
	
	m_Tab.SetImageList(&m_ImageList);
	m_Tab.InsertItem(0, "快速传输", 0);                 //如何变大些？
	m_Tab.InsertItem(1, "加密传输", 1);
	m_Tab.InsertItem(2, "关联杀毒", 2);
	m_Tab.InsertItem(3, "在线聊天", 3);
	m_Tab.InsertItem(4, "语音聊天", 4);
	
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
/*----------------------Tab Control的实现-----------------------------------------*/

/*----------------------List Control的实现----------------------------------------*/
	m_ClientInfoList.SetExtendedStyle(
		m_ClientInfoList.GetExtendedStyle()    //有用吗？
		|LVS_EX_FLATSB            //扁平风格滚动条
		|LVS_EX_FULLROWSELECT    //允许整行选中
		|LVS_EX_HEADERDRAGDROP   //允许标题拖拽
		|LVS_EX_ONECLICKACTIVATE //高亮显示
		|LVS_EX_GRIDLINES        //画出网格线
		);
	
	m_ClientInfoList.InsertColumn(0, "用户名",   LVCFMT_LEFT, 120, 0);
	m_ClientInfoList.InsertColumn(1, "计算机名", LVCFMT_LEFT, 130, 1);
	m_ClientInfoList.InsertColumn(2, "IP地址",   LVCFMT_LEFT, 150, 2);
	
/*----------------------List Control的实现----------------------------------------*/

	StartLanShow();
	AddAllClientInfo();			// add the current Client Info in g_AcceptClient

	isAnti = TRUE;  //默认关联
	
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
	if (nSel > 4)           //确定nSel值不能超过范围
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
	nid.uCallbackMessage = WM_SHOWTASK;                  //自定义的消息名称 
	nid.hIcon  = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
	strcpy(nid.szTip, "LanTran");         //信息提示条
	Shell_NotifyIcon(NIM_ADD, &nid);                     //在托盘区添加图标 
	m_nOpenState = 1;                                    //设置状态成到系统托盘
	ShowWindow(SW_HIDE);                                 //隐藏主窗口
}

LRESULT CLanTranDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	if(wParam != IDR_MAINFRAME)
	{
		return 1;
	}
	switch(lParam)
	{ 
	case WM_RBUTTONUP:                          //右键起来时弹出快捷菜单，这里只有一个“关闭”
		{ 
			LPPOINT lpoint = new tagPOINT;
			::GetCursorPos(lpoint);             //得到鼠标位置 
			CMenu menu;
			menu.CreatePopupMenu();             //声明一个弹出式菜单,增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口（已隐藏），将程序结束。
			menu.AppendMenu(MF_STRING,WM_DESTROY,"关闭"); //确定弹出式菜单的位置 
			menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this); //资源回收 
			HMENU hmenu=menu.Detach();
			menu.DestroyMenu(); 
			delete lpoint;
		}
		break; 
	case WM_LBUTTONDBLCLK://双击左键的处理
		{
			this->ShowWindow(SW_SHOW);//简单的显示主窗口完事儿 
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
	nid.uCallbackMessage = WM_SHOWTASK;                //自定义的消息名称 
	nid.hIcon  = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)); 
	strcpy(nid.szTip, "LanTran");                      //信息提示条为“计划任务提醒” 
	Shell_NotifyIcon(NIM_DELETE, &nid);                //在托盘区删除图标
}

void CLanTranDlg::OnDestroy() 
{
	CDialog::OnDestroy();	
	// TODO: Add your message handler code here
	SendLanShowData(EXIT);
//	delete this;一定要吗？

/*----------删除传文件的套接字【必须在m_pFileTranDlg被毁灭前】-------------*/
	m_pFileTranDlg->DeleteFileSock();
/*----------删除传文件的套接字-------------*/

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

	// 注释掉下面这段就不会Debug Assertion Failed了
	// 估计是因为m_pChatDlg中含的套接字没删除掉
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

// StartLanShow()本身不是线程函数，只是个过程调用
// 但是它里面有CreateThread()来开启一个线程
void CLanTranDlg::StartLanShow()
{
	BOOL bIni = InitLanShowSocket();		// init and bind a Listen Socket
	if ( bIni == FALSE )
		return;

	InitMyClientInfo();						// add own ip info to Global Variable: g_AcceptClient
	SendLanShowData(ENTRY);					// 广播发送刚上线的信息, 告诉别人我来了

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
	while( thisHost->h_addr_list[i] )                //是给结构体赋值吗？不是很懂
	{
		in.S_un.S_addr = *(unsigned long *) thisHost->h_addr_list[i];
		i++;
	}

	dwLocalIP = in.S_un.S_addr;                     //因为是引用，所以m_dwLocalIP也改变了
	sprintf(m_localIPAddr,"%d.%d.%d.%d"             //m_localIPAddr是"%d.%d.%d.%d"的这种形式
		                 ,dwLocalIP&0x000000ff
		                 ,dwLocalIP>>8&0x0000000ff
		                 ,dwLocalIP>>16&0x000000ff
		                 ,dwLocalIP>>24&0x000000ff
				          );
}

void CLanTranDlg::InitMyClientInfo()                // 把本机的IP等信息增加到g_AcceptClient中
{
	ClientInfo LocalInfo;						// 用于保存本机信息的数据结构
	strcpy(LocalInfo.m_ClientIPAddr, m_localIPAddr);
	strcpy(LocalInfo.m_ComputerName, m_hostName);

	DWORD dwNameLen = sizeof ( m_TLocalUserName );
	GetUserName(m_TLocalUserName, &dwNameLen);
	strcpy(LocalInfo.m_UserName, m_TLocalUserName);

	g_AcceptClient.push_back(LocalInfo);            // 把自己的信息放在g_AcceptClient中
}

// 把自己的ip info给别人, 并接收局域网内其他在线用户的信息
DWORD WINAPI CLanTranDlg::LanShowThread(LPVOID lParam)
{
	CLanTranDlg *pLanTranDlg = (CLanTranDlg *)lParam;		// 因为这是静态函数吗？
	FD_SET fdR;
	struct timeval timeouts;

	while( true )
	{
		timeouts.tv_sec  = 1;         //Time value, in seconds. 
		timeouts.tv_usec = 0;         //Time value, in microseconds.
		FD_ZERO(&fdR);
		FD_SET(pLanTranDlg -> m_LanShowSocket, &fdR);     // 这里pAcceptClient -> m_listenSocket相当于文件描述符的作用，为整型
		switch( select ( pLanTranDlg->m_LanShowSocket + 1, &fdR, NULL, NULL, &timeouts) )
		{
		case -1:
			break;
		case 0:
			{							// 加括号是为了iEr编译通过
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
	char BroadCastIP[20];                        //通过提取自己的IP，得出整个局域网应有的IP
	sprintf(BroadCastIP,"%d.%d.%d.255"           //Write formatted data to a string.
		               ,m_dwLocalIP & 0x000000ff
					   ,m_dwLocalIP >> 8 & 0x0000000ff
					   ,m_dwLocalIP >> 16& 0x000000ff
						);

	SOCKADDR_IN AddrSendToClient;
	AddrSendToClient.sin_addr.S_un.S_addr = inet_addr(BroadCastIP);
	AddrSendToClient.sin_family = AF_INET;
	AddrSendToClient.sin_port = htons(NET_LANSHOW_PORT);

	LanShowData *SendingInfo = new LanShowData;       //存储本机的所有信息
	memset(SendingInfo, 0, sizeof(LanShowData));     //把SendingInfo都赋值为0

	SendingInfo->nState = nState;

	strcpy(SendingInfo->m_name, m_TLocalUserName);       //用户名
	strcpy(SendingInfo->m_iPAddr, m_localIPAddr);
	struct hostent *thisHost = gethostbyname(m_hostName);   //电脑名
	if (thisHost != NULL)
	{
		strcpy(SendingInfo->m_computerName, thisHost->h_name);
	}

	int iSend = sendto(m_LanShowSocket,(char FAR *)SendingInfo,sizeof(LanShowData),0,(struct sockaddr FAR *)&AddrSendToClient,sizeof(SOCKADDR_IN));    //给局域网内的所有用户发送自己的SendingInfo
	int iError = WSAGetLastError();

	delete SendingInfo;       //不然会溢出的。
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

	if ( from.sin_addr.S_un.S_addr != m_dwLocalIP )						// 是群发，所以自己也会收到
	{
		LanShowData *OneClientInfo = ( LanShowData *)RecvDataInfo;		// 很关键; 发送的就是ClientInfo *类型的。

		vector<ClientInfo>::iterator ts;

		/*
		 * handle LanShowData: "EXIT"
		 */
		if (OneClientInfo->nState == EXIT)							// EXIT相对另两个的处理比较不同，这样可以让另两个公用一些
		{
			for( ts = g_AcceptClient.begin(); ts != g_AcceptClient.end(); ts ++ )
			{
				if ( (strcmp(ts->m_ClientIPAddr, OneClientInfo->m_iPAddr) == 0) )    //Compare strings.=0时相等
				{
					g_AcceptClient.erase(ts);
					break;
				}
			}
			delete OneClientInfo;     //是删哪个值？OneClientInfo还是RecvDataInfo
			return;    //这样后面的消息也就不用判断了
		}

		/*
		 * handle LanShowData: "ENTRY" and judge no other message
		 */
		switch (OneClientInfo->nState)       //用switch写更有效率些的
		{
		case ENTRY:
			SendLanShowData(ONLINE);
			break;

		case ONLINE:			// 仅仅只要把下面的信息添加进来就ok了
			break;
			
		default:
			AfxMessageBox("收到了其他种类的消息");
			return;				// 不再处理下面的了
		}

		/*
		 * so the LanShowData must be "ENTRY" or "ANENTRY"
		 *
		 * it's time to add info to g_AcceptClient
		 * 
		 * 先看看g_AcceptClient中是否已经有了, 如果有的话就直接丢弃了
		 */
		for( ts = g_AcceptClient.begin(); ts != g_AcceptClient.end(); ts ++ )
		{
			if ( (strcmp(ts->m_ClientIPAddr, OneClientInfo->m_iPAddr) == 0) )     //Compare strings.=0时相等
			{
				delete OneClientInfo;
				return;
			}
		}

		/*
		 * 说明g_AcceptClient中还没有, 需要拷贝进来
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
	vector<ClientInfo>::iterator ts;          //terator是在vector名字空间中定义的类似于指针的一种结构
	
	m_ClientInfoList.DeleteAllItems();           //刷新的时候先清空列表
	
	int i = 0;
	for(ts = g_AcceptClient.begin(); ts != g_AcceptClient.end(); ts ++)
	{
		m_ClientInfoList.InsertItem(i, ts->m_UserName);
		m_ClientInfoList.SetItemText(i, 1, ts->m_ComputerName);
		m_ClientInfoList.SetItemText(i, 2, ts->m_ClientIPAddr);
		
		i++;         //i控制列表中的顺序
	}
}

void CLanTranDlg::OnButtonNew() 
{
	// TODO: Add your control notification handler code here
	SendLanShowData(ONLINE);
	Sleep(200);        //为了防止狂点吗？
	AddAllClientInfo();
}

BOOL CLanTranDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message==WM_KEYDOWN && (int)pMsg->wParam==VK_ESCAPE)   //防止按Esc键关闭对话框
	{
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
