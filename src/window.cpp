
#include <QKeyEvent>
#include <QTimer>
#include <QTime>
#include <iostream>
#include "window.hpp"

Window::Window( QWidget * parent )
	: QGLWidget( parent )
	, ship( 100 )
	, controller( &ship, 1.2, 1.2 )
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
}

void Window::resizeGL( int width, int height )
{
     int side = qMin( width, height );
     glViewport( ( width - side ) / 2, ( height - side ) / 2, side, side );

     glMatrixMode( GL_PROJECTION );
     glLoadIdentity();
     glFrustum( -1.0, +1.0, -1.0, 1.0, 5.0, 60.0 );
     glMatrixMode( GL_MODELVIEW );
}

void Window::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glTranslated( ship.xpos(), ship.zpos(), -40 );
	glBegin( GL_QUADS );
		glVertex3d( 0, 0, 0 );
		glVertex3d( 1, 0, 0 );
		glVertex3d( 1, -1, 0 );
		glVertex3d( 0, -1, 0 );
	glEnd();
	std::cout << ship.xpos() << ", " << ship.zpos() << ", " << ship.speed() << std::endl;
}
