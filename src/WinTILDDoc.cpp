#include "stdafx.h"
#include "WinTILD.h"
#include "MainFrm.h"
#include "WinTILDDoc.h"
#include "DivMSDPlotter.h"
#include "CalculationThread.h"

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CInputDlg* GetInputDlg(void);
CParameterDlg* GetParameterDlg(void);
CProgressDlg* GetProgressDlg();
CProgressListDlg* GetProgressListDlg();
CDomainProperty* GetDomainProperty();
CDivMSDProperty* GetDivMSDProperty();
CPlayDlg* GetPlayDlg();

// Prameters for multi thread processing
typedef struct tag_threadcalculate_parameter{
	CData* m_pData;
	LPCTSTR m_Filename;
	int m_TargetStart,m_TargetEnd;
	CParameter *m_pParameter;
	HWND m_hWndProgress;
	HANDLE m_hMutex;
}CThreadCalculateParameter;

volatile int countThread = 0;

UINT ThreadCalculate(LPVOID p)
{
	CThreadCalculateParameter *_p = (CThreadCalculateParameter*)p;
	try{

		_p->m_pData->GetDivMSD()->SetMessageHandle( _p->m_hWndProgress );
        _p->m_pData->GetTrajectory()->CalcMovingAverage(_p->m_pParameter->GetSmoothing());
		_p->m_pData->Calculate( _p->m_Filename, _p->m_TargetStart, _p->m_TargetEnd );
		_p->m_pData->SetParameter( *_p->m_pParameter );
		_p->m_pData->UpdateAverage();
		_p->m_pData->FindDomain();
	}
	catch (...){
		::WaitForSingleObject(_p->m_hMutex, INFINITE);
		countThread--;
		::ReleaseMutex(_p->m_hMutex);
		throw;
	}
	::WaitForSingleObject(_p->m_hMutex, INFINITE);
	countThread--;
	::ReleaseMutex(_p->m_hMutex);

	return 0;
}

// CWinTILDDoc

IMPLEMENT_DYNCREATE(CWinTILDDoc, CDocument)

BEGIN_MESSAGE_MAP(CWinTILDDoc, CDocument)
END_MESSAGE_MAP()



CWinTILDDoc::CWinTILDDoc()
{
	m_TargetFrameStart = 0;
	m_TargetFrameEnd = 0;
	m_SelectedIndex = -1;
	m_bDeleteAfterCalculation = TRUE;
	m_bDontShowMessage = FALSE;
	m_Interactive = FALSE;

	m_Interval = INTERVAL;

	LoadProfiles();
}

CWinTILDDoc::~CWinTILDDoc()
{
	SaveProfiles();
}

BOOL CWinTILDDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_Result.Clear();
	m_InputFilename.RemoveAll();
	m_IndexesToUpdate.RemoveAll();
	m_SelectedInputIndex.RemoveAll();
	m_Preview = MakeCDataPtr();
	m_TargetFrameStart = 0;
	m_TargetFrameEnd = 0;
    m_Smoothing = constants::MIN_SMOOTHING;
	m_SelectedIndex = -1;
    if (auto dlg = GetInputDlg(); dlg) {
        dlg->UpdateDlgList();
        dlg->ResetSmoothing();
    }
    UpdateAllViews(NULL, UPDATE_NEWDOCUMENT, NULL);
	UpdateViewOption();

	return TRUE;
}


CInputDlg* GetInputDlg(void)
{
	if (theApp.m_pMainWnd!=NULL){
		return ((CMainFrame*)theApp.m_pMainWnd)->GetInputDlg();
	}
	return NULL;
}

CParameterDlg* GetParameterDlg(void)
{
	if (theApp.m_pMainWnd!=NULL){
		return ((CMainFrame*)theApp.m_pMainWnd)->GetParameterDlg();
	}
	return NULL;
}

CProgressDlg* GetProgressDlg()
{
	if (theApp.m_pMainWnd!=NULL){
		return ((CMainFrame*)theApp.m_pMainWnd)->GetProgressDlg();
	}
	return NULL;
}

CProgressListDlg* GetProgressListDlg()
{
	if (theApp.m_pMainWnd!=NULL){
		return ((CMainFrame*)theApp.m_pMainWnd)->GetProgressListDlg();
	}
	return NULL;
}

