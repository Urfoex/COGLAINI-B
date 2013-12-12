#ifndef _ANIMATIONNODE_H_
#define _ANIMATIONNODE_H_

#include <assimp.h>
#include <vector>

//A little tree structure to match the scene's node structure, but holding additional data.
struct TAnimationNode {
	std::string sName;
	TAnimationNode* pParent;
	std::vector<TAnimationNode*> vChildren;

	//most recently calculated local transform
	aiMatrix4x4 mLocalTransform;

	//same, but in world space
	aiMatrix4x4 mGlobalTransform;

	//index in the current animation's channel array.
	unsigned int uChannelIndex;

	//default construction
	TAnimationNode() :
		sName( "" ),
		pParent( nullptr ),
		vChildren(),
		mLocalTransform(),
		mGlobalTransform(),
		uChannelIndex( -1 )
	{}

	TAnimationNode( const TAnimationNode& );
	TAnimationNode& operator=( const TAnimationNode& );

	//construction from a given name
	TAnimationNode( const std::string& pName ):
		sName( pName ),
		pParent( nullptr ),
		vChildren(),
		mLocalTransform(),
		mGlobalTransform(),
		uChannelIndex( -1 )
	{}

	~TAnimationNode() {
		for( std::vector<TAnimationNode*>::iterator it = vChildren.begin(); it != vChildren.end(); it++ ) {
			delete *it;
		}
	}
};

#endif
