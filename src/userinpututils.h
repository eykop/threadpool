#pragma once

#include <memory>
#include <string>

#include <QCommandLineParser>
#include <QCoreApplication>

namespace userinpututils {

struct ParsedArgumentsResult {
    int threadsCount = 0;
    bool successOnParsing = false;
};

std::unique_ptr<QCommandLineParser> defineCommandLineParser(const QCoreApplication& app);

ParsedArgumentsResult parseCommandLineArguments(const std::unique_ptr<QCommandLineParser>& parser);

enum class ThreadCommand { INVALID,
    PAUSE,
    RESUME,
    STOP,
    STATUS,
    STOP_ALL };

struct UserCommand {
    ThreadCommand command = ThreadCommand::INVALID;
    int threadIndex = -1;
};

UserCommand getUserThreadCommandInput();

}
