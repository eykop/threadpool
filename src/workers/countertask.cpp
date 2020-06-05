#include "countertask.h"
#include "thread"

CounterTask::CounterTask(const int counter)
    : ITask()
    , mCounter(counter)
{
}

/**
    Implement the inherited pure method, will increment the counter and return true if it didn't reach the
    numer provided int the constructor false otherwise indicating its finished(reach the number).

    @return bool, ture if there is more work to do, false if work is done.
*/
bool CounterTask::doWork()
{
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(2000));
    //std::cout<<"FileReadTask : " <<mI<<std::endl;
    mI++;
    if (mI == mCounter) {
        return false;
    }
    return true;
}
