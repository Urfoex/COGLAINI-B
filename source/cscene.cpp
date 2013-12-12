#include "cscene.h"
#include "centitymanager.h"
#include "headers_with_gcc_warnings/assimp.h"
#include <aiPostProcess.h>
#include "headers_with_gcc_warnings/aiScene.h"
#include <stdexcept>
#include "centity.h"
#include "cmesh.h"
#include "centitylight.h"
#include "SFML_OpenGL.h"
#include "canimatedmesh.h"
#include "draw.h"
#include "headers_with_gcc_warnings/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "GLDebugDrawer.h"
#include "ccube.h"


CScene::CScene( uint64_t ID, std::string& filename ) :
	m_logError(),
	m_pMainCameraPos(),
	m_pMainCameraDir(),
	m_EntityManager(),
	m_ContentScene( nullptr ),
	m_Name( filename ),
	m_ID( ID ),
	m_cameraByID(),
	m_cameraByName(),
	m_currentCameraIterator(),
	m_meshByID(),
	m_meshByAIID(),
	m_meshByName(),
	m_lighthByID(),
	m_lightByName(),
	m_uniformLightsArray( nullptr ),
	m_sizeOfUniformLightsArray( 0 ),
	m_polygonMode( GL_FILL ),
	m_lightUBOBuffer( 0 ),
	m_lightUniformBlockBinding( 0 ),
	m_numOfActiveLights( 0 ),
	m_maxNumOfLights( 8 ),
	m_animator( nullptr ),
	m_physics(new CPhysics()),
	m_physicDebugWireframe( GL_LINE ),
	m_physicDebugDraw( true ),
	m_buffer(),
	m_sound(nullptr),
	m_hasSound(false)
	{
//	m_logError.logStream() << "Loading scene from: " << filename << std::endl;
	m_ContentScene = ( aiScene* )( aiImportFile( filename.c_str(),  aiPostProcessSteps::aiProcess_Triangulate ) );

	if( m_ContentScene == nullptr ) {
		throw( std::runtime_error( aiGetErrorString() ) );
	}
	m_hasSound = m_buffer.LoadFromFile("../resources/Sounds/2329__jovica__stab-004-mastered-16-bit.wav");
	if( m_hasSound){
		m_sound.reset( new sf::Sound(m_buffer));
	}
}

CScene::~CScene( void ) {
//	m_logError.logStream() << "Freeing data of scene from: " << m_Name << std::endl;
// 	m_EntityManager.Destroy();
	aiReleaseImport( m_ContentScene );
}

// #include <aiScene.h>

void CScene::printSceneDebug() {
//	m_logError.logStream() << "Has animations: " << m_ContentScene->HasAnimations() << std::endl;

	if( m_ContentScene->HasAnimations() ) {
	//	m_logError.logStream() << "Num : " << m_ContentScene->mNumAnimations << std::endl;
	}

//	m_logError.logStream() << "Has cameras: " << m_ContentScene->HasCameras() << std::endl;

	if( m_ContentScene->HasCameras() ) {
	//	m_logError.logStream() << "Num : " << m_ContentScene->mNumCameras << std::endl;
	}

//	m_logError.logStream() << "Has lights: " << m_ContentScene->HasLights() << std::endl;

	if( m_ContentScene->HasLights() ) {
	//	m_logError.logStream() << "Num : " << m_ContentScene->mNumLights << std::endl;
	}

//	m_logError.logStream() << "Has materials: " << m_ContentScene->HasMaterials() << std::endl;

	if( m_ContentScene->HasMaterials() ) {
	//	m_logError.logStream() << "Num : " << m_ContentScene->mNumMaterials << std::endl;
	}

//	m_logError.logStream() << "Has meshes: " << m_ContentScene->HasMeshes() << std::endl;

	if( m_ContentScene->HasMeshes() ) {
	//	m_logError.logStream() << "Num : " << m_ContentScene->mNumMeshes << std::endl;
	}

//	m_logError.logStream() << "Has textures: " << m_ContentScene->HasTextures() << std::endl;

	if( m_ContentScene->HasTextures() ) {
	//	m_logError.logStream() << "Num : " << m_ContentScene->mNumTextures << std::endl;
	}
}

