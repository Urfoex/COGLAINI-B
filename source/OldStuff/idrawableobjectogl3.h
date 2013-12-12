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


#ifndef IDRAWABLEOBJECTOGL3_H
#define IDRAWABLEOBJECTOGL3_H

#include "idrawableobject.h"
#include "cshaderprogram.h"

#include <memory>

class CAxis;

class IDrawableObjectOGL3 : public IDrawableObject {

public:
	IDrawableObjectOGL3( CGame* game );
	virtual ~IDrawableObjectOGL3();
	virtual void draw();
	virtual void init();

protected:
	GLuint m_vertexArrayObjectID;
	GLuint m_vertexBufferObjectID;
	GLuint m_vertexBufferIndexID;

	GLuint m_numberOfElements;

	std::shared_ptr<CVertexShader> m_vs;
	std::shared_ptr<CFragmentShader> m_fs;
	std::shared_ptr<CShaderProgram> m_sp;

	std::unique_ptr<CAxis> m_axis;

	virtual void createShader() = 0;
	virtual void createObject() = 0;
};

#endif // IDRAWABLEOBJECTOGL3_H
