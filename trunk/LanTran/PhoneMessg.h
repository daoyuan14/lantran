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
	CHAR    m_tag;	//0��ʾ����Ҫ�ų�������1��ʾ��Ҫ�ų�����
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
