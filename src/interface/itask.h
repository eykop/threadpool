/**
    itask.h
    Purpose: Provide an abstract interface for tasks allowed to run in thread pool.
    It also implements pause, resume , stop and status query on current task process.

    @author Eyas Kopty
*/

#pragma once

#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>

class ITask {

protected:
    enum class STATUS { INITIALIZED,
        RUNNING,
        PAUSED,
        STOPPED,
        FINISHED };
    std::condition_variable mStateCondtionVar;
    std::mutex mStateMutex;
    bool mPaused = false;
    bool mStopped = false;
    STATUS mStatus = STATUS::INITIALIZED;
    std::function<void()> mNotifyOnfinished = nullptr;

public:
    ITask() = default;
    ITask& operator=(const ITask&) = delete;
    ITask(const ITask&) = delete;

    /**
        Pure method, inherting classes must implement this methoid, this were
        actually the task will do its work, this method should work with small chuck of the work on each call
        as it will called iterativly to allow pasue and resume for the task.
        it will return false when there is no more work to do(when its done),
        otherwise it will return true.

        @return bool, ture if there is more work to do, false if work is done.
    */
    [[nodiscard]] virtual bool doWork() = 0;

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
    [[nodiscard]] std::string status();

    /**
        Run the tasks, entry point where we start running the task.
        @return void.
    */
    virtual void run();

    virtual ~ITask();

    /**
     * @brief Sets the callback to notify when task is finished
     * @param notifyOnfinished
     */
    void setNotifyOnfinished(const std::function<void()> notifyOnfinished);
};
