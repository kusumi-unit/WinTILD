// CalculationThread.cpp : �����t�@�C��
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
	// TODO:  �X���b�h���Ƃ̏������������Ŏ��s���܂��B
	return TRUE;
}

int CCalculationThread::ExitInstance()
{
	// TODO:  �X���b�h���Ƃ̌㏈���������Ŏ��s���܂��B
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCalculationThread, CWinThread)
END_MESSAGE_MAP()


// CCalculationThread ���b�Z�[�W �n���h��
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
