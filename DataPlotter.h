#pragma once
#include "plotter.h"
#include "data.h"
#include "border.h"


class CDataPlotter :
	public CPlotter
{
protected:
	CData *m_pData;
	CBorder *m_pBorder;
	RGBQUAD m_clThreshold; // �������l�ȉ��̐F
	RGBQUAD m_Palette[256];

	//�\����̍ő�E�ŏ��l
	double m_Max;
	double m_Min;
	float m_MaxPercent;
	float m_MinPercent;

	//�������l�����ۂ̒l�Ŏw��
	double m_Threshold;

public:

	CDataPlotter(void);
	~CDataPlotter(void);
/*
	virtual void Plot(CDC* pDC);
	// �\�����e��ݒ�
	void SetCalculation(CCalculation* pC);
	// percent <=1.0
	void SetMaxPercent(float percent);
	void SetMinPercent(float percent);

	// �\�������̃T�C�Y�ɍ��킹��
	void SetMagnification(CDC *pDC, float mx = 1.0, float my = 1.0);
	void SetThreshold(double threshold);

	//�䗦���炵�����l�����߂�
	double CalcThreshold(double percent);
*/
};
