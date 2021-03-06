/**********************************
Creation:
SafeLog logger() - creation without running
SafeLog logger("/logs/connection.log") - creation with running

Reset:
logger.reset("/logs/otherlog.log") - use this method when you need to change a logfile or to running logger if you have used default constructor

Write an error message:
logger(SL_ERROR) << "some error"

Write a warning message:
logger(SL_WARNING) << "some warning message"

Write a debug message:
logger(SL_DEBUG) << "some debug message"

***********************************/

#pragma once

#ifndef SL_ERROR
#define SL_ERROR safelog::SafeLog::MessageType::ERROR
#endif //SL_ERROR

#ifndef SL_WARNING
#define SL_WARNING safelog::SafeLog::MessageType::WARNING
#endif //SL_WARNING

#ifndef SL_DEBUG
#define SL_DEBUG safelog::SafeLog::MessageType::DEBUG
#endif //SL_DEBUG

#include <fstream>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace safelog {

	class ILoginWritter {
	public:
		virtual ILoginWritter & operator<<(const std::string & message) = 0;
	};

	class SafeLog
	{
	public:

		enum class MessageType
		{
			ERROR,
			WARNING,
			DEBUG
		};

		SafeLog();
		SafeLog(const std::string & filePath);
		~SafeLog();

		//SafeLog & operator<<(const std::string & message);
		ILoginWritter & operator()(const MessageType & messageType);

		void reset(const std::string & filePath);

	private:

		class InnerSafeLog
		{
		public:
			InnerSafeLog();
			InnerSafeLog(const std::string & filePath);

			void reset(const std::string & filePath);
			void pushMessage(const std::string & message);
			void stop() { isRunning_ = false; hasMessage_.notify_one(); }

		private:
			bool isRunning_;

			std::ofstream logFile_;
			std::queue<std::string> messageQueue_;
			std::mutex mutex_;
			std::condition_variable hasMessage_;

			void mainLoop();
			void safePop();
		}  *innerLog_;

		class InnerLoginWritter : public ILoginWritter {
		public:
			InnerLoginWritter(SafeLog & sl) : sl_(sl) {}
			ILoginWritter & operator<<(const std::string & message) override;
			void SetLabel(const std::string & label) { label_ = label; }

		private:
			SafeLog & sl_;
			std::string label_;

			friend SafeLog;
		} logginWritter_;

		std::string getTimeAsString();
	};

}
