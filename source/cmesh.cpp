#include "cmesh.h"
#include "headers_with_gcc_warnings/assimp.h"
#include "headers_with_gcc_warnings/aiMesh.h"
#include "headers_with_gcc_warnings/aiScene.h"
#include "cscene.h"
#include <SFML/Graphics.hpp>
#include "SFML_OpenGL.h"
#include <stdexcept>
#include "cgame.h"
#include "aiToGLM.cpp"
#include "../libraries/glm/gtc/matrix_inverse.hpp"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"

const unsigned int CMesh::m_gMaxBoneMatrixs = 100;

CMesh::CMesh( CScene* parentScene, aiMesh* mesh, uint64_t aiID ):
	CEntity( parentScene, mesh->mName.data ),
	m_mesh( mesh ),
	m_vertexArrayObjectID( 0 ),
	m_vertexBufferObjectID(),
	m_vertexBufferIndexID( 0 ),
	m_numberOfElements( 0 ),
	m_drawType( 0 ),
	m_vs(),
	m_fs(),
	m_sp(),
	m_colorDiffuse( 0.8f, 0.8f, 0.8f, 1.0f ),
	m_colorSpecular( 0.0f, 0.0f, 0.0f, 1.0f ),
	m_colorAmbient( 0.2f, 0.2f, 0.2f, 1.0f ),
	m_colorShininess( 0.0f ),
	m_materialUBOBuffer(),
	m_materialUniformBlockBufferIndex( 0 ),
	m_materialUniformBlockBinding( 0 ),
	m_aiID( aiID ),
	m_nodeID( 0 ),
	m_diffuseTexture( 0 ),
	m_useTexture( false ),
	m_vertexAttributes( nullptr ),
	m_normalMatrix( 1 ),
	m_boneMatrices( nullptr ),
	m_hasBones( mesh->HasBones()),
	m_numBones( mesh->mNumBones)
{}

CMesh::CMesh( CScene* parentScene, glm::core::type::vec3 startPos ): CEntity( parentScene, "selfmade"),
m_mesh( nullptr ),
m_vertexArrayObjectID( 0 ),
m_vertexBufferObjectID(),
m_vertexBufferIndexID( 0 ),
m_numberOfElements( 0 ),
m_drawType( 0 ),
m_vs(),
m_fs(),
m_sp(),
m_colorDiffuse( 0.8f, 0.8f, 0.8f, 1.0f ),
m_colorSpecular( 0.0f, 0.0f, 0.0f, 1.0f ),
m_colorAmbient( 0.2f, 0.2f, 0.2f, 1.0f ),
m_colorShininess( 0.0f ),
m_materialUBOBuffer(),
m_materialUniformBlockBufferIndex( 0 ),
m_materialUniformBlockBinding( 0 ),
m_aiID( 0 ),
m_nodeID( 0 ),
m_diffuseTexture( 0 ),
m_useTexture( false ),
m_vertexAttributes( nullptr ),
m_normalMatrix( 1 ),
m_boneMatrices( nullptr ),
m_hasBones( false),
m_numBones(0)
{
	m_modelMatrix = glm::translate( m_modelMatrix, startPos);
}

CMesh::~CMesh() {
// //	m_logError.logStream() << "Removing mesh." << std::endl;
	glDeleteTextures( 1, &m_diffuseTexture );
	glDeleteBuffers( 1, &m_vertexBufferObjectID );
	glDeleteBuffers( 1, &m_vertexBufferIndexID );
	glDeleteBuffers( 1, &m_materialUBOBuffer );
	glDeleteVertexArrays( 1, &m_vertexArrayObjectID );
}

