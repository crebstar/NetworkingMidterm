#ifndef included_WorkerThread
#define included_WorkerThread
#pragma once

#include "EngineMacros.hpp"

#include "JobManager.hpp"

typedef enum {

	THREAD_STATUS_IDLE,
	THREAD_STATUS_WORKING,

} WorkerThreadStatus;

const long WORKER_THREAD_WHILE_LOOP_SLEEP_DURATION = 0L;

class WorkerThread {
public:
	static void WorkerThreadEntryFunction( void* workerThreadObject );

	~WorkerThread();
	WorkerThread( TypeOfJobWork threadJobWorkType );

	void workExecutionFunction();

	const TypeOfJobWork& getTypeOfJobWork() const;
	const WorkerThreadStatus& getWorkerThreadStatus() const;

	const bool isFlaggedForDeletion() const;
protected:

	void setWorkerThreadDefaults();
private:
	PREVENT_COPY_AND_ASSIGN( WorkerThread );

	TypeOfJobWork				m_typeOfJobWork;
	WorkerThreadStatus			m_workerThreadStatus;

	mutable bool				m_flaggedForDeletion;
};

#endif