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


#include "ccamera.h"
#include "../libraries/glm/gtc/matrix_transform.hpp"
#include "../libraries/glm/gtx/rotate_vector.hpp"
#include "headers_with_gcc_warnings/aiCamera.h"
#include "headers_with_gcc_warnings/aiScene.h"
#include "aiToGLM.cpp"
#include "headers_with_gcc_warnings/assimp.h"
#include "cscene.h"

CCamera::CCamera( CScene* parentScene, aiCamera* camera ):
	CEntity( parentScene, camera->mName.data ),
	m_viewMatrix( 1.0f ),
	m_projectionMatrix( 1.0f ),
	m_projectionViewMatrix( 1.0f ),
	m_eyeGlobal( glm::vec3( camera->mPosition.x, camera->mPosition.y, camera->mPosition.z ) ),
	m_centerGlobal( glm::vec3( camera->mLookAt.x, camera->mLookAt.y, camera->mLookAt.z ) ),
	m_upGlobal( glm::vec3( camera->mUp.x, camera->mUp.y, camera->mUp.z ) ),
	m_xGlob( 0 ),
	m_yGlob( 0 ),
	m_zGlob( 0 ),
	m_camera( camera ),
	m_currentMode( FLY ),
	m_pitchAngleFPSMode( 0.0f ),
	m_translate( nullptr ),
	m_pitch( nullptr ),
	m_yaw( nullptr ),
	m_roll( nullptr ),
	m_updateAxis( nullptr ),
	m_lastPos(0,0,0),
	m_ghost(true){
		m_vecPosition=m_eyeGlobal;
}

CCamera::CCamera( CScene* parentScene ):
	CEntity( parentScene, "Standard" ),
	m_viewMatrix( 1.0f ),
	m_projectionMatrix( 1.0f ),
	m_projectionViewMatrix( 1.0f ),
	m_eyeGlobal( glm::vec3( 0.0f, 0.0f, 2.0f ) ),
	m_centerGlobal( glm::vec3( 0.0f, 0.0f, 0.0f ) ),
	m_upGlobal( glm::vec3( 0.0f, 1.0f, 0.0f ) ),
	m_xGlob( 0 ),
	m_yGlob( 0 ),
	m_zGlob( 0 ),
	m_camera( nullptr ),
	m_currentMode( FLY ),
	m_pitchAngleFPSMode( 0.0f ),
	m_translate( nullptr ),
	m_pitch( nullptr ),
	m_yaw( nullptr ),
	m_roll( nullptr ),
	m_updateAxis( nullptr ),
	m_lastPos(m_eyeGlobal[0], m_eyeGlobal[1], m_eyeGlobal[2] ),
	m_ghost(true) {
	m_projectionMatrix = glm::perspective( 90.0f, 4.0f / 3.0f, 0.1f, 10000.0f );

	switchToFLYMode();

	updateGlobals();
	updateAxis();

	updateViewMatrix();
	m_vecPosition=m_eyeGlobal;
}

CCamera::~CCamera() {

}

void CCamera::translate( glm::vec3& translateVector ) {
	m_translate( this, translateVector );
}

void CCamera::translate( glm::vec3 && translateVector ) {
	glm::vec3 tV = translateVector;
	m_translate( this, tV );
}

void CCamera::updateViewMatrix() {
	m_viewMatrix = glm::lookAt( m_eyeGlobal, m_centerGlobal, m_upGlobal );
	m_projectionViewMatrix = m_projectionMatrix * m_viewMatrix;
}

const glm::core::type::mat4 CCamera::getViewMatrix() const {
	return m_viewMatrix;
}

aiCamera* CCamera::getCamera() {
	return m_camera;
}

const glm::core::type::mat4& CCamera::getProjectionMatrix() {
	return m_projectionMatrix;
}

const glm::core::type::mat4& CCamera::getProjectionViewMatrix() {
	return m_projectionViewMatrix;
}

void CCamera::Initialize( aiNode* root ) {
	CEntity::Initialize();

	if( root == nullptr ) {
		bindToPhysics();
		return;
	}

	// Create Projection Matrix
	m_projectionMatrix = glm::perspective( glm::degrees( m_camera->mHorizontalFOV ), m_camera->mAspect, m_camera->mClipPlaneNear, m_camera->mClipPlaneFar );


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

	aiNode* myNode = root->FindNode( m_camera->mName );
	aiMatrix4x4 m4;

	while( myNode != nullptr ) {
		aiMatrix4x4 t = myNode->mTransformation;
// 		aiTransposeMatrix4(&t);
		m4 = t * m4;
		myNode = myNode->mParent;
	}

	aiMatrix4x4 aim;
	m_camera->GetCameraMatrix( aim );
// 	aiTransposeMatrix4(&aim);
	m4 = aim * m4;

	m_modelMatrix = aiToGLMM4( m4 );

	switchToFLYMode();

	updateGlobals();
	updateAxis();

	updateViewMatrix();

	bindToPhysics();
}

