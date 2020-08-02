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

static int panelHandle = NULL;


// ========================================================================== }
// ----	Private Functions -----------------------------------------------------
// ========================================================================== {

// ========================================================================== }
// ----	Public Functions ------------------------------------------------------
// ========================================================================== {

int CVICALLBACK
tmHeartbeat(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
	case EVENT_TIMER_TICK:
		tedlos_schedule(pOsEvqx);
		break;

	default:
		break;
	}

	return 0;
}

// ---- General Functions --------------------------------------------------- {
uint16_t
Cwsw_Board__Init(void)
{
	if(!Get(Cwsw_Arch, Initialized))
	{
		return kErr_Lib_NotInitialized;
	}

	// initialize UI lib. in this environment, no command line options are available (argc, argv).
	if(!InitCVIRTE (0, NULL, 0))	{ return kErr_Board_NoMem; }						/* out of memory */
	panelHandle = LoadPanel (0, "board.uir", PANEL);
	if(panelHandle < 0)				{ return kErr_Board_NoPanel; }

	DisplayPanel (panelHandle);

	SET(kBoardLed1, kLogicalOff);
	SET(kBoardLed2, kLogicalOff);
	SET(kBoardLed3, kLogicalOff);
	SET(kBoardLed4, kLogicalOff);

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
	RunUserInterface ();
	DiscardPanel(panelHandle);
}

// ---- /General Functions -------------------------------------------------- }

// ---- Common API / Highly Customized -------------------------------------- {

void
Cwsw_Board__Set_kBoardLed1(bool value)
{
	int a = SetCtrlVal(panelHandle, PANEL_Red, value);

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
