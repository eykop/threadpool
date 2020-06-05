#include "src/workers/filereadertask.h"
#include "stringsconstants.h"
#include "threadpool.h"
#include "userinpututils.h"

#include <string>

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QString>

void manageThreadPool(ThreadPool& tp)
{
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
        case userinpututils::ThreadCommand::INVALID: {
            std::cout << StringsConstants::UnknownCommnadMessage << std::endl;
            break;
        }
        }

        //to do std::bind to notirfy of done for each thread instead of looping!
        if (tp.finished()) {
            std::cout << StringsConstants::DoneMessage << std::endl;
            break;
        }
    }
}

int main(int argc, char* argv[])
{

    QCoreApplication app(argc, argv);
    const auto parser = userinpututils::defineCommandLineParser(app);
    auto commandArgsParsingResult = userinpututils::parseCommandLineArgumnets(parser);

    if (!commandArgsParsingResult.successOnParsing) {
        return 1;
    }

    const auto threadCount = commandArgsParsingResult.numberOfThreads;
    ThreadPool tp(threadCount);
    tp.startPool();
    manageThreadPool(tp);
    std::cout << "done : " << tp.threadsCount() << std::endl;

    return app.exec();
}
