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
const int MAXSENDNUM = 8192; //���������ͺͽ��յ�����ֽ���
extern bool GetSelectedIpAddr(CString &s, LPVOID lparam);           //������::������ȫ�ֺ�����
extern BOOL isAnti;
/*-------Ϊʲô��Щ���ܼ���ͷ�ļ���---------------------------*/
/*
DWORD		g_dwSendCount = 0;		//�ѷ��͵��ֽ�����
DWORD		g_dwRecvCount = 0;		//�ѽ��յ��ֽ�����
int			nSendTime = 0;
int			nRecvTime = 0;
*/
/*-------���ɴ��Ϊ��ĳ�Ա�����ɣ���Ϊʲô��Щ���ܼ���ͷ�ļ���---------------------------*/

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
	m_FileImageList.Create(32, 32, ILC_COLOR24|ILC_MASK, 0, 0);    //Ϊʲô���ڶ���Ϊ0��
	m_FileImageList.Add(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_FILE1)));      //��0��
	m_FileImageList.Add(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_FILE2)));     //��1��

	m_FileInfoList.SetImageList(&m_FileImageList, LVSIL_NORMAL);   //����������

	/*-------------���ý�������ɫ--------------------*/
	COLORREF crf = RGB(0, 0, 0);   //��ɫ
	this->GetDlgItem(IDC_PROGRESS)->SendMessage(PBM_SETBARCOLOR, 0, crf);   //���������ڵģ������߲����ñ���
	/*-------------���ý�������ɫ--------------------*/

	m_pServerSock = new CFileServerSocket(this);
	m_ServerSock  = new CFileServerSocket(this);
	m_pSendSock   = new CFileClientSocket(this);

	m_IsReveived = TRUE;
/*
	g_dwSendCount = 0;		//�ѷ��͵��ֽ�����
	g_dwRecvCount = 0;		//�ѽ��յ��ֽ�����
	nSendTime = 0;
    nRecvTime= 0;
*/
	m_pServerSock->Create(FILETRAN_SERVER_PORT);      //Ĭ�ϲ�ȡTCP�ķ�ʽ
	m_pSendSock->Create(FILETRAN_CLIENT_PORT);

	if (!m_pServerSock->Listen()) //��ʼ����   Listen: Nonzero if the function is successful
	{
		MessageBox("����ʧ��");
	}

	CString str ;
	str.Format("%i",999999);

	m_pServerSock->SetSockOpt(SO_RCVBUF, str.GetBuffer(0), str.GetLength());     //�����׽�������
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
	m_Received = TRUE;      //Ҫ�Է�ͬ�ⷢ����

	char data[100];
	memset(data, 0, 100);
	m_SendNum += 1;
	m_pSendSock->Receive(data,100);  //��öԷ���Ӧ����Ϣ  ������ôӦ��ıȽ�ģ��

	CString rdata = data;

	if (m_SendLen>=m_SendFileLen)    //�����ļ����
	{
		m_Send = FALSE;
		m_SendNext = FALSE;
/*-----------------------------------------------------
		KillTimer(1);        //��仹Ҫд����
		KillTimer(WM_TIMER_SENDCOUNT);
		m_Speed.ShowWindow(SW_HIDE);
/*-----------------------------------------------------*/
		m_SendFile.Close();
		m_pSendSock->Close();
		m_pSendSock->Create(FILETRAN_CLIENT_PORT);    //Ϊʲôÿ�ζ�Ҫ���´���
		MessageBox("�������");
		m_Progress.SetPos(0);
		return;
	}

	if ((m_SendLen==0)&&(rdata=="ȡ��"))
	{
		m_Send = FALSE;
		m_SendNext = FALSE;
		m_SendFile.Close();
		m_pSendSock->Close();
		m_pSendSock->Create(FILETRAN_CLIENT_PORT);
		
		MessageBox("�Է�ȡ�����ļ�����");
		return;
	}

	char* membuf= (char*)LocalAlloc(LMEM_FIXED, MAXSENDNUM);    //LocalAlloc������������ֲ��ڴ����
	memset(membuf, 0, MAXSENDNUM);
	int result;
