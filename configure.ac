#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Autoconf 2.59 required because recursively configured 'libdecnumber' library
# has a hard-coded 2.59 requirement and Libdfp doesn't really care, but can't
# control libdecnumber.
AC_PREREQ(2.59)
AC_INIT(libdfp, 1.0.8, rsa@us.ibm.com)
AC_CONFIG_SRCDIR([printf_dfp.c])

# We don't use Automake so this is necessary.
AC_CONFIG_HEADER([config.h])

# This indicates where config.guess and config.sub are located.  Those scripts
# are used by the AC_CANONICAL_* macros to determine platform specific data.
AC_CONFIG_AUX_DIR([scripts])

# Like GLIBC we want to prevent building/configuration in the source
# directory.
if test "`cd $srcdir; /bin/pwd`" = "`/bin/pwd`"; then
       AC_MSG_ERROR([you must configure in a separate build directory])
fi

# Defines host_cpu, host_vendor, and host_os variables.  This is where the
# code will run.
AC_CANONICAL_HOST
config_os=$host_os
#machine=$host_cpu
machine=$build_cpu

# Only allow a Linux host OS.  We don't support building libdfp on non-Linux
# systems.
if test "$config_os" != linux-gnu; then
  AC_MSG_ERROR(['linux-gnu' is the only allowed host OS])
fi

## This will fail on a pcc64 system where we're configuring for ppc.
#if test "$host_cpi" != "$target_cpu"; then
#  AC_MSG_ERROR([${PACKAGE_NAME} does not yet support cross-compliation.])
#fi

# PowerPC[64] defaults to DPD encoded DFP.  x86[_64] defaults to BID encoded DFP.
default_encoding=
case "$host_cpu" in
  s390|s390x)
    default_encoding=dpd
    ;;
  powerpc|powerpc64)
    default_encoding=dpd
    ;;
  *)
    default_encoding=bid ;;
esac

# This clones what's in libdecnumber because autoconf doesn't
# support passing unique flags to subdir configure.ac invocations
# because it is brain dead.
AC_MSG_CHECKING([for decimal floating point type encoding])
AC_ARG_ENABLE([decimal-float],
	      AS_HELP_STRING([--enable-decimal-float=@<:@yes|bid|dpd@:>@],
		[This indicates the IEEE754r Decimal Floating point type encoding to be used by libdfp.
		 Select yes or @<:@DPD|dpd@:>@ or @<:@BID|bid@:>@
		 @<:@default=dpd@:>@ for powerpc@<:@64@:>@
		 @<:@default=dpd@:>@ for s390@<:@x@:>@
		 @<:@default=bid@:>@ for x86@<:@_64@:>@.
		 ]),
	      [dnl
  case "$enable_decimal_float" in
    DPD|dpd)
      ;;
    BID|bid)
      if test "$host_cpu" = powerpc || test "$host_cpu" = powerpc64; then
        AC_MSG_ERROR([The BID encoding isn't a valid encoding for PowerPC@<:@64@:>@])
      elif test "$host_cpu" = s390 || test "$host_cpu" = s390x; then
        AC_MSG_ERROR([The BID encoding isn't a valid encoding for s390@<:@x@:>@])
      fi
      ;;
    yes|*)
       enable_decimal_float="$default_encoding"
      AC_MSG_WARN(no argument specified, using host default encoding "$default_encoding".)
      ;;
  esac],
  [enable_decimal_float="$default_encoding"])
AC_SUBST(enable_decimal_float)
AC_MSG_NOTICE(Using $enable_decimal_float decimal floating point encoding)

# Some code may need to have conditional guards based on the backend.  Give the
# developers _DPD_BACKEND or _BID_BACKEND.  Try avoiding this as much as
# possible and use sysdep file overrides.  A good place to use it is in the
# tests/ directory.
if test $enable_decimal_float = dpd; then
  AC_DEFINE_UNQUOTED(_DPD_BACKEND,1
		     ,"Defined if DPD is the backend designated by configure.")
else
  AC_DEFINE_UNQUOTED(_BID_BACKEND,1
		     ,"Defined if BID is the backend designated by configure.")
fi

# Now we know how to chose the desired backend directory, e.g.
# src/backend/libdecnumber or src/backend/libbid when we build our search
# path.  "libdecnumber" is an external project sucked into this library.  It
# is an API that can use either the DPD or BID DFP data-type encoding.

# Because the 'libdecnumber' and 'libbid' source code are out of tree they
# have historical names and aren't simply 'dpd' and 'bid', so we map a
# variable to their name based upon $enable_decimal_float.
default_backend=
case "$enable_decimal_float" in
  dpd)
    default_backend=libdecnumber
    ;;
  bid)
    default_backend=libbid
    ;;
  *)
    default_backend=libdecnumber
    ;;
