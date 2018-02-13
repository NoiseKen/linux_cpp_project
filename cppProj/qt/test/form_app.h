#ifndef FORM_APP_H
#define FORM_APP_H

#include <QMainWindow>
#include "ui_main_window.h"


class FormApp : public Ui_FormApp
{    
public:
    FormApp(void);
    ~FormApp(void);
private:
    QMainWindow *vcl;
    class EventCtrl *_event;
    void event_connect(void);
    void on_btn_click(QObject *sender);
    void on_value_change(QObject *sender);
};

#endif // FORM_APP_H
