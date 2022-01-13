#include "stdafx.h"
#include "WinTILD.h"
#include "MainFrm.h"
#include "WinTILDView.h"
#include "ResultView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define GETDOCUMENT()	\
	GetActiveDocument();\
	if (pDoc == NULL)	\
		return			\


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
	ON_COMMAND(ID_ADDTRAJECTORY,OnAddTrajectory)
	ON_COMMAND(ID_FINDDOMAIN,	OnFindDomain)
	ON_COMMAND(ID_UPDATE,		OnUpdate)
	ON_COMMAND(ID_VIEW_INPUTDLG,		OnViewInputdlg)
	ON_COMMAND(ID_VIEW_PARAMETERDLG,	OnViewParameterdlg)
	ON_COMMAND(ID_VIEW_PLAYDLG,         OnViewPlaydlg)
	ON_COMMAND(ID_VIEW_OPTION,			OnViewOption)
	ON_COMMAND(ID_OPERATE_FINDDOMAIN,	OnOperateFinddomain)
	ON_COMMAND(ID_OPERATE_UPDATE,		OnOperateUpdate)
	ON_COMMAND(ID_OPERATE_DELETE,		OnOperateDelete)
	ON_COMMAND(ID_OPERATE_SELECTALL,	OnOperateSelectall)
	ON_COMMAND(ID_FILE_FOUND_DOMAIN_RESULT, OnFileFoundDomainResult)
	ON_UPDATE_COMMAND_UI(ID_VIEW_INPUTDLG,		OnUpdateViewInputdlg)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PARAMETERDLG,	OnUpdateViewParameterdlg)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PLAYDLG,	OnUpdateViewPlaydlg)
	ON_UPDATE_COMMAND_UI(ID_FILE_FOUND_DOMAIN_RESULT, OnUpdateExportResult)
	ON_UPDATE_COMMAND_UI(ID_OPERATE_FINDDOMAIN, OnUpdateOperateFinddomain)
	ON_UPDATE_COMMAND_UI(ID_OPERATE_UPDATE,		OnUpdateOperateUpdate)
	ON_UPDATE_COMMAND_UI(ID_OPERATE_DELETE,		OnUpdateOperateDelete)
	ON_UPDATE_COMMAND_UI(ID_OPERATE_SELECTALL,	OnUpdateOperateSelectall)
	ON_COMMAND(ID_VIEW_MAGNIFY_100, OnViewMagnify100)
	ON_COMMAND(ID_VIEW_MAGNIFY_200, OnViewMagnify200)
	ON_COMMAND(ID_VIEW_MAGNIFY_150, OnViewMagnify150)
	ON_COMMAND(ID_VIEW_MAGNIFY_125, OnViewMagnify125)
	ON_COMMAND(ID_VIEW_MAGNIFY_075, OnViewMagnify075)
	ON_COMMAND(ID_VIEW_MAGNIFY_050, OnViewMagnify050)
	ON_COMMAND(ID_VIEW_MAGNIFY_025, OnViewMagnify025)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAGNIFY_200, OnUpdateViewMagnify)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAGNIFY_150, OnUpdateViewMagnify)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAGNIFY_125, OnUpdateViewMagnify)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAGNIFY_100, OnUpdateViewMagnify)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAGNIFY_075, OnUpdateViewMagnify)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAGNIFY_050, OnUpdateViewMagnify)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAGNIFY_025, OnUpdateViewMagnify)
	ON_COMMAND(ID_VIEW_DETAIL, OnViewDetail)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create the toolbar.\n");
		return -1;
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create the status bar.\n");
		return -1;
	}
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


void CMainFrame::OnAddTrajectory(void)
{
	m_wndInput.ShowWindow(SW_SHOW);
}

void CMainFrame::FindDomain(void)
{
	CWinTILDDoc *pDoc = GETDOCUMENT();

	pDoc->Calculate();

	Invalidate(TRUE);
	UpdateWindow();
}


void CMainFrame::Update(void)
{
	CWinTILDDoc *pDoc = GETDOCUMENT();
	pDoc->UpdateParameter(&m_wndProgress);
}

void CMainFrame::OnFindDomain(void)
{
	FindDomain();
}

void CMainFrame::OnUpdate(void)
{
	Update();
}

