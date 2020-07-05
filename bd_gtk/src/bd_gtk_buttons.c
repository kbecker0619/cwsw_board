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

/// Debounce time
#if 0
// this timeout value is long enough to read 64 bits of input stream, w/ ~100+ ms margin
enum { kTmButtonDebounceTime = tmr500ms + tmr250ms };
#else
		/* this timeout is just "comfortably" shy of enough time to read the full defined input
		 *	value of "noisy" input bits. the expected behavior then, is that this should transition
		 *	back to the released state, and immediately transition back here because the input stream
		 *	will have a few more "1" bits left in it. this 2nd invocation will result in a "solid"
		 *	debounced "press" reading.
		 * we know that this will take more overall time but for demonstration purposes will have
		 *	the same end result. timing: 1 cycle for our exit action, 1 cycle for "released"'s
		 *	entry action, 1 cycle for "released" to read a "1" bit, 1 cycle for "released"'s exit
		 *	action, 1 cycle for our entry action, and then we can begin accumulating debouncing bits.
		 */
enum { kTmButtonDebounceTime = tmr500ms + tmr100ms };
#endif

/// Button identifiers.
enum { kButtonNone, kButton0, kButton1, };

/// "Reason3" reasons for exiting a state.
enum { kReasonNone, kReasonDebounced, kReasonTimeout, kReasonButtonUnstuck };


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

extern bool di_read_next_button_input_bit(uint8_t idx);

static tStateReturnCodes
stDebounceButton(ptEvQ_Event pev, uint32_t *pextra)
{
	static tStateReturnCodes statephase = kStateUninit;
	static tCwswClockTics tmrMyStateTimer = 0;	// debounce time of 100 ms. if expired, mark as stuck button
	static tEvQ_EventID evId;
	static uint32_t reason2 = 0;
	static uint32_t reason3 = kReasonNone;
	static uint8_t read_bits;
	switch(statephase++)
	{
	case kStateUninit: /* on 1st entry, execute on-entry action */
	case kStateAbort: /* upon return to this state after previous normal exit, execute on-entry action */
	default: /* for any unexpected value, restart this state. */
		evId = pev->evId;			// save exit Reason1
		reason3 = kReasonDebounced;	// save default reason3
		statephase = kStateNormal;	// reinitialize state's phase marker unilaterally

		/* for this task, we assume the transition was provoked by a non-zero bit on the most recent
		 * DI bit read. seed our debounce var with that 1st bit.
		 */
		read_bits = 1;

		// start my state timer. remember, our call rate is 10 ms. 100ms == 10 bit readings, 640ms is 64 bit reads
		Set(Cwsw_Clock, tmrMyStateTimer, kTmButtonDebounceTime);
		break;

	case kStateNormal:
		// read next bit
		read_bits *= 2;			// shift current bits left one position
		read_bits = (uint8_t)(read_bits | di_read_next_button_input_bit(0));
		if(read_bits == 0)
		{
			// debounce done, recognized as an open (released) button
			evId = evButton_BtnReleased;
			reason2 = kButton0;	// todo: identify which button.
			reason3 = kReasonDebounced;
		}
		else if(read_bits == 0xFF)
		{
			// debounce done, recognized as button press, advance to next state
			evId = evButton_BtnPressed;
			reason2 = kButton0;	// todo: identify which button.
			reason3 = kReasonDebounced;
		}
		else if(TM(tmrMyStateTimer))
		{
			reason2 = 0;
			reason3 = kReasonTimeout;
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
stStart(ptEvQ_Event pev, uint32_t *pextra)
{
	static tStateReturnCodes statephase = kStateUninit;
	static tEvQ_EventID evId;

	switch(statephase++)
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
		// we'll basically leave as soon as we start.
		break;

	case kStateExit:
		// manage the state machine: set exit reasons
		pev->evId = evId;		// exit reason 1: event that provoked the exit.
		pev->evData = 0;		// exit reason 2
		*pextra = kReasonNone;	// exit reason 3 (for debugging use in transition)

		// state-specific behavior
		/* (no state-specific exit actions here) */
		break;
	}

	// the next line is part of the template and should not be touched.
	return statephase;
}


/**	Implement the Button Released state of the SM.
 *	@param idx
 *	@return
 *	@note If return here is due to a stuck-button event, this state will continue to read a stream
 *		  of "1" bits from the DI layer. In that case, there will be a continue cycle between this
 *		  state, debounce-press, and pressed, then back here. \n
 *		  The default SM has a special "stuck" state, which is intended to eliminate this cycle, but
 *		  we wanted to describe the behavior if something should go wrong.
 *	@{
 */
static tStateReturnCodes
stButtonReleased(ptEvQ_Event pev, uint32_t *pextra)
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
		statephase = kStateNormal;	// reinitialize state's phase marker unilaterally
		evId = pev->evId;			// save default exit reason 1.

		// ---- state-specific behavior ---------
		// no state-specific behavior for this state
		break;

	case kStateNormal:
		if(!di_read_next_button_input_bit(0))
		{
			// stay in this state until we see a twitch on one of the button inputs.
			//	note: in this iteration of this implementation, we're only reading "button" 0
			--statephase;
		}
		else
		{
			evData = kButton0;	// todo: identify which button.
		}
		break;

	case kStateExit:
		// manage the state machine: set exit reasons
		pev->evId = evId;		// exit reason 1: event that provoked the exit.
		pev->evData = evData;	// exit reason 2: non-zero bit read
		*pextra = kReasonNone;	// exit reason 3: legacy assignment (before creation of enum reason)

		// state-specific behavior
		/* (no state-specific exit actions here) */
		break;
	}

	// the next line is part of the template and should not be touched.
	return statephase;
}
/** @} */

