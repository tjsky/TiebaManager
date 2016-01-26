#pragma once
#include "NormalDlg.h"
#include "LoopBanPage.h"
#include "DefriendPage.h"
#include "LockThreadPage.h"
#include "Setting.h"


// CSuperFunctionDlg 对话框

class CSuperFunctionDlg : public CNormalDlg
{
	DECLARE_DYNAMIC(CSuperFunctionDlg)

public:
	CSuperFunctionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSuperFunctionDlg();

// 对话框数据
	enum { IDD = IDD_SETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	virtual void OnOK();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();

	void ShowCurrentOptions();
	void ApplyOptionsInDlg();


public:
	CTabCtrl m_tab;
	CLoopBanPage m_loopBanPage;
	CDefriendPage m_defriendPage;
	CLockThreadPage m_lockThreadPage;
	CButton m_okButton;
	CButton m_cancelButton;

	BOOL m_clearScanCache;
protected:
	CWnd* m_pages[3];
	CResizeControl m_pagesResize;
};

class CLoopBanConfig : public CConfigBase
{
public:
	COption<BOOL> m_enable;					// 开启
	COption<BOOL> m_log;					// 输出日志
	COption<vector<CString> > m_userList;	// 用户列表
	COption<vector<CString> > m_pidList;	// PID列表

	CLoopBanConfig()
		: CConfigBase("LoopBan"),
		m_enable("Enable", TRUE),
		m_log("Log"),
		m_userList("Name"),
		m_pidList("PID")
	{
		m_options.push_back(&m_enable);
		m_options.push_back(&m_log);
		m_options.push_back(&m_userList);
		m_options.push_back(&m_pidList);
	}

	BOOL LoadOld(const CString& path)
	{
		gzFile f = gzopen_w(path, "rb");
		if (f == NULL)
			return FALSE;

		// 头部
		char header[2];
		gzread(f, header, sizeof(header));
		if (header[0] != 'T' || header[1] != 'B')
		{
			gzclose(f);
			return FALSE;
		}

		int size;
		gzread(f, &size, sizeof(int)); // 长度
		m_userList->resize(size);
		m_pidList->resize(size);
		for (int i = 0; i < size; i++)
		{
			ReadText(f, (*m_userList)[i]);
			ReadText(f, (*m_pidList)[i]);
		}
		*m_log = FALSE;
		gzread(f, &*m_log, sizeof(BOOL)); // 输出日志
		if (gzread(f, &*m_enable, sizeof(BOOL)) != sizeof(BOOL)) // 开启
			*m_enable = TRUE;

		gzclose(f);
		return TRUE;
	}
};
