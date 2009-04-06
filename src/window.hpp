#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <QGLWidget>
#include "controller.hpp"

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

	Controller controller;
	QTimer * timer;
	QTime * time;
};

#endif // _WINDOW_HPP_
