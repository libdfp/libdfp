#!/bin/bash

targets="x86_64 i686 powerpc powerpc64 powerpc64le s390 s390x"

powerpc_extra="no power8"
powerpc64_extra=$powerpc_extra
powerpc64le_extra="power8 power9"
s390x_extra="no z14"
s390_extra="no z14"

s390_CFLAGS="-m31"
i686_CFLAGS="-m32"

# Use the toolchains from build_many_glibcs.py (provided by glibc)
bmg_dir=$1/install/compilers/

# Reuse x86-64 and s390x toolchains for 3x bit mode.
x86_64_prefix=${bmg_dir}x86_64-linux-gnu/bin/x86_64-glibc-linux-gnu-
i686_prefix=${x86_64_prefix}
powerpc_prefix=${bmg_dir}powerpc-linux-gnu/bin/powerpc-glibc-linux-gnu-
powerpc64_prefix=${bmg_dir}powerpc64-linux-gnu/bin/powerpc64-glibc-linux-gnu-
powerpc64le_prefix=${bmg_dir}powerpc64le-linux-gnu/bin/powerpc64le-glibc-linux-gnu-
s390_prefix=${bmg_dir}s390x-linux-gnu/bin/s390x-glibc-linux-gnu-
s390x_prefix=${s390_prefix}

function do_one()
{
  tc=$2
  cpu=$4
  targetcpu=""
  host=${1}
  dsuf=${1}
  extra=${3}
  if [ ! -z $4 ] ; then
    dsuf=${1}-${4}
    targetcpu=--with-cpu=$4
  fi
  mkdir -p build-${dsuf} &> /dev/null || { echo "mkdir fail ${dsuf}"; return 1; }
  mkdir -p install-${dsuf} &> /dev/null || { echo "mkdir fail ${dsuf}"; return 1; }
  cd build-${dsuf}
  ${SRCDIR}/configure --host=${host}-linux-gnu ${targetcpu} CC="${tc}gcc $extra" CXX="${tc}g++ $extra" 2>conf.err > conf.log && \
        echo "PASS config ${dsuf}" || { echo "FAIL config ${dsuf}"; return 1; }
  make -j20 2> make.err > make.log && echo "PASS compile ${dsuf}" || { echo "FAIL compile ${dsuf}"; return 1; }
  make -j20 cross-check 2> cross-check.err > cross-check.log && echo "PASS cross-check ${dsuf}" || { echo "FAIL cross-check ${dsuf}"; return 1; }
  make install DESTDIR=../install-${dsuf} 2> install.err > install.log && echo "PASS install ${dsuf}" || { echo "FAIL install ${dsuf}"; return 1; }
}

# Usage build_all.sh [build-many-glibcs scratch dir] [libdfp root dir]

# Sanity check to ensure all the cross-toolchains exist
fail=0
for t in $targets; do
  tc=$(eval echo \$${t}_prefix)
  if [ ! -e ${tc}gcc ]; then
    echo "Cannot find ${tc}gcc";
    fail=1
  fi
done

if [ $fail != 0 ]; then
   exit 1
fi

if [ ! -e "$2/configure" ]; then
   echo "Cannot find configure."
   exit 1
fi

SRCDIR=$(realpath $2)

for t in $targets; do
  tc=$(eval echo \$${t}_prefix)
  cflags=$(eval echo \$${t}_CFLAGS)
  extras=$(eval echo \$${t}_extra)
  # subshell to avoid workdir issues on failure
  for e in "" $extras; do
    (do_one $t $tc "$cflags" $e)
  done
done