esac

dfp_backend=
AC_ARG_WITH([backend],
	    AS_HELP_STRING([--with-backend=@<:@libdecnumber|libbid@:>@],
			   [This indicates which backend library to use
			    in conjunction with the DFP type encoding
			    specified by --enable-decimal-float:

			    @<:@default=libdecnumber@:>@ for DPD
			    @<:@optional=libbid@:>@ for BID
			    @<:@default=libbid@:>@ for BID

			    libdecnumber will also work with the BID
			    Decimal Floating Point types encoding.

			    libbid will NOT work with the DPD Decimal
			    Floating Point types encoding.]),
	    [dnl
  case "$withval" in
  libbid)
    if test "$enable_decimal_float" = dpd; then
      AC_MSG_ERROR([The DPD encoding can not be used with libbid.])
    else
      dfp_backend=libbid
    fi
    ;;
   libdecnumber|*)
    dfp_backend=libdecnumber
    ;;
esac],
	    [dfp_backend=$default_backend])
AC_SUBST(dfp_backend)

# This will invoke the sysdeps/$machine/<processor>/ machinery.
mzarch=
submachine=
with_dfp=
AC_ARG_WITH([cpu],
	    AS_HELP_STRING([--with-cpu=CPU], [select code for CPU variant]),
	    [dnl
  case "$withval" in
  yes|'') AC_MSG_ERROR([--with-cpu requires an argument]) ;;
  no)
    with_dfp=no
    ;;
  power7)
    submachine="$withval"
    AC_MSG_NOTICE(configuring for the $submachine processor)
    with_dfp=yes ;;
  power6)
    submachine="$withval"
    AC_MSG_NOTICE(configuring for the $submachine processor)
    with_dfp=yes ;;
  power6x)
    submachine="$withval"
    AC_MSG_NOTICE(configuring for the $submachine processor)
    with_dfp=yes ;;
  z9-ec)
    submachine="$withval"
    # Warning, -mzarch won't get passed to libecnumber/configure
    mzarch="-mzarch"
    AC_MSG_NOTICE(configuring for the $submachine processor)
    with_dfp=yes ;;
  z10)
    submachine="$withval"
    # Warning, -mzarch won't get passed to libecnumber/configure
    mzarch="-mzarch"
    AC_MSG_NOTICE(configuring for the $submachine processor)
    with_dfp=yes ;;
  *)
    submachine="$withval"
    AC_MSG_NOTICE(configuring for the $submachine processor)
    with_dfp=no ;;
  esac
],[with_dfp=no])

if test -n "$submachine"; then
  AC_CACHE_CHECK([for compiler option for CPU variant],
                submachine_opt, [dnl
  submachine_opt=no
  for opt in "-march=$submachine" "-mcpu=$submachine"; do
    if AC_TRY_COMMAND([${CC-cc} $opt -xc /dev/null -S -o /dev/null]); then
      submachine_opt="$opt"
      break
    fi
  done])
  if test "x$submachine_opt" = xno; then
    AC_MSG_ERROR([${CC-cc} does not support $submachine])
  fi
fi

# Unless the system is a s390 variant this is most likely empty.
AC_SUBST(mzarch)

# This tells us whether we know we have a hardware decimal floating point
# unit.  We know this based upon the submachine.
AC_SUBST(with_dfp)

AC_ARG_WITH([fp],
	    AS_HELP_STRING([--with-fp],
			   [if using floating point hardware @<:@default=yes@:>@]),
	    [with_fp=$withval],
	    [with_fp=yes])
AC_SUBST(with_fp)

dnl We need to use [ and ] for other purposes for a while now.
changequote(,)dnl

case "$machine" in
	powerpc) base_machine=powerpc machine=powerpc/powerpc32 ;;
	powerpc64) base_machine=powerpc machine=powerpc/powerpc64 ;;
	s390) base_machine=s390 machine=s390/s390-32 ;;
	s390x) base_machine=s390 machine=s390/s390-64 ;;
	i[34567]86) base_machine=i386 machine=i386/$machine ;;
	*) base_machine=$machine ;;
