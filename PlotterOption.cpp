#include "StdAfx.h"
#include "plotteroption.h"

CPlotterOption::CPlotterOption(void)
{
	m_Margin.Bottom = m_Margin.Top = m_Margin.Left = m_Margin.Right = 0;
	
	m_lpszSection = "PLOT";
}

CPlotterOption::~CPlotterOption(void)
{
}

void CPlotterOption::GetPlotArea(CRect& rc)const
{
	rc = m_Area;
	rc.top+=m_Margin.Top;
	rc.left+=m_Margin.Left;
	rc.bottom-=m_Margin.Bottom;
	rc.right-=m_Margin.Right;
}

void CPlotterOption::SetMargin(int l, int t, int r, int b)
{
	m_Margin.Bottom = b;
	m_Margin.Left = l;
	m_Margin.Right = r;
	m_Margin.Top = t;
}

void CPlotterOption::LoadProfiles(CWinApp *pApp)
{
	m_Status = pApp->GetProfileInt( m_lpszSection, "Status", -1 );
	m_bkMode = pApp->GetProfileInt( m_lpszSection, "BkMode", TRANSPARENT);
	m_bkModeText = pApp->GetProfileInt( m_lpszSection, "BkModeText", TRANSPARENT);

	LPBYTE pByte(NULL);
	UINT size;

	pApp->GetProfileBinary( m_lpszSection,"Background",&pByte,&size );
	if ( size == sizeof(COLORREF) ){
		memcpy( &m_clBackground, pByte, size );
	}
	else {
		SetDefaultBackground();
	}
	if (pByte) {
		delete [] pByte;
		pByte = NULL;
	}

	pApp->GetProfileBinary( m_lpszSection,"BackgroundText",&pByte,&size );
	if ( size == sizeof(COLORREF) ){
		memcpy( &m_clBackgroundText, pByte, size );
	}
	else {
		SetDefaultBackgroundText();
	}
	if (pByte) {
		delete [] pByte;
		pByte = NULL;
	}

	pApp->GetProfileBinary( m_lpszSection,"Margin",&pByte,&size );
	if ( size == sizeof(LOGBRUSH) ){
		memcpy( &m_Margin, pByte, size );
	}
	else {
		SetDefaultMargin();
	}
	if (pByte) {
		delete [] pByte;
		pByte = NULL;
	}

	m_Area.left   = pApp->GetProfileInt( m_lpszSection, "Area_Left",  0 );
	if ( m_Area.left != 0 ){
		m_Area.right  = pApp->GetProfileInt( m_lpszSection, "Area_Right", 0 );
		m_Area.top    = pApp->GetProfileInt( m_lpszSection, "Area_Top",   0 );
		m_Area.bottom = pApp->GetProfileInt( m_lpszSection, "Area_Bottom",0 );
	}
	else {
		SetDefaultArea();
	}


	pApp->GetProfileBinary( m_lpszSection,"LB_Background",&pByte,&size );
	if ( size == sizeof(m_lbPlotArea) ){
		memcpy( &m_lbPlotArea, pByte, size );
	}
	else {
		SetDefaultLBBackGround();
	}
	if (pByte) {
		delete [] pByte;
		pByte = NULL;
	}

	pApp->GetProfileBinary( m_lpszSection,"LB_Margin",&pByte,&size );
	if ( size == sizeof(m_lbMargin) ){
		memcpy( &m_lbMargin, pByte, size );
	}
	else {
		SetDefaultLBMargin();
	}
	if (pByte) {
		delete [] pByte;
		pByte = NULL;
	}

	pApp->GetProfileBinary( m_lpszSection,"LP_Frame",&pByte,&size );
	if ( size == sizeof(m_lpFrame) ){
		memcpy( &m_lpFrame, pByte, size );
	}
	else {
		SetDefaultLPFrame();
	}
	if (pByte) {
		delete [] pByte;
		pByte = NULL;
	}

	pApp->GetProfileBinary( m_lpszSection,"LF_Text",&pByte,&size );
	if ( size == sizeof(m_lfText) ){
		memcpy( &m_lfText, pByte, size );
	}
	else {
		m_lfText.lfHeight = 16;
		m_lfText.lfWidth = 0;
		m_lfText.lfEscapement = 0;
		m_lfText.lfOrientation = 0;
		m_lfText.lfWeight = FW_NORMAL; 
		m_lfText.lfItalic =	FALSE;
		m_lfText.lfUnderline = FALSE;
		m_lfText.lfStrikeOut = FALSE;
		m_lfText.lfCharSet =  ANSI_CHARSET;
		m_lfText.lfOutPrecision = OUT_DEFAULT_PRECIS;
		m_lfText.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		m_lfText.lfQuality = DEFAULT_QUALITY;
		m_lfText.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
		strcpy( m_lfText.lfFaceName , "Arial");
	}
	if (pByte) {
		delete [] pByte;
		pByte = NULL;
	}

	pApp->GetProfileBinary( m_lpszSection,"LF_Legend",&pByte,&size );
	if ( size == sizeof(m_lfLegend) ){
		memcpy( &m_lfLegend, pByte, size );
	}
	else {
		m_lfLegend	= m_lfText;
		m_lfLegend.lfHeight = 18;
	}
	if (pByte) {
		delete [] pByte;
		pByte = NULL;
	}

	pApp->GetProfileBinary( m_lpszSection,"LP_Line1",&pByte,&size );
	if ( size == sizeof(m_lpLine1) ){
		memcpy( &m_lpLine1, pByte, size );
	}
	else {
		SetDefaultLPLine1();
	}
	if (pByte) {
		delete [] pByte;
		pByte = NULL;
	}
	pApp->GetProfileBinary( m_lpszSection,"LP_Line2",&pByte,&size );
	if ( size == sizeof(m_lpLine2) ){
		memcpy( &m_lpLine2, pByte, size );
	}
	else {
		SetDefaultLPLine2();
	}
	if (pByte) {
		delete [] pByte;
		pByte = NULL;
	}
	pApp->GetProfileBinary( m_lpszSection,"LP_Line3",&pByte,&size );
	if ( size == sizeof(m_lpLine3) ){
		memcpy( &m_lpLine3, pByte, size );
	}
	else {
		SetDefaultLPLine3();
	}
	if (pByte) {
		delete [] pByte;
		pByte = NULL;
	}
	pApp->GetProfileBinary( m_lpszSection,"LP_Line4",&pByte,&size );
	if ( size == sizeof(m_lpLine4) ){
		memcpy( &m_lpLine4, pByte, size );
	}
	else {
		SetDefaultLPLine4();
	}
	if (pByte) {
		delete [] pByte;
		pByte = NULL;
	}
	pApp->GetProfileBinary( m_lpszSection,"LP_Line5",&pByte,&size );
	if ( size == sizeof(m_lpLine5) ){
		memcpy( &m_lpLine5, pByte, size );
	}
	else {
		SetDefaultLPLine5();
	}
	if (pByte) {
		delete [] pByte;
		pByte = NULL;
	}

}

