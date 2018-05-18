#pragma once


// ArpDlg 对话框

class ArpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ArpDlg)

public:
	ArpDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ArpDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ARP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CEdit CE_mbip;
	CEdit CE_mbmac;
	CEdit CE_wgip;
	CEdit CE_wgmac;
	CEdit CE_bjmac;
};
