#include "CBMemoryManager.hpp"

#include <malloc.h>
#include <atlstr.h>
#include <string>

#include "EngineErrorManager.hpp"
#include "Console.hpp"
#include "Vector4.hpp"


CBMemoryManager::~CBMemoryManager() {

	if ( m_isPoolInitialized && m_pool ) {

		scanForMemoryLeaksAndReportToOutputWindow();
	}
	

	resetAllTrackingStatistics();
}


void CBMemoryManager::initializeMemoryPool( size_t sizeOfPoolInBytes ) {

	FATAL_ASSERTION( !m_isPoolInitialized, "ERROR: Memory pool has already been initialized. Cannot call initialize on an already initialized memory pool." );

	m_pool = (unsigned char*) malloc( sizeOfPoolInBytes * sizeof( unsigned char ) );
	m_poolByteSize = sizeOfPoolInBytes;

	buildMetaDataForInitializedPool();

	m_isPoolInitialized = true;
}


void CBMemoryManager::buildMetaDataForInitializedPool() {

	FATAL_ASSERTION( m_pool != nullptr, "ERROR: Cannot initialize starting MetaData for pool as the pool has not been initialized" );
	FATAL_ASSERTION( !m_isPoolInitialized, "ERROR: Cannot call buildMetaDataForInitializedPool on an already initialized pool" );

	MetaData* poolBeginMetaData = reinterpret_cast<MetaData*>( &m_pool[0] );

	poolBeginMetaData->m_blockDataSegmentSizeBytes = m_poolByteSize - sizeof( MetaData );
	poolBeginMetaData->m_isOccupied = false;
	poolBeginMetaData->m_fileName = nullptr;
	poolBeginMetaData->m_lineNumber = -1;
}


void CBMemoryManager::update( float deltaSeconds ) {
	UNUSED( deltaSeconds );

	if ( m_shouldReportDetailedStatsAtEndOfFrame ) {

		reportDetailedTrackedStatisticsToConsole();
	}

	// Reset frame to frame stats
	resetFrameToFrameStats();
}

void* CBMemoryManager::allocateMemory( size_t numBytesToAllocate, const char* fileName, int lineNum ) {
	// Currently top down approach
	void* memoryToAllocate = nullptr; 
	size_t numBytesAdvanced = 0;
	size_t totalBytesNeededForAllocation = sizeof( MetaData ) + numBytesToAllocate;
	MetaData* currentBlockMetaData = reinterpret_cast<MetaData*>( &m_pool[ numBytesAdvanced ] );

	updateTrackingStatisticsWithAllocationRequest( totalBytesNeededForAllocation, fileName, lineNum );

	while ( currentBlockMetaData->m_isOccupied || ( currentBlockMetaData->m_blockDataSegmentSizeBytes <= totalBytesNeededForAllocation ) ) {

		size_t sizeOfCurrentBlock = currentBlockMetaData->m_blockDataSegmentSizeBytes;
		size_t incrementForLocationOfNextBlock = sizeOfCurrentBlock + sizeof( MetaData );
		numBytesAdvanced += incrementForLocationOfNextBlock;

		FATAL_ASSERTION( numBytesAdvanced < m_poolByteSize, "ERROR: CBMemoryManager cannot allocate memory as there is no available space left in the memory pool." );

		currentBlockMetaData = reinterpret_cast<MetaData*>( &m_pool[ numBytesAdvanced ] );

		if ( !currentBlockMetaData->m_isOccupied && currentBlockMetaData->m_blockDataSegmentSizeBytes == numBytesToAllocate ) {
			// Edge case where want we want to allocate is EXACTLY equal to metadata current size... So no new metadata creation is necessary

			currentBlockMetaData->m_isOccupied = true;
			currentBlockMetaData->m_fileName = fileName;
			currentBlockMetaData->m_lineNumber = lineNum;

			memoryToAllocate = ( reinterpret_cast<byte_t*>( currentBlockMetaData ) + sizeof( MetaData ) );

			return memoryToAllocate;
		}
	} 
	
	FATAL_ASSERTION( !currentBlockMetaData->m_isOccupied, "ERROR: Will remove this later ... here for debugging purposes" );

	// At this point.. have found a block that is not occupied and has enough memory
	// Create a new block and mark it as not occupied
	MetaData* newMetaDataBlock = reinterpret_cast<MetaData*>( &m_pool[ numBytesAdvanced + totalBytesNeededForAllocation ] ); 
	newMetaDataBlock->m_isOccupied = false;
	newMetaDataBlock->m_blockDataSegmentSizeBytes = currentBlockMetaData->m_blockDataSegmentSizeBytes - totalBytesNeededForAllocation;
	newMetaDataBlock->m_fileName = nullptr;
	newMetaDataBlock->m_lineNumber = -1;

	currentBlockMetaData->m_blockDataSegmentSizeBytes = numBytesToAllocate;
	currentBlockMetaData->m_isOccupied = true;
	currentBlockMetaData->m_fileName = fileName;
	currentBlockMetaData->m_lineNumber = lineNum;

	memoryToAllocate = ( reinterpret_cast<byte_t*>( currentBlockMetaData ) + sizeof( MetaData ) );

	return memoryToAllocate;
}


