/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  Manuel Bellersen <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "cni.h"

#include <iostream>

XnBool CNI::g_bActive = FALSE;
XnBool CNI::g_bIsInput = FALSE;
XnBool CNI::g_bInSession = FALSE;
XnBool CNI::g_bIsPushed = FALSE;

XnUInt32 CNI::g_nCurrentFrame = 0;
XnFloat CNI::g_fXValue = 0.5f;
XnFloat CNI::g_fYValue = 0.5f;
XnUInt32 CNI::g_nXIndex = 0;
XnUInt32 CNI::g_nYIndex = 0;
XnUInt32 CNI::g_TP_XDim = 9;
XnUInt32 CNI::g_TP_YDim = 9;

xn::Context CNI::g_Context;
xn::ScriptNode CNI::g_ScriptNode;
XnVSelectableSlider2D* CNI::g_pTrackPad = nullptr;
XnVSessionManager* CNI::g_pSessionManager = nullptr;

XnCallbackHandle CNI::g_nItemHoverHandle = nullptr;
XnCallbackHandle CNI::g_nItemSelectHandle = nullptr;
XnCallbackHandle CNI::g_nValueChangeHandle = nullptr;

XnCallbackHandle CNI::g_nPrimaryCreateHandle = nullptr;
XnCallbackHandle CNI::g_nPrimaryDestroyHandle = nullptr;

XnUInt32 CNI::g_TrackPadHandle = 0;

XnBool CNI::g_isPrintItemHover = TRUE;
XnBool CNI::g_isPrintValueChange = FALSE;
XnBool CNI::g_isInputStarted = FALSE;

const XnFloat CNI::GL_WIN_SIZE_X = 800.0;
const XnFloat CNI::GL_WIN_SIZE_Y = 600.0;

XnBool CNI::g_bQuit = false;

bool g_pushing = false;

struct _CurrentItem
{
	XnInt32 X, Y;
} CurrentItem;

void XN_CALLBACK_TYPE TrackPad_ValueChange( XnFloat fXValue, XnFloat fYValue, void* cxt ) {
	if( TRUE == CNI::g_isPrintValueChange ) {
		std::clog << "Value changed: " << fXValue << ", " << fYValue << std::endl;
	}

	CNI::g_fXValue = fXValue;
	CNI::g_fYValue = fYValue;
}

void XN_CALLBACK_TYPE TrackPad_ItemHover( XnInt32 nXItem, XnInt32 nYItem, void* cxt ) {
	if( TRUE == CNI::g_isPrintItemHover ) {
		std::clog << "Hover: " << nXItem << ", " << nYItem << std::endl ;
	}

	if( ( TRUE == CNI::g_bIsPushed ) && ( CurrentItem.X != nXItem || CurrentItem.Y != nYItem ) ) {
		CNI::g_bIsPushed = FALSE;
		CNI::g_nCurrentFrame = 0;
	}

	CurrentItem.X = nXItem;
	CurrentItem.Y = nYItem;
}

void XN_CALLBACK_TYPE TrackPad_ItemSelect( XnInt32 nXItem, XnInt32 nYItem, XnVDirection eDir, void* cxt ) {
	std::clog <<  "Select: " << nXItem  << ", " << nYItem << " (" << XnVDirectionAsString( eDir ) << ")" << std::endl ;
	CNI::g_bIsPushed = TRUE;
	g_pushing = true;
}

void XN_CALLBACK_TYPE TrackPad_PrimaryCreate( const XnVHandPointContext* cxt, const XnPoint3D& ptFocus, void* UserCxt ) {
	std::clog <<   "TrackPad input has started!!!, point ID: [" << cxt->nID << "] " << std::endl;
	std::clog <<  "Starting point position: [" << cxt->ptPosition.X << "],[" << cxt->ptPosition.Y << "],[" << cxt->ptPosition.Z << "]\n";
	CNI::g_isInputStarted = TRUE;
}

void XN_CALLBACK_TYPE TrackPad_PrimaryDestroy( XnUInt32 nID, void* UserCxt ) {
	std::clog << "TrackPad input has stopped!!!\n";
	CNI::g_isInputStarted = FALSE;
}

void XN_CALLBACK_TYPE SessionStart( const XnPoint3D& ptFocus, void* UserCxt ) {
	CNI::g_bInSession = true;
}

void XN_CALLBACK_TYPE SessionEnd( void* UserCxt ) {
	CNI::g_bInSession = false;
}


CNI::CNI()

{

}

float CNI::getX() {
	return CurrentItem.X;
}

float CNI::getY() {
	return CurrentItem.Y;
}

bool CNI::GetPush() {
	if( g_pushing){
		g_pushing = false;
		return true;
	}else{
		return false;
	}
}


