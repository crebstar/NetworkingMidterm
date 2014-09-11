#include "ReverseBufferJob.hpp"

#include <algorithm>

#include "EngineErrorManager.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

ReverseBufferJob::~ReverseBufferJob() {

}


ReverseBufferJob::ReverseBufferJob( char* bufferToReverse, size_t sizeOfBuffer, const std::string& eventNameToFireOnCompletion, JobPriorityLevel priorityLevel /* = JOB_PRIORITY_AVERAGE */ ) :
	AsyncJob( eventNameToFireOnCompletion, priorityLevel ),
	m_bufferToReverse( bufferToReverse ),
	m_sizeOfBuffer( sizeOfBuffer ),
	m_reverseBufferComplete( false )
{
	FATAL_ASSERTION( bufferToReverse != nullptr, "Error: Cannot reverse contents of a buffer that is nullptr" );
}


void ReverseBufferJob::executeJob() {

	std::reverse<char*>( m_bufferToReverse, &m_bufferToReverse[ ( m_sizeOfBuffer - 1 ) ] );

	m_namedPropertiesForEventCompletion.setProperty( REVERSE_BUFFER_RESULT_STRING, m_bufferToReverse );
	m_namedPropertiesForEventCompletion.setProperty( REVERSE_BUFFER_SIZE_STRING, m_sizeOfBuffer );

	m_reverseBufferComplete = true;
}