void CMesh::debugOutput() {
//	m_logError.logStream() << "Name: " << m_mesh->mName.data << std::endl;
//	m_logError.logStream() << "Has Bones: " << m_hasBones << std::endl;
//	m_logError.logStream() << "Has Faces: " << m_mesh->HasFaces() << std::endl;
//	m_logError.logStream() << "Has Normals: " << m_mesh->HasNormals() << std::endl;
//	m_logError.logStream() << "Has Position: " << m_mesh->HasPositions() << std::endl;
//	m_logError.logStream() << "Has Tang&Bitang: " << m_mesh->HasTangentsAndBitangents() << std::endl;
//	m_logError.logStream() << "Has GetNumColorChannels: " << m_mesh->GetNumColorChannels() << std::endl;

	for( uint64_t ii = 0; ii < m_mesh->GetNumColorChannels(); ++ii ) {
	//	m_logError.logStream() << "Has VertexColors(" << ii << "): " << m_mesh->HasVertexColors( ii ) << std::endl;
	}

//	m_logError.logStream() << "Has GetNumUVChannels: " << m_mesh->GetNumUVChannels() << std::endl;

	for( uint64_t ii = 0; ii < m_mesh->GetNumUVChannels(); ++ii ) {
	//	m_logError.logStream() << "Has TexCoords(" << ii << "): " << m_mesh->HasTextureCoords( ii ) << std::endl;
	}

}

void CMesh::createVerticeAttributes() {
	if( !m_mesh->HasPositions() ) {
		throw( std::runtime_error( "(EE) Strange mesh with no vertex found: " + GetName() ) );
	}

	if( !m_mesh->HasNormals() ) {
		throw( std::runtime_error( "(EE) Mesh with no normals found: " + GetName() ) );
	}

	unsigned int usage = 0x00;

	if( !m_useTexture ) {
	//	m_logError.logStream() << "(II) Not using texture." << std::endl;
	} else {
		usage |= 0x10;
	}

	std::unique_ptr< std::vector< aiVertexWeight>[]> vecBone( nullptr );

	if( m_hasBones ) {
		usage |= 0x01;

		vecBone = createVectorBoneStructur();

		if( vecBone == nullptr ) {
			throw( std::runtime_error( "(EE) VecBoneStructur has not bin created. Why?" ) );
		} else {
		//	m_logError.logStream() << "(II) VectorBoneStructur has bin created." << std::endl;
		}
	}

	switch( usage ) {
	case 0x00:
		m_vertexAttributes.reset( new VertexAttributes() );
		m_vertexAttributes->setPosNorm( m_mesh->mNumVertices );
		break;
	case 0x10:
		m_vertexAttributes.reset( new VertexAttributes() );
		m_vertexAttributes->setPosNormTex( m_mesh->mNumVertices );
		break;
	case 0x01:
		m_vertexAttributes.reset( new VertexAttributes() );
		m_vertexAttributes->setPosNormBone( m_mesh->mNumVertices );
		break;
	case 0x11:
		m_vertexAttributes.reset( new VertexAttributes( m_mesh->mNumVertices ) );
		break;
	default:
		break;
	}

	for( uint64_t i = 0; i < m_mesh->mNumVertices; ++i ) {
		m_vertexAttributes->m_positions[i] = aiToGLMV3( m_mesh->mVertices[i] );
		m_vertexAttributes->m_normals[i] = aiToGLMV3( m_mesh->mNormals[i] );

		if( m_useTexture ) {
			m_vertexAttributes->m_texCoords[i] = glm::vec2( m_mesh->mTextureCoords[0][i].x, 1.0f - m_mesh->mTextureCoords[0][i].y );
		}

		if( m_hasBones ) {
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
				// makes it crash on my pc @F
				m_vertexAttributes->m_boneIDs[j][k] = 0;
				m_vertexAttributes->m_boneWeights[j][k] = 0.0f;
			}
		}
	}

	vecBone.release();
}

std::unique_ptr< std::vector<aiVertexWeight>[]> CMesh::createVectorBoneStructur() {
	if( m_gMaxBoneMatrixs < m_numBones ) {
		std::stringstream ss;
		ss << "(EE) Too many bones: " << m_numBones << "/" << m_gMaxBoneMatrixs << " " << m_NameByNode << ":" << m_pcName;
		throw( std::runtime_error( ss.str() ) );
	}

	//read bone indices and weights for bone animation
	std::unique_ptr< std::vector<aiVertexWeight>[]> vTempWeightsPerVertex( new std::vector<aiVertexWeight>[m_mesh->mNumVertices] );

	for( unsigned int j = 0; j < m_numBones; j++ ) {
		const aiBone* pBone = m_mesh->mBones[j];

		for( unsigned int b = 0; b < pBone->mNumWeights; b++ ) {
			vTempWeightsPerVertex.get()[pBone->mWeights[b].mVertexId].push_back( aiVertexWeight( j, pBone->mWeights[b].mWeight ) );
		}
	}

	return vTempWeightsPerVertex;
}


