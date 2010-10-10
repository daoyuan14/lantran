// Sound.cpp: implementation of the CSound class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lantran.h"
#include "Sound.h"
#include "PhoneDlg.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSound::CSound()
{

}

CSound::~CSound()
{
	waveInClose(m_hWaveIn); //�ر�¼���豸
	waveOutClose(m_hWaveOut); //�ر�¼���豸
}

void CSound::Init(CPhoneDlg *dlg)
{
	int retCode;
	m_dlg=dlg;
	if(waveInGetNumDevs()==0)
	{
		AfxMessageBox("There is no sound input device");
	}
	if(waveOutGetNumDevs()==0)
	{
		AfxMessageBox("There is no sound output device");
	}
	m_soundFormat.wFormatTag=WAVE_FORMAT_PCM;
	m_soundFormat.nChannels=1;
	m_soundFormat.nSamplesPerSec=8000;
	m_soundFormat.nAvgBytesPerSec=16000;
	m_soundFormat.nBlockAlign=2;
	m_soundFormat.cbSize=0;
	m_soundFormat.wBitsPerSample=16;     //ָ��¼����ʽ
	
	retCode=waveInOpen(&m_hWaveIn,WAVE_MAPPER, &m_soundFormat, (DWORD)m_dlg->m_hWnd,0L,CALLBACK_WINDOW); //��¼���豸
	if(retCode!= MMSYSERR_NOERROR)
	{
		AfxMessageBox("Error in waveInOpen function");
		return;
	}
	retCode=waveOutOpen(&m_hWaveOut,WAVE_MAPPER, &m_soundFormat, (DWORD)m_dlg->m_hWnd,0L,CALLBACK_WINDOW); //��¼���豸
	if(retCode!= MMSYSERR_NOERROR)
	{
		AfxMessageBox("Error in waveOutOpen function");
		return;
	}
	//in
	m_pWaveHdrIn[0].lpData=m_cBufferIn;
	m_pWaveHdrIn[0].dwBufferLength=MAX_BUFFER_SIZE;
	m_pWaveHdrIn[0].dwBytesRecorded=0;
	m_pWaveHdrIn[0].dwFlags=0;
	

	retCode=waveInPrepareHeader(m_hWaveIn,&m_pWaveHdrIn[0],sizeof(WAVEHDR)); //׼���ڴ��¼��
    if(retCode!= MMSYSERR_NOERROR) 
    {
        AfxMessageBox(_T("Cannot InPrepareHeader !"));
        return;
    }
	
	retCode=waveInAddBuffer(m_hWaveIn,&m_pWaveHdrIn[0],sizeof(WAVEHDR)); //�����ڴ��
    if(retCode!= MMSYSERR_NOERROR)
	{
		AfxMessageBox(_T("Cannot InAddBuffer !"));
		return;
    }
	
	//out
	m_pWaveHdrOut[0].lpData=m_cBufferOut;
	m_pWaveHdrOut[0].dwBufferLength=MAX_BUFFER_SIZE;
	m_pWaveHdrOut[0].dwBytesRecorded=0;
	m_pWaveHdrOut[0].dwFlags=0;
	
	waveOutPrepareHeader(m_hWaveOut,&m_pWaveHdrOut[0],sizeof(WAVEHDR)); //׼���ڴ��¼��
	waveOutWrite(m_hWaveOut,&m_pWaveHdrOut[0],sizeof(WAVEHDR));
}

void CSound::Record()
{
	waveInStart(m_hWaveIn);//��ʼ¼��
}

void CSound::Play()
{
	memcpy(m_cBufferOut,m_cBufferIn,MAX_BUFFER_SIZE);
}

void CSound::Stop()
{
	waveInStop(m_hWaveIn); //ֹͣ¼��
	waveInReset(m_hWaveIn); //����ڴ��
}

void CSound::FreeRecordBuffer()
{
	int retCode;
	retCode =waveInUnprepareHeader(m_hWaveIn,&m_pWaveHdrIn[0],sizeof(WAVEHDR));
    if  (retCode!= MMSYSERR_NOERROR) 
    {
        AfxMessageBox(_T("Cannot UnInPrepareHeader !"));
        return;
    }	//in
	
	m_pWaveHdrIn[0].lpData=m_cBufferIn;
	m_pWaveHdrIn[0].dwBufferLength=MAX_BUFFER_SIZE;
	m_pWaveHdrIn[0].dwBytesRecorded=0;
	m_pWaveHdrIn[0].dwFlags=0;
	
	retCode=waveInPrepareHeader(m_hWaveIn,&m_pWaveHdrIn[0],sizeof(WAVEHDR)); //׼���ڴ��¼��
    if(retCode!= MMSYSERR_NOERROR) 
    {
        AfxMessageBox(_T("Cannot InPrepareHeader !"));
        return;
    }

	retCode=waveInAddBuffer(m_hWaveIn,&m_pWaveHdrIn[0],sizeof(WAVEHDR)); //�����ڴ��
    if(retCode!= MMSYSERR_NOERROR) 
    {
        AfxMessageBox(_T("Cannot InAddBuffer !"));
        return;
    }
}

void CSound::FreePlayBuffer()
{
	static bool once=true;
	int result;
/*	
	result=waveOutUnprepareHeader(m_hWaveOut,&m_pWaveHdrOut[0],sizeof(WAVEHDR));
 
	if  (result!= MMSYSERR_NOERROR) 
    {
	if(once)
	{
	char  errinfor[100];
	waveOutGetErrorText(result,errinfor,100);
	AfxMessageBox(_T("Cannot UnOutPrepareHeader !"));
	AfxMessageBox(errinfor);
	once=false;
	}
	return;
    };	//out	*/	
//		waveOutReset(m_hWaveOut); //����ڴ��
	
	m_pWaveHdrOut[0].lpData=m_cBufferOut;
	m_pWaveHdrOut[0].dwBufferLength=MAX_BUFFER_SIZE;
	m_pWaveHdrOut[0].dwBytesRecorded=0;
	m_pWaveHdrOut[0].dwFlags=0;
	
	result=waveOutPrepareHeader(m_hWaveOut,&m_pWaveHdrOut[0],sizeof(WAVEHDR)); //׼���ڴ��¼��
    if(result!= MMSYSERR_NOERROR) 
    {
        AfxMessageBox(_T("Cannot OutPrepareHeader !"));
        return;
    }//out
	
	result=waveOutWrite(m_hWaveOut,&m_pWaveHdrOut[0],sizeof(WAVEHDR)); //�����ڴ��
    if(result!= MMSYSERR_NOERROR) 
    {
        AfxMessageBox(_T("Cannot OutWrite !"));
        return;
    }//out
}
void CSound::InBufferZero()
{
	int i;
	for(i=0;i<MAX_BUFFER_SIZE;i++)
	{
		m_cBufferIn[i]=0;
	}
}

void CSound::OutBufferZero()
{
	int i;
	for(i=0;i<MAX_BUFFER_SIZE;i++)
	{
		m_cBufferOut[i]=0;
	}
}