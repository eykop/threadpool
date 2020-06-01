/**
    threadpool.cpp
    Purpose: initiate number of threads while assiging task
    to each thread , is also allow to cotrol progress of tasks
    while providing stop, puase , resume and status operation on each thread.

    @author Eyas Kopty
*/

#include "threadpool.h"
#include "src/workers/filereadertask.h"
#include "src/workers/filewrittertask.h"
#include "src/workers/countertask.h"

ThreadPool::ThreadPool(int numberOfThreads) : mNumberOfThreads(numberOfThreads){
    mThreads.reserve(numberOfThreads);
}

int ThreadPool::getNumberOfCurrentRunningThreads() const
{
    return mNumberOfThreads;
}

void ThreadPool::startPool(){

    for(int i = 0; i < mNumberOfThreads ; i++){
        std::unique_ptr<ITask> tsk_shptr;
        if (i >= 0 && i < 3){
            std::cout<<"create counter"<<std::endl;
            tsk_shptr = std::make_unique<CounterTask>(30);
            mThreads.push_back(std::thread(&ThreadPool::startThread, this, i, std::move(tsk_shptr)));
        }else if (i >= 3 && i < 6){
            std::cout<<"create file writter"<<std::endl;
            tsk_shptr = std::make_unique<FileWritterTask>(std::string("largeFile"+std::to_string(i)+".txt"), 1000000000);
            mThreads.push_back(std::thread(&ThreadPool::startThread, this, i, std::move(tsk_shptr)));
        }
        else{
            std::cout<<"create file reader"<<std::endl;
            tsk_shptr = std::make_unique<FileReaderTask>("largeFile.txt");
            mThreads.push_back(std::thread(&ThreadPool::startThread, this, i, std::move(tsk_shptr)));
        }
    }

}

void ThreadPool::startThread(int tId, std::unique_ptr<ITask> task){
            //here we invoke the task!
            task->run();

            {
                //lock to access map...
                std::unique_lock<std::mutex> ulk(mMutex);
                mTasksMap.emplace(tId, std::move(task));
            }


            {
                std::unique_lock<std::mutex> ulk(mMutex);
                --mNumberOfThreads;

            }

}

bool ThreadPool::waitForAllToBeFinished(){

    return (mNumberOfThreads == 0);
}

/**
    Assure all threads are joind before the threadpool object is destructed.
*/
ThreadPool::~ThreadPool(){

    for(unsigned int i = 0; i < mThreads.size() ; i++){
        if(mThreads[i].joinable()) {
            mThreads[i].join();
        }
    }

}

void ThreadPool::pauseThread(int tId){
    //will pasue thread if its only in a runnung status otherwise it will ignore request and prompt user.
    if(mTasksMap[tId] != nullptr && mTasksMap[tId]->status() == "Running"){
        mTasksMap[tId]->pause();
    }else{
        std::cout<<"The thread number: '"<<tId<<"' status is : '"<<mTasksMap[tId]->status()<<"', can't be pasues."<<std::endl;
    }
}

void ThreadPool::resumeThread(int tId){
    //will reumse thread if its only in a paused status otherwise it will ignore request and prompt user.
    if(mTasksMap[tId] != nullptr && mTasksMap[tId]->status() == "Paused"){
        mTasksMap[tId]->resume();
    }else{
        std::cout<<"The thread number: '"<<tId<<"' status is : '"<<mTasksMap[tId]->status()<<"', can't be resumed."<<std::endl;
    }
}

void ThreadPool::stopThread(int tId){
    //will reumse thread if its only in a not stopped status, otherwise if a thread is already stopped it will ignore request and prompt user.
    if(mTasksMap[tId] != nullptr && mTasksMap[tId]->status() != "Stopped" && mTasksMap[tId]->status() != "Finished"){
        mTasksMap[tId]->stop();
    }else{
       std::cout<<"The thread number: '"<<tId<<"' status is : '"<<mTasksMap[tId]->status()<<"', can't be stopped."<<std::endl;
    }
}

std::string ThreadPool::getThreadStatus(int tId){
    if(mTasksMap[tId] != nullptr){
        return mTasksMap[tId]->status();
    }else{
        return "something wrong happend should not get here!, please check thread with id of :" + std::to_string(tId);
    }
}
