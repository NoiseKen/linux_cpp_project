#ifndef __HTML_STRING_H__
#define __HTML_STRING_H__
#include <QString>
#include "property.h"


class HtmlString : public QString
{
public:
#define COLOR_RGB(r,g,b) ((r<<16)|(g<<8)|(b))	
	enum{
		colorRed=COLOR_RGB(255,0,0),
		colorGreen=COLOR_RGB(0,255,0),
		colorBlue=COLOR_RGB(0,0,255),
		colorPurple=COLOR_RGB(255,0,255),
		colorCyan=COLOR_RGB(0,64,200),
		colorBlack=COLOR_RGB(0,0,0),
		colorWhite=COLOR_RGB(255,255,255),
		colorDefault=colorBlack,
	};

	class ColorStr{
	public :
		unsigned int color;
		//unsigned int size;
		//QString face;
		QString string;
		ColorStr(unsigned int color=colorDefault, QString str="");
		~ColorStr(void);
	};

	static const QString breakTage;
	class HtmlString & operator<<(QString str);
	class HtmlString & operator<<(ColorStr cstr);	
	class HtmlString & operator=(HtmlString hstr);
	property_ro<HtmlString, QString> xml;
	property_ro<HtmlString, bool> isEmptyTitle;
		
	HtmlString(void);
	HtmlString(QString str);
	//HtmlString(Htm	//void report_style(lStyle style);
	~HtmlString(void);
protected:
private:
	bool _f_get_titme_isempty(void);
	QString _f_get_xml(void);
	static const QString head;
	QString title;
	static const QString separate;	
	QString & morph_to_br(QString &str);
};


#endif //ndef __HTML_STRING_H__
