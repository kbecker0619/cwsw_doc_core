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
 *	@xreq{SR_LIB_0003}	(Supports, Component-specific)
 *
 *	@ingroup	cwsw_lib_init_group
 */
PRIVATE bool	cwsw_lib_initialized = false;

/**	Nesting level for the Critical Section / Protected Region API.
 *	The combination of requirements describing "not-active" vs. "active"
 *	behavior upon both entry into and exit from a Critical Section, combined
 *	with requirements about when to increment or decrement the nesting counter
 *	and the range of the counter, require the presence of this counter.
 *
 * 	@xreq{SR_LIB_0303}	(Primary)
 * 	@xreq{SR_LIB_0304}	(Primary)
 * 	@xreq{SR_LIB_0305}	(Supporting)
 * 	@xreq{SR_LIB_0307}	(Supporting)
 * 	@xreq{SR_LIB_0308}	(Supporting)
 * 	@xreq{SR_LIB_0309}	(Supporting)
 * 	@xreq{SR_LIB_0310}	(Supporting)
 *
 *	@ingroup	cwsw_lib_crit_section_group
 */
PRIVATE int		cwsw_lib_cs_protection_count = 0;


// ============================================================================
// ----	Private Prototypes ----------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Public Functions ------------------------------------------------------
// ============================================================================

/**	@details This function shall be called before the main scheduler is started.
 *	This function's responsibility is to set up the local variables, to prepare
 *	for the task function's 1st call (once the scheduler has been started).
 *
 *	@returns	0 if the component is successfully initialized.
 *	@returns	error code if the component is not initialized.
 *	@note		By design, this component does not depend on specific
 *				enumerated names for the return code; in the CWSW ecosystem,
 *				the simulated event mechanism is used to handle error codes.
 *
 * 	@startuml
 *		System	->		cwsw_lib: Init()
 *		System	\\--	cwsw_lib: Error code
 *	@enduml
 *
 *	@xreq{SR_LIB_0000}	(Primary, Component-specific)
 *	@xreq{SR_LIB_0002}	(Primary, Component-specific)
 *
 *	@note By design, this function unilaterally reinitializes the component and
 *	returns all internal operating stateful behavior to original operating
 *	state. This means it takes exception to @req{SR_LIB_0003} and @req{SR_LIB_0004}
 *
 *	@ingroup	cwsw_lib_init_group
 */
