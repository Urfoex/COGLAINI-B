
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main() {
	// Create the main window
	sf::RenderWindow window( sf::VideoMode( 800, 600 ), "SFML OpenGL", sf::Style::Default, sf::ContextSettings( 32 ) );

	// Create a sprite for the background
	sf::Texture backgroundTexture;

	if( !backgroundTexture.LoadFromFile( "../../resources/Images/background.jpg" ) )
		return EXIT_FAILURE;

	sf::Sprite background( backgroundTexture );

	// Load an OpenGL texture.
	// We could directly use a sf::Texture as an OpenGL texture (with its Bind() member function),
	// but here we want more control on it (generate mipmaps, ...) so we create a new one from an image
	GLuint texture = 0;
	{
		sf::Image image;

		if( !image.LoadFromFile( "../../resources/Images/texture.jpg" ) )
			return EXIT_FAILURE;

		glGenTextures( 1, &texture );
		glBindTexture( GL_TEXTURE_2D, texture );
		gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, image.GetWidth(), image.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, image.GetPixelsPtr() );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	}

	// Enable Z-buffer read and write
	glEnable( GL_DEPTH_TEST );
	glDepthMask( GL_TRUE );
	glClearDepth( 1.f );

	// Setup a perspective projection
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 90.f, 1.f, 1.f, 500.f );

	// Bind our texture
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, texture );
	glColor4f( 1.f, 1.f, 1.f, 1.f );

	// Create a clock for measuring the time elapsed
	sf::Clock clock;

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

			// Adjust the viewport when the window is resized
			if( event.Type == sf::Event::Resized )
				glViewport( 0, 0, event.Size.Width, event.Size.Height );
		}

		// Draw the background
		window.SaveGLStates();
		window.Draw( background );
		window.RestoreGLStates();

		// Activate the window before using OpenGL commands.
		// This is useless here because we have only one window which is
		// always the active one, but don't forget it if you use multiple windows
		window.SetActive();

		// Clear the depth buffer
		glClear( GL_DEPTH_BUFFER_BIT );

		// We get the position of the mouse cursor, so that we can move the box accordingly
		float x =  sf::Mouse::GetPosition( window ).x * 200.f / window.GetWidth()  - 100.f;
		float y = -sf::Mouse::GetPosition( window ).y * 200.f / window.GetHeight() + 100.f;

		// Apply some transformations
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glTranslatef( x, y, -100.f );
		glRotatef( clock.GetElapsedTime() * 0.05f, 1.f, 0.f, 0.f );
		glRotatef( clock.GetElapsedTime() * 0.03f, 0.f, 1.f, 0.f );
		glRotatef( clock.GetElapsedTime() * 0.09f, 0.f, 0.f, 1.f );

		// Draw a cube
		float size = 20.f;
		glBegin( GL_QUADS );

		glTexCoord2f( 0, 0 );
		glVertex3f( -size, -size, -size );
		glTexCoord2f( 0, 1 );
		glVertex3f( -size,  size, -size );
		glTexCoord2f( 1, 1 );
		glVertex3f( size,  size, -size );
		glTexCoord2f( 1, 0 );
		glVertex3f( size, -size, -size );

		glTexCoord2f( 0, 0 );
		glVertex3f( -size, -size, size );
		glTexCoord2f( 0, 1 );
		glVertex3f( -size,  size, size );
		glTexCoord2f( 1, 1 );
		glVertex3f( size,  size, size );
		glTexCoord2f( 1, 0 );
		glVertex3f( size, -size, size );

		glTexCoord2f( 0, 0 );
		glVertex3f( -size, -size, -size );
		glTexCoord2f( 0, 1 );
		glVertex3f( -size,  size, -size );
		glTexCoord2f( 1, 1 );
		glVertex3f( -size,  size,  size );
		glTexCoord2f( 1, 0 );
		glVertex3f( -size, -size,  size );

		glTexCoord2f( 0, 0 );
		glVertex3f( size, -size, -size );
		glTexCoord2f( 0, 1 );
		glVertex3f( size,  size, -size );
		glTexCoord2f( 1, 1 );
		glVertex3f( size,  size,  size );
		glTexCoord2f( 1, 0 );
		glVertex3f( size, -size,  size );

		glTexCoord2f( 0, 1 );
		glVertex3f( -size, -size,  size );
		glTexCoord2f( 0, 0 );
		glVertex3f( -size, -size, -size );
		glTexCoord2f( 1, 0 );
		glVertex3f( size, -size, -size );
		glTexCoord2f( 1, 1 );
		glVertex3f( size, -size,  size );

		glTexCoord2f( 0, 1 );
		glVertex3f( -size, size,  size );
		glTexCoord2f( 0, 0 );
		glVertex3f( -size, size, -size );
		glTexCoord2f( 1, 0 );
		glVertex3f( size, size, -size );
		glTexCoord2f( 1, 1 );
		glVertex3f( size, size,  size );

		glEnd();

		// Draw some text on top of our OpenGL object
		window.SaveGLStates();
		sf::Text text( "SFML / OpenGL demo" );
		text.SetPosition( 250.f, 450.f );
		text.SetColor( sf::Color( 255, 255, 255, 170 ) );
		window.Draw( text );
		window.RestoreGLStates();

		// Finally, display the rendered frame on screen
		window.Display();
	}

	// Don't forget to destroy our texture
	glDeleteTextures( 1, &texture );

	return EXIT_SUCCESS;
}




