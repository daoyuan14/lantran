// FileTranDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LanTran.h"
#include "FileTranDlg.h"
#include "FileClientSocket.h"
#include "FileServerSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileTranDlg dialog
const int MAXSENDNUM = 8192; //缓冲区发送和接收的最大字节数
extern bool GetSelectedIpAddr(CString &s, LPVOID lparam);           //可以用::来调用全局函数吗？
extern BOOL isAnti;
/*-------为什么这些不能加在头文件里---------------------------*/
/*
DWORD		g_dwSendCount = 0;		//已发送的字节数。
DWORD		g_dwRecvCount = 0;		//已接收的字节数。
int			nSendTime = 0;
int			nRecvTime = 0;
*/
/*-------【干脆成为类的成员变量吧！】为什么这些不能加在头文件里---------------------------*/

/////////////////////////////////////////////////////////////////////////////


CFileTranDlg::CFileTranDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileTranDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileTranDlg)
	//}}AFX_DATA_INIT
}


void CFileTranDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileTranDlg)
	DDX_Control(pDX, IDC_LIST_FILE, m_FileInfoList);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileTranDlg, CDialog)
	//{{AFX_MSG_MAP(CFileTranDlg)
	ON_BN_CLICKED(IDC_BUTTON_SELECTFILE, OnButtonSelectfile)
	ON_BN_CLICKED(IDC_BUTTON_SENDFILE, OnButtonSendfile)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileTranDlg message handlers

BOOL CFileTranDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_FileImageList.Create(32, 32, ILC_COLOR24|ILC_MASK, 0, 0);    //为什么最后第二个为0？
	m_FileImageList.Add(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_FILE1)));      //第0项
	m_FileImageList.Add(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_FILE2)));     //第1项

	m_FileInfoList.SetImageList(&m_FileImageList, LVSIL_NORMAL);   //参数不懂？

	/*-------------设置进度条颜色--------------------*/
	COLORREF crf = RGB(0, 0, 0);   //黑色
	this->GetDlgItem(IDC_PROGRESS)->SendMessage(PBM_SETBARCOLOR, 0, crf);   //变量是内在的，因此这边不能用变量
	/*-------------设置进度条颜色--------------------*/

	m_pServerSock = new CFileServerSocket(this);
	m_ServerSock  = new CFileServerSocket(this);
	m_pSendSock   = new CFileClientSocket(this);

	m_IsReveived = TRUE;