CPlayDlg* GetPlayDlg()
{
	if (theApp.m_pMainWnd != NULL) {
		return ((CMainFrame*)theApp.m_pMainWnd)->GetPlayDlg();
	}
	return NULL;
}

CDomainProperty* GetDomainProperty()
{
	if (theApp.m_pMainWnd!=NULL){
		return ((CMainFrame*)theApp.m_pMainWnd)->GetDomainProperty();
	}
	return NULL;
}

CDivMSDProperty* GetDivMSDProperty()
{
	if (theApp.m_pMainWnd!=NULL){
		return ((CMainFrame*)theApp.m_pMainWnd)->GetDivMSDProperty();
	}
	return NULL;
}

void SetPos(int pos)
{
	((CMainFrame*)theApp.m_pMainWnd)->GetProgressDlg()->SetPos(pos);
}


void CWinTILDDoc::Serialize(CArchive& ar)
{
	try{
		char identCount;
		int count;

		if (ar.IsStoring())
		{
			ar.Write( &IDENTSIZE,sizeof(IDENTSIZE) );
			ar.Write( &FORMATIDENT_CDOC, IDENTSIZE );
			count = m_Result.GetSize();

			ar.Write( &count, sizeof(count) );
			for (int i=0; i<count ;i++){
				m_Result[i]->Serialize(ar);
			}
		}
		else
		{
			char buffer[ IDENTSIZE ];
			ar.Read( &identCount,sizeof(IDENTSIZE) );
			if (identCount != IDENTSIZE )
				throw CString("Unsupported format.");

			ar.Read( buffer, identCount );
			if ( strcmp( buffer, FORMATIDENT_CDOC ) != 0 )
				throw CString("Incompatible version.");

			ar.Read( &count,sizeof(count) );
			for (int i=0; i<count ;i++){
				auto pData = MakeCDataPtr();
				pData->Serialize(ar);
				m_Result.Add(pData);
			}
			UpdateAllViews(NULL,UPDATE_LOCKVIEWS,NULL);
			UpdateAllViews(NULL,UPDATE_IMPORT,NULL);
            if (m_Result.GetSize() != 0) { // Select the first trajectory
                Select(0);
            }
			UpdateAllViews(NULL,UPDATE_UNLOCKVIEWS,NULL);
			UpdateParameter();
		}
	}
	catch (CString s) {
		AfxMessageBox(s,MB_OK,0);
	}


}


#ifdef _DEBUG
void CWinTILDDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWinTILDDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


void CWinTILDDoc::LoadTrajectory(LPCTSTR filename)
{
	m_Result.AddTrajectory( filename );
}

CStringArray* CWinTILDDoc::GetInputFilename(void)
{
	return &m_InputFilename;
}

CDataPtr CWinTILDDoc::SetPreview()
{
	m_SelectedIndex = -1;
	if (m_SelectedInputIndex.IsEmpty())
		return NULL;

	// Load a trajectory for preview
	CTrajectory *pTrajectory = m_Preview->GetTrajectory();
	pTrajectory->Load( m_InputFilename[ m_SelectedInputIndex[0] ] );

	// Load default parameters from the trajectory
    // @note : Do not change the smoothing value in this time
	CParameter *pParam = m_Preview->GetParameter();
	pParam->SetThreshold( 1.0 );
	pParam->SetMaximum( 3.0 );
	pParam->SetRatio( 0.20 );
	pParam->SetWindowStart( 0 );
	pParam->SetWindowEnd(static_cast<int>(pTrajectory->GetSize()) - 1);
	pParam->SetSkip( 20 );
	pParam->SetAverageFrom( 2 );
	pParam->SetAverageTo( 4 );
    pParam->SetSmoothing(m_Smoothing);
	pParam->SetDrawBoundaries(true);

    pTrajectory->CalcMovingAverage(m_Smoothing);

	UpdateAllViews(NULL,UPDATE_PREVIEW, &m_Preview);
	UpdateParameterDlg(m_Preview);
	UpdateViewOption();

	return m_Preview;
}

