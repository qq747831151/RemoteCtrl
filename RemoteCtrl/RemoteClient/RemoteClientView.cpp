
// RemoteClientView.cpp: CRemoteClientView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "RemoteClient.h"
#endif

#include "RemoteClientDoc.h"
#include "RemoteClientView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRemoteClientView

IMPLEMENT_DYNCREATE(CRemoteClientView, CView)

BEGIN_MESSAGE_MAP(CRemoteClientView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CRemoteClientView 构造/析构

CRemoteClientView::CRemoteClientView() noexcept
{
	// TODO: 在此处添加构造代码

}

CRemoteClientView::~CRemoteClientView()
{
}

BOOL CRemoteClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CRemoteClientView 绘图

void CRemoteClientView::OnDraw(CDC* /*pDC*/)
{
	CRemoteClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}

void CRemoteClientView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CRemoteClientView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CRemoteClientView 诊断

#ifdef _DEBUG
void CRemoteClientView::AssertValid() const
{
	CView::AssertValid();
}

void CRemoteClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRemoteClientDoc* CRemoteClientView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRemoteClientDoc)));
	return (CRemoteClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CRemoteClientView 消息处理程序