esac
changequote([,])dnl
AC_SUBST(base_machine)
AC_SUBST(machine)

AC_PROG_CC(gcc)
#AC_PROC_CC checks $PATH for 'gcc' but doesn't prefix $CC with $PATH
CC=`which $CC`
echo "CC = $CC"

AC_PROG_CXX(g++)
#AC_PROC_CXX checks $PATH for 'g++' but doesn't prefix $CXX with $PATH
CXX=`which $CXX`
echo "CXX = $CXX"

AC_PATH_PROG([OBJDUMP], [objdump])
AC_ARG_VAR(OBJDUMP,The path to the objdump used by the debug scripts.
	   Defaults to that found in $$PATH)

AC_PATH_PROG([GDB], [gdb])
AC_ARG_VAR(GDB,The path to the gdb used by the debug scripts.
	   Defaults to that found in $$PATH)

AC_PATH_PROG([LDD], [ldd])
AC_ARG_VAR(LDD,The path to the ldd used by the debug scripts.
	   Defaults to that found in $$PATH)

AC_SUBST(CFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(ASFLAGS)

# Don't even bother configuring if the compiler can't handle Decimal
# Floating Point data types.
AC_CACHE_CHECK(for decimal-float-support in compiler,
	       libdfp_cv_cc_with_decimal_float, [
		cat > conftest.c <<EOF
int main (void) { _Decimal64 d64; d64 = 1.0DD; return 0; }
EOF
  if ${CC} ${CFLAGS} ${CPPFLAGS} $LDFLAGS -static -o conftest \
       conftest.c 2>&1 >/dev/null | grep -q " '_Decimal64' undeclared ";
  then
    libdfp_cv_cc_with_decimal_float=no
    AC_MSG_WARN([CC is ${CC}])
    AC_MSG_ERROR([${PACKAGE_NAME} requires a compiler configured and built
		  with --enable-decimal-float.])
  else
    libdfp_cv_cc_with_decimal_float=yes
  fi
  rm -f conftest*])
AC_SUBST(libdfp_cv_cc_with_decimal_float)
if test $libdfp_cv_cc_with_decimal_float = yes; then
  cat >> confdefs.h <<\EOF
#define __STDC_DEC_FP__ 200704L
EOF
fi

user_headers=no
glibc_headers=
AC_ARG_WITH([glibc-headers],
            AS_HELP_STRING([--with-glibc-headers=PATH],
			   [Location of glibc headers to use to find math.h,
			    fenv.h, features.h, et al.
			    @<:@default=compiler default@:>@]),
	    [glibc_headers=$withval
	     user_headers=yes
	     AC_MSG_NOTICE(Building against GLIBC headers in ${glibc_headers})
	     ],
	    [glibc_headers=''])
AC_SUBST(glibc_headers)

glibc_headers_inc=
if test "x$glibc_headers" != x; then
  glibc_headers_inc="-I$glibc_headers"
fi

# Check for GLIBC 2.10 headers
have_glibc_2_10_headers=yes
saved_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS $glibc_headers_inc"

AC_MSG_CHECKING([for printf-hook register_printf_type() in printf.h to verify GLIBC 2.10])
AC_TRY_COMPILE([#include "printf.h"],[
 static int foo;
 void foo_va (void *mem, va_list *ap)
 {
   return;
 }
 foo = register_printf_type (foo_va);
 ], [AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no]); have_glibc_2_10_headers=no])

CFLAGS="$saved_CFLAGS"

if test x$have_glibc_2_10_headers != xyes; then
  if test x$user_headers = xyes; then
    AC_MSG_ERROR([Building Libdfp requires a GLIBC printf.h header that
		  provides the register_printf_type() function which is
		  available in GLIBC Version 2.10 or later.  The headers you
		  provided in $glibc_headers do not provide this.])
  else
    AC_MSG_ERROR([Building Libdfp requires a GLIBC printf.h header that
		  provides the register_printf_type() function which is
		  available in GLIBC Version 2.10 or later.  The default
		  headers that this build is using don't provide this
		  functionality.  Use --with-glibc-header=<path>/include to
		  direct configure to a set of GLIBC Version 2.10 or later
		  headers. ])
  fi
  exit 1;
