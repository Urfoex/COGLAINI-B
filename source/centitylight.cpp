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


#include "centitylight.h"
#include "headers_with_gcc_warnings/aiScene.h"

SUniformLight::SUniformLight( glm::core::type::vec4 ambient, glm::core::type::vec4 diffuse, glm::core::type::vec4 specular, glm::core::type::vec4 position ):
	m_ambient( ambient ),
	m_diffuse( diffuse ),
	m_specular( specular ),
	m_position( position ),
	m_attenuation( 0 ) {

}

SUniformLight::SUniformLight( aiColor3D& ambient, aiColor3D& diffuse, aiColor3D& specular, aiVector3D& position, float att0, float att1, float att2 ):
	m_ambient( ambient.r, ambient.g, ambient.b, 1 ),
	m_diffuse( diffuse.r, diffuse.g, diffuse.b, 1 ),
	m_specular( specular.r, specular.g, specular.b, 1 ),
	m_position( position.x, position.y, position.z, 1 ),
	m_attenuation( att0, att1, att2 ) {

}

uint64_t SUniformLight::GetSize() {
	return sizeof( glm::vec4 ) * 4 + sizeof( glm::vec3 ) ;
}

CEntityLight::CEntityLight( CScene* parentScene, aiLight* light ):
	CEntity( parentScene, light->mName.data ),
	m_Light( light ),
	m_uniformLight( light->mColorAmbient, light->mColorDiffuse, light->mColorSpecular, light->mPosition, light->mAttenuationConstant, light->mAttenuationLinear, light->mAttenuationQuadratic ) {
}

// CEntityLight::CEntityLight( const CEntity& ) {
//
// }

CEntityLight::~CEntityLight() {
	m_logError.logStream() << "Removing light." << std::endl;
}

void CEntityLight::Initialize( aiNode* node ) {
	CEntity::Initialize( node );

	aiNode* lowNode = node->FindNode( m_pcName.c_str() );
	createModelMatrix( lowNode );
	m_uniformLight.m_position = m_modelMatrix * m_uniformLight.m_position;
}

void CEntityLight::Destroy() {
	CEntity::Destroy();
}

void CEntityLight::Update( float deltaTime ) {
	CEntity::Update( deltaTime );
}

void CEntityLight::Draw( float deltaTime ) {
	CEntity::Draw( deltaTime );
}

SUniformLight& CEntityLight::GetUniformLight() {
	return m_uniformLight;
}

SUniformLight* CEntityLight::GetUniformLightP() {
	return &m_uniformLight;
}
