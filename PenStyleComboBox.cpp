#include "stdafx.h"
#include "WinTILD.h"
#include "PenStyleComboBox.h"


IMPLEMENT_DYNAMIC(CPenStyleComboBox, CComboBox)
CPenStyleComboBox::CPenStyleComboBox()
{
}

CPenStyleComboBox::~CPenStyleComboBox()
{
}


BEGIN_MESSAGE_MAP(CPenStyleComboBox, CComboBox)
END_MESSAGE_MAP()


void CPenStyleComboBox::SetPen(LOGPEN lp)
{
	LockWindowUpdate();
	SetColor( lp.lopnColor );
	SetWidth( lp.lopnWidth );
	SetStyle( lp.lopnStyle );
	UnlockWindowUpdate();
}

void CPenStyleComboBox::SetStyle(UINT style)
{
	m_Pen.lopnStyle = style;
	for ( int i=0; i<GetCount(); i++){
		if ( style == GetItemData( i ) ){
			SetCurSel( i );
			break;
		}
	}
	Invalidate(FALSE);
}
void CPenStyleComboBox::SetColor(COLORREF color)
{
	m_Pen.lopnColor = color;;
	Invalidate(FALSE);

}
void CPenStyleComboBox::SetWidth(const CPoint& width)
{
	m_Pen.lopnWidth = width;
	Invalidate(FALSE);

}

void CPenStyleComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	int index = lpDrawItemStruct->itemID;
	if ( index < 0 )
		return;

	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CString txt;
	GetLBText( index, txt );


	if ( lpDrawItemStruct->itemState & ODS_SELECTED ){
		pDC->SetBkColor( GetSysColor(COLOR_HIGHLIGHT) );
		pDC->SetTextColor( GetSysColor(COLOR_HIGHLIGHTTEXT) );
	}
	else {
		pDC->SetBkColor( GetSysColor(COLOR_WINDOW) );
		pDC->SetTextColor( GetSysColor(COLOR_WINDOWTEXT) );
	}

	LOGPEN lp;
	lp.lopnColor = m_Pen.lopnColor;
	lp.lopnWidth = m_Pen.lopnWidth;
	lp.lopnStyle = GetItemData( index );


	CPen pen;
	HGDIOBJ oldPen;
	pen.CreatePenIndirect( &lp );

	oldPen = pDC->SelectObject( pen );
	pDC->SetBkMode( TRANSPARENT );
	pDC->SetTextAlign( TA_BOTTOM|TA_LEFT );

	CRect rc = lpDrawItemStruct->rcItem;
	
	pDC->FillSolidRect( &rc, pDC->GetBkColor() );

	pDC->MoveTo( rc.left, rc.top + rc.Height()/2 );
	pDC->LineTo( rc.right,rc.top + rc.Height()/2 );

	pDC->TextOut( rc.left, rc.bottom, txt );

	pDC->SelectObject( oldPen );

	pen.DeleteObject();
}

void CPenStyleComboBox::CreateList()
{
	ResetContent();
	SetItemData( AddString( "TRANSPARENT" ), PS_NULL );
	SetItemData( AddString( "SOLID" ),		 PS_SOLID);
	SetItemData( AddString( "DASH" ),		 PS_DASH );	
	SetItemData( AddString( "DOT" ),		 PS_DOT	 );
	SetItemData( AddString( "DASHDOT" ),	 PS_DASHDOT);
	SetItemData( AddString( "DASHDOTDOT" ),	 PS_DASHDOTDOT);	
/*
PS_SOLID      : Create a solid pen.
PS_DASH       : Create a dashed pen, which is valid when the pen's width is 1.
PS_DOT        : Create a dotted pen, which is valid when the pen's width is 1.
PS_DASHDOT    : Create a dashed and dotted pen, which is valid when the pen's width is 1.
PS_DASHDOTDOT : Create a dashed and doubly-dotted pen, which is valid when the pen's width is 1.
PS_NULL       : Create a null pen.
*/
}
