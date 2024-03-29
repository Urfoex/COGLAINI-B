// ----------------------------------------------------------------------------
// Simple sample to prove that Assimp is easy to use with OpenGL.
// It takes a file name as command line parameter, loads it using standard
// settings and displays it.
//
// If you intend to _use_ this code sample in your app, do yourself a favour
// and replace immediate mode calls with VBOs ...
//
// The vc8 solution links against assimp-release-dll_win32 - be sure to
// have this configuration built.
// ----------------------------------------------------------------------------

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
#pragma GCC diagnostic ignored "-Wparentheses"
#endif

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

// assimp include files. These three are usually needed.
#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"

#include "camera.h"

// the global Assimp scene object
const struct aiScene* scene = NULL;
GLuint scene_list = 0;
struct aiVector3D scene_min, scene_max, scene_center;

// current rotation angle
static float angle = 0.f;

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

sf::RenderWindow* pWindow;

CCamera camera;

// ----------------------------------------------------------------------------
void reshape( int width, int height ) {
	const double aspectRatio = ( float ) width / height, fieldOfView = 45.0;

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( fieldOfView, aspectRatio,
					1.0, 1000.0 ); /* Znear and Zfar */
	glViewport( 0, 0, width, height );
}

// ----------------------------------------------------------------------------
void get_bounding_box_for_node( const struct aiNode* nd,
								struct aiVector3D* min,
								struct aiVector3D* max,
								struct aiMatrix4x4* trafo
							  ) {
	struct aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4( trafo, &nd->mTransformation );

	for( ; n < nd->mNumMeshes; ++n ) {
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

		for( t = 0; t < mesh->mNumVertices; ++t ) {

			struct aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4( &tmp, trafo );

			min->x = aisgl_min( min->x, tmp.x );
			min->y = aisgl_min( min->y, tmp.y );
			min->z = aisgl_min( min->z, tmp.z );

			max->x = aisgl_max( max->x, tmp.x );
			max->y = aisgl_max( max->y, tmp.y );
			max->z = aisgl_max( max->z, tmp.z );
		}
	}

	for( n = 0; n < nd->mNumChildren; ++n ) {
		get_bounding_box_for_node( nd->mChildren[n], min, max, trafo );
	}

	*trafo = prev;
}

// ----------------------------------------------------------------------------
void get_bounding_box( struct aiVector3D* min, struct aiVector3D* max ) {
	struct aiMatrix4x4 trafo;
	aiIdentityMatrix4( &trafo );

	min->x = min->y = min->z =  1e10f;
	max->x = max->y = max->z = -1e10f;
	get_bounding_box_for_node( scene->mRootNode, min, max, &trafo );
}

// ----------------------------------------------------------------------------
void color4_to_float4( const struct aiColor4D* c, float f[4] ) {
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

// ----------------------------------------------------------------------------
void set_float4( float f[4], float a, float b, float c, float d ) {
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

// ----------------------------------------------------------------------------
void apply_material( const struct aiMaterial* mtl ) {
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	struct aiColor4D diffuse;
	struct aiColor4D specular;
	struct aiColor4D ambient;
	struct aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;

	set_float4( c, 0.8f, 0.8f, 0.8f, 1.0f );

	if( AI_SUCCESS == aiGetMaterialColor( mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse ) )
		color4_to_float4( &diffuse, c );

	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, c );

	set_float4( c, 0.0f, 0.0f, 0.0f, 1.0f );

	if( AI_SUCCESS == aiGetMaterialColor( mtl, AI_MATKEY_COLOR_SPECULAR, &specular ) )
		color4_to_float4( &specular, c );

	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, c );

	set_float4( c, 0.2f, 0.2f, 0.2f, 1.0f );

	if( AI_SUCCESS == aiGetMaterialColor( mtl, AI_MATKEY_COLOR_AMBIENT, &ambient ) )
		color4_to_float4( &ambient, c );

	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, c );

	set_float4( c, 0.0f, 0.0f, 0.0f, 1.0f );

	if( AI_SUCCESS == aiGetMaterialColor( mtl, AI_MATKEY_COLOR_EMISSIVE, &emission ) )
		color4_to_float4( &emission, c );

	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, c );

	max = 1;
	ret1 = aiGetMaterialFloatArray( mtl, AI_MATKEY_SHININESS, &shininess, &max );

	if( ret1 == AI_SUCCESS ) {
		max = 1;
		ret2 = aiGetMaterialFloatArray( mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max );

		if( ret2 == AI_SUCCESS )
			glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength );
		else
			glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, shininess );
	} else {
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 0.0f );
		set_float4( c, 0.0f, 0.0f, 0.0f, 0.0f );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, c );
	}

	max = 1;

	if( AI_SUCCESS == aiGetMaterialIntegerArray( mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max ) )
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;

	glPolygonMode( GL_FRONT_AND_BACK, fill_mode );

	max = 1;

	if( ( AI_SUCCESS == aiGetMaterialIntegerArray( mtl, AI_MATKEY_TWOSIDED, &two_sided, &max ) ) && two_sided )
		glDisable( GL_CULL_FACE );
	else
		glEnable( GL_CULL_FACE );
}

