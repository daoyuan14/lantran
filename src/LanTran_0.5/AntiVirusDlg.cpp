// AntiVirusDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LanTran.h"
#include "AntiVirusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAntiVirusDlg dialog
extern BOOL isAnti;
CString strAntiSoft;     //为什么不好在头文件中定义呢？

//////////////////////////////////////////////////////////////////////////////
CAntiVirusDlg::CAntiVirusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAntiVirusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAntiVirusDlg)
	m_AntiSoft = _T("");
	//}}AFX_DATA_INIT
}


void CAntiVirusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAntiVirusDlg)
	DDX_Control(pDX, IDC_STATIC_ISANTI, m_AntiState);
	DDX_Text(pDX, IDC_EDIT_ANTISOFT, m_AntiSoft);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAntiVirusDlg, CDialog)
	//{{AFX_MSG_MAP(CAntiVirusDlg)
	ON_BN_CLICKED(IDC_BUTTON_ANTIOK, OnButtonAntiOk)
	ON_BN_CLICKED(IDC_BUTTON_ANTICANCEL, OnButtonAntiCancel)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_EN_CHANGE(IDC_EDIT_ANTISOFT, OnChangeEditAntiSoft)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAntiVirusDlg message handlers

void CAntiVirusDlg::OnButtonAntiOk() 
{
	// TODO: Add your control notification handler code here
	isAnti = TRUE;
	AfxMessageBox("关联成功！");
	m_AntiState.SetWindowText("关联");
	GetDlgItem(IDC_BUTTON_ANTICANCEL)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_ANTIOK)->EnableWindow(false);
	
}

void CAntiVirusDlg::OnButtonAntiCancel() 
{
	// TODO: Add your control notification handler code here
	isAnti = FALSE;
	AfxMessageBox("已取消关联！但建议你关联杀毒！");
	m_AntiState.SetWindowText("不关联");
	GetDlgItem(IDC_BUTTON_ANTICANCEL)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_ANTIOK)->EnableWindow(true);
	
}

void CAntiVirusDlg::OnButtonBrowse() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, NULL, "", OFN_HIDEREADONLY, "Exe Files (*.exe)|*.exe||", this);   //打开对话框
	if(IDOK == dlg.DoModal())
	{
		m_pathName = dlg.GetPathName();    //获取完整路径和文件名
		m_AntiSoft = m_pathName;
		//m_AntiSoft.Replace("\ ", "\\");//转义符很有问题\
		UpdateData(FALSE);
		strAntiSoft = m_AntiSoft;		
	}
	
}

BOOL CAntiVirusDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_AntiSoft = "C:\\Program Files\\Rising\\Rav\\Rav.exe";
	UpdateData(FALSE);

	GetDlgItem(IDC_BUTTON_ANTIOK)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_ANTICANCEL)->EnableWindow(true);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAntiVirusDlg::OnChangeEditAntiSoft() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);   //真的到变量
	strAntiSoft = m_AntiSoft;
//	AfxMessageBox(m_AntiSoft);
}
