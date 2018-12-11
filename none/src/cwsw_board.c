/** @file
 *	@brief	Board Support Package implementation for the "none" board (i.e., desktop computer
 *	running Windows or Linux).
 *
 *	Description:
 *
 *
 *	Copyright (c) 2019 Kevin L. Becker. All rights reserved.
 *
 *	Original:
 *	Created on: Dec 27, 2017
 *	Author: kbecker
 *
 *	Current:
 *	$Revision: $
 *	$Date: $
 */

// ============================================================================
// ----	Include Files ---------------------------------------------------------
// ============================================================================

#include "projcfg.h"

// ----	System Headers --------------------------

// ----	Project Headers -------------------------
#include "cwsw_lib.h"
#include "peripheral/ports/ports_api.h"
#include "cwsw_eventsim.h"				/* tEventPayload */

// ----	Module Headers --------------------------
#include "cwsw_arch.h"
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
static char const * const bsp_RevString = "$Revision: 0123 $";

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
 * This function "connects" the board to the CPU.
 * It knows nothing about the application; sitting just above this file, would be the "bsp" (to
 * slightly abuse that term), which connects the application to the board.
 * @return error code, where 0 means no problem.
 *
 * i'm on the fence about embedding a dependency here into the architecture - on the one hand, the
 * BSP (next layer up) should manage this, but on the other hand, the board depends so much upon
 * the capabilities of the MCU on that board, that it doesn't make much sense to execute software
 * to initialize the board, when the micro responsible for executing that very self-same software
 * hasn't yet been initialized.
 */
uint16_t
Cwsw_Board__Init(void)
{
	tEventPayload ev = {0};

	if(!Get(Cwsw_Arch, Initialized))
	{
		PostEvent(evNotInitialized, ev);
		return 1;
	}

	#if defined(__GNUC__)	/* --- GNU Environment ------------------------------ */
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	#endif

	dbg_printf(
			"\tModule ID %i\t%s\t%s\n"
			"\tEntering %s()\n\n",
			Cwsw_Board, __FILE__, bsp_RevString,
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

