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
#if defined( __linux__ ) && !defined( linux)
#define linux 1
#endif

#ifndef CNI_H
#define CNI_H

#include <XnOpenNI.h>
#include <XnCppWrapper.h>

#include <XnVHandPointContext.h>
#include <XnVSessionManager.h>
#include <XnVSelectableSlider2D.h>

#define SAMPLE_XML_FILE "../resources/KinectData/Sample-Tracking.xml"

void XN_CALLBACK_TYPE TrackPad_ValueChange(XnFloat fXValue, XnFloat fYValue, void* cxt);
void XN_CALLBACK_TYPE TrackPad_ItemHover(XnInt32 nXItem, XnInt32 nYItem, void* cxt);
void XN_CALLBACK_TYPE TrackPad_ItemSelect(XnInt32 nXItem, XnInt32 nYItem, XnVDirection eDir, void* cxt);
void XN_CALLBACK_TYPE TrackPad_PrimaryCreate(const XnVHandPointContext* cxt, const XnPoint3D& ptFocus, void* UserCxt);
void XN_CALLBACK_TYPE TrackPad_PrimaryDestroy(XnUInt32 nID, void* UserCxt);
void XN_CALLBACK_TYPE SessionStart(const XnPoint3D& ptFocus, void* UserCxt);
void XN_CALLBACK_TYPE SessionEnd(void* UserCxt);

class CNI {
public:
	CNI();
	~CNI();
	static void Init(XnUInt32 x = 9, XnUInt32 y = 9);
	static void Destroy();
	static void Update();
	static void keys();
	
	
	static bool first();
	
private:
	static void AllocateAllGlobals();
	
public:
	static XnBool g_bActive ;
	static XnBool g_bIsInput ;
	static XnBool g_bInSession ;
	
	static 	XnBool g_bIsPushed ;
	
	static const XnUInt32 XN_PUSH_DISPLAY_FRAMES = 30;
	static XnUInt32 g_nCurrentFrame;
	
	static XnFloat g_fXValue ;
	static XnFloat g_fYValue ;
	
	static XnUInt32 g_nXIndex ;
	static XnUInt32 g_nYIndex ;
	
	static XnUInt32 g_TP_XDim ;
	static XnUInt32 g_TP_YDim ;
	
	static const XnUInt32 XN_MIN_X_DIM = 2;
	static const XnUInt32 XN_MAX_X_DIM = 12;
	static const XnUInt32 XN_MIN_Y_DIM = 2;
	static const XnUInt32 XN_MAX_Y_DIM = 12;
	
	static xn::Context g_Context;
	static xn::ScriptNode g_ScriptNode;
	static XnVSelectableSlider2D* g_pTrackPad ;
	static XnVSessionManager* g_pSessionManager;
	
	static XnCallbackHandle g_nItemHoverHandle ;
	static XnCallbackHandle g_nItemSelectHandle;
	static XnCallbackHandle g_nValueChangeHandle;
	
	static XnCallbackHandle g_nPrimaryCreateHandle;
	static XnCallbackHandle g_nPrimaryDestroyHandle;
	
	static XnUInt32 g_TrackPadHandle ;
	
	static XnBool g_isPrintItemHover ;
	static XnBool g_isPrintValueChange;
	static XnBool g_isInputStarted ;
	
	static const XnFloat GL_WIN_SIZE_X;
	static const XnFloat GL_WIN_SIZE_Y;
	
	static XnBool g_bQuit ;
	
	static float getX();
	static float getY();
	
	static bool GetPush();
	static bool UseNI();
};

#endif // CNI_H
