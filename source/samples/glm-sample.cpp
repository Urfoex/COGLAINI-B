#if ( defined( __GNUC__ ) || defined( __GNUG__ ) )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Werror"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC diagnostic ignored "-Wswitch-default"
#endif

#include "../headers_with_gcc_warnings/glm.hpp"

struct triangle {
	glm::vec3 Position[3];
	glm::vec3 Normal;
};


void computeNormal( triangle& Triangle ) {
	glm::vec3 const& a = Triangle.Position[0];
	glm::vec3 const& b = Triangle.Position[1];
	glm::vec3 const& c = Triangle.Position[2];
	Triangle.Normal = glm::normalize( glm::cross( c - a, b - a ) );
}

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
// glm::perspective
#include "../../libraries/glm/gtc/matrix_transform.hpp"
// glm::translate, glm::rotate, glm::scale
// glm::value_ptr
#include "../../libraries/glm/gtc/type_ptr.hpp"
#include "../SFML_OpenGL.h"
void fun( float Translate, glm::vec2 Rotate, GLint LocationMVP ) {
	glm::mat4 Projection =
		glm::perspective( 45.0f, 4.0f / 3.0f, 0.1f, 100.f );
	glm::mat4 ViewTranslate = glm::translate(
								  glm::mat4( 1.0f ),
								  glm::vec3( 0.0f, 0.0f, -Translate ) );
	glm::mat4 ViewRotateX = glm::rotate(
								ViewTranslate,
								Rotate.y, glm::vec3( -1.0f, 0.0f, 0.0f ) );
	glm::mat4 View = glm::rotate(
						 ViewRotateX,
						 Rotate.x, glm::vec3( 0.0f, 1.0f, 0.0f ) );
	glm::mat4 Model = glm::scale(
						  glm::mat4( 1.0f ),
						  glm::vec3( 0.5f ) );
	glm::mat4 MVP = Projection * View * Model;
	glUniformMatrix4fv(	LocationMVP, 1, GL_FALSE, glm::value_ptr( MVP ) );
}


#include "../../libraries/glm/gtc/type_precision.hpp"

std::size_t const VertexCount = 4;
// Float quad geometry
std::size_t const PositionSizeF32 = VertexCount * sizeof( glm::vec2 );
glm::vec2 const PositionDataF32[VertexCount] = {
	glm::vec2( -1.0f, -1.0f ),
	glm::vec2( 1.0f, -1.0f ),
	glm::vec2( 1.0f, 1.0f ),
	glm::vec2( -1.0f, 1.0f )
};
// Half-float quad geometry
std::size_t const PositionSizeF16 = VertexCount * sizeof( glm::hvec2 );
glm::hvec2 const PositionDataF16[VertexCount] = {
	glm::hvec2( -1.0f, -1.0f ),
	glm::hvec2( 1.0f, -1.0f ),
	glm::hvec2( 1.0f, 1.0f ),
	glm::hvec2( -1.0f, 1.0f )
};
// 8 bits signed integer quad geometry
std::size_t const PositionSizeI8 = VertexCount * sizeof( glm::i8vec2 );
glm::i8vec2 const PositionDataI8[VertexCount] = {
	glm::i8vec2( -1, -1 ),
	glm::i8vec2( 1, -1 ),
	glm::i8vec2( 1, 1 ),
	glm::i8vec2( -1, 1 )
};
// 32 bits signed integer quad geometry
std::size_t const PositionSizeI32 = VertexCount * sizeof( glm::i32vec2 );
glm::i32vec2 const PositionDataI32[VertexCount] = {
	glm::i32vec2( -1, -1 ),
	glm::i32vec2( 1, -1 ),
	glm::i32vec2( 1, 1 ),
	glm::i32vec2( -1, 1 )
};

struct intersection {
	glm::vec3 normal() {
		return glm::vec3( 0.0f, 0.0f, 0.0f );
	};
	glm::vec3 globalPosition() {
		return glm::vec3( 0.0f, 0.0f, 0.0f );
	};
};

struct material {
	bool isDiffuse() {
		return true;
	};
	bool isSpecular() {
		return true;
	};
	glm::vec3 diffuse() {
		return glm::vec3( 0.0f, 0.0f, 0.0f );
	};
	glm::vec3 specular() {
		return glm::vec3( 0.0f, 0.0f, 0.0f );
	};
	float exponent() {
		return 2.0f;
	};
};

struct light {
	glm::vec3 color() {
		return glm::vec3( 0.0f, 0.0f, 0.0f );
	};
	glm::vec3 position() {
		return glm::vec3( 0.0f, 0.0f, 0.0f );
	};
	float inaccuracy() {
		return 0.0f;
	};
};

#include "../../libraries/glm/gtx/random.hpp"

bool shadow( glm::vec3 a, glm::vec3 b, glm::vec3 c ) {
	return true;
}

glm::vec3 lighting(
	intersection& Intersection,
	material& Material,
	light& Light,
	glm::vec3& View
) {
	glm::vec3 LightVector;
	glm::vec3 Color = glm::vec3( 0.0f );
	glm::vec3 LightVertor = glm::normalize(
								Light.position() - Intersection.globalPosition() +
								glm::vec3( glm::vecRand3( 0.0f, Light.inaccuracy() ) ) );

	if( !shadow(
				Intersection.globalPosition(),
				Light.position(),
				LightVertor ) ) {
		float Diffuse = glm::dot( Intersection.normal(), LightVector );

		if( Diffuse <= 0.0f )
			return Color;

		if( Material.isDiffuse() )
			Color += Light.color() * Material.diffuse() * Diffuse;

		if( Material.isSpecular() ) {
			glm::vec3 Reflect = glm::reflect(
									glm::normalize( -LightVector ),
									glm::normalize( Intersection.normal() ) );
			float Dot = glm::dot( Reflect, View );
			float Base = Dot > 0.0f ? Dot : 0.0f;
			float Specular = glm::pow( Base, Material.exponent() );
			Color += Material.specular() * Specular;
		}
	}

	return Color;
}

int main() {
	triangle t;
	computeNormal( t );
	fun( 1.0f, glm::vec2( 0.0f, 0.0f ), 1 );
	intersection i;
	material m;
	light l;
	glm::vec3 v;
	v = lighting( i, m, l, v );
	return 0;
}

#if ( defined( __GNUC__ ) || defined( __GNUG__ ) )
#pragma GCC diagnostic pop
#endif
