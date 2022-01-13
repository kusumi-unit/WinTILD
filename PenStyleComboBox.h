#pragma once


class CPenStyleComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CPenStyleComboBox)

protected:
	LOGPEN m_Pen;
public:
	CPenStyleComboBox();
	virtual ~CPenStyleComboBox();

	void SetPen(LOGPEN lp);

	void SetStyle(UINT style);
	void SetColor(COLORREF color);
	void SetWidth(const CPoint& width);

	UINT GetStyle()const{ return (UINT)GetItemData( GetCurSel() ); };
	void CreateList();
protected:
	DECLARE_MESSAGE_MAP()

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};
