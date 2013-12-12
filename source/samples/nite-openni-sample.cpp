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

#include <XnPlatform.h>
#if XN_PLATFORM == XN_PLATFORM_WIN32
#include <conio.h>
#define CatchSignals(x)
#else
int _kbhit( void );
// Stores pointer to bSignalWasCaught, and starts catching signals
// when a signal is caught, *bSignalWasCaught will be set to true
void CatchSignals( XnBool* bSignalWasCaught );
#endif

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <csignal>

using namespace std;

int _kbhit() {
	int ch;
	int oldf;
	struct termios old_termios;
	struct termios new_termios;

	tcgetattr( STDIN_FILENO, &old_termios );
	new_termios = old_termios;
	new_termios.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &new_termios );
	oldf = fcntl( STDIN_FILENO, F_GETFL, 0 );
	fcntl( STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK );

	ch = getchar();

	tcsetattr( STDIN_FILENO, TCSANOW, &old_termios );
	fcntl( STDIN_FILENO, F_SETFL, oldf );

	if( ch != EOF ) {
		ungetc( ch, stdin );
		return 1;
	}

	return 0;
}


XnBool* g_pValueToSet = NULL;

void SignalHandler( int nSignal ) {
	printf( "Caught signal: %d\n", nSignal );

	if( NULL != g_pValueToSet ) {
		*g_pValueToSet = true;
	}
}

void CatchSignals( XnBool* bSignalWasCaught ) {
	g_pValueToSet = bSignalWasCaught;

	struct sigaction act;

	memset( &act, 0, sizeof( act ) );
	act.sa_handler = &SignalHandler;
	act.sa_flags = 0;
	sigaction( SIGINT, &act, NULL );
	sigaction( SIGTERM, &act, NULL );
	sigaction( SIGKILL, &act, NULL );
}



/*******************************************************************************
 *                                                                              *
 *   PrimeSense NITE 1.3 - Multi-Process Server Sample                          *
 *   Copyright (C) 2010 PrimeSense Ltd.                                         *
 *                                                                              *
 *******************************************************************************/

#include <XnOpenNI.h>
#include <XnVSessionManager.h>
#include <XnVPointControl.h>
#include <XnVMultiProcessFlowServer.h>
#include <XnVHandPointContext.h>

void XN_CALLBACK_TYPE SessionStart( const XnPoint3D& ptPosition, void* UserCxt ) {
	printf( "session start: (%f,%f,%f)\n", ptPosition.X, ptPosition.Y, ptPosition.Z );
}

void XN_CALLBACK_TYPE SessionEnd( void* UserCxt ) {
	printf( "session end!\n" );
}

void XN_CALLBACK_TYPE FocusStart( const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress, void* UserCxt ) {
	printf( "Focus gesture start detected - %5.2f!\n", fProgress );
}

xn::Context g_Context;
xn::ScriptNode g_ScriptNode;
XnVSessionManager* g_pSessionManager = NULL;
XnBool g_bQuit = false;

void CleanupExit() {
	if( NULL != g_pSessionManager ) {
		delete g_pSessionManager;
		g_pSessionManager = NULL;
	}

	exit( 1 );
}

#define SAMPLE_XML_FILE "../../resources/KinectData/Sample-Tracking.xml"

int main( int argc, char* argv[] ) {
	if( argc < 2 ) {
		printf( "usage: %s section_name\n", argv[0] );
		return 0;
	}

	printf( "\n" );

	char* strSectionName = argv[1];

	// Initialize device
	XnStatus rc = g_Context.InitFromXmlFile( SAMPLE_XML_FILE, g_ScriptNode );

	if( rc != XN_STATUS_OK ) {
		printf( "Couldn't initialize: %s\n", xnGetStatusString( rc ) );
		return 1;
	}

	// Create and initialize point tracker
	g_pSessionManager = new XnVSessionManager();
	rc = g_pSessionManager->Initialize( &g_Context, "Wave", "RaiseHand" );

	if( rc != XN_STATUS_OK ) {
		printf( "Session Manager couldn't initialize: %s\n", xnGetStatusString( rc ) );
		CleanupExit();
	}

	g_Context.StartGeneratingAll();

	g_pSessionManager->RegisterSession( NULL, &SessionStart, &SessionEnd, &FocusStart );

	CatchSignals( &g_bQuit );

	// static variable destructors will be called on exit(), while
	// automatic lifetime variables (regular variables on the stack) will not
	// have their destructors called (since stack unwinding isn't performed)
	static XnVMultiProcessFlowServer writer( strSectionName );
	rc = writer.Initialize();

	if( XN_STATUS_OK != rc ) {
		printf( "Failed to initialize server: %s\n", xnGetStatusString( rc ) );
		CleanupExit();
	}

	g_pSessionManager->RegisterSession( &writer );
	g_pSessionManager->AddListener( &writer );
	//g_pSessionManager->AddListener(&print);

	printf( "Server running in section <%s>. Hit any key to exit.\n", strSectionName );

	while( ( !g_bQuit ) && ( !_kbhit() ) ) {
		g_Context.WaitAnyUpdateAll();

		// Update point tracker
		g_pSessionManager->Update( &g_Context );

		// Publish current state
		//		rc = writer.WriteState();

		if( XN_STATUS_OK != rc ) {
			// Error
			break;
		}
	}

	g_pSessionManager->UnregisterSession( &writer );
	CleanupExit();
}

#if ( defined( __GNUC__ ) || defined( __GNUG__ ) )
#pragma GCC diagnostic pop
#endif
