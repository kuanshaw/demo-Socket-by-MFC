// ClientSocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ClientSocket.h"
#include "ClientDlg.h"

// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}


// CClientSocket ��Ա����

//���ͻ��˽��յ��������˷�������ʱ����Ӧ���պ���OnReceive������ֻ�Ǽ򵥵Ľ���ȡ��
//��Ϣ��ʾ�ڱ༭���С�SendMSG���������������������Ϣ�������������Ի������е��á�
void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	char* pData = NULL;
	pData = new char[1024];
	memset(pData, 0, sizeof(char)* 1024);
	UCHAR leng = 0;
	CString str;
	leng = Receive(pData, 1024, 0);
	str = pData;

	//�ڱ༭������ʾ���յ�������
	((CClientDlg*)theApp.GetMainWnd())->SetDlgItemTextW(IDC_DATARECEIVE, str);
	delete pData;
	pData = NULL;
	CSocket::OnReceive(nErrorCode);
}

BOOL CClientSocket::SendMSG(LPSTR lpBuff, int nlen)
{
	//����Э��ͷ
	if (Send(lpBuff, nlen) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("���ʹ���"));
		return FALSE;
	}
	return  TRUE;
}

