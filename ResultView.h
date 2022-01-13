#pragma once

#include "afxcview.h"
#include "WinTILDDoc.h"

class CResultView : public CListView
{
	DECLARE_DYNCREATE(CResultView)

protected:
	CImageList m_ImageList;

	CResultView();
	virtual ~CResultView();
	BOOL m_Lock;

public:
	CWinTILDDoc* GetDocument() const;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;

#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

public:
	void UpdateView(void);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void Select(int index);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	void Lock(){ m_Lock = TRUE; };
	void Unlock(){ m_Lock = FALSE; };
	BOOL Locking()const{ return m_Lock; };
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	// Send selected data to CWinTILDDoc
	void SelectToUpdate(void);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


