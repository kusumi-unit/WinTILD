#include "stdafx.h"
#include "WinTILD.h"
#include "WinTILDDoc.h"
#include "InputDlg.h"

#include <algorithm>


IMPLEMENT_DYNAMIC(CInputDlg, CDialog)
CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDlg::IDD, pParent)
{
	m_Lock = FALSE;
}

CInputDlg::~CInputDlg()
{
}

void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_INPUT_FROM, m_SliderFrom);
	DDX_Control(pDX, IDC_SLIDER_INPUT_TO, m_SliderTo);
	DDX_Control(pDX, IDC_SPIN_INPUT_FROM, m_SpinFrom);
	DDX_Control(pDX, IDC_SPIN_INPUT_TO, m_SpinTo);
	DDX_Control(pDX, IDC_EDIT_INPUT_FROM, m_EditFrom);
	DDX_Control(pDX, IDC_EDIT_INPUT_TO, m_EditTo);
}

CWinTILDDoc* CInputDlg::GetDocument(){
	if (theApp.m_pMainWnd==NULL)return NULL;
	return (CWinTILDDoc*)((CFrameWnd*)theApp.m_pMainWnd)->GetActiveDocument();
};


BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnBnClickedButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnBnClickedButtonDown)
	ON_LBN_SELCANCEL(IDC_LIST_INPUT, OnLbnSelcancelListInput)
	ON_LBN_SELCHANGE(IDC_LIST_INPUT, OnLbnSelchangeListInput)
	ON_WM_HSCROLL()
    ON_EN_CHANGE(IDC_EDIT_INPUT_FROM, OnEnChangeEditInputFrom)
	ON_EN_CHANGE(IDC_EDIT_INPUT_TO, OnEnChangeEditInputTo)
	ON_BN_CLICKED(IDC_CHECK_DELETEAFTER, OnBnClickedCheckDeleteafter)
	ON_BN_CLICKED(IDC_CHECK_DONTSHOW, OnBnClickedCheckDontshow)

    ON_EN_CHANGE(IDC_EDIT_SMOOTHING, OnEnChangeEditSmoothing)
END_MESSAGE_MAP()


BOOL CInputDlg::Create( CWnd* pParentWnd )
{
	return CDialog::Create(IDD_INPUT_DIALOG,  pParentWnd );
}

void CInputDlg::OnBnClickedButtonAdd()
{
	CFileDialog dlg(TRUE,".dat",NULL
		,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT
		,"trajectory files(*.dat)|*.dat|all files(*.*)|*.*||",this );
	if( IDOK== dlg.DoModal() ){
		CListBox* pList = GetListBox();
		POSITION pos = dlg.GetStartPosition();
		CString str;
		while (pos) {
			str = dlg.GetNextPathName(pos);
			pList->AddString( LPCTSTR(str) );
		}
		UpdateListWidth();
		UpdateDocList();
	}
}

void CInputDlg::UpdateListWidth()
{
	TEXTMETRIC tm;
	CListBox* pList = GetListBox();

	CString str;
	CDC* pDC = pList->GetDC();
	pDC->GetTextMetrics(&tm); 
	CFont* pFont = pList->GetFont();
	CFont* pOldFont = pDC->SelectObject(pFont);

	int result=0;

	for (int i=0; i<pList->GetCount(); i++){
		pList->GetText(i,str);
		CSize sz = pDC->GetTextExtent(str);
		sz.cx += tm.tmAveCharWidth;
		if (result < sz.cx)
			result = sz.cx;
	
	}

	pList->SetHorizontalExtent(result);

	pDC->SelectObject(pOldFont);
	pList->ReleaseDC(pDC);
}

void CInputDlg::OnDropFiles(HDROP hDropInfo)
{
	const UINT nFilename = 1024;
	TCHAR filename[ nFilename ];
    const int fileCount = DragQueryFile(hDropInfo, -1, filename, nFilename);

	CListBox * plb = GetListBox();
	for(int i = 0; i < fileCount; i++) {
		DragQueryFile(hDropInfo, i, filename, nFilename );
		plb->AddString( filename );
	}

	CDialog::OnDropFiles(hDropInfo);

	DragFinish( hDropInfo );

	UpdateListWidth();
	UpdateDocList();
}

