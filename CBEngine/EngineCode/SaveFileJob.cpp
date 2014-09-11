#include "SaveFileJob.hpp"

#include "EngineErrorManager.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

SaveFileJob::~SaveFileJob() {

}


SaveFileJob::SaveFileJob( const std::string& filePathToSave, char* bufferToSave, size_t sizeOfBuffer, const std::string eventToFireUponCompletion, JobPriorityLevel priorityLevel /* = JOB_PRIORITY_AVERAGE */ ) :
	AsyncJob( eventToFireUponCompletion, priorityLevel ),
	m_filePath( filePathToSave ),
	m_fileBuffer( bufferToSave ),
	m_sizeOfBuffer( sizeOfBuffer ),
	m_jobComplete( false )
{
	FATAL_ASSERTION( bufferToSave != nullptr, "Error: Cannot save a file with buffer = nullptr" );
}


void SaveFileJob::executeJob() {

	FILE* fileToSave = fopen( m_filePath.c_str(), "wb" );
	FATAL_ASSERTION( fileToSave != nullptr, "Error: Could not save to file path specified in SaveFileJob" );

	fwrite( m_fileBuffer, sizeof( char ), m_sizeOfBuffer, fileToSave );

	fclose( fileToSave );

	m_namedPropertiesForEventCompletion.setProperty( SF_FILE_NAME_RESULT_STRING, m_filePath );
	m_namedPropertiesForEventCompletion.setProperty( SF_FILE_BUFFER_STRING, m_fileBuffer );
	m_namedPropertiesForEventCompletion.setProperty( SF_FILE_BUFFER_SIZE_STRING, m_sizeOfBuffer );

	m_jobComplete = true;
}