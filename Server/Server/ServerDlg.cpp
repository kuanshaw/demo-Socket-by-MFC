
// ServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CServerDlg 对话框




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


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//本函数用于打开或关闭服务器，主要用到Create函数和Listen函数用于创建服务器和
//监听客户端。其中端口号从编辑框获取，应用程序的可用端口范围是1024-65535。
void CServerDlg::OnBnClickedStartserver()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_connect)
	{
		listenSocket = NULL;
		delete listenSocket;
		m_connect = false;
		SetDlgItemText(IDC_STARTSERVER, _T("打开服务器"));
		UpdateEvent(_T("系统关闭服务器."));
		return;
	}
	listenSocket = new CServerSocket();
	listenSocket->m_pDlg = this;

	// 指定对话框为主对话框，不能少了这句
	UpdateData(true);

	if(!listenSocket->Create(m_port))                     
	// 创建服务器的套接字，IP地址默认本机IP
	{
		AfxMessageBox(_T("创建套接字错误！"));
		listenSocket->Close();
		return;
	}
	if(!listenSocket->Listen())
	{
		AfxMessageBox(_T("监听失败！"));
		listenSocket->Close();
		return;
	}
	m_connect = true;
	SetDlgItemText(IDC_STARTSERVER, _T("关闭服务器"));
	UpdateEvent(_T("系统打开服务器."));
}

//本函数在CServerSocket类中的OnAccept消息中调用，用于响应用户连接服务器的请求，
//主要函数为Accept，当连接成功后，通过链表m_clientList保存新用户，更新日志，向
//新用户发送“Hello”表示欢迎。
void CServerDlg::AddClient()
{
    CServerSocket *pSocket = new CServerSocket;
    pSocket->m_pDlg = this;
    listenSocket->Accept(*pSocket);
    pSocket->AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE);
    m_clientList.AddTail(pSocket);
    m_userCount = m_clientList.GetCount();
    UpdateData(false);
    UpdateEvent(_T("用户连接服务器."));

    SendMSG(_T("Hello!YOKU~\r\n"));			///发送
}


//本函数在CServerSocket类中的OnClose消息中调用，用到POSITION结构，查找存储
//用户中哪位用户下线了，将下线用户释放，从链表中删除，并更新日志。
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
			UpdateEvent(_T("用户离开."));
			return;
		}
		nTmpPos = nPos;
	}
}


//本函数在CServerSocket类中的OnReceive消息中调用，用于处理接收到的数据并控制电脑，
//并将数据转发给所有用户（类似于群消息），通过CSocket类的GetPeerName函数可以获取
//用户的IP和端口号。
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
		ControlPC(str);     // 依据指令控制电脑
		SendMSG(str);     // 转发数据给所有用户，包括发送数据的用户
	}
	delete pData;
	pData = NULL;
}


//本函数在所有需要更新日志的地方都有调用，方便服务器记录用户的登录和退出事件。
void CServerDlg::UpdateEvent(CString str)
{
	CString string;
	CTime time = CTime::GetCurrentTime();                  
	// 获取系统当前时间
	str += _T("\r\n");                                                             
	// 用于换行显示日志
	string = time.Format(_T("%Y/%m/%d %H:%M:%S  ")) + str;         
	// 格式化当前时间
	int lastLine = m_event.LineIndex(m_event.GetLineCount() - 1);
	//获取编辑框最后一行索引
	m_event.SetSel(lastLine+1,lastLine+2,0);                                     
	//选择编辑框最后一行
	m_event.ReplaceSel(string); 
	//替换所选那一行的内容
}

//本函数在发送函数SendMSG中调用，用于字符集的转换，将宽字符转换为多字符集，
//不经转换的话，接收方只能接收一个字节。
BOOL CServerDlg::WChar2MByte(LPCWSTR srcBuff, LPSTR destBuff, int nlen)
{
	int n = 0;
	n = WideCharToMultiByte(CP_OEMCP, 0, srcBuff, -1, destBuff, 0, 0, FALSE);
	if (n<nlen)return FALSE;
	WideCharToMultiByte(CP_OEMCP, 0, srcBuff, -1, destBuff, nlen, 0, FALSE);
	return TRUE;
}

//发送函数，用于发送消息给所有用户，主要函数为Send，在AddClient和RecvData中
//都有调用，可以随时调用发消息给用户。
void CServerDlg::SendMSG(CString str)
{
	char *pSend = new char[str.GetLength()];
	memset(pSend, 0, str.GetLength()*sizeof(char));
	if (!WChar2MByte(str.GetBuffer(0), pSend, str.GetLength()))
	{
		AfxMessageBox(_T("字符转换失败"));
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

//控制功能可以自己随意添加，这里只以音乐播放为例进行说明，ShellExecute函数用于
//调用其他应用程序，关闭进程比较麻烦一点，这里先获取应用程序窗口的ID，通过
//OpenProcess和TerminateProcess终止进程。
void CServerDlg::ControlPC(CString AndroidControl)
{
	if (AndroidControl == "mop")             //打开播放器
	{
		ShellExecute(NULL, _T("open"), _T("C:\\Program Files (x86)\\KuGou\\KGMusic\\KuGou.exe"), NULL, NULL, SW_SHOWNORMAL);
	}
	else if(AndroidControl == "mcl")         //关闭播放器
	{
		DWORD id_num;
		HWND hWnd = ::FindWindow(_T("kugou_ui"), NULL);
		GetWindowThreadProcessId(hWnd, &id_num);                      
		//注意：第二个参数是进程的ID，返回值是线程的ID。
		HANDLE hd = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id_num);
		TerminateProcess(hd, 0);
	}
	else if(AndroidControl == "mpl" || AndroidControl == "mpa")  //播放/暂停
	{
		keybd_event(VK_LMENU, 0, 0, 0);
		keybd_event(VK_F5, 0, 0, 0);
		keybd_event(VK_F5, 0, KEYEVENTF_KEYUP, 0);
		keybd_event(VK_LMENU, 0, KEYEVENTF_KEYUP, 0);
	}
}


//该函数用于防止按下enter或者esc时退出程序。
BOOL CServerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (pMsg->wParam)
	{
		case VK_RETURN:
		case VK_ESCAPE:
		return true; break;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
