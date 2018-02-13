#ifndef __HISTORY_LIST_H__
#define __HISTORY_LIST_H__
#include "string_list.h"
#include "property.h"

class HistoryList : public StringList
{
private:		
protected:
	unsigned int _depth;
public:
	static const unsigned int nolimit;
	//operator
	class HistoryList& operator=(class StringList);
	class HistoryList& operator<<(const std::string);
	//method
	virtual int load_from(std::string);
	void push_back(const std::string appendStr);	
	//useful properties
	//constructor
	HistoryList(unsigned int limit);
	//destructor	
	~HistoryList(void);
};

#endif //ndef __HISTORY_LIST_H__