fi

user_glibc=no
glibc_build=
AC_ARG_WITH([glibc-build],
            AS_HELP_STRING([--with-glibc-build=PATH],
			   [Location of a GLIBC 2.10+ build to link against for make check.
			    @<:@system libdir@:>@]),
	    [glibc_build=$withval
	     user_glibc=yes
	     AC_MSG_NOTICE(Linking against GLIBC build in ${glibc_build})
	     ],
	    [glibc_build=''])
AC_SUBST(glibc_build)

glibc_headers_inc=
if test "x$glibc_headers" != x; then
  glibc_headers_inc="-I$glibc_headers"
fi
glibc_build_lib=
if test "x$glibc_build" != x; then
  # I don't like this but -L <path> -lc wasn't working.
  glibc_build_lib="$glibc_build/libc.so"
fi

# Check for GLIBC 2.10 runtime support
have_glibc_2_10=yes
saved_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS $glibc_headers_inc"
saved_LIBS="$LIBS"
LIBS="$LIBS $glibc_build_lib"

AC_MSG_CHECKING([for register_printf_type() runtime support])
AC_TRY_LINK([#include "printf.h"],[
 static int foo;
 void foo_va (void *mem, va_list *ap)
 {
   return;
 }
 foo = register_printf_type (foo_va);
 ], [AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no]); have_glibc_2_10=no])

LIBS="$saved_LIBS"
CFLAGS="$saved_CFLAGS"

if test x$have_glibc_2_10 != xyes; then
  if test x$user_glibc = xyes; then
    AC_MSG_WARN([Executing Libdfp tests requires a GLIBC runtime that
		 supports the register_printf_type() function which is
		 available in GLIBC Version 2.10 or later.  The glibc build
		 you specified with --with-glibc-build=$glibc_build does not
		 provide this.  You will not be able to successfully execute
		 the test suite.])
  else
    AC_MSG_WARN([Executing Libdfp tests requires a GLIBC runtime that
		 supports the register_printf_type() function which is
		 available in GLIBC Version 2.10 or later.  The default libc
		 that this build is using doesn't provide this functionality.
		 You will not be able to successfully execute the test
		 suite.])
  fi
fi

AC_ARG_ENABLE([static],
	      AS_HELP_STRING([--enable-static],
			     [build static library @<:@default@:>@]),
	      [case "$enableval" in
  yes)
       enable_static=static
       AC_MSG_NOTICE(enabling support for a static library.);;
   no)
       enable_static=
       AC_MSG_NOTICE(disabling support for a static library.);;
    *)
       enable_static=static
       AC_MSG_NOTICE(enabling support for a static library.);;
 esac],
	      enable_static=static)
AC_SUBST(enable_static)

AC_ARG_ENABLE([shared],
	      AS_HELP_STRING([--enable-shared],
			     [build shared library @<:@default@:>@]),
	      [case "$enableval" in
  yes)
       enable_shared=shared
       AC_MSG_NOTICE(enabling support for a shared library.);;
   no)
       enable_shared=
       AC_MSG_NOTICE(disabling support for a shared library.);;
    *)
       enable_shared=
       AC_MSG_NOTICE(enabling support for a shared library.);;
  esac],
	      enable_shared=shared )
AC_SUBST(enable_shared)

# Should be ok if we build static with this turned on.  If the linker
# complains that the GOT is too big this may need to be upgraded to -fPIC so
# we get full addressibility.
picflag="-fpic -DSHARED"
AC_MSG_NOTICE(configuring with "$picflag")
AC_SUBST(picflag)

# Compute the list of sysdeps directories.
AC_MSG_CHECKING(sysdep dirs)

