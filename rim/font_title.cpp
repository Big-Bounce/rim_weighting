#include <font_title.h>

font_title::font_title(): QFont()
{
    setPointSize(11);
    setBold(true);
    //setItalic(true);
    setCapitalization(QFont::AllUppercase);
}

font_title::~font_title()
{
}
