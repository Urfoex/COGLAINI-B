#include "centitymanager.h"
#include "centity.h"
#include <algorithm>

CEntityManager::CEntityManager():
	m_logError(),
	m_vEntities(),
	m_vKilledEntities() {
}

CEntityManager::~CEntityManager() {
}

void CEntityManager::Initialize() {
	m_vEntities.clear();
	m_vKilledEntities.clear();
}

void CEntityManager::UpdateEntites( float deltaTime ) {
	// First delete all entities which has been killed in the last frame
//    for (unsigned int i=0; i<m_vKilledEntities.size(); ++i)
//    {
//       int posFound = -1;
// 	  for (unsigned int j=0; j<m_vEntities.size(); ++j)
//       {
//          if(m_vEntities[j]->GetID() == m_vKilledEntities[i]->GetID())
//          {
//             posFound = i;
//             break;
//          }
//       }
//
//       if(posFound > -1)
//       {
// 		  (m_vEntities.begin()+posFound)->get()->SetEmpty();
//          m_vEntities.erase (m_vEntities.begin()+posFound);
//       }
//    }

	std::vector <CEntity* >& mvE = m_vEntities;
	std::for_each( m_vKilledEntities.begin(), m_vKilledEntities.end(), [&mvE]( CEntity * pCE ) {
		std::vector<CEntity*>::iterator it = std::find( mvE.begin(), mvE.end(), pCE );
		mvE.erase( it );
		( *it )->SetEmpty();
		delete *it;
	} );
	m_vKilledEntities.clear();


	// Update all alive entities
//    for(unsigned int id=0; id<m_vEntities.size(); ++id)
//    {
//       m_vEntities[id]->Update(deltaTime);
//    }
	std::for_each( m_vEntities.begin(), m_vEntities.end(), [&deltaTime]( CEntity * pCE ) {
		pCE->Update( deltaTime );
	} );
}

void CEntityManager::DrawEntities( float deltaTime ) {
	std::for_each( m_vEntities.begin(), m_vEntities.end(), [&deltaTime]( CEntity * entity ) {
		entity->Draw( deltaTime );
	} );
}

void CEntityManager::Destroy() {
	m_logError.logStream() << "Removing " << m_vEntities.size() << " + " << m_vKilledEntities.size() << " entities.\n";
	std::for_each( m_vEntities.begin(), m_vEntities.end(), []( CEntity * entity ) {
		entity->Destroy();
	} );
	m_vEntities.clear();

	m_logError.logStream() << "Removing " << m_vEntities.size() << " + " << m_vKilledEntities.size() << " entities.\n";

	std::for_each( m_vKilledEntities.begin(), m_vKilledEntities.end(), []( CEntity * entity ) {
// 		entity->Destroy();
		entity->SetEmpty();
		delete entity;
	} );
	m_vKilledEntities.clear();
}

void CEntityManager::RegisterEntity( CEntity* pxEntity ) {
	m_vEntities.push_back( pxEntity );
}

void CEntityManager::UnRegisterEntity( CEntity* pxEntity ) {
	// Don�t kill the entity directly because it�s possible
	// that the physics or something works at the moment with it
	// Destroy this entity at the beginning of the next frame update!
	m_vKilledEntities.push_back( pxEntity );
}