# Build a search list based on this precedence:
# ${srcdir}/include [internal libdfp headers]
# ${srcdir}/libdecnumber/dpd [libdecnumber headers]
# ${srcdir}/libdecnumber/bid [libdecnumber headers]
# ${srcdir}/libbid/? [libbid headers]
# [MACHINE/SUBMACHINE]
#   ${srcdir}/sysdeps/powerpc/powerpc64/power7/fpu [platform directories]
#   ${srcdir}/sysdeps/powerpc/powerpc64/power7/
#   ${srcdir}/sysdeps/powerpc/powerpc64/power6/fpu [platform directories]
#   ${srcdir}/sysdeps/powerpc/powerpc64/power6/
#   ${srcdir}/sysdeps/powerpc/powerpc64/
#   ${srcdir}/sysdeps/powerpc/powerpc32/power6/fpu [platform directories]
#   ${srcdir}/sysdeps/powerpc/powerpc32/power6/
#   ${srcdir}/sysdeps/powerpc/powerpc32/
# [BASE_MACHINE/[FPU|NOFPU]
#   ${srcdir}/sysdeps/powerpc/fpu
#   ${srcdir}/sysdeps/powerpc/fpu/bits
#   ${srcdir}/sysdeps/powerpc/nofpu
#   ${srcdir}/sysdeps/powerpc/nofpu/bits
# [BASE_MACHINE]
#   ${srcdir}/sysdeps/powerpc/
#   ${srcdir}/sysdeps/i386/
# [ENCODING]
#   ${srcdir}/sysdeps/dpd [dpd specific implementations]
#   ${srcdir}/sysdeps/bid [bid specific implementations]
# ${srcdir}/sysdeps/soft-dfp/dpd [soft-dfp dpd arithmetic, conversion, and comparison fns]
# ${srcdir}/sysdeps/soft-dfp/bid [soft-dfp bid arithmetic, conversion, and comparison fns]
# ${srcdir}/sysdeps/soft-dfp [soft-dfp arithmetic, conversion, and comparison fns]
# ${srcdir}/dfp [headers files that will be installed.  These follow the
#		 sysdep overrides so that sysdeps override directories show up
#		 before the install headers in #include_next <header>.
# ${srcdir}/ieee754r [ISO C Draft TR functions]
# ${srcdir}/base-math [Arithmetic, Conversion, and Comparison Routines.]
# ${srcdir}/[common libdfp framework code & common headers for the backends.]

# Out of Tree Sources
# ${srcdir}/libdecnumber [third-party GCC sources]
# ${srcdir}/libbid       [third-party libBID sources]

# We only assume Linux support so we don't worry about all the os stuff that
# GLIBC does.

dnl We need to use [ and ] for other purposes for a while now.
changequote(,)dnl
tail=$machine${submachine:+/$submachine}
# Build the list of dfpu|fpu|nofpu directories.
while m=`echo $tail | sed 's@^\(.*\)/\([^/]*\)$@& \1@'`; test -n "$m"; do
  set $m
  # Prepend the machine's FPU directory unless --without-fp.
  if test "$with_fp" = yes; then
    mach="$mach /$1/fpu/bits /$1/fpu"
    #mach="$mach /$1/fpu"
  else
    mach="$mach /$1/fpu/bits /$1/nofpu"
    #mach="$mach /$1/nofpu"
  fi
  # Only look for 'dfpu' directory if the system supports hardware dfp.
  if test "$with_dfp" = yes; then
    mach="$mach /$1/dfpu/bits /$1/dfpu"
    #mach="$mach /$1/dfpu"
  fi
  mach="$mach /$1"
  tail="$2"
done

dnl We are done with glob and regexp uses of [ and ]; return to autoconf.
changequote([,])dnl

# Thankfully we don't support add-ons or non-Linux operating systems so this
# is relatively simple compared to what it could be.  We also don't care if
# the machine or submachine don't show up in the sysdeps directory.  The base
# configuration doesn't require it.
sysnames=
try_srcdir="${srcdir}/"
for m in $mach ''; do
  # We don't want to add /src/sysdeps yet.
  if test -n "$m"; then
    try="sysdeps${m}"
    if test -d "${try_srcdir}${try}"; then
      sysnames="$sysnames $try"
    fi
  fi
done

AC_SUBST(submachine)
AC_SUBST(submachine_opt)

if test "$with_dfp" = no; then
  # Add src/sysdeps/soft-dfp and src/sysdeps/soft-dfp/dpd or
  # src/sysdeps/soft-dfp/bid to the search path.
  if test -d "${try_srcdir}/sysdeps/soft-dfp/"; then
    if test -d "${try_srcdir}/sysdeps/soft-dfp/${enable_decimal_float}"; then
      sysnames="$sysnames `echo sysdeps/soft-dfp/${enable_decimal_float} | sed -e 's,//,/,g'`"
    fi
    sysnames="$sysnames `echo sysdeps/soft-dfp | sed -e 's,//,/,g'`"
  fi
fi

