#include "window.hpp"
#include <QApplication>

int main ( int argc, char** argv )
{
	QApplication app( argc, argv );
	Window window;
	window.show();
	return app.exec();
}
