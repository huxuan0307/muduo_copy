#ifndef _LOGFILE_H_
#define _LOGFILE_H_

#include "../include/MyGlobal.h"
MUDUO_COPY_BEGIN
FILEUTIL_BEGIN

FILEUTIL_END

class LogFile
{
	NONCOPYABLE (LogFile)
public:
	LogFile (const string& basename,
		long rollSize,
		bool threadSafe = true,
		int flushInterval = 3,
		int checkEveryN = 1024);

private:

};

MUDUO_COPY_END
#endif // !_LOGFILE_H_