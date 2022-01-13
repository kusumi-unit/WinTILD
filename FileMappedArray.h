#pragma once

#include <afx.h>
#include <cstring>
#include <new>

#ifdef CFILEMAPPEDARRAY_TEST
#include <cstdlib>
#include <ctime>
#define TEST_MAPPING_ELEMENT_COUNT 16
#endif

template <typename T>
class CFileMappedArray
{
private:
	enum {
        MAPPING_BASE_SIZE = 524288000UL, // The basic size of an allocated memory (currently, 500 Mbytes).
                                         // If the size of memory allocation exceeds this size, this class maps some contents to a file.
        SEGMENT_COUNT = 8UL,             // the size of segments
        SEGMENT_REFRESH_SCORE = 128UL,   // the score of a segment immediately after it loads a file
        MAX_FILE_NAME = 4096,            // the maximum length of a filename
        ERROR_NOERROR = 0,               // Error type: No error
        ERROR_MEMORYALLOCATION = 1,      // Error type: Failed to allocate a memory
        ERROR_FILE = 2,                  // Error type: Failed to create a file
	};

	unsigned int m_mappingSize;      // the size of data allocated in the logical memory
	unsigned int m_segmentSize;      // the size of a memory per segment
	mutable unsigned __int64 m_size; // the size of elements
	mutable unsigned __int64 m_currentSegment[SEGMENT_COUNT]; // segments existing in the memory
	mutable unsigned int m_segmentAccessScore[SEGMENT_COUNT]; // scores of a segment, which is determined by the number of accesses to the segment
	mutable bool m_usingFile;        // Is a file used?
	mutable LPBYTE m_buf;
	mutable CFile m_file;

	static CString CreateTemporaryFileName(void);
	void CreateMappingFile(const CString &fileName, unsigned __int64 bytes);
	void Initialize(unsigned __int64 size);
	CFileMappedArray(void);
	unsigned int AccessPreparation(unsigned __int64 index) const;
	T *GetLocalPointer(unsigned __int64 index) const;

public:
	CFileMappedArray(unsigned __int64 size);
	CFileMappedArray(const CFileMappedArray<T> &source);
	virtual ~CFileMappedArray(void);
	unsigned __int64 GetSize(void) const;
	void SetAt(unsigned __int64 index, const T &element);
	const T &GetAt(unsigned __int64 index) const;
	T &GetAt(unsigned __int64 index);
	void Copy(const CFileMappedArray<T> &source);
	void Fill(const T &element);
	const T &operator[](unsigned __int64 index) const;
	T &operator[](unsigned __int64 index);
	const CFileMappedArray<T> &operator=(const CFileMappedArray<T> &source);
#ifdef CFILEMAPPEDARRAY_TEST
	static void Test(void);
#endif
};

template <typename T>
inline CString CFileMappedArray<T>::CreateTemporaryFileName(void)
{
	LPTSTR dirName(NULL), fileName(NULL);
	try {
		dirName = new TCHAR[MAX_FILE_NAME];
		fileName = new TCHAR[MAX_FILE_NAME];
	}
	catch (std::bad_alloc xa) {
		if (dirName) {
			delete [] dirName;
		}
		if (fileName) {
			delete [] fileName;
		}
		throw ERROR_MEMORYALLOCATION;
	}

	if (::GetTempPath(MAX_FILE_NAME, dirName)) {
		if (!::GetTempFileName(dirName, _T("mmf"), 0, fileName)) {
			memset(fileName, NULL, sizeof(TCHAR) * MAX_FILE_NAME);
		}
	} else {
		memset(fileName, NULL, sizeof(TCHAR) * MAX_FILE_NAME);
	}

	CString result(fileName);
	delete [] dirName;
	delete [] fileName;

	if (result.IsEmpty()) {
		result = _T("mmf00000.TMP");
	}

	return result;
}