/*
	g_dwSendCount = 0;		//已发送的字节数。
	g_dwRecvCount = 0;		//已接收的字节数。
	nSendTime = 0;
    nRecvTime= 0;
*/
	m_pServerSock->Create(FILETRAN_SERVER_PORT);      //默认采取TCP的方式
	m_pSendSock->Create(FILETRAN_CLIENT_PORT);

	if (!m_pServerSock->Listen()) //开始监听   Listen: Nonzero if the function is successful
	{
		MessageBox("监听失败");
	}

	CString str ;
	str.Format("%i",999999);

	m_pServerSock->SetSockOpt(SO_RCVBUF, str.GetBuffer(0), str.GetLength());     //设置套接字属性
	m_pSendSock->SetSockOpt(SO_SNDBUF, str.GetBuffer(0), str.GetLength());	
	m_ServerSock->SetSockOpt(SO_RCVBUF, str.GetBuffer(0), str.GetLength());

	m_Send            = FALSE;
	m_SendNext        = FALSE;
	m_Received        = FALSE;
	m_SendFinished    = FALSE;

	m_Count = 0;
	m_FileSize = 0;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFileTranDlg::SendFileData()
{
	m_Send = TRUE;
	m_SendNext = TRUE;
	m_Received = TRUE;      //要对方同意发送了

	char data[100];
	memset(data, 0, 100);
	m_SendNum += 1;
	m_pSendSock->Receive(data,100);  //获得对方的应答信息  到底怎么应答的比较模糊

	CString rdata = data;

	if (m_SendLen>=m_SendFileLen)    //发送文件完成
	{
		m_Send = FALSE;
		m_SendNext = FALSE;
/*-----------------------------------------------------
		KillTimer(1);        //这句还要写不？
		KillTimer(WM_TIMER_SENDCOUNT);
		m_Speed.ShowWindow(SW_HIDE);
/*-----------------------------------------------------*/
		m_SendFile.Close();
		m_pSendSock->Close();
		m_pSendSock->Create(FILETRAN_CLIENT_PORT);    //为什么每次都要重新创建
		MessageBox("发送完成");
		m_Progress.SetPos(0);
		return;
	}

	if ((m_SendLen==0)&&(rdata=="取消"))
	{
		m_Send = FALSE;
		m_SendNext = FALSE;
		m_SendFile.Close();
		m_pSendSock->Close();
		m_pSendSock->Create(FILETRAN_CLIENT_PORT);
		
		MessageBox("对方取消了文件接收");
		return;
	}

	char* membuf= (char*)LocalAlloc(LMEM_FIXED, MAXSENDNUM);    //LocalAlloc函数用来分配局部内存对象
	memset(membuf, 0, MAXSENDNUM);
	int result;
/*----------------------------------------------------------
	nSendTime = 1;
	m_Speed.ShowWindow(SW_SHOW);
	SetTimer(WM_TIMER_SENDCOUNT, 1000, NULL);
/*----------------------------------------------------------*/

	if ((m_SendLen <m_SendFileLen)&&((m_SendLen+MAXSENDNUM)>m_SendFileLen))    //最后一次发送了
	{
		m_SendFile.ReadHuge(membuf, m_SendFileLen-m_SendLen);    //将剩下的读取到缓冲区membuf
		result = m_pSendSock->Send(membuf, m_SendFileLen-m_SendLen);
		if (result ==SOCKET_ERROR)
		{
			m_Send = FALSE;
			m_SendNext = FALSE;
			m_SendFile.Close();
			m_pSendSock->Close();
			m_pSendSock->Create(FILETRAN_CLIENT_PORT);
			LocalFree(membuf);   //LocalFree函数用来释放局部内存对象，当一个局部内存对象被释放时，其内容从局部堆移走，并且其句柄也从有效的局部内存表中移走，原来的内存句柄变为不可用。
			KillTimer(1);        //取消定时器
//			KillTimer(WM_TIMER_SENDCOUNT);
//			m_Speed.ShowWindow(SW_HIDE);
			MessageBox("发送失败");	
			return ;
		}
		LocalFree(membuf);    //这是我加的
		m_SendLen =m_SendFileLen;
	}
	else    //前面的n多次发送
	{
		m_SendFile.ReadHuge(membuf, MAXSENDNUM);
		result = m_pSendSock->Send(membuf, MAXSENDNUM);
		if (result ==SOCKET_ERROR)
		{
			m_Send = FALSE;
			m_SendNext = FALSE;
			m_SendFile.Close();
			m_pSendSock->Close();
			m_pSendSock->Create(FILETRAN_CLIENT_PORT);
			
			LocalFree(membuf);
			KillTimer(1);
//			KillTimer(WM_TIMER_SENDCOUNT);
//			m_Speed.ShowWindow(SW_HIDE);
			MessageBox("发送失败");	
			return;
		}
		LocalFree(membuf);
		m_SendLen += MAXSENDNUM;
	}
	m_Progress.SetPos(m_SendLen);
//	g_dwSendCount = m_SendLen;
}

void CFileTranDlg::OnDropFiles(HDROP hDropInfo) 
{
	char filename[MAX_PATH];
	DragQueryFile(hDropInfo, 0, filename, MAX_PATH);   //把文件名写入数组
	m_FileInfoList.DeleteAllItems();
	m_FileName = filename;
	m_FileInfoList.InsertItem(0, m_FileName, 1);
}

void CFileTranDlg::AcceptConnection()
{
	m_ServerSock->Close();   //为了保险先关闭再说
	m_pServerSock->Accept(*m_ServerSock);    //又是把m_ServerSock在打开
}