void CInputDlg::OnBnClickedButtonRemove()
{
	CListBox* plb = GetListBox();
	int count = plb->GetSelCount();
	int * selected = new int[count];
	plb->GetSelItems( count , selected );
	for (int i=count-1; i>=0; i--){
		plb->DeleteString(selected[i]);
	}
	delete[] selected;

	UpdateListWidth();
	UpdateDocList();
}

void CInputDlg::OnBnClickedButtonClear()
{
	CListBox* plb = GetListBox();
	plb->ResetContent();
	UpdateListWidth();
	UpdateDocList();
}

void CInputDlg::OnBnClickedButtonUp()
{
	CListBox* pList = GetListBox();
	int count = pList->GetCount();
	bool *SelIndex = new bool[count];
	for (int i=0; i<count; i++){
		if (pList->GetSel(i) > 0){ // swap items
			SwapListString(i-1,i);
			if (i-1>=0)SelIndex[i-1] = TRUE;
			SelIndex[i] = FALSE;
		}
		else {
			SelIndex[i] = FALSE;
		}
	}
	for (int i=0; i<count; i++){ // select items which are selected
		pList->SetSel(i,SelIndex[i]);
	}
	delete[] SelIndex;

	UpdateListWidth();
	UpdateDocList();
}

void CInputDlg::OnBnClickedButtonDown()
{
	CListBox* pList = GetListBox();
	int count = pList->GetCount();
	bool *SelIndex = new bool[count];
	for (int i=count-1; i>=0; i--){
		if (pList->GetSel(i) > 0){
			SwapListString(i,i+1);
			SelIndex[i] = FALSE;
			if ( i+1<count )SelIndex[i+1] = TRUE;
		}
		else {
			SelIndex[i] = FALSE;
		}
	}
	for (int i=0; i<count; i++){
		pList->SetSel(i,SelIndex[i]);
	}
	delete[] SelIndex;

	UpdateListWidth();
	UpdateDocList();
}

void CInputDlg::SwapListString(int index1,int index2)
{
	CListBox* pList = GetListBox();
	if (index1 < 0 || pList->GetCount() <= index1 || index2 < 0 || pList->GetCount() <= index2)return;
	if (index1==index2)return;
	CString string1,string2;
	pList->GetText(index1,string1);
	pList->GetText(index2,string2);
	pList->DeleteString(index1);
	pList->InsertString(index1,string2);
	pList->DeleteString(index2);
	pList->InsertString(index2,string1);
}

void CInputDlg::UpdateDocList()
{
	CWinTILDDoc* pDoc = GetDocument();
	if(pDoc == NULL)
		return;
	CStringArray  *pSL = pDoc->GetInputFilename();
	CArray<int> *pSII = pDoc->GetSelectedInputIndex();
	CListBox *pLB = GetListBox();

	CString str;
	int count = pLB->GetCount();

	pSL->RemoveAll();
	pSII->RemoveAll();
	for(int i = 0; i < count; i++) {
		pLB->GetText(i, str);
		pSL->Add(str);
		if(pLB->GetSel(i) > 0) {
			pSII->Add(i);
		}
	}

	auto pData = pDoc->SetPreview();
	int max;
	if(!pData || pData->GetTrajectory() == NULL) {
		max = 0;
	} else {
		max = static_cast<int>(pData->GetTrajectory()->GetSize()) - 1;
	}

	UpdateControlRange(0, max);

	Invalidate(FALSE);
}

void CInputDlg::ResetSmoothing() {
    auto spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_SMOOTHING);
    spin->SetPos(constants::MIN_SMOOTHING);
}

void CInputDlg::OnLbnSelcancelListInput()
{
	if ( !Locking() ){
		UpdateDocList();
	}
}

void CInputDlg::OnLbnSelchangeListInput()
{
	if ( !Locking() ){
		UpdateDocList();
	}
}

