#ifndef _PHYSICS_H
#define _PHYSICS_H

#include "cshaderprogram.h"
#include <memory>
#include "headers_with_gcc_warnings/btBulletDynamicsCommon.h"
#include "GL_ShapeDrawer.h"

class CPhysics {
public:
	CPhysics( void );
	virtual ~CPhysics( void );
	void Initialize();
	void renderPhysicsScene( glm::mat4 projectionViewMatrix );
	void toggleRenderPhysics();
	GLuint& getPhysicsShaderProgram();
	int stepSimulation( btScalar timeStep = 1.0f / 60.0f, int maxSubsteps = 10 );

	btRigidBody* addRigidBody( btCollisionShape* shape, const btTransform& startTransform, btScalar mass = 5.0f );
	std::shared_ptr<btCollisionDispatcher> Dispatcher() const { return m_dispatcher; }

protected:
	CLogErrorStream m_logError; // todo remove?
	bool bRenderPhysics;

	std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
	std::unique_ptr<btBroadphaseInterface> m_broadphase;
	std::shared_ptr<btCollisionDispatcher> m_dispatcher;
	std::unique_ptr<btConstraintSolver> m_solver;
	std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
	std::unique_ptr<GL_ShapeDrawer> m_shapeDrawer;

	std::shared_ptr<CShaderProgram> m_physicsShader;

	void initPhysics();
	void cleanPhysics();

	void initPhysicsShader();

	void createStackOfCubes();

};

#endif
