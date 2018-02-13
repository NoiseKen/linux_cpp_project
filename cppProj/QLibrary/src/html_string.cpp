#include "html_string.h"
#include "debug.h"
const QString HtmlString::breakTage="<BR>";
const QString HtmlString::head="<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">"
"p, li { white-space: pre-wrap; }"
"</style></head><body style=\" font-family:'Ubuntu Mono'; font-size:12pt; font-weight:bold; font-style:normal;\">"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>";

const QString HtmlString::separate="<hr size=20 align=center noshade width=90% color=0000ff>";

/*
HtmlString::HtmlStyle::HtmlStyle(void)
{
	foreColor = COLOR_RGB(0, 0, 0);
	backColor = COLOR_RGB(255, 255, 255);
	title = "TEST";	
}


HtmlString::HtmlStyle::~HtmlStyle(void)
{
}
*/

HtmlString::ColorStr::ColorStr(unsigned int c, QString str)
{
	color = c;
	//size = 3;
	//face = "Ubuntu Mono";
	string = str;
}

HtmlString::ColorStr::~ColorStr(void)
{
}

QString
HtmlString::_f_get_xml(void)
{
	QString xml;
	if(title.isEmpty())
	{
		xml = *this;
	}
	else
	{
		xml = head + "<center>" + title + "</center>" + separate;
		xml += *this;
		xml += "</body></html>";		
	}
	return xml;
}

class HtmlString & 
HtmlString::operator=(HtmlString hstr)
{	
	clear();
	*this += hstr;
	return *this;
}

class HtmlString & 
HtmlString::operator<<(QString str)
{	
	morph_to_br(str);
	*this += str;
	//*this += breakTage;
	return *this;
}

class HtmlString & 
HtmlString::operator<<(ColorStr cstr)
{
	QString tag;
	//tag.sprintf("<font size=%d color = #%06X face=%s>",cstr.size, cstr.color
	//				,cstr.face.toStdString().c_str());	
	tag.sprintf("<font color = #%06X>",cstr.color);
	tag += morph_to_br(cstr.string);
	tag += "</font>";
	*this += tag;
	return *this;
}

bool 
HtmlString::_f_get_titme_isempty(void)
{
	return title.isEmpty();
}

QString &
HtmlString::morph_to_br(QString &str)
{
	QString br;
	br = breakTage;// + "\n";
	str.replace("\n", br);
	return str;
}

HtmlString::HtmlString(QString str) : QString()
		,xml(this, &HtmlString::_f_get_xml)
		,isEmptyTitle(this, &HtmlString::_f_get_titme_isempty)
{
	title = "<H1>" + str + "</H>";
	clear();
}

HtmlString::HtmlString(void) : QString()
		,xml(this, &HtmlString::_f_get_xml)	
		,isEmptyTitle(this, &HtmlString::_f_get_titme_isempty)
{
	title.clear();
	clear();
}		

HtmlString::~HtmlString(void)
{
}
