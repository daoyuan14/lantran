//#include "StdAfx.h"

// 端口的选择原则? 下列的端口选择合理不?
#define NET_LANSHOW_PORT      5212		// 广播端口, 这个端口用于动态显示局域网内所有在线用户, 也就是这种端口专门用来发一些控制数据的
// 但是飞鸽只用了一个端口啊
#define FILETRAN_SERVER_PORT  1004		// 因为我们的应用既要承担服务器的角色
#define FILETRAN_CLIENT_PORT  1521		// 又要承担客户端的角色, 所以是需要有两个端口的


// 动态显示在线用户时, 所发送的数据包
#define NET_LANSHOW_BUFFER 1024			// 感觉用不了这么大?

// 怎么才能防止尽可能不会出现这些数据，即不会干扰
// int 类型不就是32位嘛
#define ENTRY    0xffffffff			// 新用户上线			
#define ONLINE	 0xeeeeeeee			// 在线用户
#define EXIT     0xdddddddd			// 老用户离线

//#define MAXSENDNUM 8192  宏和常量定义有什么区别

// 发送的LanShow数据包中的结构
typedef struct tagLanShowData
{
	int  nState;
	char m_name[80];
	char m_iPAddr[20];
	char m_computerName[80];
} LanShowData, *pLanShowData;

// 显示在列表中对应的每个Item的数据结构
typedef struct tagClientInfo
{
	char m_ComputerName[80];
	char m_UserName[80];      //即用户名
	char m_ClientIPAddr[20];
} ClientInfo, *pClientInfo;