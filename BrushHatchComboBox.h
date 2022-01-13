#pragma once


class CBrushHatchComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CBrushHatchComboBox)
protected:
	LOGBRUSH m_Brush;
public:
	CBrushHatchComboBox();
	virtual ~CBrushHatchComboBox();

	void SetBrush(LOGBRUSH lb);

	void SetStyle(UINT style);
	void SetColor(COLORREF color);
	void SetHatch(long hatch);
	DWORD GetHatch() const { return static_cast<DWORD>(GetItemData(GetCurSel())); };

protected:
	DECLARE_MESSAGE_MAP()

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

public:
	void CreateList();
};


