/** @file
 *	@brief	Board support for the LabWindows/CVI "board".
 *	
 *	Copyright (c) 2020 Kevin L. Becker. All rights reserved.
 *	
 *	Original:
 *	Created on: Jul 31, 2020
 *	Author: kevin
 */

// ============================================================================
// ----	Include Files ---------------------------------------------------------
// ========================================================================== {

// ----	System Headers --------------------------
#include <stdbool.h>

// ----	Project Headers -------------------------
#include "cwsw_arch.h"		// Get(Iniitalized)

// ----	Module Headers --------------------------
#include "cwsw_board.h"

#include "ManagedAlarms.h"	// temporary until i get architecture sorted out. the BSP should not know about
#include "tedlosevents.h"


// ========================================================================== }
// ----	Constants -------------------------------------------------------------
// ========================================================================== {

// ========================================================================== }
// ----	Type Definitions ------------------------------------------------------
// ========================================================================== {

// ========================================================================== }
// ----	Global Variables ------------------------------------------------------
// ========================================================================== {

// ========================================================================== }
// ----	Module-level Variables ------------------------------------------------
// ========================================================================== {


static bool initialized = false;

static ptEvQ_QueueCtrlEx pOsEvqx = NULL;


// ========================================================================== }
// ----	Private Functions -----------------------------------------------------
// ========================================================================== {

// ========================================================================== }
// ----	Public Functions ------------------------------------------------------
// ========================================================================== {


// ---- General Functions --------------------------------------------------- {
uint16_t
Cwsw_Board__Init(void)
{
	initialized = true;
	return kErr_Bsp_NoError;
}

bool
Cwsw_Board__Get_Initialized(void)
{
	return initialized;
}

void
Cwsw_Board__StartScheduler(ptEvQ_QueueCtrlEx pEvqx)
{
	extern void Btn_SetQueue(ptEvQ_QueueCtrlEx pEvqx);
	pOsEvqx = pEvqx;		// save for heartbeat usage
	Btn_SetQueue(pEvqx);
//	gtk_main();
}

// ---- /General Functions -------------------------------------------------- }

// ---- Common API / Highly Customized -------------------------------------- {

void
Cwsw_Board__Set_kBoardLed1(bool value)
{
}

void
Cwsw_Board__Set_kBoardLed2(bool value)
{
}

void
Cwsw_Board__Set_kBoardLed3(bool value)
{
}

void
Cwsw_Board__Set_kBoardLed4(bool value)
{
}

// ---- /Common API / Highly Customized ------------------------------------- }

// ========================================================================== }
