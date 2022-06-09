
// RemoteClientView.h: CRemoteClientView 类的接口
//

#pragma once


class CRemoteClientView : public CView
{
protected: // 仅从序列化创建
	CRemoteClientView() noexcept;
	DECLARE_DYNCREATE(CRemoteClientView)

// 特性
public:
	CRemoteClientDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CRemoteClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // RemoteClientView.cpp 中的调试版本
inline CRemoteClientDoc* CRemoteClientView::GetDocument() const
   { return reinterpret_cast<CRemoteClientDoc*>(m_pDocument); }
#endif

