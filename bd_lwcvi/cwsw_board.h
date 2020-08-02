/** @file
 *	@brief	Board Support Package Header File for board that uses LabWindows/CVI for the simulated
 *	board - Windows desktop.
 *
 *	Copyright (c) 2020 Kevin L. Becker. All rights reserved.
 *
 *	Original:
 *	Created on: Jul 30, 2020
 *	Author: kevin
 */

#ifndef BOARD_BD_LWCVI_H
#define BOARD_BD_LWCVI_H

// ============================================================================
// ----	Include Files ---------------------------------------------------------
// ============================================================================

// ----	System Headers --------------------------
#include <stdint.h>			/* uint16_t */
#include <stdbool.h>		/* bool */

// ----	Project Headers -------------------------
#include "tedlos.h"
#include <cvirte.h>
#include <userint.h>

// ----	Module Headers --------------------------
#include "../cwsw_board_common.h"
#include "cwsw_board_ui.h"


#ifdef	__cplusplus
extern "C" {
#endif


// ============================================================================
// ----	Constants -------------------------------------------------------------
// ============================================================================

/** Button IDs for this board. */
enum eBoardButtons
{
	kBoardButtonNone,/**< kBoardButtonNone */
	kBoardButton0,   /**< kBoardButton0 */
	kBoardButton1,   /**< kBoardButton1 */
	kBoardButton2,   /**< kBoardButton2 */
	kBoardButton3,   /**< kBoardButton3 */
	kBoardNumButtons /**< kBoardNumButtons */
};

enum eBoardLeds
{
	kBoardLed1,
	kBoardLed2,
	kBoardLed3,
	kBoardLed4,
	kBoardNumLeds
};


// ============================================================================
// ----	Type Definitions ------------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Public Variables ------------------------------------------------------
// ============================================================================

extern tCwswSwAlarm	Btn_tmr_ButtonRead;


// ============================================================================
// ----	Public API ------------------------------------------------------------
// ============================================================================

// ---- Discrete Functions -------------------------------------------------- {
extern void		Btn_tsk_ButtonRead(tEvQ_Event ev, uint32_t extra);
extern void		Cwsw_Board__StartScheduler(ptEvQ_QueueCtrlEx pEvqx);

// ---- /Discrete Functions ------------------------------------------------- }

// ---- Targets for Get/Set APIs -------------------------------------------- {

/** Target for `SET(kBoardLed1, kLogicalOff);`
 *	This function is equivalent to an arch-layer function that directly writes to the ports that
 *	drive the output "LED1".
 *	@param [in]	value	On or off indication. In this implementation, there is no notion of "tristate".
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

/** Target 1 for TM(tmr) */
#define GET_tmrdebounce()	Cwsw_GetTimeLeft(tmrdebounce)	/* timer local to one SM state */
#define GET_tmrPressed()	Cwsw_GetTimeLeft(tmrPressed)	/* timer local to one SM state */

// ---- /Targets for Get/Set APIs ------------------------------------------- }


#ifdef	__cplusplus
}
#endif

#endif /* BOARD_BD_LWCVI_H */
