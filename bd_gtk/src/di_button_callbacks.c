/** @file di_button_callbacks.c
 *	@brief	One-sentence short description of file.
 *
 *	Description:
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

//  consecutive 1s: 	    8          7        6       5      4     3   2  1 (meaningless noise to fill 64 bits)
//	noisy input:		1111 1111 0111 1111 0111 1110 1111 1011 1101 1101 1010 0000 0011 1111 0000 0001
#define noisypatterna	0xFF7F7EFBDDA03F01

//	consecutive 0s:          8          7        6        5     4    3    2  1
// noisy input:			1000 0000 0000 1000 0000 1000 0001 0000 0100 0010 0010 01010
//						0x1 0010 1020 844A
#define noisypatternb	0x100101020844A

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

static uint64_t buttoninputbits[kBoardNumButtons]	= {0};
static uint32_t buttonstatus = 0;	// bitmapped image of current button state. 32 bits mostly to avoid compiler warnings.

GObject *btn0		= NULL;
GObject *btn1		= NULL;
GObject *btn2		= NULL;
GObject *btn3		= NULL;
GObject *btn4		= NULL;
GObject *btn5		= NULL;
GObject *btn6		= NULL;
GObject *btn7		= NULL;


// ============================================================================
// ----	Private Functions -----------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Public Functions ------------------------------------------------------
// ============================================================================

void
cbUiButtonPressed(GtkWidget *widget, gpointer data)
{
	uint32_t idx = kBoardButtonNone;
	UNUSED(data);

	// this layer knows about button assignments. for now, only the 1st 4 buttons are assigned.
	//	we're a bit inverted here, in that the GUI is hooked to this layer, but in a real board, the
	//	lower layers know which button is pressed and feed that info back up here.
	// 'twould rather use a switch statement, but can't b/c GCC complains about a pointer not being a scalar type
	if(widget == (GtkWidget *)btn0) { idx = kBoardButton0; } else
	if(widget == (GtkWidget *)btn1) { idx = kBoardButton1; } else
	if(widget == (GtkWidget *)btn2) { idx = kBoardButton2; } else
	if(widget == (GtkWidget *)btn3) { idx = kBoardButton3; } else
	if(widget == (GtkWidget *)btn4) { idx = kBoardButton4; } else
	if(widget == (GtkWidget *)btn5) { idx = kBoardButton5; } else
	if(widget == (GtkWidget *)btn6) { idx = kBoardButton6; } else
	if(widget == (GtkWidget *)btn7) { idx = kBoardButton7; } else
	{
		// empty final else clause
	}

	// toggle back and forth between two noisy sets of inputs.
	// - the 1st is 64 bits of noise, the tail of which is 8 "on" bits (hence a debounced button press).
	// - the 2nd intentionally fails qualification for a button press; remember that this function
	//	"sets" the pressed indicator, we we have to book-end the embedded 0 bits with "1" markers to
	// make the detection algorithms work.

	// call into the next layer down (arch)
	// for exploration, we'll use 8 consecutive bits of the same value to detect a state change
#if 0
	static bool noisypattern = false;	// location here in this function requires C99, prohibits Visual Studio
	buttoninputbits[idx] = noisypattern ? noisypatterna : noisypatternb; noisypattern = !noisypattern;
#else
	if(buttoninputbits[idx])
	{
		buttoninputbits[idx] += cleanpatterna * 4096; // clean pattern is 12 bits
	}
	else
	{
		buttoninputbits[idx] = cleanpatterna;
	}
#endif
	BIT_SET(buttonstatus, idx);
	return;
}

void
cbUiButtonReleased(GtkWidget *widget, gpointer data)
{
	uint32_t idx = 0;
	UNUSED(data);
	// 'twould rather use a switch statement, but can't b/c GCC complains about a pointer not be a scalar type
	if(widget == (GtkWidget *)btn0) { idx = kBoardButton0; } else
	if(widget == (GtkWidget *)btn1) { idx = kBoardButton1; } else
	if(widget == (GtkWidget *)btn2) { idx = kBoardButton2; } else
	if(widget == (GtkWidget *)btn3) { idx = kBoardButton3; } else
	if(widget == (GtkWidget *)btn4) { idx = kBoardButton4; } else
	if(widget == (GtkWidget *)btn5) { idx = kBoardButton5; } else
	if(widget == (GtkWidget *)btn6) { idx = kBoardButton6; } else
	if(widget == (GtkWidget *)btn7) { idx = kBoardButton7; } else
	{
		// empty final else clause
	}

	// call into the next layer down (arch)
#if 0
	static bool noisypattern = false;	// location here in this function requires C99, prohibits Visual Studio
	buttoninputbits[idx] = noisypattern ? noisypatternb : noisypatterna; noisypattern = !noisypattern;
#else
	if(buttoninputbits[idx])
	{
		buttoninputbits[idx] += (uint64_t)((cleanpatternb & 0xFFF) * 4096);
	}
	else
	{
		buttoninputbits[idx] = (uint64_t)((cleanpatternb & 0xFFF));
	}
#endif
	BIT_CLR(buttonstatus, idx);

	/* running commentaire, to be moved to more formal documentation.
	 * - if the DI button SM is in the "released" state, the 1st 1 bit will provoke a transition to
	 *   the debounce-press state; after the bit stream settles down to all 0s, it'll return to the
	 *   released state.
	 */
	return;
}


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
