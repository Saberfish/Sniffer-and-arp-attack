
// Sniffer_TestDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Sniffer_Test.h"
#include "Sniffer_TestDlg.h"
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
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSnifferTestDlg 对话框



CSnifferTestDlg::CSnifferTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SNIFFER_TEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSnifferTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Wangka, CB_Wangka);
	DDX_Control(pDX, IDC_LIST1, List1);
	DDX_Control(pDX, IDC_EDIT1, packet_data);
	DDX_Control(pDX, IDC_TREE1, info_tree);
	DDX_Control(pDX, IDC_ProtocalType, CB_Filter);
	DDX_Control(pDX, IDC_IP, ip_address);
	DDX_Control(pDX, IDC_PORT, port_lala);
}

BEGIN_MESSAGE_MAP(CSnifferTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CSnifferTestDlg::OnTvnSelchangedTree1)
//	ON_CBN_SELENDCANCEL(IDC_Wangka, &CSnifferTestDlg::OnCbnSelendcancelWangka)
	ON_BN_CLICKED(B_Start, &CSnifferTestDlg::OnBnClickedStart)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CSnifferTestDlg::OnLvnItemchangedList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CSnifferTestDlg::OnNMClickList1)
	ON_CBN_SELCHANGE(IDC_Wangka, &CSnifferTestDlg::OnSelchangeWangka)
	ON_BN_CLICKED(B_Stop, &CSnifferTestDlg::OnClickedBStop)
	ON_CBN_SELCHANGE(IDC_ProtocalType, &CSnifferTestDlg::OnSelchangeProtocaltype)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CSnifferTestDlg::OnClickedButtonSave)
	ON_BN_CLICKED(IDC_ARP, &CSnifferTestDlg::OnClickedArp)
END_MESSAGE_MAP()


// CSnifferTestDlg 消息处理程序

BOOL CSnifferTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//获取所有网卡设备
	alldevs = myPcap.GetAllAdapter();
	for (d = alldevs; d; d = d->next) {
		CB_Wangka.InsertString(CB_Wangka.GetCount(), CString(d->description));
	}
	pcap_freealldevs(d);
	//设置ListCtrl
	List1.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	List1.InsertColumn(0, _T("Num"), LVCFMT_LEFT, 50);
	List1.InsertColumn(1, _T("Time"), LVCFMT_LEFT, 90);
	List1.InsertColumn(2, _T("Source"), LVCFMT_LEFT, 120);
	List1.InsertColumn(3, _T("Destination"), LVCFMT_LEFT, 120);
	List1.InsertColumn(4, _T("Protocol"), LVCFMT_LEFT, 80);
	List1.InsertColumn(5, _T("Length"), LVCFMT_LEFT, 90);
	List1.InsertColumn(6, _T("Info"), LVCFMT_LEFT, 300);
	//设置字体
	m_Font.CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_SWISS | DEFAULT_PITCH, _T("DejaVu Sans Mono"));
	packet_data.SetFont(&m_Font);
	info_tree.SetFont(&m_Font);
	List1.SetFont(&m_Font);
	ip_string = "";
	port_string = "";
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSnifferTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSnifferTestDlg::OnPaint()
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
HCURSOR CSnifferTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSnifferTestDlg::OnBnClickedButton1()//Button1改名了，叫start
{
	// TODO: 在此添加控件通知处理程序代码
}


void CSnifferTestDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


//void CSnifferTestDlg::OnCbnSelendcancelWangka()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}