/*----------------------------------------------------------
	nSendTime = 1;
	m_Speed.ShowWindow(SW_SHOW);
	SetTimer(WM_TIMER_SENDCOUNT, 1000, NULL);
/*----------------------------------------------------------*/

	if ((m_SendLen <m_SendFileLen)&&((m_SendLen+MAXSENDNUM)>m_SendFileLen))    //���һ�η�����
	{
		m_SendFile.ReadHuge(membuf, m_SendFileLen-m_SendLen);    //��ʣ�µĶ�ȡ��������membuf
		result = m_pSendSock->Send(membuf, m_SendFileLen-m_SendLen);
		if (result ==SOCKET_ERROR)
		{
			m_Send = FALSE;
			m_SendNext = FALSE;
			m_SendFile.Close();
			m_pSendSock->Close();
			m_pSendSock->Create(FILETRAN_CLIENT_PORT);
			LocalFree(membuf);   //LocalFree���������ͷžֲ��ڴ���󣬵�һ���ֲ��ڴ�����ͷ�ʱ�������ݴӾֲ������ߣ���������Ҳ����Ч�ľֲ��ڴ�������ߣ�ԭ�����ڴ�����Ϊ�����á�
			KillTimer(1);        //ȡ����ʱ��
//			KillTimer(WM_TIMER_SENDCOUNT);
//			m_Speed.ShowWindow(SW_HIDE);
			MessageBox("����ʧ��");	
			return ;
		}
		LocalFree(membuf);    //�����Ҽӵ�
		m_SendLen =m_SendFileLen;
	}
	else    //ǰ���n��η���
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
			MessageBox("����ʧ��");	
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
	DragQueryFile(hDropInfo, 0, filename, MAX_PATH);   //���ļ���д������
	m_FileInfoList.DeleteAllItems();
	m_FileName = filename;
	m_FileInfoList.InsertItem(0, m_FileName, 1);
}

void CFileTranDlg::AcceptConnection()
{
	m_ServerSock->Close();   //Ϊ�˱����ȹر���˵
	m_pServerSock->Accept(*m_ServerSock);    //���ǰ�m_ServerSock�ڴ�
}

void CFileTranDlg::ReceiveData()
{
	void* buffer = (void*)LocalAlloc(LMEM_FIXED, MAXSENDNUM+1);   //ΪʲôҪ��MAXSENDNUM��1��
	if (m_IsReveived) //��һ�ν�������,��ȡ���յ��ļ���Ϣ
	{
		char* fileinfo = (char*)LocalAlloc(LMEM_FIXED,MAX_PATH+4);
		memset(fileinfo, 0, MAX_PATH+4);

		int len = m_ServerSock->Receive(fileinfo, MAX_PATH+4);   //����fileinfo
		m_ReceiveFile.Format("%s",fileinfo); //��¼���յ��ļ���

		char* temp = fileinfo;
		while (temp[0] !=0)   //ͳ�ƴ�С
		{
			temp +=1;
		}
		temp+=1;   //���һ����0����
		m_FileSize= atoi(temp); //��¼���յ��ļ���С
		m_Progress.SetRange32(0, m_FileSize);
		m_Progress.SetPos(0);
		LocalFree(fileinfo);

		m_FileLen = 0;
		CString name = GetFileName(m_ReceiveFile);
		if (MessageBox("�Ƿ��������?","��ʾ",MB_YESNO)==IDYES)
		{
			BROWSEINFO bInfo;
			bInfo.hwndOwner = m_hWnd;
			bInfo.pidlRoot  = NULL;
			char filename[MAX_PATH];  //���ܷ�������ļ���
			char dir[MAX_PATH];    //���շ������·��
			memset(filename, 0, MAX_PATH);
			memset(dir, 0, MAX_PATH);

			bInfo.pszDisplayName = filename;
			bInfo.lpszTitle ="��ѡ��Ŀ��·��";
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
				filedir += "\\";     //����Ҫ����б��
				m_File.Open(filedir+name, CFile::modeCreate|CFile::modeWrite);

				strAntiFile = filedir+name;

				m_IsReveived = FALSE;  //�ļ���Ϣ���ս���
				m_ServerSock->Send("1111",4);  //����һ���ļ���Ϣ���ܳɹ���ȷ����Ϣ
				
				LocalFree(buffer);
				return;
			}
			else  //�ܾ������ļ�
			{
				m_File.Close();
				LocalFree(buffer);
				m_ServerSock->Send("ȡ��",4);
				m_ServerSock->Close();
				m_IsReveived = TRUE; 
				return;					
			}
		}
		else
		{	
			LocalFree(buffer);
			m_ServerSock->Send("ȡ��",4);
			m_ServerSock->Close();
			m_IsReveived = TRUE; 
			return;
		}
	}
	else //��ʼ��������
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
			MessageBox("���ݽ��մ���");
			
			return;
		}
		m_FileLen += filelen;
		m_File.WriteHuge(buffer, filelen);
