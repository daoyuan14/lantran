//#include "StdAfx.h"

// �˿ڵ�ѡ��ԭ��? ���еĶ˿�ѡ�����?
#define NET_LANSHOW_PORT      5212		// �㲥�˿�, ����˿����ڶ�̬��ʾ�����������������û�, Ҳ�������ֶ˿�ר��������һЩ�������ݵ�
// ���Ƿɸ�ֻ����һ���˿ڰ�
#define FILETRAN_SERVER_PORT  1004		// ��Ϊ���ǵ�Ӧ�ü�Ҫ�е��������Ľ�ɫ
#define FILETRAN_CLIENT_PORT  1521		// ��Ҫ�е��ͻ��˵Ľ�ɫ, ��������Ҫ�������˿ڵ�


// ��̬��ʾ�����û�ʱ, �����͵����ݰ�
#define NET_LANSHOW_BUFFER 1024			// �о��ò�����ô��?

// ��ô���ܷ�ֹ�����ܲ��������Щ���ݣ����������
// int ���Ͳ�����32λ��
#define ENTRY    0xffffffff			// ���û�����			
#define ONLINE	 0xeeeeeeee			// �����û�
#define EXIT     0xdddddddd			// ���û�����

//#define MAXSENDNUM 8192  ��ͳ���������ʲô����

// ���͵�LanShow���ݰ��еĽṹ
typedef struct tagLanShowData
{
	int  nState;
	char m_name[80];
	char m_iPAddr[20];
	char m_computerName[80];
} LanShowData, *pLanShowData;

// ��ʾ���б��ж�Ӧ��ÿ��Item�����ݽṹ
typedef struct tagClientInfo
{
	char m_ComputerName[80];
	char m_UserName[80];      //���û���
	char m_ClientIPAddr[20];
} ClientInfo, *pClientInfo;