#include "stdafx.h"
#include "WinTILD.h"
#include "BrushStyleComboBox.h"


IMPLEMENT_DYNAMIC(CBrushStyleComboBox, CComboBox)
CBrushStyleComboBox::CBrushStyleComboBox()
{
}

CBrushStyleComboBox::~CBrushStyleComboBox()
{
}


BEGIN_MESSAGE_MAP(CBrushStyleComboBox, CComboBox)
END_MESSAGE_MAP()



void CBrushStyleComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	int index = lpDrawItemStruct->itemID;
	if ( index < 0 )
		return;
	CString txt;
	GetLBText( index, txt );

	LOGBRUSH lb;
	lb.lbColor = m_Brush.lbColor;
	lb.lbHatch = m_Brush.lbHatch;
	lb.lbStyle = (UINT)GetItemData( index );
	LOGPEN lp;
	lp.lopnStyle = PS_NULL;

	CPen pen;
	CBrush br;
	HGDIOBJ oldPen, oldBrush;	

	if ( lpDrawItemStruct->itemState & ODS_SELECTED ){
		SetBkColor( lpDrawItemStruct->hDC, GetSysColor(COLOR_HIGHLIGHT) );
		SetTextColor( lpDrawItemStruct->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT) );
		if ( lb.lbStyle == BS_NULL ){
			lb.lbColor = GetSysColor(COLOR_HIGHLIGHT);
			lb.lbStyle = BS_SOLID;
		}
	}
	else {
		SetBkColor( lpDrawItemStruct->hDC, GetSysColor(COLOR_WINDOW) );
		SetTextColor( lpDrawItemStruct->hDC, GetSysColor(COLOR_WINDOWTEXT) );
		if ( lb.lbStyle == BS_NULL ){
			lb.lbColor = GetSysColor(COLOR_WINDOW);
			lb.lbStyle = BS_SOLID;
		}		
	}

	pen.CreatePenIndirect( &lp );
	br.CreateBrushIndirect(&lb );

	oldPen = SelectObject( lpDrawItemStruct->hDC, &pen );
	oldBrush = SelectObject( lpDrawItemStruct->hDC, &br);

	CRect rc = lpDrawItemStruct->rcItem;

	FillRect( lpDrawItemStruct->hDC, &rc, br);

	SetTextAlign( lpDrawItemStruct->hDC, TA_BOTTOM|TA_LEFT );
	SetBkMode( lpDrawItemStruct->hDC, TRANSPARENT );
	TextOut( lpDrawItemStruct->hDC, rc.left, rc.bottom, txt, txt.GetLength() );

	SelectObject( lpDrawItemStruct->hDC, oldPen );
	SelectObject( lpDrawItemStruct->hDC, oldBrush );

	br.DeleteObject();
	pen.DeleteObject();
}


void CBrushStyleComboBox::CreateList()
{
	ResetContent();
	SetItemData( AddString( "TRANSPARENT" ), BS_NULL );
	SetItemData( AddString( "SOLID" ), BS_SOLID );
	SetItemData( AddString( "HATCHED" ), BS_HATCHED);
/*
BS_HATCHED : a hatched brush 
BS_NULL    : a null brush 
BS_SOLID   : a solid brush
*/
}
void CBrushStyleComboBox::SetBrush(LOGBRUSH lb)
{ 
	LockWindowUpdate();
	SetColor( lb.lbColor );
	SetHatch( static_cast<long>(lb.lbHatch) );
	SetStyle( lb.lbStyle );
	UnlockWindowUpdate();
	Invalidate(FALSE);
}
void CBrushStyleComboBox::SetStyle(UINT style)
{ 
	m_Brush.lbStyle = style;
	for ( int i=0; i<GetCount(); i++){
		if ( style == GetItemData( i ) ){
			SetCurSel( i );
			break;
		}
	}
	Invalidate(FALSE);
}
void CBrushStyleComboBox::SetColor(COLORREF color)
{ 
	m_Brush.lbColor = color; 
	Invalidate(FALSE);
}
void CBrushStyleComboBox::SetHatch(long hatch)
{
	m_Brush.lbHatch = hatch;
	Invalidate(FALSE);
}
