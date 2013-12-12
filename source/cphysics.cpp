#include "cphysics.h"

#include "headers_with_gcc_warnings/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "headers_with_gcc_warnings/type_ptr.hpp"

CPhysics::CPhysics( void ) : bRenderPhysics( true ) {

}


CPhysics::~CPhysics( void ) {
	cleanPhysics();
}

void CPhysics::Initialize() {
	initPhysics();
	initPhysicsShader();
}


void CPhysics::initPhysics() {
	m_logError.logStream() << "(II) Initialize bullet physics." << std::endl;
	///collision configuration contains default setup for memory, collision setup
	m_collisionConfiguration.reset( new btDefaultCollisionConfiguration() );
	//m_collisionConfiguration->setConvexConvexMultipointIterations();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_dispatcher.reset( new	btCollisionDispatcher( m_collisionConfiguration.get() ) );

	m_broadphase.reset( new btDbvtBroadphase() );

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	// 	btSequentialImpulseConstraintSolver* sol = ;
	m_solver.reset( new btSequentialImpulseConstraintSolver() );

	m_dynamicsWorld.reset( new btDiscreteDynamicsWorld( m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfiguration.get() ) );

	m_dynamicsWorld->setGravity( btVector3( 0, -10, 0 ) );

	m_shapeDrawer.reset( new GL_ShapeDrawer() );
	m_shapeDrawer->enableTexture( true );
}

void CPhysics::initPhysicsShader() {
	std::shared_ptr<CVertexShader> cvs = CVertexShader::CreateVertexShader( "../resources/Shader/physics.vert" );
	std::shared_ptr<CFragmentShader> cfs = CFragmentShader::CreateFragmentShader( "../resources/Shader/physics.frag" );
	m_physicsShader = CShaderProgram::CreateShaderProgram( cvs, cfs );
	m_physicsShader->createProgram();
}

// renders physics objects of the scene if bRenderPhysics
// param projectionViewMatrix note: don't use this directly but make a new var and assign it
// call with getCurrentCamera()->getProjectionViewMatrix();
void CPhysics::renderPhysicsScene( glm::mat4 projectionViewMatrix ) {
	if( !bRenderPhysics ) {
		return;
	}

	btScalar	m[16];
	btMatrix3x3	rot;
	btVector3 pos;
	btQuaternion quat;

	rot.setIdentity();
	const int	numObjects = m_dynamicsWorld->getNumCollisionObjects();
	// 	m_logError.logStream() << "(II) num PhysObjs: " << numObjects << std::endl;
	btVector3 wireColor( 1, 0, 0 );

	for( int i = 0; i < numObjects; i++ ) {
		btCollisionObject*	colObj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody*		body = btRigidBody::upcast( colObj );
		btTransform trans;

		//trans.setFromOpenGLMatrix( glm::value_ptr(getCurrentCamera()->getProjectionMatrix()));
		//trans.setFromOpenGLMatrix( glm::value_ptr(getCurrentCamera()->getViewMatrix()));
		//body->setWorldTransform( trans);
		if( body && body->getMotionState() ) {
			btDefaultMotionState* myMotionState = ( btDefaultMotionState* )body->getMotionState();
			myMotionState->m_graphicsWorldTrans.getOpenGLMatrix( m );
			rot = myMotionState->m_graphicsWorldTrans.getBasis();
			pos = myMotionState->m_graphicsWorldTrans.getOrigin();
			quat = myMotionState->m_graphicsWorldTrans.getRotation();
		} else {
			colObj->getWorldTransform().getOpenGLMatrix( m );
			rot = colObj->getWorldTransform().getBasis();
			pos = colObj->getWorldTransform().getOrigin();
			quat = colObj->getWorldTransform().getRotation();
		}

		btVector3 wireColor( 1.f, 1.0f, 0.5f ); //wants deactivation

		if( i & 1 )
			wireColor = btVector3( 0.f, 0.0f, 1.f );

		///color differently for active, sleeping, wantsdeactivation states
		if( colObj->getActivationState() == 1 ) { //active
			if( i & 1 ) {
				wireColor += btVector3( 1.f, 0.f, 0.f );
			} else {
				wireColor += btVector3( .5f, 0.f, 0.f );
			}
		}

		if( colObj->getActivationState() == 2 ) { //ISLAND_SLEEPING
			if( i & 1 ) {
				wireColor += btVector3( 0.f, 1.f, 0.f );
			} else {
				wireColor += btVector3( 0.f, 0.5f, 0.f );
			}
		}

		btVector3 aabbMin, aabbMax;
		m_dynamicsWorld->getBroadphase()->getBroadphaseAabb( aabbMin, aabbMax );

		aabbMin -= btVector3( BT_LARGE_FLOAT, BT_LARGE_FLOAT, BT_LARGE_FLOAT );
		aabbMax += btVector3( BT_LARGE_FLOAT, BT_LARGE_FLOAT, BT_LARGE_FLOAT );
		//		printf("aabbMin=(%f,%f,%f)\n",aabbMin.getX(),aabbMin.getY(),aabbMin.getZ());
		//		printf("aabbMax=(%f,%f,%f)\n",aabbMax.getX(),aabbMax.getY(),aabbMax.getZ());
		//m_dynamicsWorld->getDebugDrawer()->drawAabb(aabbMin,aabbMax,btVector3(1,1,1));


		//if (!(getDebugMode()& btIDebugDraw::DBG_DrawWireframe))
		//{
		/*case	0:*/
		GLint currentMatMode;
		glGetIntegerv( GL_MATRIX_MODE, &currentMatMode );
		glPushMatrix();
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glUseProgram( m_physicsShader->getShaderProgram() );
		GLint useTex = glGetUniformLocation( m_physicsShader->getShaderProgram(), "useTexture" );
		glUniform1ui( useTex, false );
		{
			GLint modelViewProjectionMatrixLocation = glGetUniformLocation( m_physicsShader->getShaderProgram(), "modelViewProjectionMatrix" );
			checkGLErrorLF( m_logError.errorStream() );
			glm::mat4 p;

			for( int ii = 0; ii < 4; ++ii ) {
				for( int jj = 0; jj < 4; ++jj ) {
					p[ii][jj] = m[ii * 4 + jj];
				}
			}

			glm::mat4 mvp = projectionViewMatrix;

// 			if( i == 1 )
// 				m_logError.logStream() << pos.getX() << " " << pos.getY() << " " << pos.getZ() << std::endl;

			mvp = glm::translate( mvp, glm::vec3( pos.getX(), pos.getY(), pos.getZ() ) );
			glm::mat4 mrot(	rot[0][0], rot[1][0], rot[2][0], 0,
							rot[0][1], rot[1][1], rot[2][1], 0,
							rot[0][2], rot[1][2], rot[2][2], 0,
							0, 0, 0, 1
						  );
			mvp = mvp * mrot;
			glUniformMatrix4fv( modelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr( mvp ) );
			checkGLErrorLF( m_logError.errorStream() );
		}
		m_shapeDrawer->drawOpenGL( m, colObj->getCollisionShape(), wireColor, 0/*btIDebugDraw::DBG_FastWireframe*//*btIDebugDraw::DBG_DrawWireframe*/, aabbMin, aabbMax );
		glMatrixMode( currentMatMode );
		glPopMatrix();
		//}


	}

	glDisable( GL_LIGHTING );
	glColor3f( 0, 1, 0 );

}

