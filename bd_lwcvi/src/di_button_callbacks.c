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
		retval = BIT_TEST(buttonstatus, idx);
	}
	return retval;
}

int CVICALLBACK
cbBtn0(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
	case EVENT_LEFT_CLICK:
		/* using the pattern established for GTK, if the current bit stream is not empty, add the
		 * new bit stream to the "far end" of the current bitstream. otherwise, just add the new
		 * bitstream directly.
		 */
		buttoninputbits[kBoardButton0] += cleanpatterna * 4096; // clean pattern is 12 bits
		BIT_SET(buttonstatus, kBoardButton0);
		break;

	case EVENT_COMMIT:	// LW/CVI's equivalent to a mouse-up (button release) event
		buttoninputbits[kBoardButton0] += ((cleanpatternb & 0xFFF) * 4096);
		BIT_CLR(buttonstatus, kBoardButton0);

		/* running commentaire, to be moved to more formal documentation.
		 * - if the DI button SM is in the "released" state, the 1st 1 bit will provoke a transition to
		 *   the debounce-press state; after the bit stream settles down to all 0s, it'll return to the
		 *   released state.
		 */
		break;

	default:
		break;
	}
	return 0;
}

int CVICALLBACK
cbBtn1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
	case EVENT_LEFT_CLICK:
		buttoninputbits[kBoardButton1] += cleanpatterna * 4096; // clean pattern is 12 bits
		BIT_SET(buttonstatus, kBoardButton1);
		break;

	case EVENT_COMMIT:
		buttoninputbits[kBoardButton1] += ((cleanpatternb & 0xFFF) * 4096);
		BIT_CLR(buttonstatus, kBoardButton1);
		break;

	default:
		break;
	}
	return 0;
}

int CVICALLBACK
cbBtn2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
	case EVENT_LEFT_CLICK:
		buttoninputbits[kBoardButton2] += cleanpatterna * 4096; // clean pattern is 12 bits
		BIT_SET(buttonstatus, kBoardButton2);
		break;

	case EVENT_COMMIT:
		buttoninputbits[kBoardButton2] += ((cleanpatternb & 0xFFF) * 4096);
		BIT_CLR(buttonstatus, kBoardButton2);
		break;

	default:
		break;
	}
	return 0;
}

int CVICALLBACK
cbBtn3(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
	case EVENT_LEFT_CLICK:
		buttoninputbits[kBoardButton3] += cleanpatterna * 4096; // clean pattern is 12 bits
		BIT_SET(buttonstatus, kBoardButton3);
		break;

	case EVENT_COMMIT:
		buttoninputbits[kBoardButton3] += ((cleanpatternb & 0xFFF) * 4096);
		BIT_CLR(buttonstatus, kBoardButton3);
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
