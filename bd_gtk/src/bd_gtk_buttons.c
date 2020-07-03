/** @file
 *	@brief	Board-specific button-reading functions.
 *
 *	Copyright (c) 2020 Kevin L. Becker. All rights reserved.
 *
 *	Original:
 *	Created on: Jun 28, 2020
 *	Author: kevin
 */

// ============================================================================
// ----	Include Files ---------------------------------------------------------
// ============================================================================

// ----	System Headers --------------------------

// ----	Project Headers -------------------------
#include "tedlos.h"				/* event queue used by this SME */
#include "cwsw_sme.h"

// ----	Module Headers --------------------------
#include "cwsw_board.h"


// ============================================================================
// ----	Constants -------------------------------------------------------------
// ============================================================================

/// States for this State Machine.
typedef enum eSmStates {
	kStateStart,
	kStateReleased,
	kStateDebouncePress,
	kStatePressed,
	kStateDebounceRelease
} tSmStates;

/// Stuck button timeout value.
enum { kButtonStuckTimeoutValue = tmr1000ms * 30 };

/// Button identifiers.
enum { kButtonNone, kButton0, kButton1, };


// ============================================================================
// ----	Type Definitions ------------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Global Variables ------------------------------------------------------
// ============================================================================

tCwswSwAlarm	Btn_tmr_ButtonRead = {
	/* .tm			= */tmr10ms,
	/* .reloadtm	= */tmr10ms,
	/* .pEvQX		= */&tedlos_evqx,
	/* .evid		= */evButton_Task,
	/* .tmrstate	= */kTmrState_Enabled
};


// ============================================================================
// ----	Module-level Variables ------------------------------------------------
// ============================================================================

static ptEvQ_QueueCtrlEx pBtnEvqx = NULL;

// this next item is a standard part of the template. it is not normally something you need to update.
static const ptCwswSwAlarm pMyTimer = &Btn_tmr_ButtonRead;


// ============================================================================
// ----	State Functions -------------------------------------------------------
// ============================================================================

static tStateReturnCodes
StateStart(ptEvQ_Event pev, uint32_t *pextra)
{
	static tStateReturnCodes statephase = kStateUninit;
	static tEvQ_EventID evId;

	switch(statephase)
	{
	case kStateUninit:	/* on 1st entry, execute on-entry action */
	case kStateAbort:	/* upon return to this state after previous normal exit, execute on-entry action */
	default:			/* for any unexpected value, restart this state. */
		// generic state management, common to all states
		statephase = kStateNormal;
		evId = pev->evId;			// save default exit reason 1.

		// ---- state-specific behavior ---------
		// no state-specific behavior for this state
		break;

	case kStateNormal:
		++statephase;	// we'll basically leave as soon as we start.
		break;

	case kStateExit:
		// manage the state machine: set exit reasons
		pev->evId = evId;	// exit reason 1: event that provoked the exit.
		pev->evData = 0;	// exit reason 2
		*pextra = 0;		// exit reason 3 (for debugging use in transition)
		++statephase;		// set conditions for correct handling upon reentry

		// state-specific behavior
		/* (no state-specific exit actions here) */
		break;
	}

	// the next line is part of the template and should not be touched.
	return statephase;
}

