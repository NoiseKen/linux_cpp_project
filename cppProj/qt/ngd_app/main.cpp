#include "form_app.h"

//#define STYLE	"windows"
#define STYLE	"GTK"


int main(int argc, char *argv[])
{
	int result;
	FormApp *form;
	QApplication *app;
	app = new QApplication(argc, argv);
	form = new FormApp();	
	app->setStyle(STYLE);
	//app->setStyle("QPlastiqueStyle");
	result = app->exec();
	delete form;
	delete app;
	return result;
}

