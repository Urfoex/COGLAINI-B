#include "clight.h"

CLight::CLight( glm::vec3 vPosition, glm::vec4 vAmbientColor, glm::vec4 vDiffuseColor ) {
	SetPosition( vPosition );
	SetAmbientColor( vAmbientColor );
	SetDiffuseColor( vDiffuseColor );
}


CLight::~CLight() {
}


void CLight::SetPosition( glm::vec3 vPosition ) {
	m_vPosition = vPosition;
}


void CLight::SetAmbientColor( glm::vec4 vAmbientColor ) {
	m_vAmbientColor = vAmbientColor;
}


void CLight::SetDiffuseColor( glm::vec4 vDiffuseColor ) {
	m_vDiffuseColor = vDiffuseColor;
}