extern bool di_read_next_button_input_bit(uint8_t idx);
static tStateReturnCodes
StateReleased(ptEvQ_Event pev, uint32_t *pextra)
{
	static tStateReturnCodes statephase = kStateUninit;
	static tEvQ_EventID evId;
	static uint32_t evData = 0;

	switch(statephase++)
	{
	case kStateUninit:	/* on 1st entry, execute on-entry action */
	case kStateAbort:	/* upon return to this state after previous normal exit, execute on-entry action */
	default:			/* for any unexpected value, restart this state. */
		// generic state management, common to all states
		evId = pev->evId;			// save default exit reason 1.

		// ---- state-specific behavior ---------
		// no state-specific behavior for this state
		break;

	case kStateNormal:
		do {
			bool nextbit = false;
			nextbit = di_read_next_button_input_bit(0);
			if(!nextbit)
			{
				// stay in this state until we see a twitch on one of the button inputs.
				//	note: in this iteration of this implementation, we're only reading "button" 0
				--statephase;
			}
			else
			{
				evData = kButton0;	// todo: identify which button.
			}
		} while(0);
		break;

	case kStateExit:
		// manage the state machine: set exit reasons
		pev->evId = evId;			// exit reason 1: event that provoked the exit.
		pev->evData = evData;		// exit reason 2: non-zero bit read
		*pextra = kStateReleased;	// exit reason 3:

		// state-specific behavior
		/* (no state-specific exit actions here) */
		break;
	}

	// the next line is part of the template and should not be touched.
	return statephase;
}

static tStateReturnCodes
StateDebouncePress(ptEvQ_Event pev, uint32_t *pextra)
{
	static tStateReturnCodes statephase = kStateUninit;
	static tCwswClockTics tmrMyStateTimer = 0;	// debounce time of 100 ms. if expired, mark as stuck button
	static tEvQ_EventID evId;
	static uint32_t reason2 = 0;
	static uint32_t reason3 = 0;
	static uint8_t read_bits;
	switch(statephase++)
	{
	case kStateUninit: /* on 1st entry, execute on-entry action */
	case kStateAbort: /* upon return to this state after previous normal exit, execute on-entry action */
	default: /* for any unexpected value, restart this state. */
		evId = pev->evId;	// save exit Reason1
		reason3 = kButtonNone;

		/* for this task, we assume the transition was provoked by a non-zero bit on the most recent
		 * DI bit read. seed our debounce var with that 1st bit.
		 */
		read_bits = 1;

		// start my state timer. remember, our call rate is 10 ms. 100ms == 10 bit readings, 640ms is 64 bit reads
		Set(Cwsw_Clock, tmrMyStateTimer, tmr500ms + tmr250ms);
		Set(Cwsw_Clock, tmrMyStateTimer, tmr500ms + tmr100ms + tmr25ms);	// just shy of 64 bits' worth of time
		break;

	case kStateNormal:
		// read next bit
		read_bits <<= 1;		// shift current bits left one position
		read_bits = (uint8_t)(read_bits | di_read_next_button_input_bit(0));
		if(read_bits == 0)
		{
			// debounce done, wasn't a qualified button press, return to released state
			reason2 = kButtonNone;
			reason3 = 0;
		}
		else if(read_bits == 0xFF)
		{
			// debounce done, recognized as button press, advance to next state
			reason2 = kButton0;	// todo: identify which button.
			reason3 = 1;
		}
		else if(TM(tmrMyStateTimer))
		{
			reason2 = 0;
			reason3 = 2;
		}
		else
		{
			--statephase;		// nothing of note happened, stay in this state
		}
		break;

	case kStateExit:
		// for this edition of this state, no state-specific exit action is required.
		//	let the caller (normally the SME) know what event and what guard provoked the change.
		pev->evId = evId;		// save exit reason 1 (event that provoked the exit)
		pev->evData = reason2;	// save exit reason 2 (button recognized)
		*pextra = reason3;		// save exit reason 3 (reason for exit (no button, button, timeout)
		break;
	}

	// the next line is part of the template and should not be touched.
	return statephase;
}

static tStateReturnCodes
StatePressed(ptEvQ_Event pev, uint32_t *pextra)
{
	UNUSED(pev);
	UNUSED(pextra);
	// todo: confirm we posted the correct event in the transition function leading into this state
	return 0;
}

// ============================================================================
// ----	Transition Functions --------------------------------------------------
// ============================================================================

