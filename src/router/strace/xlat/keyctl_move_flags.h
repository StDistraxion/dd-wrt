/* Generated by ./xlat/gen.sh from ./xlat/keyctl_move_flags.in; do not edit. */

#include "gcc_compat.h"
#include "static_assert.h"

#if defined(KEYCTL_MOVE_EXCL) || (defined(HAVE_DECL_KEYCTL_MOVE_EXCL) && HAVE_DECL_KEYCTL_MOVE_EXCL)
DIAG_PUSH_IGNORE_TAUTOLOGICAL_COMPARE
static_assert((KEYCTL_MOVE_EXCL) == (0x1), "KEYCTL_MOVE_EXCL != 0x1");
DIAG_POP_IGNORE_TAUTOLOGICAL_COMPARE
#else
# define KEYCTL_MOVE_EXCL 0x1
#endif

#ifndef XLAT_MACROS_ONLY

# ifdef IN_MPERS

#  error static const struct xlat keyctl_move_flags in mpers mode

# else

static const struct xlat_data keyctl_move_flags_xdata[] = {
 XLAT(KEYCTL_MOVE_EXCL),
 #define XLAT_VAL_0 ((unsigned) (KEYCTL_MOVE_EXCL))
 #define XLAT_STR_0 STRINGIFY(KEYCTL_MOVE_EXCL)
};
static
const struct xlat keyctl_move_flags[1] = { {
 .data = keyctl_move_flags_xdata,
 .size = ARRAY_SIZE(keyctl_move_flags_xdata),
 .type = XT_NORMAL,
 .flags_mask = 0
#  ifdef XLAT_VAL_0
  | XLAT_VAL_0
#  endif
  ,
 .flags_strsz = 0
#  ifdef XLAT_STR_0
  + sizeof(XLAT_STR_0)
#  endif
  ,
} };

#  undef XLAT_STR_0
#  undef XLAT_VAL_0
# endif /* !IN_MPERS */

#endif /* !XLAT_MACROS_ONLY */
