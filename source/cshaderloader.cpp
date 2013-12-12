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


#include "cshaderloader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <cassert>

CShaderLoader::CShaderLoader( std::string filename, GLenum && type ):
	m_logError(),
	m_shader( 0 ),
	m_filename( filename ) {
	m_logError.logStream() << "Creating shader: " << filename << std::endl;
	compileShader( std::move( filename ), std::move( type ) );
}

CShaderLoader::CShaderLoader( std::string shadername, std::string shadercode, GLenum && type ):
	m_logError(),
	m_shader( 0 ),
	m_filename( shadername ) {
	m_logError.logStream() << "Creating shader: " << shadername << std::endl;
	compileShader( std::move( shadername ), shadercode, std::move( type ) );
}


GLint CShaderLoader::getType() {
	GLint type;
	glGetShaderiv( m_shader, GL_SHADER_TYPE, &type );
	return type;
}

GLuint& CShaderLoader::getShader() {
	return m_shader;
}

CShaderLoader::~CShaderLoader() {
	m_logError.logStream() << "Deleting shader: " << m_shader << std::endl;
	glDeleteShader( m_shader );
}

std::string CShaderLoader::loadFile( std::string& filename ) {
	std::ifstream file( filename );

	if( !file.good() ) {
		throw( std::runtime_error( "Could not work with file: " + filename ) );
	}

	std::stringstream ss;
	ss << file.rdbuf();
	file.close();
	return ss.str();
}

void CShaderLoader::compileShader( std::string && filename, GLenum && type ) {
	std::string shaderFile = loadFile( filename );
	compileShader( std::move( filename ), shaderFile, std::move( type ) );
}


void CShaderLoader::compileShader( std::string && shadername, std::string shadercode, GLenum && type ) {
	if( shadercode.length() == 0 ) {
		throw( std::runtime_error( "Empty shader file: " + shadername + " found.\n" ) );
	}

	m_shader = glCreateShader( type );

	if( !m_shader ) {
		throw( std::runtime_error( "Could not create shader object: " + shadername + ".\n" ) );
	}

	const char* shaderString = shadercode.c_str();

	glShaderSource( m_shader, 1, ( const GLchar** )( &shaderString ), nullptr );

	maybeThrowShaderError( glGetError(), "Could not set source code in shader object: " + shadername + ".\n" );

	glCompileShader( m_shader );

	maybeThrowShaderError( glGetError(), "Could not compile shader object: " + shadername + ".\n" );


	GLint status;

	glGetShaderiv( m_shader, GL_COMPILE_STATUS, &status );

	maybeThrowShaderError( status == GL_FALSE, "Could not compile shader: " + shadername );
}

void CShaderLoader::logShaderError( ) {
	GLint length;
	glGetShaderiv( m_shader, GL_INFO_LOG_LENGTH, &length );

	if( length > 0 ) {
		std::unique_ptr<GLchar[]> string( new GLchar[length] );
		GLint readlength;
		glGetShaderInfoLog( m_shader, length, &readlength, string.get() );
		m_logError.logStream() << string.get();
	}
}

void CShaderLoader::maybeThrowShaderError( GLenum status, std::string && message ) {
	if( status != GL_NO_ERROR ) {
		logShaderError( );
		glDeleteShader( m_shader );
		throw( std::runtime_error( message ) );
	}
}

std::string CShaderLoader::getFilename() {
	return m_filename;
}
