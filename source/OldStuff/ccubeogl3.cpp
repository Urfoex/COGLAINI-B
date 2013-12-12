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


#include "ccubeogl3.h"
#include "cgame.h"


CCubeOGL3::CCubeOGL3( CGame* game ): IDrawableObjectOGL3( game ) {

}

void CCubeOGL3::transform() {

}

CCubeOGL3::~CCubeOGL3() {
}

void CCubeOGL3::createObject() {

	//////////////////////
	// Set up coordinates
	float size = 4.0f;
	glm::vec3 cubeCoords[8];
	cubeCoords[0] = glm::vec3( -size,  size,  size );
	cubeCoords[1] = glm::vec3( size,  size,  size );
	cubeCoords[2] = glm::vec3( -size, -size,  size );
	cubeCoords[3] = glm::vec3( size, -size,  size );

	cubeCoords[4] = glm::vec3( -size,  size, -size );
	cubeCoords[5] = glm::vec3( size,  size, -size );
	cubeCoords[6] = glm::vec3( -size, -size, -size );
	cubeCoords[7] = glm::vec3( size, -size, -size );

	///////////////////////////////
	// Set up index to coordinates
	glm::detail::tvec3<GLubyte> cubeIndex[12]; // 2 triangle per quad, 6 quads on cube -> 2*6
	cubeIndex[0] = glm::detail::tvec3<GLubyte>( 0, 1, 2 );
	cubeIndex[1] = glm::detail::tvec3<GLubyte>( 2, 1, 3 );

	cubeIndex[2] = glm::detail::tvec3<GLubyte>( 5, 4, 7 );
	cubeIndex[3] = glm::detail::tvec3<GLubyte>( 7, 4, 6 );

	cubeIndex[4] = glm::detail::tvec3<GLubyte>( 1, 5, 3 );
	cubeIndex[5] = glm::detail::tvec3<GLubyte>( 3, 5, 7 );

	cubeIndex[6] = glm::detail::tvec3<GLubyte>( 4, 0, 6 );
	cubeIndex[7] = glm::detail::tvec3<GLubyte>( 6, 0, 2 );

	cubeIndex[8] = glm::detail::tvec3<GLubyte>( 4, 5, 0 );
	cubeIndex[9] = glm::detail::tvec3<GLubyte>( 0, 5, 1 );

	cubeIndex[10] = glm::detail::tvec3<GLubyte>( 2, 3, 6 );
	cubeIndex[11] = glm::detail::tvec3<GLubyte>( 6, 3, 7 );


	///////////////////////////////////
	// Create and bind the vertex array
	glGenVertexArrays( 1, &m_vertexArrayObjectID );
	glBindVertexArray( m_vertexArrayObjectID );

	/////////////////////////////////////
	// Create and bind the vertex object
	glGenBuffers( 1, &m_vertexBufferObjectID );
	glBindBuffer( GL_ARRAY_BUFFER, m_vertexBufferObjectID );
	glBufferData( GL_ARRAY_BUFFER, 8 * sizeof( glm::vec3 ), cubeCoords, GL_STATIC_DRAW );

	////////////////////////////
	// Set up vertex attributes
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( glm::vec3 ), nullptr );

	///////////////////////////////////////////
	// Setting number of elements inside index
	m_numberOfElements = 12 * sizeof( glm::detail::tvec3<GLubyte> );

	////////////////////////////////////
	// Create and bind the vertex index
	glGenBuffers( 1, &m_vertexBufferIndexID );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_vertexBufferIndexID );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_numberOfElements, cubeIndex, GL_STATIC_DRAW );

	//////////////
	// Unbind VAO
	glBindVertexArray( 0 );
}

void CCubeOGL3::createShader() {
	m_vs = CVertexShader::CreateVertexShader( "../resources/Shader/cube.vert" );
	m_fs = CFragmentShader::CreateFragmentShader( "../resources/Shader/cube.frag" );
	m_sp = CShaderProgram::CreateShaderProgram( m_vs, m_fs );
	m_sp->setAttributes( 1, "in_Position" );
	m_sp->createProgram();
}
