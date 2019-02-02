/** @file
 *	@brief	Supporting & implementation code for CWSW Library component.
 *
 *	Copyright (c) 2019 Kevin L. Becker. All rights reserved.
 *
 *	Original:
 *	Created on: Jun 4, 2017
 *	Author: kbecker
 *
 *	Current:
 *	$Revision: $
 *	$Date: $
 */

// ============================================================================
// ----	Include Files ---------------------------------------------------------
// ============================================================================

// ----	System Headers --------------------------
#include <limits.h>     // INT_MAX

// ----	Project Headers -------------------------

// ----	Module Headers --------------------------
#include "cwsw_lib.h"


// ============================================================================
// ----	Constants -------------------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Type Definitions ------------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Global Variables ------------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Module-level Variables ------------------------------------------------
// ============================================================================
static char const * const cwsw_lib_RevString = "$Revision: 0123 $";

/** "Has this module been initialized?" flag.
 *	For the SQSP Library, the import of this flag is less than in most modules; there are no
 *	state-related or HW initializations that must be done, and there is no task function that needs
 *	to be set up. In addition, nothing in this module needs to be deinitialized (such as, in
 *	preparation for entry into sleep mode, etc.)
 */
static bool initialized = false;


// ============================================================================
// ----	Private Prototypes ----------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Public Functions ------------------------------------------------------
// ============================================================================

uint16_t
Cwsw_Lib__Init(void)
{
	UNUSED(cwsw_lib_RevString);
	
	SUPPRESS_CONST_EXPR;
	if(	(XPRJ_Win_MinGW_Debug) 	||  \
		(XPRJ_Win_MinGW_UT)		||	\
		(XPRJ_Debug_Linux_GCC) 	||  \
		(XPRJ_NB_Debug)			||  \
		(XPRJ_Win_MSVC_Debug)	||  \
		(XPRJ_CVI_Debug) )
	{
		disable_console_buffering();

		#if defined(__GNUC__)	/* --- GNU Environment ------------------------------ */
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wpedantic"
		#endif

		dbg_printf(
				"\tModule ID %i\t%s\t%s\n"
				"\tEntering %s()\n\n",
				Cwsw_Lib, __FILE__, cwsw_lib_RevString,
				__FUNCTION__);

		#if defined(__GNUC__)	/* --- GNU Environment ------------------------------ */
		#pragma GCC diagnostic pop
		#endif
	}
	RESTORE_WARNING_LEVEL;

	initialized = true;
	return 0;
}

bool
Cwsw_Lib__Get_Initialized(void)
{
	return initialized;
}


/** Logging function specifically for cwsw_assert_helper.
 *  Weak binding so that application can override.
 */
WEAK void
cwsw_assert_helper_log(char const * const test,
                       char const * const filename,
                       int const lineno,
                       char const * const descrip)
{
	dbg_printf(
		"\nAssertion failed: \"%s\", file::line: %s::%i\nDescription: %s\n\n",
		test, filename, lineno, descrip);
}

/** Helper function for CWSW Assert statement.
 *
 * @param[in]	test		The stringified text of the the test that failed the assertion
 * @param[in]	filename	The name of the file that held the failed assertion.
 * @param[in]	lineno 		The line number in the file that held the failed assertion.
 * @param[in]	descrip     The user-supplied description that augments the assertion notice.
 *                          Suitable for a logging statement.
 */
void
cwsw_assert_helper(char const * const test,
                   char const * const filename,
                   int const lineno,
                   char const * const descrip)
{
	volatile uint16_t delay = 0;

    cwsw_assert_helper_log(test, filename, lineno, descrip);

	while(--delay);
}


PRIVATE int protection_count = 0;
int
Cwsw_Critical_Protect(int param)
{
	UNUSED(param);

	SUPPRESS_CONST_EXPR;
	cwsw_assert((protection_count >= 0) &&
                (protection_count < INT_MAX),
                "Invalid Critical Section Protection Count");
	RESTORE_WARNING_LEVEL;

    if(protection_count < 0)    {protection_count = 0;}
	if(protection_count)
	{
		// no need to engage protection, it's already protected
	}
	else
	{
		// todo: engage protection (e.g., disable interrupts, or specific interrupts anyway)
	}
	return ++protection_count;
}

int
Cwsw_Critical_Release(int param)
{
	UNUSED(param);

	SUPPRESS_CONST_EXPR;
	cwsw_assert(protection_count > 0, "Invalid Critical Section Protection Count");		// must have valid count, and must have previously engaged protection
	RESTORE_WARNING_LEVEL;

	if(!--protection_count)
	{
		// protection count now zero, disengage protection in some way
		// todo: disengage protection (e.g., reenable the disabled, etc.)
	}
	return protection_count;
}


// ====	COMPONENT DOCUMENTATION ============================================== {
#if defined(IN_DOXY)														/* { */
/* documentation formerly here, moved to *.dox files in the documentation submodule. */
#endif																		/* } */
// ==== /COMPONENT DOCUMENTATION ============================================= }
