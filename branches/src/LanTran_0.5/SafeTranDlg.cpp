// SafeTranDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LanTran.h"
#include "SafeTranDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSafeTranDlg dialog


CSafeTranDlg::CSafeTranDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSafeTranDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSafeTranDlg)
	m_EnMiyao = _T("");
	m_DeMiyao = _T("");
	//}}AFX_DATA_INIT
}


void CSafeTranDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSafeTranDlg)
	DDX_Control(pDX, IDC_STATIC_DECRYPT, m_dePath);
	DDX_Control(pDX, IDC_STATIC_ENCRYPT, m_enPath);
	DDX_Text(pDX, IDC_EDIT_KEYEN, m_EnMiyao);
	DDV_MaxChars(pDX, m_EnMiyao, 20);
	DDX_Text(pDX, IDC_EDIT_KEYDE, m_DeMiyao);
	DDV_MaxChars(pDX, m_DeMiyao, 20);
//============================================
	DDX_Control(pDX, IDC_PICTURE, m_picture);
//=============================================
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSafeTranDlg, CDialog)
	//{{AFX_MSG_MAP(CSafeTranDlg)
	ON_BN_CLICKED(IDC_BUTTON_ENSELECT, OnButtonEnSelect)
	ON_BN_CLICKED(IDC_BUTTON_NOKEYSEND, OnButtonNokeySend)
	ON_MESSAGE(WM_SOCKETMSG, OnSocket)     //���Զ������Ϣ
	ON_BN_CLICKED(IDC_BUTTON_NOKEYDECRYPT, OnButtonNokeyDecrypt)
	ON_BN_CLICKED(IDC_BUTTON_KEYSEND, OnButtonKeySend)
	ON_BN_CLICKED(IDC_BUTTON_KEYDECRYPT, OnButtonKeyDecrypt)
	ON_EN_CHANGE(IDC_EDIT_KEYEN, OnChangeEditKeyEn)
	ON_EN_CHANGE(IDC_EDIT_KEYDE, OnChangeEditKeyDe)
	ON_BN_CLICKED(IDC_BUTTON_DESELECT, OnButtonDeSelect)
//======================================================================
	ON_BN_CLICKED(IDC_BTSELECTPIC, OnSelectPic)
	ON_BN_CLICKED(IDC_BTENCRY, OnEncry)
	ON_BN_CLICKED(IDC_BTADDFILE, OnAddFile)
//======================================================================
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSafeTranDlg message handlers
UINT ListenSafeTranThread(LPVOID lparam)        // �൱�ڿͻ���,���ռ����ļ�
{
	CSafeTranDlg *pDlg = (CSafeTranDlg *)lparam;

	pDlg->num = 0;
	pDlg->m_isStop = TRUE;   //���Ǽ��ں��棿
	
	WSAAsyncSelect(pDlg->m_listenSocket, pDlg->GetSafeHwnd(), WM_SOCKETMSG, FD_READ);      //�����첽ѡ��
	
	return 0;
	
}
//�ѵ�ȫ�ֺ�������д�����棿
//////////////////////////////////////////////////////////////////////////////

BOOL CSafeTranDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	StartServerThread();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSafeTranDlg::OnButtonEnSelect() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,NULL,"",OFN_HIDEREADONLY,"All Files (*.*)|*.*||",this);   //�򿪶Ի���
	if(IDOK == dlg.DoModal())
	{
		m_pathName = dlg.GetPathName();
		m_strName = dlg.GetFileName();
		m_enPath.SetWindowText(m_pathName);
	}
	
}

