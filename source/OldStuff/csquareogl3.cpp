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


#include "csquareogl3.h"
#include "cgame.h"
#include "caxis.h"

CSquareOGL3::CSquareOGL3( CGame* game ): IDrawableObjectOGL3( game ) { /*, m_myAxis(game, *this)*/

}

// CGraphicObject::CGraphicObject( const CGraphicObject& other ) {
//
// }

CSquareOGL3::~CSquareOGL3() {
}

// CGraphicObject& CGraphicObject::operator=( const CGraphicObject& other ) {
// 	return *this;
// }
//
// bool CGraphicObject::operator==( const CGraphicObject& other ) const {
// ///TODO: return ...;
// }

void CSquareOGL3::createObject() {
	//////////////////////
	// Set up coordinates
	float size = 1.0f;
	glm::vec3 squareCoords[4];
	squareCoords[0] = glm::vec3( -size,  size,  size );
	squareCoords[1] = glm::vec3( size,  size,  size );
	squareCoords[2] = glm::vec3( -size, -size,  size );
	squareCoords[3] = glm::vec3( size, -size,  size );

	///////////////////////////////
	// Set up index to coordinates
	GLubyte squareIndex[4] = {0, 1, 2, 3}; // STRIP


	///////////////////////////////////
	// Create and bind the vertex array
	glGenVertexArrays( 1, &m_vertexArrayObjectID );
	glBindVertexArray( m_vertexArrayObjectID );

	/////////////////////////////////////
	// Create and bind the vertex object
	glGenBuffers( 1, &m_vertexBufferObjectID );
	glBindBuffer( GL_ARRAY_BUFFER, m_vertexBufferObjectID );
	glBufferData( GL_ARRAY_BUFFER, 4 * sizeof( glm::vec3 ), squareCoords, GL_STATIC_DRAW );

	////////////////////////////
	// Set up vertex attributes
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( glm::vec3 ), nullptr );

	///////////////////////////////////////////
	// Setting number of elements inside index
	m_numberOfElements = 4 * sizeof( GLubyte );

	////////////////////////////////////
	// Create and bind the vertex index
	glGenBuffers( 1, &m_vertexBufferIndexID );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_vertexBufferIndexID );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_numberOfElements, squareIndex, GL_STATIC_DRAW );

	//////////////
	// Unbind VAO
	glBindVertexArray( 0 );
}

void CSquareOGL3::createShader() {
	m_vs = CVertexShader::CreateVertexShader( "../resources/Shader/square.vert" );
	m_fs = CFragmentShader::CreateFragmentShader( "../resources/Shader/square.frag" );
	m_sp = CShaderProgram::CreateShaderProgram( m_vs, m_fs );

	///////////////////////////////////////
	// Bind variables used in shader files
	m_sp->setAttributes( 1, "in_Position" );
	m_sp->createProgram();
}

void CSquareOGL3::transform() {
	// Apply some transformations
// 	glMatrixMode(GL_MODELVIEW);

	float r = float( rand() ) / float( RAND_MAX );

	m_modelMatrix = glm::rotate( m_modelMatrix, 0.1f, glm::vec3( 1.0f, 0.0f, 0.0f ) );
	m_modelMatrix = glm::rotate( m_modelMatrix, r * 2.5f, glm::vec3( 0.0f, 1.0f, 0.0f ) );

// 	glLoadIdentity();
	// 	glTranslatef(m_x, m_y, -100.f);
// 	glRotatef(m_game->getElapsedTime() * 0.05f, 1.f, 0.f, 0.f);
// 	glRotatef(m_game->getElapsedTime() * 0.03f, 0.f, 1.f, 0.f);
// 	glRotatef(m_game->getElapsedTime() * 0.09f, 0.f, 0.f, 1.f);
}

void CSquareOGL3::draw() {
	glUseProgram( m_sp->getShaderProgram() );
	glBindVertexArray( m_vertexArrayObjectID );

	GLint m_projectionMatrixLocation = glGetUniformLocation( m_sp->getShaderProgram(), "projectionMatrix" );
	GLint m_viewMatrixLocation = glGetUniformLocation( m_sp->getShaderProgram(), "viewMatrix" );
	m_modelMatrixLocation = glGetUniformLocation( m_sp->getShaderProgram(), "modelMatrix" );

	GLint time = glGetUniformLocation( m_sp->getShaderProgram(), "time" );
	GLint resolution = glGetUniformLocation( m_sp->getShaderProgram(), "resolution" );

	glUniformMatrix4fv( m_projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr( m_game->getProjetionMatrix() ) );
	glUniformMatrix4fv( m_viewMatrixLocation, 1, GL_FALSE, glm::value_ptr( m_game->getCamera().getViewMatrix() ) );
	glUniformMatrix4fv( m_modelMatrixLocation, 1, GL_FALSE, glm::value_ptr( m_modelMatrix ) );

	glUniform1f( time, std::time( NULL ) / 1000.0f );
	glUniform2f( resolution, 640.0f, 480.0f );

	glDrawElements( GL_TRIANGLE_STRIP, m_numberOfElements, GL_UNSIGNED_BYTE, nullptr );

	m_axis->draw();
}
