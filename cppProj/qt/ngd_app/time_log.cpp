#include <time.h>
#include "time_log.h"

void TimeLog::currentDateTime(QString &date)
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "[%F@%R:%S]", &tstruct);
	date = QString(buf);
}

class TimeLog & TimeLog::operator<<(QString msg)
{
	bufferLine += msg;
	return *this;
}

class TimeLog & TimeLog::operator<<(TimeLogCode lc)
{
	QString str;
	
	switch(lc)
	{
		case Time:
			currentDateTime(bufferLine);
			break;
		case Err:
			bufferLine += "[ERR]";
			break;
		case Endl:
			append(bufferLine);
			bufferLine.clear();
			break;
		default:
			append(QString().sprintf("[undef lc=0x%08X]\n", lc));
			break;
	}
	
	return *this;
}

TimeLog::TimeLog(QWidget *parent) : QTextEdit(parent)
{
}

TimeLog::~TimeLog(void)
{
}
