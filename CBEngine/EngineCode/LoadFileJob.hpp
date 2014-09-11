#ifndef included_LoadFileJob
#define included_LoadFileJob
#pragma once

#include "AsyncJob.hpp"

#include <string>
#include <cstdio>

#include "EngineMacros.hpp"

const std::string LF_SIZE_OF_FILE_BYTES_STRING = "LoadFileSize";
const std::string LF_BYTE_BUFFER_RESULT_STRING = "LoadFileResult";
const std::string LF_FILE_PATH_STRING = "LoadFilePath";
const std::string LF_FILE_NAME_STRING = "LoadFileName";


class LoadFileJob : public AsyncJob {
public:
	virtual ~LoadFileJob();
	LoadFileJob( const std::string filePath, const std::string& fileName, const std::string& eventToFireUponCompletion, JobPriorityLevel priorityLevel );

	// NOTE: USER IS RESPONSIBLE FOR MEMORY MANAGEMENT OF CREATED BUFFER!
	virtual void executeJob();

protected:

	size_t determineSizeOfFile( FILE* fileToDetermineSize );

private:
	PREVENT_COPY_AND_ASSIGN( LoadFileJob );

	std::string			m_filePathToLoad;
	std::string			m_fileName;
	mutable bool		m_jobCompleted;
};

#endif