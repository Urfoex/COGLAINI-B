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


#include "cfragmentshader.h"
#include <stdexcept>

std::map< std::string, std::shared_ptr<CFragmentShader>> CFragmentShader::m_listOfCFS;

std::shared_ptr< CFragmentShader > CFragmentShader::CreateFragmentShader( std::string filename ) {
	std::shared_ptr< CFragmentShader > pcfs = nullptr;

	try {
		pcfs = m_listOfCFS.at( filename );
	} catch( std::out_of_range e ) {
		pcfs = std::shared_ptr< CFragmentShader >( new CFragmentShader( filename ) );
		m_listOfCFS.insert( std::pair< std::string, std::shared_ptr< CFragmentShader >>( filename, pcfs ) );
	}

	return pcfs;
}

std::shared_ptr< CFragmentShader > CFragmentShader::CreateFragmentShader( std::string shadername, std::string shadercode ) {
	std::shared_ptr< CFragmentShader > pcfs = nullptr;

	try {
		pcfs = m_listOfCFS.at( shadername );
	} catch( std::out_of_range e ) {
		pcfs = std::shared_ptr< CFragmentShader >( new CFragmentShader( shadername, shadercode ) );
		m_listOfCFS.insert( std::pair< std::string, std::shared_ptr< CFragmentShader >>( shadername, pcfs ) );
	}

	return pcfs;
}

void CFragmentShader::free() {
	m_listOfCFS.clear();
}

CFragmentShader::CFragmentShader( std::string filename ):
	CShaderLoader( filename, GL_FRAGMENT_SHADER ) {
}

CFragmentShader::CFragmentShader( std::string shadername, std::string shadercode ):
	CShaderLoader( shadername, shadercode,  GL_FRAGMENT_SHADER ) {

}


CFragmentShader::~CFragmentShader() {
}

GLint CFragmentShader::getType() {
	return GL_FRAGMENT_SHADER;
}