void CSafeTranDlg::OnButtonNokeySend() 
{
	// TODO: Add your control notification handler code here
	CString	selectedIP;
	if( !GetSelectedIpAddr(selectedIP, this) )
	{
		return;
	}

	SOCKADDR_IN AddrSendToClient;
	AddrSendToClient.sin_addr.S_un.S_addr = inet_addr(selectedIP);
	AddrSendToClient.sin_port = htons(SAFETRAN_PORT);
	AddrSendToClient.sin_family = AF_INET;

	CFile file(m_pathName, CFile::modeRead);
	DWORD size = file.GetLength();

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, size);
	void* lpBuf = GlobalLock(hGlobal);

	file.ReadHuge(lpBuf, size);

	int ret = sendto(m_listenSocket, m_strName, m_strName.GetLength(), 0, (struct sockaddr FAR *) &AddrSendToClient, sizeof(SOCKADDR_IN));   //�Ȱ��ļ������͹�ȥ
	if(ret == SOCKET_ERROR)
	{
		CString str;
		str.Format("��������ʧ�ܣ�������:%d",WSAGetLastError());
		AfxMessageBox(str);
	}

	Sleep(100);   //Ҫ�������
	char *buf;
	int mod = size % 1024;
	for(int i=0; i<INT(size/1024); i++)     //i��Ϊ���ĸ�����ÿ�ζ���1024�ֽڵİ������������mod����
	{
		CString encrypt;
		buf = (char*)lpBuf + 1024*i;
		
		if(i%2 == 1)
		{
			encrypt = EncryptOne(buf);     //�����İ��Ĵ���
		}
		else
		{
			encrypt = EncryptTwo(buf);     //ż�����Ĵ���
		}
		
		int ret = sendto(m_listenSocket, encrypt, 2048, 0, (SOCKADDR*) &AddrSendToClient, sizeof(AddrSendToClient));    //Ϊʲô���2048�ˣ�
		if(ret == SOCKET_ERROR)
		{
			CString str;
			str.Format("��������ʧ�ܣ�������:%d",WSAGetLastError());
			AfxMessageBox(str);
		}
		Sleep(100);   //Ҫ����ָ���
	}

	if(mod != 0)     //����0�����ò�Ҫ������
	{
		CString encrypt;
		buf = (char*)lpBuf + (size-mod);
		if(i%2 == 1)
		{
			encrypt = EncryptOne(buf);
		}
		else
		{
			encrypt = EncryptTwo(buf);
		}
		
		int ret = sendto(m_listenSocket, encrypt, encrypt.GetLength(), 0, (SOCKADDR*)&AddrSendToClient, sizeof(AddrSendToClient));	
		if(ret == SOCKET_ERROR)
		{
			CString str;
			str.Format("��������ʧ�ܣ�������:%d",WSAGetLastError());
			AfxMessageBox(str);
			return ;
		}
		Sleep(100);
	}

	char a[1] = {'^'}; //������
	sendto(m_listenSocket, (char*)a, 1, 0, (SOCKADDR*)&AddrSendToClient, sizeof(AddrSendToClient));
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);	
}

BOOL CSafeTranDlg::IniListenSocket()
{
	m_listenSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if ( m_listenSocket == INVALID_SOCKET )
	{
		CString str;
		str.Format("�����׽���ʧ�ܣ�������:%d", WSAGetLastError());
		AfxMessageBox(str);
		return FALSE;
	}

	GetLocalIP(m_dwLocalIP);

	SOCKADDR_IN m_clientAddr;
	m_clientAddr.sin_family = AF_INET;
	m_clientAddr.sin_port = htons(SAFETRAN_PORT);
	m_clientAddr.sin_addr.S_un.S_addr = m_dwLocalIP;

	int iBind = bind(m_listenSocket, (struct sockaddr FAR *) &m_clientAddr, sizeof(SOCKADDR_IN));
	if ( iBind == SOCKET_ERROR )
	{
		AfxMessageBox("binging is failed!");
		return FALSE;
	}

	return TRUE;
}

void CSafeTranDlg::GetLocalIP(DWORD &dwLocalIP)
{
	struct in_addr in;
	struct hostent *thisHost;
	
	char m_hostName[80];
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

}

void CSafeTranDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, 8192);
	void* lpBuf = GlobalLock(hGlobal);
	
	CString sMessage;    //Ҫ��ʾ����Ϣ
	SOCKADDR_IN	from;
	int addrLen = sizeof(SOCKADDR_IN);
	
	int message = lParam & 0x0000FFFF;  //��ⷢ���������¼�
	if(message == FD_READ) //���¼�
	{
		num++;   //����ģ�
		
		int ret = recvfrom(m_listenSocket, (char*)lpBuf, 8192, 0, (SOCKADDR*)&from, &addrLen);	//Ϊʲô��8192�أ�fromָ�����ݰ����Ե�Դ��ַ
		if(ret == SOCKET_ERROR)
		{
			CString str;
			str.Format("��������ʧ�ܣ�������:%d", WSAGetLastError());
			AfxMessageBox(str);
			return;
		}
		
		char* temp = (char*)lpBuf;
		
		if((ret ==1)&&(temp[0]=='^'))
		{
			m_isStop = TRUE;
			m_file.Close();
			AfxMessageBox("�������");
			return;
		}
		
		if(m_isStop)     //���������
		{
			char *path;
			path = (char*)lpBuf;
			path[ret] = '\0';
			m_pathName = "e:\\" + (CString)path;
			m_pathName = m_pathName.Left(ret + 4);
			m_file.Open(m_pathName, CFile::modeCreate|CFile::modeWrite);
			m_isStop = FALSE;      //�ָ��ɳ�ʼ״̬����Ϊ��ν����Ѿ�����ˣ���������ʼ״̬Ҳ��TRUE
			GlobalUnlock(hGlobal);
			GlobalFree(hGlobal);
			return;
		}
		else 
		{
			m_file.WriteHuge(lpBuf, ret);
			GlobalUnlock(hGlobal);
			GlobalFree(hGlobal);
		}
	}
	UpdateData(FALSE);   //������

}

