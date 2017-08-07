#include "SafeLog.h"

#include <thread>
#include <string>

using namespace std::chrono;

SafeLog::InnerSafeLog::InnerSafeLog(const std::string & filePath) :
	logFile_(filePath),
	isRunning_(false)
{
	if (logFile_.is_open())
	{
		isRunning_ = true;
		std::thread thread(std::bind(&InnerSafeLog::mainLoop, this));
		thread.detach();
	}
}

void SafeLog::InnerSafeLog::pushMessage(const std::string & message)
{
	mutex_.lock();
	messageQueue_.push(message);
	mutex_.unlock();
}

void SafeLog::InnerSafeLog::mainLoop()
{
	while (isRunning_)
	{
		if (messageQueue_.size() > 0)
		{
			mutex_.lock();
			logFile_ << messageQueue_.front();
			messageQueue_.pop();
			mutex_.unlock();
		}
	}
	logFile_.close();
	delete this;
}

SafeLog::SafeLog(const std::string & filePath)
{
	innerLog_ = new InnerSafeLog(filePath);
	labelNameMap[MessageType::ERROR] = "[Error]: ";
	labelNameMap[MessageType::WARNING] = "[Warning]: ";
	labelNameMap[MessageType::DEBUG] = "[Debug]: ";
}

SafeLog::~SafeLog()
{
	innerLog_->stop();
}

SafeLog & SafeLog::operator<<(const std::string & message)
{
	innerLog_->pushMessage(message + "\n\n");
	return *this;
}

SafeLog & SafeLog::operator()(const MessageType & messageType)
{
	innerLog_->pushMessage(labelNameMap[messageType]);
	return *this;
}