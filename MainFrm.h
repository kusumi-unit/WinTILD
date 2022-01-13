#pragma once

#include "WinTILDDoc.h"
#include "InputDlg.h"
#include "ParameterDlg.h"
#include "ProgressDlg.h"
#include "OptionSheet.h"
#include "DetailSheet.h"
#include "DomainProperty.h"
#include "TrajectoryProperty.h"
#include "DivMSDProperty.h"
#include "ProgressListDlg.h"
#include "GeneralProperty.h"
#include "PlayDlg.h"

class CMainFrame : public CFrameWnd
{
	
protected:
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

protected:
	CInputDlg		 m_wndInput;
	CParameterDlg	 m_wndParameter;
	CProgressDlg	 m_wndProgress;
	CProgressListDlg m_wndProgressList;
	CPlayDlg         m_wndPlay;

	COptionSheet		m_wndOptionSheet;
	CTrajectoryProperty m_wndTrajectoryProperty;
	CDivMSDProperty		m_wndDivMSDProperty;
	CGeneralProperty    m_wndGeneralProperty;

	CDetailSheet	m_wndDetailSheet;
	CDomainProperty m_wndDomainProperty;

	void FindDomain(void);
	void Update(void);

public:
	CWinTILDDoc*		 GetActiveDocument(void);
	CParameterDlg*	 GetParameterDlg(){ return &m_wndParameter; };
	CDomainProperty* GetDomainProperty(){ return &m_wndDomainProperty; };
	CDivMSDProperty* GetDivMSDProperty(){ return &m_wndDivMSDProperty; };
	CInputDlg*		 GetInputDlg(void);
	CProgressDlg*	 GetProgressDlg(void);
	CProgressListDlg* GetProgressListDlg(void){ return &m_wndProgressList; };
	CPlayDlg* GetPlayDlg(void) {return &m_wndPlay; }
	void UpdateViewOption(void);

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CStatusBar   m_wndStatusBar;
	CToolBar     m_wndToolBar;
	CSplitterWnd m_wndSplitter;

protected:
	DECLARE_MESSAGE_MAP()
	void OnAddTrajectory(void);
	void OnFindDomain(void);
	void OnUpdate(void);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnViewInputdlg();
	afx_msg void OnViewParameterdlg();
	afx_msg void OnViewPlaydlg();
	afx_msg void OnViewOption();
	afx_msg void OnOperateFinddomain();
	afx_msg void OnOperateUpdate();
	afx_msg void OnOperateDelete();
	afx_msg void OnOperateSelectall();
	afx_msg void OnViewMagnify100();
	afx_msg void OnViewMagnify200();
	afx_msg void OnViewMagnify150();
	afx_msg void OnViewMagnify125();
	afx_msg void OnViewMagnify075();
	afx_msg void OnViewMagnify050();
	afx_msg void OnViewMagnify025();
	afx_msg void OnUpdateExportResult(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewInputdlg(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewParameterdlg(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewPlaydlg(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOperateFinddomain(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOperateUpdate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOperateDelete(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOperateSelectall(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewMagnify(CCmdUI * pCmdUI);
public:
	afx_msg void OnViewDetail();
	afx_msg void OnFileFoundDomainResult();


};


