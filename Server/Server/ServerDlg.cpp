
// ServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CServerDlg �Ի���




CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerDlg::IDD, pParent)
	, m_userCount(0)
	, m_port(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USERCOUNT, m_userCount);
	DDX_Text(pDX, IDC_PORT, m_port);
	DDX_Control(pDX, IDC_EVENTRECORD, m_event);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_STARTSERVER, &CServerDlg::OnBnClickedStartserver)
END_MESSAGE_MAP()


// CServerDlg ��Ϣ�������

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//���������ڴ򿪻�رշ���������Ҫ�õ�Create������Listen�������ڴ�����������
//�����ͻ��ˡ����ж˿ںŴӱ༭���ȡ��Ӧ�ó���Ŀ��ö˿ڷ�Χ��1024-65535��
void CServerDlg::OnBnClickedStartserver()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_connect)
	{
		listenSocket = NULL;
		delete listenSocket;
		m_connect = false;
		SetDlgItemText(IDC_STARTSERVER, _T("�򿪷�����"));
		UpdateEvent(_T("ϵͳ�رշ�����."));
		return;
	}
	listenSocket = new CServerSocket();
	listenSocket->m_pDlg = this;

	// ָ���Ի���Ϊ���Ի��򣬲����������
	UpdateData(true);

	if(!listenSocket->Create(m_port))                     
	// �������������׽��֣�IP��ַĬ�ϱ���IP
	{
		AfxMessageBox(_T("�����׽��ִ���"));
		listenSocket->Close();
		return;
	}
	if(!listenSocket->Listen())
	{
		AfxMessageBox(_T("����ʧ�ܣ�"));
		listenSocket->Close();
		return;
	}
	m_connect = true;
	SetDlgItemText(IDC_STARTSERVER, _T("�رշ�����"));
	UpdateEvent(_T("ϵͳ�򿪷�����."));
}

//��������CServerSocket���е�OnAccept��Ϣ�е��ã�������Ӧ�û����ӷ�����������
//��Ҫ����ΪAccept�������ӳɹ���ͨ������m_clientList�������û���������־����
//���û����͡�Hello����ʾ��ӭ��
void CServerDlg::AddClient()
{
    CServerSocket *pSocket = new CServerSocket;
    pSocket->m_pDlg = this;
    listenSocket->Accept(*pSocket);
    pSocket->AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE);
    m_clientList.AddTail(pSocket);
    m_userCount = m_clientList.GetCount();
    UpdateData(false);
    UpdateEvent(_T("�û����ӷ�����."));

    SendMSG(_T("Hello!YOKU~\r\n"));			///����
}


//��������CServerSocket���е�OnClose��Ϣ�е��ã��õ�POSITION�ṹ�����Ҵ洢
//�û�����λ�û������ˣ��������û��ͷţ���������ɾ������������־��
void CServerDlg::RemoveClient(CServerSocket* pSocket)
{
	POSITION nPos = m_clientList.GetHeadPosition();
	POSITION nTmpPos = nPos;
	while (nPos)
	{
		CServerSocket* pSockItem = (CServerSocket*)m_clientList.GetNext(nPos);
		if (pSockItem->m_hSocket == pSocket->m_hSocket)
		{                          
			pSockItem->Close();
			delete pSockItem;
			m_clientList.RemoveAt(nTmpPos);
			m_userCount = m_clientList.GetCount();
			UpdateData(false);
			UpdateEvent(_T("�û��뿪."));
			return;
		}
		nTmpPos = nPos;
	}
}


//��������CServerSocket���е�OnReceive��Ϣ�е��ã����ڴ�����յ������ݲ����Ƶ��ԣ�
//��������ת���������û���������Ⱥ��Ϣ����ͨ��CSocket���GetPeerName�������Ի�ȡ
//�û���IP�Ͷ˿ںš�
void CServerDlg::RecvData(CServerSocket* pSocket)
{
	char* pData = NULL;
	pData = new char[1024];
	memset(pData, 0, sizeof(char)* 1024);
	UCHAR leng = 0;
	CString str;
	if (pSocket->Receive(pData, 1024, 0) != SOCKET_ERROR)
	{
		str = pData;
		ControlPC(str);     // ����ָ����Ƶ���
		SendMSG(str);     // ת�����ݸ������û��������������ݵ��û�
	}
	delete pData;
	pData = NULL;
}


