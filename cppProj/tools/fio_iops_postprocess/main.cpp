#include <iostream>
#include <string>
#include "average.h"
#include "string_list.h"

#define OUT_LOG_NAME	"iops.log"

int
strToInt(std::string str)
{
	return atoi(str.c_str());
}

//raw : raw log file reference
//second : second of time stamp
//return : avg iops (same job avg, different job plus)
unsigned int
iops_calculate(StringList &log, unsigned int second)
{
	Average<unsigned int> avg(128);
	unsigned int maxTimeStamp;
	unsigned int minTimeStamp;
	unsigned int logSize;
	unsigned int iops;
	unsigned int job;
	unsigned int i;
	
	ExtensionString ext(",");
	bool jobChange;
	jobChange=false;
	job=0;
	iops = 0;
	minTimeStamp = second*1000;// - 500;
	maxTimeStamp = second*1000 + 999;	
	logSize = log.size();
	for(i=0;i<logSize;i++)
	{
		unsigned int val;
		ext.string = log[i];
		val = strToInt(ext[0]);
		if((minTimeStamp<=val) && (val<=maxTimeStamp))
		{
			jobChange=false;
			avg << strToInt(ext[1]);
		}
		else
		{
			if(!jobChange)
			{
				avg >> val;
				//std::cout << "job[" << job++ << "]: avg iops = " << val
				//		  << ", sample count = " << avg.count << std::endl;
				iops += val;
				avg.clear();
				jobChange=true;
			}
		}	
	}
	job^=0;
	return iops;
}

int 
main(int argc, char *argv[])
{
	StringList *list;
	StringList *save;
	std::string msg;
	
	if(argc<2)
	{
		std::cout << "please give fio iops log file" << std::endl;
		return -1;
	}
	
	list = new StringList();
	
	if(0!=list->load_from(argv[1]))
	{
		msg = "Open file ";
		msg += argv[1];
		msg += " fail\n";
		std::cout << msg;
		return -1;
	}
	
	std::cout << "line number =" << list->size() << std::endl;
	//find maximum time 
	unsigned int i;
	unsigned int maxTime=0;
	unsigned int val;
	ExtensionString *log;
	
	log = new ExtensionString(",");
	for(i=0;i<list->size();i++)
	{
		log->string = (*list)[i];
		val = strToInt((*log)[0]);
		if(maxTime<val)
		{
			maxTime = val;
		}
	}
	
	std::cout << "max time = " << maxTime << std::endl;
	//add all time zone element
	ExtensionString *ext;
	unsigned int j;
	ext = new ExtensionString(",");
	ext->resize(2);
	maxTime=maxTime/1000;	//maxTime now is save file line number

	save = new StringList();

	//unsigned int fetchNumber;
	for(j=0;j<maxTime;j++)
	{
		std::cout<<"S["<<j<<"]:";
		(*ext)[0] = std::to_string(j);
		val = iops_calculate(*list, j);
		std::cout<< val << std::endl;
		(*ext)[1] = std::to_string(val);
		save->push_back(ext->string);			
	}
	
	std::cout << "OK\n";
	save->save_to(OUT_LOG_NAME);
	std::cout << "output file : " << OUT_LOG_NAME << std::endl;
	
	delete list;
	delete save;
	delete log;
	delete ext;
	return 0;
}