void CWinTILDDoc::SetPreviewTargetFrame(int start, int end)
{
	if (start > end){
		end = start;
	}
	m_TargetFrameStart = start;
	m_TargetFrameEnd = end;
	m_Preview->SetTargetFrames(start, end);

	CParameterDlg *pdlg = GetParameterDlg();
	int length = std::max(0, end - start);
	pdlg->Lock();
	pdlg->SetWindowRange(0, length);
	pdlg->SetWindow(0, length);
	pdlg->Unlock();

	UpdateAllViews(NULL, UPDATE_PREVIEW, &m_Preview);
}

void CWinTILDDoc::SetSmoothing(int smoothing) {
    m_Smoothing = smoothing;
    m_Preview->GetTrajectory()->CalcMovingAverage(m_Smoothing);
    m_Preview->GetParameter()->SetSmoothing(m_Smoothing);

    CParameterDlg* dlg = GetParameterDlg();
    dlg->Lock();
    dlg->SetSmoothing(m_Smoothing);
    dlg->Unlock();

    UpdateAllViews(NULL, UPDATE_PREVIEW, &m_Preview);
}

void CWinTILDDoc::Calculate(void)
{
	if (m_SelectedInputIndex.IsEmpty())
		return;
	if (m_TargetFrameEnd - m_TargetFrameStart <= 0)
		return;

	// prevent views from updating
	UpdateAllViews(NULL, UPDATE_LOCKVIEWS, NULL);

	CString filename;
	CParameter *pparam = GetCurrentParameter();
	int prevLastFrames = -1;
	int currenLastFrames = -1;
	int targetCount = static_cast<int>(m_SelectedInputIndex.GetSize());

	// Select the trajectory, which is selected first
	m_SelectedIndex = targetCount > 0 ? m_Result.GetSize() : -1;

	CWinThread *pWinThread;
	CThreadCalculateParameter *pTCP= new CThreadCalculateParameter[targetCount];
	// handles of running threads
	CArray<HANDLE> hThread;

	CProgressListDlg *pProgress = GetProgressListDlg();
	pProgress->ResetList();

	pProgress->ShowWindow(SW_SHOW);
	pProgress->Invalidate(FALSE);
	pProgress->UpdateWindow();

	int listIndex;
	HANDLE hMutex(::CreateMutex(NULL, TRUE, NULL));
	if (hMutex == NULL) {
		AfxMessageBox(_T("Failed to create a thread"), MB_OK | MB_ICONSTOP);
		return;
	}

	for ( int i = 0; i<targetCount;){
		filename = m_InputFilename[ m_SelectedInputIndex[i] ];
		currenLastFrames = CheckFrames( filename )-1;

		if ( m_bDontShowMessage == FALSE && prevLastFrames != -1 ){
			CString confirmMessage;
			if ( currenLastFrames  < m_TargetFrameEnd ){
				confirmMessage.Format( "%s\nThe last frame of the trajectory(%d)\nis smaller than\nthe end of the target frame(%d).\nConsider the end if the target frame = %d and continue to process.", filename, currenLastFrames, m_TargetFrameEnd, currenLastFrames);				
				AfxMessageBox( confirmMessage );
			}
			else if ( currenLastFrames != prevLastFrames ){
				confirmMessage.Format( "%s\nThe number of frames is different from the previous file.\nPrevious:%d Current:%d\nDo you process this file with the current target frame(from:%d to:%d)?",filename, prevLastFrames+1, currenLastFrames+1 , m_TargetFrameStart, m_TargetFrameEnd);
				if ( IDNO == AfxMessageBox( confirmMessage, MB_YESNO) ){
					m_SelectedInputIndex.RemoveAt(i);
					continue;
				}
			} 
		}
		prevLastFrames = currenLastFrames;

		auto pdata = MakeCDataPtr();
		listIndex = pProgress->AddText( ExtractFileName(filename) );
		pProgress->SetItemData(listIndex, pdata->GetDivMSD() );
		pProgress->Invalidate(FALSE);
		pProgress->UpdateWindow();

		pTCP[i].m_hWndProgress = pProgress->GetListBox()->GetSafeHwnd();
		pTCP[i].m_pData = pdata.Get();
		pTCP[i].m_Filename = filename;
		pTCP[i].m_TargetStart = m_TargetFrameStart;
		pTCP[i].m_TargetEnd = m_TargetFrameEnd;
		pTCP[i].m_pParameter = pparam;
		pTCP[i].m_hMutex = hMutex;
		//---	file reading was made not in a thread	---
		pTCP[i].m_pData->GetTrajectory()->Load(filename);

		// Invoke a thread
		countThread++;
		pWinThread = AfxBeginThread(::ThreadCalculate, pTCP+i );
		hThread.Add( pWinThread->m_hThread );

		m_Result.Add( pdata );

		i++;
	}
	::ReleaseMutex(hMutex);

	// Waiting threads
	MSG msg;
	for ( ; ; ) {
		::WaitForSingleObject(hMutex, INFINITE);
		if (countThread <= 0) {
			::ReleaseMutex(hMutex);
			break;
		}
		::ReleaseMutex(hMutex);
		Sleep(15);
		if ( PeekMessage(&msg,pProgress->m_hWnd, NULL,NULL,PM_NOREMOVE ) ){
			if (msg.message == WM_COMMAND && msg.wParam == ID_UPDATE_PROGRESS && msg.lParam == NULL){
				pProgress->GetListBox()->Invalidate(FALSE);
				pProgress->GetListBox()->UpdateWindow();
				PeekMessage(&msg,pProgress->m_hWnd,NULL,NULL,PM_REMOVE);
			}
		}
	}

	delete[] pTCP;
	::CloseHandle(hMutex);

	// Remove processed files
	if ( m_bDeleteAfterCalculation == TRUE ){
		for (int i = static_cast<int>(m_SelectedInputIndex.GetSize()) - 1; i >= 0; i--){
			m_InputFilename.RemoveAt( m_SelectedInputIndex[i] );
		}
		m_SelectedInputIndex.RemoveAll();
		CInputDlg* pidlg = GetInputDlg();
		if (pidlg!=NULL){
			pidlg->UpdateDlgList();
		}
	}
	pProgress->ShowWindow(SW_HIDE);

	Select( m_SelectedIndex );

	UpdateAllViews(NULL, UPDATE_ADDRESULT, NULL);
	UpdateAllViews(NULL, UPDATE_UNLOCKVIEWS, NULL);
}

