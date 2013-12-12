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


#ifndef CFRAGMENTSHADER_H
#define CFRAGMENTSHADER_H

#include "cshaderloader.h"
#include <map>

class CFragmentShader : public CShaderLoader {
public:
	static std::shared_ptr<CFragmentShader> CreateFragmentShader( std::string filename );
	static std::shared_ptr<CFragmentShader> CreateFragmentShader( std::string shadername, std::string shadercode );
	static void free();
	virtual ~CFragmentShader();
	virtual GLint getType();

private:
	static std::map< std::string, std::shared_ptr<CFragmentShader>> m_listOfCFS;

	CFragmentShader( std::string filename );
	CFragmentShader( std::string shadername, std::string shadercode );
};

#endif // CFRAGMENTSHADER_H
