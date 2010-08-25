// LanTranDlg.h : header file
//

#if !defined(AFX_LANTRANDLG_H__C08D9189_141B_41DC_A82A_D0DB90DE6591__INCLUDED_)
#define AFX_LANTRANDLG_H__C08D9189_141B_41DC_A82A_D0DB90DE6591__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileTranDlg.h"
#include "AntiVirusDlg.h"
#include "ChatDlg.h"
#include "SafeTranDlg.h"
#include "PhoneDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CLanTranDlg dialog

class CLanTranDlg : public CDialog
{
// Construction
public:
	void AddAllClientInfo();
	void RecvLanShowData();
	void SendLanShowData(int nState);
	static DWORD WINAPI LanShowThread(LPVOID lParam);
	DWORD m_id;
	TCHAR m_TLocalUserName[80];
	void InitMyClientInfo();
	SOCKADDR_IN m_listenAddr;
	char m_localIPAddr[20];
	char m_hostName[80];
	void GetLocalIP(DWORD &dwLocalIP);
	DWORD m_dwLocalIP;					// my own IP
	SOCKET m_LanShowSocket;				// 动态显示所有在线用户的套接字, 是UDP套接字
	BOOL InitLanShowSocket();
	void StartLanShow();
	CPhoneDlg * m_pPhoneDlg;
	void DeleteTray();
	int m_nOpenState;					// =0为主界面; =1为系统托盘
	void ToTray();
	void DoTab(int nSel);
	void SetDlgState(CWnd *pWnd, BOOL bShow);
	CImageList m_ImageList;
	CAntiVirusDlg * m_pAntiVirusDlg;
	CSafeTranDlg * m_pSafeTranDlg;
	CChatDlg * m_pChatDlg;
	CFileTranDlg  * m_pFileTranDlg;
	CLanTranDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLanTranDlg)
	enum { IDD = IDD_LANTRAN_DIALOG };
	CTabCtrl	m_Tab;
	CListCtrl	m_ClientInfoList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLanTranDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLanTranDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonHide();
	afx_msg LRESULT OnShowTask(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnButtonNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LANTRANDLG_H__C08D9189_141B_41DC_A82A_D0DB90DE6591__INCLUDED_)
