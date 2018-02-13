#ifndef __SPEC_FIELD_H__
#define __SPEC_FIELD_H__
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QFrame>
#include "property.h"


class SpecField : QObject
{
	Q_OBJECT
	
public:
	typedef class{
	public:
			QString descriptor;
			unsigned int value;
	}FieldEnum;

	typedef class{
	public:
		QString desc;
		unsigned char hiBit;
		unsigned char lowBit;
		bool	bHexMode;
		unsigned int defaultValue;
		QList<FieldEnum> enumList;
	}FieldConfiguration;
	
	bool ok;
	void clear(void);
	void configure(FieldConfiguration &config);
	property_wo<SpecField, bool> visible;
	property_ro<SpecField, unsigned int> field;
	SpecField(QFrame *container);
	~SpecField(void);
public slots:
	void on_index_change(void);
	
protected:
	QFrame *_owner;
private:
	static unsigned char decDigitalTable[32];
	unsigned int shift;
	unsigned int mask;
	int base;
	void _f_set_visible(bool en);
	unsigned int _f_get_field(void);
	QLabel * label;
	QLineEdit * edit;
	QComboBox * enumBox;
};

#endif //ndef __SPEC_FIELD_H__

