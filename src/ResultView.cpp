#include "stdafx.h"
#include "WinTILD.h"
#include "ResultView.h"


IMPLEMENT_DYNCREATE(CResultView, CListView)

CResultView::CResultView()
{
	m_Lock = FALSE;
}

CResultView::~CResultView()
{
}

BEGIN_MESSAGE_MAP(CResultView, CListView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnLvnItemchanged)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


#ifdef _DEBUG
void CResultView::AssertValid() const
{
	CListView::AssertValid();
}

void CResultView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG


void CResultView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
	switch ( lHint ){
		case UPDATE_NEWDOCUMENT:
		case UPDATE_ADDRESULT:
		case UPDATE_IMPORT:
		case UPDATE_DELETERESULT:
			UpdateView();
			break;
		case UPDATE_LOCKVIEWS:
			Lock();
			break;
		case UPDATE_UNLOCKVIEWS:
			Unlock();
			break;
		case UPDATE_RESULTVIEW:
			Lock();
			UpdateView();
			Unlock();
		default:
			break;
	}
	if ( !Locking() ){
		Invalidate(FALSE);
		UpdateWindow();
	}
}

CWinTILDDoc* CResultView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinTILDDoc)));
	return (CWinTILDDoc*)m_pDocument;
}

void CResultView::UpdateView(void)
{
	CWinTILDDoc *pDoc = GetDocument();
	CDataArray *pArray = pDoc->GetResult();	
	CListCtrl &list = GetListCtrl();

	list.DeleteAllItems();

	TCHAR drive[8];
	TCHAR dir[1024];
	TCHAR filename[512];
	TCHAR ext[128];
	for (int i=0; i<pArray->GetSize(); i++){
		_splitpath( *pArray->GetAt(i)->GetFilename(), drive, dir, filename, ext );
		TCHAR str[639];
		sprintf( str, "%s%s", filename, ext );
		list.InsertItem(i, str, 0);
	}

	CArray<int> *pIndexArray = pDoc->GetIndexesToUpdate();
	int index;
	for (int i=0; i<pIndexArray->GetSize(); i++){
		index = pIndexArray->GetAt(i);
		list.SetItemState( index,LVIS_SELECTED,LVIS_SELECTED );
	}

}

int CResultView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CListCtrl& theCtrl = GetListCtrl();
	theCtrl.InsertColumn(0, _T("File Name"), LVCFMT_LEFT, 80);

	m_ImageList.Create(32,32,ILC_COLOR8 ,0, 1);
	
	HICON icon;

	icon = theApp.LoadIcon(IDI_ICON_RESULT);
	ASSERT( icon != NULL );
	m_ImageList.Add( icon );

	theCtrl.SetImageList( &m_ImageList, LVSIL_NORMAL);

	return 0;
}

BOOL CResultView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |=  ( LVS_EX_FULLROWSELECT | LVS_SHOWSELALWAYS | LVS_ICON  );
	cs.style &= ~LVS_SORTASCENDING;
	cs.style &= ~LVS_SORTDESCENDING;

	return CListView::PreCreateWindow(cs);
}

void CResultView::Select(int index)
{
	CWinTILDDoc *pDoc = GetDocument();
	pDoc->Select( index );
	pDoc->InteractiveUpdate();
}

void CResultView::OnLvnItemchanged(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
	//LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if ( !Locking() ){
		SelectToUpdate();
	}
	*pResult = 0;
}

void CResultView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	if ( !Locking() ){
		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		Select( pNMLV->iItem );
	}
	*pResult = 0;
}


void CResultView::SelectToUpdate(void)
{
	CListCtrl& list = GetListCtrl();
	CArray<int> selected;
	UINT count = list.GetSelectedCount();
	int itemIndex = -1;

	for (UINT i=0; i<count; i++)
	{
		itemIndex = list.GetNextItem(itemIndex, LVNI_SELECTED);
		selected.Add(itemIndex);
	}

	CWinTILDDoc *pDoc = GetDocument();
	if (pDoc == NULL)
		return;
	pDoc->SelectToUpdate(selected);	
}

void CResultView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ( nChar == VK_DELETE){
		CWinTILDDoc *pDoc = GetDocument();
		pDoc->DeleteSelelctedResult();
	}

	CListView::OnKeyDown(nChar, nRepCnt, nFlags);
}
