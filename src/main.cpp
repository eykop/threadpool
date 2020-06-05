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
        case userinpututils::ThreadCommand::STOP_ALL: {
            for (int i = 0; i < tp.threadsCount(); i++) {
                tp.stopThread(i);
            }
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
    tp.setNotifyOnAllFinished(std::bind([&app]() {
        app.exit();
    }));
    tp.startPool();

    // we manage the threads/tasks in a separate thread sho that we allow the application to work on other things.
    // we could make this run in main thread, but then it would be diffcult to gently exits the application.
    // as the manage function will block the main thread and the app exit call will happen before we start the application
    // event loop which in turn is a blocking call, and will not be triggered correctly so we end up with the application
    // never exiting as a result of that.
    std::thread managingThread([&tp] { manageThreadPool(tp); });

    //make sure we allow the thread to finish before be termintae the application.
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&managingThread]() { managingThread.join(); });

    return app.exec();
}
