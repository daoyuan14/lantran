#if !defined(AFX_ANTIVIRUSDLG_H__B5CFE7C0_A532_4175_9A36_483FB4172296__INCLUDED_)
#define AFX_ANTIVIRUSDLG_H__B5CFE7C0_A532_4175_9A36_483FB4172296__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AntiVirusDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAntiVirusDlg dialog

class CAntiVirusDlg : public CDialog
{
// Construction
public:
	CAntiVirusDlg(CWnd* pParent = NULL);   // standard constructor
	CString m_pathName;

// Dialog Data
	//{{AFX_DATA(CAntiVirusDlg)
	enum { IDD = IDD_DIALOG_ANTIVIRUS };
	CStatic	m_AntiState;
	CString	m_AntiSoft;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAntiVirusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAntiVirusDlg)
	afx_msg void OnButtonAntiOk();
	afx_msg void OnButtonAntiCancel();
	afx_msg void OnButtonBrowse();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditAntiSoft();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANTIVIRUSDLG_H__B5CFE7C0_A532_4175_9A36_483FB4172296__INCLUDED_)