void CScene::Initialize() {
	printSceneDebug();
	initOwn();
	m_physics->Initialize();
	
	if( m_ContentScene->HasCameras() ) {
		for( uint64_t i = 0; i < m_ContentScene->mNumCameras; ++i ) {
			CCamera* pCC = new CCamera( this, m_ContentScene->mCameras[i] );
			pCC->Initialize( m_ContentScene->mRootNode );
			m_cameraByID.insert( std::pair<uint64_t, CCamera*>( pCC->GetID(), pCC ) );
			m_cameraByName.insert( std::pair<std::string, CCamera*>( pCC->GetName(), pCC ) );
		}

		m_currentCameraIterator = m_cameraByID.begin();
	} else {
		CCamera* pCC( new CCamera( this ) );
		pCC->Initialize( nullptr );
		m_cameraByID.insert( std::pair<uint64_t, CCamera*>( pCC->GetID(), pCC ) );
		m_cameraByName.insert( std::pair<std::string, CCamera*>( pCC->GetName(), pCC ) );
		m_currentCameraIterator = m_cameraByID.begin();
	}

	if( m_ContentScene->HasMeshes() ) {
		for( uint64_t i = 0; i < m_ContentScene->mNumMeshes; ++i ) {
			auto mesh = m_ContentScene->mMeshes[i];
			CMesh* pCM = nullptr;

			if( mesh->HasBones() ) {
				pCM = new CAnimatedMesh( this, m_ContentScene->mMeshes[i], i );
			} else {
				pCM = new CMesh( this, m_ContentScene->mMeshes[i], i );
			}
			pCM->Initialize( m_ContentScene->mMaterials );
			m_meshByID.insert( std::pair<uint64_t, CMesh*>( pCM->GetID(), pCM ) );
			m_meshByAIID.insert( std::pair<uint64_t, CMesh*>( pCM->getAIID(), pCM ) );
			m_meshByName.insert( std::pair<std::string, CMesh*>( pCM->GetName(), pCM ) );
		}

		recursiveInitMeshes( m_ContentScene->mRootNode );
	}

	if( m_ContentScene->HasLights() ) {
		for( uint64_t i = 0; i < m_ContentScene->mNumLights; ++i ) {
			CEntityLight* pCL = new CEntityLight( this, m_ContentScene->mLights[i] );
			pCL->Initialize( m_ContentScene->mRootNode );
			m_lighthByID.insert( std::pair<uint64_t, CEntityLight*>( pCL->GetID(), pCL ) );
			m_lightByName.insert( std::pair<std::string, CEntityLight*>( pCL->GetName(), pCL ) );
		}

		makeLightsArray();
		putLightsIntoBuffer();
	}

	if( m_ContentScene->HasAnimations() ) {
		m_animator.reset( new CAnimator( this->GetContentScene(), 0 ) );
	}
	
	stackOfCubes();
	{
		CCube* ground = new CCube( this, glm::vec3(0,-50,0), glm::vec3(0,0,0), 500, 10, 500, 0);
		ground->Initialize();
		m_meshByID.insert(std::pair<uint64_t, CMesh*>( (dynamic_cast<CMesh*>( ground ))->GetID(), ground ));
	}

}