void CSnifferTestDlg::OnCbnSelchangeWangka()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CSnifferTestDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	GetModuleFileName(0, FilePath, 255);
	CapFilePath = FilePath;
	CapFilePath = CapFilePath.Left(CapFilePath.ReverseFind('\\'));
	CapFilePath += "\\tmpData.CAP";//文档存储设置 
	int devnum = CB_Wangka.GetCurSel();
	if (CB_ERR == devnum)
	{
		AfxMessageBox(_T("Please select an adapter!"));
		return;
	}
	int i;
	//定位网络设备
	for (d = alldevs, i = 0; i<devnum; d = d->next, i++);
	//myPcap.devnow=d;
	//AfxMessageBox(CString(d->description));
	List1.DeleteAllItems();
	info_tree.DeleteAllItems();
	this->SetDlgItemText(IDC_EDIT1, CString(""));
	GetDlgItem(B_Start)->EnableWindow(FALSE);
	GetDlgItem(B_Stop)->EnableWindow(TRUE);
	GetDlgItem(B_Exit)->EnableWindow(FALSE);
	ip_address.GetWindowTextW(ip_string);
	port_lala.GetWindowTextW(port_string);
	// 	GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_CAPTURE_START, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	// 	GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_CAPTURE_STOP, MF_BYCOMMAND|MF_ENABLED);
	capStatus = TRUE;
	hCapThread = AfxBeginThread(CapThread, (LPVOID)d);

}
BOOL  CSnifferTestDlg::IsHttp(struct pcap_pkthdr *header, const u_char* pkt_data)
{
	// 	ip_header* ipheader=(ip_header*)(p+14);
	// 	u_short ipLen=ipheader->ihl*4;
	// 	tcp_header* tcpheader=(tcp_header*)(p+14+ipLen);
	// 	u_short tcpLen=tcpheader->offset*4;
	// 	
	// 	u_char* httppac=(u_char*)p+14+ipLen+tcpLen;
	// 	u_short httpLen=ntohs(ipheader->tlen)-ipLen-tcpLen;
	// 	char tmp[10];
	// 	CString res;
	// 	for(int i=0;i<httpLen;i++)
	// 	{
	// 		sprintf_s(tmp,10,"%c",httppac[i]);
	// 		res+=(CString)tmp;
	// 		if(i>2&&13==httppac[i-1]&&10==httppac[i])
	// 			break;
	// 	}
	// 	int httppos=res.Find(_T("HTTP"),0);
	// 	if(httppos!=-1&&httppos!=65535)
	// 		return TRUE;
	// 	else
	// 		return FALSE;
	ip_header* ipheader = (ip_header*)(pkt_data + 14);
	u_short ipLen = ipheader->ihl * 4;
	tcp_header* tcpheader = (tcp_header*)(pkt_data + 14 + ipLen);
	u_short tcpLen = tcpheader->offset * 4;

	u_char* httppac = (u_char*)pkt_data + 14 + ipLen + tcpLen;
	u_short httpLen = ntohs(ipheader->tlen) - ipLen - tcpLen;
	char s[64];
	CString data;
	char ipDataOut[65535] = { 0 };
	int end = 0;
	if (httpLen>0)
	{
		for (int i = 0; i <= httpLen; i++)
		{
			if (isgraph(httppac[i]))
				ipDataOut[end] = httppac[i];
			else if (isgraph(httppac[i]) == ' ')
				ipDataOut[end] = httppac[i];
			else ipDataOut[end] = '.';
			++end;
			if (i>2 && 13 == httppac[i - 1] && 10 == httppac[i])
				break;
		}
		ipDataOut[end] = '\0';
	}

	int httppos = (CString(ipDataOut)).Find(_T("HTTP"));
	if (httppos != -1 && httppos != 65536)
		return TRUE;
	else
		return FALSE;
}
UINT CapThread(LPVOID lpParameter)
{
	pcap_t *adhandle;
	pcap_if_t* devnow = (pcap_if_t*)lpParameter;
	char errbuf[PCAP_ERRBUF_SIZE + 1];
	int res;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	int num = 0;
	CString errstring;
	u_int netmask;
	struct bpf_program fcode;

	if ((adhandle = pcap_open(devnow->name,          // 设备名
		65536,            // 65536保证能捕获到不同数据链路层上的每个数据包的全部内容
		PCAP_OPENFLAG_PROMISCUOUS,    // 混杂模式
		1000,             // 读取超时时间
		NULL,             // 远程机器验证
		errbuf            // 错误缓冲池
	)) == NULL)
	{
		errstring.Format(_T("Unable to open the adapter. %s is not supported by WinPcap"), CString(devnow->name));
		AfxMessageBox(errstring);
		pcap_freealldevs(devnow);
		//GetDlgItem(IDC_BUTTON1);
		return -1;
	}

	if (devnow->addresses != NULL)
		/* 获取接口第一个地址的掩码 */
		netmask = ((struct sockaddr_in *)(devnow->addresses->netmask))->sin_addr.S_un.S_addr;

	else
		/* 如果这个接口没有地址，那么我们假设这个接口在C类网络中 */
		netmask = 0xffffff;
	/*char* packet_filter=CFilterDlg::UnicodeToANSI(filterstr.GetBuffer());*/
	CString http_str(_T("http"));
	if (filterstr.Compare(http_str) == 0) {
		filterstr.Empty();
		filterstr=CString("ip and tcp and port 80");
	}
	else {
		if (!ip_string.IsEmpty()) {
			if (!filterstr.IsEmpty()) {
				filterstr = filterstr + " " + "and src " + ip_string;
			}
			else {
				filterstr = "src ";
				filterstr += ip_string;
			}
		}
		ip_string.Empty();
		if (!port_string.IsEmpty()) {
			if (!filterstr.IsEmpty()) {
				filterstr = filterstr + " and port " + port_string;
			}
			else {
				filterstr = "port ";
				filterstr += port_string;
			}
		}
		port_string.Empty();
	}
	if (pcap_compile(adhandle, &fcode, CStringA(filterstr.GetBuffer()), 1, netmask) <0)
	{
		errstring = CString("Unable to compile the packet filter. Check the syntax.");
		AfxMessageBox(errstring);
		/* 释放设备列表 */
		pcap_freealldevs(devnow);
		return -1;
	}
	filterstr.Empty();
	if (pcap_setfilter(adhandle, &fcode) < 0)
	{
		errstring = CString("Unable to set the filter.Please ensure the expression is correct");
		AfxMessageBox(errstring);
		pcap_freealldevs(devnow);
		return -1;
	}

	pcap_dumper_t* dumpfile;
	dumpfile = pcap_dump_open(adhandle, CStringA(CapFilePath.GetBuffer()));
	if (NULL == dumpfile)
	{
		AfxMessageBox(_T("Can't open the dump file!"));
		return -1;
	}
	while ((res = pcap_next_ex(adhandle, &header, &pkt_data)) >= 0)
	{
		if (res == 0)
			/* 超时时间到 */
			continue;
		//_itoa(packetNum,list.num,10);
		//sprintf(list.time,timestr);

		//PostMessage(hmainDialog,WM_PACKET_IN,NULL,(LPARAM)pkt);
		/*SendMessageTimeout(hmainDialog,WM_UPDATE_LIST,(WPARAM)&list,0,SMTO_BLOCK,1000,&res);*/
		++num;
		time_t local_tv_sec;
		struct tm *ltime;
		char timestr[16];

		/* 将时间戳转换成可识别的格式 */
		local_tv_sec = header->ts.tv_sec;
		ltime = localtime(&local_tv_sec);
		strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);
		char temp[50];
		_itoa(num, temp, 10);
		/*处理以太网首部*/
		ethernet_header *ethheader = (ethernet_header*)pkt_data;
		char protocaltype[10];
		CSnifferTestDlg::GetEthernetType(ethheader, protocaltype);

		TCHAR srcAddr[18];
		TCHAR desAddr[18];
		/*处理IPv4协议的类型*/
		if (CString(protocaltype) == CString("IPv4"))
		{
			/* 处理IP首部*/
			ip_header *ipheader = (ip_header *)(pkt_data + 14);
			swprintf_s(srcAddr, 16, _T("%d.%d.%d.%d"), ipheader->saddr.byte1, ipheader->saddr.byte2, ipheader->saddr.byte3, ipheader->saddr.byte4);
			swprintf_s(desAddr, 16, _T("%d.%d.%d.%d"), ipheader->daddr.byte1, ipheader->daddr.byte2, ipheader->daddr.byte3, ipheader->daddr.byte4);
			CSnifferTestDlg::GetIPv4Type(ipheader, protocaltype);
		}
		if (CString(protocaltype) == CString("ARP"))
		{
			u_char* tmpSrc = ethheader->srcmac;
			u_char* tmpDst = ethheader->dstmac;
			swprintf_s(srcAddr, 18, _T("%02X:%02X:%02X:%02X:%02X:%02X"), tmpSrc[0], tmpSrc[1], tmpSrc[2], tmpSrc[3], tmpSrc[4], tmpSrc[5]);
			swprintf_s(desAddr, 18, _T("%02X:%02X:%02X:%02X:%02X:%02X"), tmpDst[0], tmpDst[1], tmpDst[2], tmpDst[3], tmpDst[4], tmpDst[5]);
		}
		/*确认为TCP包过后进一步判断协议类型*/
		 		if(CString(protocaltype)==CString("TCP"))
		 		{
		 			if(CSnifferTestDlg::IsHttp(header,pkt_data))
		 				strcpy_s(protocaltype,10,"HTTP");
		 		}
		/*处理包长度*/
		char lenstr[10];
		_itoa(header->len, lenstr, 10);
		CSnifferTestDlg* 	mDlg = ((CSnifferTestDlg*)(AfxGetApp()->GetMainWnd()));
		int i = mDlg->List1.InsertItem(mDlg->List1.GetItemCount(), CString(temp));
		//m_List.SetItemText(i,0,CString(list->num));

		mDlg->List1.SetTextBkColor(0xFFE070);
		mDlg->List1.SetItemText(i, 0, CString(temp));
		mDlg->List1.SetItemText(i, 1, CString(timestr));
		mDlg->List1.SetItemText(i, 2, CString(srcAddr));
		mDlg->List1.SetItemText(i, 3, CString(desAddr));
		mDlg->List1.SetItemText(i, 4, CString(protocaltype));
		mDlg->List1.SetItemText(i, 5, CString(lenstr));
		//EnterCriticalSection(&CapThreadCS);
		// 		if((CString)protocaltype==CString("ARP"))
		// 			++arpnum;
		// 		if((CString)protocaltype==CString("TCP"))
		// 		{
		// 			tcpnum++;
		// 			CFile mFile;
		// 			mFile.Open(_T("num.dat"),CFile::modeWrite|CFile::modeCreate);
		// 			CArchive ar(&mFile,CArchive::store);
		// 			ar<<tcpnum;
		// 		}
		// 		if((CString)protocaltype==CString("UDP"))
		// 			++udpnum;
		// 		if((CString)protocaltype==CString("ICMP"))
		// 			++icmpnum;
		// 		if((CString)protocaltype==CString("IGMP"))
		// 			++igmpnum;
		//LeaveCriticalSection(&CapThreadCS);
		CMyPcap::SavePacket(header, pkt_data, dumpfile);
	}

	if (res == -1) {
		CString errstr;
		errstr.Format(_T("Error reading the packets: %s\n"), CString(pcap_geterr(adhandle)));
		AfxMessageBox(errstr);
		return -1;
	}

	return 0;
}
void CSnifferTestDlg::GetEthernetType(ethernet_header * e, char *typestr)
{
	u_short etype = ntohs(e->eth_type);
	switch (etype)
	{
	case XNS_IDP:
		strcpy_s(typestr, 10, ("XNS IDP"));
		break;
	case DLOG:
		strcpy_s(typestr, 10, ("DLOG"));
		break;
	case IP:
		strcpy_s(typestr, 10, ("IPv4"));
		break;
	case X75:
		strcpy_s(typestr, 10, ("X.75"));
		break;
	case NBS:
		strcpy_s(typestr, 10, ("NBS"));
		break;
	case ECMA:
		strcpy_s(typestr, 10, ("ECMA"));
		break;
	case Chaosnet:
		strcpy_s(typestr, 10, ("Chaosnet"));
		break;
	case X25L3:
		strcpy_s(typestr, 10, ("X25 L3"));
		break;
	case ARP:
		strcpy_s(typestr, 10, ("ARP"));
		break;
	case FARP:
		strcpy_s(typestr, 10, ("FARP"));
		break;
	case RFR:
		strcpy_s(typestr, 10, ("RFR"));
		break;
	case RARP:
		strcpy_s(typestr, 10, ("RARP"));
		break;
	case NNIPX:
		strcpy_s(typestr, 10, ("NNIPX"));
		break;
	case EtherTalk:
		strcpy_s(typestr, 10, ("EtherTalk"));
		break;
	case ISSE:
		strcpy_s(typestr, 10, ("ISSE"));
		break;
	case AARP:
		strcpy_s(typestr, 10, ("AAPR"));
		break;
	case EAPS:
		strcpy_s(typestr, 10, ("EAPS"));
		break;
	case IPX:
		strcpy_s(typestr, 10, ("IPX"));
		break;
	case SNMP:
		strcpy_s(typestr, 10, ("SNMP"));
		break;
	case IPv6:
		strcpy_s(typestr, 10, ("IPv6"));
		break;
	case OAM:
		strcpy_s(typestr, 10, ("OAM"));
		break;
	case PPP:
		strcpy_s(typestr, 10, ("PPP"));
		break;
	case GSMP:
		strcpy_s(typestr, 10, ("GSMP"));
		break;
	case MPLSu:
		strcpy_s(typestr, 10, ("MPLS"));
		break;
	case MPLSm:
		strcpy_s(typestr, 10, ("MPLS"));
		break;
	case PPPoEds:
		strcpy_s(typestr, 10, ("PPPoE"));
		break;
	case PPPoEss:
		strcpy_s(typestr, 10, ("PPPoE"));
		break;
	case LWAPP:
		strcpy_s(typestr, 10, ("LWAPP"));
		break;
	case LLDP:
		strcpy_s(typestr, 10, ("LLDP"));
		break;
	case EAP:
		strcpy_s(typestr, 10, ("EAP"));
		break;
	case LOOPBACK:
		strcpy_s(typestr, 10, ("LOOPBACK"));
		break;
	case VLAN:
		strcpy_s(typestr, 10, ("VLAN"));
		break;
	default:
		strcpy_s(typestr, 10, ("UNKNOW"));
		break;
	}
}
void CSnifferTestDlg::GetIPv4Type(ip_header* ih, char* pt)
{
	u_short iptype = ih->proto;
	switch (iptype)
	{
	case ICMP:
		strcpy_s(pt, 10, "ICMP");
		break;
	case IGMP:
		strcpy_s(pt, 10, "IGMP");
		break;
	case TCP:
		strcpy_s(pt, 10, "TCP");
		break;
	case UDP:
		strcpy_s(pt, 10, "UDP");
		break;
	case OSPF:
		strcpy_s(pt, 10, "OSPF");
		break;
	default:
		strcpy_s(pt, 10, "UNKNOW IP");
	}
}
void CSnifferTestDlg::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult) {
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// 	struct pcap_pkthdr *header;
	// 	const u_char *pkt_data;
	if (pNMListView->iItem != -1)
	{
		int ItemClick = pNMListView->iItem;
		AfxBeginThread(ReadDumpThread, (LPVOID)ItemClick);
	}
	*pResult = 0;
}
UINT ReadDumpThread(LPVOID lpParameter)
{/*这个线程用于从临时文件中读取选择的包并将其分析生成分析树*/
	int packetFind = (int)lpParameter;
	pcap_t *adhandle;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	char errbuff[PCAP_ERRBUF_SIZE + 1];
	//char* show=new char[];
	CString data;
	//CString tmp;
	CSnifferTestDlg* mDlg = ((CSnifferTestDlg*)(AfxGetApp()->GetMainWnd()));
	if (NULL == (adhandle = pcap_open_offline(CStringA(CapFilePath.GetBuffer()), errbuff)))
	{
		CString errstr;
		errstr.Format(_T("Open dump file error,error code:s%"), errbuff);
		AfxMessageBox(errstr);
		return -1;
	}
	for (int i = 0; i <= packetFind; i++)
	{
		pcap_next_ex(adhandle, &header, &pkt_data);
	}
	data = CSnifferTestDlg::PackToEdit(header, pkt_data);
	/*处理以太网首部*/
	ethernet_header *ethheader = (ethernet_header*)pkt_data;
	char type[10];//以太网包类型
	CSnifferTestDlg::GetEthernetType(ethheader, type);
	// 获取MAC地址
	TCHAR eth_src[18];
	TCHAR eth_dst[18];
	u_char* tmpSrc;
	u_char* tmpDst;
	tmpSrc = ethheader->srcmac;
	tmpDst = ethheader->dstmac;
	swprintf_s(eth_src, 18, _T("%02X:%02X:%02X:%02X:%02X:%02X"), tmpSrc[0], tmpSrc[1], tmpSrc[2], tmpSrc[3], tmpSrc[4], tmpSrc[5]);
	swprintf_s(eth_dst, 18, _T("%02X:%02X:%02X:%02X:%02X:%02X"), tmpDst[0], tmpDst[1], tmpDst[2], tmpDst[3], tmpDst[4], tmpDst[5]);
	mDlg->SetDlgItemText(IDC_EDIT1, data);
	mDlg->info_tree.DeleteAllItems();
	char Frame[1024];
	CString str;
	//Frame
	sprintf_s(Frame, 1024, "Frame %d: %u bytes on wire (%u bits)", (packetFind + 1), header->len, (header->len) * 8);
	HTREEITEM HFrame = mDlg->info_tree.InsertItem(CString(Frame));
	//Ethernet
	str.Format(_T("Ethernet,Src:  %s,Dst:  %s"), eth_src, eth_dst);
	HTREEITEM HEthernet = mDlg->info_tree.InsertItem(str);

	//Ethernet Source
	str.Format(_T("Source:  %s"), eth_src);
	mDlg->info_tree.InsertItem(str, HEthernet, TVI_LAST);

	//Ethernet Destination
	str.Format(_T("Destination:  %s"), eth_dst);
	mDlg->info_tree.InsertItem(str, HEthernet, TVI_LAST);
	//Ethernet Type
	str = CString("Type:  ") + CString(type);
	mDlg->info_tree.InsertItem(str, HEthernet, TVI_LAST);
	if ((CString)type == CString("IPv4"))
	{
		ip_header* ipheader = (ip_header*)(pkt_data + 14);
		u_short ipLen = ipheader->ihl * 4;
		CSnifferTestDlg::ShowIPInfo(pkt_data, mDlg);
		CSnifferTestDlg::GetIPv4Type(ipheader, type);
		if ((CString)type == CString("UDP"))
		{
			udp_header* udpheader = (udp_header*)(pkt_data + 14 + ipLen);
			CSnifferTestDlg::ShowUDPInfo(udpheader, mDlg);
		}
		if ((CString)type == CString("ICMP"))
		{
			icmp_header* icmpheader = (icmp_header*)(pkt_data + 14 + ipLen);
			CSnifferTestDlg::ShowICMPInfo(icmpheader, mDlg);
		}
		if ((CString)type == CString("IGMP"))
		{
			igmp_header* igmpheader = (igmp_header*)(pkt_data + 14 + ipLen);
			CSnifferTestDlg::ShowIGMPInfo(igmpheader, mDlg);
		}
		if ((CString)type == CString("TCP"))
		{
			tcp_header* tcpheader = (tcp_header*)(pkt_data + 14 + ipLen);
			u_short tcpdataLen = header->len - 14 - ipLen - tcpheader->offset * 4;
			CSnifferTestDlg::ShowTCPInfo(tcpheader, mDlg, tcpdataLen);
			if ((tcpheader->sport == 80) || (tcpheader->dport = 80)) {
				CString str;
				//int a =
				//CString content = data.Left(a);
				/*for (int i = 0; i < 4; i++) {
					char neirong[100];
					int j = 0;
					while ((pkt_data+54)[j] != '\r') {
						neirong[j] = (pkt_data+54)[j];
						j++;
					}
					neirong[j] = '\r';
					neirong[j + 1] = '\n';
					neirong[j + 2] = '\0';
					str.Format(_T("Content: %s"), neirong);
					mDlg->info_tree.InsertItem(str);
				}*/
				//http_packet* http_bao = (http_packet*)(pkt_data + 54);
				TCHAR temp_content[430];
				TCHAR* p_temp = temp_content;
				for (int i = 0; i < 300; i++) {
					temp_content[i] = (pkt_data+54)[i];
					if (temp_content[i] == '\r') {
						temp_content[i] = '\0';
						str.Format(_T("%s"), p_temp);
						mDlg->info_tree.InsertItem(str);
						p_temp += i + 1;
					}
				}
				
			}
		}
	}
	if ((CString)type == CString("ARP"))
	{
		arp_header* arpheader = (arp_header*)(pkt_data + 14);
		CSnifferTestDlg::ShowArpInfo(arpheader, mDlg);
	}
	return 0;
}
CString CSnifferTestDlg::PackToEdit(struct pcap_pkthdr* header, const u_char* pkt_data)
{
	CString data;
	char s[64];
	int ipDataLength = header->len;
	char ipDataOut[65535] = { 0 };
	int end = 0;
	if (ipDataLength>0)
	{
		for (int i = 0; i <= ipDataLength; i++)
		{
			//printf("%02x",pkt_data[i]);
			//tmp.Format(_T(" %02x"),pkt_data[i]);
			sprintf_s(s, 64, " %02X", pkt_data[i]);
			data += (CString)s;
			if (isgraph(pkt_data[i]))
				ipDataOut[end] = pkt_data[i];
			else if (isgraph(pkt_data[i]) == ' ')
				ipDataOut[end] = pkt_data[i];
			else ipDataOut[end] = '.';
			end = end + 1;

			if (i % 16 == 15)
			{
				ipDataOut[end] = '\0';
				/*printf("  %s",ipDataOut);*/
				//sprintf(show,"  %s")
				//tmp.Format(_T("  %s"),ipDataOut);
				sprintf_s(s, 64, " %s", ipDataOut);
				data += (CString)s;
				end = 0;
				//printf("\n");
				data += CString("\r\n");
			}
		}

		if (end>0)
		{
			for (int k = end * 3; k<48; k++)
			{
				//printf("");
				data += CString(" ");
			}
			ipDataOut[end] = 0;
			// 			printf("  %s",ipDataOut);
			// 			printf("\n");
			//tmp.Format(("  %s"),ipDataOut);
			sprintf_s(s, 64, " %s", ipDataOut);
			data += (CString)s;
			data += CString("\r\n");
		}
	}
	return data;
}

void CSnifferTestDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if (capStatus)
	{
		AfxMessageBox(_T("Please stop capture first!"));
		return;
	}
	//DeleteCriticalSection(&CapThreadCS);
	pcap_freealldevs(alldevs);
	CDialog::OnClose();
}
void CSnifferTestDlg::ShowIPInfo(const u_char* pkt_data, CSnifferTestDlg* mDlg)
{
	CString str;
	char iptype[10];
	ip_header *ipheader = (ip_header *)(pkt_data + 14);
	TCHAR ipsrcAddr[16];
	TCHAR ipdesAddr[16];
	swprintf_s(ipsrcAddr, 16, _T("%d.%d.%d.%d"), ipheader->saddr.byte1, ipheader->saddr.byte2, ipheader->saddr.byte3, ipheader->saddr.byte4);
	swprintf_s(ipdesAddr, 16, _T("%d.%d.%d.%d"), ipheader->daddr.byte1, ipheader->daddr.byte2, ipheader->daddr.byte3, ipheader->daddr.byte4);
	CSnifferTestDlg::GetIPv4Type(ipheader, iptype);
	//IP
	str.Format(_T("Internet Protocol Version 4,Src:  %s,Dst:  %s"), ipsrcAddr, ipdesAddr);
	HTREEITEM HIPv4 = mDlg->info_tree.InsertItem(str);
	//Version
	str = CString("Version:  4");
	mDlg->info_tree.InsertItem(str, HIPv4, TVI_LAST);
	//Header Length
	u_short iphLen = ipheader->ihl * 4;
	str.Format(_T("Header length:  %u bytes"), iphLen);
	mDlg->info_tree.InsertItem(str, HIPv4, TVI_LAST);
	//Services
	int dscp = ((ipheader->tos)&(0xEC)) >> 2;
	int ecn = (ipheader->tos)&(0x03);
	str.Format(_T("Differentiated Services Field: 0x%02x (DSCP 0x%02x; ECN: 0x%02x)"), ipheader->tos, dscp, ecn);
	HTREEITEM HSer = mDlg->info_tree.InsertItem(str, HIPv4, TVI_LAST);
	//Ser Dscp Ecn
	char tosb[8];
	CSnifferTestDlg::DecToBinary(int(ipheader->tos), tosb);
	str.Format(_T("Differentiated Services Codepoint: %c%c%c%c %c%c.."), tosb[0], tosb[1], tosb[2], tosb[3], tosb[4], tosb[5]);
	mDlg->info_tree.InsertItem(str, HSer, TVI_LAST);
	str.Format(_T("Explicit Congestion Notification:  .... ..%c%c"), tosb[6], tosb[7]);
	mDlg->info_tree.InsertItem(str, HSer, TVI_LAST);
	//Total Length
	str.Format(_T("Total Length: %d"), ntohs(ipheader->tlen));
	mDlg->info_tree.InsertItem(str, HIPv4, TVI_LAST);
	//Identification
	str.Format(_T("Identification: %04x (%d)"), ntohs(ipheader->identification), ntohs(ipheader->identification));
	mDlg->info_tree.InsertItem(str, HIPv4, TVI_LAST);
	//Flags
	str.Format(_T("Flags: 0x%02x"), ipheader->flags);
	HTREEITEM HFlag = mDlg->info_tree.InsertItem(str, HIPv4, TVI_LAST);
	//Flags Detail
	char flagsb[8];
	CSnifferTestDlg::DecToBinary((int)(ipheader->flags), flagsb);
	str.Format(_T("%c.. = Reserved bit: %s"), flagsb[5], (flagsb[5]>'0') ? _T("Set") : _T("Not Set"));
	mDlg->info_tree.InsertItem(str, HFlag, TVI_LAST);
	str.Format(_T(".%c. = Don't fragment: %s"), flagsb[6], (flagsb[6]>'0') ? _T("Set") : _T("Not Set"));
	mDlg->info_tree.InsertItem(str, HFlag, TVI_LAST);
	str.Format(_T("..%c = More fragments: %s"), flagsb[7], (flagsb[6]>'0') ? _T("Set") : _T("Not Set"));
	mDlg->info_tree.InsertItem(str, HFlag, TVI_LAST);
	//Flag Offset
	str.Format(_T("Fragment offset: %d"), ipheader->fo);
	mDlg->info_tree.InsertItem(str, HIPv4, TVI_LAST);
	//Time to Live
	str.Format(_T("Time to live: %d"), ipheader->ttl);
	mDlg->info_tree.InsertItem(str, HIPv4, TVI_LAST);
	//Protocol
	char protocol[10];
	CSnifferTestDlg::GetIPv4Type(ipheader, protocol);
	str = CString("Protocol: ") + CString(protocol);
	mDlg->info_tree.InsertItem(str, HIPv4, TVI_LAST);
	//Check Sum
	str.Format(_T("Header checksum: 0x%04x"), ntohs(ipheader->crc));
	mDlg->info_tree.InsertItem(str, HIPv4, TVI_LAST);
	//Source
	str.Format(_T("Source: %s"), ipsrcAddr);
	mDlg->info_tree.InsertItem(str, HIPv4, TVI_LAST);
	//Destination
	str.Format(_T("Destination: %s"), ipdesAddr);
}
void CSnifferTestDlg::DecToBinary(int n, char* ch)
{
	for (int i = 0; i<8; i++)
	{
		if ((n & 0x80) == 0x80)
			ch[i] = '1';
		else
			ch[i] = '0';
		n << 1;
	}
}
void CSnifferTestDlg::ShowArpInfo(arp_header* arpheader, CSnifferTestDlg* mDlg)
{
	CString str;
	str.Format(_T("Address Resolution Protocol (%s)"), (1 == ntohs(arpheader->opcode)) ? _T("request") : _T("reply"));
	HTREEITEM HArp = mDlg->info_tree.InsertItem(str);
	//Hardware Type
	switch (ntohs(arpheader->hrd))
	{
	case 1:
		str = CString("Hardware Type: Ethernet (1)");
		break;
	case 6:
		str = CString("Hardware Type: Token Ring (6)");
		break;
	default:
		str.Format(_T("Hardware Type: Unknow (%d)"), ntohs(arpheader->eth_type));
	}
	mDlg->info_tree.InsertItem(str, HArp, TVI_LAST);
	//Prptocol Type
	if (2048 == ntohs(arpheader->eth_type))
		str = CString("Protocol type: IP (0x0800)");
	else
		str = CString("Protocol type: Unknow");
	mDlg->info_tree.InsertItem(str, HArp, TVI_LAST);
	//Hardware Size
	str.Format(_T("Hardware size: %d"), arpheader->maclen);
	mDlg->info_tree.InsertItem(str, HArp, TVI_LAST);
	//Protocol Size
	str.Format(_T("Protocol size: %d"), arpheader->iplen);
	mDlg->info_tree.InsertItem(str, HArp, TVI_LAST);
	//Opcode
	str.Format(_T("Opcode: %s"), (1 == ntohs(arpheader->opcode)) ? _T("request (1)") : _T("reply (2)"));
	mDlg->info_tree.InsertItem(str, HArp, TVI_LAST);
	//Sender Mac
	u_char* tmpSrc = arpheader->smac;
	str.Format(_T("Sender MAC address: %02X:%02X:%02X:%02X:%02X:%02X"), tmpSrc[0], tmpSrc[1], tmpSrc[2], tmpSrc[3], tmpSrc[4], tmpSrc[5]);
	mDlg->info_tree.InsertItem(str, HArp, TVI_LAST);
	//Sender IP
	str.Format(_T("Sender IP address: %d.%d.%d.%d"), arpheader->saddr.byte1, arpheader->saddr.byte2, arpheader->saddr.byte3, arpheader->saddr.byte4);
	mDlg->info_tree.InsertItem(str, HArp, TVI_LAST);
	//Target Mac
	u_char* tmpDst = arpheader->dmac;
	str.Format(_T("Target MAC address: %02X:%02X:%02X:%02X:%02X:%02X"), tmpDst[0], tmpDst[1], tmpDst[2], tmpDst[3], tmpDst[4], tmpDst[5]);
	mDlg->info_tree.InsertItem(str, HArp, TVI_LAST);
	//Target IP
	str.Format(_T("Target IP address: %d.%d.%d.%d"), arpheader->daddr.byte1, arpheader->daddr.byte2, arpheader->daddr.byte3, arpheader->daddr.byte4);
	mDlg->info_tree.InsertItem(str, HArp, TVI_LAST);
}

