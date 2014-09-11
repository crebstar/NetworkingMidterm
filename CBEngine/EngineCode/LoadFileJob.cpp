#include "LoadFileJob.hpp"

#include "EngineErrorManager.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

LoadFileJob::~LoadFileJob() {

}


LoadFileJob::LoadFileJob( const std::string filePath, const std::string& fileName, const std::string& eventToFireUponCompletion, JobPriorityLevel priorityLevel ) :
	AsyncJob( eventToFireUponCompletion, priorityLevel ),
	m_filePathToLoad( filePath ),
	m_fileName( fileName ),
	m_jobCompleted( false )
{

}


void LoadFileJob::executeJob() {

	FILE* fileToLoad = fopen( m_filePathToLoad.c_str(), "rb" );
	FATAL_ASSERTION( fileToLoad != nullptr, "Error: Failed to load file in LoadFileJob. The file path specified did not result in a file being loaded" );

	size_t sizeOfFile = determineSizeOfFile( fileToLoad );
	char* fileBuffer = new char[sizeOfFile];

	fread( fileBuffer, sizeof( char ), sizeOfFile, fileToLoad );

	fclose( fileToLoad );

	m_namedPropertiesForEventCompletion.setProperty( LF_BYTE_BUFFER_RESULT_STRING, fileBuffer );
	m_namedPropertiesForEventCompletion.setProperty( LF_SIZE_OF_FILE_BYTES_STRING, sizeOfFile );
	m_namedPropertiesForEventCompletion.setProperty( LF_FILE_PATH_STRING, m_filePathToLoad );
	m_namedPropertiesForEventCompletion.setProperty( LF_FILE_NAME_STRING, m_fileName );

	m_jobCompleted = true;
}


size_t LoadFileJob::determineSizeOfFile( FILE* fileToDetermineSize ) {

	FATAL_ASSERTION( fileToDetermineSize != nullptr, "Error: Cannot determine size of a FILE which is nullptr" );

	size_t startPosition = 0;
	size_t endPosition = 0;

	startPosition = ftell( fileToDetermineSize );
	fseek( fileToDetermineSize, 0, SEEK_END );
	endPosition = ftell( fileToDetermineSize );
	fseek( fileToDetermineSize, startPosition, SEEK_SET );

	return endPosition;
}