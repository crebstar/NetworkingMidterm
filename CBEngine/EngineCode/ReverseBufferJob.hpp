#ifndef included_ReverseBufferJob
#define included_ReverseBufferJob
#pragma once

#include "AsyncJob.hpp"

#include "EngineMacros.hpp"

const std::string REVERSE_BUFFER_RESULT_STRING = "ReverseBufferResult";
const std::string REVERSE_BUFFER_SIZE_STRING = "ReverseBufferSize";


class ReverseBufferJob : public AsyncJob {
public:
	virtual ~ReverseBufferJob();
	ReverseBufferJob( char* bufferToReverse, size_t sizeOfBuffer, const std::string& eventNameToFireOnCompletion, JobPriorityLevel priorityLevel = JOB_PRIORITY_AVERAGE );

	virtual void executeJob();
protected:

private:
	PREVENT_COPY_AND_ASSIGN( ReverseBufferJob );

	char*				m_bufferToReverse;
	size_t				m_sizeOfBuffer;
	mutable bool		m_reverseBufferComplete;
};



#endif