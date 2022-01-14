#pragma once

#include "ColorStatic.h"
#include "BrushStatic.h"
#include "PenStatic.h"

void DDX_Color(CDataExchange *pDX, CColorStatic &member, RGBQUAD &color);
void DDX_Color(CDataExchange *pDX, CColorStatic &member, COLORREF &color);
void DDX_Brush(CDataExchange *pDX, CBrushStatic &member, LOGBRUSH &brush);
void DDX_Pen  (CDataExchange *pDX, CPenStatic &member, LOGPEN &pen);
void DDX_Font (CDataExchange *pDX, CStatic &member, LOGFONT &font);
void DDX_Status(CDataExchange *pDX, CWnd* pCheck, unsigned long mask, unsigned long &status);
void DDX_BkMode(CDataExchange *pDX, CWnd* check, int &bkMode);
