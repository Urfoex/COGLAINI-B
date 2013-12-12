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


#include "ccubeogl2.h"
#include "cgame.h"

CCubeOGL2::CCubeOGL2( CGame* game ): IDrawableObject( game ) {

}

CCubeOGL2::~CCubeOGL2() {

}

void CCubeOGL2::init() {

}

void CCubeOGL2::draw() {
	// Draw a cube
	float size = 20.f;
	glColor3f( 1.0f, 0.0f, 0.0f );
	glBegin( GL_QUADS );

	glTexCoord2f( 0, 0 );
	glVertex3f( -size, -size, -size );
	glTexCoord2f( 0, 1 );
	glVertex3f( -size,  size, -size );
	glTexCoord2f( 1, 1 );
	glVertex3f( size,  size, -size );
	glTexCoord2f( 1, 0 );
	glVertex3f( size, -size, -size );

	glTexCoord2f( 0, 0 );
	glVertex3f( -size, -size, size );
	glTexCoord2f( 0, 1 );
	glVertex3f( -size,  size, size );
	glTexCoord2f( 1, 1 );
	glVertex3f( size,  size, size );
	glTexCoord2f( 1, 0 );
	glVertex3f( size, -size, size );

	glTexCoord2f( 0, 0 );
	glVertex3f( -size, -size, -size );
	glTexCoord2f( 0, 1 );
	glVertex3f( -size,  size, -size );
	glTexCoord2f( 1, 1 );
	glVertex3f( -size,  size,  size );
	glTexCoord2f( 1, 0 );
	glVertex3f( -size, -size,  size );

	glTexCoord2f( 0, 0 );
	glVertex3f( size, -size, -size );
	glTexCoord2f( 0, 1 );
	glVertex3f( size,  size, -size );
	glTexCoord2f( 1, 1 );
	glVertex3f( size,  size,  size );
	glTexCoord2f( 1, 0 );
	glVertex3f( size, -size,  size );

	glTexCoord2f( 0, 1 );
	glVertex3f( -size, -size,  size );
	glTexCoord2f( 0, 0 );
	glVertex3f( -size, -size, -size );
	glTexCoord2f( 1, 0 );
	glVertex3f( size, -size, -size );
	glTexCoord2f( 1, 1 );
	glVertex3f( size, -size,  size );

	glTexCoord2f( 0, 1 );
	glVertex3f( -size, size,  size );
	glTexCoord2f( 0, 0 );
	glVertex3f( -size, size, -size );
	glTexCoord2f( 1, 0 );
	glVertex3f( size, size, -size );
	glTexCoord2f( 1, 1 );
	glVertex3f( size, size,  size );

	glEnd();
}

void CCubeOGL2::transform() {
	// Apply some transformations
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef( m_game->getInput()->getPosition().x * 100.0f, m_game->getInput()->getPosition().z * 100.0f , -100.f );
	glRotatef( m_game->getInput()->getRotation().x, 1.f, 0.f, 0.f );
	glRotatef( m_game->getInput()->getRotation().y , 0.f, 1.f, 0.f );
	glRotatef( m_game->getInput()->getRotation().z * 0.0f, 0.f, 0.f, 1.f );
}
