/** @file
 *	@brief	Board Support Package Header File for "none" board - Windows or Linux desktop.
 *
 *	\copyright
 *	Copyright (c) 2020 Kevin L. Becker. All rights reserved.
 *
 *	Created on: Dec 21, 2017
 *	@author Kevin L. Becker
 */

#ifndef CWSW_BOARD_H
#define CWSW_BOARD_H

// ============================================================================
// ----	Include Files ---------------------------------------------------------
// ============================================================================

// ----	System Headers --------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// ----	Project Headers -------------------------
#include "cwsw_arch.h"

/* note: while i myself strongly agree with the coding guideline that says,
 * "no path statements in include statements", i am doing that here in my
 * attempt to support multiple MCU architectures and multiple boards; in my idea,
 * the include paths specified on the command line should point to the correct
 * board family, or board, and the published structure should be identical
 * across all boards.
 *
 * of course there are other, perhaps better, ways to do this, but this is my path during
 * development of my system.
 */
#include "peripheral/ports/ports_api.h"

// ----	Module Headers --------------------------
#include "../cwsw_board_common.h"
#if (XPRJ_Debug_CVI)
#include "cwsw_dio_uir.h"		/* CVI's control defines (PANEL_LED1, PANEL_BTN_1, et. al. */
#endif


#ifdef	__cplusplus
extern "C" {
#endif


// ============================================================================
// ----	Constants -------------------------------------------------------------
// ============================================================================

/** USB VBUS Switch State.
 * Summary:
 * 	Defines the possible states of the USB VBUS Switch on this board
 *
 * Description:
 * 	This enumeration defines the possible states of the USB VBUS Switch on this
 * 	board.
 *
 * Remarks:
 * 	None.
 */
enum eBrdUsbVbusSwitchState
{
	/* USB VBUS Switch disable */
	kBrdUsbVbusSwitchState_Disable = 0,

	/* USB VBUS Switch enable */
	kBrdUsbVbusSwitchState_Enable = 1
};


/** Button IDs for this board. */
enum eBoardButtons
{
	kBoardButtonNone,
	kBoardButton0,
	kBoardButton1,
	kBoardButton2,
	kBoardButton3,
	kBoardButton4,
	kBoardButton5,
	kBoardButton6,
	kBoardButton7,
	kBoardNumButtons
};

/** tBoardLed.
 * Summary:
 *	Defines the LEDs available on this board.
 * Description:
 * 	This enumeration defines the LEDs available on this board.
 * Remarks:
 * 	None.
 */
#if (XPRJ_Debug_CVI)
enum eBoardLeds
{
	kBoardLed1 = PANEL_LED1,
	kBoardLed2 = PANEL_LED2,
	kBoardLed3 = PANEL_LED3,
	kBoardNumLeds = 3
};

#else
enum eBoardLeds
{
	kBoardLed1,
	kBoardLed2,
	kBoardLed3,
	kBoardLed4,
	kBoardNumLeds
};

#endif


// ============================================================================
// ----	Type Definitions ------------------------------------------------------
// ============================================================================

typedef enum eBrdUsbVbusSwitchState	tBrdUsbVbusSwitchState;
typedef enum eBrdSwitch				tBrdUserSwitch;
//typedef enum eBSP_SWITCH_STATE	tBrdUserSwitchState;

typedef enum eBoardLeds				tBoardLed;


// ============================================================================
// ----	Public Variables ------------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Public API ------------------------------------------------------------
// ============================================================================

// --- discrete functions --------------------------------------------------- {

// --- /discrete functions -------------------------------------------------- }

// --- targets for Get/Set APIS --------------------------------------------- {

/** Target for `SET(kBoardLed1, kLogicalOff);`
 *	This function is equivalent to an arch-layer function that directly writes to the ports that
 *	drive the output "LED1".
 *	@param [in]	onoff On or off indication. In this implementation, there is no notion of "tristate".
 *	@note This is a "local" API, not designed to work across components.
 *	@{
 */
#define SET_kBoardLed1(onoff)				Set(Cwsw_Board, kBoardLed1, onoff)
#define SET_kBoardLed2(onoff)				Set(Cwsw_Board, kBoardLed2, onoff)
#define SET_kBoardLed3(onoff)				Set(Cwsw_Board, kBoardLed3, onoff)
#define SET_kBoardLed4(onoff)				Set(Cwsw_Board, kBoardLed4, onoff)
/**	@} */

/** Target for `Set(Cwsw_Board, kBoardLed, on_off)`
 * 	@{
 */
extern void Cwsw_Board__Set_kBoardLed1(bool value);
extern void Cwsw_Board__Set_kBoardLed2(bool value);
extern void Cwsw_Board__Set_kBoardLed3(bool value);
extern void Cwsw_Board__Set_kBoardLed4(bool value);
/**	@} */

// --- /targets for Get/Set APIS -------------------------------------------- }


#ifdef	__cplusplus
}
#endif

#endif /* CWSW_BOARD_H */
