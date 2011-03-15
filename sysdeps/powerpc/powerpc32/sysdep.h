/* Assembly macros for 32-bit PowerPC.

   Copyright (C) 1999, 2001, 2002, 2003, 2006, 2009, 2011
   Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

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

   Please see libdfp/COPYING.txt for more information.  */

#include <sysdeps/powerpc/sysdep.h>

/* Derived from GLIBC  */
#ifdef __ASSEMBLER__

#ifdef __ELF__

#undef ENTRY
#define	ENTRY(name)							      \
  ASM_GLOBAL_DIRECTIVE name;						      \
  ASM_TYPE_DIRECTIVE (name,@function)					      \
  .align ALIGNARG(2);							      \
  LABEL(name)								      \
  cfi_startproc;

#define EALIGN_W_0  /* No words to insert.  */
#define EALIGN_W_1  nop
#define EALIGN_W_2  nop;nop
#define EALIGN_W_3  nop;nop;nop
#define EALIGN_W_4  EALIGN_W_3;nop
#define EALIGN_W_5  EALIGN_W_4;nop
#define EALIGN_W_6  EALIGN_W_5;nop
#define EALIGN_W_7  EALIGN_W_6;nop

#undef EALIGN
/* EALIGN is like ENTRY, but does alignment to 'words'*4 bytes
   past a 2^align boundary.  */
#define EALIGN(name, alignt, words)					      \
 ASM_GLOBAL_DIRECTIVE name;						      \
 ASM_TYPE_DIRECTIVE (name,@function)					      \
 .align ALIGNARG(alignt);						      \
 EALIGN_W_##words;							      \
 LABEL(name)								      \
 cfi_startproc;

#undef	END
#define END(name)							      \
  cfi_endproc;								      \
  ASM_SIZE_DIRECTIVE(name)

#define DO_CALL(syscall)						      \
    li 0,syscall;							      \
    sc

#undef JUMPTARGET
#ifdef PIC
# define JUMPTARGET(name) name##@plt
#else
# define JUMPTARGET(name) name
#endif

#if defined SHARED && defined DO_VERSIONING && defined PIC \
    && !defined NO_HIDDEN
# undef HIDDEN_JUMPTARGET
# define HIDDEN_JUMPTARGET(name) __GI_##name##@local
#endif

#define PSEUDO(name, syscall_name, args)				      \
  .section ".text";							      \
  ENTRY (name)								      \
    DO_CALL (SYS_ify (syscall_name));

#define PSEUDO_RET							      \
    bnslr+;								      \
    b __syscall_error@local
#define ret PSEUDO_RET

#undef	PSEUDO_END
#define	PSEUDO_END(name)						      \
  END (name)

#define PSEUDO_NOERRNO(name, syscall_name, args)			      \
  .section ".text";							      \
  ENTRY (name)								      \
    DO_CALL (SYS_ify (syscall_name));

#define PSEUDO_RET_NOERRNO						      \
    blr
#define ret_NOERRNO PSEUDO_RET_NOERRNO

#undef	PSEUDO_END_NOERRNO
#define	PSEUDO_END_NOERRNO(name)					      \
  END (name)

#define PSEUDO_ERRVAL(name, syscall_name, args)				      \
  .section ".text";							      \
  ENTRY (name)								      \
    DO_CALL (SYS_ify (syscall_name));

#define PSEUDO_RET_ERRVAL						      \
    blr
#define ret_ERRVAL PSEUDO_RET_ERRVAL

#undef	PSEUDO_END_ERRVAL
#define	PSEUDO_END_ERRVAL(name)						      \
  END (name)

/* Local labels stripped out by the linker.  */
#undef L
#define L(x) .L##x

#endif /* __ELF__ */

#endif	/* __ASSEMBLER__ */
