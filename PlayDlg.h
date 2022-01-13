#pragma once

#include "WinTILDDoc.h"


class FRECT
{
public:
	float top;
	float bottom;
	float right;
	float left;
	float getCenterX() { return (right + left)/2;}
	float getCenterY() { return (top + bottom)/2;}
	float getWidth() { return (right - left); }
	float getHeight() { return (bottom - top); }
};


class CPlayDlg : public CDialog
{
	DECLARE_DYNAMIC(CPlayDlg)

public:
	CPlayDlg(CWnd* pParent = NULL);
	virtual ~CPlayDlg();

	enum { IDD = IDD_PLAY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

private:
	CWinTILDDoc* GetDocument(void);
	CDataPtr m_data;
	FRECT m_dataArea;
	int m_frameStart;
	int m_frameEnd;
	int m_frameCurrent;
	static UINT AnimationThread(LPVOID pParam);
	static UINT AnimationBackThread(LPVOID pParam);

	BOOL m_animationThreadAlive;
	CPoint m_moveBeforePoint;
	BOOL m_isLButtonDowned;
	CPoint m_previewDelta;
	RECT mPreviewRect;
	float mScale;
protected:
	void SetWindow(int start, int end);
	void setCurrentFrame(int currentFrame);
public:
	static const int FPS_MAX = 120;
	static const int FPS_MIN = 1;
	static const int FPS_DEFAULT = 10;
	static const int PREVIEW_MARGIN = 15;
	static const int MAX_SCALE = 10;
	virtual BOOL OnInitDialog();
	virtual BOOL Create( CWnd* pParentWnd = NULL);
	void SetData(CDataPtr data);


public:

	afx_msg void OnDeltaposPlaySpinFps(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposPlaySpinCurrentframe(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedPlayButtonPlay();
	afx_msg void OnBnClickedPlayButtonStop();
	afx_msg void OnBnClickedPlayButtonPlaybackward();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
