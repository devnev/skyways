#include <QKeyEvent>
#include <QTimer>
#include <QTime>
#include <iostream>
#include <cmath>
#include "window.hpp"

Window::Window( QWidget * parent )
	: QGLWidget( parent )
	, ship( 100 )
	, world( 10 )
	, controller( &ship, 8, 3 )
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

int Window::mapKey( int key )
{
	switch ( key )
	{
	case Qt::Key_Left: return Controller::STRAFE_L_KEY;
	case Qt::Key_Right: return Controller::STRAFE_R_KEY;
	case Qt::Key_Up: return Controller::ACCEL_KEY;
	case Qt::Key_Down: return Controller::DECEL_KEY;
	case Qt::Key_Space: return Controller::JUMP_KEY;
	default: return 0;
	}
}

void Window::keyPressEvent( QKeyEvent * event )
{
	if ( !event->isAutoRepeat() )
	{
		if ( event->key() == Qt::Key_Escape )
			QCoreApplication::instance()->quit();
		else
		{
			int key = mapKey( event->key() );
			if ( key > 0 )
				controller.keydown( key );
		}
	}
}

void Window::keyReleaseEvent( QKeyEvent * event )
{
	if ( !event->isAutoRepeat() )
	{
		int key = mapKey( event->key() );
		if ( key > 0 )
			controller.keyup( key );
	}
}

void Window::initializeGL()
{
	glClearColor( 0, 0, 0, 0 );
	glClearDepth( 1.0 );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	glEnable( GL_LIGHTING );
	glShadeModel( GL_SMOOTH );
	glEnable( GL_COLOR_MATERIAL );

	float ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float position[] = { 0.0f, 4.0f, -2.0f, 1.0f };
	glLightfv( GL_LIGHT1, GL_AMBIENT, ambient );
	glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuse );
	glLightfv( GL_LIGHT1, GL_POSITION, position );
	glEnable( GL_LIGHT1 );

	world.loadBlocks();

	for (size_t i = 0; i < 100; ++i)
	{
		world.add(Element(
			rand() % 8 - 4,
			((double)(rand() % 5))/4 - 0.5,
			rand() % 400,
			rand() % 20 + 2,
			world.block( "" )
		));
	}
	world.optimize();

	std::cout << "Loaded " << world.blocksLoaded() << " blocks.\n";
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
	world.glDraw( ship.zpos() - 4 );
	std::cout << ship.xpos() << ", " << ship.zpos() << ", " << ship.speed() << ", " << world.elementsDrawn() << std::endl;
}
