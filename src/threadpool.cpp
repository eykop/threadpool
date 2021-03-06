/**
    threadpool.cpp
    Purpose: initiate number of threads while assigning task
    to each thread , is also allow to control progress of tasks
    while providing stop, pause , resume and status operation on each thread.

    @author Eyas Kopty
*/

#include "threadpool.h"
#include "workers/countertask.h"
#include "workers/filereadertask.h"
#include "workers/filewritertask.h"

ThreadPool::ThreadPool(int numberOfThreads)
    : mThreadsCount(numberOfThreads)
{
    mThreads.reserve(numberOfThreads);
}

int ThreadPool::threadsCount() const
{
    return mThreadsCount;
}

void ThreadPool::updateFinishedCount()
{
    {
        //lock to access map...
        std::unique_lock<std::mutex> ulk(mMutex);
        mFinishedThreadsCount++;
    }
}

bool ThreadPool::finished() const
{
    std::cout << "all finished \n";
    if (mFinishedThreadsCount == mThreadsCount) {
        mNotifyOnFinished();
        return true;
    } else {
        return false;
    }
}

void ThreadPool::startPool()
{
    for (int i = 0; i < mThreadsCount; i++) {
        std::unique_ptr<ITask> tsk_shptr;
        if (i >= 0 && i < 3) {
            std::cout << "create counter" << std::endl;
            tsk_shptr = std::make_unique<CounterTask>(30);
            tsk_shptr->setNotifyOnFinished(std::bind(&ThreadPool::updateFinishedCount, this));
            mThreads.push_back(std::thread(&ThreadPool::startThread, this, i, std::move(tsk_shptr)));
        } else if (i >= 3 && i < 6) {
            std::cout << "create file writer" << std::endl;
            tsk_shptr = std::make_unique<FileWriterTask>(std::string("largeFile" + std::to_string(i) + ".txt"), 1000000000);
            tsk_shptr->setNotifyOnFinished(std::bind(&ThreadPool::updateFinishedCount, this));
            mThreads.push_back(std::thread(&ThreadPool::startThread, this, i, std::move(tsk_shptr)));
        } else {
            std::cout << "create file reader" << std::endl;
            tsk_shptr = std::make_unique<FileReaderTask>("largeFile.txt");
            tsk_shptr->setNotifyOnFinished(std::bind(&ThreadPool::updateFinishedCount, this));
            mThreads.push_back(std::thread(&ThreadPool::startThread, this, i, std::move(tsk_shptr)));
        }
    }
}

void ThreadPool::startThread(int tId, std::unique_ptr<ITask> task)
{

    {
        //lock to access map...
        std::unique_lock<std::mutex> ulk(mMutex);
        mTasksMap.emplace(tId, std::move(task));
    }

    mTasksMap[tId]->run();
}

/**
    Assure all threads are joined before the threadpool object is destructed.
*/
ThreadPool::~ThreadPool()
{

    for (unsigned int i = 0; i < mThreads.size(); i++) {
        if (mThreads[i].joinable()) {
            mThreads[i].join();
        }
    }
}

// will pause thread if its only in a running status otherwise it will ignore request and prompt user.
void ThreadPool::pauseThread(int tId)
{
    std::cout << "The thread number: '" << tId << "' is null : '" << (mTasksMap[tId] == nullptr) << std::endl;

    if (mTasksMap[tId] != nullptr && mTasksMap[tId]->status() == "Running") {
        mTasksMap[tId]->pause();
    } else {
        std::cout << "The thread number: '" << tId << "' status is : '" << mTasksMap[tId]->status() << "', can't be pasues." << std::endl;
    }
}

// will resume thread if its only in a paused status otherwise it will ignore request and prompt user.
void ThreadPool::resumeThread(int tId)
{
    if (mTasksMap[tId] != nullptr && mTasksMap[tId]->status() == "Paused") {
        mTasksMap[tId]->resume();
    } else {
        std::cout << "The thread number: '" << tId << "' status is : '" << mTasksMap[tId]->status() << "', can't be resumed." << std::endl;
    }
}

// will resume thread if its only in a not stopped status, otherwise if a thread is already stopped it will ignore request
// and prompt user.
void ThreadPool::stopThread(int tId)
{
    if (mTasksMap[tId] != nullptr && mTasksMap[tId]->status() != "Stopped" && mTasksMap[tId]->status() != "Finished") {
        mTasksMap[tId]->stop();
    } else {
        std::cout << "The thread number: '" << tId << "' status is : '" << mTasksMap[tId]->status() << "', can't be stopped." << std::endl;
    }
}

std::string ThreadPool::getThreadStatus(int tId)
{
    if (mTasksMap[tId] != nullptr) {
        return mTasksMap[tId]->status();
    } else {
        return "something wrong happened should not get here!, please check thread with id of :" + std::to_string(tId);
    }
}

void ThreadPool::setNotifyOnAllFinished(const std::function<void()>& notifyOnAllFinished)
{
    mNotifyOnFinished = notifyOnAllFinished;
}
