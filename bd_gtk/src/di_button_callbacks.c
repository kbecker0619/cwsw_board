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

// ============================================================================
// ----	Type Definitions ------------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Global Variables ------------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Module-level Variables ------------------------------------------------
// ============================================================================

static uint64_t buttoninputbits[8]	= {0};
static uint32_t buttonbitreadidx[8] = {0};	// only need a 5-bit var for this index value, but to represent MVP, use 32-bit size-of-native-register var

GObject *btn0		= NULL;
GObject *btn1		= NULL;
GObject *btn2		= NULL;
GObject *btn3		= NULL;
//GObject *btn4		= NULL;
//GObject *btn5		= NULL;
//GObject *btn6		= NULL;
//GObject *btn7		= NULL;


// ============================================================================
// ----	Private Functions -----------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Public Functions ------------------------------------------------------
// ============================================================================

void
cbUiButtonPressed(GtkWidget *widget, gpointer data)
{
	uint32_t idx = 0;
	UNUSED(data);

	// this layer knows about button assignments. for now, only the 1st 4 buttons are assigned.
	//	we're a bit inverted here, in that the GUI is hooked to this layer, but in a real board, the
	//	lower layers know which button is pressed and feed that info back up here.
	// 'twould rather use a switch statement, but can't b/c GCC complains about a pointer not being a scalar type
	if(widget == (GtkWidget *)btn0) { idx = 0; } else
	if(widget == (GtkWidget *)btn1) { idx = 1; } else
	if(widget == (GtkWidget *)btn2) { idx = 2; } else
//	if(widget == (GtkWidget *)btn3) { idx = 3; } else
//	if(widget == (GtkWidget *)btn4) { idx = 4; } else
//	if(widget == (GtkWidget *)btn5) { idx = 5; } else
//	if(widget == (GtkWidget *)btn6) { idx = 6; } else
//	if(widget == (GtkWidget *)btn7) { idx = 7; } else
	if(widget == (GtkWidget *)btn3)
	{
		Cwsw_EvQX__PostEventId(&tedlos_evqx, evStoplite_ForceYellow);
		return;
	}

	// call into the next layer down (arch)
	// for exploration, we'll use 8 consecutive bits of the same value to detect a state change
	//  consecutive 1s:     8          7        6       5      4     3   2  1
	//	noisy input:	1111 1111 0111 1111 0111 1110 1111 1011 1101 1101 1010 0000 0011 1111 0000 0001
	//					0x‭FF7F7EFBDDA03F01‬
	// create a noisy bit stream and pass it to the DI-level button-reading task.
	buttoninputbits[idx] += 0xFF7F7EFBDDA03F01;
	return;
}

void
cbUiButtonReleased(GtkWidget *widget, gpointer data)
{
	uint32_t idx = 0;
	UNUSED(data);
	// 'twould rather use a switch statement, but can't b/c GCC complains about a pointer not be a scalar type
	if(widget == (GtkWidget *)btn0) { idx = 0; } else
	if(widget == (GtkWidget *)btn1) { idx = 1; } else
	if(widget == (GtkWidget *)btn2) { idx = 2; } else
//	if(widget == (GtkWidget *)btn3) { idx = 3; } else
//	if(widget == (GtkWidget *)btn4) { idx = 4; } else
//	if(widget == (GtkWidget *)btn5) { idx = 5; } else
//	if(widget == (GtkWidget *)btn6) { idx = 6; } else
//	if(widget == (GtkWidget *)btn7) { idx = 7; } else
	if(widget == (GtkWidget *)btn3)
	{
		Cwsw_EvQX__PostEventId(&tedlos_evqx, evStoplite_ForceYellow);
		return;
	}

	// call into the next layer down (arch)
	// for exploration, we'll use 8 consecutive bits of the same value to detect a state change
	//	consecutive 0s:     8          7        6        5     4    3    2  1
	// noisy input:		0111 1111 0000 0011 1111 1010 0
	// noisy input:		0000 0000 1000 0000 1000 0001 0000 0100 0010 0010 01010
	//					0x101020844A
	// create a noisy bit stream and pass it to the DI-level button-reading task.
	buttoninputbits[idx] += 0x101020844A;
	return;
}


bool
di_read_next_button_input_bit(uint8_t idx)
{
	bool retval = ((buttoninputbits[idx] & 1) != 0);
	buttoninputbits[idx] /= 2;
	return retval;
}
