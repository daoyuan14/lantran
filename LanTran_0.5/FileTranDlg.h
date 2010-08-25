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
	CString strAntiFile;    //接受到的本地文件路径加名字
//	void SetRecvSpeed();
//	void SetSendSpeed();
	void DeleteFileSock();
	CString GetFileName(CString FileName);
	void ReceiveData();
	void AcceptConnection();
	void SendFileData();	
	CFileTranDlg(CWnd* pParent = NULL);   // standard constructor

	CFileServerSocket *m_pServerSock; //接收文件套接字
	CFileServerSocket *m_ServerSock; 
	CFileClientSocket *m_pSendSock;   //发送文件套接字

    CString m_FileName;    //传送的文件名称
	CImageList m_FileImageList;   //图像列表
	BOOL m_IsReveived;  //是否要接受文件信息，第一次接受的话要

	CString m_ReceiveFile; //接收的文件名称
	DWORD m_FileSize;    //接收的文件大小
	DWORD m_FileLen ;    //接收的文件长度

	CFile m_File;        //保存接收的文件

	BOOL m_Send;        //是否开始发送数据
	BOOL m_SendNext;    //是否继续发送数据
	DWORD m_SendNum;     //记录当前发送的次数

	CFile m_SendFile;    //发送的文件
	DWORD m_SendLen;     //记录已经发送的文件长度
	DWORD m_SendFileLen; //发送文件的长度

	BOOL m_Received;    //判断是否接收到对方的回应信息
	BOOL m_SendFinished;//发送是否完成

	int  m_Count;
/*
	DWORD		g_dwSendCount; //= 0;		//已发送的字节数。
	DWORD		g_dwRecvCount; //= 0;		//已接收的字节数。
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