CWinTILDDoc* CMainFrame::GetActiveDocument(void)
{
	return (CWinTILDDoc*)CFrameWnd::GetActiveDocument();
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	m_wndInput.Create(this);
	m_wndParameter.Create(this);
	m_wndPlay.Create(this);

	CRect rcWindow;
	GetWindowRect( &rcWindow );

	CRect rcInput;
	m_wndInput.GetWindowRect( &rcInput );
	m_wndInput.MoveWindow( rcInput+CPoint( 0 , 20 ) );

	CRect rcParam;
	m_wndParameter.GetWindowRect( rcParam );
	m_wndParameter.MoveWindow( rcParam+CPoint(rcWindow.Width(), 0)-CPoint(rcParam.Width(),0)+CPoint(0,20 ) );
	m_wndParameter.ShowWindow(SW_SHOW);

	m_wndProgress.Create(this);
	m_wndProgress.CenterWindow(this);
	m_wndProgressList.Create(this);
	m_wndProgressList.CenterWindow(this);

	m_wndOptionSheet.Construct("Option",this);
	m_wndOptionSheet.AddPage( &m_wndGeneralProperty );
	m_wndOptionSheet.AddPage( &m_wndTrajectoryProperty );
	m_wndOptionSheet.AddPage( &m_wndDivMSDProperty );
	

	m_wndDetailSheet.Construct("Detail",this);
	m_wndDetailSheet.AddPage( &m_wndDomainProperty );

	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to create the splitter.\n");
		return FALSE;
	}

	if (!m_wndSplitter.CreateView(0, 0,
		RUNTIME_CLASS(CResultView), CSize(150, 0), pContext))
	{
		TRACE0("Failed to create CResultView.\n");
		return FALSE;
	}

	if (!m_wndSplitter.CreateView(0, 1,
		RUNTIME_CLASS(CWinTILDView), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create CWinTILDView.\n");
		return FALSE;
	}

	SetActiveView((CView*)m_wndSplitter.GetPane(0, 0));

	return TRUE;
}

CInputDlg* CMainFrame::GetInputDlg(void)
{
	return &m_wndInput;
}

CProgressDlg* CMainFrame::GetProgressDlg(void)
{
	return &m_wndProgress;
}

void CMainFrame::OnViewInputdlg()
{
	m_wndInput.ShowWindow(m_wndInput.IsWindowVisible()?SW_HIDE:SW_SHOW);
}

void CMainFrame::OnViewParameterdlg()
{
	m_wndParameter.ShowWindow(m_wndParameter.IsWindowVisible()?SW_HIDE:SW_SHOW);
}

void CMainFrame::OnViewPlaydlg()
{
	m_wndPlay.ShowWindow(m_wndPlay.IsWindowVisible()?SW_HIDE:SW_SHOW);
}

void CMainFrame::OnUpdateViewInputdlg(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( m_wndInput.IsWindowVisible()?1:0 );
}

void CMainFrame::OnUpdateViewParameterdlg(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( m_wndParameter.IsWindowVisible()?1:0 );
}

void CMainFrame::OnUpdateViewPlaydlg(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( m_wndPlay.IsWindowVisible()?1:0 );
}

void CMainFrame::OnUpdateExportResult(CCmdUI *pCmdUI)
{
	CWinTILDDoc *pDoc = GetActiveDocument();
	if (pDoc == NULL)
		return;
	pCmdUI->Enable( pDoc->GetResult()->GetSize() > 0 );
}

void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
	CWinTILDDoc *pDoc = GETDOCUMENT();

	const UINT nFilename = 1024;
	char filename[ nFilename ];
	const int fileCount = DragQueryFile( hDropInfo, -1, filename , nFilename );
	CStringList sl;

	for(int i = 0; i < fileCount; i++) {
		DragQueryFile(hDropInfo, i, filename, nFilename );
		sl.AddTail( filename );
	}	

//	CFrameWnd::OnDropFiles(hDropInfo);
}


void CMainFrame::OnViewOption()
{
	m_wndOptionSheet.DoModal();
}


void CMainFrame::OnViewDetail()
{
	if ( ::IsWindowVisible( m_wndDetailSheet.m_hWnd ) ){
		m_wndDetailSheet.DestroyWindow();
	}
	else {
		m_wndDetailSheet.Create(this);
	}
}

