#include "OperatorNewDelete.hpp"

#include <malloc.h>

#include "CBMemoryManager.hpp"
#include "EngineErrorManager.hpp"


#ifdef USE_MEMORY_MANAGER

#undef new
void* operator new( size_t objectSizeInBytes ) {

	void* objectToAllocate = nullptr;

	CBMemoryManager& sharedMemoryManager = CBMemoryManager::getSharedMemoryManager();
	objectToAllocate = sharedMemoryManager.allocateMemory( objectSizeInBytes );

	FATAL_ASSERTION( objectToAllocate != nullptr, "ERROR: Program is out of virtual memory. Custom Operator new has failed." );

	return objectToAllocate;
}


void* operator new( size_t objectSizeInBytes, const char* fileName, int lineNum ) {

	void* objectToAllocate = nullptr;

	CBMemoryManager& sharedMemoryManager = CBMemoryManager::getSharedMemoryManager();
	objectToAllocate = sharedMemoryManager.allocateMemory( objectSizeInBytes, fileName, lineNum );

	FATAL_ASSERTION( objectToAllocate != nullptr, "ERROR: Program is out of virtual memory. Custom Operator new has failed." );

	return objectToAllocate;
}


void* operator new[]( size_t objectSizeInBytes ) {

	void* objectToAllocate = nullptr;

	CBMemoryManager& sharedMemoryManager = CBMemoryManager::getSharedMemoryManager();
	objectToAllocate = sharedMemoryManager.allocateMemory( objectSizeInBytes );

	FATAL_ASSERTION( objectToAllocate != nullptr, "ERROR: Program is out of virtual memory. Custom Operator new has failed." );

	return objectToAllocate;
}


void* operator new[]( size_t objectSizeInBytes, const char* fileName, int lineNum ) {

	void* objectToAllocate = nullptr;

	CBMemoryManager& sharedMemoryManager = CBMemoryManager::getSharedMemoryManager();
	objectToAllocate = sharedMemoryManager.allocateMemory( objectSizeInBytes, fileName, lineNum );
	
	FATAL_ASSERTION( objectToAllocate != nullptr, "ERROR: Program is out of virtual memory. Custom Operator new has failed." );

	return objectToAllocate;
}


void operator delete( void* objectToDelete ) {

	CBMemoryManager& sharedMemoryManager = CBMemoryManager::getSharedMemoryManager();
	sharedMemoryManager.freeMemory( objectToDelete );
}


void operator delete( void* objectToDelete, const char* fileName, int lineNum ) {

	CBMemoryManager& sharedMemoryManager = CBMemoryManager::getSharedMemoryManager();
	sharedMemoryManager.freeMemory( objectToDelete );
}


void operator delete[]( void* objectToDelete ) {

	CBMemoryManager& sharedMemoryManager = CBMemoryManager::getSharedMemoryManager();
	sharedMemoryManager.freeMemory( objectToDelete );
}


void operator delete[]( void* objectToDelete, const char* fileName, int lineNum ) {

	CBMemoryManager& sharedMemoryManager = CBMemoryManager::getSharedMemoryManager();
	sharedMemoryManager.freeMemory( objectToDelete );
}

#define new new(__FILE__,__LINE__)

#endif