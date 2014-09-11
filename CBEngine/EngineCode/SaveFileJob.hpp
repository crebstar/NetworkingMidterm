#ifndef included_SaveFileJob
#define included_SaveFileJob
#pragma once

#include "AsyncJob.hpp"

#include <string>
#include <cstdio>

#include "EngineMacros.hpp"

const std::string SF_FILE_NAME_RESULT_STRING = "SaveFileResult";
const std::string SF_FILE_BUFFER_STRING = "SaveFileBuffer";
const std::string SF_FILE_BUFFER_SIZE_STRING = "SaveFileSize";

class SaveFileJob : public AsyncJob {
public:

	virtual ~SaveFileJob();
	SaveFileJob( const std::string& filePathToSave, 
		char* bufferToSave, 
		size_t sizeOfBuffer, 
		const std::string eventToFireUponCompletion, 
		JobPriorityLevel priorityLevel = JOB_PRIORITY_AVERAGE );

	// NOTE:: USER IS RESPONSIBLE FOR DELETING THE MEMORY ALLOCATED FOR BUFFER PROVIDED
	virtual void executeJob();

protected:

private:
	PREVENT_COPY_AND_ASSIGN( SaveFileJob );

	std::string			m_filePath;
	const char*			m_fileBuffer;
	size_t				m_sizeOfBuffer;
	mutable bool		m_jobComplete;
};

#endif