void CNI::Destroy() {
	if( nullptr != g_pTrackPad ) {
		// Unregister for the Hover event of the TrackPad
		if( nullptr != g_nItemHoverHandle )
			g_pTrackPad->UnregisterItemHover( g_nItemHoverHandle );

		// Unregister for the Value Change event of the TrackPad
		if( nullptr != g_nValueChangeHandle )
			g_pTrackPad->UnregisterValueChange( g_nValueChangeHandle );

		// Unregister for the Select event of the TrackPad
		if( nullptr != g_nItemSelectHandle )
			g_pTrackPad->UnregisterItemSelect( g_nItemSelectHandle );

		// Unregister for Input Stop event of the TrackPad
		if( nullptr != g_nPrimaryDestroyHandle )
			g_pTrackPad->UnregisterPrimaryPointDestroy( g_nPrimaryDestroyHandle );

		// Unregister for Input Start event of the TrackPad
		if( nullptr != g_nPrimaryCreateHandle )
			g_pTrackPad->UnregisterPrimaryPointCreate( g_nPrimaryCreateHandle );
	}

	if( nullptr != g_pSessionManager ) {
		if( 0 != g_TrackPadHandle )
			g_pSessionManager->RemoveListener( g_TrackPadHandle );

		delete g_pSessionManager;
		g_pSessionManager = nullptr;
	}

	if( nullptr != g_pTrackPad ) {
		delete g_pTrackPad;
		g_pTrackPad = nullptr;
	}
}

CNI::~CNI() {
	Destroy();
}


void CNI::Init( XnUInt32 x, XnUInt32 y ) {
	if( nullptr != g_pTrackPad ) {
		g_pTrackPad->SetItemCount( x,y );
	}
}

void CNI::Update() {
	// Read next available data
	g_Context.WaitAnyUpdateAll();
	// Process the data
	g_pSessionManager->Update( &g_Context );

	// Draw the TrackPad
// 	DrowTrackPad();
}

void CNI::keys() {
	int key;

	switch( key ) {
	case 27:
		Destroy();
		break;
	case 'v': //Enable/Disable Value Change presentation on screen
		g_isPrintValueChange = !g_isPrintValueChange;
		break;
	case 'h': //Enable/Disable Item Hover presentation on screen
		g_isPrintItemHover = !g_isPrintItemHover;
		break;
	case 't': //Toggle between Value Change and Item Hover presentation on screen

		if( FALSE == g_isPrintItemHover ) {
			g_isPrintValueChange = FALSE;
			g_isPrintItemHover = TRUE;
		} else {
			g_isPrintValueChange = TRUE;
			g_isPrintItemHover = FALSE;
		}

		break;
	case 'r': // Remove a row from the TrackPad

		if( XN_MIN_X_DIM >= g_TP_YDim ) //Check if we have reached minimum size
			return;

		--g_TP_YDim;
		Init();
		break;
	case 'R': // Add a row to the TrackPad

		if( XN_MAX_X_DIM <= g_TP_YDim ) //Check if we have reached maximum size
			return;

		++g_TP_YDim;
		Init();
		break;
	case 'c': // Remove a column from the TrackPad

		if( XN_MIN_Y_DIM >= g_TP_XDim ) //Check if we have reached minimum size
			return;

		--g_TP_XDim;
		Init();
		break;
	case 'C': // Add a column to the TrackPad

		if( XN_MAX_Y_DIM <= g_TP_XDim ) //Check if we have reached maximum size
			return;

		++g_TP_XDim;
		Init();
		break;
	}
}

void CNI::AllocateAllGlobals() {

	// Configure
	XnStatus rc = g_Context.InitFromXmlFile( SAMPLE_XML_FILE, g_ScriptNode );

	if( rc != XN_STATUS_OK ) {
		printf( "Error initializing: %s\n", xnGetStatusString( rc ) );
		Destroy();
	}

	if( nullptr == g_pSessionManager ) {
		g_pSessionManager = new XnVSessionManager();
	}

	if( nullptr == g_pSessionManager ) {
		printf( "Couldn't create PointTracker!! (out of memory)\n" );
		Destroy();
	}

	if( nullptr == g_pTrackPad ) {
		g_pTrackPad = new XnVSelectableSlider2D( g_TP_XDim, g_TP_YDim );
	}

	if( nullptr == g_pTrackPad ) {
		printf( "Couldn't create TrackPad!! (out of memory)\n" );
		Destroy();
	}

	g_Context.StartGeneratingAll();
}

static bool g_useNI = false;

bool CNI::UseNI() {
	return g_useNI;
}


bool CNI::first() {
	AllocateAllGlobals();

	// Initialize the point tracker
	XnStatus rc = g_pSessionManager->Initialize( &g_Context, "Wave", "RaiseHand" );

	if( rc != XN_STATUS_OK ) {
		std::clog << "Couldn't initialize the Session Manager: " << xnGetStatusString( rc ) << std::endl;
		Destroy();
		return false;
	}
	
	g_useNI = true;

	g_pSessionManager->RegisterSession( nullptr, &SessionStart, &SessionEnd );

	// Add TrackPad to the point tracker
	g_TrackPadHandle = g_pSessionManager->AddListener( g_pTrackPad );

	// Register for the Hover event of the TrackPad
	g_nItemHoverHandle = g_pTrackPad->RegisterItemHover( nullptr, &TrackPad_ItemHover );
	// Register for the Value Change event of the TrackPad
	g_nValueChangeHandle = g_pTrackPad->RegisterValueChange( nullptr, &TrackPad_ValueChange );
	// Register for the Select event of the TrackPad
	g_nItemSelectHandle = g_pTrackPad->RegisterItemSelect( nullptr, &TrackPad_ItemSelect );

	// Register for Input Start event of the TrackPad
	g_nPrimaryCreateHandle = g_pTrackPad->RegisterPrimaryPointCreate( nullptr, &TrackPad_PrimaryCreate );
	// Register for Input Stop event of the TrackPad
	g_nPrimaryDestroyHandle = g_pTrackPad->RegisterPrimaryPointDestroy( nullptr, &TrackPad_PrimaryDestroy );

	return true;
}
