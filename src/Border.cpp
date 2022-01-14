#include "StdAfx.h"
#include "border.h"

CBorder::CBorder(void)
{
}

CBorder::~CBorder(void)
{
}

void CBorder::Export(CFile& file)
{
	unsigned __int64 s;
	s = GetSize();
	file.Write(&s,sizeof(s));
	if (s > 0) {
		for (unsigned __int64 i = 0; i < s; ++i) {
			int element(GetAt(i));
			file.Write(&element, sizeof(element));
		}
	}
}

void CBorder::Import(CFile& file)
{
	RemoveAll();

	unsigned __int64 s;
	file.Read(&s,sizeof(s));

	if (s > 0) {
		for (unsigned __int64 i = 0; i < s; ++i) {
			int element;
			file.Read(&element, sizeof(element));
			SetAt(i, element);
		}
	}
}

void CBorder::Serialize(CArchive &ar)
{
	if (ar.IsStoring()) {
		unsigned __int64 s;
		s = GetSize();
		ar.Write(&s, sizeof(s));
		if (s > 0) {
			for (unsigned __int64 i = 0; i < s; ++i) {
				int element(GetAt(i));
				ar.Write(&element, sizeof(element));
			}
		}
	} else {
		RemoveAll();

		unsigned __int64 s;
		ar.Read(&s, sizeof(s));
		Resize(s);
		if (s > 0) {
			for (unsigned __int64 i = 0; i < s; ++i) {
				int element;
				ar.Read(&element, sizeof(element));
				SetAt(i, element);
			}
		}
	}
}
