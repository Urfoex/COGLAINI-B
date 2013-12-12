/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Manuel Bellersen <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef CSHADERLOADER_H
#define CSHADERLOADER_H

#include "SFML_OpenGL.h"

#include <string>
#include "clogerrorstream.h"

class CShaderLoader {
public:
	CShaderLoader( std::string filename, GLenum && type );
	CShaderLoader( std::string shadername, std::string shadercode, GLenum && type );
	virtual ~CShaderLoader();
	virtual GLuint& getShader();
	virtual GLint getType();
	std::string getFilename();
protected:
	CLogErrorStream m_logError;
	GLuint m_shader;
	std::string m_filename;

	std::string loadFile( std::string& filename );
	void compileShader( std::string && filename, GLenum && type );
	void compileShader( std::string && shadername, std::string shadercode, GLenum && type );
	void logShaderError( );
	void maybeThrowShaderError( GLenum status, std::string && message );
};

#endif // CSHADERLOADER_H
