#include <ftgl.h>
#include "textprinter.hpp"

TextPrinter::TextPrinter( const std::string& fontfilename )
	: font( 0 )
{
	font = new FTGLPixmapFont( fontfilename.c_str() );
	if ( font->Error() )
		delete font;
}

TextPrinter::~TextPrinter()
{
	delete font;
}

void TextPrinter::print( const std::string& text,
	double x, double y,
	TextPrinter::Alignment align
)
{
	font->FaceSize( 28 );
	if ( align == ALIGN_CENTER )
	{
		float w = font->Advance( text.c_str(), text.length() );
		x -= w / 2;
	}
	font->Render( text.c_str(), text.length(), FTPoint( x, y ) );
}
