/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  Manuel Bellersen <email>

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


#include "ccube.h"
#include <glm/gtc/matrix_transform.hpp>

CCube::CCube( CScene* parentScene, glm::vec3 startPos, glm::vec3 impulse, float size, float mass):
	CMesh( parentScene, startPos ),
	m_sizeX(size),
	m_sizeY(size),
	m_sizeZ(size),
	m_mass(mass),
	m_initialImpulse(impulse)
{
	m_colorDiffuse = glm::vec4(	m_colorDiffuse[0]*startPos[0],
								m_colorDiffuse[1]*startPos[1],
								m_colorDiffuse[2]*startPos[2],
								1.0f );
	m_colorSpecular = glm::vec4(	m_colorSpecular[0]*startPos[0],
									m_colorSpecular[1]*startPos[1],
									m_colorSpecular[2]*startPos[2],
									1.0f );
	m_colorAmbient = glm::vec4(	m_colorAmbient[0]*startPos[0],
								m_colorAmbient[1]*startPos[1],
								m_colorAmbient[2]*startPos[2],
								1.0f );
}

CCube::CCube( CScene* parentScene, glm::vec3 startPos, glm::vec3 impulse, float sizeX, float sizeY, float sizeZ, float mass ): 
CMesh( parentScene, startPos ),
m_sizeX(sizeX),
m_sizeY(sizeY),
m_sizeZ(sizeZ),
m_mass(mass),
m_initialImpulse(impulse)
{
	m_colorDiffuse = glm::vec4(	m_colorDiffuse[0]*startPos[0],
								m_colorDiffuse[1]*-startPos[1],
								m_colorDiffuse[2]*startPos[2],
								1.0f );
	m_colorSpecular = glm::vec4(	m_colorSpecular[0]*startPos[0],
									m_colorSpecular[1]*startPos[1],
									m_colorSpecular[2]*startPos[2],
									1.0f );
	m_colorAmbient = glm::vec4(	m_colorAmbient[0]*startPos[0],
								m_colorAmbient[1]*startPos[1],
								m_colorAmbient[2]*startPos[2],
								1.0f );
}

void CCube::Initialize() {
	CMesh::Initialize();
	CEntity::bindToPhysics(m_mass);
	m_rigidBody->applyImpulse( btVector3(m_initialImpulse[0], m_initialImpulse[1], m_initialImpulse[2]), btVector3(0,0,0));
}

void CCube::Update( float deltaTime ) {
// 	CMesh::Update( deltaTime );
	if( !m_bIsAlive )
		return;
	else {
		if( m_rigidBody != nullptr){
			const btVector3& pos =  m_rigidBody->getCenterOfMassPosition();
			btQuaternion rot = m_rigidBody->getOrientation();
			btMatrix3x3 rotm(rot);
			for( int i = 0; i < 3; ++i){
				for( int j = 0; j < 3; ++j){
					m_modelMatrix[i][j] = rotm[j][i];
				}	
				m_modelMatrix[3][i] = pos[i];
			}
		}
	}
	createNormalMatrix();
}

void CCube::createVerticeAttributes() {
	//////////////////////
	// Set up coordinates
	glm::vec3 cubeCoords[8];
	cubeCoords[0] = glm::vec3( -m_sizeX,  m_sizeY,  m_sizeZ );
	cubeCoords[1] = glm::vec3( m_sizeX,  m_sizeY,  m_sizeZ );
	cubeCoords[2] = glm::vec3( -m_sizeX, -m_sizeY,  m_sizeZ );
	cubeCoords[3] = glm::vec3( m_sizeX, -m_sizeY,  m_sizeZ );

	cubeCoords[4] = glm::vec3( -m_sizeX,  m_sizeY, -m_sizeZ );
	cubeCoords[5] = glm::vec3( m_sizeX,  m_sizeY, -m_sizeZ );
	cubeCoords[6] = glm::vec3( -m_sizeX, -m_sizeY, -m_sizeZ );
	cubeCoords[7] = glm::vec3( m_sizeX, -m_sizeY, -m_sizeZ );

	m_vertexAttributes.reset( new VertexAttributes() );
	m_vertexAttributes->setPosNorm( 8 );


	for( uint64_t i = 0; i < 8; ++i ) {
		for( unsigned j = 0; j < 3; ++j ) {
			m_vertexAttributes->m_positions[i] = cubeCoords[ i];
			m_vertexAttributes->m_normals[i] = glm::vec3( 0 );
		}
	}
}

std::unique_ptr< GLuint[] > CCube::createVertixIndex() {
// 	return CMesh::createVertixIndex();

	uint64_t indexMax = 2*6;
	m_drawType = GL_TRIANGLES;
	indexMax *= 3;
		
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
	
	///////////////////////////////////////////
	// Setting number of elements inside index
	m_numberOfElements = indexMax;

	std::unique_ptr< GLuint[] > index( new GLuint[indexMax] );
	uint64_t i = 0;

	for( uint64_t j = 0; j < 2*6; ++j ) {
		for( uint8_t k = 0; k < 3; ++k ) {
			index.get()[i++] = cubeIndex[j][k];
		}
	}

	return index;
}

btCollisionShape* CCube::getCollisionShape() {
	if( 0 && m_mass != 0.0f){
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

		btTriangleMesh* btTM = new btTriangleMesh();
		assert( m_vertexAttributes->m_sizePositions%3 == 0);

		for( unsigned int i = 0; i < 2*6; ++i){
			int i0 = cubeIndex[i][0];
			int i1 = cubeIndex[i][1];
			int i2 = cubeIndex[i][2];
			glm::vec3& a0 = m_vertexAttributes->m_positions[i0];
			glm::vec3& a1 = m_vertexAttributes->m_positions[i1];
			glm::vec3& a2 = m_vertexAttributes->m_positions[i2];
			btVector3 v1(a0[0], a0[1], a0[2]);
			btVector3 v2(a1[0], a1[1], a1[2]);
			btVector3 v3(a2[0], a2[1], a2[2]);
			btTM->addTriangle(v1,v2,v3);
		}

		return new btBvhTriangleMeshShape(btTM, true);
	}else{
		btCollisionShape* groundShape = new btBoxShape( btVector3( btScalar( m_sizeX ), btScalar( m_sizeY ), btScalar( m_sizeZ ) ) );
		return groundShape;
	}
}


void CCube::createShader() {
// 	CMesh::createShader();
	m_vs = CVertexShader::CreateVertexShader( "../resources/Shader/cube.vert" );
	m_fs = CFragmentShader::CreateFragmentShader( "../resources/Shader/cube.frag" );
	m_sp = CShaderProgram::CreateShaderProgram( m_vs, m_fs );

	///////////////////////////////////////
	// Bind variables used in shader files
	// 	m_sp->setAttributes(1, "in_Position");
	m_sp->createProgram();
}