void CSafeTranDlg::StartServerThread()
{
	BOOL bIni = IniListenSocket();
	if ( bIni == FALSE )
	{
		return ;
	}

	AfxBeginThread(ListenSafeTranThread, this);
}





CString CSafeTranDlg::EncryptOne(CString S)
{
	CString Str, Str1, Result;
	int i, j;
	Str = S;
	for(i=0; i<S.GetLength(); i++)
	{
		Str.SetAt(i, S.GetAt(i)+7);
	}
	S = Str;
	for(i=0; i<S.GetLength(); i++)
	{
		j = (BYTE)S.GetAt(i);
		Str1 = "01";
		Str1.SetAt(0, 65+j/26);
		Str1.SetAt(1, 65+j%26);
		Result += Str1;
	}
	return Result;

}

CString CSafeTranDlg::EncryptTwo(CString S)
{
	CString Str, Str1, Result;
	int i, j;
	Str = S;
	for(i=0; i<S.GetLength(); i++)
	{
		Str.SetAt(i, S.GetAt(i)-7);
	}
	S = Str;
	for(i=0; i<S.GetLength(); i++)
	{
		j = (BYTE)S.GetAt(i);
		Str1 = "01";
		Str1.SetAt(0,65+j/26);
		Str1.SetAt(1,65+j%26);
		Result += Str1;
	}
	return Result;

}

void CSafeTranDlg::OnButtonNokeyDecrypt() 
{
	// TODO: Add your control notification handler code here
	m_file.Open(m_pathName, CFile::modeRead);
	DWORD size = m_file.GetLength();
	
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, size);
	void *lpBuf = GlobalLock(hGlobal);
	
	m_file.ReadHuge(lpBuf, size);
	char* buffer = (char*)lpBuf;
	m_file.Close();
	
	m_file.Open(m_pathName, CFile::modeCreate|CFile::modeWrite);
	int mod = size % 2048;   //Ϊʲô�ֱ��2048�ˣ�
	int i;
	for(i=0;i<size/2048;i++)
	{
		CString decrypt;
		char* buf = buffer+2048*i;
		if(i%2 == 1)
		{
			decrypt = DecryptOne(buf);
		}
		else
		{
			decrypt = DecryptTwo(buf);
		}
		m_file.WriteHuge(decrypt,1024);
	}
	
	if(mod != 0)
	{
		CString decrypt;
		char *buf = buffer + 2048*i;
		if(i%2 == 1)
		{
			decrypt = DecryptOne(buf);
		}
		else
		{
			decrypt = DecryptTwo(buf);
		}
		m_file.WriteHuge(decrypt, decrypt.GetLength());
	}
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);
	m_file.Close();
	AfxMessageBox("���ܳɹ�");
}

CString CSafeTranDlg::DecryptOne(CString S)
{
	CString Result,Str;
	int i, j;
	for(i=0; i<S.GetLength()/2; i++)
	{
		j = ((BYTE)S.GetAt(2*i)-65)*26;
		j += (BYTE)S.GetAt(2*i+1)-65;
		Str = "0";
		Str.SetAt(0,j);
		Result += Str;
	}
	S = Result;
	for(i=0; i<S.GetLength(); i++)
	{
		Result.SetAt(i, (BYTE)S.GetAt(i)-7);
	}
	return Result;
}

CString CSafeTranDlg::DecryptTwo(CString S)
{
	CString Result, Str;
	int i, j;
	for(i=0; i<S.GetLength()/2; i++)
	{
		j = ((BYTE)S.GetAt(2*i)-65)*26;
		j += (BYTE)S.GetAt(2*i+1)-65;
		Str = "0";
		Str.SetAt(0,j);
		Result += Str;
	}
	S = Result;
	for(i=0; i<S.GetLength(); i++)
	{
		Result.SetAt(i,(BYTE)S.GetAt(i)+7);
	}
	return Result;
}

