#if !defined(AFX_PHONEDLG_H__064AC888_996B_4936_A677_DFF2B9A6917B__INCLUDED_)
#define AFX_PHONEDLG_H__064AC888_996B_4936_A677_DFF2B9A6917B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PhoneDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg dialog
#include "PhoneServerSocket.h"
#include "PhoneClientSocket.h"
#include "Sound.h"
#include <afxmt.h>

class CPhoneDlg : public CDialog
{
// Construction
public:
//==================================================
	HMIXER m_hMixer;
	MIXERCAPS m_mxcaps;
	DWORD m_curvalue;
	DWORD m_controlid;
//===================================================
	CPhoneDlg(CWnd* pParent = NULL);   // standard constructor
	~CPhoneDlg();
// Dialog Data
	//{{AFX_DATA(CPhoneDlg)
	enum { IDD = IDD_DIALOG_PHONE };
	CString	m_sInputString;
	CString	m_sShowString;
	UINT	m_uPort;		// NOTE: the ClassWizard will add data members here
//========================================================================
	CSliderCtrl	m_control;
//===============================================================================
//}}AFX_DATA

public:
	void ProcessPendingAccept();
	void CloseSessionSocket();
	void ClearContent();
	void OnClearconnection();
	
public:

	bool m_bInit;
	bool m_bClient;
	CPhoneClientSocket m_clientsocket;
	CPhoneServerSocket m_pListenSocket;
	CPtrList m_connectionList;
	CString m_sMsgList;
	
	CSound  m_sound;
	CMutex  m_mutex;
	BOOL    m_bIsFileClosed;
	BOOL    m_willchating;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhoneDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPhoneDlg)
    virtual	BOOL OnInitDialog();
	afx_msg void OnInputText();
	afx_msg void OnSetserver();
	afx_msg void OnNewsend();
	afx_msg void WriteBufferFull(LPARAM lp,WPARAM wp);		// NOTE: the ClassWizard will add member functions here
	afx_msg void OnConnectserver();
	afx_msg void OnSound();
//=======================================================================================
	afx_msg LONG OnMixerCtrlChange(UINT wParam, LONG lParam);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//============================================================
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHONEDLG_H__064AC888_996B_4936_A677_DFF2B9A6917B__INCLUDED_)