void CCamera::updateGlobals() {
	glm::vec4 eG = m_modelMatrix * glm::vec4( m_eyeGlobal, 0.0f );
	m_eyeGlobal = glm::vec3( eG.x, eG.y, eG.z );
	m_vecPosition=m_eyeGlobal;
	
	glm::vec4 cG = m_modelMatrix * glm::vec4( m_centerGlobal, 0.0f );
	m_centerGlobal = glm::vec3( cG.x, cG.y, cG.z );

	glm::vec4 uG = m_modelMatrix * glm::vec4( m_upGlobal, 0.0f );
	m_upGlobal = glm::vec3( uG.x, uG.y, uG.z );
}

void CCamera::updateAxis() {
	m_updateAxis( this );
}

// TODO: use quaternions for rotation (but works already fine)
void CCamera::pitchYawRoll( float pitchAngle, float yawAngle, float rollAngle ) {
	pitch( pitchAngle );
	yaw( yawAngle );
	roll( rollAngle );
}

void CCamera::pitchYawRoll( glm::vec3& pyaAngles ) {
	pitch( pyaAngles.x );
	yaw( pyaAngles.y );
	roll( pyaAngles.z );
}

void CCamera::pitchYawRoll( glm::vec3 && pyaAngles ) {
	pitch( pyaAngles.x );
	yaw( pyaAngles.y );
	roll( pyaAngles.z );
}

void CCamera::roll( float angle ) { // local z
	m_roll( this, angle );
}

void CCamera::pitch( float angle ) { // local X
	m_pitch( this, angle );
}

void CCamera::yaw( float angle ) { // local Y (up)
	m_yaw( this, angle );
}

void CCamera::translateFLYMode( glm::vec3& translateVector ) {
	glm::vec3 l_z = translateVector.z * glm::normalize( m_zGlob );
	glm::vec3 l_y = translateVector.y * glm::normalize( m_yGlob );
	glm::vec3 l_x = translateVector.x * glm::normalize( m_xGlob );

	glm::vec3 t0 = l_z + l_y + l_x;

	translatePhysics( t0 );

	updateViewMatrix();
	updateAxis();
}

void CCamera::translateFPSMode( glm::vec3& translateVector ) {
	glm::vec3 l_z = translateVector.z * glm::normalize( glm::vec3( m_zGlob.x, 0.0f, m_zGlob.z ) );
	glm::vec3 l_y = translateVector.y * glm::normalize( m_yGlob );
	glm::vec3 l_x = translateVector.x * glm::normalize( m_xGlob );

	glm::vec3 t0 = l_z + l_y + l_x;

	translatePhysics( t0 );

	updateViewMatrix();
	updateAxis();
}

void CCamera::pitchFLYMode( float angle ) {
	glm::vec3 diffCenter = glm::rotate( m_zGlob, -angle, m_xGlob ) - m_zGlob;
	glm::vec3 diffUp = glm::rotate( m_yGlob, -angle, m_xGlob ) - m_yGlob;
	m_centerGlobal += diffCenter;
	m_upGlobal += diffUp;
	updateViewMatrix();
	updateAxis();
}

void CCamera::pitchFPSMode( float angle ) {
	if( m_pitchAngleFPSMode + angle <  179.0f && m_pitchAngleFPSMode + angle > 1.0f ) {
		m_pitchAngleFPSMode += angle;
		glm::vec3 diffCenter = glm::rotate( m_centerGlobal - m_eyeGlobal, -angle, m_xGlob ) - ( m_centerGlobal - m_eyeGlobal );
		m_centerGlobal += diffCenter;
		updateViewMatrix();
		updateAxis();
	}
}

void CCamera::rollFLYMode( float angle ) {
	glm::vec3 diffRotZ = glm::rotate( m_yGlob, angle, m_zGlob ) - m_yGlob;
	m_upGlobal += diffRotZ;
	updateViewMatrix();
	updateAxis();
}

void CCamera::rollFPSMode( float ) {
	// no rolling in FPS mode
}

void CCamera::yawFLYMode( float angle ) {
	glm::vec3 diffLR = glm::rotate( m_zGlob, -angle, m_yGlob ) - m_zGlob;
	m_centerGlobal += diffLR;
	updateViewMatrix();
	updateAxis();
}

void CCamera::yawFPSMode( float angle ) {
	m_centerGlobal += glm::rotate( m_zGlob, -angle, m_yGlob ) - m_zGlob;
	updateViewMatrix();
	updateAxis();
}

