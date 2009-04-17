//
// This file is part of the game Skyways.
// Copyright (C) 2009 Mark Nevill
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

#include <QCoreApplication>
#include <QKeyEvent>
#include <QTimer>
#include <QTime>
#include <iostream>
#include <cmath>
#include "textprinter.hpp"
#include "controller.hpp"
#include "qtwindow.hpp"

void quitFunc()
{
	qApp->quit();
}

Window::Window( QWidget * parent )
	: QGLWidget( parent )
{
	timer = new QTimer( this );
	connect( timer, SIGNAL(timeout()), this, SLOT(update()) );
	timer->start( 0 );
	time = new QTime();
	setFocusPolicy( Qt::StrongFocus );
	std::auto_ptr< TextPrinter > printer( new TextPrinter( "DejaVuSans.ttf" ) );
	controller.reset( new Controller( &quitFunc, printer ) );
	QStringList argv = qApp->arguments();
	if ( argv.size() > 1 )
		controller->loadWorld( argv.at(1).toStdString() );
	else
		controller->generateWorld();
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
		controller->update( time->restart() );
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
			controller->keydown( key );
	}
}

void Window::keyReleaseEvent( QKeyEvent * event )
{
	if ( !event->isAutoRepeat() )
	{
		int key = mapKey( event->key() );
		if ( key > 0 )
			controller->keyup( key );
	}
}

void Window::initializeGL()
{
	controller->initialize();
}

void Window::resizeGL( int width, int height )
{
	controller->resize( width, height );
}

void Window::paintGL()
{
	controller->draw();
}
