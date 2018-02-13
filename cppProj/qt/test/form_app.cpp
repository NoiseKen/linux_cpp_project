#include <QSerialPort>
#include <QSerialPortInfo>
#include "form_app.h"
#include "event_ctrl.h"

void
FormApp::event_connect(void)
{
    _event = new EventCtrl(this, vcl);
    _event->btn_click_callback = &FormApp::on_btn_click;
    //_event->action_trigger_callback = &FormApp::on_action_trigger;
    //_event->index_change_callback = &FormApp::on_index_change;
    //_event->text_change_callback = &FormApp::on_text_change;
    _event->value_change_callback = &FormApp::on_value_change;
    //_event->specific_connect(msgBoxBtnCancel);
}

void
FormApp::on_value_change(QObject *sender)
{
    QAbstractSlider *slider;
    slider=(QAbstractSlider *)sender;
    if(slider==dial)
    {
        spinBox->setValue(dial->value());
    }
}

void
FormApp::on_btn_click(QObject *sender)
{
    QAbstractButton *btn;
    QString msg;
    btn = (QAbstractButton *)sender;
    if(btn==pushButton)
    {
        QList<QSerialPortInfo> list;
        QList<QSerialPortInfo>::iterator item;
        list = QSerialPortInfo::availablePorts();
        msg.sprintf("COM count = %d", list.count());
        textEdit->append(msg);
        for(item=list.begin();item<list.end();item++)
        {
            msg = item->portName() + "\n";

            if(item->hasVendorIdentifier())
            {
                msg += QString().sprintf("VID : 0x%04X\n", item->vendorIdentifier());
            }

            if(item->hasProductIdentifier())
            {
                msg += QString().sprintf("PID : 0x%04X\n", item->productIdentifier());
            }

            msg += "Serial Number : " + item->serialNumber();
            msg += "\n";
            msg += "System Location : " + item->systemLocation();
            msg += "\n------------------\n";

            textEdit->append(msg);
        }

    }
}

FormApp::FormApp(void)// :
    //QMainWindow(parent)
{    
    vcl = new QMainWindow();
    //vcl = this;
    setupUi(vcl);
    event_connect();
    vcl->show();
}

FormApp::~FormApp(void)
{
    delete vcl;
    delete _event;
}
