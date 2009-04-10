#ifndef _TEXTPRINTER_HPP_
#define _TEXTPRINTER_HPP_

#include <string>

class FTGLPixmapFont;

class TextPrinter
{

public:

	TextPrinter( const std::string& fontfilename );
	~TextPrinter();

	void print( const std::string& text, double x, double y );

private:

	FTGLPixmapFont * font;

};

#endif // _TEXTPRINTER_HPP_