void CSafeTranDlg::OnButtonKeySend() 
{
	// TODO: Add your control notification handler code here
	CString	selectedIP;
	if( !GetSelectedIpAddr(selectedIP, this) )
	{
		return;
	}
	
	SOCKADDR_IN AddrSendToClient;
	AddrSendToClient.sin_addr.S_un.S_addr = inet_addr(selectedIP);
	AddrSendToClient.sin_port = htons(SAFETRAN_PORT);
	AddrSendToClient.sin_family = AF_INET;
	
	CFile file(m_pathName, CFile::modeRead);
	DWORD size = file.GetLength();

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, size);
	void* lpBuf = GlobalLock(hGlobal);
	
	file.ReadHuge(lpBuf, size);
	
	int ret = sendto(m_listenSocket, m_strName, m_strName.GetLength(), 0, (struct sockaddr FAR *) &AddrSendToClient, sizeof(SOCKADDR_IN));   //�Ȱ��ļ������͹�ȥ
	if(ret == SOCKET_ERROR)
	{
		CString str;
		str.Format("��������ʧ�ܣ�������:%d",WSAGetLastError());
		AfxMessageBox(str);
	}

	Sleep(100);   //Ҫ�������
	char *buf;
	int mod = size % 1024;
	for(int i=0; i<INT(size/1024); i++)     //i��Ϊ���ĸ�����ÿ�ζ���1024�ֽڵİ������������mod����
	{
		CString encrypt;
		buf = (char*)lpBuf + 1024*i;    //buf����ô���ƺþ���ôһ�εģ�

		if(i%2 == 1)
		{
			encrypt = KeyEncryptOne(buf, m_EnMiyao);     //�����İ��Ĵ���
		}
		else
		{
			encrypt = KeyEncryptTwo(buf, m_EnMiyao);     //ż�����Ĵ���
		}
		
		int ret = sendto(m_listenSocket, encrypt, 2048, 0, (SOCKADDR*) &AddrSendToClient, sizeof(AddrSendToClient));    //Ϊʲô���2048�ˣ�
		if(ret == SOCKET_ERROR)
		{
			CString str;
			str.Format("��������ʧ�ܣ�������:%d",WSAGetLastError());
			AfxMessageBox(str);
		}
		Sleep(100);   //Ҫ����ָ���
	}

	if(mod != 0)     //����0�����ò�Ҫ������
	{
		CString encrypt;
		buf = (char*)lpBuf + (size-mod);
		if(i%2 == 1)
		{
			encrypt = KeyEncryptOne(buf, m_EnMiyao);
		}
		else
		{
			encrypt = KeyEncryptTwo(buf, m_EnMiyao);
		}
		
		int ret = sendto(m_listenSocket, encrypt, encrypt.GetLength(), 0, (SOCKADDR*)&AddrSendToClient, sizeof(AddrSendToClient));	
		if(ret == SOCKET_ERROR)
		{
			CString str;
			str.Format("��������ʧ�ܣ�������:%d",WSAGetLastError());
			AfxMessageBox(str);
			return ;
		}
		Sleep(100);
	}
	
	char a[1] = {'^'}; //������
	sendto(m_listenSocket, (char*)a, 1, 0, (SOCKADDR*)&AddrSendToClient, sizeof(AddrSendToClient));
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);
	
}

void CSafeTranDlg::OnButtonKeyDecrypt() 
{
	// TODO: Add your control notification handler code here
	m_file.Open(m_pathName, CFile::modeRead);
	DWORD size = m_file.GetLength();
	
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, size);
	void *lpBuf = GlobalLock(hGlobal);
	
	m_file.ReadHuge(lpBuf, size);
	char* buffer = (char*)lpBuf;
	m_file.Close();
	
	m_file.Open(m_pathName, CFile::modeCreate|CFile::modeWrite);
	int mod = size % 2048;   //Ϊʲô�ֱ��2048�ˣ�
	int i;
	for(i=0;i<size/2048;i++)
	{
		CString decrypt;
		char* buf = buffer+2048*i;
		if(i%2 == 1)
		{
			decrypt = KeyDecryptOne(buf, m_DeMiyao);
		}
		else
		{
			decrypt = KeyDecryptTwo(buf, m_DeMiyao);
		}
		m_file.WriteHuge(decrypt,1024);
	}
	
	if(mod != 0)
	{
		CString decrypt;
		char *buf = buffer + 2048*i;
		if(i%2 == 1)
		{
			decrypt = KeyDecryptOne(buf, m_DeMiyao);
		}
		else
		{
			decrypt = KeyDecryptTwo(buf, m_DeMiyao);
		}
		m_file.WriteHuge(decrypt, decrypt.GetLength());
	}
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);
	m_file.Close();
	AfxMessageBox("���ܳɹ�");
	
}