void CBMemoryManager::freeMemory( void* locationOfMemoryToFree, const char* fileName /* = nullptr */, int lineNum /* = -1 */ ) {

	MetaData* metaDataForMemoryBlockToFree = reinterpret_cast<MetaData*>(  reinterpret_cast<byte_t*>( locationOfMemoryToFree ) - sizeof( MetaData ) );
	size_t totalSizeOfBlockToFree = metaDataForMemoryBlockToFree->m_blockDataSegmentSizeBytes + sizeof( MetaData );
	size_t numBytesAdvanced = 0;

	updateTrackingStatisticsWithFreeMemoryRequest( metaDataForMemoryBlockToFree->m_blockDataSegmentSizeBytes + sizeof( MetaData ) );

	// Need to find MetaData for the block above us
	MetaData* previousMetaDataBlock = reinterpret_cast<MetaData*>( &m_pool[ numBytesAdvanced ] ); 
	MetaData* currentMetaDataBlock = previousMetaDataBlock;
	MetaData* metaDataBlockAfterBlockToFree = reinterpret_cast<MetaData*>(  ( reinterpret_cast<byte_t*>( metaDataForMemoryBlockToFree ) +
		sizeof( MetaData ) +
		metaDataForMemoryBlockToFree->m_blockDataSegmentSizeBytes ) );

	// TODO:: Assert special case where this is the last block and this might overreach our memory pool
	if ( currentMetaDataBlock == metaDataForMemoryBlockToFree ) {
		// Edge case where the memory we need to free is the first block which has no previous block
		if ( metaDataBlockAfterBlockToFree->m_isOccupied ) {
			// No mergers required as block is current occupied
			metaDataForMemoryBlockToFree->m_isOccupied = false;
			metaDataForMemoryBlockToFree->m_fileName = nullptr;
			metaDataForMemoryBlockToFree->m_lineNumber = -1;
			
		} else {
			// Block IS NOT occupied and needs to be merged
			size_t mergedMetaDataTotalBlockSize = metaDataForMemoryBlockToFree->m_blockDataSegmentSizeBytes +
				sizeof( MetaData ) +
				metaDataBlockAfterBlockToFree->m_blockDataSegmentSizeBytes;

			metaDataForMemoryBlockToFree->m_blockDataSegmentSizeBytes = mergedMetaDataTotalBlockSize;
			metaDataForMemoryBlockToFree->m_isOccupied = false;
			metaDataForMemoryBlockToFree->m_fileName = nullptr;
			metaDataForMemoryBlockToFree->m_lineNumber = -1;
				
		}
		return;
	}

	// MetaData Block has a previous block... Need to find it!
	while ( currentMetaDataBlock != metaDataForMemoryBlockToFree ) {

		previousMetaDataBlock = currentMetaDataBlock;

		size_t sizeOfCurrentBlock =  currentMetaDataBlock->m_blockDataSegmentSizeBytes;
		size_t incrementForLocationOfNextBlock = sizeOfCurrentBlock + sizeof( MetaData );
		numBytesAdvanced += incrementForLocationOfNextBlock;

		FATAL_ASSERTION( numBytesAdvanced < m_poolByteSize, "ERROR: CBMemoryManager cannot delete memory as its search has exceeded memory pool size. This likely entails data corruption." );

		 currentMetaDataBlock = reinterpret_cast<MetaData*>( &m_pool[ numBytesAdvanced ] );
	}

	FATAL_ASSERTION( currentMetaDataBlock->m_isOccupied, "ERROR: CBMemoryManager cannot delete memory which has already been freed or was never allocated" );

	// At this point the previousMetaDataBlock is the block previous to the block we want to free
	// Four possible cases
	// Both block before and after are free and should be merged with the block we are about to free
	// Block before it is free and should be merged but the block after is occupied
	// Block after it is free and should be merged but the block before is occupied
	// Both blocks before and after are occupied and cannot be merged

	if ( previousMetaDataBlock->m_isOccupied && metaDataBlockAfterBlockToFree->m_isOccupied ) {
		// Both before and after are occupied ( Simple Case... Just need to mark as unoccupied )
		metaDataForMemoryBlockToFree->m_isOccupied = false;
		metaDataForMemoryBlockToFree->m_fileName = nullptr;
		metaDataForMemoryBlockToFree->m_lineNumber = -1;

	} else if ( !previousMetaDataBlock->m_isOccupied && metaDataBlockAfterBlockToFree->m_isOccupied ) {
		// Before is not occupied and after is 
		size_t mergedMetaDataTotalBlockSize = previousMetaDataBlock->m_blockDataSegmentSizeBytes +
			sizeof( MetaData ) +
			metaDataForMemoryBlockToFree->m_blockDataSegmentSizeBytes;

		previousMetaDataBlock->m_blockDataSegmentSizeBytes = mergedMetaDataTotalBlockSize;
		previousMetaDataBlock->m_fileName = nullptr;
		previousMetaDataBlock->m_lineNumber = -1;

	} else if ( previousMetaDataBlock->m_isOccupied && !metaDataBlockAfterBlockToFree->m_isOccupied ) {
		// Before is occupied and after is not
		size_t mergedMetaDataTotalBlockSize = metaDataForMemoryBlockToFree->m_blockDataSegmentSizeBytes +
			sizeof( MetaData ) +
			metaDataBlockAfterBlockToFree->m_blockDataSegmentSizeBytes;

		metaDataForMemoryBlockToFree->m_blockDataSegmentSizeBytes = mergedMetaDataTotalBlockSize;
		metaDataForMemoryBlockToFree->m_isOccupied = false;
		metaDataForMemoryBlockToFree->m_fileName = nullptr;
		metaDataForMemoryBlockToFree->m_lineNumber = -1;

	} else if ( !previousMetaDataBlock->m_isOccupied && !metaDataBlockAfterBlockToFree->m_isOccupied ) {
		// Before and after are not occupied ( Merger of both is required )
		size_t mergedMetaDataTotalBlockSize = previousMetaDataBlock->m_blockDataSegmentSizeBytes + 
			sizeof( MetaData ) + 
			metaDataForMemoryBlockToFree->m_blockDataSegmentSizeBytes + 
			sizeof( MetaData ) + 
			metaDataBlockAfterBlockToFree->m_blockDataSegmentSizeBytes;

		previousMetaDataBlock->m_blockDataSegmentSizeBytes = mergedMetaDataTotalBlockSize;
		previousMetaDataBlock->m_fileName = nullptr;
		previousMetaDataBlock->m_lineNumber = -1;
	} 
}


