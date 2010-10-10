// PhoneMessg.h: interface for the CPhoneMessg class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _CMESSG_
#define _CMESSG_
#include "Sound.h"

class CPhoneMessg  :public CObject
{
protected:
	DECLARE_DYNCREATE(CPhoneMessg)
public:
	CPhoneMessg();
	
	// Attributes
public:
	CString m_strText;
	CHAR    m_tag;	//0表示不需要放出声音，1表示需要放出声音
	CHAR    m_buffer[MAX_BUFFER_SIZE];
	// Operations
public:
	void Init();
	
	// Implementation
public:
	virtual ~CPhoneMessg();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#endif

/////////////////////////////////////////////////////////////////////////////
