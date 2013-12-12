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


#include "cgame.h"

#include <stdexcept>
#include <iostream>
#include "ckeyboardmouseinput.h"
#include "cscene.h"
#include "cni.h"

CGame::CGame():
	m_logError(),
	m_window( nullptr ),
	m_clock(),
	m_event(),
	m_contextsettings(),
	m_width( 800 ),
	m_height( 600 ),
	m_input( new CKeyboardMouseInput() ),
	m_sceneManager(),
	m_buffer(),
	m_hasSound(false),
	m_sound(nullptr),
	m_bufferMusic(),
	m_soundMusic(nullptr),
	m_currentSong(0)
	{
		m_hasSound = m_buffer.LoadFromFile("../resources/Sounds/2329__jovica__stab-004-mastered-16-bit.wav");
		if( m_hasSound){
			m_sound.reset( new sf::Sound(m_buffer));
		}
		m_logError.logStream() << "(II) Loading music: ";
		m_logError.logStream() <<  m_bufferMusic[0].LoadFromFile("../resources/Sounds/album/01 - No Bother.ogg") ;
		m_logError.logStream() <<  m_bufferMusic[1].LoadFromFile("../resources/Sounds/album/02 - Just a Dream.ogg") ;
		m_logError.logStream() <<  m_bufferMusic[2].LoadFromFile("../resources/Sounds/album/03 - See Your Face.ogg") ;
		m_logError.logStream() <<  m_bufferMusic[3].LoadFromFile("../resources/Sounds/album/04 - Who's Who.ogg") ;
		m_logError.logStream() <<  m_bufferMusic[4].LoadFromFile("../resources/Sounds/album/05 - Peace Pipe.ogg") ;
		m_logError.logStream() <<  m_bufferMusic[5].LoadFromFile("../resources/Sounds/album/06 - Let it Go.ogg") ;
		m_logError.logStream() <<  m_bufferMusic[6].LoadFromFile("../resources/Sounds/album/07 - What I Know.ogg") ;
		m_logError.logStream() << std::endl;
}

// Game::Game( const Game& other ) {
//
// }

CGame::~CGame() {
	CVertexShader::free();
	CFragmentShader::free();
	CShaderProgram::free();
	m_window.reset();
}

CGame& CGame::operator=( const CGame& other ) {
	throw( std::runtime_error( "Don't assign !\n" ) );

	if( &other == this ) {
		return *this;
	} else {
		return *this;
	}
}

bool CGame::operator==( const CGame& other ) const {
///TODO: return ...;
	throw( std::runtime_error( "You can't compare!\n" ) );

	if( &other == this ) {
		return true;
	} else {
		return false;
	}
}

void CGame::init() {	
	if( m_hasSound){
		m_sound->Play();
	}
	m_window =  std::unique_ptr<sf::RenderWindow>(
					new sf::RenderWindow(
						sf::VideoMode( m_width, m_height ),
						"COGLAINI-B",
						sf::Style::Titlebar | sf::Style::Close,
						sf::ContextSettings( 32, 8, 8 ) ) );

	if( m_window == nullptr ) {
		throw( std::runtime_error( "Could not initialize RenderWindow\n" ) );
	}

	m_contextsettings = m_window->GetSettings();
	m_logError.logStream() << "AntialiasingLevel: " << m_contextsettings.AntialiasingLevel << std::endl;
	m_logError.logStream() << "DepthBits: " << m_contextsettings.DepthBits << std::endl;
	m_logError.logStream() << "StencilBits: " << m_contextsettings.StencilBits << std::endl;
	m_logError.logStream() << "OpenGL Version: " << m_contextsettings.MajorVersion << "." << m_contextsettings.MinorVersion << std::endl;

	GLenum err = glewInit();

	if( err != GLEW_OK ) {
		throw( std::runtime_error( ( char* )( glewGetErrorString( err ) ) ) );
	}

	if( m_contextsettings.MajorVersion >= 3 ) {
		initOpenGL3Plus();
	} else {
		initOpenGL2();
	}
	

}

void CGame::initOpenGL2() {
	m_logError.logStream() << "Using OpenGL 2 and previous old-style.\n";
	// Enable Z-buffer read and write
	glEnable( GL_DEPTH_TEST );
	glDepthMask( GL_TRUE );
	glClearDepth( 1.f );

	// Setup a perspective projection
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 90.f, 1.f, 1.f, 500.f );
}

void CGame::initOpenGL3Plus() {
	m_logError.logStream() << "Using OpenGL 3 and next new-style!\n";

	// config opengl
	glClearColor( .0f, .0f, .0f, 1.0f );
	glClearDepth( 1.0f );

	glViewport( 0, 0, m_width, m_height );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable( GL_POLYGON_SMOOTH );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	// It requires us to specify a fov (field of view) in degrees,
	// followed by the aspect ratio for our window
	// and finally the near and far planes for our scene.

	m_sceneManager.loadScene( "../resources/Models/dwarf.x" );
	m_sceneManager.loadScene( "../resources/Test-DAE/cube.cam.dae" );
	m_sceneManager.loadScene( "../resources/Engine1-Data/stillscene1b.dae" );
	m_sceneManager.loadScene( "../resources/GT3-Session4-Data/testMan14.dae" );
	m_sceneManager.loadScene( "../resources/GT3-Session4-Data/test11_3Anim.dae" );

	m_sceneManager.setScene( 0 );
}

const glm::core::type::mat4& CGame::getProjetionMatrix() {
	return getCamera().getProjectionMatrix();
}

CCamera& CGame::getCamera() {
	return *( m_sceneManager.getCurrentScene()->getCurrentCamera() );
}

