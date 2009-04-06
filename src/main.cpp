#include <QApplication>
#include "window.hpp"

int main ( int argc, char** argv )
{
	srand(time(0));
	QApplication app( argc, argv );
	Window window;
	window.show();
	return app.exec();
}
