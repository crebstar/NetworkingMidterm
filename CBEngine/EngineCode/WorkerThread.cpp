#include "WorkerThread.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h> // Ugh
#include <process.h>

#include "GameDirector.hpp"

#include "AsyncJob.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

STATIC void WorkerThread::WorkerThreadEntryFunction( void* workerThreadObject ) {

	WorkerThread* workerThreadToRun =  static_cast<WorkerThread*>( workerThreadObject );
	workerThreadToRun->workExecutionFunction();
}


WorkerThread::~WorkerThread() {

}


WorkerThread::WorkerThread( TypeOfJobWork threadJobWorkType ) {

	m_typeOfJobWork = threadJobWorkType;
	
	setWorkerThreadDefaults();
}


void WorkerThread::workExecutionFunction() {

	cbengine::GameDirector* sharedGameDirector = cbengine::GameDirector::sharedDirector();
	JobManager& sharedJobManager = JobManager::getSharedJobManager();

	while ( !sharedGameDirector->m_isQuiting ) {

		AsyncJob* jobToPerform = sharedJobManager.getAsyncJob( m_typeOfJobWork );

		if ( jobToPerform != nullptr ) {

			m_workerThreadStatus = THREAD_STATUS_WORKING;
			jobToPerform->executeJob();

			sharedJobManager.reportCompletedAsyncJob( jobToPerform );
		}
		
		m_workerThreadStatus = THREAD_STATUS_IDLE;

		Sleep( WORKER_THREAD_WHILE_LOOP_SLEEP_DURATION );
	}
	
	m_flaggedForDeletion = true;
}


void WorkerThread::setWorkerThreadDefaults() {

	m_workerThreadStatus = THREAD_STATUS_IDLE;
	m_flaggedForDeletion = false;
}


const bool WorkerThread::isFlaggedForDeletion() const {

	return m_flaggedForDeletion;
}