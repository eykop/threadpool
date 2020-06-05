#include "filewritertask.h"

#include <thread>

FileWriterTask::FileWriterTask(const std::string& filePath, int requiredFileSize)
    : mFilePath(filePath)
    , mFileSize(requiredFileSize)
{
    mBuffer = "Adding a new line to file ...\r\n";
    init();
}

void FileWriterTask::init()
{
    mFout.open(mFilePath.c_str(), std::ios::binary | std::ios::out);
    if (!mFout.is_open()) {
        std::cout << "failed to open file" << std::endl;
    }
}

/**
    Implement the inhertied pure method, will create a new file part after part while,
    until we reach required file size provided in constructor,
    it will return true if it didn't reach the reach required size of the file,
    and will return false otherwise indicating its finished(wrote full file size).

    @return bool, ture if there is more work to do, false if work is done.
*/
bool FileWriterTask::doWork()
{

    if (mFout.is_open()) {

        mFout.write(&mBuffer[0], mBuffer.size());
        mFout.flush();

        mTotalWrittenSize += mBuffer.size();
        std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1));
        return (mTotalWrittenSize < mFileSize);
    } else {
        //file was not open return false!
        return false;
    }
}

FileWriterTask::~FileWriterTask()
{

    if (mFout.is_open()) {
        mFout.close();
    }
}
