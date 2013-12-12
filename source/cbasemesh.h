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


#ifndef CBASEMESH_H
#define CBASEMESH_H

#include "headers_with_gcc_warnings/type_ptr.hpp"
#include "headers_with_gcc_warnings/glm.hpp"
#include "headers_with_gcc_warnings/aiMesh.h"
#include "cshaderprogram.h"
#include <memory>
#include <vector>


class CBaseMesh {
public:
	CBaseMesh();
	CBaseMesh( CBaseMesh& baseMesh );
	virtual ~CBaseMesh();

	virtual void Initialize( std::shared_ptr< std::vector<glm::vec3>> vertices );
	virtual void Initialize( aiMesh* mesh );
	virtual void Destroy();

protected:
	// VertexAttributes
	// modelMatrix = 1
// 	///////////////////////////////////////
// 	// Load color & textures from material
// 	createColors( materials );
// 	loadTextures( materials );
//
// 	////////////////////////////
// 	// Set up vertex attributes
// 	createVerticeAttributes();
// 	putVerticeAttributesToGPU();

private:
	void createVerticeAttributes( std::shared_ptr< std::vector<glm::vec3>> vertices );
	void createVerticeAttributes( aiMesh* mesh );
	void putVerticeAttributesToGPU();

	std::unique_ptr< std::vector< aiVertexWeight >[] > createVectorBoneStructur( aiMesh* mesh );

	CLogErrorStream m_logError;
	std::shared_ptr<VertexAttributes> m_vertexAttributes;

	bool m_isInitialized;
	static const unsigned int m_gMaxBoneMatrixs;
};

#endif // CBASEMESH_H
