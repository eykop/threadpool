#pragma once
#include "src/interface/itask.h"

#include <fstream>
#include <string>
#include <vector>

class FileWritterTask : public ITask {
private:
    std::string mFilePath;
    std::ofstream mFout;
    std::string mBuffer;
    int mFileSize = 0;
    int mTotalWrittenSize = 0;
    virtual bool doWork() override;

    /**
        Init the work (open of file for write).
    */
    void init();

public:
    FileWritterTask(const std::string& filePath, const int requiredFileSize);
    virtual ~FileWritterTask();
};
