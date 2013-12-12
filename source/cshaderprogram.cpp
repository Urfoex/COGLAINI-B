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


#include "cshaderprogram.h"
#include <algorithm>
#include <stdexcept>
#include <cstdarg>

std::map< std::pair<CVertexShader*, CFragmentShader*>, std::shared_ptr<CShaderProgram>> CShaderProgram::m_listOfCSP;
GLuint CShaderProgram::m_nextFreeUniformBlockBinding = 1;

std::shared_ptr< CShaderProgram > CShaderProgram::CreateShaderProgram( std::shared_ptr< CVertexShader > vs, std::shared_ptr< CFragmentShader > fs ) {
	std::shared_ptr< CShaderProgram > pcsp = nullptr;

	try {
		pcsp = m_listOfCSP.at( std::pair<CVertexShader*, CFragmentShader*>( vs.get(), fs.get() ) );
	} catch( std::out_of_range e ) {
		pcsp = std::shared_ptr< CShaderProgram >( new CShaderProgram( vs, fs ) );
		m_listOfCSP.insert( std::pair<std::pair<CVertexShader*, CFragmentShader*>, std::shared_ptr<CShaderProgram>>( std::pair<CVertexShader*, CFragmentShader*>( vs.get(), fs.get() ), pcsp ) );
	}

	return pcsp;
}


void CShaderProgram::free() {
	m_listOfCSP.clear();
}

CShaderProgram::CShaderProgram( std::shared_ptr< CVertexShader > vs, std::shared_ptr< CFragmentShader > fs ):
	m_logError(),
	m_shaderprogram( glCreateProgram() ),
	m_shadervector(),
	m_attributeCount( 0 ),
	m_attached( false ),
	m_created( false ) {
// 	m_logError.logStream() << "Creating Shader-Program: (" << m_shaderprogram << ") " << vs->getShader() << "-" << fs->getShader() << std::endl;
	m_shadervector.push_back( vs );

	if( fs.get() != nullptr ) {
		m_shadervector.push_back( fs );
	}
}

// CShaderProgram::CShaderProgram( std::shared_ptr< CVertexShader > vs, std::shared_ptr< CFragmentShader > fs, std::shared_ptr< CGeometryShader > gs ):
// 	m_shaderprogram( glCreateProgram() ),
// 	m_shadervector(),
// 	m_attributeCount(0)
// {
// 	logStream() << "Creating Shader-Program: (" << m_shaderprogram << ") " << vs->getShader() << "-" << fs->getShader() << "-" << gs->getShader() << std::endl;
// 	m_shadervector.push_back( vs );
// 	m_shadervector.push_back( fs );
// 	m_shadervector.push_back( gs );
// }

CShaderProgram::~CShaderProgram() {
	m_logError.logStream() << "Removing Shader-Program: " << m_shaderprogram << std::endl;
	detachShader();
	m_shadervector.clear();
	glDeleteProgram( m_shaderprogram );
}

void CShaderProgram::createProgram() {
	if( m_created ) {
// 		m_logError.logStream() << "Program is already created: " << m_shaderprogram << std::endl;;
		return;
	}

	attachShader();
	// TODO: BindAttributLocation ??
	glLinkProgram( m_shaderprogram );
	GLint status;
	glGetProgramiv( m_shaderprogram, GL_LINK_STATUS, &status );

	if( status == GL_FALSE ) {
		// TODO: Get info-log
		logProgramError();
		throw( std::runtime_error( "Failed to link shader program.\n" ) );
	}

	m_created = true;
}

void CShaderProgram::addAttribute( std::string& attributeName ) {
	addAttribute( std::move( attributeName ) );
}

void CShaderProgram::addAttribute( std::string && attributeName ) {
	glBindAttribLocation( m_shaderprogram, m_attributeCount++, attributeName.c_str() );
	GLenum err = glGetError();

	switch( err ) {
	case GL_INVALID_VALUE:
		throw( std::runtime_error( "GL_INVALID_VALUE is generated if index is greater than or equal to GL_MAX_VERTEX_ATTRIBS." ) );
		break;
	case GL_INVALID_OPERATION:
		throw( std::runtime_error( "GL_INVALID_OPERATION is generated if name starts with the reserved prefix \"gl_\"." ) );
		break;
	case GL_NO_ERROR:
		break;
	default:
		throw( std::runtime_error( "HELP!!!" ) );
	}
}

void CShaderProgram::setAttributes( uint32_t quantity, ... ) {
	std::va_list attribList;
	va_start( attribList, quantity );

	while( quantity-- ) {
		addAttribute( std::string( va_arg( attribList, char* ) ) );
	}

	va_end( attribList );
}

