#include "centity.h"
#include "cmesh.h"
#include "centitymanager.h"
#include "cscene.h"
#include "headers_with_gcc_warnings/aiScene.h"
#include "aiToGLM.cpp"
#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>
#include "headers_with_gcc_warnings/assimp.h"

uint64_t CEntity::m_nextFreeID = 0;

CEntity::CEntity( CScene* parentScene, std::string pcEntityName ):
	m_vecPosition(),
	m_vecRotation(),
	m_bIsVisible( true ),
	m_bIsAlive( true ),
	m_pParentScene( parentScene ),
	m_pContentNode( nullptr ),
	m_iID( m_nextFreeID++ ),
	m_pcName( pcEntityName ),
	m_NameByNode( "" ),
	m_modelMatrix( 1 ),
	m_originalModelMatrix( 1 ),
	m_logError(),
	m_rigidBody( nullptr ),
	m_lastPosition(){
}

CEntity::~CEntity() {
// 	m_logError.logStream() << "Removing: " << m_iID << " : " << m_NameByNode << " ... " << m_pcName << std::endl;
	this->m_pParentScene = nullptr;
}

CEntity::CEntity( const CEntity& entity ) {
	throw( std::runtime_error( "Not done yet." ) );	// TODO link attributes to other entity
}

glm::vec3 CEntity::GetPosition() {
	return m_vecPosition;
}
glm::vec3 CEntity::GetRotation() {
	return m_vecRotation;
}
void CEntity::SetVisible( bool isVisible ) {
	m_bIsVisible = isVisible;
}
bool CEntity::IsVisible() {
	return m_bIsVisible;
}
bool CEntity::IsAlive() {
	return m_bIsAlive;
}
int64_t CEntity::GetID() {
	return m_iID;
}
const std::string CEntity::GetName() {
	return m_pcName;
}

void CEntity::Initialize() {
// 	m_logError.logStream() << "Registering entity...\n";
	// Do some additional init stuff
	this->m_pParentScene->GetEntityManager().RegisterEntity( this );

	// setting m_modelMatrix ...
//    createModelMatrix(node);

}

void CEntity::Initialize( aiNode* ) {
// 	m_logError.logStream() << "Registering entity...\n";
	// Do some additional init stuff
	this->m_pParentScene->GetEntityManager().RegisterEntity( this );

	// setting m_modelMatrix ...
	//    createModelMatrix(node);
}

void CEntity::createModelMatrix( aiNode* node ) {
	if( m_NameByNode.empty() ) {
		m_NameByNode = node->mName.data;
	}

	m_pContentNode = node;

	// Create Model Matrix

	// 	// Get the camera matrix for a camera at a specific time
	// 	// if the node hierarchy for the camera does not contain
	// 	// at least one animated node this is a static computation
	// 	get-camera-matrix (node sceneRoot, camera cam) : matrix
	// 	{
	// 		node   cnd = find-node-for-camera(cam)
	// 		matrix cmt = identity()
	//
	// 		// as usual - get the absolute camera transformation for this frame
	// 		for each node nd in hierarchy from sceneRoot to cnd
	// 			matrix cur
	// 			if (is-animated(nd))
	// 				cur = eval-animation(nd)
	// 			else cur = nd->mTransformation;
	// 				cmt = mult-matrices( cmt, cur )
	// 		end for
	//
	// 		// now multiply with the camera's own local transform
	// 		cam = mult-matrices (cam, get-camera-matrix(cmt) )
	// 	}

// 		aiNode* myNode = node->FindNode( m_pcName.c_str());
// 		if( myNode == nullptr){
// 			m_modelMatrix = glm::mat4(1);
// 			throw( std::runtime_error("Why is there no Node for this entity: " + m_pcName));
// 			return;
// 		}

	aiNode* myNode = node;
	aiMatrix4x4 m4;

	while( myNode != nullptr ) {
		aiMatrix4x4 toTranspose = myNode->mTransformation;
		aiTransposeMatrix4( &toTranspose );
		m4 = toTranspose * m4;
		myNode = myNode->mParent;
	}

// 		aiMatrix4x4 aim;
// 		m_camera->GetCameraMatrix(aim);
// 		m4 = aim * (myNode->mTransformation * m4);

	m_modelMatrix = aiToGLMM4( m4 );
	m_originalModelMatrix = m_modelMatrix;

	bindToPhysics();
}

void CEntity::bindToPhysics( float mass) {
	btVector3 transvec;
	for( int i = 0; i < 3; ++i){
		transvec[i] = m_modelMatrix[3][i];
	}
// 	m_logError.logStream() << "(II) PhyCenter: " << transvec[0] << " \t" << transvec[1] << " \t" << transvec[2] << std::endl;
	m_lastPosition = transvec;
	
	btMatrix3x3 mat;
	mat.setIdentity();;
	btTransform trans(  mat, transvec);
	m_rigidBody = m_pParentScene->addRigidBody(getCollisionShape(), trans, /*0.0f**/mass);
}

void CEntity::applyAdditionalMatrix( aiMatrix4x4& transform ) {
	throw( std::runtime_error( "void CEntity::applyAdditionalMatrix(aiMatrix4x4& transform)" ) );
	aiMatrix4x4 t = transform;
	aiTransposeMatrix4( &t );
	m_modelMatrix = aiToGLMM4( t ) * m_modelMatrix;
}

void CEntity::Destroy() {
// 	m_logError.logStream() << "Unregistering entity...\n";
	// Deinit all addional stuff
	this->m_pParentScene->GetEntityManager().UnRegisterEntity( this );
}

void CEntity::SetEmpty() {
// 	m_logError.logStream() << "Getting empty...\n";
}

void CEntity::Update( float ) {
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
}


void CEntity::Draw( float ) {
	if( !m_bIsVisible )
		return;
}


void CEntity::Translate( glm::core::type::vec3& deltaMove ) {
	m_vecPosition += deltaMove;
}


void CEntity::Translate( float posX, float posY, float posZ ) {
	m_vecPosition += glm::vec3( posX, posY, posZ );
}


void CEntity::Rotate( glm::core::type::vec3& deltaRot ) {
	m_vecRotation += deltaRot;
}


void CEntity::Rotate( float rotX, float rotY, float rotZ ) {
	m_vecRotation += glm::vec3( rotX, rotY, rotZ );
}


void CEntity::SetPosition( glm::core::type::vec3& newPos ) {
	m_vecPosition = newPos;
}


void CEntity::SetRotation( glm::core::type::vec3& newRot ) {
	m_vecRotation = newRot;
}

btCollisionShape* CEntity::getCollisionShape() {
	return new btBoxShape( btVector3(1.0f, 1.0f, 1.0f) );
}
