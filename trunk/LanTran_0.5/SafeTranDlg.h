#if !defined(AFX_SAFETRANDLG_H__7DD4D222_649D_445F_8F51_064CA6814106__INCLUDED_)
#define AFX_SAFETRANDLG_H__7DD4D222_649D_445F_8F51_064CA6814106__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SafeTranDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSafeTranDlg dialog
#define SAFETRAN_PORT  34567
#define WM_SOCKETMSG 10000
extern bool GetSelectedIpAddr(CString &s, LPVOID lparam);
//////////////////////////////////////////////////////////////////////////////
class CSafeTranDlg : public CDialog
{
// Construction
public:
//==================================================================
	CString strpathtemp;
	CString path;
//==================================================================
	CString KeyDecryptOne(CString S, CString m_DeMiyao);
	CString KeyDecryptTwo(CString S, CString m_DeMiyao);
	CString KeyEncryptTwo(CString S, CString m_Miyao);
	CString KeyEncryptOne(CString S, CString m_Miyao);
	CString DecryptTwo(CString S);
	CString DecryptOne(CString S);
	CString EncryptTwo(CString S);
	CString EncryptOne(CString S);
	void StartServerThread();
	void GetLocalIP(DWORD &dwLocalIP);
	SOCKET m_listenSocket;
	BOOL IniListenSocket();
	CSafeTranDlg(CWnd* pParent = NULL);   // standard constructor
	CString m_pathName, m_strName;
	DWORD m_dwLocalIP;
	int num;
	BOOL m_isStop;
	CFile m_file;

// Dialog Data
	//{{AFX_DATA(CSafeTranDlg)
	enum { IDD = IDD_DIALOG_SAFETRAN };
	CStatic	m_dePath;
	CStatic	m_enPath;
	CString	m_EnMiyao;
	CString	m_DeMiyao;
//======================================
	CStatic	m_picture;
//======================================
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSafeTranDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSafeTranDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEnSelect();
	afx_msg void OnButtonNokeySend();
	afx_msg void OnSocket(WPARAM wParam, LPARAM lParam);   //必须要在这里添加
	afx_msg void OnButtonNokeyDecrypt();
	afx_msg void OnButtonKeySend();
	afx_msg void OnButtonKeyDecrypt();
	afx_msg void OnChangeEditKeyEn();
	afx_msg void OnChangeEditKeyDe();
	afx_msg void OnButtonDeSelect();
	//=============================================================
	afx_msg void OnSelectPic();
	afx_msg void OnEncry();
	afx_msg void OnAddFile();
	//==================================================================
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAFETRANDLG_H__7DD4D222_649D_445F_8F51_064CA6814106__INCLUDED_)