CDataPtr CWinTILDDoc::Select(int index)
{
	m_SelectedIndex = index;
	m_IndexesToUpdate.RemoveAll();

	if ( index >= 0 ){
		m_IndexesToUpdate.Add(index);
	}

	auto pdata = (m_Result.InRange(index) ? m_Result.GetAt(index) : nullptr);
	UpdateAllViews(NULL, UPDATE_CHANGELISTITEM, &pdata);
	UpdateParameterDlg( pdata );
	UpdateViewOption();

	UpdatePlayDlg(pdata);
	return pdata;
}

void CWinTILDDoc::UpdateParameterDlg(CDataPtr const& data)
{
	if (!data) return;

	CParameter *p = data->GetParameter();
	if (theApp.m_pMainWnd != NULL){
		CParameterDlg *pdlg = GetParameterDlg();
		pdlg->Lock();
		pdlg->LockWindowUpdate();
		pdlg->SetThreshold( p->GetThreshold() );
		pdlg->SetMaximum(p->GetMaximum());
		pdlg->SetRatio( p->GetRatio() );
		int frames = data->GetTrajectory()->GetTargetFrames();
		pdlg->SetWindowRange( 0, frames-1 );
		pdlg->SetWindow( p->GetWindowStart(), p->GetWindowEnd() );
		pdlg->SetSkip( p->GetSkip() );
		pdlg->SetSmoothing(p->GetSmoothing());
		pdlg->SetAverageFrom( p->GetAverageFrom() );
		pdlg->SetAverageTo( p->GetAverageTo() );
		pdlg->SetDrawBoundaries( p->GetDrawBoundaries());
		pdlg->Unlock();
		pdlg->UnlockWindowUpdate();
		pdlg->Invalidate(FALSE);
		pdlg->UpdateWindow();
	}
}

void CWinTILDDoc::UpdatePlayDlg(CDataPtr data)
{
	if (!data) return;
	
	CPlayDlg* pdlg = GetPlayDlg();
	if (pdlg != NULL) {
		pdlg->SetData(data);
	}
}


