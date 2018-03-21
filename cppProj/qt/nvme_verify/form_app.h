#ifndef __FORM_H__
#define __FORM_H__
#include <QPlastiqueStyle>
#include <QProgressBar>
#include <QTimer>
#include <QMessageBox>
#include "nvme_tool.h"
#include "event_ctrl.h"
#include "spec_dword.h"
#include "html_string.h"
#include "nvme_device.h"
#include "nvme_task.h"
#include "ui_main_window.h"
#include "property.h"


//class NVMeTask;

enum{
	DRIVER_REPLACE_KW=0,
	DRIVER_REPLACE_COMPLIANCE,
	DRIVER_REPLACE_STANDARD,
	DRIVER_REPLACE_CNT
};


class FormApp : public Ui_MainWindow, public NVMeDevice, public QTimer
{	
private:
#define QCOLOR_HI_RED	QColor(255, 0, 0)
#define QCOLOR_HI_GREEN	QColor(0, 255, 0)
#define QCOLOR_HI_BLUE	QColor(0, 0, 255)
	enum{
		TIMER_INTERVAL_WRC_TASK = 300,
	};
	QLabel *stateTitle;
	QLabel *stateStatus;
	QLabel *nsAttributeTitle;
	QLabel *nsAttributeStatus;	
	QProgressBar *wrcProgress;
	QMainWindow *_coreWin;
	class EventCtrl *_event;
	bool isIntimerEvent;
	
	//void remove_driver(bool show);
	void remove_driver(HtmlString &msg);
	void gui_init(void);	
	void event_connect(void);
	void on_btn_click(QObject *sender);
	void on_action_trigger(QObject *sender);
	void on_index_change(QObject *sender);
	void on_text_change(QObject *sender);
	void on_value_change(QObject *sender);
	int system_call(QString cmd, QStringList arg, HtmlString &msg);
	int system_call(QString cmd1, QStringList arg1, QString cmd2, QStringList arg2, HtmlString &msg);
	unsigned int _f_get_admin_cmd_op(void);	
	unsigned int _f_get_submis_cmd_op(void);
	NVMeTask::PatternType _f_get_pattern_type(void);
	NVMeTask::TestType _f_get_test_type(void);
	class SpecDWord *dword10;
	class SpecDWord *dword11;
	unsigned int lastFID;
	QPlastiqueStyle *sheetStyle;
	QString wrcLog;		//for timerEvent
	NVMeTask *wrc;
	//SystemTask *sys;
	//int systemTaskLastExitCode;
	void on_device_state_change(void);
	void on_ns_arrtibute_change(void);
	bool store_ds(void);
	void wrc_task_management(bool en, NVMeTask::Config &cfg);
	//void system_task_management(bool en, QStringList cmdArg);	
	void timerEvent(QTimerEvent * e);
	void load_report(QString filePath);
	void wrc_cfg_grab(NVMeTask::Config &cfg);
	QPushButton *msgBoxBtnCancel;
	int console_system_call(QString cli);
	void config_environment(void);
	void wrc_cancel_msg_box(bool en);
protected:
	int driverVer;
public:
	property_ro<FormApp, unsigned int> adminCmdOp;
	property_ro<FormApp, unsigned int> submisCmdOp;
	property_ro<FormApp, NVMeTask::PatternType> patternType;
	property_ro<FormApp, NVMeTask::TestType> testType;

	FormApp(void);
	~FormApp(void);	
};

#endif //ndef __FORM_H__