void CSnifferTestDlg::ShowUDPInfo(udp_header* udpheader, CSnifferTestDlg* mDlg)
{
	CString str;
	str.Format(_T("User Datagram Protocol, Src Port: %d, Dst Port: %d"), ntohs(udpheader->sport), ntohs(udpheader->dport));
	HTREEITEM HUDP = mDlg->info_tree.InsertItem(str);
	//Source Port
	str.Format(_T("Source port: %d"), ntohs(udpheader->sport));
	mDlg->info_tree.InsertItem(str, HUDP, TVI_LAST);
	//Destination Port
	str.Format(_T("Destination port: %d"), ntohs(udpheader->dport));
	mDlg->info_tree.InsertItem(str, HUDP, TVI_LAST);
	//Length
	str.Format(_T("Length: %d"), ntohs(udpheader->len));
	mDlg->info_tree.InsertItem(str, HUDP, TVI_LAST);
	//CheckSum
	str.Format(_T("CheckSum: 0x%04x"), ntohs(udpheader->crc));
	mDlg->info_tree.InsertItem(str, HUDP, TVI_LAST);
	u_char* data = (u_char*)(udpheader + udpheader->len);
	//Data
	u_short datasize = ntohs(udpheader->len) - 8;
	str.Format(_T("Data (%d bytes)"), datasize);
	mDlg->info_tree.InsertItem(str, HUDP, TVI_LAST);
}

