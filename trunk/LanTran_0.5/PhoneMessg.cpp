// PhoneMessg.cpp: implementation of the CPhoneMessg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lantran.h"
#include "PhoneMessg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
IMPLEMENT_DYNCREATE(CPhoneMessg, CObject)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPhoneMessg::CPhoneMessg()
{
	Init();
}

CPhoneMessg::~CPhoneMessg()
{

}

/////////////////////////////////////////////////////////////////////////////
// CPhoneMsg Operations

void CPhoneMessg::Init()
{
	m_strText = _T("");
	m_tag=0;	//数据是0表示该数组为空，不需要放出声音,为1需要放出声音
	m_buffer[0]=0;
}

/////////////////////////////////////////////////////////////////////////////
// CPhoneMsg serialization

void CPhoneMessg::Serialize(CArchive& ar)
{
	int i=0;
	if (ar.IsStoring())
	{
		ar << m_strText;
		ar << m_tag;
		if(m_tag==1)
		{
			for(i=0;i<MAX_BUFFER_SIZE;i++)
				ar << m_buffer[i];
		}
	}
	else
	{
		ar >> m_strText;
		ar >> m_tag;
		if(m_tag==1)
		{
			for(i=0;i<MAX_BUFFER_SIZE;i++)
				ar >> m_buffer[i];
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPhoneMsg diagnostics

#ifdef _DEBUG
void CPhoneMessg::AssertValid() const
{
	CObject::AssertValid();
}

void CPhoneMessg::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG
