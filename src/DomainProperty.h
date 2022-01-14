#pragma once

#include "WinTILDDoc.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "TrajectoryPlotter.h"
#include "DivMSDPlotter.h"


class CDomainProperty : public CPropertyPage
{
	DECLARE_DYNAMIC(CDomainProperty)

public:
	CDomainProperty();
	virtual ~CDomainProperty();
	BOOL Create(CWnd *pParentWnd = NULL);
	
	enum { IDD = IDD_DOMAIN_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

	CWinTILDDoc* GetDocument();
	BOOL m_Lock;
public:
	CEdit m_EditIndex;
	CSpinButtonCtrl m_SpinIndex;
	virtual BOOL OnInitDialog();
	void SelectDomain(int index);
	void Update();
	afx_msg void OnEnChangeEditDomainindex();
	afx_msg void OnDestroy();
	void Lock(){ m_Lock = TRUE; };
	void Unlock(){ m_Lock = FALSE; };
	BOOL Locking()const{ return m_Lock; };
};
