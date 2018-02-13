#ifndef __SYSTEM_TASK_H__
#define __SYSTEM_TASK_H__
#include <QProcess>
#include <QThread>
#include "property.h"
#include "report_channel.h"

class SystemTask : public QThread
{
public:
	enum{
		systemTaskCancel=1234,
	};
	static void delay(unsigned int ms);	
	void call(QString cmd, QStringList arg);
	void run(void);
	int exitCode;
	property_ro<SystemTask, QProcess::ProcessState> state;	
	property_ro<SystemTask, QString> stdOut;	
	SystemTask(void);
	~SystemTask(void);
protected:
	//void on_stdout_update(void){}
private:
	//bool logUpdating;
	ReportChannel repCh;
	//QString out;
	QProcess::ProcessState _f_get_current_state(void);
	QString _f_get_std_out(void);	
	QProcess *process;	
};


#endif //ndef __SYSTEM_TASK_H__
