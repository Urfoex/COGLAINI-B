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


#ifndef SVERTEXDATA_H
#define SVERTEXDATA_H

#include "SFML_OpenGL.h"

// Position
template< typename vector_P_t> struct SVertexData_P {
	vector_P_t m_position;

	SVertexData_P<vector_P_t>():
		m_position()
	{};

	SVertexData_P<vector_P_t>( vector_P_t position ):
		m_position( position )
	{};

	SVertexData_P<vector_P_t>( vector_P_t && position ):
		m_position( position )
	{};

	static size_t size() {
		return sizeof( SVertexData_P<vector_P_t> );
	};
};

// Position
// Color: 0xRRGGBBAA
template< typename vector_P_t> struct SVertexData_PC {
	vector_P_t m_position;
	GLuint m_color;

	SVertexData_PC<vector_P_t>():
		m_position(),
		m_color()
	{};

	SVertexData_PC<vector_P_t>( vector_P_t position, GLuint color ):
		m_position( position ),
		m_color( color )
	{};

	SVertexData_PC<vector_P_t>( vector_P_t && position, GLuint color ):
		m_position( position ),
		m_color( color )
	{};

	static size_t size() {
		return sizeof( SVertexData_PC< vector_P_t > );
	};
};

// Position
// Normal
template< typename vector_PN_t> struct SVertexData_PN {
	vector_PN_t m_position;
	vector_PN_t m_normal;

	SVertexData_PN<vector_PN_t>():
		m_position(),
		m_normal()
	{};

	SVertexData_PN<vector_PN_t>( vector_PN_t position, vector_PN_t normal ):
		m_position( position ),
		m_normal( normal )
	{};

	SVertexData_PN<vector_PN_t>( vector_PN_t && position, vector_PN_t && normal ):
		m_position( position ),
		m_normal( normal )
	{};

	static size_t size() {
		return sizeof( SVertexData_PN< vector_PN_t > );
	};
};

// Position
// Texture
template< typename vector_P_t, typename vector_T_t> struct SVertexData_PT {
	vector_P_t m_position;
	vector_T_t m_texture;

	SVertexData_PT<vector_P_t, vector_T_t>():
		m_position(),
		m_texture()
	{};

	SVertexData_PT<vector_P_t, vector_T_t>( vector_P_t position, vector_T_t texture ):
		m_position( position ),
		m_texture( texture )
	{};

	SVertexData_PT<vector_P_t, vector_T_t>( vector_P_t && position, vector_T_t && texture ):
		m_position( position ),
		m_texture( texture )
	{};

	static size_t size() {
		return sizeof( SVertexData_PT< vector_P_t, vector_T_t > );
	};
};

// Position
// Normal
// Texture
template< typename vector_PN_t, typename vector_T_t> struct SVertexData_PNT {
	vector_PN_t m_position;
	vector_PN_t m_normal;
	vector_T_t m_texture;

	SVertexData_PNT<vector_PN_t, vector_T_t>():
		m_position(),
		m_normal(),
		m_texture()
	{};

	SVertexData_PNT<vector_PN_t, vector_T_t>( vector_PN_t position, vector_PN_t normal, vector_T_t texture ):
		m_position( position ),
		m_normal( normal ),
		m_texture( texture )
	{};

	SVertexData_PNT<vector_PN_t, vector_T_t>( vector_PN_t && position, vector_PN_t normal, vector_T_t && texture ):
		m_position( position ),
		m_normal( normal ),
		m_texture( texture )
	{};

	static size_t size() {
		return sizeof( SVertexData_PNT< vector_PN_t, vector_T_t > );
	};
};

static_assert( sizeof( SVertexData_P<GLuint> ) == sizeof( GLuint ) , "SVertexData_P has wrong size." );
static_assert( sizeof( SVertexData_PC<GLuint> ) == sizeof( GLuint ) * 2 , "SVertexData_PC has wrong size." );
static_assert( sizeof( SVertexData_PN<GLuint> ) == sizeof( GLuint ) * 2 , "SVertexData_PN has wrong size." );
static_assert( sizeof( SVertexData_PNT<GLuint, GLbyte> ) == sizeof( GLuint ) * 3 , "SVertexData_PNT has wrong size." );
static_assert( sizeof( SVertexData_PT<GLuint, GLbyte> ) == sizeof( GLuint ) * 2 , "SVertexData_PT has wrong size." );
// HINT: sizeof( struct{ bool; int;}) != sizeof(bool) + sizeof(int)

#endif // SVERTEXDATA_H
