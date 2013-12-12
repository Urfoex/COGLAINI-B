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


#include "cbasemesh.h"
#include <stdexcept>
#include "aiToGLM.cpp"

const unsigned int CBaseMesh::m_gMaxBoneMatrixs = 100;

CBaseMesh::CBaseMesh( CBaseMesh& baseMesh ):
	m_logError( baseMesh.m_logError ),
	m_vertexAttributes( baseMesh.m_vertexAttributes ),
	m_isInitialized( baseMesh.m_isInitialized ) {
}

CBaseMesh::~CBaseMesh() {

}

void CBaseMesh::Initialize( std::shared_ptr< std::vector< glm::vec3 > > vertices ) {
	if( m_isInitialized ) {
		m_logError.logStream() << "(II) BaseMesh is already inizialized.\n";
		return;
	}

	createVerticeAttributes( vertices );

	m_isInitialized = true;
}

void CBaseMesh::Destroy() {

}

void CBaseMesh::createVerticeAttributes( std::shared_ptr< std::vector< glm::vec3 > > vertices ) {
	if( vertices == nullptr || vertices.get()->size() == 0 ) {
		throw( std::runtime_error( "(EE) Strange mesh with no vertices found: BaseMash" ) );
	}

// 	if( !m_mesh->HasNormals() ) {
// 		throw( std::runtime_error( "(EE) Mesh with no normals found: " + GetName() ) );
// 	}

// 	unsigned int usage = 0x00;

// 	if( !m_useTexture ) {
// 		m_logError.logStream() << "(II) Not using texture." << std::endl;
// 	} else {
// 		usage |= 0x10;
// 	}

// 	std::unique_ptr< std::vector< aiVertexWeight>[]> vecBone( nullptr );

// 	if( m_mesh->HasBones() ) {
// 		usage |= 0x01;
//
// 		vecBone = createVectorBoneStructur();
//
// 		if( vecBone == nullptr ) {
// 			throw( std::runtime_error( "(EE) VecBoneStructur has not bin created. Why?" ) );
// 		} else {
// 			m_logError.logStream() << "(II) VectorBoneStructur has bin created." << std::endl;
// 		}
// 	}

// 	switch( usage ) {
// 		case 0x00:
	m_vertexAttributes.reset( new VertexAttributes() );
	m_vertexAttributes->setPos( vertices.get()->size() );
// 			break;
// 		case 0x10:
// 			m_vertexAttributes.reset( new VertexAttributes() );
// 			m_vertexAttributes->setPosNormTex( m_mesh->mNumVertices );
// 			break;
// 		case 0x01:
// 			m_vertexAttributes.reset( new VertexAttributes() );
// 			m_vertexAttributes->setPosNormBone( m_mesh->mNumVertices );
// 			break;
// 		case 0x11:
// 			m_vertexAttributes.reset( new VertexAttributes( m_mesh->mNumVertices ) );
// 			break;
// 		default:
// 			break;
// 	}

	for( uint64_t i = 0; i < vertices.get()->size(); ++i ) {
		m_vertexAttributes->m_positions[i] = vertices.get()->at( i );
// 		m_vertexAttributes->m_normals[i] = aiToGLMV3( m_mesh->mNormals[i] );

// 		if( m_useTexture ) {
// 			m_vertexAttributes->m_texCoords[i] = glm::vec2( m_mesh->mTextureCoords[0][i].x, 1.0f - m_mesh->mTextureCoords[0][i].y );
// 		}
//
// 		if( m_mesh->HasBones() ) {
// 			unsigned int j = i;
// 			m_vertexAttributes->m_boneIDs[j] = glm::uvec4( 0, 0, 0, 0 );
// 			m_vertexAttributes->m_boneWeights[j] = glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f );
//
// 			if( vecBone == nullptr ) {
// 				throw( std::runtime_error( "(EE) This is not possible?" ) );
// 			}
//
// 			if( vecBone.get()[j].size() > 4 ) {
// 				auto vec = vecBone.get()[j];
// 				std::sort( vec.begin(), vec.end(), [](aiVertexWeight a, aiVertexWeight b){ return a.mWeight < b.mWeight;});
// 				vecBone.get()[j].resize(4);
// 				vecBone.get()[j][0] = vec[ vec.size() - 1];
// 				vecBone.get()[j][1] = vec[ vec.size() - 2];
// 				vecBone.get()[j][2] = vec[ vec.size() - 3];
// 				vecBone.get()[j][3] = vec[ vec.size() - 4];
// 				// 				std::cerr << "The model has invalid bone weights and is not loaded: " << vecBone.get()[j].size() << std::endl;
// 				// 				throw( std::runtime_error( "The model has invalid bone weights and is not loaded." ) );
// 			}
//
// 			for( unsigned int k = 0; k < vecBone.get()[j].size(); k++ ) {
// 				//NOTE: the indices are cast to GLuint here, the weights to GLfloat!
// 				m_vertexAttributes->m_boneIDs[j][k] = ( GLuint ) vecBone.get()[j][k].mVertexId;
// 				m_vertexAttributes->m_boneWeights[j][k] = ( GLfloat ) vecBone.get()[j][k].mWeight;
// 			}
//
// 			for( unsigned int k = vecBone.get()[j].size(); k < 4; k++ ) {
// 				//NOTE: the indices are cast to GLuint here, the weights to GLfloat!
// 				m_vertexAttributes->m_boneIDs[j][k] = ( GLuint ) vecBone.get()[j][k].mVertexId;
// 				m_vertexAttributes->m_boneWeights[j][k] = 0.0f;
// 			}
// 		}
	}

// 	vecBone.release();
}