template <typename T>
inline void CFileMappedArray<T>::CreateMappingFile(const CString &fileName, unsigned __int64 bytes)
{
	if (bytes <= static_cast<unsigned __int64>(m_mappingSize)) {
		// the case that mapping to a file is unnecessary
		try {
			m_buf = new BYTE[bytes];
		}
		catch (std::bad_alloc xa) {
			throw ERROR_MEMORYALLOCATION;
		}
		return;
	}

	try {
		m_buf = new BYTE[m_mappingSize];
	}
	catch (std::bad_alloc xa) {
		throw ERROR_MEMORYALLOCATION;
	}

	if (!m_file.Open(fileName, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive | CFile::osRandomAccess, NULL)) {
		throw ERROR_FILE;
	}

	TRY {
		m_file.SetLength(bytes);
		m_file.SeekToBegin();
	}
	CATCH (CFileException, e) {
		m_file.Abort();
		::MoveFileEx(fileName, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
		delete [] m_buf;
		m_buf = NULL;
	}
	END_CATCH

	if (!m_buf) {
		throw ERROR_FILE;
	}
	m_usingFile = true;

	unsigned int i;
	for (i = 0; i < SEGMENT_COUNT; ++i) {
		m_currentSegment[i] = static_cast<unsigned __int64>(i);
		m_segmentAccessScore[i] = SEGMENT_REFRESH_SCORE;
	}
}

// Initialization called in all constructors
template <typename T>
inline void CFileMappedArray<T>::Initialize(unsigned __int64 size)
{
	ASSERT(size > 0);

	const unsigned __int64 bytes(static_cast<unsigned __int64>(sizeof(T)) * size);
#ifndef CFILEMAPPEDARRAY_TEST
	const unsigned int segmentElementSize(MAPPING_BASE_SIZE / SEGMENT_COUNT);
#else
	const unsigned int segmentElementSize(TEST_MAPPING_ELEMENT_COUNT * sizeof(T) / SEGMENT_COUNT);
#endif
	const unsigned int arrayLength(segmentElementSize / sizeof(T));
	m_segmentSize = arrayLength * sizeof(T);
	m_mappingSize = m_segmentSize * SEGMENT_COUNT;

	unsigned int i;
	for (i = 0; i < SEGMENT_COUNT; ++i) {
		m_currentSegment[i] = 0;
		m_segmentAccessScore[i] = 0;
	}

	m_size = 0;
	m_usingFile = false;
	m_buf = NULL;

	CreateMappingFile(CreateTemporaryFileName(), bytes);
	m_size = size;
}

template <typename T>
inline CFileMappedArray<T>::CFileMappedArray(unsigned __int64 size)
{
	Initialize(size);
}

template <typename T>
inline CFileMappedArray<T>::CFileMappedArray(const CFileMappedArray<T> &source)
{
	Initialize(source.GetSize());
	Copy(source);
}

template <typename T>
inline CFileMappedArray<T>::~CFileMappedArray(void)
{
	if (m_buf) {
		delete [] m_buf;
		const CString fileName(m_file.GetFilePath());
		m_file.Abort();
		::MoveFileEx(fileName, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
	}
}

template <typename T>
inline unsigned __int64 CFileMappedArray<T>::GetSize(void) const
{
	return m_size;
}

// Before accessing an element, this method load it to the memory
// @return : an local index of the requested data
template <typename T>
inline unsigned int CFileMappedArray<T>::AccessPreparation(unsigned __int64 index) const
{
	ASSERT(index < m_size);

	if (!m_usingFile) {
		return static_cast<unsigned int>(sizeof(T)) * static_cast<unsigned int>(index);
	}

	const unsigned __int64 segmentSize64(static_cast<unsigned __int64>(m_segmentSize));
	const unsigned __int64 sizeT64(static_cast<unsigned __int64>(sizeof(T)));
	const unsigned __int64 targetPointer(sizeT64 * static_cast<unsigned __int64>(index));
	const unsigned __int64 targetSegment(targetPointer / segmentSize64);
	unsigned int result(0);
	unsigned int i;

	// Check whether each segment has been loaded to the memory
	for (i = 0; i < SEGMENT_COUNT; ++i) {
		if (m_currentSegment[i] == targetSegment) {
			// already loaded
			const unsigned __int64 segmentOffset(m_currentSegment[i] * segmentSize64);
			result = i * m_segmentSize + static_cast<unsigned int>(targetPointer - segmentOffset);
			break;
		}
	}
	if (i < SEGMENT_COUNT) {
		unsigned int j;
		for (j = 0; j < SEGMENT_COUNT; ++j) {
			if (j != i) {
				if (m_segmentAccessScore[j] > 0) {
					--m_segmentAccessScore[j];
				}
			} else {
				if (m_segmentAccessScore[j] < (2 * SEGMENT_REFRESH_SCORE)) {
					++m_segmentAccessScore[j];
				}
			}
		}
		return result;
	}

	// Replace the most inaccessible segment which has not been loaded
	unsigned int accessMin(m_segmentAccessScore[0]);
	unsigned int accessMinIndex(0);
	for (i = 1; i < SEGMENT_COUNT; ++i) {
		if (m_segmentAccessScore[i] < accessMin) {
			accessMinIndex = i;
			accessMin = m_segmentAccessScore[i];
		}
	}

	TRY {
		const unsigned __int64 totalSize(sizeT64 * m_size);
		unsigned __int64 segmentOffset(m_currentSegment[accessMinIndex] * segmentSize64);
		unsigned int ioSize(m_segmentSize);
		unsigned int localSegmentOffset(accessMinIndex * m_segmentSize);

		// Save the current data to a file
		m_file.Seek(static_cast<__int64>(segmentOffset), CFile::begin);
		if (segmentOffset + segmentSize64 > totalSize) {
			ioSize = static_cast<unsigned int>(totalSize - segmentOffset);
		}
		m_file.Write(m_buf + localSegmentOffset, ioSize);
		m_file.Flush();

		// Load a file data to the memory
		m_currentSegment[accessMinIndex] = targetSegment;
		segmentOffset = targetSegment * segmentSize64;
		for (i = 0; i < SEGMENT_COUNT; ++i) {
			if (i != accessMinIndex) {
				if (m_segmentAccessScore[i] > 0) {
					--m_segmentAccessScore[i];
				}
			} else {
				m_segmentAccessScore[i] = SEGMENT_REFRESH_SCORE;
			}
		}
		m_file.Seek(static_cast<__int64>(segmentOffset), CFile::begin);
		ioSize = m_segmentSize;
		if (segmentOffset + segmentSize64 > totalSize) {
			ioSize = static_cast<unsigned int>(totalSize - segmentOffset);
		}
		m_file.Read(m_buf + localSegmentOffset, ioSize);
		result = localSegmentOffset + static_cast<unsigned int>(targetPointer - segmentOffset);
	}
	CATCH (CFileException, e) {
		const CString fileName(m_file.GetFilePath());
		m_file.Abort();
		::MoveFileEx(fileName, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
		delete [] m_buf;
		m_buf = NULL;
		for (i = 0; i < SEGMENT_COUNT; ++i) {
			m_currentSegment[i] = 0;
			m_segmentAccessScore[i] = 0;
		}
		m_size = 0;
		m_usingFile = false;
		throw ERROR_FILE;
	}
	END_CATCH

	return result;
}

// Return the pointer in the logical memory space.
template <typename T>
inline T *CFileMappedArray<T>::GetLocalPointer(unsigned __int64 index) const
{
	const unsigned int localIndex(AccessPreparation(index));
	LPBYTE p(m_buf + localIndex);
	return reinterpret_cast<T *>(p);
}

template <typename T>
inline void CFileMappedArray<T>::SetAt(unsigned __int64 index, const T &element)
{
	T *p(GetLocalPointer(index));
	*p = element;
}

template <typename T>
inline const T &CFileMappedArray<T>::GetAt(unsigned __int64 index) const
{
	const T *p(GetLocalPointer(index))
	return *p;
}

template <typename T>
inline T &CFileMappedArray<T>::GetAt(unsigned __int64 index)
{
	T *p(GetLocalPointer(index));
	return *p;
}

// Copy elements from a given array.
// The size of the array does not change.
template <typename T>
inline void CFileMappedArray<T>::Copy(const CFileMappedArray<T> &source)
{
	if (!m_usingFile) {
		const unsigned int size(static_cast<unsigned int>((m_size < source.GetSize()) ? m_size : source.GetSize()));
		unsigned int i;
		T *pMe(reinterpret_cast<T *>(m_buf)), *pSource(reinterpret_cast<T *>(source.m_buf));

		source.AccessPreparation(0);
		for (i = 0; i < size; ++i) {
			pMe[i] = pSource[i];
		}
		AccessPreparation(0);
	} else {
		const unsigned __int64 size((m_size < source.GetSize()) ? m_size : source.GetSize());
		const unsigned int bufSize(m_segmentSize / sizeof(T));
		const unsigned __int64 bufSize64(static_cast<unsigned __int64>(bufSize));
		unsigned __int64 i64;
		unsigned int i;
		T *pMe(NULL), *pSource(NULL);

		for (i64 = 0; i64 < size; i64 += bufSize64) {
			pMe = GetLocalPointer(i64);
			pSource = source.GetLocalPointer(i64);
			for (i = 0; i < bufSize; ++i) {
				pMe[i] = pSource[i];
			}
		}
	}
}

// Fill an array with the given value
template <typename T>
inline void CFileMappedArray<T>::Fill(const T &element)
{
	if (!m_usingFile) {
		unsigned int i;
		T *p(reinterpret_cast<T *>(m_buf));

		for (i = 0; i < m_size; ++i) {
			p[i] = element;
		}
	} else {
		const unsigned int bufSize(m_segmentSize / sizeof(T));
		const unsigned __int64 bufSize64(static_cast<unsigned __int64>(bufSize));

		unsigned __int64 i64;
		unsigned int i;
		T *p(NULL);

		for (i64 = 0; i64 < m_size; i64 += bufSize64) {
			p = GetLocalPointer(i64);
			for (i = 0; i < bufSize; ++i) {
				p[i] = element;
			}
		}
	}
}

template <typename T>
inline const T &CFileMappedArray<T>::operator[](unsigned __int64 index) const
{
	return GetAt(index);
}

template <typename T>
inline T &CFileMappedArray<T>::operator[](unsigned __int64 index)
{
	return GetAt(index);
}

template <typename T>
inline const CFileMappedArray<T> &CFileMappedArray<T>::operator=(const CFileMappedArray<T> &source)
{
	Copy(source);
	return *this;
}


#ifdef CFILEMAPPEDARRAY_TEST
template <typename T>
inline void CFileMappedArray<T>::Test(void)
{
	const int testElementCount(1004);
	const int testLoopCount(10);
	const int testRandomAccess(2000);

	int i, j, n;
	CFileMappedArray<int> me(testElementCount), me2(testElementCount / 2);
	static int a[testElementCount];

	TRACE(_T("*** Begin CFileMappedArray Test ***\n"));

	srand((time(NULL)));

	TRACE(_T("GetSize method test"));
	ASSERT(static_cast<int>(me.GetSize()) == testElementCount);
	TRACE(_T(" - OK\n"));

	for (i = 0; i < testLoopCount; ++i) {
		TRACE(_T("access test (%d / %d)"), i + 1, testLoopCount);
		for (j = 0; j < testElementCount; ++j) {
			a[j] = rand();
			me[j] = a[j];
		}
		for (j = 0; j < testElementCount; ++j) {
			ASSERT(me[j] == a[j]);
		}
		for (j = testElementCount - 1; j >= 0; --j) {
			ASSERT(me[j] == a[j]);
		}
		for (j = 0; j < testRandomAccess; ++j) {
			n = rand() % testElementCount;
			ASSERT(me[n] == a[n]);
		}
		TRACE(_T(" - OK\n"));
	}

	for (i = 0; i < testLoopCount; ++i) {
		TRACE(_T("Fill method test (%d / %d)"), i + 1, testLoopCount);
		n = rand();
		me.Fill(n);
		for (j = 0; j < testElementCount; ++j) {
			ASSERT(me[j] == n);
		}
		TRACE(_T(" - OK\n"));
	}

	for (i = 0; i < testLoopCount; ++i) {
		TRACE(_T("copy test (%d / %d)"), i + 1, testLoopCount);
		n = static_cast<int>((me.GetSize() < me2.GetSize()) ? me.GetSize() : me2.GetSize());

		// me -> me2
		for (j = 0; j < static_cast<int>(me.GetSize()); ++j) {
			me[j] = rand();
		}
		me2.Copy(me);
		for (j = 0; j < n; ++j) {
			ASSERT(me[j] == me2[j]);
		}

		// me2 -> me
		for (j = 0; j < static_cast<int>(me2.GetSize()); ++j) {
			me2[j] = rand();
		}
		me.Copy(me2);
		for (j = 0; j < n; ++j) {
			ASSERT(me[j] == me2[j]);
		}

		TRACE(_T(" - OK\n"));
	}

	TRACE(_T("*** Done ***\n"));
}
#endif
