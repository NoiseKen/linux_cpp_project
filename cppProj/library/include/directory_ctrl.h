#ifndef _DIRECTORY_CTRL_H_
#define _DIRECTORY_CTRL_H_

#include <linux/limits.h>
#include <string>

class DirectoryCtrl
{
public:	
	std::string const directoryName;
	//is directory exist
	bool is_exist(std::string dir="");
	//create a brand new file
	int create(std::string dir="");
	//delete specific file	
	int destory(std::string dir="");
	DirectoryCtrl(std::string dir="");
	~DirectoryCtrl(void);
protected:	
private:
	
};

#endif //_FILE_CTRL_H_
