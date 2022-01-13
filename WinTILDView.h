#pragma once
#include "TrajectoryPlotter.h"
#include "divmsdplotter.h"
#include "informationPlotter.h"

class CWinTILDView : public CScrollView
{
protected:
	CWinTILDView();
	DECLARE_DYNCREATE(CWinTILDView)

private:
	HCURSOR m_hcsScroll;
	HCURSOR m_hcsArrow;
	HCURSOR m_hcsMagnify;
	CPoint m_PrevPoint;
protected:
	BOOL m_Lock;
	CImage m_bmpBuffer;
	BOOL m_bValidBuffer;

	CTrajectoryPlotter	m_plotTrajectory, *m_pPrintTrajectory;
	CDivMSDPlotter		m_plotDivMSD    , *m_pPrintDivMSD;
	CInformationPlotter m_plotInformation,*m_pPrintInformation;

public:
	void Lock();
	void Unlock();
	BOOL Locking();
protected:
	void Draw(CDC* pDC);
	CWinTILDDoc* GetDocument() const;

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate();
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

public:
	virtual ~CWinTILDView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG
inline CWinTILDDoc* CWinTILDView::GetDocument() const
   { return reinterpret_cast<CWinTILDDoc*>(m_pDocument); }
#endif

