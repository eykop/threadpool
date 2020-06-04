#include "threadpool.h"
#include "src/workers/filereadertask.h"
#include "stringsconstants.h"

#include <string>

#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QString>


int main(int argc, char *argv[])
{

    QCoreApplication app(argc, argv);
    QCommandLineParser parser;
    parser.setApplicationDescription(StringsConstants::HelpMessage.c_str());
    parser.addHelpOption();

    const QCommandLineOption threadCountOption("threads",
                                               StringsConstants::ThreadCountOptionDescription.c_str(),
                                               "threads");
    parser.addOption(threadCountOption);
    parser.process(app);

    const QStringList args = parser.optionNames();
    if (args.size() < 1) {
        std::cerr<<"Error: Please specify an argument."<<std::endl;
        parser.showHelp(1);
        return 1;
    }

    int threadCount = 0;
    threadCount  = parser.value("threads").toInt();
    if (threadCount == 0){
        std::cerr<<"You have provided : '"<<parser.value("threads").toStdString()
                <<StringsConstants::InvalidThreadCountInputErrorMessage<<std::endl;
        parser.showHelp(1);
        return 1;
    }

    ThreadPool tp(threadCount);
    tp.startPool();
    while(!tp.waitForAllToBeFinished()){

        //get command until whitespace.
        std::string inputCommand;
        std::cout<<StringsConstants::RequestForInputMessage<<std::endl;
        std::cout<<StringsConstants::HelpMessage<<std::endl;
        std::getline(std::cin, inputCommand, ' ');

        //get treadid after whitespace and until new line.
        std::string threadNumber;
        std::getline(std::cin, threadNumber);
        std::cout<<"you have choosed to: '"<<inputCommand<<"' '" <<threadNumber<<"'."<<std::endl;;

        //check if number is convertable to int!
        int intThreadNumber = -1;
        try{
            intThreadNumber = std::stoi(threadNumber);
        }catch(std::invalid_argument &ia){
            std::cout<<StringsConstants::RequestForValidThreadNumberMessage<<threadNumber<<"'"<<std::endl;
            continue;
        }

        if (intThreadNumber+1 > threadCount){
            std::cout<<StringsConstants::RequestForInRangeThreadNumberMessage<<intThreadNumber<<"'"<<std::endl;
            continue;
        }
        if (inputCommand == "pause"){
            tp.pauseThread(intThreadNumber);
        }else if (inputCommand == "status"){
            std::cout<<"The thread number: '"<<intThreadNumber<<"' status is : '"
                    <<tp.getThreadStatus(intThreadNumber)<<"'"<<std::endl;
        }else if (inputCommand == "resume"){
            tp.resumeThread(intThreadNumber);
        }else if (inputCommand == "stop"){
            tp.stopThread(intThreadNumber);
        }else{
            std::cout<<StringsConstants::UnknownCommnadMessage<<std::endl;
        }
        if(tp.getNumberOfCurrentRunningThreads() == 0){
            std::cout<<StringsConstants::DoneMessage<<std::endl;
            break;
        }
    }
    std::cout<<"done : "<<tp.getNumberOfCurrentRunningThreads()<<std::endl;

    return 0;//app.exec();
}
