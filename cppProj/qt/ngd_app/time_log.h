#ifndef __TIME_LOG_H__
#define __TIME_LOG_H__
#include <QTextEdit>

class TimeLog : public QTextEdit
{
public:
	typedef enum{
		Time=0,
		Err,
		Clear,
		Endl,		
	}TimeLogCode;
	class TimeLog &operator<<(TimeLogCode lc);
	class TimeLog &operator<<(QString msg);
	TimeLog(QWidget *parent);
	~TimeLog(void);
protected:
private:
	QString bufferLine;
	void currentDateTime(QString &date);
};

#endif //ndef __TIME_LOG_H__
