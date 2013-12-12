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

#ifndef CGAME_H
#define CGAME_H

#include "clogerrorstream.h"
#include "cshaderprogram.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "SFML_OpenGL.h"
#include <ostream>
#include <functional>
#include <memory>
#include <cstdint>
#include <list>

#include "headers_with_gcc_warnings/glm.hpp"
#include "../libraries/glm/gtc/type_ptr.hpp"
#include "../libraries/glm/gtc/matrix_transform.hpp"
#include "ccamera.h"
#include "iinput.h"
#include "cscenemanager.h"

#include <array>

class CGame {

public:
	CGame();
	CGame( const CGame& other );
	virtual ~CGame();
	virtual CGame& operator=( const CGame& other );
	virtual bool operator==( const CGame& other ) const;
	void init();
	void run();
	const std::unique_ptr< IInput >& getInput();
	float getElapsedTime() const;
	const glm::mat4& getProjetionMatrix();
	CCamera& getCamera();

private:
	CLogErrorStream m_logError;
	std::unique_ptr<sf::RenderWindow> m_window;
	sf::Clock m_clock;
	sf::Event m_event;
	sf::ContextSettings m_contextsettings;

	uint32_t m_width;
	uint32_t m_height;

	// The projection matrix is a 4×4 matrix that defines the 2D to 3D projection for a scene.
// 	glm::mat4 m_projectionMatrix;
// 	CCamera m_camera;

	std::unique_ptr<IInput> m_input;

	CSceneManager m_sceneManager;

	void initOpenGL2();
	void initOpenGL3Plus();
	void handleEvents();

	void drawText( );

	void clearScreen();

private: // Sound
	sf::SoundBuffer m_buffer;
	std::unique_ptr<sf::Sound> m_sound;
	bool m_hasSound;
	
	std::array< sf::SoundBuffer, 7> m_bufferMusic;
	std::unique_ptr<sf::Sound> m_soundMusic;
	unsigned m_currentSong;
	
	void playAlbum();
	void playNext();
	void playPrev();
	
};

#endif // CGAME_H
