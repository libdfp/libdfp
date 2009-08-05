#!/bin/bash

if test -z "$1"; then
	echo "$1 must be the test-*.conf file name you wish to debug."
fi
	source test-debug.conf
	source $1

$DBG -x ${GDB_SCRIPT} -d ${LIBDFP_SRC} ${GLIBC_BUILD}/elf/ld.so