void CPlotterOption::SaveProfiles(CWinApp *pApp)
{
	pApp->WriteProfileInt( m_lpszSection, "Status", m_Status );
	pApp->WriteProfileInt( m_lpszSection, "BkMode", m_bkMode);
	pApp->WriteProfileInt( m_lpszSection, "BkModeText", m_bkModeText);
	pApp->WriteProfileBinary( m_lpszSection, "Background", (LPBYTE)&m_clBackground, sizeof(m_clBackground));
	pApp->WriteProfileBinary( m_lpszSection, "BackgroundText", (LPBYTE)&m_clBackgroundText, sizeof(m_clBackgroundText));

	pApp->WriteProfileInt( m_lpszSection, "Area_Left",   m_Area.left );
	pApp->WriteProfileInt( m_lpszSection, "Area_Right",  m_Area.right );
	pApp->WriteProfileInt( m_lpszSection, "Area_Top",    m_Area.top );
	pApp->WriteProfileInt( m_lpszSection, "Area_Bottom", m_Area.bottom );

	pApp->WriteProfileBinary( m_lpszSection,"Margin",       (LPBYTE)&m_Margin, sizeof(m_Margin) );
	pApp->WriteProfileBinary( m_lpszSection,"LB_Background",(LPBYTE)&m_lbPlotArea,sizeof(m_lbPlotArea) );
	pApp->WriteProfileBinary( m_lpszSection,"LB_Margin",    (LPBYTE)&m_lbMargin,    sizeof(m_lbMargin) );
	pApp->WriteProfileBinary( m_lpszSection,"LP_Frame",     (LPBYTE)&m_lpFrame,     sizeof(m_lpFrame)   );
	pApp->WriteProfileBinary( m_lpszSection,"LF_Text",      (LPBYTE)&m_lfText,      sizeof(m_lfText) );
	pApp->WriteProfileBinary( m_lpszSection,"LF_Legend",    (LPBYTE)&m_lfLegend,    sizeof(m_lfLegend) );
	pApp->WriteProfileBinary( m_lpszSection,"LP_Line1",     (LPBYTE)&m_lpLine1,     sizeof(m_lpLine1) );
	pApp->WriteProfileBinary( m_lpszSection,"LP_Line2",     (LPBYTE)&m_lpLine2,     sizeof(m_lpLine2) );
	pApp->WriteProfileBinary( m_lpszSection,"LP_Line3",     (LPBYTE)&m_lpLine3,     sizeof(m_lpLine3) );
	pApp->WriteProfileBinary( m_lpszSection,"LP_Line4",     (LPBYTE)&m_lpLine4,     sizeof(m_lpLine4) );
	pApp->WriteProfileBinary( m_lpszSection,"LP_Line5",     (LPBYTE)&m_lpLine5,     sizeof(m_lpLine5) );
}

