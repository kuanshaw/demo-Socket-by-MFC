#pragma once

//#define port 8089

// CClientSocket ����Ŀ��

class CClientSocket : public CSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();

	virtual void OnReceive(int nErrorCode);		// ��д���պ�����ͨ����������
	BOOL SendMSG(LPSTR lpBuff, int nlen);     	// ���ͺ��������ڷ������ݸ�������      

};