void CScene::stackOfCubes() {
	float SCALING = 0.8f;
	btScalar mass( 1.f );
	
	static const unsigned int ARRAY_SIZE_X = 5;
	static const unsigned int ARRAY_SIZE_Y = 5;
	static const unsigned int ARRAY_SIZE_Z = 5;
	static const unsigned int MAX_PROXIES = ARRAY_SIZE_X * ARRAY_SIZE_Y * ARRAY_SIZE_Z + 1024;
	static const signed int STARTPOS_X = -5;
	static const signed int STARTPOS_Y = 25;
	static const signed int STARTPOS_Z = -3;
	
	float start_x = STARTPOS_X - ARRAY_SIZE_X / 2.f;
	float start_y = STARTPOS_Y;
	float start_z = STARTPOS_Z - ARRAY_SIZE_Z / 2.f;
	
	for( int k = 0; k < ARRAY_SIZE_Y; k++ ) {
		for( int i = 0; i < ARRAY_SIZE_X; i++ ) {
			for( int j = 0; j < ARRAY_SIZE_Z; j++ ) {
				CCube* cu = new CCube( this, 
					glm::vec3( btScalar( 2.0 * i + start_x ), btScalar( 20 + 2.0 * k + start_y ), btScalar( 2.0 * j + start_z ) ),
					glm::vec3(0,0,0), SCALING, mass
				);
				cu->Initialize();
				m_meshByID.insert(std::pair<uint64_t, CMesh*>( (dynamic_cast<CMesh*>(cu))->GetID(), cu ));
			}
		}
	}
}

void CScene::initMeshes() {

}

void CScene::recursiveInitMeshes( aiNode* node ) {
//	m_logError.logStream() << "(II) Node name: " << node->mName.data << std::endl;
//	m_logError.logStream() << "(II) Meshes: " << node->mNumMeshes << std::endl;

	for( uint64_t i = 0; i < node->mNumMeshes; ++i ) {
	//	m_logError.logStream() << "(II)  MesheIDs: " << node->mMeshes[i] << std::endl;
		m_meshByAIID[ node->mMeshes[i]]->createModelMatrix( node );
		m_meshByAIID[ node->mMeshes[i]]->setNodeID( i );
	}

	for( uint64_t i = 0; i < node->mNumChildren; ++i ) {
		recursiveInitMeshes( node->mChildren[i] );
	}
}

void CScene::initOwn() {
	glGenBuffers( 1, &m_lightUBOBuffer );
}

void CScene::Destroy() {
	// Destroy all entities
	m_EntityManager.Destroy();
	glDeleteBuffers( 1, &m_lightUBOBuffer );
	delete[] m_uniformLightsArray;
}


void CScene::Update( float deltaTime ) {
	m_EntityManager.UpdateEntites( deltaTime );
	m_physics->stepSimulation( /*1.0f/60.0f,1*/ );
}


void CScene::Draw( float deltaTime ) {
	setGlobalUniforms();
	m_EntityManager.DrawEntities( deltaTime );

	if( m_physicDebugDraw ) {
		glPolygonMode( GL_FRONT_AND_BACK, m_physicDebugWireframe );
		m_physics->renderPhysicsScene( getCurrentCamera()->getProjectionViewMatrix() );
		glPolygonMode( GL_FRONT_AND_BACK, m_polygonMode );
	}

}


// TLightType as additional parameter?
int CScene::AddLight( glm::core::type::vec3& , glm::core::type::vec3& , float ) {
	return 0;
}


void CScene::SetCameraPosition( glm::core::type::vec3& ) {
}

void CScene::SetCameraDirection( glm::core::type::vec3& ) {
}

CCamera* CScene::getCurrentCamera() {
	return m_currentCameraIterator->second;
}

void CScene::setCurrentCameraByID( uint64_t id ) {
	m_currentCameraIterator = m_cameraByID.find( id );
}

void CScene::setCurrentCameraByName( std::string name ) {
	m_currentCameraIterator = m_cameraByID.find( m_cameraByName.at( name )->GetID() );
}

void CScene::setNextCamera() {
	if( ++m_currentCameraIterator == m_cameraByID.end() ) {
		m_currentCameraIterator = m_cameraByID.begin();
	}

//	m_logError.logStream() << "Changing to camera: " << m_currentCameraIterator->first << std::endl;
}

uint64_t CScene::getID() {
	return m_ID;
}

