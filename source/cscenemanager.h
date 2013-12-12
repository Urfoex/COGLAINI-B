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


#ifndef CSCENEMANAGER_H
#define CSCENEMANAGER_H

#include <cstdint>
#include <string>
#include <map>
#include <memory>
#include "clogerrorstream.h"

class CScene;
class CSceneManager {

public:
	CSceneManager();
	CSceneManager( const CSceneManager& other );
	virtual ~CSceneManager();
	virtual CSceneManager& operator=( const CSceneManager& other );

	void loadScene( std::string filename );
	void setScene( uint64_t id );
	void setScene( std::string name );
	void setNextScene();

	std::shared_ptr<CScene> getCurrentScene();
	uint64_t getCurrentSceneID();
	std::string getCurrentSceneName();

private:
	CLogErrorStream m_logError;
	std::map< uint64_t, std::shared_ptr<CScene>> m_scenesByID;
	std::map< std::string, std::shared_ptr<CScene>> m_scenesByName;

	uint64_t m_counterID;
	std::shared_ptr<CScene> m_currentScene;

};

#endif // CSCENEMANAGER_H
