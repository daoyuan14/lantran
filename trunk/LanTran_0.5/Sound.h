// Sound.h: interface for the CSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUND_H__A6CC82BC_86BB_4199_A10A_A9F764229370__INCLUDED_)
#define AFX_SOUND_H__A6CC82BC_86BB_4199_A10A_A9F764229370__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmsystem.h>

#define MAX_BUFFER_SIZE 2048

class CPhoneDlg;

class CSound  
{
public:
	void Init(CPhoneDlg* dlg);
	void Record();
	void Play();
	void Stop();
	void InBufferZero();
	void OutBufferZero();
	void FreeRecordBuffer();
	void FreePlayBuffer();
	CSound();
	virtual ~CSound();
public:
	CPhoneDlg             * m_dlg;
	WAVEFORMATEX            m_soundFormat;
	HWAVEIN					m_hWaveIn;
	HWAVEOUT				m_hWaveOut;
	WAVEHDR					m_pWaveHdrIn[3];
	WAVEHDR					m_pWaveHdrOut[3];
	CHAR					m_cBufferIn[MAX_BUFFER_SIZE];
	CHAR					m_cBufferOut[MAX_BUFFER_SIZE];
	
};

#endif // !defined(AFX_SOUND_H__A6CC82BC_86BB_4199_A10A_A9F764229370__INCLUDED_)
