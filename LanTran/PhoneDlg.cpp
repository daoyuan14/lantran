// PhoneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LanTran.h"
#include "PhoneDlg.h"
#include "fstream"
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg dialog


CPhoneDlg::CPhoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPhoneDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPhoneDlg)
	m_sInputString = _T("");
	m_sShowString = _T("");
	m_uPort = 4000;		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CPhoneDlg::~CPhoneDlg()
{

}

void CPhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CPhoneDlg)
	DDX_Control(pDX, IDC_CONTROL, m_control);
	DDX_Text(pDX, IDC_INPUTTEXT, m_sInputString);
	DDV_MaxChars(pDX, m_sInputString, 1024);
	DDX_Text(pDX, IDC_SHOWTEXT, m_sShowString);
	DDX_Text(pDX, IDC_PORT, m_uPort);
	DDV_MinMaxUInt(pDX, m_uPort, 1024, 100000);
	// NOTE: the ClassWizard will add DDX and DDV calls here

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPhoneDlg, CDialog)
	//{{AFX_MSG_MAP(CPhoneDlg)
	ON_BN_CLICKED(IDC_CONNECTSERVER, OnConnectserver)
	ON_BN_CLICKED(IDC_SETSERVER, OnSetserver)
	ON_BN_CLICKED(IDC_NEWSEND, OnNewsend)
	ON_EN_CHANGE(IDC_INPUTTEXT, OnInputText)
	ON_MESSAGE(MM_WIM_DATA,WriteBufferFull)
	//======================================================
	ON_MESSAGE(MM_MIXM_CONTROL_CHANGE,OnMixerCtrlChange)
	ON_WM_HSCROLL()
	//========================================================
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg message handlers

BOOL CPhoneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//==================================================================================
	BYTE f0,f1,f2,f3;
	CString name;
	CPhoneClientSocket::GetLocalHostName(name);
	CPhoneClientSocket::GetIpAddress(name,f0,f1,f2,f3);
	((CIPAddressCtrl *)(GetDlgItem(IDC_IPADDRESS)))->SetAddress(f0,f1,f2,f3);
	m_bInit=false;
	m_bClient=false;
	GetDlgItem(IDC_NEWSEND)->EnableWindow(false);
	m_willchating=TRUE;
//============================================================================================================	
	MIXERLINE mxl;
	MIXERCONTROL mxc;
	MIXERLINECONTROLS mxlc;
	
	mixerOpen(&m_hMixer,0,(DWORD)this->GetSafeHwnd(),
		NULL,MIXER_OBJECTF_MIXER | CALLBACK_WINDOW);
	mixerGetDevCaps((UINT)m_hMixer, &m_mxcaps, sizeof(MIXERCAPS));
	mxl.cbStruct = sizeof(MIXERLINE);
	mxl.dwComponentType =MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	
	mixerGetLineInfo((HMIXEROBJ)m_hMixer,&mxl,
		MIXER_OBJECTF_HMIXER|MIXER_GETLINEINFOF_COMPONENTTYPE);
	
	mxlc.cbStruct=sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID=mxl.dwLineID;
	mxlc.dwControlType=MIXERCONTROL_CONTROLTYPE_VOLUME;
	mxlc.cbmxctrl=sizeof(MIXERCONTROL);
	mxlc.cControls=1;//一般为1
	mxlc.pamxctrl=&mxc;
	mixerGetLineControls((HMIXEROBJ)m_hMixer,&mxlc,
		MIXER_OBJECTF_HMIXER|MIXER_GETLINECONTROLSF_ONEBYTYPE);
	
	m_controlid=mxc.dwControlID;
	
	m_control.SetRange(mxc.Bounds.lMinimum,mxc.Bounds.lMaximum);
	MIXERCONTROLDETAILS_SIGNED mxcdVolume;
	MIXERCONTROLDETAILS mxcd;
	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = mxc.dwControlID;
	mxcd.cChannels = 1;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_SIGNED);
	mxcd.paDetails = &mxcdVolume;
	
	mixerGetControlDetails((HMIXEROBJ)m_hMixer,&mxcd,
		MIXER_OBJECTF_HMIXER|MIXER_GETCONTROLDETAILSF_VALUE);
	m_control.SetPos(mxcdVolume.lValue);
