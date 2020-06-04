#pragma once

#include <string>
#include <memory>

#include <QCoreApplication>
#include <QCommandLineParser>

namespace userinpututils
{

struct ParsedArgumentsResult{
    int numberOfThreads = 0;
    bool successOnParsing = false;
};

std::unique_ptr<QCommandLineParser> defineCommandLineParser(const QCoreApplication& app);

ParsedArgumentsResult parseCommandLineArgumnets(const std::unique_ptr<QCommandLineParser>& parser);

enum class ThreadCommand { INVALID, PAUSE, RESUME, STOP, STATUS };

struct UserCommand{
    ThreadCommand command = ThreadCommand::INVALID;
    int threadIndex = -1;
};


UserCommand getUserThreadCommandInput();


}