std::unique_ptr< GLuint[] > CMesh::createVertixIndex() {
	if( !m_mesh->HasFaces() ) {
		throw( std::runtime_error( "Strange mesh with no faces found: " + GetName() ) );
	}

	uint64_t indexMax = m_mesh->mNumFaces;

	switch( m_mesh->mPrimitiveTypes ) {
	case aiPrimitiveType_POINT:
		m_drawType = GL_POINTS;
		// 			indexMax = m_mesh->mNumFaces;
		break;
	case aiPrimitiveType_LINE:
		m_drawType = GL_LINES;
		indexMax *= 2;
		break;
	case aiPrimitiveType_TRIANGLE:
		m_drawType = GL_TRIANGLES;
		indexMax *= 3;
		break;
	case aiPrimitiveType_POLYGON:
		throw( std::runtime_error( "(EE) Strange polygon mesh found: " + GetName() ) );
		break;
	default:
		throw( std::runtime_error( "(EE) Strange unknown mesh found: " + GetName() ) );
		break;
	}

	///////////////////////////////////////////
	// Setting number of elements inside index
	m_numberOfElements = indexMax;

	std::unique_ptr< GLuint[] > index( new GLuint[indexMax] );
	uint64_t i = 0;

	for( uint64_t j = 0; j < m_mesh->mNumFaces; ++j ) {
		for( uint8_t k = 0; k < m_mesh->mFaces[j].mNumIndices; ++k ) {
			index.get()[i++] = m_mesh->mFaces[j].mIndices[k];
		}
	}

	return index;
}

void CMesh::createColors( aiMaterial** materials ) {
	if( materials == nullptr ) {
		return;
	}

	aiMaterial* material = materials[m_mesh->mMaterialIndex];
	aiColor4D aiDiffuse;
	aiColor4D aiSpecular;
	aiColor4D aiAmbient;
// 	aiColor4D aiEmit;

// 	material->Get()
	if( AI_SUCCESS == aiGetMaterialColor( material, AI_MATKEY_COLOR_DIFFUSE, &aiDiffuse ) ) {
		m_colorDiffuse = aiToGLMV4( aiDiffuse );
	}

	if( AI_SUCCESS == aiGetMaterialColor( material, AI_MATKEY_COLOR_SPECULAR, &aiSpecular ) ) {
		m_colorSpecular = aiToGLMV4( aiSpecular );
	}

	if( AI_SUCCESS == aiGetMaterialColor( material, AI_MATKEY_COLOR_AMBIENT, &aiAmbient ) ) {
		m_colorAmbient = aiToGLMV4( aiAmbient );
	}

	float shi;

	if( AI_SUCCESS == material->Get( AI_MATKEY_SHININESS, shi ) ) {
		m_colorShininess = shi;
	}

// 	if(AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &aiEmit)){
// 		m_colorEmit = aiToGLMV4(aiEmit);
// 	}

}

std::unique_ptr<GLbyte[]> CMesh::createArrayOfVerticeAttributes() {
	unsigned int sizeAllElements = 0;
	sizeAllElements += m_vertexAttributes->m_sizePositions;
	sizeAllElements += m_vertexAttributes->m_sizePositions;
	sizeAllElements += m_vertexAttributes->m_sizeNormals;
	sizeAllElements += m_vertexAttributes->m_sizeTexCoords;
	sizeAllElements += m_vertexAttributes->m_sizeBoneWeights;
	sizeAllElements += m_vertexAttributes->m_sizeBoneIDs;

	std::unique_ptr<GLbyte[]> vertAttrArray( new GLbyte[sizeAllElements] );

	unsigned int next = 0;
	memcpy( vertAttrArray.get(), m_vertexAttributes->m_positions.get(), m_vertexAttributes->m_sizePositions );
	next += m_vertexAttributes->m_sizePositions;
	memcpy( vertAttrArray.get() + next, m_vertexAttributes->m_normals.get(), m_vertexAttributes->m_sizeNormals );
	next += m_vertexAttributes->m_sizeNormals;

	if( m_vertexAttributes->m_normals != nullptr ) {
		memcpy( vertAttrArray.get() + next, m_vertexAttributes->m_texCoords.get(), m_vertexAttributes->m_sizeTexCoords );
		next += m_vertexAttributes->m_sizeTexCoords;
	}

	if( m_hasBones ) {
		memcpy( vertAttrArray.get() + next, m_vertexAttributes->m_boneWeights.get(), m_vertexAttributes->m_sizeBoneWeights );
		next += m_vertexAttributes->m_sizeBoneWeights;
		memcpy( vertAttrArray.get() + next, m_vertexAttributes->m_boneIDs.get(), m_vertexAttributes->m_sizeBoneIDs );
	}

	return vertAttrArray;
}

