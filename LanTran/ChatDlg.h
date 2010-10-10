#if !defined(AFX_CHATDLG_H__C1A06541_A8AD_4C5D_B4F6_3898D4D30A84__INCLUDED_)
#define AFX_CHATDLG_H__C1A06541_A8AD_4C5D_B4F6_3898D4D30A84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChatDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChatDlg dialog
#define		CHATPORT		5208
extern bool GetSelectedIpAddr(CString &s, LPVOID lparam);
/////////////////////////////////////////////////////////////////////////////
class CChatDlg : public CDialog
{
// Construction
public:
	char m_localUserName[80];
	SOCKET m_sockChat;
	CChatDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChatDlg)
	enum { IDD = IDD_DIALOG_CHAT };
	CButton	m_SendChat;
	CButton	m_Clear;
	CEdit	m_editSendChat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChatDlg)
	afx_msg void OnButtonClear();
	afx_msg void OnButtonSendChat();
	afx_msg void OnMaxtextEditReceive();
	afx_msg void OnMaxtextEditSandChat();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATDLG_H__C1A06541_A8AD_4C5D_B4F6_3898D4D30A84__INCLUDED_)
