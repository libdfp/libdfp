#!/bin/bash

if test -z "$1"; then
	echo "$1 must be the test-*.conf file name you wish to debug."
fi
	source debug-test.conf
	source $1

# Depending on whether a [system|standalone] loader or a non-installed glibc
# build's loader this will be invoked differently.
$DBG -x ${GDB_SCRIPT} -d ${LIBDFP_SRC} ${APP_OR_LOADER}
