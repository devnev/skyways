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

#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <QGLWidget>
#include <memory>

class Controller;

class QKeyEvent;
class QTimer;
class QTime;

class Window : public QGLWidget
{
	Q_OBJECT

public:
	Window( QWidget * parent = 0 );
	virtual ~Window();

public slots:
	void update();

protected:
	virtual void keyPressEvent( QKeyEvent * event );
	virtual void keyReleaseEvent( QKeyEvent * event );
	virtual void initializeGL();
	virtual void resizeGL( int width, int height );
	virtual void paintGL();

private:
	int mapKey( int key );

	std::auto_ptr< Controller > controller;
	QTimer * timer;
	QTime * time;
};

#endif // _WINDOW_HPP_
