#include "string_list.h"
#include "lib_extract.h"
#include "spec_dword.h"
#include "utils.h"
#include "debug.h"


void
SpecDWord::scheme(QString code)
{
	QList<std::string> *extlist;
	QString str;
	extlist = new QList<std::string>();
	clear();
	
	str = code + "." + _tag;

	if(LibExtract().get_lib_list(str, *extlist) >= 0)
	{
		int cnt;
		int i;
		SpecField::FieldConfiguration *cfg;
		ExtensionString ext(".");
		cfg = new SpecField::FieldConfiguration();

		cnt = extlist->size();

		for(i=0;i<cnt;i++)
		{			
			ext.string = (*extlist)[i];
			str = QString::fromStdString(ext[0]);				
			cfg->desc = str;
			str = QString::fromStdString(ext[1]);
			cfg->hiBit = str.toUInt();
			str = QString::fromStdString(ext[2]);
			cfg->lowBit = str.toUInt();
			str = QString::fromStdString(ext[3]);
			cfg->defaultValue = str.toUInt();
			str = QString::fromStdString(ext[4]);
			cfg->bHexMode = str.toUInt();
			str = QString::fromStdString(ext[5]);
			if(str == "@")
			{
				cfg->enumList.clear();
			}
			else
			{
				QList<std::string> *emunlist;
				SpecField::FieldEnum enums;
				emunlist = new QList<std::string>();
				str = "enumTag." + str;
				if(LibExtract().get_lib_list(str, *emunlist) >= 0)
				{
					int enumCnt;
					int j;
					enumCnt = emunlist->size();
					for(j=0;j<enumCnt;j++)
					{
						ext.string = (*emunlist)[j];
						str = QString::fromStdString(ext[0]);				
						enums.descriptor = str;
						str = QString::fromStdString(ext[1]);
						enums.value = str.toUInt();
									
						cfg->enumList << enums;
					}					
				}
				
				delete emunlist;
			}
			
			fields[i]->configure(*cfg);
		}
						
		delete cfg;
	}

	delete extlist;
}

void
SpecDWord::clear(void)
{
	int cnt;
	cnt = fields.count();
	for(int i=0;i<cnt;i++)
	{
		fields[i]->clear();;
	}	
}

void 
SpecDWord::configure(QList<SpecField::FieldConfiguration *> &config)
{
	int cnt;
	cnt = config.count();
	for(int i=0;i<cnt;i++)
	{
		fields[i]->configure(*config[i]);
	}
}

unsigned int
SpecDWord::_f_get_dword(void)
{
	unsigned int d32;
	int cnt;	
	cnt = fields.count();
	d32 = 0;
	for(int i=0;i<cnt;i++)
	{
		d32 |= fields[i]->field;
	}	
	return d32;
}

void
SpecDWord::update(void)
{	
	QString title;
	unsigned int d32;
	d32 = dword;
	title = _tag + QString().sprintf(" = 0x%08X", d32);
	_owner->setTitle(title);
}


SpecDWord::SpecDWord(QGroupBox *container, QString tag) : dword(this, &SpecDWord::_f_get_dword)
{	
	QList<QFrame *> frames;
	SpecField *field;
	int cnt;	
	_owner = container;
	_tag = tag;
	frames = container->findChildren<QFrame *>();
	cnt = frames.count();	
	ok = false;
	for(int i =0;i<cnt;i++)
	{
		field = new SpecField(frames[i]);
		if(field->ok)
		{
			fields << field;
		}
	}	
}

SpecDWord::~SpecDWord(void)
{
	int cnt;
	cnt = fields.count();
	for(int i=0;i<cnt;i++)
	{
		delete fields[i];
	}
	
	fields.clear();	
}
