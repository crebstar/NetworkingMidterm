#include "JobManager.hpp"

#include "EngineErrorManager.hpp"
#include "EventSystem.hpp"
#include "WorkerThread.hpp"
#include "AsyncJob.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

JobManager::~JobManager() {

	DeleteCriticalSection( &m_csJobsToDoRegistry );
	DeleteCriticalSection( &m_csCompletedJobs );
	DeleteCriticalSection( &m_csWorkerThreads );
}


void JobManager::setJobManagerDefaults() {

	TypeOfJobWork jobWorkType;
	for ( size_t i = 0; i < NUM_JOB_TYPES; ++i ) {

		jobWorkType = static_cast<TypeOfJobWork>( i );
		JobPriorityQueue jobPQueue;

		m_jobsToDoRegistry.insert( std::pair<TypeOfJobWork,JobPriorityQueue>( jobWorkType, jobPQueue ) );
	}

	InitializeCriticalSection( &m_csJobsToDoRegistry );
	InitializeCriticalSection( &m_csCompletedJobs );
	InitializeCriticalSection( &m_csWorkerThreads );
}


void JobManager::update( float deltaSeconds ) {

	UNUSED( deltaSeconds );
	// TODO:: Logic to run jobs if there are no threads

	EnterCriticalSection( &m_csCompletedJobs );

	performJobIfNoWorkerThreads();

	for ( size_t i = 0; i < m_completedJobs.size(); ++i ) {

		AsyncJob* completedJob = m_completedJobs[i];
		cbengine::FireEvent( completedJob->m_eventNameToFireUponCompletion, completedJob->m_namedPropertiesForEventCompletion );

	}

	for ( size_t i = 0; i < m_completedJobs.size(); ++i ) {

		AsyncJob* jobToDelete = m_completedJobs[i];
		delete jobToDelete;
	}

	m_completedJobs.clear();

	LeaveCriticalSection( &m_csCompletedJobs );
}


void JobManager::performJobIfNoWorkerThreads() {

	//EnterCriticalSection( &m_csJobsToDoRegistry ); // Causes lag

	if ( m_workerThreads.empty() ) {
		// Do the job yourself

		JobsTodoRegisterIterator itJob;
		for ( itJob = m_jobsToDoRegistry.begin(); itJob != m_jobsToDoRegistry.end(); ++itJob ) {

			JobPriorityQueue& jobQueue = itJob->second;

			if ( !jobQueue.empty() ) {

				AsyncJob* job = jobQueue.top();
				jobQueue.pop();

				job->executeJob();
				this->reportCompletedAsyncJob( job );
			}
		}
	}

	//LeaveCriticalSection( &m_csJobsToDoRegistry );
}


int JobManager::addAsyncJobToWorkerThreadQueue( AsyncJob* jobToAdd, TypeOfJobWork jobType ) {

	EnterCriticalSection( &m_csJobsToDoRegistry );

	FATAL_ASSERTION( jobToAdd != nullptr, "Error: Cannot add an AsyncJob to the job queue which is equal to nullptr." );

	JobsTodoRegisterIterator itJobTodo = m_jobsToDoRegistry.find( jobType );
	
	FATAL_ASSERTION( itJobTodo != m_jobsToDoRegistry.end(), "Error: Job type category not recognized! Cannot add job to the queue" );

	JobPriorityQueue& jobTypePQueue = itJobTodo->second;
	jobTypePQueue.push( jobToAdd );

	LeaveCriticalSection( &m_csJobsToDoRegistry );
}


AsyncJob* JobManager::getAsyncJob( const TypeOfJobWork& typeOfJobWork ) {

	EnterCriticalSection( &m_csJobsToDoRegistry );

	AsyncJob* jobToReturn = nullptr;
	JobsTodoRegisterIterator itJobTodo = m_jobsToDoRegistry.find( typeOfJobWork );

	FATAL_ASSERTION( itJobTodo != m_jobsToDoRegistry.end(), "Error: Job type category not recognized! Cannot get job from the job queue" );

	JobPriorityQueue& jobTypePQueue = itJobTodo->second;
	
	if ( !jobTypePQueue.empty() ) {

		jobToReturn = jobTypePQueue.top();
		jobTypePQueue.pop();
	}

	LeaveCriticalSection( &m_csJobsToDoRegistry );

	return jobToReturn;
}


void JobManager::reportCompletedAsyncJob( AsyncJob* completedJob ) {

	EnterCriticalSection( &m_csCompletedJobs );

	m_completedJobs.push_back( completedJob );

	LeaveCriticalSection( &m_csCompletedJobs );
}


void JobManager::createWorkerThread( TypeOfJobWork workerThreadJobType ) {

	EnterCriticalSection( &m_csWorkerThreads );

	WorkerThread* workerThread = new WorkerThread( workerThreadJobType );
	m_workerThreads.push_back( workerThread );

	_beginthread( &WorkerThread::WorkerThreadEntryFunction, 0, static_cast<void*>( workerThread ) );

	LeaveCriticalSection( &m_csWorkerThreads );
}