BOOL CInputDlg::OnInitDialog() {
	CDialog::OnInitDialog();

	m_SpinFrom.SetBuddy( &m_EditFrom );
	m_SpinTo.SetBuddy( &m_EditTo );

    { // Initialize components for the smoothing value
        CEdit* edit_smoothing = (CEdit*)GetDlgItem(IDC_EDIT_SMOOTHING);
        SetDlgItemInt(IDC_EDIT_SMOOTHING, constants::MIN_SMOOTHING);
        CSpinButtonCtrl* spin_smoothing = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_SMOOTHING);
        spin_smoothing->SetRange(constants::MIN_SMOOTHING, 10); // there is no reason for choosing 10
        spin_smoothing->SetBuddy(edit_smoothing);
    }

	UpdateControlRange(0, 0);
	((CButton*)GetDlgItem(IDC_CHECK_DELETEAFTER))->SetCheck(BST_CHECKED);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CInputDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	static char buff[64];
	if ( pScrollBar == (CScrollBar*)&m_SliderFrom ){
		m_EditFrom.SetWindowText( itoa( m_SliderFrom.GetPos(), buff, 10 ) );
	}
	else if ( pScrollBar == (CScrollBar*)&m_SliderTo ){
		m_EditTo.SetWindowText( itoa( m_SliderTo.GetPos(), buff, 10 ) );
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CInputDlg::OnEnChangeEditInputFrom()
{
	if ( !m_Lock ){
		TextUpdate();
		CWinTILDDoc *pDoc = GetDocument();
		if (pDoc!=NULL){
			pDoc->SetPreviewTargetFrame( GetDlgItemInt(IDC_EDIT_INPUT_FROM) , GetDlgItemInt(IDC_EDIT_INPUT_TO) );
		}
	}
}

void CInputDlg::OnEnChangeEditInputTo()
{
	if ( !m_Lock ){
		TextUpdate();
		CWinTILDDoc *pDoc = GetDocument();
		if (pDoc!=NULL){
			pDoc->SetPreviewTargetFrame( GetDlgItemInt(IDC_EDIT_INPUT_FROM) , GetDlgItemInt(IDC_EDIT_INPUT_TO) );
		}
	}
}

void CInputDlg::OnEnChangeEditSmoothing() {
    if (m_Lock) return;

    CWinTILDDoc* pDoc = GetDocument();
    if (pDoc) {
        pDoc->SetSmoothing(GetDlgItemInt(IDC_EDIT_SMOOTHING));
    }
}

void CInputDlg::TextUpdate(void)
{
	int value = GetDlgItemInt(IDC_EDIT_INPUT_TO) - GetDlgItemInt(IDC_EDIT_INPUT_FROM) + 1;
	int frames = std::max( value  ,0) ;
	CString str;
	str.Format("%8d frames",frames);
	SetDlgItemText(IDC_STATIC_FRAMES, str);
}

void CInputDlg::UpdateControlRange(int min, int max)
{
	char buffer[256];
	m_EditFrom.SetWindowText( itoa( min, buffer, 10 ) );
	m_EditTo.SetWindowText( itoa( max, buffer, 10 ) );
	m_SpinFrom.SetRange32(min,max);
	m_SpinTo.SetRange32(min,max);
	m_SliderFrom.SetRange(min,max);
	m_SliderFrom.SetPos(0);
	m_SliderTo.SetRange(min,max);
	m_SliderTo.SetPos(max);
}

void CInputDlg::GetTargetFrame(int& start, int& end)
{
	start = GetDlgItemInt(IDC_EDIT_INPUT_FROM);
	end = GetDlgItemInt(IDC_EDIT_INPUT_TO);
	
	if (start < end)
		end = start;
}

void CInputDlg::OnBnClickedCheckDeleteafter()
{
	CWinTILDDoc *pDoc = GetDocument();
	if (pDoc == NULL)
		return;
	pDoc->DeleteAfterCalculation( ((CButton*)GetDlgItem(IDC_CHECK_DELETEAFTER))->GetCheck() == BST_CHECKED );
}

void CInputDlg::OnBnClickedCheckDontshow()
{
	CWinTILDDoc *pDoc = GetDocument();
	if (pDoc == NULL)
		return;
	pDoc->DontShowMessage( ((CButton*)GetDlgItem(IDC_CHECK_DONTSHOW))->GetCheck() == BST_CHECKED );
}


void CInputDlg::UpdateDlgList(void)
{
	Lock();

	CWinTILDDoc* pDoc = GetDocument();
	if (pDoc == NULL)
		return;
	CStringArray  *pSL = pDoc->GetInputFilename();
	CListBox *pLB = GetListBox();

	CString str;

	pLB->ResetContent();
	for (int i=0; i<pSL->GetSize(); i++){
		pLB->AddString( pSL->GetAt(i) );
	}

	UpdateControlRange( 0 , 0);

	Unlock();

	Invalidate(FALSE);
}

void CInputDlg::Lock()
{
	m_Lock = TRUE;
	LockWindowUpdate();
};

void CInputDlg::Unlock()
{
	UnlockWindowUpdate();
	m_Lock = FALSE;
};