void CBMemoryManager::reportCurrentOverallTrackedStatisticsToConsole() {

	const cbengine::Vector4<float> textColorForTrackingReport( 0.90f, 0.30f, 0.15f, 1.0f );
	Console* sharedDeveloperConsole = Console::getSharedDeveloperConsole();

	size_t averageSizeOfAllocationRequest = 0;
	if ( m_totalNumberOfAllocationRequests <= 0 ) {
		// Avoid divide by 0 case
		averageSizeOfAllocationRequest = 0;
	} else {

		averageSizeOfAllocationRequest = ( m_totalNumberOfBytesAllocated / m_totalNumberOfAllocationRequests );
	}

	std::string totalSizeOfMemoryPoolAsString = std::to_string( static_cast<long long int>( m_poolByteSize ) );
	std::string totalNumAllocationsAsString = std::to_string( m_totalNumberOfAllocationRequests );
	std::string totalNumberOfBytesAllocated = std::to_string( static_cast<long long int>( m_totalNumberOfBytesAllocated ) );
	std::string currentBytesAllocatedAsString = std::to_string( static_cast<long long int>( m_currentNumBytesAllocated ) );
	std::string largestByteAllocationRequested = std::to_string( static_cast<long long int>( m_largestAllocationRequested ) );
	std::string averageSizeOfByeAllocationRequestedString = std::to_string( static_cast<long long int>( averageSizeOfAllocationRequest ) );

	std::string statisticsStringForConsole;
	statisticsStringForConsole += "\\----------- Memory Manager Tracking Statistics -----------//";
	sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
	statisticsStringForConsole.clear();

	statisticsStringForConsole += "Total Number Of Allocations: ";
	statisticsStringForConsole += totalNumAllocationsAsString;
	sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
	statisticsStringForConsole.clear();

	statisticsStringForConsole += "Total Number Of Bytes Allocated Since Program Start: ";
	statisticsStringForConsole += totalNumberOfBytesAllocated;
	sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
	statisticsStringForConsole.clear();

	statisticsStringForConsole += "Total Size In Bytes For Memory Pool: ";
	statisticsStringForConsole += totalSizeOfMemoryPoolAsString;
	sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
	statisticsStringForConsole.clear();

	statisticsStringForConsole += "Current Number Of Bytes Allocated: ";
	statisticsStringForConsole += currentBytesAllocatedAsString;
	sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
	statisticsStringForConsole.clear();

	statisticsStringForConsole += "Largest Byte Allocation Requested: ";
	statisticsStringForConsole += largestByteAllocationRequested;
	sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
	statisticsStringForConsole.clear();

	if ( m_fileNameOfLargestAllocationRequested != nullptr ) {

		statisticsStringForConsole += "  From FilePath: ";
		statisticsStringForConsole += m_fileNameOfLargestAllocationRequested;
		sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
		statisticsStringForConsole.clear();

		std::string largeLineNumAsString = std::to_string( static_cast<long long int>( m_lineNumOfLargestAllocationRequested ) );

		statisticsStringForConsole += "  On Line Number ";
		statisticsStringForConsole += largeLineNumAsString;
		sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
		statisticsStringForConsole.clear();
	}
	
	statisticsStringForConsole += "Average Size Of Byte Allocation Requested: ";
	statisticsStringForConsole += averageSizeOfByeAllocationRequestedString;
	sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
	statisticsStringForConsole.clear();

	statisticsStringForConsole += "\\-------- End Memory Manager Tracking Statistics --------//";
	sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
	statisticsStringForConsole.clear();
}