# Add src/sysdeps/dpd or src/sysdeps/bid to the search path.
if test -d "${try_srcdir}/sysdeps/${enable_decimal_float}"; then
  sysnames="$sysnames `echo sysdeps/${enable_decimal_float} | sed -e 's,//,/,g'`"
fi

# Close out the sysdeps directory.
#sysnames="$sysnames sysdeps"

set $sysnames
names=
while test $# -gt 0; do
  name=$1
  shift

  case " $names " in *" $name "*)
  # Already in the list.
  continue
  esac

  case $name in
    /*) xsrcdir=;;
    *) xsrcdir=$srcdir/ ;;
  esac

  if test -f $xsrcdir$name/Implies; then
    implied_candidate="`sed 's/#.*$//' < $xsrcdir$name/Implies`"
    implied=
    for x in $implied_candidate; do
	  found=no
	  if test -d $xsrcdir/$x; then
	    implied="$implied $x";
	    found=yes
	  fi

	  if test $found = no; then
	    echo "$name/Implies specifies nonexistent $x"
	  fi
	done
  else
    implied=
  fi

  names="$names $name"

  parent="`echo $name | sed -n -e 's=/[^/]*$==' -e '/sysdeps$/q' -e p`"

  sysnames="`echo $implied $* $parent`"
  test -n "$sysnames" && set $sysnames
done

sysdep_dirs="$names"

# Just the directories in the sysdeps tree.
AC_SUBST(sysdep_dirs)
AC_MSG_RESULT($sysdep_dirs)

# Taken from GLIBC's configure.in
AC_CACHE_CHECK(for assembler global-symbol directive,
	       libc_cv_asm_global_directive, [dnl
libc_cv_asm_global_directive=UNKNOWN
for ac_globl in .globl .global .EXPORT; do
  cat > conftest.s <<EOF
	${libc_cv_dot_text}
	${ac_globl} foo
foo:
EOF
  if AC_TRY_COMMAND(${CC-cc} $ASFLAGS -c conftest.s 1>&AS_MESSAGE_LOG_FD); then
    libc_cv_asm_global_directive=${ac_globl}
  fi
  rm -f conftest*
  test $libc_cv_asm_global_directive != UNKNOWN && break
done])
if test $libc_cv_asm_global_directive = UNKNOWN; then
  AC_MSG_ERROR(cannot determine asm global directive)
else
  AC_DEFINE_UNQUOTED(ASM_GLOBAL_DIRECTIVE,
		     ${libc_cv_asm_global_directive},"Define to the name of the assembler's directive for declaring a symbol global (default `.globl').")
fi

AC_CACHE_CHECK(for assembler .type directive prefix,
	       libc_cv_asm_type_prefix, [dnl
libc_cv_asm_type_prefix=no
for ac_try_prefix in '@' '%' '#'; do
  cat > conftest.s <<EOF
	${libc_cv_dot_text}
	${libc_cv_asm_global_directive} foo
	.type foo, ${ac_try_prefix}object
	.size foo, 1
foo:
	.byte 1
EOF
  if AC_TRY_COMMAND(${CC-cc} $ASFLAGS -c conftest.s 1>&AS_MESSAGE_LOG_FD); then
    libc_cv_asm_type_prefix=${ac_try_prefix}
  fi
  rm -f conftest*
  test "x$libc_cv_asm_type_prefix" != xno && break
done])
if test "x$libc_cv_asm_type_prefix" != xno; then
  AC_DEFINE_UNQUOTED(ASM_TYPE_DIRECTIVE_PREFIX, ${libc_cv_asm_type_prefix}, "Define to the prefix before `object' or `function' in the assembler's `.type' directive, if it has one.")
fi

AC_PROG_RANLIB # Required for static archives.
AC_PROG_AWK

dnl AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
dnl AC_HEADER_STDC
dnl AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.


# Wherever there's a standalone configure fragment we need to tell the top
# level configure to recurse into it.
if test "$dfp_backend" = libdecnumber; then
  AC_CONFIG_SUBDIRS(libdecnumber)
else
  AC_CONFIG_SUBDIRS(libbid)
fi

# This tells autoconf to generate Makefile from the Makefile.in.  This is the
# top level Makefile for the project.  This Makefile will then recurse into
# the fragment Makefiles in the sysdeps directories.
AC_CONFIG_FILES([Makefile])

AC_OUTPUT
