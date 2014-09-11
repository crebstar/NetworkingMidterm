#ifndef included_HashBufferJob
#define included_HashBufferJob
#pragma once

#include "AsyncJob.hpp"

#include "EngineMacros.hpp"

const std::string HASH_BUFFER_RESULT_STRING = "HashBufferResult";
const std::string HASH_BUFFER_CONTENT = "HashBufferContent";
const std::string HASH_BUFFER_SIZE = "HashBufferSize";

class HashBufferJob : public AsyncJob {
public:

	virtual ~HashBufferJob();
	HashBufferJob( const char* bufferToHash, size_t sizeOfBuffer, const std::string& eventToFireUponCompletion, JobPriorityLevel jobPriorityLevel = JOB_PRIORITY_AVERAGE );

	virtual void executeJob();

protected:


private:
	PREVENT_COPY_AND_ASSIGN( HashBufferJob );

	const char*		m_bufferToHash;
	size_t			m_sizeOfBuffer;
	unsigned int	m_hashValue;
	mutable bool    m_hashComplete;
};


#endif