#ifndef __UTILS_H__
#define __UTILS_H__
#include <QObject>
#include <QString>
#include <QProcess>
#include "string_list.h"
#include "html_string.h"

#define APP_CONFIG_FILE_NAME	"config.dat"
#define INVALID_ITEM			0xFFFFFFFF

#ifndef __EOS
#define __EOS	0
#endif

class UtilsQT
{
public:
	enum{
		systemCallTimeOut=0xABABABAB,
		systemCallBlocked = 0xBABABABA,		
	};
//static const int systemCallTimeOut;
//static const int systemCallBlocked;
    //char *qstrig_to_char(QString);
    void extend_all_object(QObject *from, QObjectList &to);
	void all_children_set_enable(QObject *parent, bool en, QObjectList &expect);
    static QString load_text_file(QString filePath);
    static void * aligned_malloc(int size, int pow);
    static void aligned_free(void *ptr);
    QString printf_128(char *int128);
#ifdef OS_LINUX
	int system_call(QString cmd, QStringList arg, HtmlString &msg);	
	int system_call(QString cmd1, QStringList arg1, QString cmd2, QStringList arg2, HtmlString &msg);
	int system_call(QString cli, StringList &stdOut);
	static int get_eth0_ip4(QString &ip4);
	static void flush_stdin(bool waitFor1stByte);
	static int stdin_ready_byte(void);
private:
	static int identify_system_call(QProcess *process, HtmlString &msg);
	static int identify_system_call(QProcess *process, QString &msg);
	static const QString tmpFile;
#endif//def OS_LINUX
};




#endif //ndef __UTILS_H__
