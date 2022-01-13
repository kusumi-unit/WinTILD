// CalculationThread.cpp : 実装ファイル
//

#include "stdafx.h"
#include "WinDFR.h"
#include "CalculationThread.h"


// CCalculationThread

IMPLEMENT_DYNCREATE(CCalculationThread, CWinThread)

CCalculationThread::CCalculationThread()
{
}

CCalculationThread::~CCalculationThread()
{
}

BOOL CCalculationThread::InitInstance()
{
	// TODO:  スレッドごとの初期化をここで実行します。
	return TRUE;
}

int CCalculationThread::ExitInstance()
{
	// TODO:  スレッドごとの後処理をここで実行します。
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCalculationThread, CWinThread)
END_MESSAGE_MAP()


// CCalculationThread メッセージ ハンドラ
void
CCalculationThread::
SetParameter( CData *pData, LPCTSTR Filename, int TargetStart, int TargetEnd,
		BOOL Reverse, BOOL UseAverage, CParameter *pParameter )
{
	m_pData = pData;
	m_Filename = Filename;
	m_TargetStart = TargetStart;
	m_TargetEnd = TargetEnd;
	m_bReverseScanning = Reverse;
	m_UseAverage = UseAverage;
	m_pParameter = pParameter;
}
