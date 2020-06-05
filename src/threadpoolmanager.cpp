#include "threadpoolmanager.h"
#include "stringsconstants.h"
#include "threadpool.h"
#include "userinpututils.h"

namespace threadpoolmanager {

void manageThreadPool(ThreadPool& tp)
{
    tp.startPool();

    while (!tp.finished()) {

        const auto userThreadCommand = userinpututils::getUserThreadCommandInput();

        if (userThreadCommand.threadIndex + 1 > tp.threadsCount()) {
            std::cout << StringsConstants::RequestForInRangeThreadNumberMessage << userThreadCommand.threadIndex << "'" << std::endl;
            continue;
        }

        switch (userThreadCommand.command) {

        case userinpututils::ThreadCommand::PAUSE: {
            tp.pauseThread(userThreadCommand.threadIndex);
            break;
        }
        case userinpututils::ThreadCommand::RESUME: {
            tp.resumeThread(userThreadCommand.threadIndex);
            break;
        }
        case userinpututils::ThreadCommand::STATUS: {
            std::cout << "The thread number: '" << userThreadCommand.threadIndex << "' status is : '"
                      << tp.getThreadStatus(userThreadCommand.threadIndex) << "'" << std::endl;
            break;
        }
        case userinpututils::ThreadCommand::STOP: {
            tp.stopThread(userThreadCommand.threadIndex);
            break;
        }
        case userinpututils::ThreadCommand::STOP_ALL: {
            for (int i = 0; i < tp.threadsCount(); i++) {
                tp.stopThread(i);
            }
            break;
        }
        case userinpututils::ThreadCommand::INVALID: {
            std::cout << StringsConstants::UnknownCommandMessage << std::endl;
            break;
        }
        }

        //to do std::bind to notify of done for each thread instead of looping!
        if (tp.finished()) {
            std::cout << StringsConstants::DoneMessage << std::endl;
            break;
        }
    }
}

}
