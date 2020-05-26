/**
    threadpool.h
    Purpose: initiate number of threads while assiging task
    to each thread , is also allow to cotrol progress of tasks
    while providing stop, puase , resume and status operation on each thread.

    @author Eyas Kopty
*/

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <mutex>
#include <iostream>
#include <unordered_map>
#include <condition_variable>
#include <memory>
#include <thread>

#include "itask.h"

class ThreadPool
{
private:

    std::vector<std::thread> mThreads;
    std::mutex mMutex;
    std::unordered_map<int,std::shared_ptr<ITask>> mTasksMap;
    int mNumberOfThreads = 0;
    std::condition_variable mStartContion;

public:
    /**
        Constructor, assign required number of threads to run in the thread pool.
    */
    ThreadPool(int numberOfThreads);
    ThreadPool(const ThreadPool& other) = delete;

    /**
        startPool a thread  pool while assignig tasks to required threads.
        @return void.
    */
    void startPool();

    /**
        Pauses a running thread by its thread id.

        @param tId int, the thread id which we want to pause.
        @return void.
    */
    void pauseThread(int tId);

    /**
        Resumes a paused thread by its thread id.

        @param tId int, the thread id which we want to resume.
        @return void.
    */
    void resumeThread(int tId);

    /**
        Stops a thread by its thread id.

        @param tId int, the thread id which we want to stop.
        @return void.
    */
    void stopThread(int tId);

    /**
        Return a thread status by its thread id.

        @param tId int, the thread id which we want to get its status.
        @return std::string, thread status.
    */
    std::string getThreadStatus(int tId);

    /**
        Starts a thread while running provided task.

        @param ITask shared pointer, the task the thread should run.
        @return void.
    */
    void startThread(int tId, std::shared_ptr<ITask> task);

    /**
        Check if all threads are done.
        @return bool, true if all threads are done false otherwise
    */
    bool waitForAllToBeFinished();

    /**
        Return current number of running threads.
        @return int, current number of running threads.
    */
    int getNumberOfCurrentRunningThreads() const;

    ~ThreadPool();


};

#endif // THREADPOOL_H
