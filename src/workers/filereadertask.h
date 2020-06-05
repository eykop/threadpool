/**
    filereadertask.h
    Purpose: Provide an example for implemnting the ITask interface inorder to be allowed to run in the thread pool.
    Reads a large file part by part while counting how many words there are int the file , this will allow good demonstration of
    pause, resume , stop and status capabilities of the interface.

    @author Eyas Kopty
*/

#pragma once
#include "src/interface/itask.h"

#include <fstream>
#include <string>
#include <vector>

class FileReaderTask : public ITask {
private:
    std::string mFilePath;
    std::ifstream mFin;
    int mBufferSize = 1024;
    std::string mBuffer;
    int mFileSize = 0;
    int mTotalReadedSize = 0;
    int mWordsCount = 0;
    long mCharCount = 0;
    virtual bool doWork() override;

    /**
        Init the work (open of file for read).
    */
    void init();

public:
    /**
        Constructor, assign required filepath to read.
    */
    FileReaderTask(const std::string& filePath);
    virtual ~FileReaderTask();
};
