#ifndef included_CBMemoryManager
#define included_CBMemoryManager
#pragma once

#include "EngineMacros.hpp"

typedef unsigned char byte_t;
typedef short word_t;
typedef unsigned int dword_t;

class CBMemoryManager {
public:
	static CBMemoryManager& getSharedMemoryManager() {

		static CBMemoryManager memoryManager;
		return memoryManager;
	}


	struct MetaData {
	public:
		size_t			m_blockDataSegmentSizeBytes;
		bool			m_isOccupied;	
		const char*		m_fileName;
		int				m_lineNumber;
	};

	~CBMemoryManager();

	void update( float deltaSeconds );

	void* allocateMemory( size_t numBytesToAllocate, const char* fileName = nullptr, int lineNum = -1 );
	void freeMemory( void* locationOfMemoryToFree, const char* fileName = nullptr, int lineNum = -1 );

	// Statistics
	void reportDetailedTrackedStatisticsToConsoleAtEndOfFrame() const;
	void reportCurrentOverallTrackedStatisticsToConsole();
	void resetAllTrackingStatistics();

	// Memory Leak Detection
	void scanForMemoryLeaksAndReportToOutputWindow();
	
	// Inline Functions
	bool isMemoryPoolInitialized() const;
protected:

	CBMemoryManager() {

		setMemoryManagerDefaults();

		#ifdef USE_MEMORY_MANAGER
		initializeMemoryPool( 1024 * 1024 * 750 ); // For some reason fails with > 1GB
		#endif
	}

	void initializeMemoryPool( size_t sizeOfPoolInBytes );

	// ---------------------- Statistical Tracking ---------------------- //
	void updateTrackingStatisticsWithAllocationRequest( size_t numBytesAllocated, const char* fileName, int lineNum );
	void updateTrackingStatisticsWithFreeMemoryRequest( size_t numBytesFreed );
	void reportDetailedTrackedStatisticsToConsole();
	void resetFrameToFrameStats();

	// Overall ish
	long long int					m_totalNumberOfAllocationRequests;
	size_t							m_currentNumBytesAllocated;
	size_t							m_totalNumberOfBytesAllocated;
	size_t							m_largestAllocationRequested;
	const char*						m_fileNameOfLargestAllocationRequested;
	int								m_lineNumOfLargestAllocationRequested;

	// Frame to Frame
	long long int					m_currentNumAllocationsForFrame;
	long long int					m_currentNumFreeMemoryRequestsPerFrame;
	size_t							m_numBytesAllocatedForFrame;

	void setMemoryManagerDefaults();
private:
	PREVENT_COPY_AND_ASSIGN( CBMemoryManager );

	void buildMetaDataForInitializedPool();

	byte_t*				m_pool;
	size_t				m_poolByteSize;
	mutable bool		m_isPoolInitialized;
	mutable bool		m_shouldReportDetailedStatsAtEndOfFrame;
};


// ---------------------- Inline Statistical Tracking ---------------------- //
inline void CBMemoryManager::updateTrackingStatisticsWithAllocationRequest( size_t numBytesAllocated, const char* fileName, int lineNum ) {

	++m_totalNumberOfAllocationRequests;
	m_currentNumBytesAllocated += numBytesAllocated;
	m_totalNumberOfBytesAllocated += numBytesAllocated;

	if ( numBytesAllocated > m_largestAllocationRequested ) {

		m_largestAllocationRequested = numBytesAllocated;
		m_fileNameOfLargestAllocationRequested = fileName;
		m_lineNumOfLargestAllocationRequested = lineNum;
	}

	// Frame By Frame
	m_numBytesAllocatedForFrame += numBytesAllocated;
	++m_currentNumAllocationsForFrame;
}


inline void CBMemoryManager::updateTrackingStatisticsWithFreeMemoryRequest( size_t numBytesFreed ) {

	m_currentNumBytesAllocated -= numBytesFreed;

	// Frame by Frame
	++m_currentNumFreeMemoryRequestsPerFrame;
}


inline bool CBMemoryManager::isMemoryPoolInitialized() const {

	return m_isPoolInitialized;
}

#endif