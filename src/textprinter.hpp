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
