#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <glm/glm.hpp>

class CLight {
public:
	CLight( glm::vec3 vPosition, glm::vec4 vAmbientColor, glm::vec4 vDiffuseColor );
	~CLight();

	void SetPosition( glm::vec3 vPosition );
	glm::vec3 GetPosition() {
		return m_vPosition;
	}

	void SetAmbientColor( glm::vec4 vAmbientColor );
	glm::vec4 GetAmbientColor() {
		return m_vAmbientColor;
	}

	void SetDiffuseColor( glm::vec4 vDiffuseColor );
	glm::vec4 GetDiffuseColor() {
		return m_vDiffuseColor;
	}

private:
	glm::vec3 m_vPosition;
	glm::vec4 m_vAmbientColor;
	glm::vec4 m_vDiffuseColor;
};

#endif
