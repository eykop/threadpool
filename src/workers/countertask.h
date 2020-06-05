/**
    countertask.h
    Purpose: Provide an example for implementing the ITask interface inorder to be allowed to run in the thread pool.
    IT just count to required Number, while sleeping between each count to allow good demonstration of
    pause, resume , stop and status capabilities of the interface.

    @author Eyas Kopty
*/

#pragma once

#include "src/interface/itask.h"

class CounterTask : public ITask {
private:
    int mI = 1;
    int mCounter = 0;
    virtual bool doWork() override;

public:
    /**
        Constructor, assign required number to count until.
    */
    explicit CounterTask(const int counter);
    virtual ~CounterTask() = default;
};
