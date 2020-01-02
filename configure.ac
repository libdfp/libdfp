# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libdfp],[1.0.14],[https://github.com/libdfp/libdfp/issues])
AC_CONFIG_SRCDIR([generate-changelog.sh])

# This is used for libdecnumber.
AC_C_BIGENDIAN

# We don't use Automake so this is necessary.
AC_CONFIG_HEADER([config.h])

# This indicates where config.guess and config.sub are located.  Those scripts
# are used by the AC_CANONICAL_* macros to determine platform specific data.
AC_CONFIG_AUX_DIR([scripts])

# Defines host_cpu, host_vendor, and host_os variables.  This is where the
# code will run.
AC_CANONICAL_HOST
config_os=$host_os
machine=$host_cpu

# Only allow a Linux host OS.  We don't support building libdfp on non-Linux
# systems.
if test "$config_os" != linux-gnu; then
  AC_MSG_ERROR(['linux-gnu' is the only allowed host OS])
fi

# Ensure basic toolchain tools are available.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_AWK
AC_PROG_RANLIB

AC_PATH_PROG([OBJDUMP], [objdump])
AC_ARG_VAR(OBJDUMP,The path to the objdump used by the debug scripts.
Defaults to that found in $$PATH)

AC_PATH_PROG([READELF], [readelf])
AC_ARG_VAR(READELF,The path to the readelf used by test scripts.
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


# This will invoke the sysdeps/$machine/<processor>/ machinery.
submachine=
with_dfp=
# This tells us whether we know we have a hardware decimal floating point
# unit.  We know this based upon the submachine.
AC_MSG_CHECKING([for hardware decimal floating point unit])
AC_ARG_WITH([cpu],
	    AS_HELP_STRING([--with-cpu=CPU],
			   [Select the CPU to run on. Accepted values
			    are similar to those accepted by gcc's -mcpu
			    option. power6 and newer, and some z processors
			    are supported. no is also accepted to indicate
			    software only dfp should be used.

			    If this option is not specified, configure will
			    attempt to guess the latest powerpc processor
			    supported by the toolchain, or use the soft
			    dfp implementation.]),
	    [dnl
  case "$withval" in
  yes|'') AC_MSG_ERROR([--with-cpu requires an argument]) ;;
  no)
    with_dfp=no
    ;;
  power[[6-9]]|power6x)
    submachine="$withval"
    AC_MSG_NOTICE(configuring for the $submachine processor)
    with_dfp=yes ;;
  z9-ec|z10|z196|zEC12|z13|arch12|z14|arch13|z15)
    submachine="$withval"
    AC_MSG_NOTICE(configuring for the $submachine processor)
    with_dfp=yes ;;
  *)
    submachine="$withval"
    AC_MSG_NOTICE(configuring for the $submachine processor)
    with_dfp=no ;;
  esac
],[
  case "$host_cpu" in
  powerpc*)
    # Test successively newer processors and select the
    # best implementation.
    AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
      #ifndef _ARCH_PWR6
      # error "Support for hardware dfp isn't available"
      #endif
      ]])], [with_dfp=yes; submachine=power6], [with_dfp=no])
    AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
      #ifndef _ARCH_PWR6X
      # error "POWER6X not supported"
      #endif
      ]])], [submachine=power6x], [])
    AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
      #ifndef _ARCH_PWR7
      # error "POWER7 not supported"
      #endif
      ]])], [submachine=power7], [])
    AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
      #ifndef _ARCH_PWR8
      # error "POWER8 not supported"
      #endif
      ]])], [submachine=power8], [])
    ;;
    s390|s390x)
    with_dfp=no
    # Check if the used compiler supports dfp instructions by converting
    # from dfp to bfp. If supported, the pfpo instruction is used.
    # Note: On s390 (31bit) we have to use -mzarch for this instruction.
    cat > conftest.c <<EOF
double test (_Decimal64 in) { return (double) in; }
EOF
    if AC_TRY_COMMAND([${CC-cc} ${CFLAGS} -mzarch -c conftest.c]);
    then
      if AC_TRY_COMMAND([objdump -d conftest.o 2>&1 | grep pfpo > /dev/null]);
      then
	with_dfp=yes
      fi
    fi
    rm -f conftest*
    ;;
  *)
    with_dfp=no ;;
  esac
])

if test -n "$submachine"; then
  AC_CACHE_CHECK([for compiler option for CPU variant],
                libdfp_cv_submachine_opt, [dnl
  libdfp_cv_submachine_opt=no
  for opt in "-march=$submachine" "-mcpu=$submachine"; do
    if AC_TRY_COMMAND([${CC-cc} $opt -xc /dev/null -S -o /dev/null]); then
      libdfp_cv_submachine_opt="$opt"
      break
    fi
  done])
  if test "x$libdfp_cv_submachine_opt" = xno; then
    AC_MSG_ERROR([${CC-cc} does not support $submachine])
  fi
fi

AC_ARG_WITH([fp],
	    AS_HELP_STRING([--with-fp],
			   [if using floating point hardware @<:@default=yes@:>@]),
	    [with_fp=$withval],
	    [with_fp=yes])