CParameter* CWinTILDDoc::GetCurrentParameter(void)
{
	if (theApp.m_pMainWnd!=NULL){
		return ((CMainFrame*)theApp.m_pMainWnd)->GetParameterDlg()->GetParameter();
	}
	return NULL;
}

void CWinTILDDoc::SetThreshold(double threshold)
{
	auto pData = GetSelectedData();
	if (!pData) return;
	pData->GetParameter()->SetThreshold( threshold );

	UpdateAllViews( NULL, UPDATE_LOCKVIEWS );
	UpdateAllViews( NULL, UPDATE_THRESHOLD, &pData );
	InteractiveUpdate();
	UpdateAllViews( NULL, UPDATE_UNLOCKVIEWS );
}

void CWinTILDDoc::SetMaximum(double maximum)
{
	auto pData = GetSelectedData();
	if (!pData) return;
	pData->GetParameter()->SetMaximum( maximum );

	UpdateAllViews( NULL, UPDATE_LOCKVIEWS );
	UpdateAllViews( NULL);
	InteractiveUpdate();
	UpdateAllViews( NULL, UPDATE_UNLOCKVIEWS );
}

void CWinTILDDoc::SetWindowStart(int frame)
{
	auto pData = GetSelectedData();
	if (!pData) return;
	pData->GetParameter()->SetWindowStart( frame );

	UpdateAllViews( NULL, UPDATE_LOCKVIEWS );
	UpdateAllViews( NULL, UPDATE_WINDOWSIZE, &pData );
	InteractiveUpdate();
	UpdateAllViews( NULL, UPDATE_UNLOCKVIEWS );
}

void CWinTILDDoc::SetWindowEnd(int frame)
{
	auto pData = GetSelectedData();
	if (!pData) return;
	pData->GetParameter()->SetWindowEnd( frame );

	UpdateAllViews( NULL, UPDATE_LOCKVIEWS );
	UpdateAllViews( NULL, UPDATE_WINDOWSIZE, &pData );
	InteractiveUpdate();
	UpdateAllViews( NULL, UPDATE_UNLOCKVIEWS );
}

void CWinTILDDoc::SetSkip(int frames)
{
	auto pData = GetSelectedData();
	if (!pData) return;
	pData->GetParameter()->SetSkip( frames );

	UpdateAllViews( NULL, UPDATE_LOCKVIEWS );
	UpdateAllViews( NULL, UPDATE_SKIP, &pData );
	InteractiveUpdate();
	UpdateAllViews( NULL, UPDATE_UNLOCKVIEWS );
}

void CWinTILDDoc::SetAverageFrom(int from)
{
	auto pData = GetSelectedData();
	if (!pData) return;
	pData->GetParameter()->SetAverageFrom( from );
	pData->UpdateAverage();

	UpdateAllViews( NULL, UPDATE_LOCKVIEWS );
	UpdateAllViews( NULL, UPDATE_PARAMETER, &pData );
	InteractiveUpdate();
	UpdateAllViews( NULL, UPDATE_UNLOCKVIEWS );
}

void CWinTILDDoc::SetAverageTo(int to)
{
	auto pData = GetSelectedData();
	if (!pData) return;
	pData->GetParameter()->SetAverageTo( to );
	pData->UpdateAverage();

	UpdateAllViews( NULL, UPDATE_LOCKVIEWS );
	UpdateAllViews( NULL, UPDATE_PARAMETER, &pData );
	InteractiveUpdate();
	UpdateAllViews( NULL, UPDATE_UNLOCKVIEWS );
}

#define CHECKFRAMES_BUFFERSIZE 512
int CWinTILDDoc::CheckFrames(const LPCTSTR filename)
{
	static char buffer[ CHECKFRAMES_BUFFERSIZE ];

	FILE *fs = fopen(filename, "r");
	int count = 0;
	while (NULL != fgets(buffer, CHECKFRAMES_BUFFERSIZE, fs) ){
		count++;
	}
	fclose(fs);
	return count;
}

void CWinTILDDoc::DeleteAfterCalculation(bool del)
{
	m_bDeleteAfterCalculation = del;
}

