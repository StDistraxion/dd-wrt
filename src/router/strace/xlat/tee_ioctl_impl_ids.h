/* Generated by ./xlat/gen.sh from ./xlat/tee_ioctl_impl_ids.in; do not edit. */

#include "gcc_compat.h"
#include "static_assert.h"

#if defined(TEE_IMPL_ID_OPTEE) || (defined(HAVE_DECL_TEE_IMPL_ID_OPTEE) && HAVE_DECL_TEE_IMPL_ID_OPTEE)
DIAG_PUSH_IGNORE_TAUTOLOGICAL_COMPARE
static_assert((TEE_IMPL_ID_OPTEE) == (1), "TEE_IMPL_ID_OPTEE != 1");
DIAG_POP_IGNORE_TAUTOLOGICAL_COMPARE
#else
# define TEE_IMPL_ID_OPTEE 1
#endif
#if defined(TEE_IMPL_ID_AMDTEE) || (defined(HAVE_DECL_TEE_IMPL_ID_AMDTEE) && HAVE_DECL_TEE_IMPL_ID_AMDTEE)
DIAG_PUSH_IGNORE_TAUTOLOGICAL_COMPARE
static_assert((TEE_IMPL_ID_AMDTEE) == (2), "TEE_IMPL_ID_AMDTEE != 2");
DIAG_POP_IGNORE_TAUTOLOGICAL_COMPARE
#else
# define TEE_IMPL_ID_AMDTEE 2
#endif

#ifndef XLAT_MACROS_ONLY

# ifdef IN_MPERS

#  error static const struct xlat tee_ioctl_impl_ids in mpers mode

# else

static const struct xlat_data tee_ioctl_impl_ids_xdata[] = {
 XLAT(TEE_IMPL_ID_OPTEE),
 #define XLAT_VAL_0 ((unsigned) (TEE_IMPL_ID_OPTEE))
 #define XLAT_STR_0 STRINGIFY(TEE_IMPL_ID_OPTEE)
 XLAT(TEE_IMPL_ID_AMDTEE),
 #define XLAT_VAL_1 ((unsigned) (TEE_IMPL_ID_AMDTEE))
 #define XLAT_STR_1 STRINGIFY(TEE_IMPL_ID_AMDTEE)
};
static
const struct xlat tee_ioctl_impl_ids[1] = { {
 .data = tee_ioctl_impl_ids_xdata,
 .size = ARRAY_SIZE(tee_ioctl_impl_ids_xdata),
 .type = XT_NORMAL,
 .flags_mask = 0
#  ifdef XLAT_VAL_0
  | XLAT_VAL_0
#  endif
#  ifdef XLAT_VAL_1
  | XLAT_VAL_1
#  endif
  ,
 .flags_strsz = 0
#  ifdef XLAT_STR_0
  + sizeof(XLAT_STR_0)
#  endif
#  ifdef XLAT_STR_1
  + sizeof(XLAT_STR_1)
#  endif
  ,
} };

#  undef XLAT_STR_0
#  undef XLAT_VAL_0
#  undef XLAT_STR_1
#  undef XLAT_VAL_1
# endif /* !IN_MPERS */

#endif /* !XLAT_MACROS_ONLY */
