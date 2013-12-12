#include "SFML_OpenGL.h"
#include <ostream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

namespace GL {
	void checkGLError( std::ostream& stream, std::string s ) {
		if( DEBUG_GL ) {
			GLenum e = glGetError();

			if( e != GL_NO_ERROR ) {
				stream << "\n(EE) glError: " << e << std::endl;
				stream << "\n(EE) glError: " << s << std::endl;

				switch( e ) {
				case GL_NO_ERROR:
					stream << "(EE) glError: GL_NO_ERROR\n";
					break;
				case GL_INVALID_ENUM:
					stream << "(EE) glError: GL_INVALID_ENUM\n";
					break;
				case GL_INVALID_VALUE:
					stream << "(EE) glError: GL_INVALID_VALUE\n";
					break;
				case GL_INVALID_OPERATION:
					stream << "(EE) glError: GL_INVALID_OPERATION\n";
					break;
				case GL_STACK_OVERFLOW:
					stream << "(EE) glError: GL_STACK_OVERFLOW\n";
					break;
				case GL_STACK_UNDERFLOW:
					stream << "(EE) glError: GL_STACK_UNDERFLOW\n";
					break;
				case GL_OUT_OF_MEMORY:
					stream << "(EE) glError: GL_OUT_OF_MEMORY\n";
					break;
				default:
					stream << "(EE) glError: UNKNOWN\n";
					break;
				}

				stream.flush();
				throw( s );
			}
		}
	}

	void printUniformBlockInfo( GLuint prog, GLint block_index ) {
		static const std::string indent( "(II)" );
		// Fetch uniform block name:
		GLint name_length = 0;
		glGetActiveUniformBlockiv( prog, block_index, GL_UNIFORM_BLOCK_NAME_LENGTH, &name_length );

		if( name_length == 0 ) {
			std::clog << indent << " No names found at: " << block_index << std::endl;
			return;
		}

		char* c = new char[ name_length + 1];
		glGetActiveUniformBlockName( prog, block_index, name_length, NULL, c );
		std::string block_name( c );
		delete[] c;

		// Fetch info on each active uniform:
		GLint active_uniforms = 0;
		glGetActiveUniformBlockiv( prog, block_index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &active_uniforms );

		std::vector<GLuint> uniform_indices( active_uniforms, 0 );
		glGetActiveUniformBlockiv( prog, block_index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, reinterpret_cast<GLint*>( &uniform_indices[0] ) );

		std::vector<GLint> name_lengths( uniform_indices.size(), 0 );
		glGetActiveUniformsiv( prog, uniform_indices.size(), &uniform_indices[0], GL_UNIFORM_NAME_LENGTH, &name_lengths[0] );

		std::vector<GLint> offsets( uniform_indices.size(), 0 );
		glGetActiveUniformsiv( prog, uniform_indices.size(), &uniform_indices[0], GL_UNIFORM_OFFSET, &offsets[0] );

		std::vector<GLint> types( uniform_indices.size(), 0 );
		glGetActiveUniformsiv( prog, uniform_indices.size(), &uniform_indices[0], GL_UNIFORM_TYPE, &types[0] );

		std::vector<GLint> sizes( uniform_indices.size(), 0 );
		glGetActiveUniformsiv( prog, uniform_indices.size(), &uniform_indices[0], GL_UNIFORM_SIZE, &sizes[0] );

		std::vector<GLint> strides( uniform_indices.size(), 0 );
		glGetActiveUniformsiv( prog, uniform_indices.size(), &uniform_indices[0], GL_UNIFORM_ARRAY_STRIDE, &strides[0] );

		// Build a string detailing each uniform in the block:
		std::vector<std::string> uniform_details;
		uniform_details.reserve( uniform_indices.size() );

		for( std::size_t i = 0; i < uniform_indices.size(); ++i ) {
			GLuint const uniform_index = uniform_indices[i];
			char* c = new char[name_lengths[i] + 1];
			glGetActiveUniformName( prog, uniform_index, name_lengths[i], NULL, c );

			std::string name( c );
			delete[] c;

			std::ostringstream details;
			details << std::setfill( '0' ) << std::setw( 4 ) << offsets[i] << ": " << std::setfill( ' ' ) << std::setw( 5 ) << ( types[i] ) << " " << name;

			if( sizes[i] > 1 ) {
				details << "[" << sizes[i] << "]";
			}

			details << "\n";
			uniform_details.push_back( details.str() );
		}

		// Sort uniform detail string alphabetically. (Since the detail strings
		// start with the uniform's byte offset, this will order the uniforms in
		// the order they are laid out in memory:
		std::sort( uniform_details.begin(), uniform_details.end() );

		// Output details:
		std::clog << indent << "Uniform block \"" << block_name << "\":\n";

		for( auto detail = uniform_details.begin(); detail != uniform_details.end(); ++detail ) {
			std::clog << indent << "  " << *detail;
		}
	}

}