void CPlotterOption::SetDefaultMargin()
{
	m_Margin.Top = 0;
	m_Margin.Left = 0;
	m_Margin.Bottom = 0;
	m_Margin.Right = 0;
}

void CPlotterOption::SetDefaultArea()
{
	m_Area.top = 0;
	m_Area.left = 0;
	m_Area.bottom = 0;
	m_Area.right = 0;
}

void CPlotterOption::SetDefaultLBBackGround()
{
	m_lbPlotArea.lbColor = GetSysColor( COLOR_WINDOW );
	m_lbPlotArea.lbHatch = HS_DIAGCROSS;
	m_lbPlotArea.lbStyle = BS_SOLID;
}

void CPlotterOption::SetDefaultLBMargin()
{
	m_lbMargin.lbColor = GetSysColor( COLOR_APPWORKSPACE );
	m_lbMargin.lbHatch = HS_DIAGCROSS;
	m_lbMargin.lbStyle = BS_SOLID;
}

void CPlotterOption::SetDefaultLPFrame()
{
	m_lpFrame.lopnColor = GetSysColor( COLOR_ACTIVECAPTION );
	m_lpFrame.lopnStyle = PS_SOLID;
	m_lpFrame.lopnWidth = CPoint(1,0);
}

void CPlotterOption::SetDefaultBackground()
{
	m_clBackground = GetSysColor( COLOR_WINDOW );
} 

void CPlotterOption::SetDefaultBackgroundText()
{
	m_clBackgroundText = GetSysColor( COLOR_WINDOW );
}

void CPlotterOption::SetDefaultLPLine1()
{
	m_lpLine1.lopnColor = 0x00FF0000;
	m_lpLine1.lopnStyle = PS_SOLID;
	m_lpLine1.lopnWidth = CPoint(1,0);
}
void CPlotterOption::SetDefaultLPLine2()
{
	m_lpLine2.lopnColor = 0x00FFA000;
	m_lpLine2.lopnStyle = PS_SOLID;
	m_lpLine2.lopnWidth = CPoint(1,0);
}
void CPlotterOption::SetDefaultLPLine3()
{
	m_lpLine3.lopnColor = 0x0000E000;
	m_lpLine3.lopnStyle = PS_SOLID;
	m_lpLine3.lopnWidth = CPoint(1,0);
}
void CPlotterOption::SetDefaultLPLine4()
{
	m_lpLine4.lopnColor = 0x0000A0FF;
	m_lpLine4.lopnStyle = PS_SOLID;
	m_lpLine4.lopnWidth = CPoint(1,0);
}
void CPlotterOption::SetDefaultLPLine5()
{
	m_lpLine5.lopnColor = 0x000000FF;
	m_lpLine5.lopnStyle = PS_SOLID;
	m_lpLine5.lopnWidth = CPoint(1,0);
}

void CPlotterOption::SetState(unsigned long status, unsigned long mask)
{
	m_Status = (status & mask) | (m_Status & ~mask);
}