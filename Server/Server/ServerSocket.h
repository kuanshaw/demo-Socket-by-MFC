#pragma once

// CServerSocket ����Ŀ��
#include "ServerDlg.h"				//
class CServerDlg;					//

class CServerSocket : public CSocket
{
public:
	CServerDlg * m_pDlg;			//���Ի���ָ�����

	CServerSocket();
	virtual ~CServerSocket();
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};


