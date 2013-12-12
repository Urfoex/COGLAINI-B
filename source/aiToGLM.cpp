#include "headers_with_gcc_warnings/glm.hpp"
#include "headers_with_gcc_warnings/aiCamera.h"
#include <iostream>
#include "stdint.h"

inline glm::mat4 aiToGLMM4( const aiMatrix4x4& mat ) {
	return glm::mat4(
			   mat.a1, mat.a2, mat.a3, mat.a4,
			   mat.b1, mat.b2, mat.b3, mat.b4,
			   mat.c1, mat.c2, mat.c3, mat.c4,
			   mat.d1, mat.d2, mat.d3, mat.d4
		   );
}

inline void print( const aiMatrix4x4& mat, std::ostream& out ) {
	out << mat.a1 << " " << mat.a2 << " " << mat.a3 << " " << mat.a4 << "\n";
	out << mat.b1 << " " << mat.b2 << " " << mat.b3 << " " << mat.b4 << "\n";
	out << mat.c1 << " " << mat.c2 << " " << mat.c3 << " " << mat.c4 << "\n";
	out << mat.d1 << " " << mat.d2 << " " << mat.d3 << " " << mat.d4 << "\n";
}

inline void print( const glm::mat4& mat, std::ostream& out ) {
	for( uint16_t i = 0; i < 4; ++i ) {
		out << mat[i].x << " " << mat[i].y << " " << mat[i].z << " " << mat[i].w << "\n";
	}
}


inline std::ostream& print( const glm::vec3& vec, std::ostream& out ) {
	out << vec.x << " " << vec.y << " " << vec.z << "\n";
	return out;
}

inline std::ostream& print( const glm::vec4& vec, std::ostream& out ) {
	out << vec.r << " " << vec.g << " " << vec.b << " " << vec.a << "\n";
	return out;
}

inline std::ostream& print( const aiColor3D& vec, std::ostream& out ) {
	out << vec.r << " " << vec.g << " " << vec.b << "\n";
	return out;
}

inline std::ostream& print( const aiVector3D& vec, std::ostream& out ) {
	out << vec.x << " " << vec.y << " " << vec.z << "\n";
	return out;
}

inline glm::vec4 aiToGLMV4( const aiColor4D& vec ) {
	return glm::vec4(
			   vec.r, vec.g, vec.b, vec.a
		   );
}

inline glm::vec3 aiToGLMV3( const aiVector3D& vec ) {
	return glm::vec3(
			   vec.x, vec.y, vec.z
		   );
}
