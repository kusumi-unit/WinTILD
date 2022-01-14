#include "stdafx.h"
#include "WinTILD.h"
#include "PenStatic.h"


IMPLEMENT_DYNAMIC(CPenStatic, CStatic)
CPenStatic::CPenStatic()
{
}

CPenStatic::~CPenStatic()
{
}


BEGIN_MESSAGE_MAP(CPenStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CPenStatic::OnPaint()
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);

	CPen pen,*poldPen;
	pen.CreatePenIndirect( &m_Pen );
	poldPen = dc.SelectObject( &pen );

	dc.MoveTo( rc.left, rc.top + rc.Height()/2 );
	dc.LineTo( rc.right, rc.top+ rc.Height()/2 );

	dc.SelectObject( poldPen );
	pen.DeleteObject();
}

void CPenStatic::SetPen(const LOGPEN& pen)
{
	m_Pen = pen;
	Invalidate(FALSE);
	GetParent()->Invalidate(TRUE);
}
