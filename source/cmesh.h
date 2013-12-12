#ifndef _MESH_H_
#define _MESH_H_
#include <cstdint>
#include <memory>
#include <array>
#include "centity.h"
#include "headers_with_gcc_warnings/aiMesh.h"

#include "cshaderprogram.h"

class CScene;
class CGame;
struct aiScene;
class aiNode;
struct aiMesh;
class aiMaterial;

class CMesh : public CEntity {
public:
	CMesh( CScene* parentScene, aiMesh* mesh, uint64_t aiID );
	CMesh( CScene* parentScene, glm::core::type::vec3 startPos );
	void operator=( const CMesh& );
	~CMesh();

	virtual void Initialize( aiMaterial** materials );
	virtual void Update( float deltaTime );
	virtual void Draw( float deltaTime );
	virtual void Destroy();


	uint64_t getAIID();
	void setNodeID( unsigned int nodeID ) {
		m_nodeID = nodeID;
	};

protected:
	CMesh( CScene* parentScene, glm::vec3 startPos, glm::vec3 force);
	virtual void Initialize();
	
	virtual bool useTexture();
// TODO: 	refactor for using same stuff on different instances
// 			e.g. shared_ptr, friend class?
// TODO:	same mesh for different entities == instances?
private:
	void ApplyMaterial( const struct aiMaterial* mtl );
	void set_float4( float f[4], float a, float b, float c, float d );
	void color4_to_float4( const struct aiColor4D* c, float f[4] );
protected:
	virtual void createShader();
	void debugOutput();

protected:
	virtual void createVerticeAttributes();
	virtual std::unique_ptr<GLuint[]> createVertixIndex();
	void createColors( aiMaterial** materials );
	void setMaterial();
	void createMaterial();
	void loadTextures( aiMaterial** materials );
	void loadDiffuseTexture( aiMaterial* material );
	std::unique_ptr< std::vector< aiVertexWeight >[] > createVectorBoneStructur();
	std::unique_ptr< GLbyte[] > createArrayOfVerticeAttributes();
	void putVerticeAttributesToGPU();
	std::unique_ptr< glm::mat4[]> createBoneMats();
	void createNormalMatrix();

	aiMesh* m_mesh;

	GLuint m_vertexArrayObjectID;
	GLuint m_vertexBufferObjectID;
	GLuint m_vertexBufferIndexID;
	GLuint m_numberOfElements;
	GLuint m_drawType;

	std::shared_ptr<CVertexShader> m_vs;
	std::shared_ptr<CFragmentShader> m_fs;
	std::shared_ptr<CShaderProgram> m_sp;

	glm::vec4 m_colorDiffuse;
	glm::vec4 m_colorSpecular;
	glm::vec4 m_colorAmbient;
	GLfloat m_colorShininess;
	GLuint m_materialUBOBuffer;
	GLuint m_materialUniformBlockBufferIndex;
	GLuint m_materialUniformBlockBinding;

	const uint64_t m_aiID;
	unsigned int m_nodeID;

	GLuint m_diffuseTexture;
	GLboolean m_useTexture;
	static const unsigned int m_gMaxBoneMatrixs;

	std::unique_ptr<VertexAttributes> m_vertexAttributes;

	glm::mat3 m_normalMatrix;
	std::unique_ptr<glm::mat4[]> m_boneMatrices;
	
	bool m_hasBones;
	unsigned m_numBones;

	// physics
// 	virtual btCollisionShape* getCollisionShape();
// 	void getBoundingBox(struct aiVector3D* min, struct aiVector3D* max);
public:
// 	void setContentNode(aiNode* node);
	virtual void bindToPhysics( float mass = 0.1f );
	virtual btCollisionShape* getCollisionShape();
	
protected:
	
};

#endif
