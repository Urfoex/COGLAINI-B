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


#ifndef CANIMATEDMESH_H
#define CANIMATEDMESH_H

#include "cmesh.h"


class CAnimatedMesh : public CMesh {

public:
	CAnimatedMesh( CScene* parentScene, aiMesh* mesh, uint64_t aiID );
	virtual ~CAnimatedMesh();
	virtual void Update( float deltaTime );

	static const double m_gDEFAULT_ANIMATION_TICKS_PER_SECOND;
	
public: // physics
	virtual void bindToPhysics( float mass = 0.0f ){};
};

#endif // CANIMATEDMESH_H
