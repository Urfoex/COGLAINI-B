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


#include "idrawableobjectogl3.h"

#include "cgame.h"
#include "caxis.h"

IDrawableObjectOGL3::IDrawableObjectOGL3( CGame* game ):
	IDrawableObject( game ),
	m_vertexArrayObjectID( 0 ),
	m_vertexBufferObjectID( 0 ),
	m_vertexBufferIndexID( 0 ),
	m_numberOfElements( 0 ),
	m_vs( nullptr ),
	m_fs( nullptr ),
	m_sp( nullptr ),
	m_axis( new CAxis( game, *this ) ) {

}

#include <iostream>
IDrawableObjectOGL3::~IDrawableObjectOGL3() {
	glDeleteBuffers( 1, &m_vertexBufferObjectID );
	glDeleteBuffers( 1, &m_vertexBufferIndexID );
	glDeleteVertexArrays( 1, &m_vertexArrayObjectID );
}


void IDrawableObjectOGL3::draw() {
	glUseProgram( m_sp->getShaderProgram() );
	glBindVertexArray( m_vertexArrayObjectID );

	GLint m_projectionMatrixLocation = glGetUniformLocation( m_sp->getShaderProgram(), "projectionMatrix" );
	GLint m_viewMatrixLocation = glGetUniformLocation( m_sp->getShaderProgram(), "viewMatrix" );
	m_modelMatrixLocation = glGetUniformLocation( m_sp->getShaderProgram(), "modelMatrix" );

	glUniformMatrix4fv( m_projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr( m_game->getProjetionMatrix() ) );
	glUniformMatrix4fv( m_viewMatrixLocation, 1, GL_FALSE, glm::value_ptr( m_game->getCamera().getViewMatrix() ) );
	glUniformMatrix4fv( m_modelMatrixLocation, 1, GL_FALSE, glm::value_ptr( m_modelMatrix ) );

	glDrawElements( GL_TRIANGLES, m_numberOfElements, GL_UNSIGNED_BYTE, nullptr );

	m_axis->draw();
}

void IDrawableObjectOGL3::init() {
	createObject();
	createShader();
	m_axis->init();
}