void CSnifferTestDlg::ShowICMPInfo(icmp_header* icmpheader, CSnifferTestDlg* mDlg)
{
	CString str;
	str = CString("Internet Control Message Protocol");
	HTREEITEM HICMP = mDlg->info_tree.InsertItem(str);
	//Type
	TCHAR Type[64];
	HTREEITEM HIC_Type;
	switch (icmpheader->type)
	{
	case ICMP4_ECHO_REPLY:
		wcscpy_s(Type, 64, _T("Echo (ping) reply"));
		str.Format(_T("Type: %d (%s)"), icmpheader->type, Type);
		HIC_Type = mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		str.Format(_T("Identifier (Big Endian): %d"), ntohs(icmpheader->id));
		mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		str.Format(_T("Identifier (Little Endian): %d"), icmpheader->id);
		mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		str.Format(_T("Sequence number (Big Endian): %d"), ntohs(icmpheader->seq));
		mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		str.Format(_T("Sequence number (Little Endian): %d"), icmpheader->seq);
		mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		break;
	case ICMP4_ECHO_REQUEST:
		wcscpy_s(Type, 64, _T("Echo (ping) request"));
		str.Format(_T("Type: %d (%s)"), icmpheader->type, Type);
		HIC_Type = mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		str.Format(_T("Identifier (Big Endian): %d"), ntohs(icmpheader->id));
		mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		str.Format(_T("Identifier (Little Endian): %d"), icmpheader->id);
		mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		str.Format(_T("Sequence number (Big Endian): %d"), ntohs(icmpheader->seq));
		mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		str.Format(_T("Sequence number (Little Endian): %d"), icmpheader->seq);
		mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		break;
	case ICMP4_MASK_REPLY:
		wcscpy_s(Type, 64, _T("Mask reply"));
		str.Format(_T("Type: %d (%s)"), icmpheader->type, Type);
		HIC_Type = mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		break;
	case ICMP4_MASK_REQUEST:
		wcscpy_s(Type, 64, _T("Mask request"));
		str.Format(_T("Type: %d (%s)"), icmpheader->type, Type);
		HIC_Type = mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		break;
	case ICMP4_ROUTER_SOLICIT:
		wcscpy_s(Type, 64, _T("Router solicit"));
		str.Format(_T("Type: %d (%s)"), icmpheader->type, Type);
		HIC_Type = mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		break;
	case ICMP4_DST_UNREACH:
		wcscpy_s(Type, 64, _T("Dest unreach"));
		str.Format(_T("Type: %d (%s)"), icmpheader->type, Type);
		HIC_Type = mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		break;
	case ICMP4_SOURCE_QUENCH:
		wcscpy_s(Type, 64, _T("Source quench"));
		str.Format(_T("Type: %d (%s)"), icmpheader->type, Type);
		HIC_Type = mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		break;
	case ICMP4_REDIRECT:
		wcscpy_s(Type, 64, _T("Redirect"));
		str.Format(_T("Type: %d (%s)"), icmpheader->type, Type);
		HIC_Type = mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		break;
	case ICMP4_ROUTER_ADVERT:
		wcscpy_s(Type, 64, _T("Router Advert"));
		str.Format(_T("Type: %d (%s)"), icmpheader->type, Type);
		HIC_Type = mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		break;
	case ICMP4_TIME_EXCEEDED:
		wcscpy_s(Type, 64, _T("Time exceeded"));
		str.Format(_T("Type: %d (%s)"), icmpheader->type, Type);
		HIC_Type = mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		break;
	case ICMP4_PARAM_PROB:
		wcscpy_s(Type, 64, _T("Param prob"));
		str.Format(_T("Type: %d (%s)"), icmpheader->type, Type);
		HIC_Type = mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		break;
	case ICMP4_TIMESTAMP_REQUEST:
		wcscpy_s(Type, 64, _T("Timestamp request"));
		str.Format(_T("Type: %d (%s)"), icmpheader->type, Type);
		HIC_Type = mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		break;
	case ICMP4_TIMESTAMP_REPLY:
		wcscpy_s(Type, 64, _T("Timestamp reply"));
		str.Format(_T("Type: %d (%s)"), icmpheader->type, Type);
		HIC_Type = mDlg->info_tree.InsertItem(str, HICMP, TVI_LAST);
		break;

	}
	//Code
	str.Format(_T("Code: %d"), icmpheader->code);
	HTREEITEM HIC_Code = mDlg->info_tree.InsertItem(str, HICMP, HIC_Type);
	//CheckSum
	str.Format(_T("Checksum:0x%04x"), ntohs(icmpheader->chk_sum));
	mDlg->info_tree.InsertItem(str, HICMP, HIC_Code);
}

