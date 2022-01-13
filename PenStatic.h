#pragma once


class CPenStatic : public CStatic
{
	DECLARE_DYNAMIC(CPenStatic)

protected:
	LOGPEN m_Pen;
public:
	CPenStatic();
	virtual ~CPenStatic();

	LOGPEN GetPen()const{return m_Pen;};
	void SetPen(const LOGPEN& pen);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