CString CSafeTranDlg::KeyEncryptOne(CString S, CString m_Miyao)
{
	CString Result;
	int i;
	TCHAR *temp = new TCHAR[20];
	temp = m_Miyao.GetBuffer(m_Miyao.GetLength());
	for(i=0; i<S.GetLength(); i++)
	{
		S.SetAt(i, S.GetAt(i)+(*temp));
	}
	Result = S;
	return Result;
}

CString CSafeTranDlg::KeyEncryptTwo(CString S, CString m_Miyao)
{
	CString Result;
	int i;
	TCHAR *temp = new TCHAR[20];
	temp = m_Miyao.GetBuffer(m_Miyao.GetLength());
	for(i=0; i<S.GetLength(); i++)
	{
		S.SetAt(i, S.GetAt(i)-(*temp));
	}
	Result = S;
	return Result;
}

void CSafeTranDlg::OnChangeEditKeyEn() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);   //��ĵ�����	
}

CString CSafeTranDlg::KeyDecryptTwo(CString S, CString m_DeMiyao)
{
	CString Result;
	int i;
	TCHAR *temp = new TCHAR[20];
	temp = m_DeMiyao.GetBuffer(m_DeMiyao.GetLength());
	for(i=0; i<S.GetLength(); i++)
	{
		S.SetAt(i, S.GetAt(i)+(*temp));
	}
	Result = S;
	return Result;
}

CString CSafeTranDlg::KeyDecryptOne(CString S, CString m_DeMiyao)
{
	CString Result;
	int i;
	TCHAR *temp = new TCHAR[20];
	temp = m_DeMiyao.GetBuffer(m_DeMiyao.GetLength());
	for(i=0; i<S.GetLength(); i++)
	{
		S.SetAt(i, S.GetAt(i)-(*temp));
	}
	Result = S;
	return Result;
}

void CSafeTranDlg::OnChangeEditKeyDe() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);   //��ĵ�����
}

void CSafeTranDlg::OnButtonDeSelect() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,NULL,"",OFN_HIDEREADONLY,"All Files (*.*)|*.*||",this);   //�򿪶Ի���
	if(IDOK == dlg.DoModal())
	{
		m_pathName = dlg.GetPathName();
		m_strName = dlg.GetFileName();
		m_dePath.SetWindowText(m_pathName);
	}
}
//==================================================================================================
void CSafeTranDlg::OnAddFile() 
{
	CFileDialog log(TRUE,"�ļ�",NULL,OFN_HIDEREADONLY,"�ı�(*.txt)|*.txt||",NULL);
	if(log.DoModal()==IDOK)
	{
		CString path;
		path=log.GetPathName();
		strpathtemp=log.GetFileName();
		int pos=path.Find(strpathtemp);
		strpathtemp=path.Left(pos);
		GetDlgItem(IDC_EDFILEPATH)->SetWindowText(path);
	}
}

void CSafeTranDlg::OnEncry() 
{
	CString strpic,strtxt,desname;
	GetDlgItem(IDC_EDFILEPATH)->GetWindowText(strtxt);
	GetDlgItem(IDC_EDPICPATH)->GetWindowText(strpic);
	CFile readpic,readfile,writefile;
	
	desname.Format("%smingrisofttemp.txt",strpathtemp);
	HANDLE handle=::CreateFile(desname,GENERIC_WRITE,FILE_SHARE_WRITE,0,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
	if(handle)::CloseHandle(handle);
	int i=readfile.Open(strtxt,CFile::modeRead);
	readpic.Open(strpic,CFile::modeRead);
	writefile.Open(desname,CFile::modeCreate|CFile::modeReadWrite);
	
	if(i==0)return;
	char picbuf[128];
	char filebuf[128];
	char desbuf[128];
	while(1)
	{
		ZeroMemory(filebuf,128);
		ZeroMemory(picbuf,128);
		ZeroMemory(desbuf,128);
		DWORD i=readfile.Read(filebuf,128);
		readpic.Read(picbuf,128);
		for(int p=0;p<i;p++)
		{
			char m=filebuf[p];
			char n=picbuf[p];
			desbuf[p]=m^n;
		}
		writefile.Write(desbuf,i);
		if(i==0)goto end;
	}
end:
	readpic.Close();
	readfile.Close();
	writefile.Close();
	::DeleteFile(strtxt);
	::rename(desname,strtxt);
	AfxMessageBox("�ɹ�");
}

void CSafeTranDlg::OnSelectPic() 
{
	CFileDialog log(TRUE,"�ļ�",NULL,OFN_HIDEREADONLY,"λͼ(*.BMP)|*.bmp||",NULL);
	if(log.DoModal()==IDOK)
	{
		path=log.GetPathName();
		GetDlgItem(IDC_EDPICPATH)->SetWindowText(path);
	}
}


//======================================================================================================
