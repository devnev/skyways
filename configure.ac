#                                               -*- Autoconf -*-
#                                               vim: ft=m4
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([skyways], [0.0.1], [mark.nevill@gmail.com])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/configuration.hpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
PKG_PROG_PKG_CONFIG

# Checks for libraries.

AC_CHECK_LIB([GL], [glGetString])
AC_CHECK_LIB([GLEW], [glewInit])
PKG_CHECK_MODULES([FTGL],[ftgl >= 2.1])
AX_BOOST_BASE([1.35.0])
AX_BOOST_FILESYSTEM
AX_BOOST_PROGRAM_OPTIONS

programs=""

AC_ARG_ENABLE([qt], AS_HELP_STRING([--enable-qt], [Enable Qt backend (default: test)]),
	[	if test "x$enableval" = "xyes" ; then qt_backend=yes
		elif test "x$enableval" = "xtest" ; then qt_backend=test
		elif test "x$enableval" = "xno" ; then qt_backend=no
		else echo "Error: unknown --enable-qt option $enableval" ; exit 1
		fi
	],
	[	qt_backend=test
	])
if test "x$qt_backend" == "xyes" ; then
	PKG_CHECK_MODULES([QT],[QtCore >= 4.5.0 QtGui >= 4.5.0 QtOpenGL >= 4.5.0])
	AC_CHECK_LIB([pthread],[pthread_create],[QT_LIBS="-lpthread $QT_LIBS"])
	AC_CHECK_LIB([GLU], [gluGetString],[QT_LIBS="-lGLU $QT_LIBS"])
elif test "x$qt_backend" == "xtest" ; then
	PKG_CHECK_MODULES([QT],[QtCore >= 4.5.0 QtGui >= 4.5.0 QtOpenGL >= 4.5.0],[qt_backend="yes"],[qt_backend="no"])
	if test "x$qt_backend" = "xyes" ; then
		AC_CHECK_LIB([pthread],[pthread_create],[QT_LIBS="-lpthread $QT_LIBS"; qt_backend="yes"],[qt_backend="no"])
	fi
	if test "x$qt_backend" = "xyes" ; then
		AC_CHECK_LIB([GLU], [gluGetString],[QT_LIBS="-lGLU $QT_LIBS"; qt_backend="yes"],[qt_backend="no"])
	fi
fi
AC_SUBST(qt_backend)
if test "x$qt_backend" = "xyes" ; then
	programs="SkywaysQt $programs"
fi

AC_ARG_ENABLE([sdl], AS_HELP_STRING([--enable-sdl], [Enable SDL backend (default: test)]),
	[	if test "x$enableval" = "xyes" ; then sdl_backend=yes
		elif test "x$enableval" = "xtest" ; then sdl_backend=test
		elif test "x$enableval" = "xno" ; then sdl_backend=no
		else echo "Error: unknown --enable-sdl option $enableval" ; exit 1
		fi
	],
	[	sdl_backend=test
	])
if test "x$sdl_backend" == "xyes" ; then
	PKG_CHECK_MODULES([SDL],[sdl >= 1.2.13])
elif test "x$sdl_backend" == "xtest" ; then
	PKG_CHECK_MODULES([SDL],[sdl >= 1.2.13],[sdl_backend="yes"],[sdl_backend="no"])
fi
AC_SUBST(sdl_backend)
if test "x$sdl_backend" = "xyes" ; then
	programs="SkywaysSdl $programs"
fi

AC_ARG_ENABLE([glut], AS_HELP_STRING([--enable-glut], [Enable GLUT backend (default: test)]),
	[	if test "x$enableval" = "xyes" ; then glut_backend=yes
		elif test "x$enableval" = "xtest" ; then glut_backend=test
		elif test "x$enableval" = "xno" ; then glut_backend=no
		else echo "Error: unknown --enable-glut option $enableval" ; exit 1
		fi
	],
	[	glut_backend=test
	])
if test "x$glut_backend" == "xyes" ; then
	AC_CHECK_LIB([glut],[glutMainLoop],[GLUT_LIBS="-lglut"])
elif test "x$glut_backend" == "xtest" ; then
	AC_CHECK_LIB([glut],[glutMainLoop],[GLUT_LIBS="-lglut"; glut_backend="yes"],[glut_backend="no"])
fi
AC_SUBST(glut_backend)
AC_SUBST(GLUT_LIBS)
if test "x$glut_backend" = "xyes" ; then
	programs="SkywaysGlut $programs"
fi

if test -z "$programs" ; then
	echo "Error: no backends will be built, aborting."
	exit 1
fi

AC_SUBST(programs)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo "Configuration complete. Backends to be built:"
echo "        Qt backend...   $qt_backend"
echo "        SDL backend...  $sdl_backend"
echo "        GLUT backend... $glut_backend"