void CFileTranDlg::ReceiveData()
{
	void* buffer = (void*)LocalAlloc(LMEM_FIXED, MAXSENDNUM+1);   //为什么要比MAXSENDNUM多1？
	if (m_IsReveived) //第一次接收数据,获取接收的文件信息
	{
		char* fileinfo = (char*)LocalAlloc(LMEM_FIXED,MAX_PATH+4);
		memset(fileinfo, 0, MAX_PATH+4);

		int len = m_ServerSock->Receive(fileinfo, MAX_PATH+4);   //接受fileinfo
		m_ReceiveFile.Format("%s",fileinfo); //记录接收的文件名

		char* temp = fileinfo;
		while (temp[0] !=0)   //统计大小
		{
			temp +=1;
		}
		temp+=1;   //最后一个以0结束
		m_FileSize= atoi(temp); //记录接收的文件大小
		m_Progress.SetRange32(0, m_FileSize);
		m_Progress.SetPos(0);
		LocalFree(fileinfo);

		m_FileLen = 0;
		CString name = GetFileName(m_ReceiveFile);
		if (MessageBox("是否接收数据?","提示",MB_YESNO)==IDYES)
		{
			BROWSEINFO bInfo;
			bInfo.hwndOwner = m_hWnd;
			bInfo.pidlRoot  = NULL;
			char filename[MAX_PATH];  //接受方保存的文件名
			char dir[MAX_PATH];    //接收方保存的路径
			memset(filename, 0, MAX_PATH);
			memset(dir, 0, MAX_PATH);

			bInfo.pszDisplayName = filename;
			bInfo.lpszTitle ="请选择目的路径";
			bInfo.ulFlags = BIF_BROWSEINCLUDEFILES|BIF_RETURNFSANCESTORS|BIF_RETURNONLYFSDIRS|BIF_BROWSEFORCOMPUTER;
			bInfo.lParam  =NULL;
			bInfo.lpfn  =NULL;
			bInfo.iImage = 0;

			LPITEMIDLIST lpList = SHBrowseForFolder(&bInfo);

			CString filedir;
			if (SHGetPathFromIDList(lpList,dir))
			{
				m_File.Abort();
				filedir = dir;
				filedir += "\\";     //必须要两个斜杠
				m_File.Open(filedir+name, CFile::modeCreate|CFile::modeWrite);

				strAntiFile = filedir+name;

				m_IsReveived = FALSE;  //文件信息接收结束
				m_ServerSock->Send("1111",4);  //返回一个文件信息接受成功的确认信息
				
				LocalFree(buffer);
				return;
			}
			else  //拒绝接收文件
			{
				m_File.Close();
				LocalFree(buffer);
				m_ServerSock->Send("取消",4);
				m_ServerSock->Close();
				m_IsReveived = TRUE; 
				return;					
			}
		}
		else
		{	
			LocalFree(buffer);
			m_ServerSock->Send("取消",4);
			m_ServerSock->Close();
			m_IsReveived = TRUE; 
			return;
		}
	}
	else //开始接收数据
	{
//		nRecvTime = 1;
//		m_Speed.ShowWindow(SW_SHOW);
//		SetTimer(WM_TIMER_RECVCOUNT, 1000, NULL);

		int filelen =  m_ServerSock->Receive(buffer, MAXSENDNUM+1);
		if (filelen==SOCKET_ERROR)
		{
			m_IsReveived = TRUE;
//			m_Speed.ShowWindow(SW_HIDE);
//			KillTimer(WM_TIMER_RECVCOUNT);
			m_File.Close();
			m_ServerSock->Close();
			LocalFree(buffer);
			MessageBox("数据接收错误");
			
			return;
		}
		m_FileLen += filelen;
		m_File.WriteHuge(buffer, filelen);
//		m_Progress.SetStep(m_FileLen);      //让先写入文件再变进度条
		LocalFree(buffer);
		if (m_FileLen>=m_FileSize)  //接收完成   说明m_FileSize就是文件长度
		{
			m_Progress.SetPos(m_FileSize);
//			g_dwRecvCount = m_FileSize;
//			m_Speed.ShowWindow(SW_HIDE);
//			KillTimer(WM_TIMER_RECVCOUNT);
			m_File.Close();
			m_ServerSock->Send("结束",4);
			m_IsReveived = TRUE;
			m_ServerSock->Close();
			MessageBox("接收完成");
			m_Progress.SetPos(0);
			if (isAnti)
			{
				::ShellExecute(this->GetSafeHwnd(), "open",
					"C:\\Program Files\\Rising\\Rav\\Rav.exe", strAntiFile, NULL, SW_SHOW);
			}
		}
		else
		{
			m_ServerSock->Send("1111",4);
			m_Progress.SetPos(m_FileLen);
//			g_dwRecvCount = m_FileLen;
		}
	}

}

void CFileTranDlg::OnButtonSelectfile() 
{
	// TODO: Add your control notification handler code here
//	CFileDialog fDlg (TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,NULL,this);没太大差别吧？
	CFileDialog fDlg(TRUE, NULL, "", OFN_HIDEREADONLY, "All Files (*.*)|*.*||", this);   //打开对话框
	if (fDlg.DoModal()==IDOK)
	{
		m_FileInfoList.DeleteAllItems();      //可以不删吗？保留以前的
		m_FileName = fDlg.GetPathName();
		m_FileInfoList.InsertItem(0, m_FileName, 0);    //添加行   int InsertItem( int nItem, LPCTSTR lpszItem, int nImage );
	}
}

