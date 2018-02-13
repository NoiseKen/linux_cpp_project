#ifndef __LIB_EXTRACT_H__
#define __LIB_EXTRACT_H__
#include <QString>
#include <QList>
//#include "extension_string.h"

class LibExtract
{
public:
	typedef enum{
		KEY_VERIFY_DRIVER=0,
		KEY_CMPL_DRIVER,
		KEY_TEST_IOMETER,
		KEY_TEST_TNVME,
		KEY_CMPLMGN_PATH,
		KEY_NODE_DEV,
		KEY_NODE_BLK,
		KEY_PCI_REMOVE_SLOT,
		KEY_MAX
	}ValidConfigKey;
	
	QString get_config_value(ValidConfigKey keyIdx);
	int get_lib_list(QString tag, QList<std::string> &list);
	LibExtract(void);
	~LibExtract(void);
protected:
private:
	static const std::string keyTable[KEY_MAX];
	static const std::string libFile;
	static const std::string configFile;
};

#endif //ndef __LIB_EXTRACT_H__