void CWinTILDDoc::DontShowMessage(bool dontshow)
{
	m_bDontShowMessage = dontshow;
}

void CWinTILDDoc::SelectToUpdate(const CArray<int>& indexes)
{
	m_IndexesToUpdate.Copy( indexes );
}

void CWinTILDDoc::SetRatio(double ratio)
{
	auto pData = GetSelectedData();
	if (!pData) return;
	pData->GetParameter()->SetRatio( ratio );

	UpdateAllViews( NULL, UPDATE_LOCKVIEWS );
	InteractiveUpdate();
	UpdateAllViews( NULL, UPDATE_WINDOWSIZE, &pData );
	UpdateAllViews( NULL, UPDATE_UNLOCKVIEWS );
}

void CWinTILDDoc::UpdateParameter(CProgressDlg* pdlg)
{
	CString txt;

	if (pdlg != NULL)
		pdlg->ShowWindow(SW_SHOW);

	CParameter *pParameter = GetParameterDlg()->GetParameter();
	
	const int size = static_cast<int>(m_IndexesToUpdate.GetSize());
	for (int i=0; i<size; i++){
		auto pData = m_Result[ m_IndexesToUpdate[i] ];
		if (pdlg != NULL){
			txt.Format( "updating %d/%d\n%s", i+1, size, (char*)pData->GetFilename()->GetString() );
			pdlg->SetMessage(txt);
			pdlg->SetPos( (i+1)*100/size );
		}
		pData->FindDomain( pParameter );
	}
	UpdateAllViews(NULL,UPDATE_UPDATEPARAMETER,NULL);
	UpdateViewOption();

	if (pdlg != NULL)
		pdlg->ShowWindow(SW_HIDE);

}

// アップデート用インデックスを取得
CArray<int>* CWinTILDDoc::GetIndexesToUpdate(void)
{
	return &m_IndexesToUpdate;
}


BOOL CWinTILDDoc::InteractiveUpdate(void)
{
	if ( m_Interactive == TRUE ){
		UpdateParameter();
	}
	return m_Interactive;
}

void CWinTILDDoc::SetInteractiveUpdate(BOOL interactive)
{
	m_Interactive = interactive;
	InteractiveUpdate();
}

void CWinTILDDoc::SetDrawBoundaries(BOOL draw_boundaries){
	m_DrawBoundaries = draw_boundaries;
	if(m_DrawBoundaries) m_DivMSDPlotterOption.SetState( 0xFFFFFF, DP_BORDER);
	else  m_DivMSDPlotterOption.SetState( 0x000000, DP_BORDER);
	
	CParameter *pParam = m_Preview->GetParameter();
	pParam->SetDrawBoundaries(draw_boundaries);
	CParameterDlg *pldg = GetParameterDlg();
	if(pldg !=NULL)
		pldg->SetDrawBoundaries(draw_boundaries);
	UpdateParameter();
}


CDataPtr CWinTILDDoc::GetSelectedData(void)
{
	if(m_Result.InRange(m_SelectedIndex)) {
		return m_Result.GetAt(m_SelectedIndex);
	}
	return nullptr;
}

CArray<int>* CWinTILDDoc::GetSelectedInputIndex(void)
{
	return &m_SelectedInputIndex;
}

void CWinTILDDoc::BeginImport(void)
{
	m_ImportedIndexFirst = -1;
}

void CWinTILDDoc::EndImport(void)
{
	UpdateAllViews(NULL,UPDATE_LOCKVIEWS, NULL);
	auto pData = Select( m_ImportedIndexFirst );
	UpdateAllViews(NULL,UPDATE_IMPORT, &pData);
	UpdateAllViews(NULL,UPDATE_UNLOCKVIEWS, NULL);
	UpdateParameterDlg(pData);
	UpdateViewOption();
}

void CWinTILDDoc::ImportResult(CArchive& ar)
{
	auto pData = MakeCDataPtr();
	pData->Serialize(ar);

	int select = m_Result.Add( pData );
	if (m_ImportedIndexFirst == -1){
		m_ImportedIndexFirst = select;
	}
}

