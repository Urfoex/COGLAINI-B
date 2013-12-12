#ifndef _SCENE_H
#define _SCENE_H

#include "headers_with_gcc_warnings/glm.hpp"
#include <string>
#include <cstdint>
#include <map>
#include <memory>
#include "ccamera.h"
#include "clogerrorstream.h"
#include "centitymanager.h"
#include "SFML_OpenGL.h"
#include "canimator.h"
#include <cstdint>
#include "headers_with_gcc_warnings/btBulletDynamicsCommon.h"
#include "cshaderprogram.h"
#include "GL_ShapeDrawer.h"
#include <SFML/Audio.hpp>

#include "cphysics.h"

struct SUniformLight;
class CEntityLight;
class aiMaterial;
class CMesh;
struct aiScene;

class CScene {
public:
	CScene( uint64_t ID, std::string& filename );
	CScene( const CScene& );
	void operator=( const CScene& );
	~CScene( void );

	void Initialize();
	void Destroy();

	void Update( float deltaTime );
	void Draw( float deltaTime );

	// TLightType as additional parameter?
	int AddLight( glm::vec3& pos, glm::vec3& color, float intensity );

	void SetCameraPosition( glm::vec3& camPos );
	void SetCameraDirection( glm::vec3& camDir );
	CCamera* getCurrentCamera();
	void setCurrentCameraByID( uint64_t id );
	void setCurrentCameraByName( std::string name );
	void setNextCamera();

	std::shared_ptr<btCollisionDispatcher> PhysicsDispatcher() const { return m_physics->Dispatcher() ; }

	const aiScene* GetContentScene() {
		return m_ContentScene;
	}

	CEntityManager& GetEntityManager() {
		return m_EntityManager;
	}

	std::string getName();
	uint64_t getID();
	aiMaterial* getMaterial( uint64_t id );
	void togglePolygonMode();
	CAnimator* getAnimator() {
		return m_animator.get();
	};

private:
	CLogErrorStream m_logError;
	glm::vec3 m_pMainCameraPos;
	glm::vec3 m_pMainCameraDir;

	CEntityManager m_EntityManager;

	aiScene* m_ContentScene;

	std::string m_Name;
	uint64_t m_ID;

	std::map<uint64_t, CCamera*> m_cameraByID;
	std::map<std::string, CCamera*> m_cameraByName;
	std::map<uint64_t, CCamera*>::iterator m_currentCameraIterator;

	std::map< uint64_t, CMesh*> m_meshByID;
	std::map< uint64_t, CMesh*> m_meshByAIID;
	std::map< std::string, CMesh*> m_meshByName;

	std::map< uint64_t, CEntityLight*> m_lighthByID;
	std::map< std::string, CEntityLight*> m_lightByName;
	SUniformLight** m_uniformLightsArray;
	GLuint m_sizeOfUniformLightsArray;

	GLint m_polygonMode;

	GLuint m_lightUBOBuffer;
	GLuint m_lightUniformBlockBinding;
	GLuint m_numOfActiveLights;
	const GLuint m_maxNumOfLights;

	std::unique_ptr<CAnimator> m_animator;

	void initMeshes( );
	void recursiveInitMeshes( aiNode* node );
	void printSceneDebug();

	void makeLightsArray();
	void putLightsIntoBuffer( GLint program );
	void putLightsIntoBuffer();
	void setGlobalUniforms();
	void setGlobalUniforms( GLuint shaderprogram );
	void initOwn();

	void putLightIntoBuffer( SUniformLight* cel, unsigned int num );



public:	// physics

	void togglePhysicDebugDraw();
	void togglePhysicDebugWireframe();

	btRigidBody* addRigidBody( btCollisionShape* shape, const btTransform& startTransform, btScalar mass = 0.0f );
	
	void shoot();

private:
	std::unique_ptr<CPhysics> m_physics;
	GLint m_physicDebugWireframe;
	bool m_physicDebugDraw;
	
	void stackOfCubes();
	
private: //sound
	sf::SoundBuffer m_buffer;
	std::unique_ptr< sf::Sound> m_sound;
	bool m_hasSound;
	
};


#endif
