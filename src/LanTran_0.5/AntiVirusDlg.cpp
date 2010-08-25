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
CString strAntiSoft;     //Ϊʲô������ͷ�ļ��ж����أ�

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
	AfxMessageBox("�����ɹ���");
	m_AntiState.SetWindowText("����");
	GetDlgItem(IDC_BUTTON_ANTICANCEL)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_ANTIOK)->EnableWindow(false);
	
}

void CAntiVirusDlg::OnButtonAntiCancel() 
{
	// TODO: Add your control notification handler code here
	isAnti = FALSE;
	AfxMessageBox("��ȡ�������������������ɱ����");
	m_AntiState.SetWindowText("������");
	GetDlgItem(IDC_BUTTON_ANTICANCEL)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_ANTIOK)->EnableWindow(true);
	
}

void CAntiVirusDlg::OnButtonBrowse() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, NULL, "", OFN_HIDEREADONLY, "Exe Files (*.exe)|*.exe||", this);   //�򿪶Ի���
	if(IDOK == dlg.DoModal())
	{
		m_pathName = dlg.GetPathName();    //��ȡ����·�����ļ���
		m_AntiSoft = m_pathName;
		//m_AntiSoft.Replace("\ ", "\\");//ת�����������\
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
	UpdateData(TRUE);   //��ĵ�����
	strAntiSoft = m_AntiSoft;
//	AfxMessageBox(m_AntiSoft);
}