void CMesh::Initialize() {
	CEntity::Initialize();
	
	////////////////////////////
	// Set up vertex attributes
	createVerticeAttributes();
	putVerticeAttributesToGPU();
	
	createShader();
	createMaterial();
	setMaterial();
}

void CMesh::Initialize( aiMaterial** materials ) {
	CEntity::Initialize();
// 	debugOutput();

	///////////////////////////////////////
	// Load color & textures from material
	createColors( materials );
	loadTextures( materials );

	////////////////////////////
	// Set up vertex attributes
	createVerticeAttributes();
	putVerticeAttributesToGPU();

	createShader();
	createMaterial();
	setMaterial();
}

void CMesh::putVerticeAttributesToGPU() {
	///////////////////////////////////////
	// Load vertex attributes in one array
	std::unique_ptr<GLbyte[]> vertices = createArrayOfVerticeAttributes();

	///////////////////////////////
	// Set up index to coordinates
	std::unique_ptr< GLuint[] > index = createVertixIndex();


	///////////////////////////////////
	// Create and bind the vertex array
	glGenVertexArrays( 1, &m_vertexArrayObjectID );
	glBindVertexArray( m_vertexArrayObjectID );

	/////////////////////////////////////
	// Create and bind the vertex object
	glGenBuffers( 1, &m_vertexBufferObjectID );
	glBindBuffer( GL_ARRAY_BUFFER, m_vertexBufferObjectID );

	GLsizei sizeAllElements = 0;
	sizeAllElements += m_vertexAttributes->m_sizePositions;
	sizeAllElements += m_vertexAttributes->m_sizePositions;
	sizeAllElements += m_vertexAttributes->m_sizeNormals;
	sizeAllElements += m_vertexAttributes->m_sizeTexCoords;
	sizeAllElements += m_vertexAttributes->m_sizeBoneWeights;
	sizeAllElements += m_vertexAttributes->m_sizeBoneIDs;

	glBufferData( GL_ARRAY_BUFFER, sizeAllElements, vertices.get(), GL_STATIC_DRAW );


	////////////////////////////
	// Set up vertex attributes
	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );

	if( m_useTexture ) {
		glEnableVertexAttribArray( 2 );
	}

	if( m_hasBones ) {
		glEnableVertexAttribArray( 3 );
		glEnableVertexAttribArray( 4 );
	}

	unsigned int next = 0;
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( glm::vec3 ), nullptr );
	next += m_vertexAttributes->m_sizePositions;
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( glm::vec3 ), ( GLvoid* )next );
	next += m_vertexAttributes->m_sizeNormals;

	if( m_useTexture ) {
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( glm::vec2 ), ( GLvoid* )next );
		next += m_vertexAttributes->m_sizeTexCoords;
	}

	if( m_hasBones ) {
		glVertexAttribPointer( 3, 4, GL_FLOAT, GL_FALSE, sizeof( glm::vec4 ), ( GLvoid* )next );
		next += m_vertexAttributes->m_sizeBoneWeights;
		glVertexAttribPointer( 4, 4, GL_FLOAT, GL_FALSE, sizeof( glm::vec4 ), ( GLvoid* )next );
	}


	////////////////////////////////////
	// Create and bind the vertex index
	glGenBuffers( 1, &m_vertexBufferIndexID );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_vertexBufferIndexID );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_numberOfElements * sizeof( GLuint ), index.get(), GL_STATIC_DRAW );

	//////////////
	// Unbind VAO
	glBindVertexArray( 0 );
}

