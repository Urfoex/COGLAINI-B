////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef SFML_OPENGL_HPP
#define SFML_OPENGL_HPP

#define GL_GLEXT_PROTOTYPES 1

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>


////////////////////////////////////////////////////////////
/// This file just includes the OpenGL (GL and GLU) headers,
/// which have actually different paths on each system
////////////////////////////////////////////////////////////
#if defined(SFML_SYSTEM_WINDOWS)

// The Visual C++ version of gl.h uses WINGDIAPI and APIENTRY but doesn't define them
#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/gl.h>
//#include <GL/glu.h> // already included by glew
#include <GL/glext.h>

#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD)

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#elif defined(SFML_SYSTEM_MACOS)

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>

#endif

#include <string>
#include <sstream>

namespace GL {
#define checkGLErrorLF(stream) 				\
	{										\
		std::stringstream s;				\
		s << __FILE__ << " : " << __LINE__;	\
		GL::checkGLError( stream, s.str());	\
	}

	const bool DEBUG_GL = true;
	void checkGLError( std::ostream& stream, std::string s );
	void printUniformBlockInfo( GLuint prog, GLint block_index );
}

#endif // SFML_OPENGL_HPP
