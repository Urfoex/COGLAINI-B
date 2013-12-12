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


#include "cvertexshader.h"
#include <stdexcept>

std::map< std::string, std::shared_ptr<CVertexShader>> CVertexShader::m_listOfCVS;

std::shared_ptr< CVertexShader > CVertexShader::CreateVertexShader( std::string filename ) {
	std::shared_ptr< CVertexShader > pcvs = nullptr;

	try {
		pcvs = m_listOfCVS.at( filename );
	} catch( std::out_of_range e ) {
		pcvs = std::shared_ptr< CVertexShader >( new CVertexShader( filename ) );
		m_listOfCVS.insert( std::pair< std::string, std::shared_ptr< CVertexShader >>( filename, pcvs ) );
	}

	return pcvs;
}

std::shared_ptr< CVertexShader > CVertexShader::CreateVertexShader( std::string shadername, std::string shadercode ) {
	std::shared_ptr< CVertexShader > pcvs = nullptr;

	try {
		pcvs = m_listOfCVS.at( shadername );
	} catch( std::out_of_range e ) {
		pcvs = std::shared_ptr< CVertexShader >( new CVertexShader( shadername, shadercode ) );
		m_listOfCVS.insert( std::pair< std::string, std::shared_ptr< CVertexShader >>( shadername, pcvs ) );
	}

	return pcvs;
}


CVertexShader::CVertexShader( std::string filename ):
	CShaderLoader( filename, GL_VERTEX_SHADER ) {
}

CVertexShader::CVertexShader( std::string shadername, std::string shadercode ):
	CShaderLoader( shadername, shadercode, GL_VERTEX_SHADER ) {

}

CVertexShader::~CVertexShader() {
}

GLint CVertexShader::getType() {
	return GL_VERTEX_SHADER;
}

void CVertexShader::free() {
	m_listOfCVS.clear();
}
