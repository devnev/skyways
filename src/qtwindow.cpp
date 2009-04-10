#include <QCoreApplication>
#include <QKeyEvent>
#include <QTimer>
#include <QTime>
#include <iostream>
#include <cmath>
#include "qtwindow.hpp"

void quitFunc()
{
	qApp->quit();
}

Window::Window( QWidget * parent )
	: QGLWidget( parent ), controller( &quitFunc )
{
	timer = new QTimer( this );
	connect( timer, SIGNAL(timeout()), this, SLOT(update()) );
	timer->start( 0 );
	time = new QTime();
	setFocusPolicy( Qt::StrongFocus );
	QStringList argv = qApp->arguments();
	if ( argv.size() > 1 )
		controller.loadWorld( argv.at(1).toStdString() );
	else
		controller.generateWorld();
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
	case Qt::Key_Escape: return Controller::QUIT_KEY;
	default: return 0;
	}
}

void Window::keyPressEvent( QKeyEvent * event )
{
	if ( !event->isAutoRepeat() )
	{
		int key = mapKey( event->key() );
		if ( key > 0 )
			controller.keydown( key );
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
	controller.initialize();
}

void Window::resizeGL( int width, int height )
{
	controller.resize( width, height );
}

void Window::paintGL()
{
	controller.draw();
}
