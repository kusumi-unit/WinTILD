#include "stdafx.h"
#include "WinTILD.h"
#include "BrushHatchComboBox.h"


IMPLEMENT_DYNAMIC(CBrushHatchComboBox, CComboBox)
CBrushHatchComboBox::CBrushHatchComboBox()
{
}

CBrushHatchComboBox::~CBrushHatchComboBox()
{
}


BEGIN_MESSAGE_MAP(CBrushHatchComboBox, CComboBox)
END_MESSAGE_MAP()



void CBrushHatchComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	int index = lpDrawItemStruct->itemID;
	if ( index < 0 )
		return;
	CString txt;
	GetLBText( index, txt );

	LOGBRUSH lb;
	lb.lbColor = m_Brush.lbColor;
	lb.lbHatch = GetItemData( index );
	lb.lbStyle = BS_HATCHED;
	LOGPEN lp;
	lp.lopnStyle = PS_NULL;

	CPen pen;
	CBrush br;
	HGDIOBJ oldPen, oldBrush;	

	if ( lpDrawItemStruct->itemState & ODS_SELECTED ){
		SetBkColor( lpDrawItemStruct->hDC, GetSysColor(COLOR_HIGHLIGHT) );
		SetTextColor( lpDrawItemStruct->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT) );
	}
	else {
		SetBkColor( lpDrawItemStruct->hDC, GetSysColor(COLOR_WINDOW) );
		SetTextColor( lpDrawItemStruct->hDC, GetSysColor(COLOR_WINDOWTEXT) );	
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


void CBrushHatchComboBox::SetBrush(LOGBRUSH lb){
	LockWindowUpdate();
	SetColor( lb.lbColor );
	SetHatch( static_cast<long>(lb.lbHatch) );
	SetStyle( lb.lbStyle );
	UnlockWindowUpdate();
	Invalidate(FALSE);
}

void CBrushHatchComboBox::SetStyle(UINT style){
	m_Brush.lbStyle = style;
	Invalidate(FALSE);
}

void CBrushHatchComboBox::SetColor(COLORREF color){
	m_Brush.lbColor = color;
	Invalidate(FALSE);
}

void CBrushHatchComboBox::SetHatch(long hatch){
	m_Brush.lbHatch = hatch;
	for ( int i=0; i<GetCount(); i++){
		if ( hatch == GetItemData( i ) ){
			SetCurSel( i );
			break;
		}
	}
	Invalidate(FALSE);
}


void CBrushHatchComboBox::CreateList()
{
	ResetContent();
	SetItemData( AddString( "BDIAGONAL" ) ,HS_BDIAGONAL );
	SetItemData( AddString( "CROSS" )     ,HS_CROSS		);
	SetItemData( AddString( "DIAGCROSS" ) ,HS_DIAGCROSS	);
	SetItemData( AddString( "FDIAGONAL" ) ,HS_FDIAGONAL );
	SetItemData( AddString( "HORIZONTAL" ),HS_HORIZONTAL);
	SetItemData( AddString( "VERTICAL" )  ,HS_VERTICAL	);
/*
HS_BDIAGONAL   a hatch of 45 degree angle from left-bottom to right-top
HS_CROSS       a horizontal and vertical lattice hatch 
HS_DIAGCROSS   a lattice hatch of 45 degree angle 
HS_FDIAGONAL   a hatch fo 45 degree angle from left-top to right-bottom
HS_HORIZONTAL  a horizontal hatch 
HS_VERTICAL    a vertical hatch
*/
}

