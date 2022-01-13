#include "stdafx.h"
#include "WinTILD.h"
#include "ProgressListBox.h"
#include "DivMSD.h"


IMPLEMENT_DYNAMIC(CProgressListBox, CListBox)
CProgressListBox::CProgressListBox()
{
	m_lgProgress.lbColor = 0xF08080;
	m_lgProgress.lbHatch = NULL;
	m_lgProgress.lbStyle = BS_SOLID;
}

CProgressListBox::~CProgressListBox()
{
}


BEGIN_MESSAGE_MAP(CProgressListBox, CListBox)
	ON_COMMAND(ID_UPDATE_PROGRESS,Update)
END_MESSAGE_MAP()


void CProgressListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	int index = lpDIS->itemID;
	if (index==-1)
		return;
	static CString txt;
	GetText(index,txt);
	int length = txt.GetLength();
	if (length <= 0)
		return;
	CDivMSD *pdmsd = (CDivMSD*)GetItemData(index);
	if (pdmsd==NULL)
		return;
	float percent = (float)pdmsd->GetProgress()/(float)pdmsd->GetProgressMax();
	CRect rc(lpDIS->rcItem);
	rc.SetRect(rc.left, rc.top, static_cast<int>(rc.left + rc.Width() * percent), rc.bottom);

	CBrush br;
	br.CreateBrushIndirect(&m_lgProgress);

	int oldBM = ::SetBkMode(lpDIS->hDC,TRANSPARENT);

	::FillRect(lpDIS->hDC,&rc,(HBRUSH)br.GetSafeHandle());
	::TextOut(lpDIS->hDC,lpDIS->rcItem.left,lpDIS->rcItem.top,txt,length);

	br.DeleteObject();
	::SetBkMode(lpDIS->hDC,oldBM);

}

void CProgressListBox::Update()
{
	Invalidate(FALSE);
	UpdateWindow();
}
