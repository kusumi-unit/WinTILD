#include "stdafx.h"
#include "WinTILD.h"
#include "BrushStatic.h"


IMPLEMENT_DYNAMIC(CBrushStatic, CStatic)
CBrushStatic::CBrushStatic()
{
}

CBrushStatic::~CBrushStatic()
{
}


BEGIN_MESSAGE_MAP(CBrushStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



void CBrushStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(rc);

	CBrush br, *poldBrush;
	br.CreateBrushIndirect(&m_Brush);
	poldBrush = dc.SelectObject( &br );

	dc.RoundRect( &rc, CPoint(8,8) );

	dc.SelectObject( poldBrush );
	br.DeleteObject();
}

void CBrushStatic::SetBrush(const LOGBRUSH& brush)
{
	m_Brush = brush;
	if (m_Brush.lbStyle == BS_NULL ){
		GetParent()->Invalidate(TRUE);
	}
	Invalidate(FALSE);
}
