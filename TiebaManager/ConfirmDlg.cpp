﻿/*
Copyright (C) 2015  xfgryujk
http://tieba.baidu.com/f?kw=%D2%BB%B8%F6%BC%AB%C6%E4%D2%FE%C3%D8%D6%BB%D3%D0xfgryujk%D6%AA%B5%C0%B5%C4%B5%D8%B7%BD

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

// ConfirmDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ConfirmDlg.h"
#include "ImageViewDlg.h"
#include "GetImages.h"


// CConfirmDlg 对话框

IMPLEMENT_DYNAMIC(CConfirmDlg, CDialog)

CConfirmDlg::CConfirmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfirmDlg::IDD, pParent),
	m_resize(this)
{
	m_operation = NULL;
}

CConfirmDlg::CConfirmDlg(const Operation* operation, CWnd* pParent)
	: CDialog(CConfirmDlg::IDD, pParent),
	m_resize(this)
{
	m_operation = operation;
}

#pragma region MFC
CConfirmDlg::~CConfirmDlg()
{
}

void CConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_contentEdit);
	DDX_Control(pDX, IDC_STATIC1, m_static);
	DDX_Control(pDX, IDOK, m_yesButton);
	DDX_Control(pDX, IDCANCEL, m_noButton);
	DDX_Control(pDX, IDC_BUTTON1, m_explorerButton);
}


BEGIN_MESSAGE_MAP(CConfirmDlg, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CConfirmDlg::OnBnClickedButton1)
END_MESSAGE_MAP()
#pragma endregion

// CConfirmDlg 消息处理程序

#pragma region UI
// 改变尺寸
void CConfirmDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	m_resize.Resize();
}
#pragma endregion

// 初始化
BOOL CConfirmDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_resize.AddControl(&m_contentEdit, RT_NULL, NULL, RT_NULL, NULL, RT_KEEP_DIST_TO_RIGHT, this, RT_KEEP_DIST_TO_BOTTOM, this);
	m_resize.AddControl(&m_static, RT_NULL, NULL, RT_KEEP_DIST_TO_BOTTOM, &m_contentEdit);
	m_resize.AddControl(&m_explorerButton, RT_KEEP_DIST_TO_RIGHT, this, RT_KEEP_DIST_TO_BOTTOM, &m_contentEdit);
	m_resize.AddControl(&m_yesButton, RT_KEEP_DIST_TO_RIGHT, this, RT_KEEP_DIST_TO_BOTTOM, &m_contentEdit);
	m_resize.AddControl(&m_noButton, RT_KEEP_DIST_TO_RIGHT, this, RT_KEEP_DIST_TO_BOTTOM, &m_contentEdit);

	if (m_operation != NULL)
	{
		SetWindowText(m_operation->title);
		m_contentEdit.SetWindowText(m_operation->object->GetContent() + _T("\r\n\r\n作者：") + m_operation->object->author);
		m_contentEdit.SetSel(m_operation->pos, m_operation->pos + m_operation->length);

		unique_ptr<vector<CString> > img(new vector<CString>());
		CGetImages(*m_operation->object)(*img);
		if (!img->empty())
		{
			m_imageViewDlg = new CImageViewDlg(m_imageViewDlg, this);
			m_imageViewDlg->Create(m_imageViewDlg->IDD, this);
			m_imageViewDlg->SetImages(std::move(img));
		}
	}
	MessageBeep(MB_ICONQUESTION);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

// 浏览器
void CConfirmDlg::OnBnClickedButton1()
{
	if (m_operation == NULL)
		return;

	CString url;
	if (m_operation->object->m_type == TBObject::THREAD) // 主题
		url = _T("http://tieba.baidu.com/p/") + m_operation->object->tid;
	else if (m_operation->object->m_type == TBObject::POST) // 帖子
	{
		PostInfo* post = (PostInfo*)m_operation->object.get();
		url.Format(_T("http://tieba.baidu.com/p/%s?pid=%s#%s"), (LPCTSTR)post->tid,
			(LPCTSTR)post->pid, (LPCTSTR)post->pid);
	}
	else if (m_operation->object->m_type == TBObject::LZL) // 楼中楼
	{
		LzlInfo* lzl = (LzlInfo*)m_operation->object.get();
		url.Format(_T("http://tieba.baidu.com/p/%s?pid=%s&cid=%s#%s"), (LPCTSTR)lzl->tid, 
			(LPCTSTR)lzl->cid, (LPCTSTR)lzl->cid, (LPCTSTR)lzl->cid);
	}

	ShellExecute(NULL, _T("open"), url, NULL, NULL, SW_NORMAL);
}
