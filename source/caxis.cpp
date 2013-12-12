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

#include "caxis.h"
#include "cscene.h"

CAxis::CAxis( CScene* scene, glm::mat4* modelMatrix ):
	m_vertexArrayObjectID( 0 ),
	m_vertexBufferObjectID( 0 ),
	m_vertexBufferIndexID( 0 ),
	m_numberOfElements( 0 ),
	m_vs( nullptr ),
	m_fs( nullptr ),
	m_sp( nullptr ),
	m_modelMatrix( modelMatrix ),
	m_parentScene( scene ) {
	throw( "Stuff" );
}

CAxis::~CAxis() {
	glDeleteBuffers( 1, &m_vertexBufferObjectID );
	glDeleteBuffers( 1, &m_vertexBufferIndexID );
	glDeleteVertexArrays( 1, &m_vertexArrayObjectID );
}

void CAxis::init() {
	createObject();
	createShader();
}

void CAxis::draw() {
	glUseProgram( m_sp->getShaderProgram() );
	glBindVertexArray( m_vertexArrayObjectID );

// 	GLint m_projectionMatrixLocation = glGetUniformLocation( m_sp->getShaderProgram(), "projectionMatrix");
// 	GLint m_viewMatrixLocation = glGetUniformLocation( m_sp->getShaderProgram(), "viewMatrix");
// 	m_modelMatrixLocation = glGetUniformLocation(m_sp->getShaderProgram(), "modelMatrix");

	glm::mat4 mvpM = m_parentScene->getCurrentCamera()->getProjectionViewMatrix() * ( *m_modelMatrix );
	GLint mvpMatrix = glGetUniformLocation( m_sp->getShaderProgram(), "mvpMatrix" );
	glUniformMatrix4fv( mvpMatrix, 1, GL_FALSE, glm::value_ptr( mvpM ) );

// 	glUniformMatrix4fv( m_projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr( m_game->getProjetionMatrix()));
// 	glUniformMatrix4fv( m_viewMatrixLocation, 1, GL_FALSE, glm::value_ptr( m_game->getCamera().getViewMatrix()));
// 	glUniformMatrix4fv( m_modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix()));

	glDrawElements( GL_LINES, m_numberOfElements, GL_UNSIGNED_BYTE, nullptr );
}

void CAxis::createObject() {

	//////////////////////
	// Set up coordinates
	float size = 2.0f;
	glm::vec3 axisCoords[4];
	axisCoords[0] = glm::vec3( size,  0.0f,  0.0f );
	axisCoords[1] = glm::vec3( 0.0f,  size,  0.0f );
	axisCoords[2] = glm::vec3( 0.0f,  0.0f,  size );
	axisCoords[3] = glm::vec3( 0.0f,  0.0f,  0.0f );

	///////////////////////////////
	// Set up index to coordinates
	glm::detail::tvec2<GLubyte> cubeIndex[3]; // 2 triangle per quad, 6 quads on cube -> 2*6
	cubeIndex[0] = glm::detail::tvec2<GLubyte>( 3, 0 );
	cubeIndex[1] = glm::detail::tvec2<GLubyte>( 3, 1 );
	cubeIndex[2] = glm::detail::tvec2<GLubyte>( 3, 2 );


	///////////////////////////////////
	// Create and bind the vertex array
	glGenVertexArrays( 1, &m_vertexArrayObjectID );
	glBindVertexArray( m_vertexArrayObjectID );

	/////////////////////////////////////
	// Create and bind the vertex object
	glGenBuffers( 1, &m_vertexBufferObjectID );
	glBindBuffer( GL_ARRAY_BUFFER, m_vertexBufferObjectID );
	glBufferData( GL_ARRAY_BUFFER, 4 * sizeof( glm::vec3 ), axisCoords, GL_STATIC_DRAW );

	////////////////////////////
	// Set up vertex attributes
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( glm::vec3 ), nullptr );

	///////////////////////////////////////////
	// Setting number of elements inside index
	m_numberOfElements = 3 * sizeof( glm::detail::tvec2<GLubyte> );

	////////////////////////////////////
	// Create and bind the vertex index
	glGenBuffers( 1, &m_vertexBufferIndexID );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_vertexBufferIndexID );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_numberOfElements, cubeIndex, GL_STATIC_DRAW );

	//////////////
	// Unbind VAO
	glBindVertexArray( 0 );
}

void CAxis::createShader() {
	m_vs = CVertexShader::CreateVertexShader( "../resources/Shader/axis.vert" );
	m_fs = CFragmentShader::CreateFragmentShader( "../resources/Shader/axis.frag" );
	m_sp = CShaderProgram::CreateShaderProgram( m_vs, m_fs );
// 	m_sp->setAttributes(1, "in_Position");
	m_sp->createProgram();
}
