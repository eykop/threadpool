/**
    threadpool.h
    Purpose: initiate number of threads while assigning task
    to each thread , is also allow to control progress of tasks
    while providing stop, pause , resume and status operation on each thread.

    @author Eyas Kopty
*/

#pragma once
#include "src/interface/itask.h"

#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

class ThreadPool {
private:
    std::vector<std::thread> mThreads;
    std::mutex mMutex;
    std::unordered_map<int, std::unique_ptr<ITask>> mTasksMap;
    int mThreadsCount = 0;
    int mFinishedThreadsCount = 0;
    std::function<void()> mNotifyOnFinished = nullptr;

public:
    /**
        Constructor, assign required number of threads to run in the thread pool.
    */
    explicit ThreadPool(int numberOfThreads);
    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool& operator=(const ThreadPool& other) = delete;

    /**
        startPool a thread  pool while assigning tasks to required threads.
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
    [[nodiscard]] std::string getThreadStatus(int tId);

    /**
        Starts a thread while running provided task.

        @param ITask shared pointer, the task the thread should run.
        @return void.
    */
    void startThread(int tId, std::unique_ptr<ITask> task);

    /**
        Return current number of running threads.
        @return int, current number of running threads.
    */
    [[nodiscard]] int threadsCount() const;

    /**
      Updates finished threads count.
    */
    void updateFinishedCount();

    /**
      Check if all threads in pool are finished or not.
    */
    [[nodiscard]] bool finished() const;

    ~ThreadPool();

    /**
     * @brief Sets the callback to notify when all threads are finished
     * @param notifyOnAllFinished
     */
    void setNotifyOnAllFinished(const std::function<void()>& notifyOnAllFinished);
};
