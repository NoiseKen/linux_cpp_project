#ifndef _FILE_CTRL_H_
#define _FILE_CTRL_H_

#include <linux/limits.h>
#include <string>

class FileCtrl
{
public:
	static int get_current_root(std::string &root);
	static std::string get_date_time(void);
	std::string const fileName;
	//create a brand new file
	uint64_t file_size(std::string file="");
	//is file exist
	bool is_exist(std::string file="");
	//create a brand new file
	int create(std::string file="");
	//delete specific file	
	int destory(std::string file="");
	FileCtrl(std::string fn);
	~FileCtrl(void);
protected:	
private:
	
};

#endif //_FILE_CTRL_H_