// ----------------------------------------------------------------------------
void recursive_render( const struct aiScene* sc, const struct aiNode* nd ) {
	int i;
	unsigned int n = 0, t;
	struct aiMatrix4x4 m = nd->mTransformation;

	// update transform
	aiTransposeMatrix4( &m );
	glPushMatrix();
	glMultMatrixf( ( float* )&m );

	// draw all meshes assigned to this node
	for( ; n < nd->mNumMeshes; ++n ) {
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

		apply_material( sc->mMaterials[mesh->mMaterialIndex] );

		if( mesh->mNormals == NULL ) {
			glDisable( GL_LIGHTING );
		} else {
			glEnable( GL_LIGHTING );
		}

		for( t = 0; t < mesh->mNumFaces; ++t ) {
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch( face->mNumIndices ) {
			case 1:
				face_mode = GL_POINTS;
				break;
			case 2:
				face_mode = GL_LINES;
				break;
			case 3:
				face_mode = GL_TRIANGLES;
				break;
			default:
				face_mode = GL_POLYGON;
				break;
			}

			glBegin( face_mode );

			for( i = 0; i < face->mNumIndices; i++ ) {
				int index = face->mIndices[i];

				if( mesh->mColors[0] != NULL )
					glColor4fv( ( GLfloat* )&mesh->mColors[0][index] );

				if( mesh->mNormals != NULL )
					glNormal3fv( &mesh->mNormals[index].x );

				glVertex3fv( &mesh->mVertices[index].x );
			}

			glEnd();
		}

	}

	// draw all children
	for( n = 0; n < nd->mNumChildren; ++n ) {
		recursive_render( sc, nd->mChildren[n] );
	}

	glPopMatrix();
}

// ----------------------------------------------------------------------------
//void do_motion (void)
//{
//	static GLint prev_time = 0;
//	sf::Clock clock;
//	int time = clock.GetElapsedTime();
//	std::cout<<clock.GetElapsedTime()<<time<<prev_time<<std::endl;
//	angle += (time-prev_time)*0.1;
//	prev_time = time;
//}

void do_motion( void ) {
	static double prev_time = 0;
	static sf::Clock clock;
	sf::Uint32 s = clock.GetElapsedTime();
	double time = clock.GetElapsedTime() / 1000.0;
	angle += ( time - prev_time ) * 10.0;
	//angle += (window.GetFrameTime() / 1000.0) * 10.0;
	//	std::cout<<angle<<" :: "<<(time-prev_time)<<std::endl;
	prev_time = time;
}

// ----------------------------------------------------------------------------
void display( void ) {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	// rotate object or camera
	// depending on mouse (put it above glutlookat to rotate camera instead of object=
	sf::Vector2i mouse = sf::Mouse::GetPosition( *pWindow );

	static int lastMouseX = mouse.x;
	int deltaMouseX = mouse.x - lastMouseX;
	lastMouseX = mouse.x;

	static int lastMouseY = mouse.y;
	int deltaMouseY = mouse.y - lastMouseY;
	lastMouseY = mouse.y;

	camera.RotateY( -deltaMouseX / 4 );
	camera.RotateX( -deltaMouseY / 4 );

	camera.Render();

	// scale the whole asset to fit into our view frustum
	float tmp;
	tmp = scene_max.x - scene_min.x;
	tmp = aisgl_max( scene_max.y - scene_min.y, tmp );
	tmp = aisgl_max( scene_max.z - scene_min.z, tmp );
	tmp = 1.f / tmp;
	glScalef( tmp, tmp, tmp );

	// center the model
	glTranslatef( -scene_center.x, -scene_center.y, -scene_center.z );

	// if the display list has not been made yet, create a new one and
	// fill it with scene contents
	if( scene_list == 0 ) {
		scene_list = glGenLists( 1 );
		glNewList( scene_list, GL_COMPILE );
		// now begin at the root node of the imported data and traverse
		// the scenegraph by multiplying subsequent local transforms
		// together on GL's matrix stack.
		recursive_render( scene, scene->mRootNode );
		glEndList();
	}

	glCallList( scene_list );


	do_motion();
}

// ----------------------------------------------------------------------------
int loadasset( const char* path ) {
	// we are taking one of the postprocessing presets to avoid
	// spelling out 20+ single do flags here.
	scene = aiImportFile( path, aiProcessPreset_TargetRealtime_MaxQuality );

	if( scene ) {
		get_bounding_box( &scene_min, &scene_max );
		scene_center.x = ( scene_min.x + scene_max.x ) / 2.0f;
		scene_center.y = ( scene_min.y + scene_max.y ) / 2.0f;
		scene_center.z = ( scene_min.z + scene_max.z ) / 2.0f;
		return 0;
	}

	return 1;
}

