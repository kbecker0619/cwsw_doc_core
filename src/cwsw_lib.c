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
 *	For the CWSW Library, the import of this flag is less than in most modules;
 *	there are no state-related or HW initializations that must be done, and
 *	there is no task function that needs to be set up. In addition, nothing in
 *	this module needs to be deinitialized (such as, in preparation for entry
 *	into sleep mode, etc.)
 *
 *	\xreq{SR_LIB_0004}	(Supports, Component-specific)
 *
 *	@ingroup	cwsw_lib_init_group
 */
PRIVATE bool initialized = false;

PRIVATE int protection_count = 0;


// ============================================================================
// ----	Private Prototypes ----------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Public Functions ------------------------------------------------------
// ============================================================================

/**	@details This function shall be called before the main scheduler is started.
 *	This function's responsibility is to set up the local vars, to prepare for
 *	the task function's 1st call (once the scheduler has been started).
 *
 *	@returns	0 if the component is successfully initialized.
 *	@returns	error code if the component is not initialized.
 *	@note		By design, this component does not depend on specific
 *				enumerated names for the return code; in the CWSW ecosystem,
 *				the simulated event mechanism is used to handle error codes.
 *
 * 	@startuml
 *	System	->		cwsw_lib: Init()
 *	System	\\--	cwsw_lib: Error code
 *	@enduml
 *
 *	@xreq{SR_LIB_0002}	(Primary, Component-specific)
 *	@xreq{SR_LIB_0003}	(Primary, Component-specific)
 *
 *	@note By design, this function unilaterally reinitializes the component and
 *	returns all internal operating stateful behavior to original operating
 *	state. This means it takes exception to @req{SR_LIB_0004} and @req{SR_LIB_0005}
 *
 *	@ingroup	cwsw_lib_init_group
 */
uint16_t
Cwsw_Lib__Init(void)
{
	uint16_t rv = initialized ? 2 : 1;	/* already initialized, or not initialized */
	UNUSED(cwsw_lib_RevString);

	SUPPRESS_CONST_EXPR;			/* as these are all compile-time constants, we know they're constant, and do this intenionally; suppress compiler warning for this */
	if(	(XPRJ_Win_MinGW_Debug) 	||  \
		(XPRJ_Win_MinGW_UT)		||	\
		(XPRJ_Debug_Linux_GCC) 	||  \
		(XPRJ_NB_Debug)			||  \
		(XPRJ_Win_MSVC_Debug)	||  \
		(XPRJ_CVI_Debug) )
	{
		disable_console_buffering();

		SUPPRESS_EXTRAISO_IDENT;	/* suppress warning for function name */
		dbg_printf(
				"\tModule ID %i\t%s\t%s\n"
				"\tEntering %s()\n\n",
				Cwsw_Lib, __FILE__, cwsw_lib_RevString,
				__FUNCTION__);
		RESTORE_WARNING_CONTEXT;
	}
	RESTORE_WARNING_CONTEXT;

	initialized = true;
    protection_count = 0;
	if(rv != 2)	/* if not reinitializing, clear error codes */
	{
		rv = 0;
	}
	return rv;
}


/**	@details Target for Get(Cwsw_Lib, Initialized) interface.
 *
 *	@returns	true if component is initialized.
 *	@returns	false if the component is not initialized.
 *
 *	@xreq{SR_LIB_0001}	(Primary, Component-specific)
 *
 *	@ingroup	cwsw_lib_init_group
 */
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


int
Cwsw_Critical_Protect(int param)
{
	UNUSED(param);

	SUPPRESS_CONST_EXPR;
	cwsw_assert((protection_count >= 0) &&
                (protection_count < INT_MAX),
                "Invalid Critical Section Protection Count");
	RESTORE_WARNING_CONTEXT;

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
	RESTORE_WARNING_CONTEXT;

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
