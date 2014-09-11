#include "AsyncJob.hpp"

#include "JobManager.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

AsyncJob::~AsyncJob() {

	DeleteCriticalSection( &m_csJobPriorityLevel );
}


AsyncJob::AsyncJob( const std::string& eventToFireUponCompletion, JobPriorityLevel jobPriorityLevel ) {

	setAsyncJobDefaults();
	
	m_jobPriorityLevel = jobPriorityLevel;
	m_eventNameToFireUponCompletion = eventToFireUponCompletion;
}


void AsyncJob::setAsyncJobDefaults() {

	m_isBeingExecuted = false;
	m_jobPriorityLevel = JOB_PRIORITY_AVERAGE;

	InitializeCriticalSection( &m_csJobPriorityLevel );
}


void AsyncJob::setJobPriorityLevel( JobPriorityLevel priorityLevelToSet ) {

	EnterCriticalSection( &m_csJobPriorityLevel );

	m_jobPriorityLevel = priorityLevelToSet;

	LeaveCriticalSection( &m_csJobPriorityLevel );
}


void AsyncJob::setEventNameForCompletion( const std::string& eventNameToFireUponCompletion ) {

	EnterCriticalSection( &m_csJobPriorityLevel ); // TODO:: Possibly make own critical section object

	m_eventNameToFireUponCompletion = eventNameToFireUponCompletion;

	LeaveCriticalSection( &m_csJobPriorityLevel );
}


const JobPriorityLevel AsyncJob::getJobPriorityLevel() const {

	JobPriorityLevel tempJobPLevel;

	EnterCriticalSection( &m_csJobPriorityLevel );
	
	tempJobPLevel = m_jobPriorityLevel;

	LeaveCriticalSection( &m_csJobPriorityLevel );

	return tempJobPLevel;
}


const bool AsyncJob::isJobBeingExecuted() const {

	return m_isBeingExecuted;
}