void CWinTILDDoc::Import(LPCTSTR filename)
{
	CFile file;
	if ( file.Open(filename,CFile::modeRead|CFile::shareDenyWrite) ){
		CArchive ar( &file, CArchive::load );
	
		char *ident = NULL;
		try {
			char size;
			ar.Read( &size, sizeof(size) );
			ident = new char[size];
			ar.Read( ident, size );

			ar.Flush();
			ar.GetFile()->Seek(0,CFile::begin);			

			if ( strncmp( ident, FORMATIDENT_CDOC , FORMATINDENT_LENGTH) == 0 ){
				Serialize( ar );
			}
			else if ( strncmp( ident, FORMATIDENT_CDATA, FORMATINDENT_LENGTH) == 0 ){
				ImportResult(ar);
			}
			else {
				AfxMessageBox("This file is not supported.",MB_OK,0);
			}
		}
		catch (CString s){
			AfxMessageBox(s,MB_OK,0);
		}
		if (ident)
			delete[] ident;
		ar.Close();
	}
	file.Close();
}

void CWinTILDDoc::ExportFoundDomain(LPCTSTR filename){
	CStdioFile stdFile;
	if(!stdFile.Open(filename, CFile::modeWrite | CFile::shareExclusive | CFile::modeCreate)){
		//---	error message	---
		CString strData;
		strData.Format("file: %s \ncannot be open\n", filename);
		AfxMessageBox(strData);
		return;
	}
	stdFile.Close();

	for(int i=0; i<m_Result.GetSize(); ++i)
		ExportFoundDomainOneResult(filename, i);
}
void CWinTILDDoc::ExportFoundDomainOneResult(LPCTSTR filename, int index){
	if(index >= m_Result.GetSize()) return;

	auto pData = (m_Result.InRange(index) ? m_Result.GetAt(index) : nullptr);
	CBorder *pBorder = pData->GetBorder();
	if (pBorder == NULL)
		return;

	CStdioFile stdFile;
	int err = 0;
	if(!err){
		if(!stdFile.Open(filename, CFile::modeWrite | CFile::shareExclusive | CFile::modeCreate | CFile::modeNoTruncate)) err = 1;
	}
	//---	error message	---
	if(err){
		CString strData;
		strData.Format("file: %s \ncannot be open\n", filename);
		AfxMessageBox(strData);
	}

    if(!err){
		stdFile.SeekToEnd();
		size_t domain=0, domain_pre=0;
		for(size_t i=0; i<pBorder->GetSize(); ++i){
			domain = pBorder->GetAt(i);
			CTrajectoryPoint tp = pData->getAvgCordinate(static_cast<int>(domain_pre) + 1, static_cast<int>(domain) - 1);
			CString str;
			str.Format("\"%s\" %d %d %lf %lf\n", _T(*pData->GetFilename()), domain, tp.frame+1, tp.x, tp.y);
			TRY {stdFile.WriteString(str);}
			CATCH (CFileException, eP) {err = 1;}
			END_CATCH
			domain_pre = domain;
		}
		domain = pData->GetTrajectory()->GetSize()-1;
		CTrajectoryPoint tp2 = pData->getAvgCordinate(domain_pre+1, domain-1);
		CString str;
		str.Format("\"%s\" %d %d %lf %lf\n", _T(*pData->GetFilename()), domain, tp2.frame+1, tp2.x, tp2.y);
		TRY {stdFile.WriteString(str);}
		CATCH (CFileException, eP) {err = 1;}
		END_CATCH
		domain_pre = domain;
	}

	stdFile.Close();
}

void CWinTILDDoc::DeleteSelelctedResult(void)
{
	for (int i = static_cast<int>(m_IndexesToUpdate.GetSize()) - 1; i >= 0; i--){
		m_Result.RemoveAt( m_IndexesToUpdate[i] );
	}
	m_IndexesToUpdate.RemoveAll();
	m_SelectedIndex = -1;
	UpdateAllViews(NULL,UPDATE_DELETERESULT,NULL);
	UpdateViewOption();
}

CSize CWinTILDDoc::CalculateScrollSize(void)
{
	CRect divrc;
	CRect inforc;
	m_DivMSDPlotterOption.GetArea( divrc );
	m_InformationPlotterOption.GetArea( inforc );
	return CSize( std::max(inforc.right, divrc.right) + m_Interval , divrc.bottom + m_Interval);
}

