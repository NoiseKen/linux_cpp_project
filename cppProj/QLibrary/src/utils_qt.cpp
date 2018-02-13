#include <stdio.h>
#include <QFile>
#include <QTextStream>
#include <QWidget>
#include <unistd.h>
#include "utils_qt.h"
#include "value_list.h"
#ifdef OS_LINUX
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "io_ctrl.h"
#include "debug.h"
const QString UtilsQT::tmpFile=".tmputilsqt00";
#endif//def OS_LINUX
/*
char *
UtilsQT::qstrig_to_char(QString str)
{
	QByteArray qb;
	qb = str.toLocal8Bit();
	return qb.data();
}
*/
QString 
UtilsQT::load_text_file(QString filePath)
{
	QString str;
	QFile *fn;		
	fn = new QFile(filePath);
	if (fn->open(QIODevice::ReadOnly))
	{
		QTextStream *fnStream;
		fnStream = new QTextStream(fn);
		//txtReport->setPlainText(fnStream->readAll());
		str = fnStream->readAll();
		delete fnStream;
	}
		
	delete fn;
	return str;	
}

void
UtilsQT::extend_all_object(QObject *from, QObjectList &to)
{
    QObjectList *children;
    QObjectList::iterator item;
    children = new QObjectList();
    *children = from->children();
    to += *children;
    for(item=children->begin();item<children->end();item++)
    {
        extend_all_object(*item, to);
    }
    delete children;
}

void *
UtilsQT::aligned_malloc(int size, int pow)
{
    unsigned int align;
    void *mem;
    void **ptr;
    align = 1<<pow;
    mem = malloc(size+align+sizeof(void*));
    if(mem!=NULL)
        ptr = (void**)(((long)mem+align+sizeof(void*)) & ~((long)align-1));
    else
        ptr = NULL;
    ptr[-1] = mem;
    return ptr;
}

void
UtilsQT::aligned_free(void *ptr)
{
    free(((void**)ptr)[-1]);
}


QString
UtilsQT::printf_128(char *int128)
{
    QString str;
    QString d;
    int loop;
    unsigned char *p;

    p = (unsigned char *)&int128[(128/8)-1];
    loop = 128/8;
    str.clear();
    while(loop--)
    {
        str += d.sprintf("%02X",*p--);
    }

    return str;
}

void
UtilsQT::all_children_set_enable(QObject *parent, bool en, QObjectList &expect)
{
    QObjectList all;
    QWidget *widget;
    extend_all_object(parent, all);
    QObjectList::iterator item;
    for(item=all.begin();item<all.end();item++)
    {
        QObject *obj;
        obj = *item;
        if(obj->inherits("QWidget"))
        {
            widget = (QWidget*)obj;
            widget->setEnabled(en);
        }
    }

    for(item=expect.begin();item<expect.end();item++)
    {
        widget = (QWidget*)*item;
        widget->setEnabled(!en);
    }

}

#ifdef OS_LINUX
void 
UtilsQT::flush_stdin(bool waitFor1stByte)
{
    char ch;
    int bytesWaiting=0;
    
    if(waitFor1stByte)
    {
		ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);
		while(bytesWaiting)
		{		
			ch = read(0,&ch,1);
			bytesWaiting--;
		}   		
	}
    
    
    do{
		ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);
		if(bytesWaiting!=0)
		{
			waitFor1stByte=false;
		}
	}while(waitFor1stByte);
    
	while(bytesWaiting)
	{		
		ch = read(0,&ch,1);
		bytesWaiting--;
	}    
    
}

int UtilsQT::stdin_ready_byte(void)
{
	int bytesWaiting=0;
	ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);
	return bytesWaiting;
}

int
UtilsQT::get_eth0_ip4(QString &ip4)
{
	int fd;
	int ioStatus=-1;
	IOCtrl *io;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	io = new IOCtrl(fd);
	if(io->bConnected)
	{
		struct ifreq ifr;
		/* I want to get an IPv4 IP address */
		ifr.ifr_addr.sa_family = AF_INET;

		/* I want IP address attached to "eth0" */
		strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

		//ioStatus = ioctl(fd, SIOCGIFADDR, &ifr);
		ioStatus = io->pass_through(SIOCGIFADDR, &ifr);		

		if(ioStatus==0)
		{
			ip4.sprintf("%s", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
		}
	}
	delete io;
	return ioStatus;
}

int
UtilsQT::identify_system_call(QProcess *process, HtmlString &msg)
{
	int result;

	if(!process->waitForStarted())
	{
		result = systemCallBlocked;
		msg << (HtmlString::ColorStr){HtmlString::colorRed, "not be executed"};
	}
	else
	{
		if(!process->waitForFinished())
		{
			result = systemCallTimeOut;
			msg << (HtmlString::ColorStr){HtmlString::colorRed, "fail"};
		}
		else
		{
			QString stdOut;
			result = process->exitCode();
			msg << QString("Done, exit code = %1\n").arg(result);
			stdOut = QString("%1").arg(QString::fromAscii(process->readAll()));	

			stdOut.remove(stdOut.length()-1,1);

			if(!stdOut.isEmpty())
			{
				HtmlString::ColorStr cstr;
				//msg << QString("------------------- Output -------------\n%1").arg(stdOut);
				if(result!=0)
				{				
					cstr.color = HtmlString::colorRed;					
				}
				else
				{
					cstr.color = HtmlString::colorDefault;
				}
				cstr.string = stdOut;
				msg << cstr << "\n";
			}
		}
	}
	
	return result;
}

int 
UtilsQT::identify_system_call(QProcess *process, QString &msg)
{
	int result;

	if(!process->waitForStarted())
	{
		result = systemCallBlocked;
		msg += "not be executed";
	}
	else
	{
		if(!process->waitForFinished(1000))
		{
			result = systemCallTimeOut;
			msg += "fail";
		}
		else
		{
			result = process->exitCode();
			msg += QString("%1").arg(QString::fromAscii(process->readAll()));	
		}
	}
	
	return result;	
}

int
UtilsQT::system_call(QString cmd, QStringList arg, HtmlString &msg)
{
	int result;
	QProcess *process;
	process = new QProcess();
	process->setProcessChannelMode(QProcess::MergedChannels);
	process->start(cmd, arg);

	result = identify_system_call(process, msg);
	delete process;
	return result;		
}

int
UtilsQT::system_call(QString cmd1, QStringList arg1, QString cmd2, QStringList arg2, HtmlString &msg)
{
	int result;
	QProcess *process1;
	QProcess *process2;
	process1 = new QProcess();
	process2 = new QProcess();	
	process2->setProcessChannelMode(QProcess::MergedChannels);
	process1->setStandardOutputProcess(process2);
	
	process1->start(cmd1, arg1);
	process2->start(cmd2, arg2);
	
	process1->waitForFinished();
	result = identify_system_call(process2, msg);
	delete process1;
	delete process2;
	return result;		
}

int
UtilsQT::system_call(QString cli, StringList &stdOut)
{
	int result;
	cli += QString(" > %1").arg(tmpFile);
	result = system(cli.toLocal8Bit().data());
	stdOut.load_from(tmpFile.toStdString());
	cli = QString("rm -f %1").arg(tmpFile);
	system(cli.toLocal8Bit().data());
	return result;	
}

#endif //def OS_LINUX

