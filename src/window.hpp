
#include <QGLWidget>
#include "controller.hpp"
#include "ship.hpp"

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
	virtual void paintGL();

private:
	Ship ship;
	Controller controller;
	QTimer * timer;
	QTime * time;
};