void CSnifferTestDlg::ShowIGMPInfo(igmp_header* igmpheader, CSnifferTestDlg* mDlg)
{
	CString str;
	str = CString("Internet Group Management Protocol");
	HTREEITEM HIGMP = mDlg->info_tree.InsertItem(str);
	//Type
	switch (igmpheader->type)
	{
	case 0x11:
		str.Format(_T("Type:Membership Query (0x%02x)"), igmpheader->type);
		break;
	case 0x16:
		str.Format(_T("Type:Membership Report (0x%02x)"), igmpheader->type);
		break;
	case 0x17:
		str.Format(_T("Type:Leave Group (0x%02x)"), igmpheader->type);
		break;
	}
	mDlg->info_tree.InsertItem(str, HIGMP, TVI_LAST);
	//Max Response Time
	str.Format(_T("Max response time: %d sec (0x%02x)"), (igmpheader->mrtime) / 10.0, igmpheader->mrtime);
	mDlg->info_tree.InsertItem(str, HIGMP, TVI_LAST);
	//Checksum
	str.Format(_T("Header Checksum: 0x%04x"), igmpheader->chk_sum);
	mDlg->info_tree.InsertItem(str, HIGMP, TVI_LAST);
	//Multicast add
	str.Format(_T("Multicast Address: %d.%d.%d.%d"), (igmpheader->mcadd & 0xff000000) >> 24, (igmpheader->mcadd & 0x00ff0000) >> 16, (igmpheader->mcadd & 0x0000ff00) >> 8, (igmpheader->mcadd & 0x000000ff));
	mDlg->info_tree.InsertItem(str, HIGMP, TVI_LAST);
}

