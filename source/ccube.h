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


#ifndef CCUBE_H
#define CCUBE_H

#include "cmesh.h"


class CCube : public CMesh {
public:
	CCube( CScene* parentScene, glm::vec3 startPos, glm::vec3 impulse, float size = 1.0f, float mass = 1.0f );
	CCube( CScene* parentScene, glm::vec3 startPos, glm::vec3 impulse, float sizeX, float sizeY, float sizeZ , float mass = 1.0f);
	virtual void Initialize(aiNode*){}
	virtual void Initialize(aiMaterial**){}
	virtual void Initialize();
	virtual void createModelMatrix( aiNode* node ) {} 
	virtual void Update( float deltaTime );
protected:
	virtual void createVerticeAttributes();
	virtual std::unique_ptr< GLuint[] > createVertixIndex();
	virtual btCollisionShape* getCollisionShape();
	virtual void createShader();
	
	float m_sizeX;
	float m_sizeY;
	float m_sizeZ;
	float m_mass;
	glm::vec3 m_initialImpulse;
};

#endif // CCUBE_H
