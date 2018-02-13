#include "form_app.h"

int main(int argc, char *argv[])
{
	int result;
	FormApp *form;
	QApplication *app;
	app = new QApplication(argc, argv);
	form = new FormApp();	
	app->setStyle("GTK");
	result = app->exec();
	delete form;
	delete app;
	return result;
}