void CMesh::createMaterial() {
	m_materialUniformBlockBufferIndex = glGetUniformBlockIndex( m_sp->getShaderProgram(), "Material" );
	checkGLErrorLF( m_logError.errorStream() );
	
	GLint activeBlock = 0;
	
	glGetActiveUniformBlockiv( m_sp->getShaderProgram(), m_materialUniformBlockBufferIndex, GL_UNIFORM_BLOCK_BINDING, &activeBlock );
	checkGLErrorLF( m_logError.errorStream() );
	if( activeBlock != 0){
		m_materialUniformBlockBinding = activeBlock;
	}else{
		m_materialUniformBlockBinding = m_sp->getNextFreeUniformBlockBinding();
	}	

	glGenBuffers( 1, &m_materialUBOBuffer );
	checkGLErrorLF( m_logError.errorStream() );

	// Alternative
	glBindBuffer( GL_UNIFORM_BUFFER, m_materialUBOBuffer );
	checkGLErrorLF( m_logError.errorStream() );

	glBufferData( GL_UNIFORM_BUFFER, sizeof( glm::vec4 ) * 3 + sizeof( float ), nullptr, GL_STREAM_DRAW );
	checkGLErrorLF( m_logError.errorStream() );

	glBindBuffer( GL_UNIFORM_BUFFER, 0 );
	checkGLErrorLF( m_logError.errorStream() );
}

void CMesh::loadTextures( aiMaterial** materials ) {
	if( materials == nullptr ) {
// 	//	m_logError.logStream() << "(II) No materials." << std::endl;
		return;
	}

// //	m_logError.logStream() << "(II) Ambient: " << materials[m_mesh->mMaterialIndex]->GetTextureCount( aiTextureType_AMBIENT ) << std::endl;
// //	m_logError.logStream() << "(II) Diffuse: " << materials[m_mesh->mMaterialIndex]->GetTextureCount( aiTextureType_DIFFUSE ) << std::endl;
// //	m_logError.logStream() << "(II) Displac: " << materials[m_mesh->mMaterialIndex]->GetTextureCount( aiTextureType_DISPLACEMENT ) << std::endl;
// //	m_logError.logStream() << "(II) Emissiv: " << materials[m_mesh->mMaterialIndex]->GetTextureCount( aiTextureType_EMISSIVE ) << std::endl;
// //	m_logError.logStream() << "(II) Height : " << materials[m_mesh->mMaterialIndex]->GetTextureCount( aiTextureType_HEIGHT ) << std::endl;
// //	m_logError.logStream() << "(II) Lightma: " << materials[m_mesh->mMaterialIndex]->GetTextureCount( aiTextureType_LIGHTMAP ) << std::endl;
// //	m_logError.logStream() << "(II) None   : " << materials[m_mesh->mMaterialIndex]->GetTextureCount( aiTextureType_NONE ) << std::endl;
// //	m_logError.logStream() << "(II) Normals: " << materials[m_mesh->mMaterialIndex]->GetTextureCount( aiTextureType_NORMALS ) << std::endl;
// //	m_logError.logStream() << "(II) Opacity: " << materials[m_mesh->mMaterialIndex]->GetTextureCount( aiTextureType_OPACITY ) << std::endl;
// //	m_logError.logStream() << "(II) Reflect: " << materials[m_mesh->mMaterialIndex]->GetTextureCount( aiTextureType_REFLECTION ) << std::endl;
// //	m_logError.logStream() << "(II) Shinine: " << materials[m_mesh->mMaterialIndex]->GetTextureCount( aiTextureType_SHININESS ) << std::endl;
// //	m_logError.logStream() << "(II) Specula: " << materials[m_mesh->mMaterialIndex]->GetTextureCount( aiTextureType_SPECULAR ) << std::endl;
// //	m_logError.logStream() << "(II) Unknown: " << materials[m_mesh->mMaterialIndex]->GetTextureCount( aiTextureType_UNKNOWN ) << std::endl;

	if( materials[m_mesh->mMaterialIndex]->GetTextureCount( aiTextureType_DIFFUSE ) ) {
		loadDiffuseTexture( materials[m_mesh->mMaterialIndex] );
	}
}