void CBaseMesh::Initialize( aiMesh* mesh ) {
	if( m_isInitialized ) {
		m_logError.logStream() << "(II) BaseMesh is already inizialized.\n";
		return;
	}

	createVerticeAttributes( mesh );

	m_isInitialized = true;
}

void CBaseMesh::createVerticeAttributes( aiMesh* mesh ) {
	if( !mesh->HasPositions() ) {
		throw( std::runtime_error( "(EE) Strange mesh with no vertex found: " + std::string( mesh->mName.data ) ) );
	}

	if( !mesh->HasNormals() ) {
		throw( std::runtime_error( "(EE) Mesh with no normals found: " + std::string( mesh->mName.data ) ) );
	}

	unsigned int usage = 0x00;

	if( !mesh->HasTextureCoords( 0 ) ) {
		m_logError.logStream() << "(II) Not using texture." << std::endl;
	} else {
		usage |= 0x10;
	}

	std::unique_ptr< std::vector< aiVertexWeight>[]> vecBone( nullptr );

	if( mesh->HasBones() ) {
		usage |= 0x01;

		vecBone = createVectorBoneStructur( mesh );

		if( vecBone == nullptr ) {
			throw( std::runtime_error( "(EE) VecBoneStructur has not bin created. Why?" ) );
		} else {
			m_logError.logStream() << "(II) VectorBoneStructur has bin created." << std::endl;
		}
	}

	switch( usage ) {
	case 0x00:
		m_vertexAttributes.reset( new VertexAttributes() );
		m_vertexAttributes->setPosNorm( mesh->mNumVertices );
		break;
	case 0x10:
		m_vertexAttributes.reset( new VertexAttributes() );
		m_vertexAttributes->setPosNormTex( mesh->mNumVertices );
		break;
	case 0x01:
		m_vertexAttributes.reset( new VertexAttributes() );
		m_vertexAttributes->setPosNormBone( mesh->mNumVertices );
		break;
	case 0x11:
		m_vertexAttributes.reset( new VertexAttributes( mesh->mNumVertices ) );
		break;
	default:
		break;
	}

	for( uint64_t i = 0; i < mesh->mNumVertices; ++i ) {
		m_vertexAttributes->m_positions[i] = aiToGLMV3( mesh->mVertices[i] );
		m_vertexAttributes->m_normals[i] = aiToGLMV3( mesh->mNormals[i] );

		if( mesh->HasTextureCoords( 0 ) ) {
			m_vertexAttributes->m_texCoords[i] = glm::vec2( mesh->mTextureCoords[0][i].x, 1.0f - mesh->mTextureCoords[0][i].y );
		}

		if( mesh->HasBones() ) {
			unsigned int j = i;
			m_vertexAttributes->m_boneIDs[j] = glm::uvec4( 0, 0, 0, 0 );
			m_vertexAttributes->m_boneWeights[j] = glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f );

			if( vecBone == nullptr ) {
				throw( std::runtime_error( "(EE) This is not possible?" ) );
			}

			if( vecBone.get()[j].size() > 4 ) {
				auto vec = vecBone.get()[j];
				std::sort( vec.begin(), vec.end(), []( aiVertexWeight a, aiVertexWeight b ) {
					return a.mWeight < b.mWeight;
				} );
				vecBone.get()[j].resize( 4 );
				vecBone.get()[j][0] = vec[ vec.size() - 1];
				vecBone.get()[j][1] = vec[ vec.size() - 2];
				vecBone.get()[j][2] = vec[ vec.size() - 3];
				vecBone.get()[j][3] = vec[ vec.size() - 4];
				// 				std::cerr << "The model has invalid bone weights and is not loaded: " << vecBone.get()[j].size() << std::endl;
				// 				throw( std::runtime_error( "The model has invalid bone weights and is not loaded." ) );
			}

			for( unsigned int k = 0; k < vecBone.get()[j].size(); k++ ) {
				//NOTE: the indices are cast to GLuint here, the weights to GLfloat!
				m_vertexAttributes->m_boneIDs[j][k] = ( GLuint ) vecBone.get()[j][k].mVertexId;
				m_vertexAttributes->m_boneWeights[j][k] = ( GLfloat ) vecBone.get()[j][k].mWeight;
			}

			for( unsigned int k = vecBone.get()[j].size(); k < 4; k++ ) {
				//NOTE: the indices are cast to GLuint here, the weights to GLfloat!
				m_vertexAttributes->m_boneIDs[j][k] = ( GLuint ) vecBone.get()[j][k].mVertexId;
				m_vertexAttributes->m_boneWeights[j][k] = 0.0f;
			}
		}
	}

	vecBone.release();
}

