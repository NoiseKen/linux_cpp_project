#ifndef __DEBUG_H__
#define __DEBUG_H__
#include <iostream>
#include <QString>



class Debug{
public:

class Debug& operator<<(QString msg)
//{ printf("%s\n", msg.toLocal8Bit().data()); return *this;};
{
	std::cout << msg.toStdString(); return *this;
};

Debug(void){};
~Debug(void){};

private:

};


#endif //ndef __DEBUG_H__
