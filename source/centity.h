#ifndef _ENTITY_H
#define _ENTITY_H

#include <cstdint>
#include "headers_with_gcc_warnings/glm.hpp"
#include <string>
#include <memory>
#include "headers_with_gcc_warnings/type_ptr.hpp"
#include "clogerrorstream.h"
#include "headers_with_gcc_warnings/btBulletDynamicsCommon.h"
#include <memory>

class aiMatrix4x4;
class CGame;

class CScene;
class CMesh;
class aiNode;

class CEntity {
public:
	CEntity( CScene* parentScene, std::string pcEntityName );
	CEntity( const CEntity& entity );
	void operator=( const CEntity& );
	virtual ~CEntity();

	virtual void Initialize( );
	virtual void Initialize( aiNode* node );
	virtual void Destroy();
	virtual void SetEmpty();

	virtual void Update( float deltaTime );
	virtual void Draw( float deltaTime );

	void Translate( glm::vec3& deltaMove );
	void Translate( float posX, float posY, float posZ );

	void Rotate( glm::vec3& deltaRot );
	void Rotate( float rotX, float rotY, float rotZ );

	void SetPosition( glm::vec3& newPos );
	glm::vec3 GetPosition();

	void SetRotation( glm::vec3& newRot );
	glm::vec3 GetRotation();

	void SetVisible( bool isVisible );
	bool IsVisible();

	bool IsAlive();

	int64_t GetID();
	const std::string GetName();

	virtual void createModelMatrix( aiNode* node );
protected:
	void SetAlive( bool isAlive );

private:
	void applyAdditionalMatrix( aiMatrix4x4& transform );
	static uint64_t m_nextFreeID;

protected:

// private:
	glm::vec3 m_vecPosition;
	glm::vec3 m_vecRotation;

	bool m_bIsVisible;
	bool m_bIsAlive;

	CScene* m_pParentScene;
	aiNode* m_pContentNode;

	int64_t m_iID;
	const std::string m_pcName;
	std::string m_NameByNode;

	glm::mat4 m_modelMatrix;
	glm::mat4 m_originalModelMatrix;
	CLogErrorStream m_logError;

	// physics
public:
protected:
	virtual void bindToPhysics( float mass = 0.0f ) = 0;
	
	btRigidBody* m_rigidBody; // for the whole entity
	virtual btCollisionShape* getCollisionShape();
	btVector3 m_lastPosition;
};

#endif
