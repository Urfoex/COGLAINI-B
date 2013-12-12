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


#include "idrawableobject.h"
#include <stdexcept>
#include "../libraries/glm/gtc/matrix_transform.hpp"

IDrawableObject::IDrawableObject( CGame* game ):
	m_game( game ),
	m_modelMatrix( 1.0f ),
	m_modelMatrixLocation( 0 ) {
	if( m_game == nullptr ) {
		throw( std::runtime_error( "This object needs the pointer to a CGame-object but got nullptr.\n" ) );
	}
}

IDrawableObject::IDrawableObject( const IDrawableObject& object ):
	m_game( object.m_game ),
	m_modelMatrix( object.m_modelMatrix ),
	m_modelMatrixLocation( object.m_modelMatrixLocation ) {
}

IDrawableObject::~IDrawableObject() {
}


IDrawableObject& IDrawableObject::operator=( const IDrawableObject& object ) {
	if( this != &object ) {
		this->m_game = object.m_game;
	}

	return *this;
}

const glm::core::type::mat4& IDrawableObject::getModelMatrix() const {
	return m_modelMatrix;
}

void IDrawableObject::translate( glm::core::type::vec3& translation ) {
	m_modelMatrix = glm::translate( m_modelMatrix, translation );
}

void IDrawableObject::translate( glm::core::type::vec3 && translation ) {
	m_modelMatrix = glm::translate( m_modelMatrix, translation );
}

void IDrawableObject::rotate( float angle, glm::core::type::vec3& axis ) {
	m_modelMatrix = glm::rotate( m_modelMatrix, angle, axis );
}

void IDrawableObject::rotate( float angle, glm::core::type::vec3 && axis ) {
	m_modelMatrix = glm::rotate( m_modelMatrix, angle, axis );
}

void IDrawableObject::scale( glm::core::type::vec3& scaling ) {
	m_modelMatrix = glm::scale( m_modelMatrix, scaling );
}

void IDrawableObject::scale( glm::core::type::vec3 && scaling ) {
	m_modelMatrix = glm::scale( m_modelMatrix, scaling );
}

// void IDrawableObject::setPosition(glm::core::type::vec3& position)
// {
// 	m_modelMatrix = glm::translate( glm::mat4(1), position);
// 	m_modelMatrix = glm::rotate( m_modelMatrix, 1.0f, m_rotation );
// 	m_modelMatrix = glm::scale( m_modelMatrix, m_scale);
// }
//
// void IDrawableObject::setPosition(glm::core::type::vec3&& position)
// {
// 	m_modelMatrix = glm::translate( glm::mat4(1), position);
// 	m_modelMatrix = glm::rotate( m_modelMatrix, 1.0f, m_rotation );
// 	m_modelMatrix = glm::scale( m_modelMatrix, m_scale);
// }
//
// void IDrawableObject::setRotation(float angle, glm::core::type::vec3& axis)
// {
// }
//
// void IDrawableObject::setRotation(float angle, glm::core::type::vec3&& axis)
// {
//
// }
//
// void IDrawableObject::setScale(glm::core::type::vec3& scale)
// {
//
// }
//
// void IDrawableObject::setScale(glm::core::type::vec3&& scale)
// {
//
// }

