#include "stdafx.h"
#include "Function.h"

void DDX_Color(CDataExchange *pDX, CColorStatic &member, RGBQUAD &color)
{
	if (pDX->m_bSaveAndValidate){
		color.rgbRed  =  member.GetColor() & 0xFF;
		color.rgbGreen= (member.GetColor()>>8) & 0xFF;
		color.rgbBlue = (member.GetColor()>>16) & 0xFF;
	}
	else {
		member.SetColor( color.rgbRed + (color.rgbGreen << 8) + (color.rgbBlue << 16) );
	}
}

void DDX_Color(CDataExchange *pDX, CColorStatic &member, COLORREF &color)
{
	if (pDX->m_bSaveAndValidate) {
		color = member.GetColor();
	}
	else {
		member.SetColor( color );
	}
}

void DDX_Brush(CDataExchange *pDX, CBrushStatic &member, LOGBRUSH &brush)
{
	if (pDX->m_bSaveAndValidate) {
		brush = member.GetBrush();
	}
	else {
		member.SetBrush( brush );
	}
}


void DDX_Pen(CDataExchange *pDX, CPenStatic &member, LOGPEN &pen)
{
	if (pDX->m_bSaveAndValidate) {
		pen = member.GetPen();
	}
	else {
		member.SetPen( pen );
	}
}

void DDX_Status(CDataExchange *pDX,CWnd* pCheck, unsigned long mask, unsigned long &status)
{
	if (pDX->m_bSaveAndValidate) {
		status = ((((CButton*)pCheck)->GetCheck()==BST_CHECKED)?mask:0) | (status & ~mask);
	}
	else {
		((CButton*)pCheck)->SetCheck( (status&mask)?BST_CHECKED:BST_UNCHECKED );
	}	
}

void DDX_Font(CDataExchange *pDX, CStatic &member, LOGFONT &font)
{
	if (pDX->m_bSaveAndValidate) {
		member.GetFont()->GetLogFont( &font );
	}
	else {
		member.GetFont()->DeleteObject();
		CFont ft;
		ft.CreateFontIndirect(&font);
		member.SetFont( &ft );
	}	
}

void DDX_BkMode(CDataExchange *pDX, CWnd* check, int &bkMode)
{
	CButton *pCheck = (CButton*)check;
	if (pDX->m_bSaveAndValidate) {
		bkMode = (pCheck->GetCheck()==BST_CHECKED)?OPAQUE:TRANSPARENT;
	}
	else {
		pCheck->SetCheck( (bkMode == TRANSPARENT)?BST_UNCHECKED:BST_CHECKED );
	}	
}
