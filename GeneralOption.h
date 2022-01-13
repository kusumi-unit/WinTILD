#pragma once


class CGeneralOption : public CObject
{
protected:
	CString m_lpszSection;
public:
	COLORREF m_clBackGround;

public:
	CGeneralOption();
	virtual ~CGeneralOption();

	void LoadProfiles(CWinApp *pApp);
	void SaveProfiles(CWinApp *pApp);
};
