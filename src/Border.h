#pragma once
#include "FileMappedDynamicArray.h"

class CBorder :
	public CFileMappedDynamicArray<int>
{
public:
	CBorder(void);
	~CBorder(void);
	void Export(CFile& file);
	void Import(CFile& file);
	void Serialize(CArchive &ar);
};