/*
 *
 *


#include <iostream>
using namespace std;

#define GL_GLEXT_PROTOTYPES 1
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window/Event.hpp>

void setupOpenGL() {
	// Set color and depth clear value
	glClearDepth( 1.f );
	glClearColor( 0.f, 0.f, 0.f, 0.f );

	// Enable Z-buffer read and write
	glEnable( GL_DEPTH_TEST );
	glDepthMask( GL_TRUE );

	// Setup a perspective projection
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 90.f, 1.f, 1.f, 500.f );

	clog << glGetString( GL_VERSION ) << endl;
}

void drawSomething( float elapsedTime ) {
	// Clear color and depth buffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Apply some transformations
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef( 0.f, 0.f, -200.f );
	glRotatef( elapsedTime * 50, 1.f, 0.f, 0.f );
	glRotatef( elapsedTime * 30, 0.f, 1.f, 0.f );
	glRotatef( elapsedTime * 90, 0.f, 0.f, 1.f );

	// Draw a cube
	glBegin( GL_QUADS );

	glVertex3f( -50.f, -50.f, -50.f );

	glColor4f( .5f, .0f, .5f, .5f);

	glVertex3f( -50.f,  50.f, -50.f );
	glColor4f( .5f, .0f, .5f, .5f);
	glVertex3f( 50.f,  50.f, -50.f );
	glColor4f( .0f, .5f, .5f, .5f);
	glVertex3f( 50.f, -50.f, -50.f );

	glColor4f( .0f, .5f, .5f, .5f);

	glVertex3f( -50.f, -50.f, 50.f );
	glVertex3f( -50.f,  50.f, 50.f );
	glVertex3f( 50.f,  50.f, 50.f );
	glVertex3f( 50.f, -50.f, 50.f );

	glColor4f( .25f, .25f, .25f, .25f);

	glVertex3f( -50.f, -50.f, -50.f );
	glVertex3f( -50.f,  50.f, -50.f );
	glVertex3f( -50.f,  50.f,  50.f );
	glVertex3f( -50.f, -50.f,  50.f );

	glColor4f( .75f, .75f, .75f, .75f);

	glVertex3f( 50.f, -50.f, -50.f );
	glVertex3f( 50.f,  50.f, -50.f );
	glVertex3f( 50.f,  50.f,  50.f );
	glVertex3f( 50.f, -50.f,  50.f );

	glVertex3f( -50.f, -50.f,  50.f );
	glVertex3f( -50.f, -50.f, -50.f );
	glVertex3f( 50.f, -50.f, -50.f );
	glVertex3f( 50.f, -50.f,  50.f );

	glVertex3f( -50.f, 50.f,  50.f );
	glVertex3f( -50.f, 50.f, -50.f );
	glVertex3f( 50.f, 50.f, -50.f );
	glVertex3f( 50.f, 50.f,  50.f );

	glEnd();
}

int main( int argc, char** argv ) {
	sf::Window App( sf::VideoMode( 800, 600, 32 ), "COGLET" );
	App.EnableVerticalSync( true);
	sf::Clock Clock;

	setupOpenGL();

	// Start game loop
	while( App.IsOpened() ) {
		// Process events
		sf::Event Event;

		while( App.PollEvent( Event ) ) {
			switch( Event.Type ) {
			case sf::Event::Closed:	// Close window : exit
				App.Close();
				break;
			case sf::Event::KeyPressed:	// Escape key : exit
				switch( Event.Key.Code ) {
					case sf::Keyboard::Key::Escape:
					App.Close();
					break;
				default:
					break;
				}
			case sf::Event::Resized:	// Resize event : adjust viewport
				glViewport( 0, 0, Event.Size.Width, Event.Size.Height );
				break;
			default:
				break;
			}
		}

		// Set the active window before using OpenGL commands
		// It's useless here because active window is always the same,
		// but don't forget it if you use multiple windows or controls
		App.SetActive();

		drawSomething( Clock.GetElapsedTime() );

		// Finally, display rendered frame on screen
		App.Display();
	}

	return 0;
}
*/
