#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0A00
#endif

#define _WIN32_WINNT 0x0A00


#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0400
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#define _AFX_ALL_WARNINGS

#include <afxwin.h>
#include <afxext.h>
#include <afxdisp.h>

#include <afxdtctl.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxcview.h>
#include <afxdlgs.h>
#include <afxdhtml.h>
#include <afx.h>
#include <atlstr.h>
#include <atlimage.h>

#undef min
#undef max

// the length of format id in an identifier(FORMATIDENT_CDATA)
const size_t FORMATINDENT_LENGTH = 4;
// the maximum length of an identifier
const char IDENTSIZE = 10;
// Identifiers: the first 4 characters indicate the kind of data, and the rest indicates the version.
// Document identifier
const char FORMATIDENT_CDOC[ IDENTSIZE ] = "PROJ01";
// CData identifier
const char FORMATIDENT_CDATA[ IDENTSIZE ] = "DATA02";

#define INTERVAL 20
#define PLOTSIZE 300
#define FLOATMAGNIFY 1e4
#define MARGIN 60
