#pragma once


class CProgressListBox : public CListBox
{
	DECLARE_DYNAMIC(CProgressListBox)

protected:
	LOGBRUSH m_lgProgress;
public:
	CProgressListBox();
	virtual ~CProgressListBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void Update();
};
