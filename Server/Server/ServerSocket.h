#pragma once

// CServerSocket 命令目标
#include "ServerDlg.h"				//
class CServerDlg;					//

class CServerSocket : public CSocket
{
public:
	CServerDlg * m_pDlg;			//主对话框指针对象

	CServerSocket();
	virtual ~CServerSocket();
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};


