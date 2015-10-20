#!/bin/bash

logfile=${PWD}/release.$(date +%y-%m-%d-%T).log

# This script attempts to generate a release like tarball
# of the project in the absense of full autotooling support
#

# Since the autotooling support is missing for now
# we'll create an archive with git-archive.
if [ ! -d .git ]; then
	echo "Fatal: Not a git project. Bailing"
	exit 1
fi

# Determine the library name and version.
PROJVER=libdfp-$(git describe --tags)

# Generate the archive.
if ! git archive --format=tar --prefix=${PROJVER}/ HEAD > ${PROJVER}.tar ; then
	echo "Failed to archive"
	exit 1
fi

# Generate a changelog and append it.
mkdir ${PROJVER}
./generate-changelog.sh > ${PROJVER}/ChangeLog.md
tar rf ${PROJVER}.tar ${PROJVER}/ChangeLog.md

# Remove any files which needn't exist.
tar f ${PROJVER}.tar \
    --delete ${PROJVER}/{make-release.sh,generate-changelog.sh,ChangeLog.old.md}
rm -rf ${PROJVER}
gzip -f ${PROJVER}.tar

tarballs="${PROJVER}.tar.gz"


tar xf ${PROJVER}.tar.gz --warning=no-timestamp
# Attempt a VPATH build.
cd ${PROJVER}

# Update timestamps to prevent odd issues.
touch configure.ac aclocal.m4 configure Makefile.am Makefile.in

mkdir _build
cd _build

echo "Running configure"
if ! ../configure >> ${logfile} 2>&1 ; then
	echo "Failed to configure"
	exit 1
fi

echo "Running make"
if ! make -j8 >> ${logfile} 2>&1 ; then
	echo "Failed to make"
	exit 1
fi

echo "Running make check"
if ! make check >> ${logfile} 2>&1 ; then
	echo "Failed to check"
fi

cd ../..

rm -rf libdfp

echo "Found tarballs: $tarballs"

# Generate some popular checksums for the tarball.
for tarball in ${tarballs}; do
	for sum in sha256 md5; do
		echo "Generating ${tarball}.${sum}"
		${sum}sum ${tarball} > ${tarball}.${sum}
	done
done


# TODO: Support signing these releases

exit 0