void CBMemoryManager::reportDetailedTrackedStatisticsToConsoleAtEndOfFrame() const {

	m_shouldReportDetailedStatsAtEndOfFrame = true;


}


void CBMemoryManager::reportDetailedTrackedStatisticsToConsole() {

	const cbengine::Vector4<float> textColorForTrackingReport( 0.90f, 0.30f, 0.15f, 1.0f );
	Console* sharedDeveloperConsole = Console::getSharedDeveloperConsole();

	size_t averageSizeOfAllocationRequest = 0;
	if ( m_totalNumberOfAllocationRequests <= 0 ) {
		// Avoid divide by 0 case
		averageSizeOfAllocationRequest = 0;
	} else {

		averageSizeOfAllocationRequest = ( m_totalNumberOfBytesAllocated / m_totalNumberOfAllocationRequests );
	}

	size_t numBytesRemainingInPool = m_poolByteSize - m_currentNumBytesAllocated;

	std::string totalSizeOfMemoryPoolAsString = std::to_string( static_cast<long long int>( m_poolByteSize ) );
	std::string totalNumAllocationsAsString = std::to_string( m_totalNumberOfAllocationRequests );
	std::string currentBytesAllocatedAsString = std::to_string( static_cast<long long int>( m_currentNumBytesAllocated ) );
	std::string numBytesRemainingInPoolString = std::to_string( static_cast<long long int>( numBytesRemainingInPool ) );
	std::string largestByteAllocationRequested = std::to_string( static_cast<long long int>( m_largestAllocationRequested ) );
	std::string averageSizeOfByeAllocationRequestedString = std::to_string( static_cast<long long int>( averageSizeOfAllocationRequest ) );

	std::string numBytesAllocatedForFrameString = std::to_string( static_cast<long long int>( m_numBytesAllocatedForFrame ) );
	std::string numAllocationsForFrameString = std::to_string( m_currentNumAllocationsForFrame );
	std::string numFreeMemoryRequestesForFrameString = std::to_string( m_currentNumFreeMemoryRequestsPerFrame );

	std::string statisticsStringForConsole;
	statisticsStringForConsole += "\\----------- Memory Manager Detailed Tracking Statistics -----------//";
	sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
	statisticsStringForConsole.clear();

	statisticsStringForConsole += "Total Number Of Allocations: ";
	statisticsStringForConsole += totalNumAllocationsAsString;
	sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
	statisticsStringForConsole.clear();

	statisticsStringForConsole += "Total Size In Bytes For Memory Pool: ";
	statisticsStringForConsole += totalSizeOfMemoryPoolAsString;
	sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
	statisticsStringForConsole.clear();

	statisticsStringForConsole += "Current Number Of Bytes Allocated: ";
	statisticsStringForConsole += currentBytesAllocatedAsString;
	sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
	statisticsStringForConsole.clear();

	statisticsStringForConsole += "Total Number Of Bytes Remaining For Memory Pool: ";
	statisticsStringForConsole += numBytesRemainingInPoolString;
	sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
	statisticsStringForConsole.clear();

	statisticsStringForConsole += "Largest Byte Allocation Requested: ";
	statisticsStringForConsole += largestByteAllocationRequested;
	sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
	statisticsStringForConsole.clear();

	if ( m_fileNameOfLargestAllocationRequested != nullptr ) {

		statisticsStringForConsole += "  From FilePath: ";
		statisticsStringForConsole += m_fileNameOfLargestAllocationRequested;
		sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
		statisticsStringForConsole.clear();

		std::string largeLineNumAsString = std::to_string( static_cast<long long int>( m_lineNumOfLargestAllocationRequested ) );

		statisticsStringForConsole += "  On Line Number ";
		statisticsStringForConsole += largeLineNumAsString;
		sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
		statisticsStringForConsole.clear();
	}

	statisticsStringForConsole += "Average Size Of Byte Allocation Requested: ";
	statisticsStringForConsole += averageSizeOfByeAllocationRequestedString;
	sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
	statisticsStringForConsole.clear();

	statisticsStringForConsole += "Total Number Of Allocations Requested This Frame: ";
	statisticsStringForConsole += numAllocationsForFrameString;
	sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
	statisticsStringForConsole.clear();

	statisticsStringForConsole += "Total Number Of Bytes Allocated This Frame: ";
	statisticsStringForConsole += numBytesAllocatedForFrameString;
	sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
	statisticsStringForConsole.clear();

	statisticsStringForConsole += "Total Number Of FreeMemory Requests This Frame: ";
	statisticsStringForConsole += numFreeMemoryRequestesForFrameString;
	sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
	statisticsStringForConsole.clear();

	statisticsStringForConsole += "\\-------- End Memory Manager Detailed Tracking Statistics --------//";
	sharedDeveloperConsole->addStringToConsole( statisticsStringForConsole, textColorForTrackingReport );
	statisticsStringForConsole.clear();
}


