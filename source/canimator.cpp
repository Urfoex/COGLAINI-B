#include "canimator.h"
#include "tanimationnode.h"
#include "cscene.h"

// static const float ANIMATION_TICKS_PER_SECOND = 20.0;
#define DEFAULT_ANIMATION_TICKS_PER_SECOND 20.0


CAnimator::CAnimator( const aiScene* pScene, unsigned int uAnimationIndex ):
	m_mapNodesByName(),
	m_mapBoneNodesByName(),
	m_dLastTime( 0.0 ),
	m_pLastFramePosition( nullptr ),
	m_vTransforms(),
	m_mIdentityMatrix( aiMatrix4x4() ),
	m_pScene( pScene ),
	m_pRootNode( nullptr ),
	m_uCurrentAnimationIndex( -1 ),
	m_pCurrentAnimation( nullptr ),
	m_lElapsedTime( 0.0f ) {

	//build the nodes-for-bones table
	for( unsigned int i = 0; i < m_pScene->mNumMeshes; i++ ) {
		const aiMesh* pCurrentMesh = m_pScene->mMeshes[i];

		for( unsigned int j = 0; j < pCurrentMesh->mNumBones; j++ ) {
			const aiBone* pCurrentBone = pCurrentMesh->mBones[j];
			m_mapBoneNodesByName[pCurrentBone->mName.data] = m_pScene->mRootNode->FindNode( pCurrentBone->mName );
		}
	}

	//changing the current animation also creates the node tree for this animation
	SetAnimationIndex( uAnimationIndex );
}

CAnimator::~CAnimator() {
	if( m_pRootNode != NULL ) {
		delete m_pRootNode;
	}

	if( m_pLastFramePosition != NULL ) {
		delete[] m_pLastFramePosition;
	}
}

void CAnimator::SetAnimationIndex( unsigned int uAnimationIndex ) {
	if( uAnimationIndex == m_uCurrentAnimationIndex ) {
		return;
	}

	//kill data of the previous anim
	if( m_pRootNode != NULL ) {
		delete m_pRootNode;
		m_pRootNode = NULL;
	}

	if( m_pLastFramePosition != NULL ) {
		delete[] m_pLastFramePosition;
	}

	m_mapNodesByName.clear();
	m_pCurrentAnimation = NULL;

	m_uCurrentAnimationIndex = uAnimationIndex;
	m_pCurrentAnimation = m_pScene->mAnimations[m_uCurrentAnimationIndex];

	//create the internal node tree. Do this even in case of invalid animation index
	//so that the transformation matrices are properly set up to mimic the current scene
	m_pRootNode = CreateNodeTree( m_pScene->mRootNode, NULL );

	//invalid anim index
	if( uAnimationIndex >= m_pScene->mNumAnimations ) {
		m_uCurrentAnimationIndex = 0;
		m_pCurrentAnimation = m_pScene->mAnimations[m_uCurrentAnimationIndex];
	}

	m_pLastFramePosition = new glm::uvec3[m_pCurrentAnimation->mNumChannels];
}

TAnimationNode* CAnimator::CreateNodeTree( aiNode* pNode, TAnimationNode* pParent ) {
	//create a node
	TAnimationNode* pInternalNode = new TAnimationNode( pNode->mName.data );
	pInternalNode->pParent = pParent;
	m_mapNodesByName[pNode] = pInternalNode;

	//copy its transformation
	pInternalNode->mLocalTransform = pNode->mTransformation;
	CalculateGlobalTransform( pInternalNode );

	//find the index of the animation track affecting this node, if any
	if( m_uCurrentAnimationIndex < m_pScene->mNumAnimations ) {
		pInternalNode->uChannelIndex = -1;

		for( unsigned int a = 0; a < m_pCurrentAnimation->mNumChannels; a++ ) {
			if( m_pCurrentAnimation->mChannels[a]->mNodeName.data == pInternalNode->sName ) {
				pInternalNode->uChannelIndex = a;
				break;
			}
		}
	}

	//continue for all child nodes and assign the created internal nodes as our children
	for( unsigned int a = 0; a < pNode->mNumChildren; a++ ) {
		TAnimationNode* childNode = CreateNodeTree( pNode->mChildren[a], pInternalNode );
		pInternalNode->vChildren.push_back( childNode );
	}

	return pInternalNode;
}

