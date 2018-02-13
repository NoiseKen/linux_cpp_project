#ifndef __FORM_APP_H__
#define __FORM_APP_H__
#include <QPlastiqueStyle>
#include "event_ctrl.h"
#include "ui_main_window.h"
#include "vcl_nvme_dev.h"
#include "property.h"
#include "nvme_io.h"

class FormApp : public Ui_MainWindow
{
public:
	properties_ro<FormApp, class VCLNVMeDev *> nvmeDev;
	property_ro<FormApp, unsigned int> nvmeDevCnt;
	FormApp(void);
	~FormApp(void);	
protected:
private:	
	QStringList opGrpList;
	class VCLNVMeDev * _f_get_nvme_dev(unsigned int idx);
	unsigned int _f_get_nvme_dev_cnt(void);
	QMainWindow *vcl;
	void on_btn_click(QObject *sender);
	void on_action_trigger(QObject *sender);
	void on_index_change(QObject *sender);
	QPlastiqueStyle *sheetStyle;
	class EventCtrl *_event;
	void clr_nvme_dev_list(void);
	void enum_nvmd_dev_list(void);
	void event_connect(void);
	void gui_init(void);
	#define CB_INDEX(cb) cb->currentIndex()
	#define CB_VALUE(cb) cb->itemData(cb->currentIndex(), Qt::UserRole).toUInt()
	#define CB_TEXT(cb)	 cb->itemText(cb->currentIndex())
	#define LE_VALUE(le) le->text().toUInt()

};

#endif //ndef __FORM_APP_H__
