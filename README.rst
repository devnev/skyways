SkyWays
=======

Skyways is a toy project to create a game roughly based on the DOS game called
"Skyroads". The aim is not to recreate the original game, just make something
similarly fun, and to explore possibilities along the way.

Depends
-------

One or more of: SDL, Qt4, freeglut, OpenGlut.

All of: Boost, OpenGL, FTGL

To explicitly select the backends to be built, use::

	./configure --enable-{qt,sdl,glut}={yes,no,test}

Build
-----

Commands to build & install from sources::

	autoreconf
	./configure
	make
	make install
