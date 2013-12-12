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


#include "cscenemanager.h"
#include "cscene.h"
#include <algorithm>

CSceneManager::CSceneManager():
	m_logError(),
	m_scenesByID(),
	m_scenesByName(),
	m_counterID( 0 ),
	m_currentScene( nullptr ) {

}

// CSceneManager::CSceneManager( const CSceneManager& other ) {
// 	throw("Don't!");
// }

CSceneManager::~CSceneManager() {
	m_logError.logStream() << "Removing all " << m_scenesByID.size() << " scenes.\n";
	std::for_each( m_scenesByID.begin(), m_scenesByID.end(), []( std::pair<uint64_t, std::shared_ptr<CScene>> scene ) {
		scene.second->Destroy();
	} );
	m_scenesByID.clear();
	m_scenesByName.clear();
}

CSceneManager& CSceneManager::operator=( const CSceneManager& ) {
	throw( "Don't!" );
	return *this;
}

void CSceneManager::loadScene( std::string filename ) {
	std::shared_ptr<CScene> cs( new CScene( m_counterID++, filename ) );
	m_scenesByID.insert( std::pair<uint64_t, std::shared_ptr<CScene>>( cs->getID(), cs ) );
	m_scenesByName.insert( std::pair<std::string, std::shared_ptr<CScene>>( cs->getName(), cs ) );
	cs->Initialize();
}

void CSceneManager::setScene( uint64_t id ) {
	m_currentScene = m_scenesByID.at( id );
}

void CSceneManager::setScene( std::string name ) {
	m_currentScene = m_scenesByName.at( name );
}

std::shared_ptr< CScene > CSceneManager::getCurrentScene() {
	return m_currentScene;
}

uint64_t CSceneManager::getCurrentSceneID() {
	return m_currentScene->getID();
}

std::string CSceneManager::getCurrentSceneName() {
	return m_currentScene->getName();
}

void CSceneManager::setNextScene() {
	if( getCurrentSceneID() + 1 == m_counterID ) {
		setScene( 0 );
	} else {
		setScene( getCurrentSceneID() + 1 );
	}
	m_logError.logStream() << "(II) Scene: " << getCurrentSceneID() << "\n";
}
