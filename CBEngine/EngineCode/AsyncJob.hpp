#ifndef included_AsyncJob
#define included_AsyncJob
#pragma once

#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h> // Ugh
#include <process.h>

#include "EngineMacros.hpp"

#include "NamedProperties.hpp"
#include "NamedProperty.hpp"

typedef enum {

	JOB_PRIORITY_VERY_LOW = 0,
	JOB_PRIORITY_LOW,
	JOB_PRIORITY_AVERAGE,
	JOB_PRIORITY_HIGH,
	JOB_PRIORITY_VERY_HIGH,
	JOB_PRIORITY_URGENT,
} JobPriorityLevel;



class AsyncJob {
public:
	friend class JobManager;

	virtual ~AsyncJob();
	AsyncJob( const std::string& eventToFireUponCompletion, JobPriorityLevel jobPriorityLevel = JOB_PRIORITY_AVERAGE );

	virtual void executeJob() = 0;

	void setEventNameForCompletion( const std::string& eventNameToFireUponCompletion );
	void setJobPriorityLevel( JobPriorityLevel priorityLevelToSet );
	const JobPriorityLevel getJobPriorityLevel() const;
	const bool isJobBeingExecuted() const;

protected:

	std::string						m_eventNameToFireUponCompletion;
	NamedProperties					m_namedPropertiesForEventCompletion;

	void setAsyncJobDefaults();
private:
	PREVENT_COPY_AND_ASSIGN( AsyncJob );

	mutable CRITICAL_SECTION		m_csJobPriorityLevel;
	bool							m_isBeingExecuted;
	JobPriorityLevel				m_jobPriorityLevel;

};

#endif