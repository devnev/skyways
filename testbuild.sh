#!/bin/sh
mkdir -p build && \
	cd build && \
	../configure --prefix=`pwd` && \
	make appdatadir=.
