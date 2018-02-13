#include "lib_extract.h"
#include "string_list.h"
#include "value_list.h"
#include "nvme_tool.h"
#include "debug.h"
const std::string LibExtract::libFile = "admin_ds.lib";
const std::string LibExtract::configFile = "config.dat";
const std::string LibExtract::keyTable[KEY_MAX]={
	TO_STR(KEY_NXGN_DRIVER),
	TO_STR(KEY_CMPL_DRIVER),
	TO_STR(KEY_TEST_IOMETER),
	TO_STR(KEY_TEST_TNVME),
	TO_STR(KEY_CMPLMGN_PATH),
	TO_STR(KEY_NODE_DEV),
	TO_STR(KEY_NODE_BLK),
	TO_STR(KEY_PCI_REMOVE_SLOT),
};

int 
LibExtract::get_lib_list(QString tag, QList<std::string> &list)
{
	StringList *lib;
	std::string line;	
	bool empty;
	int lineIdx;

	lib = new StringList();
	list.clear();
	lineIdx = lib->load_from(LibExtract::libFile);
	if(lineIdx==0)
	{
		lineIdx = lib->search(tag.toStdString());
		if(lineIdx>=0)
		{
			do{
				line = (*lib)[++lineIdx];
				empty = line.empty();
				if(!empty)
				{
					list << line;
				}
			}while(!empty);			
		}
	}
	
	return lineIdx-list.size();
}

QString 
LibExtract::get_config_value(ValidConfigKey keyIdx)
{
	return QString::fromStdString(ValueList::get_config_value(LibExtract::configFile, keyTable[keyIdx]));
}

LibExtract::LibExtract(void)
{
}

LibExtract::~LibExtract(void)
{
}