//===================================================================================================================

	//===============================================================================================


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPhoneDlg::OnInputText()
{
	if(!m_bInit)
	{
		AfxMessageBox("没有连接");
		return;
	}
	CString in;
	CPhoneMessg msg;
	GetDlgItemText(IDC_INPUTTEXT,in);
	if(in.GetLength()<1)
	{
		return;
	}
	if(in.GetAt(in.GetLength()-1)=='\n')
	{
		in.TrimRight(" ");
		SetDlgItemText(IDC_INPUTTEXT,"");
		if(in.GetLength()>2)
		{
			m_sMsgList+=in;
			
			SetDlgItemText(IDC_SHOWTEXT,m_sMsgList);
			int m_iLineCurrentPos=((CEdit *)(GetDlgItem(IDC_SHOWTEXT)))->GetLineCount();
			((CEdit *)(GetDlgItem(IDC_SHOWTEXT)))->LineScroll(m_iLineCurrentPos);
			msg.m_strText=in;
			if(!m_bClient)
			{
				POSITION pos;
				for(pos=m_connectionList.GetHeadPosition();pos!=NULL;)
				{
					CPhoneClientSocket * t= (CPhoneClientSocket *)m_connectionList.GetNext(pos);
					t->SendMessage(&msg);
				}
			}
			else
			{
				m_clientsocket.SendMessage(&msg);
			}
		}
	}
}



void CPhoneDlg::ProcessPendingAccept()
{
	CPhoneClientSocket* pSocket = new CPhoneClientSocket();

	if (m_pListenSocket.Accept(*pSocket))
	{
		CPhoneMessg msg;
		msg.m_strText="一个游客进入聊天室了";
		m_sShowString+="一个游客进入聊天室了\n";
		POSITION pos;
		for(pos=m_connectionList.GetHeadPosition();pos!=NULL;)
		{
			CPhoneClientSocket * t= (CPhoneClientSocket *)m_connectionList.GetNext(pos);
			t->SendMessage(&msg);
		}
		pSocket->Init(this);
		m_connectionList.AddTail(pSocket);
	}
	else
		delete pSocket;
}


void CPhoneDlg::ClearContent()
{
	if(m_bClient)
	{
		m_clientsocket.Close();
	}
	else
	{
		m_pListenSocket.Close();
		m_connectionList.RemoveAll();
	}
	m_bInit=false;
	m_sMsgList="";
	SetDlgItemText(IDC_INPUTTEXT,"");
	SetDlgItemText(IDC_SHOWTEXT,"");
}

void CPhoneDlg::OnClearconnection() 
{
	ClearContent();
}

void CPhoneDlg::CloseSessionSocket()
{
	if(!m_bClient)
	{
		for(POSITION pos=m_connectionList.GetHeadPosition();pos!=NULL;)
		{
			POSITION t_pos=pos;
			CPhoneClientSocket * t = (CPhoneClientSocket*)m_connectionList.GetNext(pos);
			if(t->m_bClose==true)
			{
				m_connectionList.RemoveAt(t_pos);
			}
		}
	}
}

void CPhoneDlg::OnConnectserver() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit)
	{
		BYTE f0,f1,f2,f3;
		CString name;
		((CIPAddressCtrl *)(GetDlgItem(IDC_IPADDRESS)))->GetAddress(f0,f1,f2,f3);
		CString ip;
		ip.Format("%d.%d.%d.%d",f0,f1,f2,f3);
		m_bClient=true;
		m_clientsocket.Create();
		if(m_clientsocket.Connect(ip,GetDlgItemInt(IDC_PORT)))
		{
			m_clientsocket.Init(this);
			SetDlgItemText(IDC_SHOWTEXT,"连接主机成功");
            GetDlgItem(IDC_NEWSEND)->EnableWindow(true);//开始语音可用
            GetDlgItem(IDC_SETSERVER)->EnableWindow(false);
            GetDlgItem(IDC_CONNECTSERVER)->EnableWindow(false);
			m_bInit=true;
		}
		else
		{
			m_clientsocket.Close();
			AfxMessageBox("连接主机失败");
            GetDlgItem(IDC_NEWSEND)->EnableWindow(false);
			m_bInit=false;
		}
	}	
}

