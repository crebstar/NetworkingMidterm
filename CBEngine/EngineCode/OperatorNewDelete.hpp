#ifndef included_OperatorNewDelete
#define included_OperatorNewDelete
#pragma once

#ifdef USE_MEMORY_MANAGER

void* operator new( size_t objectSizeInBytes, const char* fileName, int lineNum );
void operator delete( void* objectToDelete, const char* fileName, int lineNum );

void* operator new[]( size_t objectSizeInBytes, const char* fileName, int lineNum );
void operator delete[]( void* objectToDelete, const char* fileName, int lineNum );

#endif

#endif