std::string CScene::getName() {
	return m_Name;
}

aiMaterial* CScene::getMaterial( uint64_t id ) {
	return m_ContentScene->mMaterials[id];
}

void CScene::togglePolygonMode() {
	switch( m_polygonMode ) {
	case GL_POINT:
		m_polygonMode = GL_LINE;
	//	m_logError.logStream() << "(II) Drawmode: Lines\n";
		break;
	case GL_LINE:
	//	m_logError.logStream() << "(II) Drawmode: Filled\n";
		m_polygonMode = GL_FILL;
		break;
	case GL_FILL:
	//	m_logError.logStream() << "(II) Drawmode: Points\n";
		m_polygonMode = GL_POINT;
		break;
	default:
		throw( std::runtime_error( "Out of scope!" ) );
	}

	glPolygonMode( GL_FRONT_AND_BACK, m_polygonMode );
	checkGLErrorLF( m_logError.errorStream() );
}

void CScene::makeLightsArray() {
	m_lightUniformBlockBinding = CShaderProgram::getNextFreeUniformBlockBinding();

	glGenBuffers( 1, &m_lightUBOBuffer );
	checkGLErrorLF( m_logError.errorStream() );

	// Alternative
	glBindBuffer( GL_UNIFORM_BUFFER, m_lightUBOBuffer );
	checkGLErrorLF( m_logError.errorStream() );

	glBufferData( GL_UNIFORM_BUFFER, ( SUniformLight::GetSize() )*m_maxNumOfLights, nullptr, GL_STREAM_DRAW );
	checkGLErrorLF( m_logError.errorStream() );

	glBindBuffer( GL_UNIFORM_BUFFER, 0 );
	checkGLErrorLF( m_logError.errorStream() );

	m_numOfActiveLights = m_lightByName.size();
}

void CScene::putLightIntoBuffer( SUniformLight* sul, unsigned int num ) {
	if( num >= m_maxNumOfLights ) {
		return;
	}

	// FIXME: Get full offset somewhere ...
	unsigned int offset = num * 5 * sizeof( glm::vec4 ); //SUniformLight::GetSize();
	glBufferSubData( GL_UNIFORM_BUFFER, offset , sizeof( glm::vec4 ), glm::value_ptr( sul->m_position ) );
	checkGLErrorLF( m_logError.errorStream() );

	glBufferSubData( GL_UNIFORM_BUFFER, offset + sizeof( glm::vec4 ), sizeof( glm::vec4 ), glm::value_ptr( sul->m_ambient ) );
	checkGLErrorLF( m_logError.errorStream() );

	glBufferSubData( GL_UNIFORM_BUFFER, offset + sizeof( glm::vec4 ) * 2, sizeof( glm::vec4 ), glm::value_ptr( sul->m_diffuse ) );
	checkGLErrorLF( m_logError.errorStream() );

	glBufferSubData( GL_UNIFORM_BUFFER, offset + sizeof( glm::vec4 ) * 3, sizeof( glm::vec4 ), glm::value_ptr( sul->m_specular ) );
	checkGLErrorLF( m_logError.errorStream() );

	glBufferSubData( GL_UNIFORM_BUFFER, offset + sizeof( glm::vec4 ) * 4, sizeof( glm::vec3 ), glm::value_ptr( sul->m_attenuation ) );
	checkGLErrorLF( m_logError.errorStream() );
}

void CScene::putLightsIntoBuffer() {
	glBindBuffer( GL_UNIFORM_BUFFER, m_lightUBOBuffer );
	checkGLErrorLF( m_logError.errorStream() );
	{
		auto me = this;
		unsigned int count = 0;
		std::for_each( m_lightByName.begin(), m_lightByName.end(), [&me, &count]( std::pair<std::string, CEntityLight*> psc ) {
			me->putLightIntoBuffer( psc.second->GetUniformLightP(), count++ );
		} );
	}
	glBindBuffer( GL_UNIFORM_BUFFER, 0 );
	checkGLErrorLF( m_logError.errorStream() );
}

