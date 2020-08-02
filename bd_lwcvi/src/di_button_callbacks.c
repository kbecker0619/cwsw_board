/** @file
 *	@brief
 *
 *	Copyright (c) 2020 Kevin L. Becker. All rights reserved.
 *
 *	Original:
 *	Created on: Jun 29, 2020
 *	Author: kevin
 */

// ============================================================================
// ----	Include Files ---------------------------------------------------------
// ============================================================================

// ----	System Headers --------------------------

// ----	Project Headers -------------------------

// ----	Module Headers --------------------------
#include "cwsw_board.h"	/* pull in the GTK info */


// ============================================================================
// ----	Constants -------------------------------------------------------------
// ============================================================================

//  consecutive 1s: 	----4---- ----3---- ----2---- ----1----
//	noisy input:		1111 1111 0111 1111 0111 1110 1111 1011
#define noisypatterna	0xFF7F7EFB

//	consecutive 0s:     ----4---- ----3---- ----2---- ----1----
// noisy input:			1000 0000 0000 1000 0000 1000 0001 0000
//						0x1 0010 1020 844A
#define noisypatternb	0x10010102

//						1111 1111 1001
#define cleanpatterna	0xFF9
#define cleanpatternb	~cleanpatterna


// ============================================================================
// ----	Type Definitions ------------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Global Variables ------------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Module-level Variables ------------------------------------------------
// ============================================================================

static uint32_t buttoninputbits[kBoardNumButtons]	= {0};
static uint32_t buttonstatus = 0;	// bitmapped image of current button state. 32 bits mostly to avoid compiler warnings.


// ============================================================================
// ----	Private Functions -----------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Public Functions ------------------------------------------------------
// ============================================================================

bool
di_read_next_button_input_bit(uint32_t idx)
{
	bool retval = ((buttoninputbits[idx] & 1) != 0);
	buttoninputbits[idx] /= 2;
	if((!retval) && (!buttoninputbits[idx]))	// if this bit is clear, it could be because the input stream is depleted; if the input stream is also depleted...
	{	// ... then check whether the "button pressed" flag is true
		// toggle buttons didn't work as i wanted - each press also gets a release, but it's a press-release sandwich where the "pressed" status is the meat
//		retval = gtk_toggle_button_get_active((GtkToggleButton *)btn0);	// todo: return state of last button pressed
		retval = BIT_TEST(buttonstatus, idx);
	}
	return retval;
}

int CVICALLBACK
cbBtnGo(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK:
			break;
		case EVENT_COMMIT:	// LW/CVI's equivalent to a mouse-up (release button) event
			break;
		default:
			break;
	}
	return 0;
}

int CVICALLBACK
cbBtnPause(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
		default:
			break;
	}
	return 0;
}

int CVICALLBACK
cbBtnWalk(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
		default:
			break;
	}
	return 0;
}

int CVICALLBACK
cbBtnYellow(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			break;

		default:
			break;
	}
	return 0;
}

int CVICALLBACK
cbPanel(int panel, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_KEYPRESS:
			break;

		case EVENT_GOT_FOCUS:
			break;

		case EVENT_LOST_FOCUS:
			break;

		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;

		default:
			break;
	}

	return 0;
}
