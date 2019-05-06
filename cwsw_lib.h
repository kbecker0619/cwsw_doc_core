/** @file
 *	@brief	API for CWSW Library.
 *
 *	Copyright (c) 2019 Kevin L. Becker. All rights reserved.
 *
 *	Original:
 *	Created on: Jun 4, 2017
 *	Author: kbecker
 */

#ifndef CWSW_LIB_H_
#define CWSW_LIB_H_

// ============================================================================
// ----	Include Files ---------------------------------------------------------
// ============================================================================

// ----	System Headers --------------------------
#include <stdint.h>		/* standard integer type definitions */
#include <stdbool.h>	/* bool */
#include <stdio.h>		/* __FILE__, printf(), etc. */

// ----	Project Headers -------------------------
#include "projcfg.h"

// ----	Module Headers --------------------------


#ifdef	__cplusplus
extern "C" {
#endif


// ============================================================================
// ----	Constants -------------------------------------------------------------
// ============================================================================
#define CWSW_LIB_H__REVSTRING "$Revision: 0123 $"


// ============================================================================
// ----	Type Definitions ------------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Public Variables ------------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Public API ------------------------------------------------------------
// ============================================================================

/** Module initialization function. */
extern uint16_t 			Cwsw_Lib__Init(void);

/** Retrieve the component's initialization status. */
extern bool 				Cwsw_Lib__Get_Initialized(void);

/** @defgroup	cwsw_lib_crit_section_group	Critical Section / Protected Region API
 *
 *	The CWSW Critical Section API is designed to be a small wrapper on top of
 *	the CPU architecture's method for enabling or disabling interrupts, or, if
 *	the design allows other methods of protection that are less restrictive
 *	than interrupt manipulation, to invoke that method also.
 *
 *	The heart of the actual protection mechanism is left to the project, by
 *	defining the name of the callout.
 *
 *	The Critical Section API intentionally ignores the "initialized" status
 *	of the CWSW Library component itself, and operates as designed regardless
 *	of whether or not the CWSW Library component has been initialized.
 *
 *	@ingroup	cwsw_lib_object_group
 *	@{
 */
extern int Cwsw_Critical_Protect(int cs_prot_level);
extern int Cwsw_Critical_Release(int cs_prot_level);
/**	@} */


//	===	definitions common to all environments ================================

#if defined (__GNUC__)					/*{*/
/** GNU's recommended implementation of macros using _Pragma keyword */
#define DO_PRAGMA(x)					DO_PRAGMA_(x)
#define DO_PRAGMA_(x)					_Pragma(TO_STRING(x))

/** Building block for creating a macro to ignore a specific compiler warning. This is the
 *  argument to the "ignore" command used in the #DISABLE_WARNING macro.
 */
#define GCC_WARNING_STRING(warnname)	"-W" TO_STRING(warnname)
/** disable a specific warning. intended to take as a parameter, the unquoted readable name of the warning. */
#define DISABLE_WARNING(x)				DO_PRAGMA(GCC diagnostic ignored GCC_WARNING_STRING(x))

/* these are defined as parameterless-but-otherwise-FLM on purpose; right or wrong, my intention is
 * to make it clear that these are compiler directives, and do not expand to code.
 */
#define SAVE_WARNING_CONTEXT			DO_PRAGMA(GCC diagnostic push)
#define RESTORE_WARNING_CONTEXT			DO_PRAGMA(GCC diagnostic pop)

/** Suppress non-ISO but GCC-supported extensions, such as the __FUNCTION__ macro to retrieve as a
 * 	string the current function name.
 */
#define SUPPRESS_EXTRAISO_IDENT			SAVE_WARNING_CONTEXT; DISABLE_WARNING(pedantic)

/** Implement the macro to ignore a constant expression in an if() test. GCC does not directly
 *  support this warning, but this is defined for compatibility with code that could be compiled for
 *  other compilers that do support the warning.
 */
#define SUPPRESS_CONST_EXPR				SAVE_WARNING_CONTEXT /* no GCC compiler warning addresses this */

#elif defined(_MSC_VER)					/*}{*/

#define DISABLE_WARNING(warn_num)		__pragma(warning(disable:warn_num))
#define SAVE_WARNING_CONTEXT			__pragma(warning(push))
#define RESTORE_WARNING_CONTEXT			__pragma(warning(pop))

/** Suppress MSVC's complaint about an if() test using a constant expession. */
#define SUPPRESS_CONST_EXPR				SAVE_WARNING_CONTEXT; DISABLE_WARNING(4127)

/** Suppress non-ISO but GCC-supported extensions, such as the __FUNCTION__ macro to retrieve as a
 * 	string the current function name.
 */
#define SUPPRESS_EXTRAISO_IDENT			SAVE_WARNING_CONTEXT; DISABLE_WARNING(4555)

#endif									/*}*/


/**	Macro to allow (for example) the use of enumeration names in a string. */
#define TO_STRING(x)	#x

/**	Extract the low-order byte out of a 16-bit word. */
#define LSB_16(word16)	(uint8_t)((uint16_t)word16 & (uint8_t)0xFF)

/**	Extract the high-order byte out of a 16-bit word. */
#define MSB_16(word16)	(uint8_t)(((uint16_t)word16 / 256U) & (uint8_t)0xFF)

/** Coerce parameter to a U8 (i.e., unsigned byte).
 *	Usage of this indicates you intend to convert incompatible types, perhaps more clearly than a
 *	simple cast might.
 */
#define TO_U8(a)		(uint8_t)(a)
/** Coerce parameter to a S8 (i.e., signed byte).
 *	Usage of this indicates you intend to convert incompatible types, perhaps more clearly than a
 *	simple cast might. */
#define TO_S8(a)		(int8_t)(a)
/** Coerce var to a U16. Usage of this indicates you intend to convert incompatible types. */
#define TO_U16(a)		(uint16_t)(a)
/** Coerce var to a S16. Usage of this indicates you intend to convert incompatible types. */
#define TO_S16(a)		(int16_t)(a)
/** Coerce var to a U32. Usage of this indicates you intend to convert incompatible types. */
#define TO_U32(a)		(uint32_t)(a)
/** Coerce var to a S32. Usage of this indicates you intend to convert incompatible types. */
#define TO_S32(a)		(int32_t)(a)

/** Coerce parameter to a plain-Jane 'int'.
 *	While many coding standards, including MISRA, specifically prohibit the willy-nilly,
 *	uncontrolled usage of the built-in canonical types of 'C', nevertheless there are times when
 *	you may want to use the native format of the CPU's register (which is the definition of 'int').
 *	One example might be, in a complex formula where the compiler is complaining that transient
 *	calculations might force a conversion to or from 'int', you can use this to specifically
 *	suppress the warning. Usage implies that the review board accepts the justification supplied as
 *	part of the review process.
 */
#define TO_INT(a)		(int)(a)

/**	Determine if "val" is in between values "low" and "hi", inclusive.
 *	@note This macro is not safe, in that it evaluates one or more arguments
 *	more than once.
 */
#define IN_RANGE(val, low, hi)	(((val) <= (low)) && ((val) >= (hi)))


/** @defgroup	bitmap_api	Bit Manipulation and Test
 * 	@ingroup	cwsw_lib_object_group
 *	@{
 */
/** Extract the value of a specific bit within a scalar bitmap.
 *	@param[in] mem	A value of any integral type.
 *	@param[in] bit 	Base-0 bit number.
 *	@note Does not work on array (vector) bitmaps.
 *	@ingroup bitmap_api
 */
#define BIT_TEST(mem, bit)		!!((mem)&(1U<<(bit)))

/** Set the specified bit in a bitmap.
 *	@param[in,out] mem	A value of any integral type.
 *	@param[in] bit	Base-0 bit number.
 *	@note Does not work on array (vector) bitmaps.
 *	@ingroup bitmap_api
 */
#define BIT_SET(mem, bit)		((mem)|=(1U<<(bit)))

/** Clear (reset) the specified bit in a bitmap.
 *	@param[in,out] mem	A value of any integral type.
 *	@param[in] bit	Base-0 bit number.
 *	@note Does not work on array (vector) bitmaps.
 *	@ingroup bitmap_api
 */
#define BIT_CLR(mem, bit)		((mem)&=~(1U<<(bit)))

/** Flip the value of a specified bit in a bitmap.
 *	Sets the bit if it was clear (reset) before invocation, or resets the bit if it was set.
 *	@param[in,out] mem	A value of any integral type.
 *	@param[in] bit	Base-0 bit number.
 *	@note Does not work on array (vector) bitmaps.
 *	@ingroup bitmap_api
 */
#define BIT_TOGGLE(mem, bit)	((mem)^=(1U<<(bit)))

/** @} */


/** Determine the number of elements in a table */
#define TABLE_SIZE(table)		(sizeof(table) / sizeof(table[0]))


//! Return the maximum value of two parameters.
//! When available, this macro should be an alias for the compiler-supplied version.
//! This version is provided for compilers that do not provide an intrinsic version.
//! \note This version is \b UNSAFE in that it has side effects (it evaluates one or more arguments more than once)
#if defined(_MSC_VER)
	// msvc has max/min in stdlib, gnuc evidently doesn't
	#define MAX(a, b)	max(a, b)

#else
	#define MAX(a, b)   (((a) > (b)) ? (a) : (b))

#endif

//! Return the minimum value of two parameters.
//! When available, this macro should be an alias for the compiler-supplied version.
//! This version is provided for compilers that do not provide an intrinsic version.
//! \note This version is \b UNSAFE in that it has side effects (it evaluates one or more arguments more than once)
#if defined(_MSC_VER)
	// msvc has max/min intrinsics in stdlib, gnuc evidently doesn't
	#define MIN(a, b)   min(a, b)

#else
	#define MIN(a, b)   (((a) < (b)) ? (a) : (b))

#endif


/**	Eliminate compiler and static-checker warnings about an unused function
 *	parameter. Note for C89 code, any invocation must follow your variables definitions.
 */
#if !defined(UNUSED)
#define UNUSED(x) (void)x
#endif


/** "Chapter Designator" for Get/Set API.
 *	Intentionally unused symbol, designed to get you to the correct starting
 *	point, when you want to find macros for the Get/Set API; simply highlight
 *	the Module argument in your IDE (e.g, Eclipse, NetBeans, etc.), and select
 *	Go To Definition.
 */
enum { Cwsw_Lib = 0 };	/**< Module-specific identifier, used in API macros */


/*  Initialize API description for the CWSW Library components (all of them) */
/**	@defgroup	cwsw_lib_init_group		Initialize API
 * 	@ingroup	cwsw_lib_object_group
 */
/**	Abstract module initialization. This is the exposed, 1st-level API.
 *
 *	The intention is, all modules use the same signature for their init
 *	function, so use this API to make it more obvious to the code maintainer
 *	that we're using a standardized (template) init function.
 *
 *	@ingroup	cwsw_lib_init_group
 *
 *	@xreq{SR_LIB_0002}	(Primary)
 *	@{
 */
#define Init(instance)						_INIT(instance)

/**	This is the 2nd level of a 2-level expansion; this is done because it is
 *	possible to pass a macro to the 1st-level (external) API, and we want the
 *	macro to be fully expanded when the concatenation is done. We don't expect
 *	to pass nested macro definitions.
 *
 *	@xreq{SR_LIB_0002}	(Supports)
 */
#define _INIT(instance)						instance ## __Init()

/**	Signature for the initialization API for the CWSW Library.
 *	@xreq{SR_LIB_0002}	(Supports)
 *
 *	@return	Error code, or 0 for success.
 */
typedef uint16_t (*fpInit)(void);
/**	@} */


/*  Task API description for the CWSW Library components (all of them) */
/**	@defgroup	cwsw_lib_task_group		Task API
 * 	@ingroup	cwsw_lib_object_group
 */
/**	Abstract Module task function. This is the exposed, 1st-level API.
 *
 *	The intention is, all modules use the same signature for their task
 *	function, so use this API to make it more obvious to the code maintainer
 *	that we're using a standardized (template) task function.
 *
 *	@ingroup	cwsw_lib_task_group
 *
 *	@xreq{SR_LIB_0200}	(Primary)
 *	@{
 */
#define Task(instance)						_TASK(instance)

/**	This is the 2nd level of a 2-level expansion; this is done because it is
 * 	possible to pass a macro to the 1st-level (external) API, and we want the
 * 	macro to be fully expanded when the concatenation is done. We don't expect
 * 	to pass nested macro definitions.
 *
 * 	@xreq{SR_LIB_0200}	(Supports)
 */
#define _TASK(instance)						instance ## __Task()

/** Signature for the task API for the CWSW Library.
 *
 *	@xreq{SR_LIB_0200}	(Supports)
 */
typedef void (*fpTask)(void);
/** @} */


/**	Get the value of a module's resource or attribute.
 * <dl><dt><b>Usage:</b></dt><dd>Get(Module, Attribute);</dd></dl>
 * <dl><dt><b>Example:</b></dt><dd><code>bool is_init = Get(Cwsw_Lib, Initialized);</code></dd></dl>
 */
#define Get(component, resource)			_GET1(component, resource)
#define _GET1(component, resource)			component ## __Get(resource)

/** Set a module's attribute to the specified value.
 * <dl><dt><b>Usage:</b></dt><dd>Set(Module, Attribute, Value);</dd></dl>
 * <dl><dt><b>Example:</b></dt><dd><code>Set(AudioMeter, NeedleResponse, Dampening_VU);</code></dd></dl>
 */
#define Set(component, resource, value)		_SET1(component, resource, value)
#define _SET1(component, resource, value)	component ## __Set(resource, value)


/** Get the value of a global resource.
 * <dl><dt><b>Usage:</b></dt><dd>GET(GlobalAttribute);</dd></dl>
 * <dl><dt><b>Example:</b></dt><dd><code>GET(DiagnosticsMode);</code></dd></dl>
 */
#define GET(item)							_GET2(item)
#define _GET2(thing)						GET_ ## thing()


/** Set the value of a global resource.
 *	<dl><dt>@b Usage: </dt><dd>SET(GlobalAttribute, Value);</dd></dl>
 *	<dl><dt>@b Example: </dt><dd><tt>SET(DiagnosticsMode, kDiagMode_Mfg);</tt></dd></dl>
 */
#define SET(item, value)					_SET2(item, value)
#define _SET2(item, value)					SET_ ## item(value)


/**	Is specified condition true?
 *	@return true if condition is true, false otherwise.
 */
#define IS(cond)      						(bool)(GET(cond) != false)


/*	=== dev-on-PC API =========================================================
 */
/** @fn dbg_printf(format, args...)
 *	Redirectable replacement for printf() statement.
 * 	per https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html, variadics used
 *	as we're using them here, will break on a non-GNU compiler. buyer beware.
 */
#if defined(__GNUC__)	/* --- GNU Environment ------------------------------ */
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wvariadic-macros"
	#define	dbg_printf(format, args...)	(void)printf(format, ## args)
	#pragma GCC diagnostic pop

#elif defined(_CVI_) || defined(_MSC_VER)
	#define dbg_printf					printf

#else
	#define dbg_printf(format, args...)	do {} while(0)

#endif

#if (XPRJ_Win_MinGW_Debug) || (XPRJ_Win_Cygwin_Debug_UT) || (XPRJ_Debug_Linux_GCC)
	/**	Disable console buffering.
	 * 	Within eclipse, on Windows, using MinGW, it seems that stdout
	 *	buffering is broken. The advice I've found is to disable said
	 *	buffering. This is supposed to do that.
	 *
	 *	The alternative would be to directly open a console window; this
	 *	involves changing .gdbinit and is described at
	 *	http://stackoverflow.com/questions/13035075/printf-not-printing-on-console
	 *	and
	 *	https://mirlab.wordpress.com/2014/02/23/no-console-output-in-eclipse-with-cdt-in-windows/
	 */
	#define disable_console_buffering()	setvbuf(stdout, NULL, _IONBF, 0)

#else
	#define disable_console_buffering()	do {} while(0)

#endif

#if (XPRJ_Win_MinGW_Debug)		||  \
	(XPRJ_Win_Cygwin_Debug_UT)	||	\
	(XPRJ_Debug_Linux_GCC)		||  \
	(XPRJ_NB_Debug)				||  \
	(XPRJ_CVI_Debug) 			||  \
	(XPRJ_Win_MSVC_Debug)
	/**	When built on a PC, it's likely I want to see module elements that we would
	 *	otherwise want to keep secret.
	 */
	#define PRIVATE						/* nothing */

#else
	/**	when built on a PC, it's likely i want to see module elements that we would
	 *	otherwise want to keep secret.
	 */
	#define PRIVATE						static

#endif

/** @fn cwsw_assert(condition_to_test, optional_description...)
 *	CWSW assertion check.
 *	The intent of this assertion is not exactly the same as that of the
 *	canonical assert() statement in ISO C; in this implementation, intended as
 *	it is for deeply embedded systems, you should be able to redirect it to a
 *	logging function with breakpoint, or other functionality as appropriate.
 *
 *	We implement the API as a macro so that we have access to the preprocessor's
 *	predefined symbols.
 *
 *	Some coding standards (especially those safety- and security-related)
 *	advocate that at least some types of assertions should always be defined, as
 *	opposed to the canonical "assert" which disappears if #NDEBUG is defined.
 *	For this reason, this macro always evaluates its arguments, in all
 *	environments.
 *
 *	@note per https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html, variadics
 *	used as we're using them here, will break on a non-GNU compiler. buyer beware.
 */
#if defined(__GNUC__)	/* --- GNU Environment ------------------------------ */
//	#pragma GCC diagnostic push
//	#pragma GCC diagnostic ignored "-Wvariadic-macros"
#endif

#define	cwsw_assert(test, descrip)	do { if(!(test)) { cwsw_assert_helper(#test, __FILE__, __LINE__, descrip); } } while(0)
extern void cwsw_assert_helper(char const * const test, char const * const filename, int const lineno, char const * const descrip);

#if defined(__GNUC__)	/* --- GNU Environment ------------------------------ */
//	#pragma GCC diagnostic pop
#endif


/** Target symbol for Get(Cwsw_Lib, xxx) interface */
#define Cwsw_Lib__Get(a)				Cwsw_Lib__Get_ ## a()


#ifdef	__cplusplus
}
#endif

#endif /* CWSW_LIB_H_ */
