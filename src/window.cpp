#include <QKeyEvent>
#include <QTimer>
#include <QTime>
#include <iostream>
#include <cmath>
#include "window.hpp"

Window::Window( QWidget * parent )
	: QGLWidget( parent )
	, ship( 100 )
	, world()
	, controller( &ship, 5, 3 )
{
	timer = new QTimer( this );
	connect( timer, SIGNAL(timeout()), this, SLOT(update()) );
	timer->start( 0 );
	time = new QTime();
	setFocusPolicy( Qt::StrongFocus );
}

Window::~Window()
{
}

void Window::update()
{
	if ( time->isNull() )
	{
		time->start();
		return;
	}
	else
	{
		controller.update( time->restart() );
	}
	( (QWidget*)this )->update();
}

void Window::keyPressEvent( QKeyEvent * event )
{
	if ( !event->isAutoRepeat() )
	{
		controller.keydown( event->key() );
	}
}

void Window::keyReleaseEvent( QKeyEvent * event )
{
	if ( !event->isAutoRepeat() )
	{
		controller.keyup( event->key() );
	}
}

void Window::initializeGL()
{
	glClearColor( 0, 0, 0, 0 );
	glClearDepth( 1.0 );
	glEnable( GL_DEPTH_TEST );
	for (size_t i = 0; i < 100; ++i)
	{
		world.add(Element(
			rand() % 8 - 4,
			((double)(rand() % 5))/4 - 0.5,
			rand() % 400,
			rand() % 20 + 2
		));
	}
}

void Window::resizeGL( int width, int height )
{
     glViewport( 0, 0, width, height );

     glMatrixMode( GL_PROJECTION );
     glLoadIdentity();
	 double fH = tan( 30.0 / 180.0 * 3.14159265358979323846 );
	 double fW = ( (double)width ) / ( (double)height ) * fH;
     glFrustum( -fW, fW, -fH, fH, 1.0, 100.0 );
     glMatrixMode( GL_MODELVIEW );
}

void Window::paintGL()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	glTranslated( ship.xpos(), ship.ypos() - 2, -4 );
	glColor3f( 1, 0, 0 );
	ship.glDraw();
	glLoadIdentity();
	glColor3f( 0.8f, 1, 1 );
	glTranslatef( 0, -2, ship.zpos() - 4 );
	world.glDraw();
	std::cout << ship.xpos() << ", " << ship.zpos() << ", " << ship.speed() << std::endl;
}
