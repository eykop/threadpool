#include "filereadertask.h"

#include <sstream>
#include <thread>


FileReaderTask::FileReaderTask(const std::string& filePath)
{

    mFilePath = filePath;
    init();
}

void FileReaderTask::init(){

    mFin.open(mFilePath.c_str(), std::ios::binary | std::ios::in);

    //get file size...
    if(mFin.is_open()){

        mFin.seekg(0, std::ios::beg);
        int begin = mFin.tellg();
        mFin.seekg(0, std::ios::end);
        int end = mFin.tellg();
        mFin.seekg(0, std::ios::beg);
        mFileSize = end - begin;

    }
    mBuffer.resize(mBufferSize);

}
/**
    Implement the inhertied pure method, will read the file aprt after part while counting how many
    words the file contains, it will return true if it didn't reach the
    end of the file , and will return false otherwise indicating its finished(read full file).

    @return bool, ture if there is more work to do, false if work is done.
*/
bool FileReaderTask::doWork(){

    if(mFin.is_open()){

        mBuffer.clear();
        mBuffer.resize(mBufferSize);
        mFin.read(&mBuffer[0], mBufferSize);

        //count how many words in file
        std::istringstream iss(mBuffer);
        std::string word;
        while(std::getline(iss, word, ' ')){
            mWordsCount++;
            //count chars also to slow the file read!
            for(auto &c: word){
                mCharCount++;
            }
        }

        //get how many bytes we actually have read !
        mTotalReadedSize+=mFin.gcount();
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(1ms);
        return (mTotalReadedSize < mFileSize);
    }else{
        //file was not open return false!
        return false;
    }
}

FileReaderTask::~FileReaderTask(){
     std::cout<<"number of words in file is: "<<mWordsCount<<std::endl;
     std::cout<<"number of characters in file is: "<<mCharCount<<std::endl;
    if(mFin.is_open()){
        mFin.close();
    }

}