//		m_Progress.SetStep(m_FileLen);      //����д���ļ��ٱ������
		LocalFree(buffer);
		if (m_FileLen>=m_FileSize)  //�������   ˵��m_FileSize�����ļ�����
		{
			m_Progress.SetPos(m_FileSize);
//			g_dwRecvCount = m_FileSize;
//			m_Speed.ShowWindow(SW_HIDE);
//			KillTimer(WM_TIMER_RECVCOUNT);
			m_File.Close();
			m_ServerSock->Send("����",4);
			m_IsReveived = TRUE;
			m_ServerSock->Close();
			MessageBox("�������");
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
//	CFileDialog fDlg (TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,NULL,this);û̫����ɣ�
	CFileDialog fDlg(TRUE, NULL, "", OFN_HIDEREADONLY, "All Files (*.*)|*.*||", this);   //�򿪶Ի���
	if (fDlg.DoModal()==IDOK)
	{
		m_FileInfoList.DeleteAllItems();      //���Բ�ɾ�𣿱�����ǰ��
		m_FileName = fDlg.GetPathName();
		m_FileInfoList.InsertItem(0, m_FileName, 0);    //�����   int InsertItem( int nItem, LPCTSTR lpszItem, int nImage );
	}
}

void CFileTranDlg::OnButtonSendfile() 
{
	// TODO: Add your control notification handler code here
	CString	toAddr;   //ѡ���IP
	if( !(GetSelectedIpAddr(toAddr, this)) )   //û��ѡ��Ļ�ֱ�ӷ���
	{
		return;
	}

	if (m_Send == FALSE)       //û���ļ����ڷ���
	{
		if (m_FileInfoList.GetItemCount()<1)     //��û��ѡ���ļ�
		{
			MessageBox("��ѡ���͵��ļ�");
			return;
		}
/*-------------------����ʱ��ô������������-----------------------------*/
		//CString toAddr;   //�ȶ�Ҧ���µ�IP
		//toAddr = "192.168.18.2";
/*-------------------����ʱ��ô������������-----------------------------*/

		m_SendFile.Abort();     //Closes the file associated with this object
		m_SendFile.Open(m_FileName, CFile::modeRead);
		m_SendFileLen = m_SendFile.GetLength();

		m_Progress.SetRange32(0, m_SendFileLen);    //��������Ϊm_SendFileLen
		m_Progress.SetPos(0);

		char *fileinfo = (char*)LocalAlloc(LMEM_FIXED, MAX_PATH+4);     //�����ļ�����Ϣ��MAX_PATHĬ�ϵ�260��
		memset(fileinfo, 0, MAX_PATH+4);
		memcpy(fileinfo, m_FileName.GetBuffer(0), m_FileName.GetLength());

		CString filesize;
		filesize.Format("%i", m_SendFileLen);
		memcpy(fileinfo+m_FileName.GetLength()+1, filesize.GetBuffer(0), filesize.GetLength());   //������

		m_pSendSock->Close();     //Ϊʲôһ��Ҫ�ȹرգ�
		m_pSendSock->Create(FILETRAN_CLIENT_PORT);
		if (!m_pSendSock->Connect(toAddr, FILETRAN_SERVER_PORT))
		{
			int nError = m_pSendSock->GetLastError();
			m_SendFile.Close();    //�Ѵ򿪵��ļ��ر�

			m_pSendSock->Close();
			m_pSendSock->Create(FILETRAN_CLIENT_PORT);   //����������

			MessageBox("����ʧ��");
			m_Send = FALSE;      //û���ļ��ڷ�����
			return;
		}

		SetTimer(1, 2000, NULL);     //ÿ��2000����ϵͳ����һ��Ontimer�������ж������������

		m_SendNum = 0;
		m_pSendSock->Send(fileinfo, MAX_PATH+4);   //�����ļ���Ϣ

		m_SendLen = 0;
		LocalFree(fileinfo);      //fileinfo�ﱣ����ʲô��
	}
	else
	{
		MessageBox("��ǰ��������û�����");
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
	if (m_Send==TRUE)  //�ļ�������
	{
		if (m_Received == TRUE)   //�����������������
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
				KillTimer(1);   //������1�Ƕ�Ӧ�õ�
				if (MessageBox("�Է���ʱ��û�л�Ӧ��Ϣ,�Ƿ�Ͽ�����?","��ʾ",MB_YESNO)==IDYES)
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
					SetTimer(1,2000,NULL);   //1�Ƕ�Ӧ�õġ�
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

    speed=(g_dwRecvCount/nRecvTime++)>>10;   //�Ժ�ÿ��ʱ��Ӽ�

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
