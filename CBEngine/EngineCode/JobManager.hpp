#ifndef included_JobManager
#define included_JobManager
#pragma once

#include <vector>
#include <set>
#include <queue>
#include <unordered_map>

#define WIN32_LEAN_AND_MEAN
#include <windows.h> // Ugh
#include <process.h>

#include "EngineMacros.hpp"
#include "AsyncJob.hpp"

const unsigned int MAX_WORKER_THREADS = 8; // TODO:: Not used at the moment


typedef enum {

	JOB_TYPE_FILE_IO,
	JOB_TYPE_GENERIC,
	JOB_TYPE_HASH_ENCRYPT,
	NUM_JOB_TYPES
} TypeOfJobWork;


struct OrderByJobPriorityLevel {
public:
	bool operator() ( const AsyncJob* lhs, const AsyncJob* rhs ) {

		return lhs->getJobPriorityLevel() < rhs->getJobPriorityLevel();
	}
};

class WorkerThread;


typedef std::priority_queue<AsyncJob*,std::vector<AsyncJob*>, OrderByJobPriorityLevel> JobPriorityQueue;
typedef std::unordered_map<TypeOfJobWork,JobPriorityQueue> JobsTodoRegistery;
typedef std::unordered_map<TypeOfJobWork,JobPriorityQueue>::iterator JobsTodoRegisterIterator;

class JobManager {
public:
	friend class WorkerThread;
	friend class AsyncJob;

	static JobManager& getSharedJobManager() {

		static JobManager jobManager;
		return jobManager;
	}


	~JobManager();

	void update( float deltaSeconds );

	// Core Functions
	void createWorkerThread( TypeOfJobWork workerThreadJobType );
	int addAsyncJobToWorkerThreadQueue( AsyncJob* jobToAdd, TypeOfJobWork jobType );

protected:

	JobManager() {

		setJobManagerDefaults();
	}

	AsyncJob* getAsyncJob( const TypeOfJobWork& typeOfJobWork );
	void reportCompletedAsyncJob( AsyncJob* completedJob );

	void setJobManagerDefaults();
private:
	PREVENT_COPY_AND_ASSIGN( JobManager );

	void performJobIfNoWorkerThreads();

	mutable CRITICAL_SECTION	        m_csJobsToDoRegistry;
	mutable CRITICAL_SECTION			m_csCompletedJobs;
	mutable CRITICAL_SECTION			m_csWorkerThreads;
	JobsTodoRegistery					m_jobsToDoRegistry;
	std::vector<AsyncJob*>				m_completedJobs;
	std::vector<WorkerThread*>			m_workerThreads;
};

#endif