
#include <QGLWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QTime>
#include <iostream>
#include "controller.hpp"
#include "ship.hpp"

class Window : public QGLWidget
{
	Q_OBJECT

public:
	Window( QWidget * parent = 0 )
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

public slots:
	void update()
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

protected:
	virtual void keyPressEvent( QKeyEvent * event )
	{
		if ( !event->isAutoRepeat() )
		{
			controller.keydown( event->key() );
		}
	}
	virtual void keyReleaseEvent( QKeyEvent * event )
	{
		if ( !event->isAutoRepeat() )
		{
			controller.keyup( event->key() );
		}
	}
	virtual void paintGL()
	{
		std::cout << ship.xpos() << ", " << ship.zpos() << ", " << ship.speed() << std::endl;
	}

private:
	Ship ship;
	Controller controller;
	QTimer * timer;
	QTime * time;
};
