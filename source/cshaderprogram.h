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


#ifndef CSHADERPROGRAM_H
#define CSHADERPROGRAM_H

#include "cvertexshader.h"
#include "cfragmentshader.h"
#include "cgeometryshader.h"

#include <vector>
#include <memory>
#include <string>
#include <cstdint>

#include <cstdint>

#include "headers_with_gcc_warnings/glm.hpp"

struct SUniformLight;
class CShaderProgram {
public:
	static std::shared_ptr<CShaderProgram> CreateShaderProgram( std::shared_ptr<CVertexShader> vs, std::shared_ptr<CFragmentShader> fs );
// Currently no Geometry Shader support ...
// 	static std::shared_ptr<CShaderProgram> CreateShaderProgram( std::shared_ptr<CVertexShader> vs, std::shared_ptr<CFragmentShader> fs, std::shared_ptr<CGeometryShader> gs);
	static void free();
	~CShaderProgram();
	void addAttribute( std::string& attributeName );
	void addAttribute( std::string && attributeName );
	/// @warning be sure to put char*-strings at '...' and the number of them at 'quantity'
	void setAttributes( uint32_t quantity, ... );
	void createProgram();
	GLuint& getShaderProgram();
	static GLuint getNextFreeUniformBlockBinding();
	bool wasAlreadyCreated() {
		return m_created;
	};

	static std::map< std::pair<CVertexShader*, CFragmentShader*>, std::shared_ptr<CShaderProgram>>& getAllShaderPrograms();
private:
	CLogErrorStream m_logError;
	GLuint m_shaderprogram;
	std::vector<std::shared_ptr<CShaderLoader>> m_shadervector;
	GLuint m_attributeCount;

	static std::map< std::pair<CVertexShader*, CFragmentShader*>, std::shared_ptr<CShaderProgram>> m_listOfCSP;
	bool m_attached;
	bool m_created;

	static GLuint m_nextFreeUniformBlockBinding;

	CShaderProgram( std::shared_ptr<CVertexShader> vs, std::shared_ptr<CFragmentShader> fs );
// 	CShaderProgram( std::shared_ptr<CVertexShader> vs, std::shared_ptr<CFragmentShader> fs, std::shared_ptr<CGeometryShader> gs);

	void attachShader();
	void detachShader();
	void logProgramError();
};

struct VertexAttributes {
	std::unique_ptr<glm::vec3[]> m_positions;
	std::unique_ptr<glm::vec3[]> m_normals;
	std::unique_ptr<glm::vec2[]> m_texCoords;
	std::unique_ptr<glm::vec4[]> m_boneWeights;
	std::unique_ptr<glm::uvec4[]> m_boneIDs;

	GLsizei m_sizePositions;
	GLsizei m_sizeNormals;
	GLsizei m_sizeTexCoords;
	GLsizei m_sizeBoneWeights;
	GLsizei m_sizeBoneIDs;

	unsigned int m_numbers;

	VertexAttributes( unsigned int number );
	VertexAttributes();
	void setPosNorm( unsigned int number );
	void setPos( unsigned int number );
	void setPosNormTex( unsigned int number );
	void setPosNormBone( unsigned int number );
};

#endif // CSHADERPROGRAM_H
