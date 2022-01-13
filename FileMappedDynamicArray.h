#pragma once

#include "FileMappedArray.h"

template <typename T>
class CFileMappedDynamicArray
{
private:
	enum {
		EXTRA_ELEMENT_SIZE = 100		// the size of extra elements
	};

	CFileMappedArray<T> *m_array;		// the pointer to an array
	unsigned __int64 m_size;			// the size of elements, which a user determines
	unsigned __int64 m_sizeInner;		// the actual size of elements

	void Initialize(void);
	void ResizeInner(unsigned __int64 size, bool updateSize, bool copy);

public:
	CFileMappedDynamicArray(void);
	CFileMappedDynamicArray(unsigned __int64 size);
	CFileMappedDynamicArray(const CFileMappedDynamicArray<T> &source);
	virtual ~CFileMappedDynamicArray(void);
	void Resize(unsigned __int64 size);
	void SetSize(unsigned __int64 size);
	void Copy(const CFileMappedDynamicArray<T> &source);
	void RemoveAll(void);
	void Add(const T &element);
	void RemoveAt(unsigned __int64 index);
	unsigned __int64 GetSize(void) const;
	unsigned __int64 GetCount(void) const;
	bool IsEmpty(void) const;
	void SetAt(unsigned __int64 index, const T &element);
	const T &GetAt(unsigned __int64 index) const;
	T &GetAt(unsigned __int64 index);
	const T &operator[](unsigned __int64 index) const;
	T &operator[](unsigned __int64 index);
	const CFileMappedDynamicArray<T> &operator=(const CFileMappedDynamicArray<T> &source);
};

template <typename T>
inline void CFileMappedDynamicArray<T>::Initialize(void)
{
	m_array = NULL;
	m_size = 0;
	m_sizeInner = 0;
}

template <typename T>
inline CFileMappedDynamicArray<T>::CFileMappedDynamicArray(void)
{
	Initialize();
}

template <typename T>
inline CFileMappedDynamicArray<T>::CFileMappedDynamicArray(unsigned __int64 size)
{
	Initialize();
	ResizeInner(size, true, false);
}

template <typename T>
inline CFileMappedDynamicArray<T>::CFileMappedDynamicArray(const CFileMappedDynamicArray<T> &source)
{
	Initialize();
	Copy(source);
}

template <typename T>
inline CFileMappedDynamicArray<T>::~CFileMappedDynamicArray(void)
{
	RemoveAll();
}

// This function is for internal use.
// @param updateSize : Does ResizeInner updates m_size?
// @param copy : Does ResizeInner copies the old contents?
template <typename T>
inline void CFileMappedDynamicArray<T>::ResizeInner(unsigned __int64 size, bool updateSize, bool copy)
{
	CFileMappedArray<T> *p(new CFileMappedArray<T>(size));
	if (m_array) {
		if (copy) {
			p->Copy(*m_array);
		}
		delete m_array;
	}
	m_array = p;
	m_sizeInner = size;
	if (updateSize) {
		m_size = size;
	}
}

template <typename T>
inline void CFileMappedDynamicArray<T>::Resize(unsigned __int64 size)
{
	ResizeInner(size, true, true);
}

template <typename T>
inline void CFileMappedDynamicArray<T>::SetSize(unsigned __int64 size)
{
	ResizeInner(size, true, true);
}

template <typename T>
inline void CFileMappedDynamicArray<T>::Copy(const CFileMappedDynamicArray<T> &source)
{
	if (source.GetSize() == 0) {
		RemoveAll();
	} else {
		ResizeInner(source.m_sizeInner, false, false);
		m_size = source.m_size;
		m_array->Copy(*source.m_array);
	}
}

template <typename T>
inline void CFileMappedDynamicArray<T>::RemoveAll(void)
{
	if (m_array) {
		delete m_array;
		m_array = NULL;
	}
	m_size = 0;
	m_sizeInner = 0;
}

template <typename T>
inline void CFileMappedDynamicArray<T>::Add(const T &element)
{
	if (m_size >= m_sizeInner) {
		ResizeInner(m_sizeInner + EXTRA_ELEMENT_SIZE, false, true);
	}
	m_array->SetAt(m_size++, element);
}

template <typename T>
inline void CFileMappedDynamicArray<T>::RemoveAt(unsigned __int64 index)
{
	unsigned __int64 i;

	for (i = index + 1; i < m_size; ++i) {
		m_array->SetAt(i - 1, m_array->GetAt(i));
	}
	--m_size;
}

template <typename T>
inline unsigned __int64 CFileMappedDynamicArray<T>::GetSize(void) const
{
	return m_size;
}

template <typename T>
inline unsigned __int64 CFileMappedDynamicArray<T>::GetCount(void) const
{
	return m_size;
}

template <typename T>
inline bool CFileMappedDynamicArray<T>::IsEmpty(void) const
{
	return m_size == 0;
}

template <typename T>
inline void CFileMappedDynamicArray<T>::SetAt(unsigned __int64 index, const T &element)
{
	ASSERT(m_size > 0);
	m_array->SetAt(index, element);
}

template <typename T>
inline const T &CFileMappedDynamicArray<T>::GetAt(unsigned __int64 index) const
{
	ASSERT(m_size > 0);
	return m_array->GetAt(index);
}

template <typename T>
inline T &CFileMappedDynamicArray<T>::GetAt(unsigned __int64 index)
{
	ASSERT(m_size > 0);
	return m_array->GetAt(index);
}

template <typename T>
inline const T &CFileMappedDynamicArray<T>::operator[](unsigned __int64 index) const
{
	return GetAt(index);
}

template <typename T>
inline T &CFileMappedDynamicArray<T>::operator[](unsigned __int64 index)
{
	return GetAt(index);
}

template <typename T>
inline const CFileMappedDynamicArray<T> &CFileMappedDynamicArray<T>::operator=(const CFileMappedDynamicArray<T> &source)
{
	Copy(source);
	return *this;
}
