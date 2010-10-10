#if !defined(AFX_FILETRANDLG_H__4B55C28A_6E83_43DF_8683_870D0FA9285C__INCLUDED_)
#define AFX_FILETRANDLG_H__4B55C28A_6E83_43DF_8683_870D0FA9285C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileTranDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileTranDlg dialog
class CFileServerSocket;
class CFileClientSocket;

//#define		WM_TIMER_SENDCOUNT		WM_USER+102
//#define		WM_TIMER_RECVCOUNT		WM_USER+103
/////////////////////////////////////////////////////////////////////////////
class CFileTranDlg : public CDialog
{
// Construction
public:
	CString strAntiFile;    //���ܵ��ı����ļ�·��������
//	void SetRecvSpeed();
//	void SetSendSpeed();
	void DeleteFileSock();
	CString GetFileName(CString FileName);
	void ReceiveData();
	void AcceptConnection();
	void SendFileData();	
	CFileTranDlg(CWnd* pParent = NULL);   // standard constructor

	CFileServerSocket *m_pServerSock; //�����ļ��׽���
	CFileServerSocket *m_ServerSock; 
	CFileClientSocket *m_pSendSock;   //�����ļ��׽���

    CString m_FileName;    //���͵��ļ�����
	CImageList m_FileImageList;   //ͼ���б�
	BOOL m_IsReveived;  //�Ƿ�Ҫ�����ļ���Ϣ����һ�ν��ܵĻ�Ҫ

	CString m_ReceiveFile; //���յ��ļ�����
	DWORD m_FileSize;    //���յ��ļ���С
	DWORD m_FileLen ;    //���յ��ļ�����

	CFile m_File;        //������յ��ļ�

	BOOL m_Send;        //�Ƿ�ʼ��������
	BOOL m_SendNext;    //�Ƿ������������
	DWORD m_SendNum;     //��¼��ǰ���͵Ĵ���

	CFile m_SendFile;    //���͵��ļ�
	DWORD m_SendLen;     //��¼�Ѿ����͵��ļ�����
	DWORD m_SendFileLen; //�����ļ��ĳ���

	BOOL m_Received;    //�ж��Ƿ���յ��Է��Ļ�Ӧ��Ϣ
	BOOL m_SendFinished;//�����Ƿ����

	int  m_Count;
/*
	DWORD		g_dwSendCount; //= 0;		//�ѷ��͵��ֽ�����
	DWORD		g_dwRecvCount; //= 0;		//�ѽ��յ��ֽ�����
	int			nSendTime; //= 0;
    int			nRecvTime; //= 0;
*/
// Dialog Data
	//{{AFX_DATA(CFileTranDlg)
	enum { IDD = IDD_DIALOG_FILETRAN };
	CListCtrl	m_FileInfoList;
	CProgressCtrl	m_Progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileTranDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileTranDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSelectfile();
	afx_msg void OnButtonSendfile();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILETRANDLG_H__4B55C28A_6E83_43DF_8683_870D0FA9285C__INCLUDED_)
