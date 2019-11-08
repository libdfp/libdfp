/* Support macros for making weak and strong aliases for symbols.

   Copyright (C) 1995-2015 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Ryan S. Arnold <rsa@us.ibm.com>

   The Decimal Floating Point C Library is free software; you can
   redistribute it and/or modify it under the terms of the GNU Lesser
   General Public License version 2.1.

   The Decimal Floating Point C Library is distributed in the hope that
   it will be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
   the GNU Lesser General Public License version 2.1 for more details.

   You should have received a copy of the GNU Lesser General Public
   License version 2.1 along with the Decimal Floating Point C Library;
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see dfp/COPYING.txt for more information.  */

#ifndef _LIBDFP_SYMBOLS_H
#define _LIBDFP_SYMBOLS_H	1

/* Define a macro we can use to construct the asm name for a C symbol.  */
#ifndef LABEL
# define LABEL(name) name##:
#endif /* LABEL  */

#define	MIN(a,b) (((a)<(b))?(a):(b))

/* Define ALIASNAME as a strong alias for NAME.  */
#ifndef __ASSEMBLER__
# define strong_alias(name, aliasname) _strong_alias(name, aliasname)
# define _strong_alias(name, aliasname) \
  extern __typeof (name) aliasname __attribute__ ((alias (#name)));
#else
# define strong_alias(original, alias) \
  .globl alias; \
  .set alias, original
# define strong_data_alias(original, alias) strong_alias(original, alias)
#endif

/* Define ALIASNAME as a weak alias for NAME.
   If weak aliases are not available, this defines a strong alias.  */
#ifndef __ASSEMBLER__
# define weak_alias(name, aliasname) _weak_alias (name, aliasname)
# define _weak_alias(name, aliasname) \
  extern __typeof (name) aliasname __attribute__ ((weak, alias (#name)));
#else
# define weak_alias(original, alias) \
  .weak alias; \
  alias = original
#endif

/* Same as WEAK_ALIAS, but mark symbol as hidden.  */
#define weak_hidden_alias(name, aliasname) \
  _weak_hidden_alias (name, aliasname)
#define _weak_hidden_alias(name, aliasname) \
  extern __typeof (name) aliasname \
    __attribute__ ((weak, alias (#name), __visibility__ ("hidden")));

/* Declare SYMBOL as weak undefined symbol (resolved to 0 if not defined).  */
#define weak_extern(symbol) _weak_extern (weak symbol)
#define _weak_extern(expr) _Pragma (#expr)

/* We don't need hidden visibility with the static library libdfp.a  */
#ifdef SHARED
# define attribute_hidden __attribute__ ((visibility ("hidden")))
#else
# define attribute_hidden
#endif /* SHARED  */

/* Use the copy in cdefs.h if available, if not, bring in the glibc definition.  */
#ifndef __attribute_copy__
# if __GNUC__ >= 9
#  define __attribute_copy__(name) __attribute__ ((__copy__ (name)))
# else
#  define __attribute_copy__(name)
# endif
#endif

/* Add versioned symbol macros when necessary.  */

/* Macros to avoid PLT calls within libdfp.  */
#ifdef SHARED
# ifndef __ASSEMBLER__
#  define __hidden_proto_hiddenattr(attrs...) \
  __attribute__ ((visibility ("hidden"), ##attrs))
#  define hidden_proto(name, attrs...) \
  __hidden_proto (name, __GI_##name, ##attrs)
#  define __hidden_proto(name, internal, attrs...) \
  extern __typeof (name) name __asm__ (__hidden_asmname (#internal)) \
  __hidden_proto_hiddenattr (attrs);
#  define __hidden_asmname(name) \
  __hidden_asmname1 (__USER_LABEL_PREFIX__, name)
#  define __hidden_asmname1(prefix, name) __hidden_asmname2(prefix, name)
#  define __hidden_asmname2(prefix, name) #prefix name
#  define __hidden_ver1(local, internal, name) \
  extern __typeof (name) __EI_##name __asm__(__hidden_asmname (#internal)); \
  extern __typeof (name) __EI_##name \
        __attribute__((alias (__hidden_asmname (#local)))) \
        __attribute_copy__ (name)
#  define hidden_ver(local, name)       __hidden_ver1(local, __GI_##name, name);
#  define hidden_data_ver(local, name)  hidden_ver(local, name)
#  define hidden_def(name)              _hidden_def(name)
#  define _hidden_def(name)             __hidden_ver1(__GI_##name, name, name);

#  define hidden_data_def(name)         hidden_def(name)
#  define hidden_weak(name) \
        __hidden_ver1(__GI_##name, name, name) __attribute__((weak));
#  define hidden_data_weak(name)        hidden_weak(name)
# else /* __ASSEMBLER__  */
#  define hidden_def(name)      strong_alias (name, __GI_##name)
#  define hidden_weak(name)     hidden_def (name)
#  define hidden_ver(local, name) strong_alias (local, __GI_##name)
#  define hidden_data_def(name) strong_data_alias (name, __GI_##name)
#  define hidden_data_weak(name)        hidden_data_def (name)
#  define hidden_data_ver(local, name) strong_data_alias (local, __GI_##name)
# endif
#else
# define hidden_proto(name, attrs...)
# define hidden_weak(name)
# define hidden_def(name)
# define hidden_ver(local, name)
# define hidden_data_weak(name)
# define hidden_data_def(name)
# define hidden_data_ver(local, name)
#endif /* SHARED */

/* Bring in PLT redirects for implicit GCC functions.  */
#include <symbol-hacks.h>

/* C++ needs to know that types and declarations are C, not C++.  */
#ifdef	__cplusplus
# define __BEGIN_DECLS  extern "C" {
# define __END_DECL	}
#else
# define __BEGIN_DECLS
# define __END_DECLS
#endif

#endif /* _LIBDFP_SYMBOLS_H  */
