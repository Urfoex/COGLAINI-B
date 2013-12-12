#ifndef _ANIMATOR_H
#define _ANIMATOR_H

#include <vector>
#include <map>
#include "headers_with_gcc_warnings/assimp.h"
//#include <assimp.hpp>
#include "headers_with_gcc_warnings/aiScene.h"
#include <aiPostProcess.h>
#include "headers_with_gcc_warnings/glm.hpp"

class CScene;
struct TAnimationNode;

class CAnimator {
public:
	CAnimator( const aiScene* pScene, unsigned int pAnimIndex );
	~CAnimator();

	CAnimator( const CAnimator& );
	CAnimator& operator=( const CAnimator& );

	void SetAnimationIndex( unsigned int uAnimationIndex );
	void UpdateAnimation( float fDeltaTime, int iTicksPerSecond );

	const std::vector<aiMatrix4x4>& GetBoneMatrices( const aiNode* pNode, unsigned int pMeshIndex = 0 );
	const aiMatrix4x4& GetGlobalTransform( const aiNode* node ) const;

private:
	TAnimationNode* CreateNodeTree( aiNode* pNode, TAnimationNode* pParent );
	void CalculateGlobalTransform( TAnimationNode* pInternalNode );
	void UpdateTransforms( TAnimationNode* pNode, const std::vector<aiMatrix4x4>& vTransforms );

	typedef std::map<const aiNode*, TAnimationNode*> NodeMap;
	NodeMap m_mapNodesByName;

	typedef std::map<const char*, const aiNode*> BoneMap;
	BoneMap m_mapBoneNodesByName;

	double m_dLastTime;
	glm::uvec3* m_pLastFramePosition;

	std::vector<aiMatrix4x4> m_vTransforms;

	aiMatrix4x4 m_mIdentityMatrix;

	const aiScene* m_pScene;
	TAnimationNode* m_pRootNode;
	unsigned int m_uCurrentAnimationIndex;
	const aiAnimation* m_pCurrentAnimation;

	float m_lElapsedTime;
};

#endif
