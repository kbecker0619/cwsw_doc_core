/** @file
 *	@brief	One-sentence short description of file.
 *
 *	Description:
 *
 *	Copyright (c) 2018 Kevin L. Becker. All rights reserved.
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
	if( (XPRJ_Debug_Win_MinGW) || (XPRJ_Debug_Linux_GCC) || (XPRJ_Debug_MSC) || (XPRJ_Debug_CVI) )
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

	initialized = true;
	return 0;
}

bool
Cwsw_Lib__Get_Initialized(void)
{
	return initialized;
}

/** Helper function for CWSW Assert statement.
 *
 * @param[in]	test		The stringified text of the the test that failed the assertion
 * @param[in]	filename	The name of the file that held the failed assertion.
 * @param[in]	lineno 		The line number in the file that held the failed assertion.
 * @param[in]	descrip	The user-supplied description that augments the assertion notice. Suitable for a logging statement.
 */
void
cwsw_assert_helper(char const * const test, char const * const filename, int const lineno, char const * const descrip)
{
	volatile uint16_t delay = 0;

	dbg_printf(
		"\nAssertion failed: \"%s\", file::line: %s::%i\nDescription: %s\n\n",
		test, filename, lineno, descrip);

	while(--delay);
}


PRIVATE int protection_count = 0;
int Cwsw_Critical_Protect(int param)
{
	UNUSED(param);
	cwsw_assert(protection_count >= 0, "Invalid Critical Section Protection Count");
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

int Cwsw_Critical_Release(int param)
{
	UNUSED(param);
	cwsw_assert(protection_count > 0, "Invalid Critical Section Protection Count");		// must have valid count, and must have previously engaged protection
	if(!--protection_count)
	{
		// protection count now zero, disengage protection in some way
		// todo: disengage protection (e.g., reenable the disabled, etc.)
	}
	return protection_count;
}


// ====	COMPONENT DOCUMENTATION ============================================== {
#if defined(IN_DOXY)													/* { */
/**	\page Lib_Head Reusable Component : CWSW Library
 * 		@subpage Lib_Introduction \n
 * 		@subpage Lib_Design \n
 * 		@subpage Lib_Integration
 */

/**	@page Lib_Introduction						Introduction to the Library core component
 *	@tableofcontents
 *	The Library contains stateless utility APIs usable by all SW modules, but especially designed
 *	for support of the CWSW reusable components.
 *
 *	@section lib_related_docs					Applicable Documents
 *	TBD
 *
 *	@section lib_terms							Terms, Acronyms, Abbreviations
 */

/** @page Lib_Design							Library Subsystem Design
 *	@tableofcontents
 *		@subpage Lib_Responsibilities \n
 *		@subpage Lib_Constraints \n
 *		@subpage Lib_Interactions \n
 *		@subpage Lib_Composition \n
 *		@subpage Lib_Resources \n
 *		@subpage Lib_Interfaces \n
 *		@subpage Lib_Main_Obj_Diag \n
 *		@subpage Lib_Seq_Diag_Fail \n
 *		@subpage Lib_Dsm \n
 */

/**	@page Lib_Integration						Integrating the Reusable Component
 *	@tableofcontents
 *	To add the facilities of the CWSW Lib component to your project, follow these steps.
 *
 *	@note Author's note: These steps are generic for all IDEs; however, the specific details
 *	(e.g., screen shots) are oriented toward IDEs based on Eclipse, such as NXP S32DS; ARM DS-5; or
 *	TI's Code Composer.
 *	While I have implemented these steps in Microchip's MPLAB v4.x, the steps are so close that I
 *	didn't feel IDE-specific instructions were required.
 *
 *	Integration Steps:
 *	-# [Share the Source](#lib_source_share)
 *	-# [Update Includes paths](#lib_update_includes)
 *	-# [Configuration](#lib_configuration)
 *	-# [Update Initialization Code](#lib_init_code)
 *	-# Optional APIs.
 */

	/**	@page Lib_Integration
	 *	@section lib_source_share				Share the Source
	 *	Designed to be a reusable component, the CWSW Library is organized into a reusable section, and
	 *	requires additional project-specific configuration items in the project.
	 *
	 *	The component layout in your workspace looks like this:
	 *	@code{.sh}
	 *	cwsw_lib_prj
	 *	├── cwsw_lib
	 *	│   ├── include
	 *	│   └── src
	 *	├── doc
	 *	└── ut
	 *	@endcode
	 *
	 *	The <code>cwsw_lib_prj</code> directory is the home to the various project-management files
	 *	(such as the Doxygen configuration, the Eclipse project files, etc.), and is the parent for
	 *	the component.
	 *
	 *	The <code>ut</code> directory is the source for the developer workspace for this reusable
	 *	component, and illustrates how to configure the component is client projects.
	 *
	 *	The <code>doc</code> directory holds relevant design information not included in this
	 *	Doxygen format.
	 *
	 *	The <code>cwsw_lib</code> directory is the sharable and reusable portion of the component.\n
	 *	The design intent is for this folder to be shared into your project's directory structure.
	 *	-	If your code management system can share folders (e.g., PVCS / Dimensions, MKS), this
	 *		might be a good solution; else,
	 *	-	filesystem symlinks are supported by GIT, with appropriate configuration options set
	 *		properly.
	 *	-	Example (using symlinks):\n
	 *	Given the following workspace directory structure:
	 *	@code{.sh}
	 *	$ tree -dL 2
	 *	.
	 *	├── components
	 *	│   ├── cwsw_arch_prj
	 *	│   ├── cwsw_board_prj
	 *	│   └── cwsw_lib_prj
	 *	├── cwsw_example_proj
	 *	│   ├── appl_swc
	 *	│   ├── cfg
	 *	│   ├── drivers
	 *	│   ├── libs
	 *	│   ├── os
	 *	│   ├── rte
	 *	│   └── services
	 *	├── doc
	 *	└── ut
	 *	@endcode
	 *	Execute the following. @note This example is given using Cygwin; the same command line
	 *	verbatim would be done in Git Bash.\n
	 *	Also, 7-Zip can create symlinks, and there are other methods in Windows to create symbolic
	 *	links.
	 *
	 *	@code{.sh}
	 *	$ cd cwsw_example_proj/libs/
	 *	$ ln -s ../../components/cwsw_lib_prj/cwsw_lib cwsw_lib
	 *	$ git add cwsw_lib
	 *	$ tree -dL 4
	 *	../..
	 *	├── components
	 *	│   ├── cwsw_arch_prj
	 *	│   ├── cwsw_board_prj
	 *	│   └── cwsw_lib_prj
	 *	│       ├── cwsw_lib
	 *	│       │   ├── include
	 *	│       │   └── src
	 *	│       ├── doc
	 *	│       └── ut
	 *	├── cwsw_example_proj
	 *	│   ├── appl_swc
	 *	│   ├── cfg
	 *	│   ├── drivers
	 *	│   ├── libs
	 *	│   │   └── cwsw_lib -> ../../components/cwsw_lib_prj/cwsw_lib
	 *	│   ├── os
	 *	│   ├── rte
	 *	│   └── services
	 *	├── doc
	 *	└── ut
	 *	@endcode
	 *
	 */

	/**	@page Lib_Integration
	 *	@section lib_update_includes			Update the Includes Paths
	 *	Update your build system to add the relative path to the <code>cwsw_lib</code> folder. To
	 *	clarify what you're looking for, here is the annotation for each of the files in the
	 *	library project.
	 *	@note The design intent, is for all references within the project, to be relative to the
	 *	project itself. Portability is greatly reduced when the project refers to fully qualified
	 *	pathnames, or those outside of the project tree itself.
	 *
	 *	Here's an annotated listing of the library component itself.
	 *	@code{.sh}
	 *	$ tree -L 3 cwsw_lib_prj/
	 *	cwsw_lib_prj/
	 *	├── cwsw_lib				<<== point your includes path to this folder
	 *	│   ├── ctassert.h			<<== optional header, implements compile-time assertions
	 *	│   ├── cwsw_eventsim.h		<<== optional header, provides simulated event handling
	 *	│   ├── cwsw_lib.h			<<== main API
	 *	│   ├── include
	 *	│   │   └── ptypes.h		<<== optional header, provides Portable Typedefs for basic types
	 *	│   ├── readme.md
	 *	│   └── src
	 *	│       └── cwsw_lib.c		<<== implementation file
	 *	├── doc
	 *	│   └── readme.md
	 *	├── readme.md
	 *	└── ut
	 *	    ├── cwsw_lib_test.c		<<== testing / unit test / example-how-to-use source
	 *	    ├── projcfg.h			<<== Project configuration. All CWSW clients need this.
	 *	    └── readme.md
	 *	@endcode
	 *
	 *	Here's an annotation, in the example project.
	 *	@code{.sh}
	 *	$ tree
	 *	cwsw_example_proj
	 *	├── appl_swc												<<== application software
	 *	│   ├── CoinVendActuator.c
	 *	│   ├── CoinVendActuator.h
	 *	│   ├── CoinVendMgr.c
	 *	│   ├── CoinVendMgr.h
	 *	│   ├── CoinVendMgr_prv.h
	 *	│   ├── CoinVendMgr_pub.h
	 *	│   ├── CoinVendSensors.c
	 *	│   ├── CoinVendSensors.h
	 *	│   └── CoinVendSensors_prv.h
	 *	├── cfg														<<== configuration files for CWSW lib, ect. (suggestion)
	 *	├── drivers
	 *	├── libs
	 *	│   └── cwsw_lib -> ../../components/cwsw_lib_prj/cwsw_lib	<<== add this to the project's includes path
	 *	├── os
	 *	├── rte
	 *	└── services
	 *	@endcode
	 */

	/**	@page Lib_Integration
	 *	@section lib_configuration				Configure the Reusable Component
	 *	If the CWSW Library finds certain symbols defined on the compiler command line, some minor
	 *	optional functionality is enabled.
	 *
	 *	If either <code>XPRJ_Debug_Win_MinGW</code> or <code>XPRJ_Debug_Linux_GCC</code> are defined
	 *	with a numeric value other than zero or #false, then a function-like macro is enabled that
	 *	disables console buffering: <code>disable_console_buffering()</code>.
	 *
	 *	In addition, if either of those symbols are defined, or if the library detects that it's
	 *	being compiled in LabWindows/CVI or MS Studio Visual C, a new symbol for defining the
	 *	storage class of a symbol is defined: <code>#PRIVATE</code>. The purpose of this symbol is
	 *	to allow visibility into module-specific symbols for Unit Testing purposes.
	 *
	 */

	/**	@page Lib_Integration
	 *	@section lib_init_code					Initialize the Reusable Component
	 *	As of the time of this writing, there is little actual initialization required by the
	 *	Library component itself; however, some functionality does check to see that the library
	 *	is initialized before it works correctly.
	 *
	 *	Many of the CWSW Library components rely on functionality provided by the core reusable
	 *	Library component itself. Because the CWSW Library module is stateless and has no
	 *	dependencies on specific architecture or board features, the Init call should be done first,
	 *	before other members of the CWSW Library en banc.
	 *
	 *	One of the APIs introduced is an abstraction intended to give the same signature to all
	 *	CWSW modules: <code>Init(Component)</code>. This is optional, but when used, looks like
	 *	this:
	 *	@code{.c}
		void main(void)
		{
			(void)Init(Cwsw_Lib);
			... more initilization
			... main code follows ...
		}
	 *	@endcode
	 */

#endif																	/* } */
// ==== /COMPONENT DOCUMENTATION ============================================= }

