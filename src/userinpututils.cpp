#include "userinpututils.h"
#include "stringsconstants.h"

#include <iostream>

#include <QCommandLineOption>

namespace {

userinpututils::UserCommand parseUserThreadCommandInput(const std::string& inputCommand, const std::string& threadNumber)
{

    userinpututils::UserCommand userCommand;

    //check if number is convertable to int!
    try {
        userCommand.threadIndex = std::stoi(threadNumber);
    } catch (std::invalid_argument& ia) {
        std::cout << StringsConstants::RequestForValidThreadNumberMessage << threadNumber << "'" << std::endl;
        return userCommand;
    }

    if (inputCommand == "pasue") {
        userCommand.command = userinpututils::ThreadCommand::PAUSE;
    } else if (inputCommand == "resume") {
        userCommand.command = userinpututils::ThreadCommand::RESUME;
    } else if (inputCommand == "stop" && userCommand.threadIndex != -1) {
        userCommand.command = userinpututils::ThreadCommand::STOP;
    } else if (inputCommand == "stop" && userCommand.threadIndex == -1) {
        userCommand.command = userinpututils::ThreadCommand::STOP_ALL;
    } else if (inputCommand == "status") {
        userCommand.command = userinpututils::ThreadCommand::STATUS;
    }
    return userCommand;
}

}

namespace userinpututils {

std::unique_ptr<QCommandLineParser> defineCommandLineParser(const QCoreApplication& app)
{
    auto parser = std::make_unique<QCommandLineParser>();
    parser->setApplicationDescription(StringsConstants::HelpMessage);
    parser->addHelpOption();

    const QCommandLineOption threadCountOption("threads",
        StringsConstants::ThreadCountOptionDescription,
        "threads");
    parser->addOption(threadCountOption);
    parser->process(app);
    return parser;
};

ParsedArgumentsResult parseCommandLineArguments(const std::unique_ptr<QCommandLineParser>& parser)
{

    const QStringList args = parser->optionNames();
    if (args.size() < 1) {
        std::cerr << "Error: Please specify an argument." << std::endl;
        parser->showHelp(1);
        return { 0, false };
    }

    int threadCount = 0;
    threadCount = parser->value("threads").toInt();
    if (threadCount == 0) {
        std::cerr << "You have provided : '" << parser->value("threads").toStdString()
                  << StringsConstants::InvalidThreadCountInputErrorMessage << std::endl;
        parser->showHelp(1);
        return { 0, false };
    }

    return { threadCount, true };
};

UserCommand getUserThreadCommandInput()
{

    //get command until whitespace.
    std::string inputCommand;
    std::string threadNumber;
    std::cout << StringsConstants::RequestForInputMessage << std::endl;

    std::getline(std::cin, inputCommand, ' ');

    //get Task(thread index) id after whitespace and until new line.
    std::getline(std::cin, threadNumber);

    //flush the input stream otherwise it may not block next time we read input.
    std::cin.clear();
    std::cin.sync();

    std::cout << "you have choosed to: '" << inputCommand << "' '" << threadNumber << "'." << std::endl;

    return parseUserThreadCommandInput(inputCommand, threadNumber);
};

}
