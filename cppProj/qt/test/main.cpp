#include "form_app.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#if 0
    QApplication a(argc, argv);
    FormApp w;
    w.show();

    return a.exec();
#else
    int result;
    FormApp *form;
    QApplication *app;
    app = new QApplication(argc, argv);
    form = new FormApp();
    //app->setStyle(STYLE);
    //app->setStyle("QPlastiqueStyle");
    result = app->exec();
    delete form;
    delete app;
    return result;
#endif
}
