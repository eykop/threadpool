/**
    itask.h
    Purpose: Provide an abstract interface for tasks allowed to run in thread pool.
    It also implements pause, resume , stop and status query on current task process.

    @author Eyas Kopty
*/

#ifndef ITASK_H
#define ITASK_H

#include <condition_variable>
#include <mutex>
#include <string>
#include <iostream>


class ITask{

protected:
    enum STATUS { INITIALIZED, RUNNING, PAUSED, STOPPED, FINISHED };
    std::condition_variable mStateCondtionVar;
    std::mutex mStateMutex;
    bool mPaused = false;
    bool mStopped = false;
    STATUS mStatus = STATUS::INITIALIZED;

public:
    ITask();

    /**
        Pure method, inherting classes must implement this methoid, this were
        actually the task will do its work, this method should work with small chuck of the work on each call
        as it will called iterativly to allow pasue and resume for the task.
        it will return false when there is no more work to do(when its done),
        otherwise it will return true.

        @return bool, ture if there is more work to do, false if work is done.
    */
    virtual bool doWork() = 0;


    /**
        Pauses the task.
        @return void.
    */
    void pause();

    /**
        Check if task is current in paused status.
        @return void.
    */
    void checkPaused();

    /**
        resumes task.
        @return void.
    */
    void resume();

    /**
        stops task.
        @return void.
    */
    void stop();

    /**
        Query task status.
        @return std::string, task status.
    */
    std::string status();

    /**
        Run the tasks, entry point where we start running the task.
        @return void.
    */
    virtual void run();

    virtual ~ITask();
};
#endif // ITASK_H