void CGame::run() {
	sf::Uint32 timeNew = 0;
	sf::Uint32 timeOld = m_clock.GetElapsedTime();

	while( m_window->IsOpened() && !m_input->hasQuit() ) {
		playAlbum();
		if( CNI::UseNI()){
			CNI::Update();
		}
		timeNew = m_clock.GetElapsedTime();
		float timeDiff = static_cast<float>( timeNew - timeOld ) / 1000.0f;
		timeOld = timeNew;
		handleEvents();

		m_input->update( timeDiff * 10.0f );
		glm::vec3& rotDiff = m_input->getRotationDifference();
		glm::vec3& transDiff = m_input->getPositionDifference();
		if( !(rotDiff[0] == 0.0f && rotDiff[1] == 0.0f && rotDiff[2] == 0.0f)){
			getCamera().pitchYawRoll( rotDiff );	
		}
		if( !(transDiff[0] == 0.0f && transDiff[1] == 0.0f && transDiff[2] == 0.0f)){
			getCamera().translate( transDiff);
		}
		
		if( CNI::UseNI()){
			if( CNI::getX() != 4 || CNI::getY() != 4){
				getCamera().pitchYawRoll(glm::vec3( CNI::getY() - 4, CNI::getX() - 4, 0.0f));
			}
			
			if( CNI::GetPush()){
				m_sceneManager.getCurrentScene()->shoot();
			}
		}

// 		// Draw the background
// 		window.SaveGLStates();
// 		window.Draw(background);
// 		window.RestoreGLStates();

		// Activate the window before using OpenGL commands.
		// This is useless here because we have only one window which is
		// always the active one, but don't forget it if you use multiple windows
		m_window->SetActive();

		clearScreen();

		m_sceneManager.getCurrentScene()->Update( timeDiff );
		m_sceneManager.getCurrentScene()->Draw( timeDiff );
// 		m_sceneManager.getCurrentScene()->Update( 0);
// 		m_sceneManager.getCurrentScene()->Draw( 0);
		drawText();

		// Finally, display the rendered frame on screen
		m_window->Display();

// 		timeDiff = m_clock.GetElapsedTime() - timeOld;
	}
}

void CGame::handleEvents() {
	// Process events
	while( m_window->PollEvent( m_event ) ) {
		if( m_event.Type == sf::Event::Closed ) { // Close window : exit
			m_window->Close();
		} else
			if( m_event.Type == sf::Event::Resized ) { // Adjust the viewport when the window is resized
				glViewport( 0, 0, m_event.Size.Width, m_event.Size.Height );
			} else
				if( m_event.Type == sf::Event::KeyPressed ) {
					if( m_event.Key.Code == sf::Keyboard::Key::C ) {
						m_sceneManager.getCurrentScene()->setNextCamera();
					} else
						if( m_event.Key.Code == sf::Keyboard::Key::V ) {
							m_sceneManager.getCurrentScene()->getCurrentCamera()->toggleCameraMode();
						} else
							if( m_event.Key.Code == sf::Keyboard::Key::P ) {
								m_sceneManager.getCurrentScene()->togglePolygonMode();
							} else
								if( m_event.Key.Code == sf::Keyboard::Key::N ) {
									m_sceneManager.setNextScene();
								} else
									if( m_event.Key.Code == sf::Keyboard::Key::F1 ) {
										m_sceneManager.getCurrentScene()->togglePhysicDebugDraw();
									} else
										if( m_event.Key.Code == sf::Keyboard::Key::F2 ) {
											m_sceneManager.getCurrentScene()->togglePhysicDebugWireframe();
										} else
											if( m_event.Key.Code == sf::Keyboard::Key::F3 ) {
												m_sceneManager.getCurrentScene()->getCurrentCamera()->toggleGhost();
											}else if( m_event.Key.Code == sf::Keyboard::Key::Return ) {
												m_sceneManager.getCurrentScene()->shoot();
											}else if( m_event.Key.Code == sf::Keyboard::Key::Z ) {
												playNext();
											}else if( m_event.Key.Code == sf::Keyboard::Key::U ) {
												playPrev();
											}
				}else if( m_event.Type == sf::Event::MouseButtonPressed){
					m_sceneManager.getCurrentScene()->shoot();
				}
	}
}

void CGame::drawText( ) {
	// Draw some text on top of our OpenGL object
	m_window->SaveGLStates();
	sf::Text text( "COGLAINI-B" );
	text.SetPosition( 250.f, 450.f );
	text.SetColor( sf::Color( 255, 255, 255, 170 ) );
	m_window->Draw( text );
	m_window->RestoreGLStates();
}

void CGame::clearScreen() {
	// Clear the depth buffer and color buffer
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
}

float CGame::getElapsedTime() const {
	return m_clock.GetElapsedTime();
}

const std::unique_ptr< IInput >& CGame::getInput() {
	return m_input;
}

void CGame::playAlbum() {
	if(m_currentSong >= m_bufferMusic.size()){
		m_currentSong = 0;
	}
	if( m_soundMusic == nullptr){
		m_soundMusic.reset( new sf::Sound(m_bufferMusic[m_currentSong]));
		m_logError.logStream() << "(II) Playing : " << m_currentSong << std::endl;
		++m_currentSong;
		m_soundMusic->Play();
	}
	if( m_soundMusic->GetStatus() == sf::Sound::Status::Stopped){
		m_soundMusic.reset(nullptr);
	}
}

void CGame::playNext() {
	++m_currentSong;
	m_soundMusic.reset(nullptr);
}

void CGame::playPrev() {
	m_soundMusic.reset(nullptr);
	if( m_currentSong == 0){
		m_currentSong = m_bufferMusic.size() - 1;
	}else{
		--m_currentSong;
	}
}