// ----------------------------------------------------------------------------
int main( int argc, char** argv ) {
	struct aiLogStream stream;

	// Create the main window
	sf::RenderWindow window( sf::VideoMode( 800, 600 ), "SFML OpenGL", sf::Style::Default, sf::ContextSettings( 32 ) );
	pWindow = &window;

	// Enable Z-buffer read and write
	glEnable( GL_DEPTH_TEST );
	glDepthMask( GL_TRUE );
	glClearDepth( 1.f );

	// Setup a perspective projection
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 90.f, 1.f, 1.f, 500.f );

	glColor4f( 1.f, 1.f, 1.f, 1.f );

	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It remains active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.
	stream = aiGetPredefinedLogStream( aiDefaultLogStream_STDOUT, NULL );
	aiAttachLogStream( &stream );

	// ... same procedure, but this stream now writes the
	// log messages to assimp_log.txt
	stream = aiGetPredefinedLogStream( aiDefaultLogStream_FILE, "assimp_log.txt" );
	aiAttachLogStream( &stream );

	// the model name can be specified on the command line. If none
	// is specified, we try to locate one of the more expressive test
	// models from the repository (/models-nonbsd may be missing in
	// some distributions so we need a fallback from /models!).
//	loadasset("../../resources/Engine1-Data/stillscene1b.dae");/*
	if( 0 != loadasset( argc >= 2 ? argv[1] : "../../resources/Engine1-Data/stillscene1b.dae" ) ) {
		if( argc != 1 || 0 != loadasset( "../../../../test/models-nonbsd/X/dwarf.x" ) && 0 != loadasset( "../../test/models/X/Testwuson.X" ) ) {
			return -1;
		}
	}

	glClearColor( 0.1f, 0.1f, 0.1f, 1.f );

	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );  // Uses default lighting parameters

	glEnable( GL_DEPTH_TEST );

	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
	glEnable( GL_NORMALIZE );

	// XXX docs say all polygons are emitted CCW, but tests show that some aren't.
	if( getenv( "MODEL_IS_BROKEN" ) )
		glFrontFace( GL_CW );

	glColorMaterial( GL_FRONT_AND_BACK, GL_DIFFUSE );

	camera.Move( F3dVector( 0, 0, 3 ) ); // init cam pos

	// Start game loop
	while( window.IsOpened() ) {
		// Process events
		sf::Event event;

		while( window.PollEvent( event ) ) {
			// Close window : exit
			if( event.Type == sf::Event::Closed )
				window.Close();

			// Escape key : exit
			if( ( event.Type == sf::Event::KeyPressed ) && ( event.Key.Code == sf::Keyboard::Escape ) )
				window.Close();

			if( ( event.Type == sf::Event::KeyPressed ) && ( event.Key.Code == sf::Keyboard::A ) ) {
				camera.StrafeRight( -0.1 );
			}

			if( ( event.Type == sf::Event::KeyPressed ) && ( event.Key.Code == sf::Keyboard::D ) ) {
				camera.StrafeRight( 0.1 );
			}

			if( ( event.Type == sf::Event::KeyPressed ) && ( event.Key.Code == sf::Keyboard::W ) ) {
				camera.MoveForward( -0.1 );
			}

			if( ( event.Type == sf::Event::KeyPressed ) && ( event.Key.Code == sf::Keyboard::S ) ) {
				camera.MoveForward( 0.1 );
			}

			// Adjust the viewport when the window is resized
			if( event.Type == sf::Event::Resized )
				glViewport( 0, 0, event.Size.Width, event.Size.Height );
		}

		//// Draw the background
		//window.SaveGLStates();
		//window.RestoreGLStates();

		// Activate the window before using OpenGL commands.
		// This is useless here because we have only one window which is
		// always the active one, but don't forget it if you use multiple windows
		window.SetActive();

		// Clear the depth buffer
		glClear( GL_DEPTH_BUFFER_BIT );

		glMatrixMode( GL_MODELVIEW );

		display();
		reshape( 800, 600 );



		//// Draw some text on top of our OpenGL object
		//window.SaveGLStates();
		//sf::Text text("SFML / OpenGL demo");
		//text.SetPosition(250.f, 450.f);
		//text.SetColor(sf::Color(255, 255, 255, 170));
		//window.Draw(text);
		//window.RestoreGLStates();

		// Finally, display the rendered frame on screen
		window.Display();
	}

	// cleanup - calling 'aiReleaseImport' is important, as the library
	// keeps internal resources until the scene is freed again. Not
	// doing so can cause severe resource leaking.
	aiReleaseImport( scene );

	// We added a log stream to the library, it's our job to disable it
	// again. This will definitely release the last resources allocated
	// by Assimp.
	aiDetachAllLogStreams();
	return 0;
}

#if ( defined( __GNUC__ ) || defined( __GNUG__ ) )
#pragma GCC diagnostic pop
#endif
