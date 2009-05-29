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

#ifndef _UNIFORM_HPP_
#define _UNIFORM_HPP_

#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <GL/glew.h>

class Uniform
{

public:

	Uniform(GLint location)
	  : _location( location )
	{
	}

	Uniform(const Uniform& other)
	  : _location( other._location )
	{
	}

	~Uniform()
	{
	}

#define SETUNIFORM(n, t, glt) \
	void setUniform(BOOST_PP_ENUM_PARAMS(n, t v)) { \
		glUniform ## n ## glt (_location, BOOST_PP_ENUM_PARAMS(n, v)); \
	} \
	/**/

	SETUNIFORM(1, float, f)
	SETUNIFORM(2, float, f)
	SETUNIFORM(3, float, f)
	SETUNIFORM(4, float, f)
	SETUNIFORM(1, int, i)
	SETUNIFORM(2, int, i)
	SETUNIFORM(3, int, i)
	SETUNIFORM(4, int, i)

#undef SETUNIFORM

private:

	GLint _location;

};

#endif // _UNIFORM_HPP_
