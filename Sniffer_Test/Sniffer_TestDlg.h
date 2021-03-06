
// Sniffer_TestDlg.h: 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MyPcap.h"
#include "ArpDlg.h"

//static UINT CapThread(LPVOID lpParameter);
static wchar_t FilePath[256];//程序运行路径
static CString CapFilePath;//临时抓包文件路径
static CString filterstr;
static CString ip_string;
static CString port_string;
static UINT CapThread(LPVOID lpParameter);
static UINT ReadDumpThread(LPVOID lpParameter);

// CSnifferTestDlg 对话框
class CSnifferTestDlg : public CDialogEx
{
// 构造
public:
	CSnifferTestDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SNIFFER_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnCbnSelendcancelWangka();
	afx_msg void OnCbnSelchangeWangka();
	afx_msg void OnBnClickedStart();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelchangeWangka();
	afx_msg void OnClickedBStop();
	CComboBox CB_Wangka;
	CComboBox CB_Filter;
	pcap_if_t *alldevs;
	pcap_if_t *d;
	CListCtrl List1;
	CMyPcap myPcap;
	ArpDlg arpcheater;
	CFont m_Font;//字体
	CEdit packet_data;//Edit Control的控制变量
	CEdit ip_address;
	CEdit port_lala;
	CTreeCtrl info_tree;//tree view control的控制变量
	BOOL capStatus;
	CWinThread *hCapThread;
	static void GetIPv4Type(ip_header* ih, char* pt);//IP类型
	static void GetEthernetType(ethernet_header * e, char *typestr);//获得以太网类型
	afx_msg void OnClose();

	//几个info函数
	static void CSnifferTestDlg::ShowIPInfo(const u_char* pkt_data, CSnifferTestDlg* mDlg);
	static void CSnifferTestDlg::ShowArpInfo(arp_header* arpheader, CSnifferTestDlg* mDlg);
	static void CSnifferTestDlg::ShowUDPInfo(udp_header* udpheader, CSnifferTestDlg* mDlg);
	static void CSnifferTestDlg::ShowICMPInfo(icmp_header* icmpheader, CSnifferTestDlg* mDlg);
	static void CSnifferTestDlg::ShowIGMPInfo(igmp_header* igmpheader, CSnifferTestDlg* mDlg);
	static void CSnifferTestDlg::ShowTCPInfo(tcp_header* tcpheader, CSnifferTestDlg* mDlg, u_short tcpdataLen);

	//一个工具函数
	static void CSnifferTestDlg::DecToBinary(int n, char* ch);

	void CSnifferTestDlg::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);

	static BOOL  CSnifferTestDlg::IsHttp(struct pcap_pkthdr *header, const u_char* pkt_data);

	static CString CSnifferTestDlg::PackToEdit(struct pcap_pkthdr* header, const u_char* pkt_data);

	afx_msg void OnSelchangeProtocaltype();
	afx_msg void OnBnClickedExit2();
	afx_msg void OnClickedBSave();
	afx_msg void OnChangeIp();
	afx_msg void OnBnClickedSave();
	afx_msg void OnClickedButtonSave();
	afx_msg void OnClickedArp();
};
