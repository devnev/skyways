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

#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include <list>
#include <string>
#include <vector>
#include <memory>
#include "uniform.hpp"

class ShaderSource
{

public:

	ShaderSource() { }
	~ShaderSource() { }

	void addShaderSource( const std::string& source );

	void loadShaderSource( const char * filename );

	std::list< std::string > getSources() const
	{
		return _sources;
	}

private:

	std::list< std::string > _sources;

};

class Shader
{

public:

	enum ShaderType {
		VertexShader = GL_VERTEX_SHADER,
		FragmentShader = GL_FRAGMENT_SHADER,
	};

	Shader(ShaderType shaderType)
		: _shaderType( shaderType )
		, _shaderId( glCreateShader(shaderType) )
	{
	}

	void setSource(const ShaderSource& source);

	void compile();

	GLuint getGlId()
	{
		return _shaderId;
	}

private:

	ShaderType _shaderType;
	GLuint _shaderId;

};

class ShaderProgram
{

public:

	ShaderProgram()
		: _programId( glCreateProgram() )
		, _linked( false )
	{
	}

	void attachShader( Shader& shader );

	Uniform getUniform( const std::string& name );

	void link();

	void use();

	GLuint getGlId()
	{
		return _programId;
	}

private:

	std::vector< Shader* > shaders;
	GLuint _programId;
	bool _linked;

};

std::auto_ptr< ShaderProgram >
createShaderProgram(
	const char * vertexFilename,
	const char * fragmentFilename
);

#endif // _SHADER_HPP_
