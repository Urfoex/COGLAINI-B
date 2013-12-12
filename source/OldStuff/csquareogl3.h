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


#ifndef CSQUAREOGL3_H
#define CSQUAREOGL3_H

#include "SFML_OpenGL.h"
#include "cshaderprogram.h"
#include "idrawableobjectogl3.h"
#include <memory>

class CSquareOGL3 : public IDrawableObjectOGL3 {

public:
	CSquareOGL3( CGame* game );
// 	CGraphicObject( const CGraphicObject& other );
	virtual ~CSquareOGL3();
// 	virtual CGraphicObject& operator=( const CGraphicObject& other );
// 	virtual bool operator==( const CGraphicObject& other ) const;
	virtual void transform();
	virtual void draw();

private:

	virtual void createShader();
	virtual void createObject();

};

#endif // CSQUAREOGL3_H
