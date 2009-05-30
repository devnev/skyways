#                                               -*- Autoconf -*-
#                                               vim: ft=m4
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/configuration.hpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
PKG_PROG_PKG_CONFIG

# Checks for libraries.

AC_CHECK_LIB([GL], [glGetString])
AC_CHECK_LIB([GLEW], [glewInit])
AC_CHECK_LIB([GLU], [gluGetString])
PKG_CHECK_MODULES([FTGL],[ftgl >= 2.1])
dnl FIXME: check for boost
dnl AC_CHECK_LIB([boost_filesystem], [main])
dnl AC_CHECK_LIB([boost_program_options], [main])

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
	PKG_CHECK_MODULES([Qt],[QtCore >= 4.5.0 QtGui >= 4.5.0 QtOpenGL >= 4.5.0])
	AC_CHECK_LIB([pthread],[pthread_create])
elif test "x$qt_backend" == "xtest" ; then
	PKG_CHECK_MODULES([Qt],[QtCore >= 4.5.0 QtGui >= 4.5.0 QtOpenGL >= 4.5.0],[qt_backend="yes"],[qt_backend="no"])
	if test "x$qt_backend" = "xyes" ; then
		AC_CHECK_LIB([pthread],[pthread_create],[qt_backend="yes"],[qt_backend="no"])
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
	AC_CHECK_LIB([glut],[glutMainLoop])
elif test "x$glut_backend" == "xtest" ; then
	AC_CHECK_LIB([glut],[glutMainLoop],[glut_backend="yes"],[glut_backend="no"])
fi
AC_SUBST(glut_backend)
if test "x$glut_backend" = "xyes" ; then
	programs="SkywaysGlut $programs"
fi

AC_SUBST(programs)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
