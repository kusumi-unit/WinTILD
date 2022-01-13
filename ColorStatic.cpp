#include "stdafx.h"
#include "WinTILD.h"
#include "ColorStatic.h"


IMPLEMENT_DYNAMIC(CColorStatic, CStatic)
CColorStatic::CColorStatic()
{
	m_Color = 0x000000;
}

CColorStatic::~CColorStatic()
{
}


BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CColorStatic::OnPaint()
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(rc);

	CBrush br(m_Color), *poldBrush;
	poldBrush = dc.SelectObject( &br );

	dc.RoundRect( &rc, CPoint(8,8) );
	
	dc.SelectObject( poldBrush );
	br.DeleteObject();
}

void CColorStatic::SetColor( COLORREF color )
{
	m_Color = color;
	Invalidate(FALSE);
}
