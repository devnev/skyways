#ifndef _TEXTPRINTER_HPP_
#define _TEXTPRINTER_HPP_

#include <string>

class FTGLPixmapFont;

class TextPrinter
{

public:

	TextPrinter( const std::string& fontfilename );
	~TextPrinter();

	enum Alignment {
		ALIGN_LEFT,
		ALIGN_CENTER,
	};

	void print( const std::string& text,
		double x, double y,
		Alignment align = ALIGN_LEFT
	);

private:

	FTGLPixmapFont * font;

};

#endif // _TEXTPRINTER_HPP_