void CCamera::updateAxisFLYMode() {
	m_yGlob = m_upGlobal;
	m_zGlob = m_centerGlobal - m_eyeGlobal;
	m_xGlob = glm::cross( m_yGlob, m_zGlob );
}

void CCamera::updateAxisFPSMode() {
	m_yGlob = glm::vec3( 0.0f, std::abs( m_upGlobal.y ), 0.0f );
	m_zGlob = m_centerGlobal - m_eyeGlobal;
	m_xGlob = glm::cross( m_yGlob, m_zGlob );
}

void CCamera::toggleCameraMode() {
	switch( m_currentMode ) {
	case FLY:
		m_logError.logStream() << "Changing mode to: FPS\n";
		switchToFPSMode();
		break;
	case FPS:
		m_logError.logStream() << "Changing mode to: FLY\n";
		switchToFLYMode();
		break;
	default:
		break;
	};
}

void CCamera::switchToFLYMode() {
	m_currentMode = FLY;
	m_translate = &CCamera::translateFLYMode;
	m_pitch = &CCamera::pitchFLYMode;
	m_yaw = &CCamera::yawFLYMode;
	m_roll = &CCamera::rollFLYMode;
	m_updateAxis = &CCamera::updateAxisFLYMode;

	m_updateAxis( this );
}

void CCamera::switchToFPSMode() {
	m_currentMode = FPS;
	m_translate = &CCamera::translateFPSMode;
	m_pitch = &CCamera::pitchFPSMode;
	m_yaw = &CCamera::yawFPSMode;
	m_roll = &CCamera::rollFPSMode;
	m_updateAxis = &CCamera::updateAxisFPSMode;

	m_yGlob = glm::vec3( 0.0f, std::abs( m_yGlob.y ), 0.0f );
	m_upGlobal = glm::vec3( 0.0f, std::abs( m_upGlobal.y ), 0.0f );


	m_updateAxis( this );

	glm::vec3 lookAt = m_centerGlobal - m_eyeGlobal;
	m_pitchAngleFPSMode = 180 - glm::degrees( std::acos( glm::dot( lookAt, m_yGlob ) / ( glm::length( lookAt ) * glm::length( m_yGlob ) ) ) );
}

void CCamera::Update( float ) {
// CEntity::Update(deltaTime);

	const btVector3& pos =  m_rigidBody->getCenterOfMassPosition();
	btVector3 posDiff = pos - m_lastPos;
	if( !( posDiff[0] == 0.0f && posDiff[1] == 0.0f && posDiff[2] == 0.0f )){
		glm::vec3 t0(posDiff[0],posDiff[1],posDiff[2]);
		m_eyeGlobal += t0;
		m_centerGlobal += t0;
		
		m_vecPosition=m_eyeGlobal;

		updateViewMatrix();
		updateAxis();
		
		m_lastPos = pos;
	}
}

void CCamera::Draw( float ) {
// CEntity::Draw(deltaTime);
}

void CCamera::Destroy() {
	CEntity::Destroy();
}

void CCamera::bindToPhysics( float mass ) {
// 	CEntity::bindToPhysics();
	glm::vec4 pos = m_modelMatrix * glm::vec4( 0 );
	btVector3 transvec;

	for( unsigned int i = 0; i < 3; ++i ) {
		transvec[i] = pos[i];
	}

	btMatrix3x3 mat;
	mat.setIdentity();;
	btTransform trans( mat, transvec );
	m_rigidBody = m_pParentScene->addRigidBody( getCollisionShape(), trans, 1.0f );
	m_rigidBody->setGravity(btVector3(0,0,0));
	m_rigidBody->setDamping( 0.99f, 1.0f);
	m_stdCollisionFlags = m_rigidBody->getCollisionFlags();
	m_rigidBody->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
}

void CCamera::toggleGhost() {
	if( m_ghost){
		m_rigidBody->setCollisionFlags(m_stdCollisionFlags);
		m_logError.logStream() << "(II) Ghost mode off\n";
	}else{
		m_rigidBody->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
		m_logError.logStream() << "(II) Ghost mode on\n";
	}
	m_ghost = !m_ghost;
}

void CCamera::translatePhysics( glm::vec3& translation ) {
	m_rigidBody->activate(true);
	m_rigidBody->applyCentralImpulse(btVector3( translation[0], translation[1], translation[2] )*10);
}

btCollisionShape* CCamera::getCollisionShape() {
	float size = 1.0f;
	return new btBoxShape( btVector3( size, size, size ) );
}

glm::vec3 CCamera::getLookDirection() {
	return m_centerGlobal - m_eyeGlobal;
}