std::unique_ptr< std::vector< aiVertexWeight >[] > CBaseMesh::createVectorBoneStructur( aiMesh* mesh )  {
	if( m_gMaxBoneMatrixs < mesh->mNumBones ) {
		std::stringstream ss;
		ss << "(EE) Too many bones: " << mesh->mNumBones << "/" << m_gMaxBoneMatrixs << " " << std::string( mesh->mName.data );
		throw( std::runtime_error( ss.str() ) );
	}

	//read bone indices and weights for bone animation
	std::unique_ptr< std::vector<aiVertexWeight>[]> vTempWeightsPerVertex( new std::vector<aiVertexWeight>[mesh->mNumVertices] );

	for( unsigned int j = 0; j < mesh->mNumBones; j++ ) {
		const aiBone* pBone = mesh->mBones[j];

		for( unsigned int b = 0; b < pBone->mNumWeights; b++ ) {
			vTempWeightsPerVertex.get()[pBone->mWeights[b].mVertexId].push_back( aiVertexWeight( j, pBone->mWeights[b].mWeight ) );
		}
	}

	return vTempWeightsPerVertex;
}

void CBaseMesh::putVerticeAttributesToGPU() {
// 	///////////////////////////////////////
// 	// Load vertex attributes in one array
// 	std::unique_ptr<GLbyte[]> vertices = createArrayOfVerticeAttributes();
//
// 	///////////////////////////////
// 	// Set up index to coordinates
// 	std::unique_ptr< GLuint[] > index = createVertixIndex();
//
//
// 	///////////////////////////////////
// 	// Create and bind the vertex array
// 	glGenVertexArrays( 1, &m_vertexArrayObjectID );
// 	glBindVertexArray( m_vertexArrayObjectID );
//
// 	/////////////////////////////////////
// 	// Create and bind the vertex object
// 	glGenBuffers( 1, &m_vertexBufferObjectID );
// 	glBindBuffer( GL_ARRAY_BUFFER, m_vertexBufferObjectID );
//
// 	GLsizei sizeAllElements = 0;
// 	sizeAllElements += m_vertexAttributes->m_sizePositions;
// 	sizeAllElements += m_vertexAttributes->m_sizePositions;
// 	sizeAllElements += m_vertexAttributes->m_sizeNormals;
// 	sizeAllElements += m_vertexAttributes->m_sizeTexCoords;
// 	sizeAllElements += m_vertexAttributes->m_sizeBoneWeights;
// 	sizeAllElements += m_vertexAttributes->m_sizeBoneIDs;
//
// 	glBufferData( GL_ARRAY_BUFFER, sizeAllElements, vertices.get(), GL_STATIC_DRAW );
//
//
// 	////////////////////////////
// 	// Set up vertex attributes
// 	glEnableVertexAttribArray( 0 );
// 	glEnableVertexAttribArray( 1 );
//
// 	if( m_useTexture ) {
// 		glEnableVertexAttribArray( 2 );
// 	}
//
// 	if( m_mesh->HasBones() ) {
// 		glEnableVertexAttribArray( 3 );
// 		glEnableVertexAttribArray( 4 );
// 	}
//
// 	unsigned int next = 0;
// 	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( glm::vec3 ), nullptr );
// 	next += m_vertexAttributes->m_sizePositions;
// 	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( glm::vec3 ), ( GLvoid* )next );
// 	next += m_vertexAttributes->m_sizeNormals;
//
// 	if( m_useTexture ) {
// 		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( glm::vec2 ), ( GLvoid* )next );
// 		next += m_vertexAttributes->m_sizeTexCoords;
// 	}
//
// 	if( m_mesh->HasBones() ) {
// 		glVertexAttribPointer( 3, 4, GL_FLOAT, GL_FALSE, sizeof( glm::vec4 ), ( GLvoid* )next );
// 		next += m_vertexAttributes->m_sizeBoneWeights;
// 		glVertexAttribPointer( 4, 4, GL_FLOAT, GL_FALSE, sizeof( glm::vec4 ), ( GLvoid* )next );
// 	}
//
//
// 	////////////////////////////////////
// 	// Create and bind the vertex index
// 	glGenBuffers( 1, &m_vertexBufferIndexID );
// 	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_vertexBufferIndexID );
// 	glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_numberOfElements * sizeof( GLuint ), index.get(), GL_STATIC_DRAW );
//
// 	//////////////
// 	// Unbind VAO
// 	glBindVertexArray( 0 );
}
