#!/bin/sh
mkdir -p build && \
	cd build && \
	../configure --prefix=`pwd` && \
	make $MAKEOPTS pkgdatadir=`pwd` && \
	make $MAKEOPTS install pkgdatadir=`pwd`
