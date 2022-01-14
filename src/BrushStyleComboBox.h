#pragma once


class CBrushStyleComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CBrushStyleComboBox)

protected:
	LOGBRUSH m_Brush;
public:
	CBrushStyleComboBox();
	virtual ~CBrushStyleComboBox();

	void SetBrush(LOGBRUSH lb);

	void SetStyle(UINT style);
	void SetColor(COLORREF color);
	void SetHatch(long hatch);

	UINT GetStyle()const{ return (UINT)GetItemData( GetCurSel() ); };
	void CreateList();
protected:
	DECLARE_MESSAGE_MAP()

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


