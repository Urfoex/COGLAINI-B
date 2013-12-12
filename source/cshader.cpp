#include "cshader.h"
#include <iostream>

CShader::CShader( const char* pcVertexSource, const char* pcFragmentSource ) {
	m_uProjectionMatrixLocation = -1;
	m_uViewMatrixLocation = -1;
	m_uModelMatrixLocation = -1;
	m_uLightPositionLocation = -1;
	m_uLightAmbientColorLocation = -1;
	m_uLightDiffuseColorLocation = -1;
	m_uVertexLocation = -1;
	m_uNormalLocation = -1;
	m_uColorLocation = -1;
	m_uTexCoordLocation = -1;
	m_uBoneWeightsLocation = -1;
	m_uBoneMatricesLocation = -1;
	m_uBoneIndicesLocation = -1;
	m_uUseTextureLocation = -1;
	m_uTextureLocation = -1;

	m_uVertexShader = glCreateShader( GL_VERTEX_SHADER );
	m_uFragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( m_uVertexShader, 1, ( const GLchar** ) &pcVertexSource, NULL );
	glShaderSource( m_uFragmentShader, 1, ( const GLchar** ) &pcFragmentSource, NULL );
	glCompileShader( m_uVertexShader );

	GLint iVertexShaderCompileStatus;
	glGetShaderiv( m_uVertexShader, GL_COMPILE_STATUS, &iVertexShaderCompileStatus );

	if( ( iVertexShaderCompileStatus ) && ( glIsShader( m_uVertexShader ) ) ) {
		std::cout << "Vertex shader compiled" << std::endl;
	} else {
		std::cerr << "Vertex shader not compiled" << std::endl;
	}

	glCompileShader( m_uFragmentShader );

	GLint iFragmentShaderCompileStatus;
	glGetShaderiv( m_uFragmentShader, GL_COMPILE_STATUS, &iFragmentShaderCompileStatus );

	if( ( iFragmentShaderCompileStatus ) && ( glIsShader( m_uFragmentShader ) ) ) {
		std::cout << "Fragment shader compiled" << std::endl;
	} else {
		std::cerr << "Fragment shader not compiled" << std::endl;
	}

	m_uProgram = glCreateProgram();
	glAttachShader( m_uProgram, m_uVertexShader );
	glAttachShader( m_uProgram, m_uFragmentShader );

	glLinkProgram( m_uProgram );

	GLint iLinkedStatus;
	glGetProgramiv( m_uProgram, GL_LINK_STATUS, &iLinkedStatus );

	if( ( iLinkedStatus ) && ( glIsProgram( m_uProgram ) ) ) {
		std::cout << "Shader linked" << std::endl;
	} else {
		std::cerr << "Shader not linked" << std::endl;
	}
}

CShader::~CShader() {
	glDeleteProgram( m_uProgram );
	glDeleteShader( m_uVertexShader );
	glDeleteShader( m_uFragmentShader );
}

void CShader::Bind() const {
	glUseProgram( m_uProgram );
}

void CShader::Unbind() const {
	glUseProgram( 0 );
}

GLuint CShader::GetAttribLocation( const char* pcName ) const {
	return glGetAttribLocation( m_uProgram, pcName );
}

GLuint CShader::GetUniformLocation( const char* pcName ) const {
	return glGetUniformLocation( m_uProgram, pcName );
}

GLuint CShader::GetVertexLocation() {
	if( m_uVertexLocation == -1 ) {
		m_uVertexLocation = GetAttribLocation( "inPosition" );
	}

	return m_uVertexLocation;
}

GLuint CShader::GetNormalLocation() {
	if( m_uNormalLocation == -1 ) {
		m_uNormalLocation = GetAttribLocation( "inNormal" );
	}

	return m_uNormalLocation;
}

GLuint CShader::GetColorLocation() {
	if( m_uColorLocation == -1 ) {
		m_uColorLocation = GetAttribLocation( "inColor" );
	}

	return m_uColorLocation;
}

GLuint CShader::GetTexCoordLocation() {
	if( m_uTexCoordLocation == -1 ) {
		m_uTexCoordLocation = GetAttribLocation( "inTexCoord" );
	}

	return m_uTexCoordLocation;
}

GLuint CShader::GetBoneWeightsLocation() {
	if( m_uBoneWeightsLocation == -1 ) {
		m_uBoneWeightsLocation = GetAttribLocation( "inBoneWeights" );
	}

	return m_uBoneWeightsLocation;
}

GLuint CShader::GetBoneIndicesLocation() {
	if( m_uBoneIndicesLocation == -1 ) {
		m_uBoneIndicesLocation = GetAttribLocation( "inBoneIndices" );
	}

	return m_uBoneIndicesLocation;
}

GLuint CShader::GetProjectionMatrixLocation() {
	if( m_uProjectionMatrixLocation == -1 ) {
		m_uProjectionMatrixLocation = GetUniformLocation( "projectionMatrix" );
	}

	return m_uProjectionMatrixLocation;
}

GLuint CShader::GetViewMatrixLocation() {
	if( m_uViewMatrixLocation == -1 ) {
		m_uViewMatrixLocation = GetUniformLocation( "viewMatrix" );
	}

	return m_uViewMatrixLocation;
}

GLuint CShader::GetModelMatrixLocation() {
	if( m_uModelMatrixLocation == -1 ) {
		m_uModelMatrixLocation = GetUniformLocation( "modelMatrix" );
	}

	return m_uModelMatrixLocation;
}

GLuint CShader::GetLightPositionLocation() {
	if( m_uLightPositionLocation == -1 ) {
		m_uLightPositionLocation = GetUniformLocation( "lightPosition" );
	}

	return m_uLightPositionLocation;
}

GLuint CShader::GetLightAmbientColorLocation() {
	if( m_uLightAmbientColorLocation == -1 ) {
		m_uLightAmbientColorLocation = GetUniformLocation( "lightAmbientColor" );
	}

	return m_uLightAmbientColorLocation;
}

GLuint CShader::GetLightDiffuseColorLocation() {
	if( m_uLightDiffuseColorLocation == -1 ) {
		m_uLightDiffuseColorLocation = GetUniformLocation( "lightDiffuseColor" );
	}

	return m_uLightDiffuseColorLocation;
}

GLuint CShader::GetBoneMatricesLocation() {
	if( m_uBoneMatricesLocation == -1 ) {
		m_uBoneMatricesLocation = GetUniformLocation( "boneMatrices" );
	}

	return m_uBoneMatricesLocation;
}

GLuint CShader::GetUseTextureLocation() {
	if( m_uUseTextureLocation == -1 ) {
		m_uUseTextureLocation = GetUniformLocation( "useTexture" );
	}

	return m_uUseTextureLocation;
}

GLuint CShader::GetTextureLocation() {
	if( m_uTextureLocation == -1 ) {
		m_uTextureLocation = GetUniformLocation( "texture" );
	}

	return m_uTextureLocation;
}
