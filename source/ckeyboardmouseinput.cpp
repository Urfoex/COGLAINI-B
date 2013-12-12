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


#include "ckeyboardmouseinput.h"

CKeyboardMouseInput::CKeyboardMouseInput():
	m_window( nullptr ),
	m_position( 0.0f ),
	m_positionDifference( 0.0f ),
	m_rotation( 0.0f ),
	m_rotationDifference( 0.0f ),
	m_scale( 0.0f ),
	m_scaleDifference( 0.0f ),
	m_speedMotion( 1.0f ),
	m_speedRotation( 10.0f ) {
	m_rotation.x = -sf::Mouse::GetPosition().y;
	m_rotation.y = sf::Mouse::GetPosition().x;
}

CKeyboardMouseInput::CKeyboardMouseInput( sf::Window& window ):
	m_window( &window ),
	m_position( 0.0f ),
	m_positionDifference( 0.0f ),
	m_rotation( 0.0f ),
	m_rotationDifference( 0.0f ),
	m_scale( 0.0f ),
	m_scaleDifference( 0.0f ),
	m_speedMotion( 1.0f ),
	m_speedRotation( 10.0f ) {
	m_rotation.x = -sf::Mouse::GetPosition().y;
	m_rotation.y = sf::Mouse::GetPosition().x;
}

CKeyboardMouseInput::~CKeyboardMouseInput( ) {

}

void CKeyboardMouseInput::init() {

}

void CKeyboardMouseInput::init( sf::Window& window ) {
	m_window = &window;
}

void CKeyboardMouseInput::update( float timediff ) {
	m_positionDifference = glm::vec3( 0.0f );
	m_rotationDifference = glm::vec3( 0.0f );

	if( sf::Keyboard::IsKeyPressed( sf::Keyboard::Key::W ) ) {
		m_positionDifference.z += m_speedMotion * timediff;
	}

	if( sf::Keyboard::IsKeyPressed( sf::Keyboard::Key::S ) ) {
		m_positionDifference.z += -m_speedMotion * timediff;
	}

	if( sf::Keyboard::IsKeyPressed( sf::Keyboard::Key::A ) ) {
		m_positionDifference.x += +m_speedMotion * timediff;
	}

	if( sf::Keyboard::IsKeyPressed( sf::Keyboard::Key::D ) ) {
		m_positionDifference.x += -m_speedMotion * timediff;
	}

	if( sf::Keyboard::IsKeyPressed( sf::Keyboard::Key::LControl ) ) {
		m_positionDifference.y += -m_speedMotion * timediff;
	}

	if( sf::Keyboard::IsKeyPressed( sf::Keyboard::Key::Space ) ) {
		m_positionDifference.y += +m_speedMotion * timediff;
	}

	if( sf::Keyboard::IsKeyPressed( sf::Keyboard::Key::Q ) ) {
		m_rotationDifference.z -= m_speedRotation * timediff;
	}

	if( sf::Keyboard::IsKeyPressed( sf::Keyboard::Key::E ) ) {
		m_rotationDifference.z += m_speedRotation * timediff;
	}

	// TODO: dynamic mouse for e.g. fullscreen and over the edges
	// get mouse-movement-diff
	// set mouse back to center
	m_rotationDifference.x += static_cast<float>( -sf::Mouse::GetPosition().y ) - m_rotation.x;
	m_rotationDifference.y += static_cast<float>( sf::Mouse::GetPosition().x ) - m_rotation.y;

// 	// TODO: can't use because mouse is absolute
// 	if( sf::Keyboard::IsKeyPressed( sf::Keyboard::Key::Y)){
// 		m_rotationDifference.y -= m_speedRotation * timediff;
// 	}
// 	if( sf::Keyboard::IsKeyPressed( sf::Keyboard::Key::X)){
// 		m_rotationDifference.y += m_speedRotation * timediff;
// 	}
//
// 	if( sf::Keyboard::IsKeyPressed( sf::Keyboard::Key::F)){
// 		m_rotationDifference.x += m_speedRotation * timediff;
// 	}
// 	if( sf::Keyboard::IsKeyPressed( sf::Keyboard::Key::V)){
// 		m_rotationDifference.x -= m_speedRotation * timediff;
// 	}

	m_position += m_positionDifference;
	m_rotation += m_rotationDifference;
}

glm::core::type::vec3& CKeyboardMouseInput::getPosition() {
	return m_position;
}

glm::core::type::vec3& CKeyboardMouseInput::getPositionDifference() {
	return m_positionDifference;
}

glm::core::type::vec3& CKeyboardMouseInput::getRotation() {
	return ( m_rotation );
}

glm::core::type::vec3& CKeyboardMouseInput::getRotationDifference() {
	return m_rotationDifference;
}

glm::core::type::vec3& CKeyboardMouseInput::getScale() {
	return m_scale;
}

glm::core::type::vec3& CKeyboardMouseInput::getScaleDifference() {
	return m_scaleDifference;
}

bool CKeyboardMouseInput::hasQuit() {
	return sf::Keyboard::IsKeyPressed( sf::Keyboard::Key::Escape );
}
