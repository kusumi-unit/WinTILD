#pragma once


class CColorStatic : public CStatic
{
	DECLARE_DYNAMIC(CColorStatic)

protected:
	COLORREF m_Color;
public:
	CColorStatic();
	virtual ~CColorStatic();
protected:
	DECLARE_MESSAGE_MAP()
public:
	void SetColor( COLORREF color );
	COLORREF GetColor()const{return m_Color;};
	afx_msg void OnPaint();
};


