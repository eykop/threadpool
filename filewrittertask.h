#ifndef FILEWRITTERTASK_H
#define FILEWRITTERTASK_H

#include "itask.h"
#include <string>
#include <fstream>
#include <vector>

class FileWritterTask : public ITask
{
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

#endif // FILEWRITTERTASK_H
