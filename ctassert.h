/** @file
 *	@brief	Compile-time assertion capability.
 *
 *
 *	Copyright (c) 2019 Kevin L. Becker. All rights reserved.
 */

#ifndef INCLUDE_CTASSERT_H_
#define INCLUDE_CTASSERT_H_

// ============================================================================
// ----	Include Files ---------------------------------------------------------
// ============================================================================

// ----	System Headers --------------------------

// ----	Project Headers -------------------------

// ----	Module Headers --------------------------


#ifdef	__cplusplus
extern "C" {
#endif


// ============================================================================
// ----	Constants -------------------------------------------------------------
// ============================================================================
#define INCLUDE_CTASSERT_H__REVSTRING "$Revision: 0123 $"


// ============================================================================
// ----	Type Definitions ------------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Public Variables ------------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Public API ------------------------------------------------------------
// ============================================================================

/**
 * Compile-time check to see if test condition is valid or not.
 *
 * This macro, after expansion of the helper macros, generates a compile-time error if the test is invalid. This error
 * will be your compiler's flavor of "invalid array dimension"; if you see this error at the place where you're doing
 * the test, you will know the test failed. \see _CTASSERT() and __CTASSERT().
 *
 * @param[in] test is the logical test that is to be checked for validity.
 *
 * <dl><dt><b>Usage:</b></dt><dd>CT_ASSERT(<logical_test>);</dd></dl>
 *
 * <dl><dt><b>Example:</b></dt><dd><code>CT_ASSERT((NUM_OF_ITEMS <= 255) && (sizeof(buffer) >= NUM_OF_ITEMS));</code></dd></dl>
 *
 * @note
 *      - Macro only works for compile-time checking.  Run-time checks will not work.
 *      - This can only work where it is legal to declare a variable. Thus, for example, in a C89
 *        project, you cannot use it in a function after a line of executable code.
 */
#define CT_ASSERT(test)              _CTASSERT(__LINE__, test)

/**
 * First-level support macro, designed to work with CT_ASSERT(). It is not intended for direct use in your code.
 *
 * The purpose for this macro is solely and exclusively for the expansion of the __LINE__ argument passed by
 * CT_ASSERT(). The ANSI standard stipulates that when macros are concatenated,  the concatenation happens first, and
 * then the expansion; this means that this macro converts "__LINE__" to a numerical value, which is then passed to the
 * next macro. Without this intermediate macro, the end symbol would be "_CtAssert__LINE__[]", which is not the unique
 * symbol desired.
 *
 * @param[in] UniqueId - a unique identifier, which is used to form the name of the external array.
 * @param[in] test - the logical test that is to be checked for validity.
 */
#define _CTASSERT(UniqueId, test)   __CTASSERT(UniqueId, test)

/**
 * Second-level support macro, designed to work with CT_ASSERT(). It is not intended for direct use in your code.
 *
 * This macro uses the unique identifier passed to it, as part of the declaration of an external array. It then uses the
 * \b logical test as the size of the array. If the test is valid, it will evaluate to "1", and so the compiler will
 * see a declaration of an external array of size 1. If the test is invalid, the compiler will see a declaration of an
 * external array of size 0, which is not legal, and will cause a compilation error. Please note that the error emitted,
 * which should relate to invalid array dimension, will not relate to the thing being tested; the user should know that
 * when he receives this error, that the test was invalid.
 *
 * Please note that this macro relies on an intermediate macro, located between the API that is to be used in the source
 * code, and this macro, to properly expand all preprocessor symbols. In particular, if the symbol __LINE__ is used, it
 * must be expanded to a line number before being passed to this macro. Without the expansion, this macro will form a
 * symbol name of "_CtAssert__LINE__[]", which is not unique if the CT_ASSERT() macro is used multiple times within the
 * same source file.
 *
 * @param[in] UniqueId - a unique identifier, which is used to form the name of the external array.
 * @param[in] test - the logical test that is to be checked for validity.
 *
 * @note There are two PC-Lint warnings that are suppressed within this definition. The first, #752,
 *      suppresses a "local declarator 'Symbol' (Location) not referenced" warning, caused because
 *      we declare an external symbol and then make no reference to it. The second, #762, suppresses
 *      "Redundantly declared symbol 'Symbol' previously declared at <Location>" which can happen if,
 *      for example, you would place multiple CT_ASSERT() statements on the same line, or if the same
 *      check appeared at the same line number in two different files.
 */
#if defined(_CVI_)
#include <assert.h>
#define __CTASSERT(UniqueId, test)    assert(test)
#else
#define __CTASSERT(UniqueId, test)    /*lint --e{752} -e{762} */extern unsigned char _CtAssert##UniqueId[(test)];(void)_CtAssert##UniqueId
#endif


#ifdef	__cplusplus
}
#endif

#endif /* INCLUDE_CTASSERT_H_ */