//��������������Ҫ������־�ĵط����е��ã������������¼�û��ĵ�¼���˳��¼���
void CServerDlg::UpdateEvent(CString str)
{
	CString string;
	CTime time = CTime::GetCurrentTime();                  
	// ��ȡϵͳ��ǰʱ��
	str += _T("\r\n");                                                             
	// ���ڻ�����ʾ��־
	string = time.Format(_T("%Y/%m/%d %H:%M:%S  ")) + str;         
	// ��ʽ����ǰʱ��
	int lastLine = m_event.LineIndex(m_event.GetLineCount() - 1);
	//��ȡ�༭�����һ������
	m_event.SetSel(lastLine+1,lastLine+2,0);                                     
	//ѡ��༭�����һ��
	m_event.ReplaceSel(string); 
	//�滻��ѡ��һ�е�����
}

//�������ڷ��ͺ���SendMSG�е��ã������ַ�����ת���������ַ�ת��Ϊ���ַ�����
//����ת���Ļ������շ�ֻ�ܽ���һ���ֽڡ�
BOOL CServerDlg::WChar2MByte(LPCWSTR srcBuff, LPSTR destBuff, int nlen)
{
	int n = 0;
	n = WideCharToMultiByte(CP_OEMCP, 0, srcBuff, -1, destBuff, 0, 0, FALSE);
	if (n<nlen)return FALSE;
	WideCharToMultiByte(CP_OEMCP, 0, srcBuff, -1, destBuff, nlen, 0, FALSE);
	return TRUE;
}

//���ͺ��������ڷ�����Ϣ�������û�����Ҫ����ΪSend����AddClient��RecvData��
//���е��ã�������ʱ���÷���Ϣ���û���
void CServerDlg::SendMSG(CString str)
{
	char *pSend = new char[str.GetLength()];
	memset(pSend, 0, str.GetLength()*sizeof(char));
	if (!WChar2MByte(str.GetBuffer(0), pSend, str.GetLength()))
	{
		AfxMessageBox(_T("�ַ�ת��ʧ��"));
		delete pSend;
		return;
	}
	POSITION nPos = m_clientList.GetHeadPosition();
	while (nPos)
	{
		CServerSocket* pTemp = (CServerSocket*)m_clientList.GetNext(nPos);
		pTemp->Send(pSend, str.GetLength());
	}
	delete pSend;
}

//���ƹ��ܿ����Լ�������ӣ�����ֻ�����ֲ���Ϊ������˵����ShellExecute��������
//��������Ӧ�ó��򣬹رս��̱Ƚ��鷳һ�㣬�����Ȼ�ȡӦ�ó��򴰿ڵ�ID��ͨ��
//OpenProcess��TerminateProcess��ֹ���̡�
void CServerDlg::ControlPC(CString AndroidControl)
{
	if (AndroidControl == "mop")             //�򿪲�����
	{
		ShellExecute(NULL, _T("open"), _T("C:\\Program Files (x86)\\KuGou\\KGMusic\\KuGou.exe"), NULL, NULL, SW_SHOWNORMAL);
	}
	else if(AndroidControl == "mcl")         //�رղ�����
	{
		DWORD id_num;
		HWND hWnd = ::FindWindow(_T("kugou_ui"), NULL);
		GetWindowThreadProcessId(hWnd, &id_num);                      
		//ע�⣺�ڶ��������ǽ��̵�ID������ֵ���̵߳�ID��
		HANDLE hd = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id_num);
		TerminateProcess(hd, 0);
	}
	else if(AndroidControl == "mpl" || AndroidControl == "mpa")  //����/��ͣ
	{
		keybd_event(VK_LMENU, 0, 0, 0);
		keybd_event(VK_F5, 0, 0, 0);
		keybd_event(VK_F5, 0, KEYEVENTF_KEYUP, 0);
		keybd_event(VK_LMENU, 0, KEYEVENTF_KEYUP, 0);
	}
}


//�ú������ڷ�ֹ����enter����escʱ�˳�����
BOOL CServerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch (pMsg->wParam)
	{
		case VK_RETURN:
		case VK_ESCAPE:
		return true; break;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
