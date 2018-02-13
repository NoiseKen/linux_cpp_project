#include "spec_field.h"
//0 base table
//which mean decDigitalTable[0] = digital number for 2^1 in decimal
//           decDigitalTable[1] = digital number for 2^2 in decimal
//           decDigitalTable[31] = digital number for 2^32 in decimal
unsigned char SpecField::decDigitalTable[32]={
//	1,  2,  3,  4,  5,  6,  7,  8
	1,  1,  1,  2,  2,  2,  3,  3,
//	9, 10, 11, 12, 13, 14, 15, 16
	3,  4,  4,  4,  4,  5,  5,  5,
// 17, 18, 19, 20, 21, 22, 23, 24
	6,  6,  6,  7,  7,  7,  7,  8,  
// 25, 26, 27, 28, 29, 30, 31, 32
	8,  8,  9,  9,  9, 10, 10, 10,
};

void
SpecField::clear(void)
{	
	label->setText("(NULL)=");
	edit->setText("");
	edit->setInputMask("");
	enumBox->clear();
	enumBox->setMaxCount(0);
	enumBox->setEnabled(false);
	visible=false;
}

void
SpecField::_f_set_visible(bool en)
{
	_owner->setVisible(en);
}

unsigned int
SpecField::_f_get_field(void)
{
	unsigned int field;
	bool good;
	field = edit->text().toUInt(&good, base);
	field &= mask;
	field <<= shift; 
	return field;
}

void 
SpecField::on_index_change(void)
{
	int currentIdx;
	unsigned int value;
		
	currentIdx = enumBox->currentIndex();
	value = enumBox->itemData(currentIdx, Qt::UserRole).toUInt();
	edit->setText(QString::number(value,base).toUpper());
}

void 
SpecField::configure(FieldConfiguration &config)
{
	QString labelStr;
	QString maskStr;
	unsigned int numberBit;
	unsigned int numberDigital;

	QChar digital;
	shift = config.lowBit;
	numberBit = config.hiBit-config.lowBit+1;
	mask = (1<<numberBit)-1;
	//label for field descriptor
	labelStr = QString("[%1:%2] %3 =").arg(config.hiBit).arg(config.lowBit).arg(config.desc);
	if(config.bHexMode)
	{
		labelStr += " 0x";
		maskStr = ">";
		numberDigital = (numberBit+3)/4;
		digital = 'H';
		base = 16;
	}
	else
	{
		numberDigital = decDigitalTable[numberBit-1];
		digital = '9';
		base = 10;
	}
		
	label->setText(labelStr);
	for(unsigned int i=0;i<numberDigital;i++)
	{
		maskStr += digital;
	}

	edit->setInputMask(maskStr);
	edit->setText(QString::number(config.defaultValue, base));

	int cnt=config.enumList.count();
	if(cnt)
	{
		FieldEnum item;
		enumBox->setMaxCount(cnt);
		enumBox->setMaxVisibleItems(cnt);
		for(int i=0; i<cnt; i++)
		{
			item = config.enumList[i];
			enumBox->addItem(item.descriptor, item.value );
		}
		enumBox->setEnabled(true);
	}

	visible = true;	
}

SpecField::SpecField(QFrame *container) : visible(this, &SpecField::_f_set_visible),
				field(this, &SpecField::_f_get_field)
{
	_owner = container;
	QList<QLabel *> allLabel;
	QList<QLineEdit *> allEdit;
	QList<QComboBox *> allEnum;
	ok=false;

	allLabel = container->findChildren<QLabel *>();
	allEdit = container->findChildren<QLineEdit *>();
	allEnum = container->findChildren<QComboBox *>();

	if((allLabel.size()==1)&&(allEdit.size()==1)&&(allEnum.size()==1))
	{
		label = allLabel[0];
		edit = allEdit[0];
		enumBox = allEnum[0];
		
		QObject::connect(enumBox, SIGNAL(currentIndexChanged(int)), this,  SLOT(on_index_change()));
		
		ok = true;
		clear();		
	}

}

SpecField::~SpecField(void)
{
}