void CPhysics::cleanPhysics() {
	m_logError.logStream() << "(II) Clean bullet physics." << std::endl;
	//cleanup in the reverse order of creation/initialization

	//remove the rigidbodies from the dynamics world and delete them
	for( int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0 ; i-- ) {
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast( obj );

		if( body && body->getMotionState() ) {
			delete body->getMotionState();
		}

		m_dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	//delete collision shapes
	for( int j = 0; j < m_collisionShapes.size(); j++ ) {
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}

	m_collisionShapes.clear();

	// 	delete m_dynamicsWorld;
	//
	// 	delete m_solver;
	//
	// 	delete m_broadphase;
	//
	// 	delete m_dispatcher;
	//
	// 	delete m_collisionConfiguration;


	//m_dynamicsWorld->removeRigidBody(fallRigidBody);
	//delete fallRigidBody->getMotionState();
	//delete fallRigidBody;
	//
	//m_dynamicsWorld->removeRigidBody(groundRigidBody);
	//delete groundRigidBody->getMotionState();
	//delete groundRigidBody;
	//
	//
	//delete fallShape;
	//
	//delete groundShape;


	//delete m_dynamicsWorld;
	// to lazy to delete this (just 4 testing anyways)
	//delete solver;
	//delete collisionConfiguration;
	//delete dispatcher;
	//delete broadphase;


	// 	delete m_shapeDrawer;
}

void CPhysics::toggleRenderPhysics() {
	bRenderPhysics = !bRenderPhysics;
}

GLuint& CPhysics::getPhysicsShaderProgram() {
	return m_physicsShader->getShaderProgram();
}

int CPhysics::stepSimulation( btScalar timeStep, int maxSubsteps ) {
	return m_dynamicsWorld->stepSimulation( timeStep, maxSubsteps );
}

btRigidBody* CPhysics::addRigidBody( btCollisionShape* shape, const btTransform& startTransform, btScalar mass ) {
	m_collisionShapes.push_back( shape );

	//btTransform trans;
	//trans.setIdentity();
	//trans.setOrigin( btVector3( 0, 0, 0 ) );

	//We can also use DemoApplication::localCreateRigidBody, but for clarity it is provided here:
	{
		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = ( mass != 0.f );

		btVector3 localInertia( 0, 0, 0 );

		if( isDynamic )
 			shape->calculateLocalInertia( mass, localInertia );

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState( startTransform );
		btRigidBody::btRigidBodyConstructionInfo rbInfo( mass, myMotionState, shape, localInertia );
		btRigidBody* body = new btRigidBody( rbInfo );

		//add the body to the dynamics world
		m_dynamicsWorld->addRigidBody( body );

		return body;
	}
}
