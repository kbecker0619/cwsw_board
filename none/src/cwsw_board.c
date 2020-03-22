/** @file
 *	@brief	Board Support Package implementation for the "none" board (i.e., desktop computer
 *	running Windows or Linux).
 *
 *	\copyright
 *	Copyright (c) 2020 Kevin L. Becker. All rights reserved.
 *
 *	Original:
 *	Created on: Dec 27, 2017
 *	Author: kbecker
 */

// ============================================================================
// ----	Include Files ---------------------------------------------------------
// ============================================================================

#include "projcfg.h"

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

/**	Set the enable/disable status of the USB VBUS switch.
 *	This function is a shell on top of the functionality provided by the MHC-generated code; we
 *	break it apart like this because in our thinking, the MHC intermingles too much between the
 *	capabilities of the board, and the business logic that relies on those capabilities.
*/
void
Cwsw_Board__UsbVbusSwitchStateSet(tDO_LogicalValues state)
{
	UNUSED(state);
	/* Enable or disable the VBUS switch */
	/* in this instance, the logical value passed in is of the same polarity as
	 * the physical state required to enable or disable the USB V switch.
	 */
	PLIB_PORTS_PinWrite( PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_5, state );
}


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

	Set(Cwsw_Board, UsbVbus, kLogicalOff);	// Cwsw_Board__UsbVbusSwitchStateSet()
	SET(kBoardLed1, kLogicalOff);
	SET(kBoardLed2, kLogicalOff);
	SET(kBoardLed3, kLogicalOff);

	initialized = true;
	return 0;
}

bool
Cwsw_Board__Get_Initialized(void)
{
	return initialized;
}

