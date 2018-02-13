#ifndef __CONSOLE_CLI_H__
#define __CONSOLE_CLI_H__
#include <QTimer>
#include <QTimerEvent>
#include <QString>
#include "property.h"

class ConsoleCLI
{
public:
	int run(QString cli);
	ConsoleCLI(void);
	~ConsoleCLI(void);
protected:
private:	
	property_ro<ConsoleCLI, unsigned int> hintLine;
	property_ro<ConsoleCLI, unsigned int> commandLine;
	bool bShowHint;
	unsigned int _f_get_command_line(void);
	unsigned int _f_get_hint_line(void);
	static const int hintFlashInterval;
	static const QString hintBackToGUI;//="--- Press any key back to GUI ---";
};
#endif //ndef __CONSOLE_CLI_H__
