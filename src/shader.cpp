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

#include <list>
#include <string>
#include <sstream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <GL/glew.h>
#include "shader.hpp"

void ShaderSource::addShaderSource( const std::string& source )
{
	_sources.push_back( source );
}

void ShaderSource::loadShaderSource( const char * filename )
{
	std::ifstream fin;
	fin.exceptions( std::ios::badbit | std::ios::failbit );
	std::ostringstream sout;
	fin.open( filename );
	sout << fin.rdbuf();
	fin.close();
	addShaderSource( sout.str() );
}

void Shader::setSource(const ShaderSource& source)
{
	std::list< std::string > sources = source.getSources();
	if ( sources.size() == 0 )
	{
		throw std::runtime_error("Attempted to create shader with empty sources");
	}
	if ( sources.size() == 1 )
	{
		const char * cstr = sources.front().c_str();
		glShaderSource(_shaderId, 1, &cstr, 0);
	}
	else
	{
		std::vector< const char* > strArray(sources.size(), 0);
		size_t i = 0;
		std::list< std::string >::iterator sourcestr = sources.begin();
		for ( ; sourcestr != sources.end(); ++sourcestr, ++i )
		{
			strArray[i] = sourcestr->c_str();
		}
		glShaderSource(_shaderId, i, &strArray[0], 0);
	}
}

void Shader::compile()
{
	glCompileShader(_shaderId);

	GLint status;
	glGetShaderiv(_shaderId, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(_shaderId, GL_INFO_LOG_LENGTH, &length);
		std::vector< char > errorstr(length);
		glGetShaderInfoLog(_shaderId, length, NULL, &errorstr[0]);
		throw std::runtime_error("Compile errors(s): " + std::string(&errorstr[0]));
	}
}

void ShaderProgram::attachShader( Shader& shader )
{
	glAttachShader( _programId, shader.getGlId() );
	shaders.push_back( &shader );
}

void ShaderProgram::link()
{
	std::for_each( shaders.begin(), shaders.end(), std::mem_fun( &Shader::compile ) );
	glLinkProgram( _programId );

	GLint status;
	glGetProgramiv(_programId, GL_LINK_STATUS, &status);
	if(status == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &length);
		std::vector< char > errorstr(length);
		glGetProgramInfoLog(_programId, length, NULL, &errorstr[0]);
		throw std::runtime_error("Link error(s): " + std::string(&errorstr[0]));
	}

	_linked = true;
}

void ShaderProgram::use()
{
	if (!_linked)
		link();
	glUseProgram( _programId );
}