void CMesh::loadDiffuseTexture( aiMaterial* material ) {
	aiString texturePath;
	//aiTextureMapping textureMapping;
	//unsigned int uvindex;
	material->GetTexture( aiTextureType_DIFFUSE, 0, &texturePath );
//	m_logError.logStream() << "(II) Using texture: " << texturePath.data << std::endl;
	char sep = '/';
	char sep2 = '\\';
	std::string filename = texturePath.data;
	{
		{
			std::string s = filename;
			size_t i = s.rfind( sep2, s.length() );

			if( i != s.npos ) {
				filename = s.substr( i + 1, s.length() - i );
			}
		}{
			std::string s = filename;
			size_t i = s.rfind( sep, s.length() );

			if( i != s.npos ) {
				filename = s.substr( i + 1, s.length() - i );
			}
		}
	}
	sf::Image image;
	std::stringstream fileWithPath;
	fileWithPath << "../resources/Textures/" << filename;

	if( !image.LoadFromFile( fileWithPath.str() ) ) {
	//	m_logError.logStream() << "(II) Could not load texture: " << fileWithPath.str() << std::endl;
		return;
	} else {
	//	m_logError.logStream() << "(II) Loaded texture: " << fileWithPath.str() << std::endl;
	}

	glGenTextures( 1, &m_diffuseTexture );
	checkGLErrorLF( m_logError.errorStream() );
	glBindTexture( GL_TEXTURE_2D, m_diffuseTexture );
	checkGLErrorLF( m_logError.errorStream() );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	checkGLErrorLF( m_logError.errorStream() );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, image.GetWidth(), image.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.GetPixelsPtr() );
	checkGLErrorLF( m_logError.errorStream() );
	m_useTexture = true;

}


void CMesh::createShader() {
	// TODO: use one shader for multiplay meshes
	//
	m_vs = CVertexShader::CreateVertexShader( "../resources/Shader/pointLight.vert" );
	m_fs = CFragmentShader::CreateFragmentShader( "../resources/Shader/pointLight.frag" );
	m_sp = CShaderProgram::CreateShaderProgram( m_vs, m_fs );

	///////////////////////////////////////
	// Bind variables used in shader files
// 	m_sp->setAttributes(1, "in_Position");
	m_sp->createProgram();
}

void CMesh::setMaterial() {
	auto& program = m_sp->getShaderProgram();
	// Alternative

	glBindBuffer( GL_UNIFORM_BUFFER, m_materialUBOBuffer );
	checkGLErrorLF( m_logError.errorStream() );

	glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof( glm::vec4 ), glm::value_ptr( m_colorAmbient ) );
	checkGLErrorLF( m_logError.errorStream() );

	glBufferSubData( GL_UNIFORM_BUFFER, sizeof( glm::vec4 ), sizeof( glm::vec4 ), glm::value_ptr( m_colorDiffuse ) );
	checkGLErrorLF( m_logError.errorStream() );

	glBufferSubData( GL_UNIFORM_BUFFER, sizeof( glm::vec4 ) * 2, sizeof( glm::vec4 ), glm::value_ptr( m_colorSpecular ) );
	checkGLErrorLF( m_logError.errorStream() );

	glBufferSubData( GL_UNIFORM_BUFFER, sizeof( glm::vec4 ) * 3, sizeof( float ), &m_colorShininess );
	checkGLErrorLF( m_logError.errorStream() );

	glBindBuffer( GL_UNIFORM_BUFFER, 0 );
	checkGLErrorLF( m_logError.errorStream() );
	
	glUniformBlockBinding( program, m_materialUniformBlockBufferIndex, m_materialUniformBlockBinding );
	checkGLErrorLF( m_logError.errorStream() );

	glBindBufferRange( GL_UNIFORM_BUFFER, m_materialUniformBlockBinding, m_materialUBOBuffer, 0, sizeof( glm::vec4 ) + sizeof( float ) );
	checkGLErrorLF( m_logError.errorStream() );
}

