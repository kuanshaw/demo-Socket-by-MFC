
// ServerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include "ServerSocket.h"			//
class CServerSocket;                //һ��Ҫ����



// CServerDlg �Ի���
class CServerDlg : public CDialogEx
{
// ����
public:
	CServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	UINT m_userCount;
	UINT m_port;
	CEdit m_event;
	afx_msg void OnBnClickedStartserver();


	CServerSocket* listenSocket;     // ���ڴ򿪷�����
	CPtrList m_clientList;                 // �������ڴ洢�û�
	bool m_connect;                       // ���ڱ�Ƿ�����״̬
	void AddClient();                       // �����û�����Ӧ�û�����
	void RemoveClient(CServerSocket* pSocket);          // �Ƴ����ߵ��û�
	void RecvData(CServerSocket* pSocket);                 // ��ȡ����
	void UpdateEvent(CString str);  // �����¼���־
	BOOL WChar2MByte(LPCWSTR srcBuff, LPSTR destBuff, int nlen);        //�ַ�ת��
	void SendMSG(CString str);       // ������Ϣ�������ͻ���
	void ControlPC(CString AndroidControl);  // �ֻ�����PC����Ӧ����


	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
