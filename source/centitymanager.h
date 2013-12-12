#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_

#include <vector>
#include <memory>
#include "clogerrorstream.h"

class CEntity;

class CEntityManager {
public:
	CEntityManager();
	~CEntityManager();

	void Initialize();
	void Destroy();

	void UpdateEntites( float deltaTime );
	void DrawEntities( float deltaTime );

	void RegisterEntity( CEntity* pxEntity );
	void UnRegisterEntity( CEntity* pxEntity );

private:
	CLogErrorStream m_logError;
	std::vector<CEntity*> m_vEntities;
	// Stores all entities which has been destroyed in this frame
	// and should be removed in the next frame update
	std::vector<CEntity*> m_vKilledEntities;
};

#endif
