#include <unistd.h>
#include <dirent.h>
#include "directory_ctrl.h"


int 
DirectoryCtrl::create(std::string dir)
{	
	int sc;
	std::string cmd;
	if(dir=="") dir=directoryName;
	cmd = "mkdir " + dir;
	sc = system(cmd.c_str());
	return sc;	
}

int 
DirectoryCtrl::destory(std::string dir)
{
	int sc;
	std::string cmd;
	if(dir=="") dir=directoryName;	
	cmd = "mkdir " + dir;
	sc = system(cmd.c_str());	
	return sc;
}

bool 
DirectoryCtrl::is_exist(std::string dir)
{
	bool exist;
	DIR *dp;
	if(dir=="") dir=directoryName;
	dp = opendir(dir.c_str());
	exist = (dp==NULL)?false:true;
	closedir(dp);
	return exist;
}

DirectoryCtrl::DirectoryCtrl(std::string dir) : directoryName(dir)
{
}

DirectoryCtrl::~DirectoryCtrl(void)
{
}
