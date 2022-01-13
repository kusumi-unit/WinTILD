#include "stdafx.h"
#include "WinTILD.h"
#include "MainFrm.h"

#include "WinTILDDoc.h"
#include "WinTILDView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinTILDApp

BEGIN_MESSAGE_MAP(CWinTILDApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN, CWinTILDApp::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


CWinTILDApp::CWinTILDApp()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}


CWinTILDApp theApp;


BOOL CWinTILDApp::InitInstance()
{
	InitCommonControls();

	CWinApp::InitInstance();

	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
    free((void *)m_pszProfileName);
    m_pszProfileName = GetIniFileName();
	LoadStdProfileSettings(4);
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CWinTILDDoc),
		RUNTIME_CLASS(CMainFrame),
		RUNTIME_CLASS(CWinTILDView));
	AddDocTemplate(pDocTemplate);

    EnableShellOpen();
	RegisterShellFileTypes(TRUE);

    CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

    if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

    m_pMainWnd->DragAcceptFiles();
	return TRUE;
}


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

void CWinTILDApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


TCHAR* CWinTILDApp::GetIniFileName()const
{
	TCHAR drive[8],dir[512],name[512],ext[64],buff[1024];
	GetModuleFileName(m_hInstance, buff, 1024);
 	_tsplitpath(buff,drive,dir,name,ext);
	_tmakepath(buff, drive, dir, name, _T("ini"));
	return(_tcsdup(buff));
}

void DeleteExtension(CString& path)
{
	char drive[8],dir[512],name[512],ext[64],buff[1024];
	_splitpath(path,drive,dir,name,ext);
	_makepath(buff,drive,dir,name,NULL);
	path = buff;
}

CString ExtractFileName(const CString& path)
{
	static CString filename;
	char drive[4];
	char dir[512];
	char name[512];
	char ext[32];
	_splitpath(path,drive,dir,name,ext);
	filename.Format("%s%s",name,ext);
	return filename;
}
