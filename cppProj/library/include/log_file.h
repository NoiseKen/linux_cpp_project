#ifndef _LOG_FILE_H_
#define _LOG_FILE_H_

#include "file_ctrl.h"
#include "property.h"

class LogFile : public FileCtrl
{
public:
	bool const lifeCycleOnly;
	property_ro<LogFile, int> lineCount;
	properties_ro<LogFile, std::string> line;
	class LogFile &operator<<(std::string in);
	LogFile(std::string fn, bool deleteAfterClose=false);
	virtual ~LogFile(void);
protected:	
private:
	int _f_get_file_line_count(void);
	std::string _f_get_file_line(int i);
};

#endif //_FILE_CTRL_H_
