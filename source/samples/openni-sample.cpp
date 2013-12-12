/****************************************************************************
 *                                                                           *
 *  OpenNI 1.x Alpha                                                         *
 *  Copyright (C) 2011 PrimeSense Ltd.                                       *
 *                                                                           *
 *  This file is part of OpenNI.                                             *
 *                                                                           *
 *  OpenNI is free software: you can redistribute it and/or modify           *
 *  it under the terms of the GNU Lesser General Public License as published *
 *  by the Free Software Foundation, either version 3 of the License, or     *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  OpenNI is distributed in the hope that it will be useful,                *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU Lesser General Public License *
 *  along with OpenNI. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                           *
 ****************************************************************************/

#if ( defined( __GNUC__ ) || defined( __GNUG__ ) )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Werror"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#pragma GCC diagnostic ignored "-Wswitch-enum"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wreorder"
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-pedantic"
#endif

#if defined( __linux__ ) && !defined( linux)
#define linux 1
#endif
//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include <XnCodecIDs.h>
#include <XnLog.h>
#include <cstdlib>
#include <iostream>

#define CHECK_RC(rc, what)											\
	if (rc != XN_STATUS_OK)											\
	{																\
		printf("%s failed: %s\n", what, xnGetStatusString(rc));		\
		return rc;													\
	}

//---------------------------------------------------------------------------
// Code
//---------------------------------------------------------------------------

using namespace xn;

int main( int argc, char* argv[] ) {
	XnStatus nRetVal = XN_STATUS_OK;

	if( argc < 3 ) {
		std::cout << "usage: " << argv[0] << " <inputFile> <outputFile> [nodeType] [startFrame] [endFrame]\n";
		return -1;
	}

	const char* strInputFile = argv[1];

	const char* strOutputFile = argv[2];

	const char* strNodeType = NULL;

	XnUInt32 nStartFrame = 1;

	XnUInt32 nEndFrame = XN_MAX_UINT32;

	XnProductionNodeType seekNodeType = XN_NODE_TYPE_INVALID;

	if( argc >= 4 ) {
		strNodeType = argv[3];
		nRetVal = xnProductionNodeTypeFromString( strNodeType, &seekNodeType );

		if( nRetVal != XN_STATUS_OK ) {
			std::cout << "Bad node type specified: " << strNodeType << "\n";
			return nRetVal;
		}

		if( argc >= 5 ) {
			nStartFrame = atoi( argv[4] );

			if( argc >= 6 ) {
				nEndFrame = atoi( argv[5] );
			}
		}
	}

	Context context;
	nRetVal = context.Init();
	CHECK_RC( nRetVal, "Init" );

	// open input file
	Player player;
	nRetVal = context.OpenFileRecording( strInputFile, player );
	CHECK_RC( nRetVal, "Open input file" );

	nRetVal = player.SetPlaybackSpeed( XN_PLAYBACK_SPEED_FASTEST );
	CHECK_RC( nRetVal, "Setting playback speed" );

	// get the list of all created nodes
	NodeInfoList nodes;
	nRetVal = player.EnumerateNodes( nodes );
	CHECK_RC( nRetVal, "Enumerate nodes" );

	// create recorder
	Recorder recorder;
	nRetVal = recorder.Create( context );
	CHECK_RC( nRetVal, "Create recorder" );

	nRetVal = recorder.SetDestination( XN_RECORD_MEDIUM_FILE, strOutputFile );
	CHECK_RC( nRetVal, "Set recorder destination file" );

	ProductionNode seekNode;

	// add all nodes to recorder
	for( NodeInfoList::Iterator it = nodes.Begin(); it != nodes.End(); ++it ) {
		NodeInfo nodeInfo = *it;

		// NOTE: for now, ignore audio
		if( nodeInfo.GetDescription().Type == XN_NODE_TYPE_AUDIO ) {
			continue;
		}

		ProductionNode node;
		nRetVal = nodeInfo.GetInstance( node );
		CHECK_RC( nRetVal, "Get instance" );

		if( seekNodeType == XN_NODE_TYPE_INVALID ) {
			//No node type specified - record all nodes.
			nRetVal = recorder.AddNodeToRecording( node );
			CHECK_RC( nRetVal, "Add node to recording" );
		} else
			if( seekNodeType == nodeInfo.GetDescription().Type ) {
				//If node type is specified, we only record nodes of that type.
				nRetVal = player.SeekToFrame( node.GetName(), nStartFrame, XN_PLAYER_SEEK_SET );
				CHECK_RC( nRetVal, "Seek player to frame" );
				nRetVal = recorder.AddNodeToRecording( node );
				CHECK_RC( nRetVal, "Add node to recording" );
			}
	}

	nRetVal = player.SetRepeat( FALSE );
	XN_IS_STATUS_OK( nRetVal );

	int nFrame = 0;

	while( ( nRetVal = context.WaitAnyUpdateAll() ) != XN_STATUS_EOF ) {
		CHECK_RC( nRetVal, "Read next frame" );
		std::cout << "Recording: " << nFrame++ << "\r";

		if( ( seekNodeType != XN_NODE_TYPE_INVALID ) && ( nFrame == nEndFrame ) ) {
			break;
		}
	}

	player.Release();
	context.Release();

	return 0;
}


#if ( defined( __GNUC__ ) || defined( __GNUG__ ) )
#pragma GCC diagnostic pop
#endif