const std::vector<aiMatrix4x4>& CAnimator::GetBoneMatrices( const aiNode* pNode, unsigned int uNodeMeshIndex ) {
	if( uNodeMeshIndex < pNode->mNumMeshes ) {
		unsigned int uMeshIndex = pNode->mMeshes[uNodeMeshIndex];

		if( ( m_pScene != NULL ) && ( uMeshIndex < m_pScene->mNumMeshes ) ) {
			const aiMesh* pMesh = m_pScene->mMeshes[uMeshIndex];

			for( unsigned int i = 0; i < m_vTransforms.size(); i++ ) {
				m_vTransforms[i] = aiMatrix4x4();
			}

			//resize array and initialise it with identity matrices
			m_vTransforms.resize( pMesh->mNumBones, aiMatrix4x4() );

			//calculate the mesh's inverse global transform
			aiMatrix4x4 mGlobalInverseMeshTransform = GetGlobalTransform( pNode );
			mGlobalInverseMeshTransform.Inverse();

			//Bone matrices transform from mesh coordinates in bind pose to mesh coordinates in skinned pose
			//Therefore the formula is offsetMatrix * currentGlobalTransform * inverseCurrentMeshTransform
			for( unsigned int i = 0; i < pMesh->mNumBones; i++ ) {
				const aiBone* pBone = pMesh->mBones[i];
				const aiMatrix4x4& mCurrentGlobalTransform = GetGlobalTransform( m_mapBoneNodesByName[pBone->mName.data] );
				m_vTransforms[i] = mGlobalInverseMeshTransform * mCurrentGlobalTransform * pBone->mOffsetMatrix;
			}
		}
	}

	return m_vTransforms;
}

void CAnimator::CalculateGlobalTransform( TAnimationNode* pInternalNode ) {
	//concatenate all parent transforms to get the global transform for this node
	pInternalNode->mGlobalTransform = pInternalNode->mLocalTransform;
	TAnimationNode* pNode = pInternalNode->pParent;

	while( pNode ) {
		pInternalNode->mGlobalTransform = pNode->mLocalTransform * pInternalNode->mGlobalTransform;
		pNode = pNode->pParent;
	}
}

const aiMatrix4x4& CAnimator::GetGlobalTransform( const aiNode* node ) const {
	NodeMap::const_iterator it = m_mapNodesByName.find( node );

	if( it == m_mapNodesByName.end() ) {
		return m_mIdentityMatrix;
	}

	return it->second->mGlobalTransform;
}

