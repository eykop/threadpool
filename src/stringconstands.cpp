#include "stringsconstants.h"


    const char* StringsConstants::HelpMessage = "\r\nThreadPool Program Description: \r\n"
                                "This application will start a number of threads provided by command line argument of 'threads'\r\n"
                                "and assign tasks to each thread.\r\n"
                                "It will then allow the user to control each thread and perform the following operation while\r\n"
                                "providing the thread id to control.\r\n"
                                "Allowed operations on thread:\r\n"
                                "1. pause example: pause 1\r\n"
                                "2. resume example: resume 1\r\n"
                                "3. stop example: stop 1\r\n"
                                "4. status example: status 1\r\n";
const char* StringsConstants::ThreadCountOptionDescription = "start the application with specified number of threads, example: ./threadpool threads 10";
const char* StringsConstants::InvalidThreadCountInputErrorMessage = "' which is an invalid value for option --threads,\r\n"
                                                                          "please provide a valid number larger than 0.";
const char* StringsConstants::RequestForInputMessage = "Please provide an input command with a thread number:";
const char* StringsConstants::RequestForValidThreadNumberMessage = "Please provid a valid thread number, you have provide a non number input: '";
const char* StringsConstants::RequestForInRangeThreadNumberMessage ="Please provid a valid thread number, you have provide a thread number large than whats is cvurrently running: '";
const char* StringsConstants::UnknownCommnadMessage ="Opps, You have provided an unknown option please provide on of the following : stop , pause, resume or status \r\n"
                                                           "followed by whitespace followed by the thread number.";
const char* StringsConstants::DoneMessage = "All threads are done running the tasks while waiting for user input, will ignore command and exit now.";

