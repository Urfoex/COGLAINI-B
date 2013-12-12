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


#ifndef CKEYBOARDMOUSEINPUT_H
#define CKEYBOARDMOUSEINPUT_H

#include "iinput.h"
#include <SFML/Window.hpp>

class CKeyboardMouseInput : public IInput {

public:
	CKeyboardMouseInput( );
	CKeyboardMouseInput( const CKeyboardMouseInput& );
	void operator=( const CKeyboardMouseInput& );
	CKeyboardMouseInput( sf::Window& window );
	virtual ~CKeyboardMouseInput();
	virtual void init();
	virtual void init( sf::Window& window );
	virtual void update( float timediff );
	virtual glm::core::type::vec3& getPosition();
	virtual glm::core::type::vec3& getPositionDifference() ;
	virtual glm::core::type::vec3& getRotation() ;
	virtual glm::core::type::vec3& getRotationDifference() ;
	virtual glm::core::type::vec3& getScale();
	virtual glm::core::type::vec3& getScaleDifference() ;
	virtual bool hasQuit() ;

private:
	sf::Window* m_window;
	glm::vec3 m_position;
	glm::vec3 m_positionDifference;
	glm::vec3 m_rotation;
	glm::vec3 m_rotationDifference;
	glm::vec3 m_scale;
	glm::vec3 m_scaleDifference;

	float m_speedMotion;
	float m_speedRotation;
};

#endif // CKEYBOARDMOUSEINPUT_H
