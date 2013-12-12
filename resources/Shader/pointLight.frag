#version 330

struct Light{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 attenuation;
};

layout(std140) uniform Lights{
	Light light[8];
} lights;

uniform uint numberOfLights;
uniform uint useTexture;
uniform sampler2D theTexture;

layout(std140) uniform Material{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
} material;

smooth in vec3 vPosition;
smooth in vec3 vNormal;
smooth in vec2 vTexCoords;

out vec4 vFragmentColor;

vec4 pointLight (int lightID, vec4 matDiffuse)
{
  float nDotVP;       // normal * light direction
  float nDotR;        // normal * light reflection vector
  float pf;           // power factor
  float attenuation;  // computed attenuation factor
  float d;            // distance from surface to light position
  vec3 VP;            // direction from surface to light position
  vec3 reflection;    // direction of maximum highlights
  
  // Compute vector from surface to light position
  VP = vec3 (lights.light[lightID].position) - vPosition;
  
  // Compute distance between surface and light position
  d = length (VP);
  
  // Normalize the vector from surface to light position
  VP = normalize (VP);
  
  // Compute attenuation
  attenuation = 1.f / (lights.light[lightID].attenuation.x +
  lights.light[lightID].attenuation.y * d +
  lights.light[lightID].attenuation.z * d * d);
  
  
  reflection = normalize (reflect (-normalize (VP), normalize
  (vNormal)));
  
  nDotVP = max (0.f, dot (vNormal, VP));
  nDotR = max (0.f, dot (normalize (vNormal), reflection));
  
  if (nDotVP == 0.f)
    pf = 0.f;
  else
    pf = pow (nDotR, material.shininess);
  
  
  vec4 ambient = material.ambient * lights.light[lightID].ambient * attenuation;
  vec4 diffuse = /*material.diffuse*/matDiffuse * lights.light[lightID].diffuse * nDotVP * attenuation;
  vec4 specular = material.specular * lights.light[lightID].specular * pf * attenuation;
  
  return ambient + diffuse + specular;
  
}


vec4 calcPointLight( int lightID, vec4 matDiffuse){	
	// vector from surface to light
	vec3 directionVertexLight = vec3( lights.light[lightID].position) - vPosition;
	
	// distance from surface to light
	float distanceSurfaceLight = length( directionVertexLight);
	
	directionVertexLight = normalize( directionVertexLight);
	
	float attenuation = 1.0f / ( 
		lights.light[ lightID].attenuation.x +
		lights.light[ lightID].attenuation.y * distanceSurfaceLight + 
		lights.light[ lightID].attenuation.z * distanceSurfaceLight * distanceSurfaceLight
	);
	
	vec3 reflection = normalize( reflect( -directionVertexLight, normalize( vNormal)));

	float normalLightDirection = max( 0.0f, dot( vNormal, directionVertexLight));
	float normalLightReflection = max( 0.0f, dot( vNormal, reflection));
	
	float powerFactor = 0.0f;
	if( normalLightDirection != 0.0f){
		powerFactor = pow( normalLightReflection, material.shininess);
	}
	
	// ambient + diffuse + specular
	return 
		material.ambient * lights.light[ lightID].ambient * attenuation +
		/*material.diffuse*/matDiffuse * lights.light[ lightID].diffuse * normalLightDirection * attenuation +
		material.specular * lights.light[ lightID].specular * powerFactor * attenuation;
}

// 	vec4 position;
// 	vec4 ambient;
// 	vec4 diffuse;
// 	vec4 specular;
// 	vec3 attenuation;

void main(){
	vFragmentColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 matDiffuse = material.diffuse;
	if( useTexture != uint(0)){
		matDiffuse = texture(theTexture, vTexCoords);
	}
	if( numberOfLights > uint(0)){
		for( int i = 0; uint(i) < numberOfLights; ++i){
			vFragmentColor = vFragmentColor + calcPointLight( i, matDiffuse); 
		}
	}else{
		vFragmentColor = matDiffuse;
	}
// 	vFragmentColor = calcPointLight(1);
// 	vFragmentColor = vec4(  -9.40647, 5.83218, 1.73059, 1.0);
// 	vFragmentColor = vec4( lights.light[0].position);
// 	vFragmentColor = vec4(16.8787 , 12.0265 , 11.7787 , 1.0  );

// 	vFragmentColor = vec4( material.diffuse);
// 	vFragmentColor = vec4( lights.light[0].position.xyz , 1.0);
// 	vFragmentColor = vec4(vPosition,1.0);
}