void CFileTranDlg::OnButtonSendfile() 
{
	// TODO: Add your control notification handler code here
	CString	toAddr;   //选择的IP
	if( !(GetSelectedIpAddr(toAddr, this)) )   //没有选择的话直接返回
	{
		return;
	}

	if (m_Send == FALSE)       //没有文件正在发送
	{
		if (m_FileInfoList.GetItemCount()<1)     //即没有选择文件
		{
			MessageBox("请选择发送的文件");
			return;
		}
/*-------------------先暂时这么做，用来测验-----------------------------*/
		//CString toAddr;   //先定姚焕章的IP
		//toAddr = "192.168.18.2";
/*-------------------先暂时这么做，用来测验-----------------------------*/

		m_SendFile.Abort();     //Closes the file associated with this object
		m_SendFile.Open(m_FileName, CFile::modeRead);
		m_SendFileLen = m_SendFile.GetLength();

		m_Progress.SetRange32(0, m_SendFileLen);    //整个坐标为m_SendFileLen
		m_Progress.SetPos(0);

		char *fileinfo = (char*)LocalAlloc(LMEM_FIXED, MAX_PATH+4);     //保存文件名信息；MAX_PATH默认的260？
		memset(fileinfo, 0, MAX_PATH+4);
		memcpy(fileinfo, m_FileName.GetBuffer(0), m_FileName.GetLength());

		CString filesize;
		filesize.Format("%i", m_SendFileLen);
		memcpy(fileinfo+m_FileName.GetLength()+1, filesize.GetBuffer(0), filesize.GetLength());   //？？？

		m_pSendSock->Close();     //为什么一定要先关闭？
		m_pSendSock->Create(FILETRAN_CLIENT_PORT);
		if (!m_pSendSock->Connect(toAddr, FILETRAN_SERVER_PORT))
		{
			int nError = m_pSendSock->GetLastError();
			m_SendFile.Close();    //把打开的文件关闭

			m_pSendSock->Close();
			m_pSendSock->Create(FILETRAN_CLIENT_PORT);   //干吗又来？

			MessageBox("连接失败");
			m_Send = FALSE;      //没有文件在发送了
			return;
		}

		SetTimer(1, 2000, NULL);     //每隔2000毫秒系统调用一次Ontimer（），判断网络连接情况

		m_SendNum = 0;
		m_pSendSock->Send(fileinfo, MAX_PATH+4);   //发送文件信息

		m_SendLen = 0;
		LocalFree(fileinfo);      //fileinfo里保存了什么？
	}
	else
	{
		MessageBox("当前发送任务没有完成");
	}
}

CString CFileTranDlg::GetFileName(CString FileName)
{
	int pos = FileName.ReverseFind('\\');
	CString name = FileName.Mid(pos+1,FileName.GetLength());
	return name;
}

void CFileTranDlg::DeleteFileSock()
{
	m_pServerSock->Close();
	delete m_pServerSock;

	if (m_pSendSock != NULL)
	{
		m_pSendSock->Close();
		delete m_pSendSock;
		m_pSendSock = NULL;
	}

	m_ServerSock->Close();
	delete m_ServerSock;
}

BOOL CFileTranDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ((pMsg->hwnd == m_FileInfoList.m_hWnd) && (pMsg->message==WM_DROPFILES))
	{
		OnDropFiles((HDROP)pMsg->wParam);
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CFileTranDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_Send==TRUE)  //文件传输中
	{
		if (m_Received == TRUE)   //正常的网络连接情况
		{
			m_Count = 0;
			m_Received =FALSE;
		}
		else
		{
			m_Count += 1;

			if (m_Count >5)
			{
				m_Count  = 0;
				KillTimer(1);   //与后面的1是对应好的
				if (MessageBox("对方长时间没有回应信息,是否断开连接?","提示",MB_YESNO)==IDYES)
				{
					m_Send = FALSE;
					m_SendNext = FALSE;
					m_SendFile.Abort();
					m_ServerSock->Close();
					m_pSendSock->Close();
					m_pSendSock->Create(FILETRAN_CLIENT_PORT);
					BOOL value = TRUE;
					m_pSendSock->SetSockOpt(SO_LINGER,(void*)&value,1);
					m_IsReveived = TRUE;
				}
				else
				{
					SetTimer(1,2000,NULL);   //1是对应好的。
					m_Received = FALSE;
				}
			}
		}
	}
/*
	switch(nIDEvent)
	{
	case	WM_TIMER_SENDCOUNT:
		SetSendSpeed();
		break;
	case	WM_TIMER_RECVCOUNT:
		SetRecvSpeed();
		break;
	default:
		break;
	}
*/	
	CDialog::OnTimer(nIDEvent);
}
/*
void CFileTranDlg::SetSendSpeed()
{
	CString		s;
	DWORD		speed;

	speed = (g_dwSendCount/nSendTime++)>>10;

	if(1024>speed)	//Speed  is    k/BPS
	{
		s.Format("%dK/BPS",speed);
		m_Speed.SetWindowText(s);
	}
	else			//Speed	 is		M/BPS
	{
		s.Format("%dM/BPS",speed>>10);
		m_Speed.SetWindowText(s);
	}
}

void CFileTranDlg::SetRecvSpeed()
{
	CString		s;
	DWORD		speed;

    speed=(g_dwRecvCount/nRecvTime++)>>10;   //以后每次时间加加

	if(1024>speed)	//Speed  is    k/BPS
	{
		s.Format("%dK/BPS",speed);
		m_Speed.SetWindowText(s);
	}
	else			//Speed	 is		M/BPS
	{
		s.Format("%dM/BPS",speed>>10);
		m_Speed.SetWindowText(s);
	}

}*/