void CSnifferTestDlg::ShowTCPInfo(tcp_header* tcpheader, CSnifferTestDlg* mDlg, u_short tcpdataLen)
{
	CString str;
	str.Format(_T("Transmission Control Protocol, Src Port: %d,Dst Port: %d"), ntohs(tcpheader->sport), ntohs(tcpheader->dport));
	HTREEITEM HTCP = mDlg->info_tree.InsertItem(str);
	//Source Port
	str.Format(_T("Source port: %d"), ntohs(tcpheader->sport));
	mDlg->info_tree.InsertItem(str, HTCP, TVI_LAST);
	//Des Port
	str.Format(_T("Destination port: %d"), ntohs(tcpheader->dport));
	mDlg->info_tree.InsertItem(str, HTCP, TVI_LAST);
	//Seq num
	str.Format(_T("Sequence number: %u"), ntohl(tcpheader->seqno));
	mDlg->info_tree.InsertItem(str, HTCP, TVI_LAST);
	//Ack num

	u_char* tcpdata = (u_char*)tcpheader;
	if (1 == tcpheader->flag & 0x10)
	{
		str.Format(_T("Acknowledgement number: %u"), ntohl(tcpheader->ackno));
	}
	else
	{
		str = CString("Acknowledgement number: Acknowledgement Flag not set");
	}
	mDlg->info_tree.InsertItem(str, HTCP, TVI_LAST);
	//Header Length
	str.Format(_T("Header Length: %d bytes"), tcpheader->offset * 4);
	mDlg->info_tree.InsertItem(str, HTCP, TVI_LAST);
	//Flags
	str.Format(_T("Flags: 0x%02x"), tcpheader->flag);
	HTREEITEM HT_Flag = mDlg->info_tree.InsertItem(str, HTCP, TVI_LAST);
	str.Format(_T("..%d..... = Urgent: %s"), (tcpheader->flag & 0x20) >> 5, (1 == (tcpheader->flag & 0x20) >> 5) ? _T("Set") : _T("Not Set"));
	mDlg->info_tree.InsertItem(str, HT_Flag, TVI_LAST);
	str.Format(_T("...%d.... = Acknowledgement: %s"), (tcpheader->flag & 0x10) >> 4, (1 == (tcpheader->flag & 0x10) >> 4) ? _T("Set") : _T("Not Set"));
	mDlg->info_tree.InsertItem(str, HT_Flag, TVI_LAST);
	str.Format(_T("....%d... = Push: %s"), (tcpheader->flag & 0x08) >> 3, (1 == (tcpheader->flag & 0x08) >> 3) ? _T("Set") : _T("Not Set"));
	mDlg->info_tree.InsertItem(str, HT_Flag, TVI_LAST);
	str.Format(_T(".....%d.. = Reset: %s"), (tcpheader->flag & 0x04) >> 2, (1 == (tcpheader->flag & 0x04) >> 2) ? _T("Set") : _T("Not Set"));
	mDlg->info_tree.InsertItem(str, HT_Flag, TVI_LAST);
	str.Format(_T("......%d. = Syn: %s"), (tcpheader->flag & 0x02) >> 1, (1 == (tcpheader->flag & 0x02) >> 1) ? _T("Set") : _T("Not Set"));
	mDlg->info_tree.InsertItem(str, HT_Flag, TVI_LAST);
	str.Format(_T(".......%d = Fin: %s"), (tcpheader->flag & 0x01), (1 == (tcpheader->flag & 0x01)) ? _T("Set") : _T("Not Set"));
	mDlg->info_tree.InsertItem(str, HT_Flag, TVI_LAST);
	//Window size
	str.Format(_T("Window size value: %d"), ntohs(tcpheader->win));
	mDlg->info_tree.InsertItem(str, HTCP, TVI_LAST);
	//Checksum
	str.Format(_T("Checksum: 0x%04x"), ntohs(tcpheader->checksum));
	mDlg->info_tree.InsertItem(str, HTCP, TVI_LAST);
	//uptr
	if (1 == (tcpheader->flag & 0x20) >> 5)
	{
		str.Format(_T("Urgent point: %d"), ntohs(tcpheader->uptr));
		mDlg->info_tree.InsertItem(str, HTCP, TVI_LAST);
	}
}

void CSnifferTestDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CSnifferTestDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		int ItemClick = pNMListView->iItem;
		AfxBeginThread(ReadDumpThread, (LPVOID)ItemClick);
	}
	*pResult = 0;
}


void CSnifferTestDlg::OnSelchangeWangka()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CSnifferTestDlg::OnClickedBStop()
{
	// TODO: 在此添加控件通知处理程序代码
	if (TerminateThread(hCapThread->m_hThread, 2) == FALSE)
	{
		AfxMessageBox(_T("Stop CapThread Fail,perhaps it has stopped yet!"));
	}
	GetDlgItem(B_Start)->EnableWindow(TRUE);
	GetDlgItem(B_Stop)->EnableWindow(FALSE);
	// 	GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_CAPTURE_START, MF_BYCOMMAND|MF_ENABLED);
	// 	GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_CAPTURE_STOP, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	capStatus = FALSE;
}


void CSnifferTestDlg::OnSelchangeProtocaltype()
{
	// TODO: 在此添加控件通知处理程序代码
	CB_Filter.GetLBText(CB_Filter.GetCurSel(), filterstr);
	
}


void CSnifferTestDlg::OnBnClickedExit2()
{
	// TODO: 在此添加控件通知处理程序代码
}



void CSnifferTestDlg::OnChangeIp()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CSnifferTestDlg::OnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fDlg(TRUE, _T("(*.CAP)|*.CAP "), _T("*.CAP "), OFN_EXPLORER, _T("Sniffer数据包文件(*.CAP) "));
	if (IDOK == fDlg.DoModal())
	{
		CString CapFileSavePath = fDlg.GetPathName();
		CopyFile(CapFilePath, CapFileSavePath, FALSE);
	}
}


void CSnifferTestDlg::OnClickedArp()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nResponse = arpcheater.DoModal();
	
}
