/** @file
 *	@brief	Board Support Package implementation for the "none" board (i.e., desktop computer
 *	running Windows or Linux).
 *
 *	\copyright
 *	Copyright (c) 2020 Kevin L. Becker. All rights reserved.
 *
 *	Created on: Dec 27, 2017
 *	@author Kevin L. Becker
 */

// ============================================================================
// ----	Include Files ---------------------------------------------------------
// ============================================================================

// ----	System Headers --------------------------

// ----	Project Headers -------------------------
#include "cwsw_lib.h"
#include "cwsw_arch.h"
#include "peripheral/ports/ports_api.h"
#include "cwsw_eventsim.h"				/* tEventPayload */

// ----	Module Headers --------------------------
#include "cwsw_board.h"


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

static bool initialized = false;


// ============================================================================
// ----	Private Prototypes ----------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Public Functions ------------------------------------------------------
// ============================================================================

/** Initialize the board.
 *	This function "connects" the board to the CPU.
 *	It knows nothing about the application; sitting just above this file, would be the "bsp" (to
 *		slightly abuse that term), which connects the application [framework] to the board.
 *
 *	@returns error code, where 0 (#kErr_Lib_NoError) means no problem.
 */
uint16_t
Cwsw_Board__Init(void)
{
	tEventPayload ev = {0};

	if(!Get(Cwsw_Arch, Initialized))
	{
		PostEvent(evNotInit, ev);
		return 1;
	}

	#if defined(__GNUC__)	/* --- GNU Environment ------------------------------ */
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	#endif

	dbg_printf(
			"\tModule ID %i\t%s\t\n"
			"\tEntering %s()\n\n",
			Cwsw_Board, __FILE__,
			__FUNCTION__);

	#if defined(__GNUC__)	/* --- GNU Environment ------------------------------ */
	#pragma GCC diagnostic pop
	#endif

	return 0;
}

bool
Cwsw_Board__Get_Initialized(void)
{
	return initialized;
}

