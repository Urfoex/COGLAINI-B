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


#ifndef IDRAWABLEOBJECT_H
#define IDRAWABLEOBJECT_H

#include "headers_with_gcc_warnings/glm.hpp"
#include "../libraries/glm/gtc/type_ptr.hpp"
#include "SFML_OpenGL.h"

class CGame;

class IDrawableObject {
public:
	IDrawableObject( CGame* game );
	IDrawableObject( const IDrawableObject& object );
	virtual IDrawableObject& operator=( const IDrawableObject& object );
	virtual ~IDrawableObject();
	virtual void init() = 0;
	virtual void draw() = 0;
	virtual void transform() = 0;
	virtual const glm::mat4& getModelMatrix() const;
// 	virtual void setPosition( glm::vec3& position);
// 	virtual void setPosition( glm::vec3&& position);
	virtual void translate( glm::vec3& translation );
	virtual void translate( glm::vec3 && translation );
// 	virtual void setRotation( float angle, glm::vec3& axis);
// 	virtual void setRotation( float angle, glm::vec3&& axis);
	virtual void rotate( float angle, glm::vec3& axis );
	virtual void rotate( float angle, glm::vec3 && axis );
// 	virtual void setScale( glm::vec3& scale);
// 	virtual void setScale( glm::vec3&& scale);
	virtual void scale( glm::vec3& scaling );
	virtual void scale( glm::vec3 && scaling );
protected:
	CGame* m_game;
	glm::mat4 m_modelMatrix;
	GLint m_modelMatrixLocation;

// 	glm::vec3 m_translation;
// 	glm::vec3 m_rotation;
// 	glm::vec3 m_scale;
};

#endif // IDRAWABLEOBJECT_H
