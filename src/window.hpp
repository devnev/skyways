
#include <QGLWidget>
#include "controller.hpp"
#include "ship.hpp"
#include "world.hpp"

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
	Ship ship;
	World world;
	Controller controller;
	QTimer * timer;
	QTime * time;
};
