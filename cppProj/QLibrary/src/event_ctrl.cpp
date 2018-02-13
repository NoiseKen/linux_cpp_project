#include <iostream>
#include "utils_qt.h"
#include "event_ctrl.h"

void EventCtrl::on_btn_click(void)
{
	if(btn_click_callback)
	(owner->*btn_click_callback)(sender());
}

void EventCtrl::on_action_trigger(void)
{
	if(action_trigger_callback)
	(owner->*action_trigger_callback)(sender());
}

void EventCtrl::on_index_change(void)
{
	if(index_change_callback)
	(owner->*index_change_callback)(sender());
}

void EventCtrl::on_text_change(void)
{
	if(text_change_callback)
	(owner->*text_change_callback)(sender());
}

void EventCtrl::on_value_change(void)
{
	if(value_change_callback)
	(owner->*value_change_callback)(sender());
}

#if 0
bool EventCtrl::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::FocusIn)
    {
		//QLineEdit *lineEdit;
		//lineEdit = (QLineEdit *)obj;		
		//lineEdit->deselect();
        //lineEdit->selectAll();
        //return false;
        return true;
    }
    else
    {
        return QObject::eventFilter(obj, event);
    }
}
#endif

EventCtrl::EventCtrl(class FormApp *own, QObject *objBase)
{
	//Form App assign
	owner = own;
	//connect event
	QObjectList *objList;
	objList = new QObjectList();
    UtilsQT().extend_all_object(objBase, *objList);
	QObjectList::iterator item;
	for(item = objList->begin();item < objList->end();item++)
	{				
		QObject *obj;
		obj = *item;
		if(!obj->objectName().isEmpty())
		{
			if(obj->inherits("QAction"))
			{
				QObject::connect(obj, SIGNAL(triggered()), this,  SLOT(on_action_trigger()));
			}
			else
			if(obj->inherits("QAbstractButton"))
			{
				QObject::connect(obj, SIGNAL(clicked()), this,  SLOT(on_btn_click()));
			}			
			else
			if(obj->inherits("QComboBox"))
			{
				QObject::connect(obj, SIGNAL(currentIndexChanged(int)), this,  SLOT(on_index_change()));
			}
			else			
			if(obj->inherits("QLineEdit"))
			{
				//((QLineEdit *)obj)->installEventFilter(this);
				QObject::connect(obj, SIGNAL(textChanged(const QString &)), this,  SLOT(on_text_change()));
			}
            else
            //if(obj->inherits("QSlider"))
            if(obj->inherits("QAbstractSlider"))
			{
				//((QLineEdit *)obj)->installEventFilter(this);
				QObject::connect(obj, SIGNAL(valueChanged(int)), this,  SLOT(on_value_change()));
			}			
		}
	}
	delete objList;
	//init event handler
	btn_click_callback = NULL;
	action_trigger_callback = NULL;
	index_change_callback = NULL;
	text_change_callback = NULL;
	value_change_callback = NULL;
		
}

void EventCtrl::specific_connect(QPushButton* btn)
{
	QObject::connect(btn, SIGNAL(clicked()), this,  SLOT(on_btn_click()));	
}

EventCtrl::~EventCtrl(void)
{
}

