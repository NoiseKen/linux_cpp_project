#ifndef __SPEC_DWORD_H__
#define __SPEC_DWORD_H__
#include <QGroupBox>
#include <QList>
#include "spec_field.h"
#include "property.h"


class SpecDWord
{
public:
	bool ok;
	void configure(QList<SpecField::FieldConfiguration *> &config);
	void scheme(QString code);
	void clear(void);
	void update(void);
	property_ro<SpecDWord, unsigned int> dword;
	SpecDWord(QGroupBox *container, QString tag);
	~SpecDWord(void);
protected:
	QGroupBox *_owner;
private:
	unsigned int _f_get_dword(void);
	QString _tag;
	QList<SpecField *> fields;
};

#endif //ndef __SPEC_DWORD_H__