AC_SUBST(with_fp)

dnl We need to use [ and ] for other purposes for a while now.
changequote(,)dnl

case "$machine" in
	powerpc|powerpcle) base_machine=powerpc machine=powerpc/powerpc32 ;;
	powerpc64|powerpc64le) base_machine=powerpc machine=powerpc/powerpc64 ;;
	s390) base_machine=s390 machine=s390/s390-32 ;;
	s390x) base_machine=s390 machine=s390/s390-64 ;;
	i[34567]86) base_machine=i386 machine=i386/$machine ;;
	*) base_machine=$machine ;;
esac
changequote([,])dnl
AC_SUBST(base_machine)
AC_SUBST(machine)

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

# Check for GLIBC 2.10 headers
have_glibc_2_10_headers=yes
AC_MSG_CHECKING([for printf-hook register_printf_type() in printf.h to verify GLIBC 2.10])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include "printf.h"]], [[
 void foo_va (void *mem, va_list *ap)
 {
   return;
 }
 register_printf_type (foo_va);
 ]])],[AC_MSG_RESULT([yes])],[AC_MSG_RESULT([no]); have_glibc_2_10_headers=no])

if test x$have_glibc_2_10_headers != xyes; then
  AC_MSG_ERROR([Building Libdfp requires a GLIBC printf.h header that
		provides the register_printf_type() function which is
		available in GLIBC Version 2.10 or later.])
fi

# Check for std::decimal compiler support
AC_CACHE_CHECK(for std::decimal, libdfp_cv_cxxdecimal,
  [AC_LANG_SAVE
    AC_LANG_CPLUSPLUS
    AC_TRY_COMPILE(
      [ #include <decimal/decimal> ], [
        std::decimal::decimal64 x = 0.0000006E-90DD;
      ], [
        libdfp_cv_cxxdecimal=yes
      ], [
        libdfp_cv_cxxdecimal=no
      ])
    AC_LANG_RESTORE]
)
cxxdecimal="$libdfp_cv_cxxdecimal"
AC_SUBST(cxxdecimal)


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
#   ${srcdir}/sysdeps/powerpc/dfpu
#   ${srcdir}/sysdeps/powerpc/dfpu/bits
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
  # Only look for 'dfpu' directory if the system supports hardware dfp.
  if test "$with_dfp" = yes; then
    mach="$mach /$1/dfpu/bits /$1/dfpu"
    #mach="$mach /$1/dfpu"
  else
    mach="$mach /$1/dfpu/bits /$1/nodfpu"
  fi
  # Prepend the machine's FPU directory unless --without-fp.
  if test "$with_fp" = yes; then
    mach="$mach /$1/fpu/bits /$1/fpu"
    #mach="$mach /$1/fpu"
  else
    mach="$mach /$1/fpu/bits /$1/nofpu"
    #mach="$mach /$1/nofpu"
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
AC_SUBST(libdfp_cv_submachine_opt)

# Close out the sysdeps directory.
#sysnames="$sysnames sysdeps"

# Add the default directories.
default_sysnames="sysdeps/generic"
sysnames="$sysnames $default_sysnames"

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

# Check which is the loader
libdfp_cv_loader=`
cat > conftest.c <<EOF
int main () {}
EOF
${CC-cc} $CFLAGS $CPPFLAGS -o conftest conftest.c && ldd conftest | $AWK '
BEGIN {
  loader=$0 ~ "ld.*so"
}
{
  gsub (/[[[:space:]]]*/, "", $loader)
  split ($loader, fields, "[[=(]]")
}
END {
  print fields[[1]]
}'
rm -f conftest*`
AC_DEFINE_UNQUOTED([LIBDFP_RUNTIME_LOADER], ["$libdfp_cv_loader"],
                   [The loader to use in direct DSO call])

# Checks for libraries.

# Checks for header files.
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([fenv.h float.h langinfo.h limits.h locale.h stddef.h stdint.h stdlib.h string.h sys/time.h unistd.h wchar.h wctype.h])

# Checks for typedefs, structures, and compiler characteristics.
# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_INT64_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MBRTOWC
AC_CHECK_FUNCS([floor gettimeofday memset nl_langinfo setlocale sqrt strchr strtol])

# Copy the glibc check for GCC minimum version:
AC_CACHE_CHECK([if $CC is sufficient to build libdfp], libc_cv_compiler_ok, [
AC_TRY_COMPILE([], [
#if !defined __GNUC__ || __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 6)
#error insufficient compiler
#endif],
           [libc_cv_compiler_ok=yes],
           [libc_cv_compiler_ok=no])])
AS_IF([test $libc_cv_compiler_ok != yes],
      [critic_missing="$critic_missing compiler"])

# This tells autoconf to generate Makefile from the Makefile.in.  This is the
# top level Makefile for the project.  This Makefile will then recurse into
# the fragment Makefiles in the sysdeps directories.
AC_CONFIG_FILES([Makefile])

AC_OUTPUT
