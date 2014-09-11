#include "HashBufferJob.hpp"

#include "EngineErrorManager.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

HashBufferJob::~HashBufferJob() {

}


HashBufferJob::HashBufferJob( const char* bufferToHash, size_t sizeOfBuffer, const std::string& eventToFireUponCompletion, JobPriorityLevel jobPriorityLevel /* = JOB_PRIORITY_AVERAGE */ ) :
	AsyncJob( eventToFireUponCompletion, jobPriorityLevel ),
	m_bufferToHash( bufferToHash ),
	m_sizeOfBuffer( sizeOfBuffer ),
	m_hashValue( 0 ),
	m_hashComplete( false )
{
	FATAL_ASSERTION( bufferToHash != nullptr, "Error: Cannot hash a buffer which has a value of nullptr" );
}


void HashBufferJob::executeJob() {

	for ( size_t i = 0; i < m_sizeOfBuffer; ++i ) {

		m_hashValue &= 0x07ffffff;
		m_hashValue *= 31;
		m_hashValue += m_bufferToHash[i];
	}

	m_namedPropertiesForEventCompletion.setProperty( HASH_BUFFER_RESULT_STRING, m_hashValue );
	m_namedPropertiesForEventCompletion.setProperty( HASH_BUFFER_CONTENT, m_bufferToHash );
	m_namedPropertiesForEventCompletion.setProperty( HASH_BUFFER_SIZE, m_sizeOfBuffer );

	m_hashComplete = true;
}