static tStateReturnCodes
stDebouncePress(ptEvQ_Event pev, uint32_t *pextra)
{
	return stDebounceButton(pev, pextra);
}

#if 1
static tStateReturnCodes
stButtonPressed(ptEvQ_Event pev, uint32_t *pextra)
{
	static tStateReturnCodes statephase = kStateUninit;
	static tCwswClockTics tmrPressedStateTimer = 0;
	static tEvQ_EventID evId;
	static uint32_t reason2 = 0;
	static uint32_t reason3 = kReasonNone;

	switch(statephase++)
	{
	case kStateUninit:
	case kStateAbort:
	default:
		evId = pev->evId;			// save exit Reason1
		reason3 = kReasonNone;		// save default exit Reason3
		statephase = kStateNormal;	// reinitialize state's phase marker unilaterally

		/* for this task, we stay here as long as the button remains pressed, or until the timeout
		 * period expires. a "release" is seen as a zero bit on the bit input stream.
		 */

		Set(Cwsw_Clock, tmrPressedStateTimer, kButtonStuckTimeoutValue);
		break;

	case kStateNormal:
		if(!di_read_next_button_input_bit(0))
		{
			// button might have been released, go to debounce-release state to confirm
			reason2 = kButton0;		// todo: identify which button.
			reason3 = kReasonNone;
		}
		else if(TM(tmrPressedStateTimer))
		{
			// we've been too long in the pressed-button state, there might be a stuck button
			reason2 = 0;
			reason3 = kReasonTimeout;
		}
		else
		{
			--statephase;			// nothing of note happened, stay in this state
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
#endif

#if 1
static tStateReturnCodes
stDebounceRelease(ptEvQ_Event pev, uint32_t *pextra)
{
	return stDebounceButton(pev, pextra);
}
#endif

#if 1
static tStateReturnCodes
stButtonStuck(ptEvQ_Event pev, uint32_t *pextra)
{
	static tStateReturnCodes statephase = kStateUninit;
//	static tCwswClockTics tmrMyStateTimer = 0;
	static tEvQ_EventID evId;

	switch(statephase++)
	{
	case kStateUninit:	/* on 1st entry, execute on-entry action */
	case kStateAbort:	/* upon return to this state after previous normal exit, execute on-entry action */
	default:			/* for any unexpected value, restart this state. */
		evId = pev->evId;	// save exit Reason1
//		Set(Cwsw_Clock, tmrMyStateTimer, tmr1000ms);
		break;

	case kStateNormal:
		if(di_read_next_button_input_bit(0))
		{
			// stay in this state as long as we read a "1" bit
			--statephase;
		}
		break;

	case kStateExit:
		// for this edition of this state, no state-specific exit action is required.
		//	let the caller (normally the SME) know what event and what guard provoked the change.
		pev->evId = evId;	// save exit reason 1 (event that provoked the exit)
		// there's one and only one reason we leave this state, no reason for supplying reasons.
		//	However, to allow the transition action to make an informed decision, indicate a unique
		//	exit code.
		pev->evData = 0;
		*pextra = kReasonButtonUnstuck;
		break;
	}

	// the next line is part of the template and should not be touched.
	return statephase;
}
#endif

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
	// if we arrive here w/ extra == 3, it's because the stuck state has left since it read a none-1
	//	bit. post an announcement event
	switch(extra)
	{
	case kReasonDebounced:		// but change of state; evId contains correct event, evData contains which button is affected.
		switch(ev.evData)
		{
		case 0:
			cwsw_assert(ev.evId == evButton_BtnPressed, "Button release event expected");
			break;

		case 1:		// state change to Pressed state
			cwsw_assert(ev.evId == evButton_BtnPressed, "Button press event expected");
			break;

		default:
			ev.evId = 0;
			break;
		}
		break;

	case kReasonTimeout:
		ev.evId = evButton_BtnStuck;
		(void)Cwsw_EvQX__PostEvent(pBtnEvqx, ev);
		break;

	case kReasonButtonUnstuck:
		ev.evId = evButton_BtnUnstuck;
		(void)Cwsw_EvQX__PostEvent(pBtnEvqx, ev);
		break;

	default:
		break;
	}
	if(ev.evId)
	{
		(void)Cwsw_EvQX__PostEvent(pBtnEvqx, ev);
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
 *
 *	these transitions are in the same order as listed in the design document.
 */
static tTransitionTable tblTransitions[] = {
	// current				Reason1					Reason2			Reason3					Next State			Transition Func
	{ stStart,				evButton_Task,				0,			kReasonNone,			stButtonReleased,	NullTransition		},	// normal termination

	{ stButtonReleased,		evButton_Task,			kButton0,		kReasonNone,			stDebouncePress,	NullTransition		},	// normal termination: non-0 bit seen @ button
//	{ stButtonReleased,		evButton_Task,			kButton1,		kReasonNone,			stDebouncePress,	NullTransition		},	// normal termination: non-0 bit seen @ button
	{ stDebouncePress,		evButton_BtnPressed,	kButton0,		kReasonDebounced,		stButtonPressed,	NotifyBtnStateChg	},	// normal termination (debounced input is 0xFF)
	{ stButtonPressed,		evButton_Task,			kButton0,		kReasonNone,			stDebounceRelease,	NullTransition		},
	{ stDebounceRelease,	evButton_BtnReleased,	kButton0,		kReasonDebounced,		stButtonReleased,	NotifyBtnStateChg	},

	{ stDebouncePress,		evButton_Task,				0,			kReasonTimeout,			stButtonReleased,	NullTransition		},	// debounce timeout
	{ stDebouncePress,		evButton_Task,			kButtonNone,	kReasonNone,			stButtonReleased,	NullTransition		},	// debounced input is 0. no need to post event, since debounced state hasn't changed.

	{ stButtonPressed,		evButton_Task,				0,			kReasonTimeout,			stButtonStuck,		NotifyBtnStateChg	},	// button stuck, go directly back to "stuck" state

//	{ stButtonPressed,			evButton_Task,			kButton0,		kReasonDebounced,		stDebounceRelease, NullTransition	},	// detected a possible button release, go to debounce state

//	{ stButtonStuck,		evButton_Task,				0,			kReasonButtonUnstuck,	stButtonReleased,		NotifyBtnStateChg	},

};


// ============================================================================
// ----	Public Functions ------------------------------------------------------
// ============================================================================

void
Btn_tsk_ButtonRead(tEvQ_Event ev, uint32_t extra)	// uses DI lower layers
{
	static pfStateHandler currentstate = NULL, nextstate = NULL;
	tStateReturnCodes rc = kStateUninit;

	if(!currentstate)	{ currentstate = stStart; }
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
