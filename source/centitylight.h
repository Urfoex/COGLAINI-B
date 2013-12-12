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


#ifndef CENTITYLIGHT_H
#define CENTITYLIGHT_H

#include "centity.h"
#include "headers_with_gcc_warnings/glm.hpp"

struct aiVector3D;
struct aiLight;
struct aiColor3D;

/// Currently only point light
///
struct SUniformLight {
	glm::vec4 m_ambient;
	glm::vec4 m_diffuse;
	glm::vec4 m_specular;
	glm::vec4 m_position;
	glm::vec3 m_attenuation;

	SUniformLight( glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec4 position );
	SUniformLight( aiColor3D& ambient, aiColor3D& diffuse, aiColor3D& specular, aiVector3D& position, float att0, float att1, float att2 );

	static uint64_t GetSize();
};

class CEntityLight : public CEntity {

public:
	CEntityLight( CScene* parentScene, aiLight* light );
	CEntityLight( const CEntity& );
	CEntityLight( const CEntityLight& );
	CEntityLight& operator=( const CEntityLight& );
	virtual ~CEntityLight();
	virtual void Initialize( aiNode* node );
	virtual void Destroy();
	virtual void Update( float deltaTime );
	virtual void Draw( float deltaTime );

	SUniformLight& GetUniformLight();
	SUniformLight* GetUniformLightP();
private:
	aiLight* m_Light;
	SUniformLight m_uniformLight;
	
public:	// physics
	virtual void bindToPhysics( float mass = 0.0f ){};
};

#endif // CENTITYLIGHT_H
