//
// This file is part of the game Skyways.
// Copyright (C) 2009 Mark Nevill
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

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
