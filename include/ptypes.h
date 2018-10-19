/** @file
 *	@brief	Portable Types
 *
 *	Description:
 *
 *	Copyright (c) 2018 Kevin L. Becker. All rights reserved.
 *
 *	Original:
 *	Created on: Feb 2, 2017
 *	Author: kbecker
 *
 *	Current:
 *	$Revision: $
 *	$Date: $
 */

#ifndef PTYPES_H_
#define PTYPES_H_

#ifdef	__cplusplus
extern "C" {
#endif

// ============================================================================
// ----	Include Files ---------------------------------------------------------
// ============================================================================

// ----	System Headers --------------------------
#include <stdint.h>

// ----	Project Headers -------------------------

// ----	Module Headers --------------------------


// ============================================================================
// ----	Constants -------------------------------------------------------------
// ============================================================================
#define PTYPES_H__REVSTRING "$Revision: 0123 $"


// ============================================================================
// ----	Type Definitions ------------------------------------------------------
// ============================================================================

/**	Not sure why, but the TI compiler doesn't instantiate uint8_t.
 *	For that reason and that reason alone, define project-specific aliases that
 *	can be used in any environment.
 */
typedef unsigned char	tU8;
typedef signed char		tS8;
typedef uint16_t		tU16;
typedef uint32_t		tU32;
typedef int16_t			tS16;
typedef int32_t			tS32;


// ============================================================================
// ----	Public Variables ------------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Public API ------------------------------------------------------------
// ============================================================================


#ifdef	__cplusplus
}
#endif

#endif /* PTYPES_H_ */
