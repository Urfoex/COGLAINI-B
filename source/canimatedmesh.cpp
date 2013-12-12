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


#include "canimatedmesh.h"
#include "cscene.h"

const double CAnimatedMesh::m_gDEFAULT_ANIMATION_TICKS_PER_SECOND = 1.0;

CAnimatedMesh::CAnimatedMesh( CScene* parentScene, aiMesh* mesh, uint64_t aiID ): CMesh( parentScene, mesh, aiID ) {
	m_logError.logStream() << "(II) Being an animated mesh." << std::endl;
}

CAnimatedMesh::~CAnimatedMesh() {

}

void CAnimatedMesh::Update( float deltaTime ) {
	CMesh::Update( deltaTime );

	if( m_pParentScene->getAnimator() != nullptr ) {
		m_pParentScene->getAnimator()->UpdateAnimation( deltaTime, m_gDEFAULT_ANIMATION_TICKS_PER_SECOND );
	}
}
