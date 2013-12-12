#version 330

// attributes per position:
// - position
// - normal
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoords;
layout (location = 3) in vec4 boneWeights;
layout (location = 4) in ivec4 boneIDs;

// uniforms
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform uint useTexture;
uniform sampler2D theTexture;
/*layout(std140) */uniform mat4 boneMatrix[100];
uniform uint useBones;

// out to fragment shader
smooth out vec3 vPosition;
smooth out vec3 vNormal;
smooth out vec2 vTexCoords;

void main( ){
	vec4 newPosition = vec4( position, 1.0);
	vec4 newNormal = vec4( normal, 1.0);
	
	if( useBones != uint(0)){
		vec4 myBoneWeights = boneWeights;
		myBoneWeights.w = 1.0 - dot(myBoneWeights.xyz, vec3(1.0, 1.0, 1.0));
		
		vec4 inPosition = vec4(position, 1.0);

		newPosition = myBoneWeights.x * boneMatrix[boneIDs.x] * inPosition;
		newPosition += myBoneWeights.y * boneMatrix[boneIDs.y] * inPosition;
		newPosition += myBoneWeights.z * boneMatrix[boneIDs.z] * inPosition;
		newPosition += myBoneWeights.w * boneMatrix[boneIDs.w] * inPosition;

		vec4 myNormal = vec4(normal, 0.0);

		newNormal = myBoneWeights.x * boneMatrix[boneIDs.x] * myNormal;
		newNormal += myBoneWeights.y * boneMatrix[boneIDs.y] * myNormal;
		newNormal += myBoneWeights.z * boneMatrix[boneIDs.z] * myNormal;
		newNormal += myBoneWeights.w * boneMatrix[boneIDs.w] * myNormal;
	}
	// surface normal in eye coords
// 	vNormal = normalMatrix * normal;
	vNormal = normalMatrix * newNormal.xyz;
	
	// vertex pos in eye coords
// 	vec4 vertPos = modelMatrix * vec4( position, 1.0);
	vec4 vertPos = modelMatrix * newPosition;
	vec3 vertEyePos = vertPos.xyz/ vertPos.w;
	
	vPosition = vertEyePos;
	vTexCoords = texcoords;
	
	// transform geometry
// 	gl_Position = modelViewProjectionMatrix * vec4( position, 1.0);
	gl_Position = modelViewProjectionMatrix * newPosition;
}