void CBMemoryManager::resetAllTrackingStatistics() {

	m_totalNumberOfAllocationRequests = 0L;
	m_currentNumBytesAllocated = 0;
	m_totalNumberOfBytesAllocated = 0;
	m_largestAllocationRequested = 0;
	m_shouldReportDetailedStatsAtEndOfFrame = false;
	m_fileNameOfLargestAllocationRequested = nullptr;
	m_lineNumOfLargestAllocationRequested = -1;
}


void CBMemoryManager::resetFrameToFrameStats() {

	m_currentNumAllocationsForFrame = 0;
	m_numBytesAllocatedForFrame = 0;
	m_currentNumFreeMemoryRequestsPerFrame = 0;
	m_shouldReportDetailedStatsAtEndOfFrame = false;
}

// TODO:: This function is kinda messy... Will clean up later
void CBMemoryManager::scanForMemoryLeaksAndReportToOutputWindow() {

	size_t numBytesAdvanced = 0;
	MetaData* currentMetaData = reinterpret_cast<MetaData*>( &m_pool[ numBytesAdvanced ] );
	int countLeaks = 0;

	EngineErrorManager& sharedEngineErrorManager = EngineErrorManager::getEngineErrorManager();

	while ( numBytesAdvanced < m_poolByteSize ) {

		if ( currentMetaData->m_isOccupied ) {
			// Memory Leak Detected
			if ( currentMetaData->m_fileName != nullptr ) {

				CStringA fileNameCString( currentMetaData->m_fileName );
				char lineNumBuffer[1024];
				char sizeOfLeakBuffer[1024];
				CStringA fileLineNumberCString( _itoa( currentMetaData->m_lineNumber, lineNumBuffer, 10 ) );
				CStringA numberOfBytesLeakedCString( _itoa( currentMetaData->m_blockDataSegmentSizeBytes, sizeOfLeakBuffer, 10 ) );
				
				CStringA memoryLeakReportString;
				memoryLeakReportString += fileNameCString;
				memoryLeakReportString += "(";
				memoryLeakReportString += fileLineNumberCString;
				memoryLeakReportString += "): A Memory leak of ";
				memoryLeakReportString += numberOfBytesLeakedCString;
				memoryLeakReportString += " Bytes has been detected, which was allocated on line ";
				memoryLeakReportString += fileLineNumberCString;
				memoryLeakReportString += "\n";
				
				sharedEngineErrorManager.outputErrorMessageAsCStringToConsole( memoryLeakReportString.GetBuffer() );
			} else {
				
				CStringA unknownFileAndNumberLeak( "A Memory leak of " );
				char sizeOfLeakBuffer[1024];
				CStringA numberOfBytesLeakedCString( _itoa( currentMetaData->m_blockDataSegmentSizeBytes, sizeOfLeakBuffer, 10 ) );
				unknownFileAndNumberLeak += numberOfBytesLeakedCString;
				unknownFileAndNumberLeak += " Bytes has been detected. The file name and line number are unknown. \n";

				sharedEngineErrorManager.outputErrorMessageAsCStringToConsole( unknownFileAndNumberLeak.GetBuffer() );
				
			}

			++countLeaks;
		}

		numBytesAdvanced += sizeof( MetaData ) + currentMetaData->m_blockDataSegmentSizeBytes;
		currentMetaData = reinterpret_cast<MetaData*>( &m_pool[ numBytesAdvanced ] );
	}
	this;
}


void CBMemoryManager::setMemoryManagerDefaults() {

	m_pool = nullptr;
	m_poolByteSize = 0;
	m_isPoolInitialized = false;
	m_shouldReportDetailedStatsAtEndOfFrame = false;

	resetAllTrackingStatistics();
}