void CShaderProgram::attachShader() {
	if( m_attached ) {
// 		m_logError.logStream() << "Shader already attached to program: " << m_shaderprogram << std::endl;;
		return;
	}

#if ( defined( __GNUC__ ) || defined( __GNUG__ ) )

for( auto shader : m_shadervector ) {
#else
	GLuint& m_shaderprogram = this->m_shaderprogram;
	std::for_each( m_shadervector.begin(), m_shadervector.end(), [&m_shaderprogram]( std::shared_ptr<CShaderLoader>& shader ) {
#endif
		glAttachShader( m_shaderprogram, shader->getShader() );
	}
#if !( defined( __GNUC__ ) || defined( __GNUG__ ) )
	);
#endif
	m_attached = true;
}

void CShaderProgram::detachShader() {
	if( !m_attached ) {
		m_logError.logStream() << "Nothing was attached to program: " << m_shaderprogram << std::endl;;
		return;
	}

#if ( defined( __GNUC__ ) || defined( __GNUG__ ) )

for( auto shader : m_shadervector ) {
#else
	GLuint& m_shaderprogram = this->m_shaderprogram;
	std::for_each( m_shadervector.begin(), m_shadervector.end(), [&m_shaderprogram]( std::shared_ptr<CShaderLoader>& shader ) {
#endif
		glDetachShader( m_shaderprogram, shader->getShader() );
	}
#if !( defined( __GNUC__ ) || defined( __GNUG__ ) )
	);
#endif
	m_attached = false;
}

void CShaderProgram::logProgramError() {
	GLint length = 0;
	glGetProgramiv( m_shaderprogram, GL_INFO_LOG_LENGTH, &length );

	if( length > 0 ) {
		std::unique_ptr<GLchar[]> string( new GLchar[length] );
		GLint readlength = 0;
		glGetProgramInfoLog( m_shaderprogram, length, &readlength, string.get() );
		m_logError.logStream() << string.get();
	}
}

GLuint& CShaderProgram::getShaderProgram() {
	return m_shaderprogram;
}

std::map< std::pair< CVertexShader*, CFragmentShader* >, std::shared_ptr< CShaderProgram > >& CShaderProgram::getAllShaderPrograms() {
	return m_listOfCSP;
}

GLuint CShaderProgram::getNextFreeUniformBlockBinding() {
	return m_nextFreeUniformBlockBinding++;
}

VertexAttributes::VertexAttributes( unsigned int number ):
	m_positions( new glm::vec3[number] ),
	m_normals( new glm::vec3[number] ),
	m_texCoords( new glm::vec2[number] ),
	m_boneWeights( new glm::vec4[number] ),
	m_boneIDs( new glm::uvec4[number] ),
	m_sizePositions( sizeof( glm::vec3 ) * number ),
	m_sizeNormals( sizeof( glm::vec3 ) * number ),
	m_sizeTexCoords( sizeof( glm::vec2 ) * number ),
	m_sizeBoneWeights( sizeof( glm::vec4 ) * number ),
	m_sizeBoneIDs( sizeof( glm::uvec4 ) * number ),
	m_numbers( number ) {
}

VertexAttributes::VertexAttributes():
	m_positions( nullptr ),
	m_normals( nullptr ),
	m_texCoords( nullptr ),
	m_boneWeights( nullptr ),
	m_boneIDs( nullptr ),
	m_sizePositions( 0 ),
	m_sizeNormals( 0 ),
	m_sizeTexCoords( 0 ),
	m_sizeBoneWeights( 0 ),
	m_sizeBoneIDs( 0 ),
	m_numbers( 0 ) {
}

void VertexAttributes::setPos( unsigned int number ) {
	m_positions.reset( new glm::vec3[number] );
	m_normals.release( );
	m_texCoords.release();
	m_boneWeights.release();
	m_boneIDs.release();
	m_sizePositions = sizeof( glm::vec3 ) * number;
	m_sizeNormals = 0;
	m_sizeTexCoords = 0;
	m_sizeBoneWeights = 0;
	m_sizeBoneIDs = 0;
	m_numbers = number;
}

void VertexAttributes::setPosNorm( unsigned int number ) {
	m_positions.reset( new glm::vec3[number] );
	m_normals.reset( new glm::vec3[number] );
	m_texCoords.release();
	m_boneWeights.release();
	m_boneIDs.release();
	m_sizePositions = sizeof( glm::vec3 ) * number;
	m_sizeNormals = sizeof( glm::vec3 ) * number;
	m_sizeTexCoords = 0;
	m_sizeBoneWeights = 0;
	m_sizeBoneIDs = 0;
	m_numbers = number;
}

void VertexAttributes::setPosNormTex( unsigned int number ) {
	m_positions.reset( new glm::vec3[number] );
	m_normals.reset( new glm::vec3[number] );
	m_texCoords.reset( new glm::vec2[number] );
	m_boneWeights.release();
	m_boneIDs.release();
	m_sizePositions = sizeof( glm::vec3 ) * number;
	m_sizeNormals = sizeof( glm::vec3 ) * number;
	m_sizeTexCoords = sizeof( glm::vec2 ) * number;;
	m_sizeBoneWeights = 0;
	m_sizeBoneIDs = 0;
	m_numbers = number;
}

void VertexAttributes::setPosNormBone( unsigned int number ) {
	m_positions.reset( new glm::vec3[number] );
	m_normals.reset( new glm::vec3[number] );
	m_texCoords.release();
	m_boneWeights.reset( new glm::vec4[number] );
	m_boneIDs.reset( new glm::uvec4[number] );
	m_sizePositions = sizeof( glm::vec3 ) * number;
	m_sizeNormals = sizeof( glm::vec3 ) * number;
	m_sizeTexCoords = 0;
	m_sizeBoneWeights = sizeof( glm::vec4 ) * number;
	m_sizeBoneIDs = sizeof( glm::uvec4 ) * number;
	m_numbers = number;
}
