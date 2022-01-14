#pragma once

#include "data.h"
#include "parameter.h"


// CCalculationThread

class CCalculationThread : public CWinThread
{
	DECLARE_DYNCREATE(CCalculationThread)

protected:
	CCalculationThread();           // 動的生成で使用される protected コンストラクタ
	virtual ~CCalculationThread();

	CData* m_pData;
	LPCTSTR m_Filename;
	int m_TargetStart,m_TargetEnd;
	BOOL m_bReverseScanning;
	BOOL m_UseAverage;
	CParameter *m_pParameter;

	void SetParameter( CData *pData, LPCTSTR Filename, int TargetStart, int TargetEnd,
		BOOL Reverse, BOOL UseAverage, CParameter *pParameter );

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