uint16_t
Cwsw_Lib__Init(void)
{
	uint16_t rv = cwsw_lib_initialized ? 2 : 1;	/* already initialized, or not initialized */
	UNUSED(cwsw_lib_RevString);

	SUPPRESS_CONST_EXPR;			/* as these are all compile-time constants, we know they're constant, and do this intenionally; suppress compiler warning for this */
	if(	(XPRJ_Win_MinGW_Debug)	 	||  \
		(XPRJ_Win_Cygwin_Debug_UT)	||	\
		(XPRJ_Debug_Linux_GCC)		||  \
		(XPRJ_NB_Debug)				||  \
		(XPRJ_Win_MSVC_Debug)		||  \
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

	cwsw_lib_initialized = true;
    cwsw_lib_cs_protection_count = 0;
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
	return cwsw_lib_initialized;
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
 * @param[in]	test		The stringified text of the the test that failed the assertion.
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


/**	Enter Critical Section / Protected Region.
 *	This is a genericized API for an architecture-specific implementation.
 *
 * 	This function relies on a Function-Like Macro (FLM) that is configured by
 * 	the integration project, to be whatever is suitable for the MCU
 * 	architecture and system, for the specified protection level.
 *
 *	@param[in] cs_prot_level	Protection level, where 0 is all interrupts
 *								disabled, and every (architecture-specific)
 *								level above "0" is a progressively narrower
 *								scope of protection.
 *	@returns					New nesting level.
 *
 *	@note	If the nesting counter is already at the ceiling upon entry, it
 *			will become invalid after the incrementation. If your system is a
 *			2s-complement system, rollover will occur. To signal this, a CWSW
 *			assertion is made; default behavior of this is to simply write a
 *			log entry to the console and loop for a short, uncalibrated time.
 *			By design, this assertion behavior can be overridden.
 *			<br>
 *			If the nesting counter is so high that this implementation-defined
 *			behavior is invoked, then it is likely there are other major things
 *			that have already or will imminently fail. Our design choice is to
 *			allow the "rollover" (for 2s-complement systems), knowing that the
 *			mating "leave" api will also fail.
 *
 *	@par	Design
 *	@startuml
 *		System		->	 cwsw_lib: Cwsw_Critical_Protect()
 *			cwsw_lib 	->	 SysArch: DisableInterrupts()
 *			cwsw_lib 	\\-- SysArch
 *		System		\\-- cwsw_lib: Nesting Level
 *	@enduml
 *
 *	@xreq{SR_LIB_0301}  API exists.
 *	@xreq{SR_LIB_0303}	Increment nesting counter upon entry, provided counter
 *						is in a valid range.
 *	@xreq{SR_LIB_0305}	Data range of nesting counter.
 *	@xreq{SR_LIB_0306}	Treatment of bad nesting counter value.
 *	@xreq{SR_LIB_0307}	Behavior upon 1st activation.
 *	@xreq{SR_LIB_0308}	Behavior when protection already engaged.
 */
int
Cwsw_Critical_Protect(int cs_prot_level)
{
	UNUSED(cs_prot_level);

	SUPPRESS_CONST_EXPR;
	cwsw_assert((cwsw_lib_cs_protection_count >= 0) &&
                (cwsw_lib_cs_protection_count < INT_MAX),
                "Invalid Critical Section Protection Count");
	RESTORE_WARNING_CONTEXT;

    if(cwsw_lib_cs_protection_count < 0)
    {
    	cwsw_lib_cs_protection_count = 0;
    }
	if(!cwsw_lib_cs_protection_count++)
	{
		CWSW_LIB_CRIT_SECT_ENTER(cs_prot_level);    /* <<== PROJECT-LEVEL CALLBACK to engage protection */
	}
	return cwsw_lib_cs_protection_count;
}


/**	Leave Critical Section / Protected Region.
 *	This is a genericized API for an architecture-specific implementation.
 *
 *	@param[in] cs_prot_level	Protection level, where 0 is all interrupts
 *								disabled, and every (architecture-specific)
 *								level above "0" is a progressively narrower
 *								scope of protection.
 *	@returns					New nesting level.
 *
 *	@note	If the nesting counter is already at the floor upon entry, it will
 *			become invalid after the decrementation. If your system is a
 *			2s-complement system, rollunder will occur. To signal this, a CWSW
 *			assertion is made; default behavior of this is to simply write a
 *			log entry to the console and loop for a short, uncalibrated time.
 *			By design, this assertion behavior can be overridden.
 *			<br>
 *			If the nesting counter is so low that this implementation-defined
 *			behavior is invoked, then it is likely there are other major things
 *			that have already or will imminently fail. Our design choice is to
 *			allow the "rollunder" (for 2s-complement systems), knowing that the
 *			next "enter" api will also fail.
 *
 *	@par	Design
 *	@startuml
 *		System		->	 cwsw_lib: Cwsw_Critical_Protect()
 *			cwsw_lib 	->	 SysArch: EnableInterrupts()
 *			cwsw_lib 	\\-- SysArch
 *		System		\\-- cwsw_lib: Nesting Level
 *	@enduml
 *
 *	@xreq{SR_LIB_0302}  API exists.
 *	@xreq{SR_LIB_0304}	Decrement nesting counter upon entry, provided counter
 *						is in a valid data range.
 *	@xreq{SR_LIB_0305}	Data range of nesting counter.
 */
int
Cwsw_Critical_Release(int cs_prot_level)
{
	UNUSED(cs_prot_level);

	SUPPRESS_CONST_EXPR;
	cwsw_assert(cwsw_lib_cs_protection_count > 0, "Invalid Critical Section Protection Count");		// must have valid count, and must have previously engaged protection
	RESTORE_WARNING_CONTEXT;

	if(!--cwsw_lib_cs_protection_count)
	{
		// protection count now zero, disengage protection in some way
		CWSW_LIB_CRIT_SECT_LEAVE(cs_prot_level);    /* <<== PROJECT-LEVEL CALLBACK to disengage protection */
	}
	return cwsw_lib_cs_protection_count;
}


// ====	COMPONENT DOCUMENTATION ============================================== {
#if defined(IN_DOXY)														/* { */
/* documentation formerly here, moved to *.dox files in the documentation submodule. */
#endif																		/* } */
// ==== /COMPONENT DOCUMENTATION ============================================= }
