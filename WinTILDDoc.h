#include "DataArray.h"
#include "afxcoll.h"
#include "progressdlg.h"
#include "InformationPlotterOption.h"
#include "TrajectoryPlotterOption.h"
#include "DivMSDPlotterOption.h"
#include "GeneralOption.h"

#pragma once

#define UPDATE_PREVIEW			0x1
#define UPDATE_ADDRESULT		0x2
#define UPDATE_CHANGELISTITEM	0x4
#define UPDATE_THRESHOLD		0x8
#define UPDATE_LOCKVIEWS		0x10
#define UPDATE_UNLOCKVIEWS		0x20
#define UPDATE_NEWDOCUMENT		0x40
#define UPDATE_WINDOWSIZE		0x80
#define UPDATE_UPDATEPARAMETER	0x100
#define UPDATE_SKIP				0x200
#define UPDATE_PARAMETER		0x400
#define UPDATE_USEAVERAGE		0x800
#define UPDATE_IMPORT			0x1000
#define UPDATE_DELETERESULT		0x2000
#define UPDATE_PRINT			0x4000
#define UPDATE_OPTION			0x8000
#define UPDATE_PLOTAREA			0x10000
#define UPDATE_RESULTVIEW		0x20000
#define UPDATE_MAGNIFY			0x40000
#define UPDATE_REDRAW			0x80000

class CWinTILDDoc : public CDocument
{
protected:
	CWinTILDDoc();
	DECLARE_DYNCREATE(CWinTILDDoc)

private:
	// The first index where datum are imported
	int m_ImportedIndexFirst;

protected:
	// The results of calculation
	CDataArray m_Result;
	// Filenames input in the input dialog
	CStringArray m_InputFilename;
	// Indices selected in the input file list
	CArray<int> m_SelectedInputIndex;
	// Trajectory for preview
	CDataPtr m_Preview;
	// A range of the target frame (preview)
	int m_TargetFrameStart;
	int m_TargetFrameEnd;
    // A smoothing value used to calculate the moving average (preview)
    int m_Smoothing;
	int m_SelectedIndex;

	// Do we remove processed files after calculation?
	BOOL m_bDeleteAfterCalculation;
	// If true, do not display the confirmation message
	BOOL m_bDontShowMessage;
	// Indicies selected in the input list (this is used to update)
	CArray<int> m_IndexesToUpdate;
	// If true, recalculate domains on every update of parameters
	BOOL m_Interactive;
	// If true, draw border lines
	BOOL m_DrawBoundaries;

	int m_Interval;

	CTrajectoryPlotterOption  m_TrajectoryPlotterOption;
	CInformationPlotterOption m_InformationPlotterOption;
	CDivMSDPlotterOption      m_DivMSDPlotterOption;

	CGeneralOption m_GeneralOption;

public:
	// Calculate the scroll size from the Plotter's parameter
	CSize CalculateScrollSize(void);
	double GetMagnify()const{return m_DivMSDPlotterOption.m_Magnify; };
	void SetDivMSDMagnify(double m);

public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

public:
	virtual ~CWinTILDDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

protected:
	DECLARE_MESSAGE_MAP()
protected:
	void UpdateParameterDlg(CDataPtr const& data);
	void UpdateViewOption();
public:
	void LoadTrajectory(LPCTSTR filename);
	// Return filenames in the input list
	CStringArray* GetInputFilename(void);
	// Update the trajectory data for preview
	CDataPtr SetPreview();

    CDataArray* GetResult() { return &m_Result; };

	// Calculate MSD
	void Calculate(void);
	// Select and Display the data in a given position
    // @return : the selected data
	CDataPtr Select(int index);
	void SetPreviewTargetFrame(int start, int end);

    void SetSmoothing(int smoothing);

	void SetThreshold(double threshold);
	void SetMaximum(double maximum);
	CParameter* GetCurrentParameter(void);
	void SetWindowStart(int frame);
	void SetWindowEnd(int frame);
	void SetSkip(int frames);
	void SetRatio(double ratio);

    // Set the start frame to calculate the average value
	void SetAverageFrom(int from);
    // Set the end frame to calculate the average value
    void SetAverageTo(int to);
	// Return the number of frames in a trajectory file
	int CheckFrames(const LPCTSTR filename);
	void DeleteAfterCalculation(bool del);
	BOOL DeleteAfterCalculation(){ return m_bDeleteAfterCalculation; };

    void DontShowMessage(bool dontshow);

    // Set target indices to update
	void SelectToUpdate(const CArray<int>& indexes);

    // Recalculate domains with the current parameter
	void UpdateParameter(CProgressDlg* pdlg = NULL);

    // Return indices to update
	CArray<int>* GetIndexesToUpdate(void);

    // If interactive update is enabled, this method calls UpdateParameter
	BOOL InteractiveUpdate(void);
	void SetInteractiveUpdate(BOOL interactive);

	void SetDrawBoundaries(BOOL draw_boundaries);

    // Return the selected data
	CDataPtr GetSelectedData(void);

    // Return indices selected in the input list
	CArray<int>* GetSelectedInputIndex(void);

    // Export detected domains
	void ExportFoundDomain(LPCTSTR filename);
	void ExportFoundDomainOneResult(LPCTSTR filename, int index);
	void BeginImport(void);
	void EndImport(void);
	void Import(LPCTSTR filename);
	void ImportResult(CArchive& ar);
	// Delete the selected result
	void DeleteSelelctedResult(void);

	CTrajectoryPlotterOption*  GetTrajectoryPlotterOption(){ return &m_TrajectoryPlotterOption;};
	CInformationPlotterOption* GetInformationPlotterOption(){return &m_InformationPlotterOption;};
	CDivMSDPlotterOption*      GetDivMSDPlotterOption(){ return &m_DivMSDPlotterOption; };
	CGeneralOption*			   GetGeneralOption(){ return &m_GeneralOption; };

	void UpdateOptions(void);
	void SelectDomain(int index);
	void UpdatePlotArea(void);
	// Load .ini/Registory
	void LoadProfiles(void);
	// Save .ini/Registory
	void SaveProfiles(void);
	// Request to redraw the view
	void Redraw(void);
	void AutoAdjustArea(void);
protected:
	void UpdatePlayDlg(CDataPtr data);
};


