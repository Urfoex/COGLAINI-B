#include "cgame.h"
#include <iostream>

#include "headers_with_gcc_warnings/glm.hpp"
#include <stdexcept>
#include "cni.h"

// #include <glm/glm.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

int main() {
	CNI::Init(9,9);
	CNI::first();
	
	try {
		CGame g;
		g.init();
		g.run();
	} catch( std::runtime_error re ) {
		std::cerr << "(EE) Error: " << re.what() << std::endl;
	} catch( std::string s ) {
		std::cerr << "(EE) Error: " << s << std::endl;
	} catch( char const*  s ) {
		std::cerr << "(EE) Error: " << s << std::endl;
	}
	
	
	CNI::Destroy();
	
	return 0;
}
