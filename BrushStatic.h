#pragma once


class CBrushStatic : public CStatic
{
	DECLARE_DYNAMIC(CBrushStatic)

public:
	CBrushStatic();
	virtual ~CBrushStatic();

protected:
	LOGBRUSH m_Brush;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	void SetBrush(const LOGBRUSH& brush);
	LOGBRUSH GetBrush()const{ return m_Brush; };
};


