#include "hdr.h"
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>

class logger;
extern logger gLog;
#define INIT_LOGGING_SYSTEM	logger gLog
#define GLOG(gLog)			(gLog << gLog.header( __FILE__ , __LINE__ , __FUNCTION__))
#define GLOG_L(gLog, level)	gLog.setcurrlevel(level); \
									gLog << gLog.header( __FILE__ , __LINE__ , __FUNCTION__)
#define GLOG_CL(gLog, level)	{						\
									if (getlevel() & level) {		\
										gLog->setcurrlevel(level);	\
										gLog << logger->header( __FILE__ , __LINE__ , __FUNCTION__) \
									} \
								}
#define _INFO					GLOG_L(gLog, LOGINFO)
#define _ERR					GLOG_L(gLog, LOGERR)
#define _DEBUG			        GLOG_L(gLog, LOGDEBUG)
#define _CRETICAL				GLOG_L(gLog, LOGCRITICAL)
#define _PANIC					GLOG_L(gLog, LOGPANIC)

typedef enum {
	LOGINFO,
	LOGERR,
	LOGDEBUG,
	LOGCRITICAL,
	LOGPANIC
}log_level_t;

class logger {
	private:
		bool active = false;
		std::ofstream ofs;
	protected:
	public:
		logger() {
			consolelog = true;
			filelog = true;
			logfile = "default-logger-" + std::to_string(static_cast<int>(getpid())) + ".log";
			logheader = true;
			loglevel = LOGINFO | LOGERR | LOGDEBUG | LOGCRITICAL | LOGPANIC;
			this->ofs.open(logfile);
			if(!this->ofs.is_open()) {
				std::cout << "Unable to open " << logfile << std::endl;
			}
			active = true;
		}
		~logger() {
			this->ofs.close();
		}
		bool consolelog;
		bool filelog;
		std::string logfile;
		bool logheader;
		uint32_t loglevel;
		uint32_t currloglevel;

		void setcurrlevel(log_level_t level) {
			this->currloglevel = level;
		}
		void setlevel(log_level_t level) {
			this->loglevel |= level;
		}
		uint32_t getlevel() {
			return this->loglevel;
		}
		void resetlevel(log_level_t level) {
			this->loglevel &= ~(1 << level);
		}
		std::string getlevelstr(uint32_t level)
		{
			std::string str;
			switch (level){
				case LOGINFO:
					str = "INFO    ";
					break;
				case LOGERR:
					str = "ERROR   ";
					break;
				case LOGDEBUG:
					str = "DEBUG   ";
					break;
				case LOGCRITICAL:
					str = "CRITICAL";
					break;
				case LOGPANIC:
					str = "PANIC   ";
					break;
				default:
					str = "UNKNOWN ";
			}
			return str;
		}
		template< typename T > logger& operator<<( T data ) {
			if (this->active) {
				if (this->filelog) this->ofs << data;
				if (this->consolelog) std::cout << data;
			}
			return *this;
		}

		logger& operator<<(std::ostream& (*fun)(std::ostream&)) {
			if (this->active) {
				if (this->filelog) this->ofs << std::endl;
				if (this->consolelog) std::cout << std::endl;
			}
			return *this;
		}
		std::string header(const std::string& file, const int line, const std::string& func ) {
			std::string hdr;
			if (this->logheader) {
				hdr += CurrentTimeStr() + " :";
				hdr += getlevelstr(this->currloglevel) + ": ";
				hdr += file + "(" + std::to_string(line) + "): ";
			} else {
				hdr += func;
			}
			return hdr;
		}
		std::string FormatTime(std::chrono::system_clock::time_point tp) {
			std::stringstream ss;
			auto t = std::chrono::system_clock::to_time_t(tp);
			auto tp2 = std::chrono::system_clock::from_time_t(t);
			if (tp2 > tp)
				t = std::chrono::system_clock::to_time_t(tp - std::chrono::seconds(1));
			ss  << std::put_time(std::localtime(&t), "%Y-%m-%d %T")
				<< "." << std::setfill('0') << std::setw(3)
				<< (std::chrono::duration_cast<std::chrono::milliseconds>(
							tp.time_since_epoch()).count() % 1000);
			return ss.str();
		}

		std::string CurrentTimeStr() {
			return FormatTime(std::chrono::system_clock::now());
		}
};
