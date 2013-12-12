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


#ifndef CCAMERA_H
#define CCAMERA_H

#include "headers_with_gcc_warnings/glm.hpp"
#include "../libraries/glm/gtc/type_ptr.hpp"
#include <string>
#include <cstdint>
#include "clogerrorstream.h"
#include <functional>
#include "centity.h"

class CScene;
class aiNode;
struct aiCamera;

class CCamera : public CEntity {
public:
	enum Mode { FLY = 0, FPS};
public:
	CCamera( CScene* parentScene );
	CCamera( const CCamera& );
	void operator=( const CCamera& );
	CCamera( CScene* parentScene, aiCamera* camera );
	virtual ~CCamera();
	void translate( glm::vec3& translateVector );
	void translate( glm::vec3 && translateVector );
	const glm::core::type::mat4 getViewMatrix() const;

	/// Get aiCamera to set near-far, FOV, aspect, ...
	aiCamera* getCamera();

	const glm::core::type::mat4& getProjectionMatrix();
	const glm::mat4& getProjectionViewMatrix();
	void pitch( float angle );
	void roll( float angle );
	void yaw( float angle );
	void pitchYawRoll( float pitchAngle, float yawAngle, float rollAngle );
	void pitchYawRoll( glm::vec3& pyaAngles );
	void pitchYawRoll( glm::vec3 && pyaAngles );

	void toggleCameraMode();

	virtual void Initialize( aiNode* root );
	virtual void Update( float deltaTime );
	virtual void Draw( float deltaTime );
	virtual void Destroy();

	glm::vec3 getLookDirection();

private:
	// The view matrix, which can also be known as the world matrix determines the position of the ‘camera’ in space.
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_projectionViewMatrix;
	glm::vec3 m_eyeGlobal;		// position of camera
	glm::vec3 m_centerGlobal;	// where camera is looking to
	glm::vec3 m_upGlobal;

	glm::vec3 m_xGlob;
	glm::vec3 m_yGlob;
	glm::vec3 m_zGlob;

	aiCamera* m_camera;

	Mode m_currentMode;

	float m_pitchAngleFPSMode;

	std::function<void( CCamera*, glm::vec3& )> m_translate;
	std::function<void( CCamera*, float )> m_pitch;
	std::function<void( CCamera*, float )> m_yaw;
	std::function<void( CCamera*, float )> m_roll;
	std::function<void( CCamera* )> m_updateAxis;

	void updateViewMatrix();
	void updateGlobals();
	void updateAxis();

	void switchToFLYMode();
	void switchToFPSMode();

	void translateFLYMode( glm::vec3& translateVector );
	void translateFPSMode( glm::vec3& translateVector );

	void pitchFLYMode( float angle );
	void pitchFPSMode( float angle );

	void yawFLYMode( float angle );
	void yawFPSMode( float angle );

	void rollFLYMode( float angle );
	void rollFPSMode( float angle );

	void updateAxisFLYMode();
	void updateAxisFPSMode();

public:	// physics
	virtual void bindToPhysics( float mass = 0.0f);
	void toggleGhost();
	
private:
	void translatePhysics(glm::vec3& translation);
	virtual btCollisionShape* getCollisionShape();
	btVector3 m_lastPos;
	bool m_ghost;
	int m_stdCollisionFlags;
};

#endif // CCAMERA_H
