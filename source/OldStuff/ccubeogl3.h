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


#ifndef CCUBEOGL3_H
#define CCUBEOGL3_H

#include "idrawableobjectogl3.h"
#include "cshaderprogram.h"
#include <memory>

class CCubeOGL3 : public IDrawableObjectOGL3 {

public:
	CCubeOGL3( CGame* game );
	virtual void transform();
	virtual ~CCubeOGL3();

private:

	virtual void createShader();
	virtual void createObject();

};

#endif // CCUBEOGL3_H