void CMainFrame::UpdateViewOption(void)
{
	if ( m_wndDomainProperty && ::IsWindowVisible( m_wndDomainProperty.m_hWnd ) ){
		m_wndDomainProperty.Update();
	}

}

void CMainFrame::OnOperateFinddomain()
{
	FindDomain();
}

void CMainFrame::OnUpdateOperateFinddomain(CCmdUI *pCmdUI)
{
	CWinTILDDoc *pDoc = GETDOCUMENT();
	CArray<int> *pSII = pDoc->GetSelectedInputIndex();
	BOOL enable = (pSII->GetCount() != 0);
	pCmdUI->Enable( enable );
}


void CMainFrame::OnOperateUpdate()
{
	Update();
}

void CMainFrame::OnUpdateOperateUpdate(CCmdUI *pCmdUI)
{
	CWinTILDDoc *pDoc = GETDOCUMENT();
	CArray<int> *pITU = pDoc->GetIndexesToUpdate();
	BOOL enable = (pITU->GetCount() != 0);
	pCmdUI->Enable( enable );
}

void CMainFrame::OnOperateDelete()
{
	CWinTILDDoc *pDoc = GETDOCUMENT();
	pDoc->DeleteSelelctedResult();
}

void CMainFrame::OnUpdateOperateDelete(CCmdUI *pCmdUI)
{
	CWinTILDDoc *pDoc = GETDOCUMENT();
	CArray<int> *pITU = pDoc->GetIndexesToUpdate();
	BOOL enable = (pITU->GetCount() != 0);
	pCmdUI->Enable( enable );
}

void CMainFrame::OnOperateSelectall()
{
	CWinTILDDoc *pDoc = GETDOCUMENT();
	const int count = pDoc->GetResult()->GetSize();
	CArray<int> indexes;
	for (int i=0; i<count; i++){
		indexes.Add(i);
	}
	pDoc->SelectToUpdate(indexes);
	pDoc->UpdateAllViews(NULL,UPDATE_RESULTVIEW,NULL);
}

void CMainFrame::OnUpdateOperateSelectall(CCmdUI *pCmdUI)
{
	CWinTILDDoc *pDoc = GETDOCUMENT();
	const int count = pDoc->GetResult()->GetSize();
	BOOL enable = (count!=0);
	pCmdUI->Enable( enable );
}

void CMainFrame::OnViewMagnify100()
{
	CWinTILDDoc *pDoc = GETDOCUMENT();
	pDoc->SetDivMSDMagnify(1.00);
}

void CMainFrame::OnViewMagnify200()
{
	CWinTILDDoc *pDoc = GETDOCUMENT();
	pDoc->SetDivMSDMagnify(2.00);
}

void CMainFrame::OnViewMagnify150()
{
	CWinTILDDoc *pDoc = GETDOCUMENT();
	pDoc->SetDivMSDMagnify(1.50);
}

void CMainFrame::OnViewMagnify125()
{
	CWinTILDDoc *pDoc = GETDOCUMENT();
	pDoc->SetDivMSDMagnify(1.25);
}

void CMainFrame::OnViewMagnify075()
{
	CWinTILDDoc *pDoc = GETDOCUMENT();
	pDoc->SetDivMSDMagnify(0.75);
}

void CMainFrame::OnViewMagnify050()
{
	CWinTILDDoc *pDoc = GETDOCUMENT();
	pDoc->SetDivMSDMagnify(0.50);
}

void CMainFrame::OnViewMagnify025()
{
	CWinTILDDoc *pDoc = GETDOCUMENT();
	pDoc->SetDivMSDMagnify(0.25);
}

void CMainFrame::OnUpdateViewMagnify(CCmdUI * pCmdUI)
{
	CWinTILDDoc *pDoc = GETDOCUMENT();
	BOOL enable = (pDoc->GetResult()->GetSize() != 0);
	pCmdUI->Enable(enable);	
}

void CMainFrame::OnFileFoundDomainResult()
{
	CWinTILDDoc *pDoc = GETDOCUMENT();
	CString txt = "duration";
	CFileDialog dlg(
		FALSE,
		"txt",
		txt,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"Plane Text (*.txt)|*.txt||",
		this);
	if ( IDOK == dlg.DoModal() ){
		pDoc->ExportFoundDomain( dlg.GetPathName() );
	}
}

