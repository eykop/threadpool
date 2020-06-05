#include "threadpool.h"
#include "threadpoolmanager.h"
#include "userinpututils.h"

#include <QCoreApplication>

int main(int argc, char* argv[])
{

    QCoreApplication app(argc, argv);
    const auto parser = userinpututils::defineCommandLineParser(app);
    auto commandArgsParsingResult = userinpututils::parseCommandLineArgumnets(parser);

    if (!commandArgsParsingResult.successOnParsing) {
        return 1;
    }

    ThreadPool tp(commandArgsParsingResult.threadsCount);
    tp.setNotifyOnAllFinished([&app]() {
        QCoreApplication::exit();
    });

    // we manage the threads/tasks in a separate thread sho that we allow the application to work on other things.
    // we could make this run in main thread, but then it would be difficult to gently exits the application.
    // as the manage function will block the main thread and the app exit call will happen before we start the application
    // event loop which in turn is a blocking call, and will not be triggered correctly so we end up with the application
    // never exiting as a result of that.
    std::thread managingThread([&tp] { threadpoolmanager::manageThreadPool(tp); });

    //make sure we allow the thread to finish before be terminate the application.
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&managingThread]() { managingThread.join(); });

    return app.exec();
}