void CWinTILDDoc::UpdateOptions(void)
{
	UpdateAllViews(NULL, UPDATE_OPTION, NULL);
}

void CWinTILDDoc::UpdateViewOption()
{
	CDomainProperty* pDP = GetDomainProperty();
	CDivMSDProperty* pDMSDP = GetDivMSDProperty();
	if (pDP != NULL){
		if ( ::IsWindowVisible( pDP->m_hWnd ) ){
			pDP->Update();
		}
	}
	if (pDMSDP!=NULL){
		if ( ::IsWindowVisible( pDMSDP->m_hWnd ) ){
			pDMSDP->Update();
		}
	}
}

void CWinTILDDoc::SelectDomain(int index)
{
	GetTrajectoryPlotterOption()->SelectDomain(index);
	GetDivMSDPlotterOption()->SelectDomain(index);
	UpdateAllViews(NULL,UPDATE_OPTION,NULL);
}

void CWinTILDDoc::UpdatePlotArea(void)
{
	CRect rcTraj,rc;
	m_TrajectoryPlotterOption.GetArea(rcTraj);

	CRect rcInfo;
	m_InformationPlotterOption.GetArea(rcInfo);
	rcInfo.MoveToXY(rcTraj.Width() +2*m_Interval,m_Interval);
	m_InformationPlotterOption.SetArea( rcInfo );

	CRect rcDivMSD;
	m_DivMSDPlotterOption.GetArea(rcDivMSD);
	rcDivMSD.MoveToXY(m_Interval, std::max( rcTraj.bottom, rcInfo.bottom ) +m_Interval);
	m_DivMSDPlotterOption.SetArea( rcDivMSD );

	if(0 <= m_SelectedIndex && m_SelectedIndex < m_Result.GetSize()) {
		UpdateAllViews(NULL, UPDATE_PLOTAREA, &m_Result[m_SelectedIndex]);
	} else {
		UpdateAllViews(NULL, UPDATE_PLOTAREA, nullptr);
	}
}

void CWinTILDDoc::LoadProfiles(void)
{
	m_GeneralOption.LoadProfiles( &theApp );
	m_TrajectoryPlotterOption.LoadProfiles( &theApp );
	m_DivMSDPlotterOption.LoadProfiles( &theApp );
	m_InformationPlotterOption.LoadProfiles( &theApp );
}

void CWinTILDDoc::SaveProfiles(void)
{
	m_GeneralOption.SaveProfiles( &theApp );
	m_TrajectoryPlotterOption.SaveProfiles( &theApp );
	m_DivMSDPlotterOption.SaveProfiles( &theApp );
	m_InformationPlotterOption.SaveProfiles( &theApp );
}

void CWinTILDDoc::SetDivMSDMagnify(double m)
{
	BOOL oldFS = m_DivMSDPlotterOption.m_FixSize;
	m_DivMSDPlotterOption.m_FixSize = FALSE;
	m_DivMSDPlotterOption.m_Magnify = m;
	UpdateAllViews(NULL,UPDATE_MAGNIFY,NULL);
	m_DivMSDPlotterOption.m_FixSize = oldFS;
}

void CWinTILDDoc::Redraw(void)
{
	UpdateAllViews(NULL,UPDATE_REDRAW,NULL);
}

void CWinTILDDoc::AutoAdjustArea(void)
{
	CRect rcTraj = GetTrajectoryPlotterOption()->m_Area;
	CRect rcInfo = GetInformationPlotterOption()->m_Area;
	CRect rcDMSD = GetDivMSDPlotterOption()->m_Area;

	rcDMSD.top = rcTraj.bottom + m_Interval;
	rcDMSD.bottom = rcDMSD.top + GetDivMSDPlotterOption()->m_Area.Height();

	rcInfo.left = rcTraj.right + m_Interval;
	rcInfo.right = rcInfo.left + GetInformationPlotterOption()->m_Area.Width();

	GetTrajectoryPlotterOption()->m_Area = rcTraj;
	GetInformationPlotterOption()->m_Area= rcInfo;
	GetDivMSDPlotterOption()->m_Area	 = rcDMSD;
}