// transitions go here
/** Transition Function.
 * 	This function notifies the world of a state change in our button-reading SM.
 * 	Not sure it really matters to our design, if we post this event in the exit function vs. the
 * 	transition; philosophically, it makes sense for the state to notify the world of its own status
 * 	change, but actually (as realized in this implementation) it does not affect the timing of the
 * 	event posting, in that both the exit action and the transition function are executed in the same
 * 	instance of the SME. the one (small) factor that tilts me towards the transition function as the
 * 	poster of note, is that philosophically, the exit action is supposed to be atomic and robust (as
 * 	in, cannot fail), but in our system, the posting of an event can fail.
 *
 * 	we rely on the state providing the event detail to be posted.
 */
static void
NotifyBtnStateChg(tEvQ_Event ev, uint32_t extra)
{
	UNUSED(extra);
	switch(ev.evData)
	{
	case 0:		// state change to Released state
		(void)Cwsw_EvQX__PostEvent(pBtnEvqx, ev);
		break;

	case 1:		// state change to Pressed state
		ev.evId = evButton_BtnPressed;
		(void)Cwsw_EvQX__PostEvent(pBtnEvqx, ev);
		break;

	default:
		break;
	}
}

static void
NullTransition(tEvQ_Event ev, uint32_t extra)
{
	printf("Transition: ev: %i, Transition ID: %i\n", ev.evId, extra);
	UNUSED(extra);
}

/* the button reads use the following state machine:
 * start -> released -> debounce-press -> pressed -> debounce-release
 * 				^				|			|               |
 * 				+-------- timeout (stuck)  -/               |
 * 				\-------------------------------------------/ (unimportant if debounce achieved, or if timeout)
 */
static tTransitionTable tblTransitions[] = {
	// current				Reason1			Reason2				  Reason3	Next State			Transition Func
	{ StateStart,			evButton_Task,	0,						0,		StateReleased,		NullTransition		},	// normal termination
	{ StateReleased,		evButton_Task,	kButton0,				1,		StateDebouncePress,	NullTransition		},	// normal termination: non-0 bit seen @ button
	{ StateReleased,		evButton_Task,	kButton1,				1,		StateDebouncePress,	NullTransition		},	// normal termination: non-0 bit seen @ button

	{ StateDebouncePress,	evButton_Task,	kButtonNone,			0,		StateReleased,		NotifyBtnStateChg	},	// debounced input is 0
	{ StateDebouncePress,	evButton_Task,	kButton0,				1,		StatePressed,		NotifyBtnStateChg	},	// normal termination (debounced input is 0xFF)
	{ StateDebouncePress,	evButton_Task,	0,						2,		StateReleased,		NullTransition		},	// debounce timeout
};


// ============================================================================
// ----	Public Functions ------------------------------------------------------
// ============================================================================

void
Btn_tsk_ButtonRead(tEvQ_Event ev, uint32_t extra)	// uses DI lower layers
{
	static pfStateHandler currentstate = NULL, nextstate = NULL;
	tStateReturnCodes rc = kStateUninit;

	if(!currentstate)	{ currentstate = StateStart; }
	if(currentstate) 	{ rc = currentstate(&ev, &extra); }

	if(rc > kStateExit)
	{
		/* the SME needs to know which state to advance to, given the current state.
		 * in order for it to know that, it has to know which event & guard combination led to the end
		 * of the current state.
		 */
		nextstate = Cwsw_Sme_FindNextState(
				tblTransitions, TABLE_SIZE(tblTransitions),
				currentstate, ev, extra);
		if(nextstate)
		{
			currentstate = nextstate;
		}
		else
		{
			// disable alarm that launches this SME via its event.
			//	if restarted, we'll resume in the current state
			//	need a way to restart w/ the init state.
			pMyTimer->tmrstate = kTmrState_Disabled;
		}
	}

}

void
Btn_SetQueue(ptEvQ_QueueCtrlEx pEvqx)
{
	pBtnEvqx = pEvqx;
}