void CMesh::createNormalMatrix() {
	for( unsigned int y = 0; y < 3; ++y ) {
		for( unsigned int x = 0; x < 3; ++x ) {
			m_normalMatrix[y][x] = m_modelMatrix[y][x];
		}
	}

	m_normalMatrix = m_normalMatrix._inverse();
	m_normalMatrix = glm::transpose( m_normalMatrix );
}

void CMesh::Update( float deltaTime ) {
	CEntity::Update( deltaTime );
	
	createNormalMatrix();

	if( m_hasBones ) {
		m_boneMatrices = std::move( createBoneMats() );
	}
}

bool CMesh::useTexture() {
	return true;
}

void CMesh::Draw( float ) {
	glUseProgram( m_sp->getShaderProgram() );
	checkGLErrorLF( m_logError.errorStream() );

	glBindVertexArray( m_vertexArrayObjectID );
	checkGLErrorLF( m_logError.errorStream() );

	// 	GLint time = glGetUniformLocation( m_sp->getShaderProgram(), "time");
	// 	GLint resolution = glGetUniformLocation( m_sp->getShaderProgram(), "resolution");
	// 	glUniform1f( time, std::time(NULL)/1000.0f);
	// 	glUniform2f( resolution, 640.0f, 480.0f);

	{
		GLint modelMatrixLocation = glGetUniformLocation( m_sp->getShaderProgram(), "modelMatrix" );
		checkGLErrorLF( m_logError.errorStream() );
		glUniformMatrix4fv( modelMatrixLocation, 1, GL_FALSE, glm::value_ptr( m_modelMatrix ) );
		checkGLErrorLF( m_logError.errorStream() );
	}
	{
		GLint modelViewProjectionMatrixLocation = glGetUniformLocation( m_sp->getShaderProgram(), "modelViewProjectionMatrix" );
		checkGLErrorLF( m_logError.errorStream() );
		glm::mat4 mvp =  m_pParentScene->getCurrentCamera()->getProjectionViewMatrix() * m_modelMatrix;
		glUniformMatrix4fv( modelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr( mvp ) );
		checkGLErrorLF( m_logError.errorStream() );
	}
	{
		GLint normalMatrixLocation = glGetUniformLocation( m_sp->getShaderProgram(), "normalMatrix" );
		checkGLErrorLF( m_logError.errorStream() );
		glUniformMatrix3fv( normalMatrixLocation, 1, GL_FALSE, glm::value_ptr( m_normalMatrix ) );
		checkGLErrorLF( m_logError.errorStream() );
	}
	{
		glUniformBlockBinding( m_sp->getShaderProgram(), m_materialUniformBlockBufferIndex, m_materialUniformBlockBinding );
		checkGLErrorLF( m_logError.errorStream() );
		glBindBufferRange( GL_UNIFORM_BUFFER, m_materialUniformBlockBinding, m_materialUBOBuffer, 0, sizeof( glm::vec4 ) + sizeof( float ) );
		checkGLErrorLF( m_logError.errorStream() );
	}
	{
		GLint useTextureLocation = glGetUniformLocation( m_sp->getShaderProgram(), "useTexture" );
		checkGLErrorLF( m_logError.errorStream() );
		glUniform1ui( useTextureLocation, m_useTexture && useTexture() );
		checkGLErrorLF( m_logError.errorStream() );

		if( m_useTexture && useTexture() ) {
			GLint textureLocation = glGetUniformLocation( m_sp->getShaderProgram(), "theTexture" );
			checkGLErrorLF( m_logError.errorStream() );
			glUniform1i( textureLocation, 0 );
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, m_diffuseTexture );
			checkGLErrorLF( m_logError.errorStream() );
		}
	}
	{
		GLint useBonesLocation = glGetUniformLocation( m_sp->getShaderProgram(), "useBones" );
		glUniform1ui( useBonesLocation, m_hasBones );

		if( m_hasBones ) {
			if( m_boneMatrices != nullptr ) {
				GLint boneMatrixLocation = glGetUniformLocation( m_sp->getShaderProgram(), "boneMatrix" );
				checkGLErrorLF( m_logError.errorStream() );
				glUniformMatrix4fv( boneMatrixLocation, m_gMaxBoneMatrixs, GL_FALSE, glm::value_ptr( *m_boneMatrices.get() ) );
				checkGLErrorLF( m_logError.errorStream() );
			} else {
				throw( std::runtime_error( "(EE) Where are my bones?" ) );
			}
		}
	}

	glDrawElements( m_drawType, m_numberOfElements, GL_UNSIGNED_INT, nullptr );
	checkGLErrorLF( m_logError.errorStream() );

}