void CAnimator::UpdateAnimation( float fDeltaTime, int iTicksPerSecond ) {
	m_lElapsedTime += fDeltaTime;

	if( ( m_pCurrentAnimation ) && ( m_pCurrentAnimation->mDuration > 0.0 ) ) {
		double dTime = ( ( double ) m_lElapsedTime );

		//calculate current local transformations
		//extract ticks per second. Assume default value if not given
		double dTicksPerSecondCorrected = iTicksPerSecond != 0.0 ? iTicksPerSecond : DEFAULT_ANIMATION_TICKS_PER_SECOND;

		//map into anim's duration
		double dTimeInTicks = 0.0f;

		if( m_pCurrentAnimation->mDuration > 0.0 ) {
			dTimeInTicks = fmod( dTime * dTicksPerSecondCorrected, m_pCurrentAnimation->mDuration );
		}

		if( m_vTransforms.size() != m_pCurrentAnimation->mNumChannels ) {
			m_vTransforms.resize( m_pCurrentAnimation->mNumChannels );
		}

		//calculate the transformations for each animation channel
		for( unsigned int i = 0; i < m_pCurrentAnimation->mNumChannels; i++ ) {
			const aiNodeAnim* pChannel = m_pCurrentAnimation->mChannels[i];

			//******** Position *****
			aiVector3D vPresentPosition( 0, 0, 0 );

			if( pChannel->mNumPositionKeys > 0 ) {
				//Look for present frame number. Search from last position if time is after the last time, else from beginning
				//Should be much quicker than always looking from start for the average use case.
				unsigned int uFrame = ( dTimeInTicks >= m_dLastTime ) ? m_pLastFramePosition[i].x : 0;

				while( uFrame < pChannel->mNumPositionKeys - 1 ) {
					if( dTimeInTicks < pChannel->mPositionKeys[uFrame + 1].mTime ) {
						break;
					}

					uFrame++;
				}

				//interpolate between this frame's value and next frame's value
				unsigned int uNextFrame = ( uFrame + 1 ) % pChannel->mNumPositionKeys;
				const aiVectorKey& pKey = pChannel->mPositionKeys[uFrame];
				const aiVectorKey& pNextKey = pChannel->mPositionKeys[uNextFrame];
				double dTimeDifference = pNextKey.mTime - pKey.mTime;

				if( dTimeDifference < 0.0 ) {
					dTimeDifference += m_pCurrentAnimation->mDuration;
				}

				if( dTimeDifference > 0 ) {
					float fInterpolationFactor = ( float )( ( dTimeInTicks - pKey.mTime ) / dTimeDifference );
					vPresentPosition = pKey.mValue + ( pNextKey.mValue - pKey.mValue ) * fInterpolationFactor;
				} else {
					vPresentPosition = pKey.mValue;
				}

				m_pLastFramePosition[i].x = uFrame;
			}

			//******** Rotation *********
			aiQuaternion qPresentRotation( 1, 0, 0, 0 );

			if( pChannel->mNumRotationKeys > 0 ) {
				unsigned int uFrame = ( dTimeInTicks >= m_dLastTime ) ? m_pLastFramePosition[i].y : 0;

				while( uFrame < pChannel->mNumRotationKeys - 1 ) {
					if( dTimeInTicks < pChannel->mRotationKeys[uFrame + 1].mTime ) {
						break;
					}

					uFrame++;
				}

				//interpolate between this frame's value and next frame's value
				unsigned int uNextFrame = ( uFrame + 1 ) % pChannel->mNumRotationKeys;
				const aiQuatKey& pKey = pChannel->mRotationKeys[uFrame];
				const aiQuatKey& pNextKey = pChannel->mRotationKeys[uNextFrame];
				double dTimeDifference = pNextKey.mTime - pKey.mTime;

				if( dTimeDifference < 0.0 ) {
					dTimeDifference += m_pCurrentAnimation->mDuration;
				}

				if( dTimeDifference > 0 ) {
					float fInterpolationFactor = ( float )( ( dTimeInTicks - pKey.mTime ) / dTimeDifference );
					aiQuaternion::Interpolate( qPresentRotation, pKey.mValue, pNextKey.mValue, fInterpolationFactor );
				} else {
					qPresentRotation = pKey.mValue;
				}

				m_pLastFramePosition[i].y = uFrame;
			}

			//******** Scaling **********
			aiVector3D vPresentScaling( 1, 1, 1 );

			if( pChannel->mNumScalingKeys > 0 ) {
				unsigned int uFrame = ( dTimeInTicks >= m_dLastTime ) ? m_pLastFramePosition[i].z : 0;

				while( uFrame < pChannel->mNumScalingKeys - 1 ) {
					if( dTimeInTicks < pChannel->mScalingKeys[uFrame + 1].mTime ) {
						break;
					}

					uFrame++;
				}

				vPresentScaling = pChannel->mScalingKeys[uFrame].mValue;
				m_pLastFramePosition[i].z = uFrame;
			}

			//build a transformation matrix from it
			aiMatrix4x4& mTransformation = m_vTransforms[i];
			mTransformation = aiMatrix4x4( qPresentRotation.GetMatrix() );
			mTransformation.a1 *= vPresentScaling.x;
			mTransformation.b1 *= vPresentScaling.x;
			mTransformation.c1 *= vPresentScaling.x;
			mTransformation.a2 *= vPresentScaling.y;
			mTransformation.b2 *= vPresentScaling.y;
			mTransformation.c2 *= vPresentScaling.y;
			mTransformation.a3 *= vPresentScaling.z;
			mTransformation.b3 *= vPresentScaling.z;
			mTransformation.c3 *= vPresentScaling.z;
			mTransformation.a4 = vPresentPosition.x;
			mTransformation.b4 = vPresentPosition.y;
			mTransformation.c4 = vPresentPosition.z;
		}

		m_dLastTime = dTimeInTicks;

		//and update all node transformations with the results
		UpdateTransforms( m_pRootNode, m_vTransforms );
	}
}

void CAnimator::UpdateTransforms( TAnimationNode* pNode, const std::vector<aiMatrix4x4>& vTransforms ) {
	//update node local transform
	if( pNode->uChannelIndex != ( unsigned int )( -1 ) ) {
		if( pNode->uChannelIndex >= vTransforms.size() ) {
			return;
		}

		pNode->mLocalTransform = vTransforms[pNode->uChannelIndex];
	}

	//concatenate all parent transforms to get the global transform for this node
	pNode->mGlobalTransform = pNode->mLocalTransform;
	TAnimationNode* pParentNode = pNode->pParent;

	while( pParentNode ) {
		pNode->mGlobalTransform = pParentNode->mLocalTransform * pNode->mGlobalTransform;
		pParentNode = pParentNode->pParent;
	}

	//continue for all children
	for( std::vector<TAnimationNode*>::iterator it = pNode->vChildren.begin(); it != pNode->vChildren.end(); it++ ) {
		UpdateTransforms( *it, vTransforms );
	}
}
