#include "itask.h"

void ITask::setNotifyOnFinished(const std::function<void()> notifyOnFinished)
{
    mNotifyOnFinished = notifyOnFinished;
}

void ITask::pause()
{
    std::lock_guard<std::mutex> lkg(mStateMutex);
    mStatus = STATUS::PAUSED;
    std::cout << "paused.." << std::endl;
}

void ITask::checkPaused()
{
    //std::cout<<"check paused.."<<mPaused<< status()<<std::endl;
    while (mStatus == STATUS::PAUSED) {
        std::unique_lock<std::mutex> ulk(mStateMutex);
        mStateCondtionVar.wait(ulk);
    }
}

void ITask::resume()
{
    std::lock_guard<std::mutex> lkg(mStateMutex);
    mStatus = STATUS::RUNNING;
    std::cout << "resumed.." << std::endl;
    mStateCondtionVar.notify_one();
}

void ITask::stop()
{
    std::cout << "stopped.." << std::endl;
    std::lock_guard<std::mutex> lkg(mStateMutex);
    mStatus = STATUS::STOPPED;
    mStateCondtionVar.notify_one();
    mNotifyOnFinished();
}

std::string ITask::status()
{
    switch (mStatus) {
    case STATUS::INITIALIZED:
        return "Initialized";
    case STATUS::RUNNING:
        return "Running";
    case STATUS::PAUSED:
        return "Paused";
    case STATUS::STOPPED:
        return "Stopped";
    case STATUS::FINISHED:
        return "Finished";
    default:
        return "None";
    }
}

void ITask::run()
{
    mStatus = STATUS::RUNNING;
    std::cout << "running.." << std::endl;
    //WHILE WORKING!!!
    while (doWork()) {
        checkPaused();
        if (mStatus == STATUS::STOPPED) {
            //do cleanup (close open files /sockets and exit stopping!!)
            return;
        }
    }
    //in case we asked to pause just after that dowork has finished and before we exited our run method!
    checkPaused();
    if (mStatus == STATUS::STOPPED) {
        //do cleanup (close open files /sockets and exit stopping!!)
        return;
    }
    mStatus = STATUS::FINISHED;
    mNotifyOnFinished();
    std::cout << "finished.." << std::endl;
}
