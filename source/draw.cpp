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


#include "draw.h"
#include <string>
#include "SFML_OpenGL.h"
#include "cshaderprogram.h"
#include "headers_with_gcc_warnings/glm.hpp"
#include "headers_with_gcc_warnings/type_ptr.hpp"

//FIXME doesn't work
void Draw::Line( const btVector3& from, const btVector3& to, const btVector3& color ) {
	std::string vsshader(
		"layout (location = 10) attribute vec4 position;\n"
		"uniform mat4 projection;\n"
		"void main () {\n"
		"gl_Position = position * projection;\n"
		"}\n" );
	std::string fsshader(
		"uniform highp vec3 color;\n"
		"void main () {\n"
		"gl_FragColor = vec4(color.r, color.g, color.b, 0.0);\n"
		"}" );
	std::shared_ptr<CVertexShader> cvs = CVertexShader::CreateVertexShader( "LineVS", vsshader );
	std::shared_ptr<CFragmentShader> cfs = CFragmentShader::CreateFragmentShader( "LineFS", fsshader );
	std::shared_ptr<CShaderProgram> csp = CShaderProgram::CreateShaderProgram( cvs, cfs );

	if( !csp->wasAlreadyCreated() ) {
		csp->createProgram();
	}


	glUseProgram( csp->getShaderProgram() );

	// Set the projection matrix
	GLint pu = glGetUniformLocation( csp->getShaderProgram(), "projection" );
	glm::mat3 m; // FIXME: projection from somewhere?
	glUniformMatrix4fv( pu, 1, 0, glm::value_ptr( m ) );

	// Set the colour of the line
	GLint puc = glGetUniformLocation( csp->getShaderProgram(), "color" );
	glm::vec3 v = glm::vec3( color.getX(), color.getY(), color.getZ() );
	glUniform3fv( puc, 1, glm::value_ptr( v ) );

	// Set the line vertices
	float tmp[ 6 ] = { from.getX(), from.getY(), from.getZ(),
					   to.getX(), to.getY(), to.getZ()
					 };
	glEnableVertexAttribArray( 10 );
	glVertexAttribPointer( 10, 3, GL_FLOAT, false, 0, &tmp[0] );

	glDrawArrays( GL_LINES, 0, 2 );

	glDisableVertexAttribArray( 10 );
	glUseProgram( 0 );
}
