
// ClientDlg.h : ͷ�ļ�
//

#pragma once

#include "afxwin.h"
#include "ClientSocket.h"


// CClientDlg �Ի���
class CClientDlg : public CDialogEx
{
// ����
public:
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CLIENT_DIALOG };

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
	DECLARE_MESSAGE_MAP();

public:
	bool m_connect;
	CClientSocket* pSock;  // �ͻ����׽���ָ�����
	BOOL WChar2MByte(LPCWSTR lpSrc, LPSTR lpDest, int nlen); //�ַ�ת������
	virtual BOOL PreTranslateMessage(MSG* pMsg);		//��ֹ����enter��escʱ�˳�����

public:
	CButton m_ConPC;
	CString m_DataSend;
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedConnect();
	UINT m_port;
	CString m_IP;
};