void CScene::setGlobalUniforms() {
	auto& allShaderPrograms = CShaderProgram::getAllShaderPrograms();
	auto me = this;
	std::for_each( allShaderPrograms.begin(), allShaderPrograms.end(), [&me]( std::pair<std::pair<CVertexShader*, CFragmentShader*>, std::shared_ptr<CShaderProgram>> p ) {
		me->setGlobalUniforms( p.second->getShaderProgram() );
	} );
}

void CScene::setGlobalUniforms( GLuint shaderprogram ) {
	glUseProgram( shaderprogram );
	checkGLErrorLF( m_logError.errorStream() );

	//////////////////////////////////
	// setting projection view matrix
	{
		GLint projectionViewMatrixLocation = glGetUniformLocation( shaderprogram, "projectionViewMatrix" );
		checkGLErrorLF( m_logError.errorStream() );

		glUniformMatrix4fv( projectionViewMatrixLocation, 1, GL_FALSE, glm::value_ptr( m_currentCameraIterator->second->getProjectionViewMatrix() ) );
		checkGLErrorLF( m_logError.errorStream() );
	}
	////////////////////////////
	// setting number of lights
	{
		GLint numberOfLightsLocation = glGetUniformLocation( shaderprogram, "numberOfLights" );
		checkGLErrorLF( m_logError.errorStream() );

		GLuint numOfLights = std::min( ( GLuint )8, m_numOfActiveLights );
		glUniform1ui( numberOfLightsLocation, /*0);//*/numOfLights );
		checkGLErrorLF( m_logError.errorStream() );
	}

	if( m_numOfActiveLights > 0 ) {
		putLightsIntoBuffer( shaderprogram );
	}
}

void CScene::putLightsIntoBuffer( GLint program ) {
	GLuint lightsUniformBlockBufferIndex = glGetUniformBlockIndex( program, "Lights" );
	checkGLErrorLF( m_logError.errorStream() );

	if( lightsUniformBlockBufferIndex != -1 ) {
		glUniformBlockBinding( program, lightsUniformBlockBufferIndex, m_lightUniformBlockBinding );
		checkGLErrorLF( m_logError.errorStream() );

		glBindBufferRange( GL_UNIFORM_BUFFER, m_lightUniformBlockBinding, m_lightUBOBuffer, 0, m_maxNumOfLights * SUniformLight::GetSize() );
		checkGLErrorLF( m_logError.errorStream() );
	}
}

btRigidBody* CScene::addRigidBody( btCollisionShape* shape, const btTransform& startTransform, btScalar mass ) {
	return m_physics->addRigidBody( shape, startTransform, mass );
}

void CScene::togglePhysicDebugDraw() {
	m_physicDebugDraw = ! m_physicDebugDraw;
//	m_logError.logStream() << "(II) Physics debug draw: " << m_physicDebugDraw << "\n";
}

void CScene::togglePhysicDebugWireframe() {
	if( m_physicDebugWireframe == GL_FILL ) {
		m_physicDebugWireframe = GL_LINE;
	//	m_logError.logStream() << "(II) Physics debug lines\n";
	} else {
		m_physicDebugWireframe = GL_FILL;
	//	m_logError.logStream() << "(II) Physics debug filled\n";
	}
}

void CScene::shoot() 	{
	if( m_hasSound){
		m_sound->Play();
	}
	glm::vec3 pos = getCurrentCamera()->GetPosition();
	glm::vec3 dir = getCurrentCamera()->getLookDirection();
	dir = glm::normalize( dir);
	static const float FORCE = 100.0f;
	dir = dir*FORCE;
	CCube* cu = new CCube( this, pos, dir, 0.1f);
	cu->Initialize();
	m_meshByID.insert(std::pair<uint64_t, CMesh*>( (dynamic_cast<CMesh*>(cu))->GetID(), cu ));
}
