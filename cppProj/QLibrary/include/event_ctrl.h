#ifndef __EVENT_H__
#define __EVENT_H__
#include <QObject>
//#include <QEvent>
#include <QPushButton>
//#include <QLineEdit>
#include "form_app.h"
class FormApp;

class EventCtrl : QObject
{
typedef void (FormApp::*EventHandler)(QObject *);	
	Q_OBJECT	
private:
	class FormApp *owner;
protected:
	//bool eventFilter(QObject *obj, QEvent *event);	
public:	
	EventHandler btn_click_callback;
	EventHandler action_trigger_callback;
	EventHandler index_change_callback;
	EventHandler text_change_callback;
	EventHandler value_change_callback;
	EventCtrl(class FormApp *, QObject *);
	void specific_connect(QPushButton* btn);
	~EventCtrl(void);
public slots:
     void on_btn_click(void);
     void on_index_change(void);
     void on_action_trigger(void);
     void on_text_change(void);
     void on_value_change(void);
};

#endif //ndef __EVENT_H__