std::unique_ptr< glm::mat4[]> CMesh::createBoneMats() {
	std::unique_ptr< glm::mat4[]> matrix( nullptr );
	auto animator = m_pParentScene->getAnimator();

	//upload bone matrices
	if( animator != nullptr ) {
		const std::vector<aiMatrix4x4>& vBoneMatrices = animator->GetBoneMatrices( m_pContentNode, m_nodeID );

		if( vBoneMatrices.size() != m_numBones ) {
		//	m_logError.logStream() << "(II) " << vBoneMatrices.size() << "!=" << m_numBones << std::endl;
			return matrix;
		}

		matrix.reset( new glm::mat4[m_gMaxBoneMatrixs] );

		for( unsigned int j = 0; j < m_numBones; j++ ) {
			if( j < m_gMaxBoneMatrixs ) {
				matrix[j][0][0] = vBoneMatrices[j].a1;
				matrix[j][0][1] = vBoneMatrices[j].b1;
				matrix[j][0][2] = vBoneMatrices[j].c1;
				matrix[j][0][3] = vBoneMatrices[j].d1;
				matrix[j][1][0] = vBoneMatrices[j].a2;
				matrix[j][1][1] = vBoneMatrices[j].b2;
				matrix[j][1][2] = vBoneMatrices[j].c2;
				matrix[j][1][3] = vBoneMatrices[j].d2;
				matrix[j][2][0] = vBoneMatrices[j].a3;
				matrix[j][2][1] = vBoneMatrices[j].b3;
				matrix[j][2][2] = vBoneMatrices[j].c3;
				matrix[j][2][3] = vBoneMatrices[j].d3;
				matrix[j][3][0] = vBoneMatrices[j].a4;
				matrix[j][3][1] = vBoneMatrices[j].b4;
				matrix[j][3][2] = vBoneMatrices[j].c4;
				matrix[j][3][3] = vBoneMatrices[j].d4;
			}
		}
	} else {
	//	m_logError.logStream() << "(II) No animator found.\n";
	}

	return matrix;
}


void CMesh::Destroy() {
	CEntity::Destroy();
}

uint64_t CMesh::getAIID() {
	return m_aiID;
}

void CMesh::bindToPhysics( float mass ) {
	CEntity::bindToPhysics(2.1f);	// FIXME: All meshes currently same mass
}

#include <cassert>
btCollisionShape* CMesh::getCollisionShape() {
	btTriangleMesh* btTM = new btTriangleMesh();
	assert( m_vertexAttributes->m_sizePositions%3 == 0);

	for( unsigned int i = 0; i < m_mesh->mNumFaces; ++i){
		assert( m_mesh->mFaces[i].mNumIndices == 3);
		int i0 = m_mesh->mFaces[i].mIndices[0];
		int i1 = m_mesh->mFaces[i].mIndices[1];
		int i2 = m_mesh->mFaces[i].mIndices[2];
		aiVector3D& a0 = m_mesh->mVertices[i0];
		aiVector3D& a1 = m_mesh->mVertices[i1];
		aiVector3D& a2 = m_mesh->mVertices[i2];
		btVector3 v1(a0[0], a0[1], a0[2]);
		btVector3 v2(a1[0], a1[1], a1[2]);
		btVector3 v3(a2[0], a2[1], a2[2]);
		btTM->addTriangle(v1,v2,v3);
	}

		btGImpactMeshShape* impms = new btGImpactMeshShape(btTM);
		impms->updateBound();

		btGImpactCollisionAlgorithm::registerAlgorithm(m_pParentScene->PhysicsDispatcher().get());
	
	return impms;
}