void CPhoneDlg::OnSetserver() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit)
	{
		m_bClient=false;
		m_bInit=true;
		if(m_pListenSocket.Init(GetDlgItemInt(IDC_PORT),this)==FALSE)
		{
			m_bInit=false;
           
			return;
		}
      GetDlgItem(IDC_NEWSEND)->EnableWindow(true);
	}
	
}
//=============================================================================================
//可恶，这个dll还有对话框，不会debug只好从头开始了。
//作者最早用了这个Sound.dll来实现语音传输（程序代码如下），但是居然发现这个
//dll中老是会弹出一个对话框，十分不爽。所以后来干脆直接从底层写起。
void CPhoneDlg::OnSound() 
{
	// TODO: Add your control notification handler code here
	if(m_bInit==false)
	{
		AfxMessageBox("Please Prepare the Network");
		return;
	}
	
	static BOOL issend=TRUE;
	CString ip;
	BYTE f0,f1,f2,f3;
	((CIPAddressCtrl *)(GetDlgItem(IDC_IPADDRESS)))->GetAddress(f0,f1,f2,f3);
	ip.Format("%d.%d.%d.%d",f0,f1,f2,f3);
	int port=GetDlgItemInt(IDC_PORT);
	
	typedef long _stdcall SETIP(char *);
	typedef void _stdcall SETPORT(int);
	typedef void _stdcall STARTSOUND();
	typedef void _stdcall STOPSOUND();
	
	static HINSTANCE  sound=LoadLibrary("../Sound/Sound.dll");
	
	if(issend)
	{
		if(sound!=NULL)
		{
			SETIP *setip=(SETIP*)GetProcAddress(sound,"setIpAddr");
			SETPORT *setport=(SETPORT*)GetProcAddress(sound,"setPort");
			STARTSOUND *start=(STARTSOUND*)GetProcAddress(sound,"SoundStart");
			
			setport(port);
			if(setip(ip.GetBuffer(0)))
			{			
				start();
				SetDlgItemText(IDC_SOUND,"停止语音聊天");
				issend=FALSE;
				ip.ReleaseBuffer();
			}
			else
			{
				AfxMessageBox("Cannot connect to the server");
				FreeLibrary(sound);
				return;
			}
			
		}
		else
		{
			AfxMessageBox("Error Loading the sound.dll");
		}
	}
	else
	{
		if(sound!=NULL)
		{
			STOPSOUND *stop= (STOPSOUND*)GetProcAddress(sound,"SoundStop");
			stop();
			FreeLibrary(sound);
		}
		SetDlgItemText(IDC_SOUND,"开始语音聊天");
		issend=TRUE;
	}
	
}
//=======================================================================================================

void CPhoneDlg::OnNewsend() 
{
	// TODO: Add your control notification handler code here
	if(m_willchating==TRUE)
	{
		m_sound.Init(this);
		m_sound.Record();
		SetDlgItemText(IDC_NEWSEND,"关闭语音聊天");
		m_willchating=FALSE;	
	}
	else
	{
		CSingleLock lock(&m_mutex,TRUE);
		m_sound.Stop();
		SetDlgItemText(IDC_NEWSEND,"语音聊天");
		m_willchating=TRUE;
		//===============================================
		GetDlgItem(IDC_NEWSEND)->EnableWindow(false);
		//================================================
		lock.Unlock();
        
	}
}

void CPhoneDlg::WriteBufferFull(LPARAM lp,WPARAM wp)
{
	m_sound.Play();//发出本地声音
	CSingleLock lock(&m_mutex,TRUE);
	CPhoneMessg msg;
	msg.m_strText="";
	msg.m_tag=1;
	memcpy(msg.m_buffer,m_sound.m_cBufferIn,MAX_BUFFER_SIZE);
	if(!m_bClient)
	{
		POSITION pos;
		for(pos=m_connectionList.GetHeadPosition();pos!=NULL;)
		{
			CPhoneClientSocket * t= (CPhoneClientSocket *)m_connectionList.GetNext(pos);
			t->SendMessage(&msg);
		}
	}
	else
	{
		m_clientsocket.SendMessage(&msg);
	}
	m_sound.FreeRecordBuffer();
	m_sound.FreePlayBuffer();
	lock.Unlock();
}
//=======================================================
//========================================================

void CPhoneDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	DWORD val;
	val=((CSliderCtrl*)pScrollBar)->GetPos();
	MIXERCONTROLDETAILS_UNSIGNED mxcdVolume = {val};
	MIXERCONTROLDETAILS mxcd;
	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = m_controlid;
	mxcd.cChannels = 1;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	mxcd.paDetails = &mxcdVolume;
	
	mixerSetControlDetails((HMIXEROBJ)m_hMixer,&mxcd,
		MIXER_OBJECTF_HMIXER|MIXER_SETCONTROLDETAILSF_VALUE);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
LONG CPhoneDlg::OnMixerCtrlChange(UINT wParam, LONG lParam)
{
	if ((HMIXER)wParam == m_hMixer && (DWORD)lParam ==m_controlid)
	{
		MIXERCONTROLDETAILS_UNSIGNED mxcdVolume;
		MIXERCONTROLDETAILS mxcd;
		mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
		mxcd.dwControlID = m_controlid;
		mxcd.cChannels = 1;
		mxcd.cMultipleItems = 0;
		mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
		mxcd.paDetails = &mxcdVolume;
		
		mixerGetControlDetails((HMIXEROBJ)m_hMixer,&mxcd,
			MIXER_OBJECTF_HMIXER|MIXER_GETCONTROLDETAILSF_VALUE);
		
		m_control.SetPos(mxcdVolume.dwValue);
	}
	return 0L;
}
