/** @file
 *	@brief	Board support for the GTK "board".
 *
 *	In the same way my Microchip MZ demo board, or my NXP 5748G demo board, or my STI demo board, all
 *	have some general purpose buttons, rheostats, LEDs, and displays, so here does the GTK "board" have
 *	a panel that has 8 buttons, 8 "LEDs", and a text box.
 *
 *	\copyright
 *	Copyright (c) 2020 Kevin L. Becker. All rights reserved.
 *
 *	Created on: Jun 26, 2020
 *	@author Kevin L. Becker
 */

// ============================================================================
// ----	Include Files ---------------------------------------------------------
// ========================================================================== {

// ----	System Headers --------------------------
#include <stdbool.h>

// ----	Project Headers -------------------------
#include "cwsw_arch.h"		// Get(Initialized)

// ----	Module Headers --------------------------
#include "cwsw_board.h"

#include "ManagedAlarms.h"	// temporary until i get architecture sorted out. the BSP should not know about
#include "tedlosevents.h"


// ========================================================================== }
// ----	Constants -------------------------------------------------------------
// ========================================================================== {

/* https://developer.gnome.org/gtk-tutorial/stable/c489.html
 * since GTK doesn't expose names that correlate to the events we want to process, use our own.
 * each of these names correlates with an event callback in `gtkbutton.h`
 */
enum sButtonSignals {
	kBtnNoSignal,
	kBtnPressed,
	kBtnReleased,
	kBtnClicked,
	kBtnActivate
};


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

static int    argc = 0;
static char **argv = NULL;

// don't consider this button to belong to the simulated "hardware", but rather, a part of the UI panel that wouldn't be available on HW.
static GObject *btnQuit		= NULL;

static GtkBuilder *pUiPanel	= NULL;
/// One instance of a GTK "Window" for this board, and hence, for this application.
static GObject *pWindow		= NULL;
static GError *error		= NULL;


// ========================================================================== }
// ----	Private Functions -----------------------------------------------------
// ========================================================================== {
// time handling from demo @ http://zetcode.com/gui/gtk2/gtkevents/
//	this one designed to be called @ 1ms intervals. it is intended to simulate a 1ms heartbeat tic
//	from a real exercise kit.
#include "bdsched.h"
#include "tedlos.h"
static gboolean
bspTmrHeartbeat(GtkWidget *widget)
{
	UNUSED(widget);
//	cbHEARTBEAT_ACTION();	<<== defined as `tedlos_schedule(pOsEvqx)`
	tedlos_schedule(pOsEvqx);
	return (gboolean)true;
}


static gboolean
bspTmrIdle(gpointer user_data)
{
	UNUSED(user_data);
//	cdIDLE_ACTION();		<<== tbd
	return (gboolean)true;
}


// ========================================================================== }
// ----	Public Functions ------------------------------------------------------
// ========================================================================== {

// ---- General Functions --------------------------------------------------- {

uint16_t
BSP_Core__Init(void)
{
	if(!Get(Cwsw_Arch, Initialized)) { return kErr_Lib_NotInitialized; }

	// initialize gtk lib.
	//	in this environment, no command line options are available.
	gtk_init(&argc, &argv);

	/* Construct a GtkBuilder instance and load our UI description */
	pUiPanel = gtk_builder_new();

	/* Note: hard-coded location of UI panel. relative to the location of the Eclipse project. */
	if(gtk_builder_add_from_file(pUiPanel, "../../cwsw_cfg/bsp/gtkboard.ui", &error) == 0)
	{
		g_printerr("Error loading file: %s\n", error->message);
		g_clear_error(&error);
		return kErr_Bsp_InitFailed;
	}

	/* Connect signal handlers to the constructed widgets. */
	// here & below: reaction to bad "connection" call from https://developer.gnome.org/gtk3/stable/GtkWidget.html#gtk-widget-destroy
	pWindow = gtk_builder_get_object (pUiPanel, "GTK_Board");		// run-time association, must match "ID" field.
	return (!pWindow);
}

uint16_t
BSP_DI__Init(void)
{
	// note: conceptually, this could be "remote" DI, such as external expansion IC or ASIC core communicating via SPI

	if(!pWindow) { return kErr_Lib_NotInitialized; }

	// make the "x" in the window upper-right corner close the window
	g_signal_connect(pWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	btnQuit = gtk_builder_get_object(pUiPanel, "btnQuit");
	if(btnQuit)
	{
		// make the quit button an alias for the "X"
		g_signal_connect(btnQuit, "clicked", G_CALLBACK(gtk_main_quit), NULL);
		return kErr_Bsp_NoError;
	}

	return kErr_Bsp_InitFailed;
}

uint16_t
BSP_DO__Init(void)
{
	// note: conceptually, this could be "remote" DO, such as external expansion IC or ASIC core communicating via SPI

	// on the GTK board, you don't need the "Quit" button connected in order to do output.
	//	t'uther way to view this is, we allow the DO feature to be initialized before the DI
	//	feature, but there must be a window already (BSP Core must already be initialized).
	if(!pWindow) { return kErr_Lib_NotInitialized; }

	return kErr_Bsp_NoError;
}

uint16_t
BSP_Timers__Init(void)
{
	if(!pWindow) { return kErr_Lib_NotInitialized; }

	// set up 1ms heartbeat
	// in GTK, you only need the return value, if you want to cancel a timer. the ID returned here is used by the cancel function.
	(void)g_timeout_add(1, bspTmrHeartbeat, (gpointer)pWindow);		/* hard-coded 1 ms tic rate */

	// set up idle callback
    (void)g_idle_add(bspTmrIdle, NULL);
	return kErr_Bsp_NoError;
}

uint16_t
BSP_DI_Buttons__Init(ptEvQ_QueueCtrlEx pEvQX)
{
	extern bool di_button_init(GtkBuilder *pUiPanel, ptEvQ_QueueCtrlEx pEvQX);
	if(!btnQuit) { return kErr_Lib_NotInitialized; }
	return di_button_init(pUiPanel, pEvQX);
}

uint16_t
Cwsw_Board__Init(void)
{
	bool bad_init = false;

	if(!bad_init) {		// init timers
	}

	if(bad_init) {		// early exit if failure seen
		gtk_widget_destroy((GtkWidget *)pWindow);
		return kErr_Bsp_InitFailed;
	}

	do {	// init things that "cannot" fail
//		TODO: SET BUTTON QUEUE HERE

		SET(kBoardLed1, kLogicalOff);
		SET(kBoardLed2, kLogicalOff);
		SET(kBoardLed3, kLogicalOff);
		SET(kBoardLed4, kLogicalOff);
	} while(0);

	initialized = true;
	return kErr_Bsp_NoError;
}

bool
Cwsw_Board__Get_Initialized(void)
{
	return initialized;
}

// ---- /General Functions -------------------------------------------------- }

// ---- Common API / Highly Customized -------------------------------------- {

void
Cwsw_Board__Set_kBoardLed1(bool value)
{
	GObject *pind = gtk_builder_get_object(pUiPanel, "ind0");	// run-time association w/ "ID" field in UI
	if(pind)
	{
		gtk_toggle_button_set_active((GtkToggleButton *)pind, value);
	}
}

void
Cwsw_Board__Set_kBoardLed2(bool value)
{
	GObject *pind = gtk_builder_get_object(pUiPanel, "ind1");	// run-time association w/ "ID" field in UI
	if(pind)
	{
		gtk_toggle_button_set_active((GtkToggleButton *)pind, value);
	}
}

void
Cwsw_Board__Set_kBoardLed3(bool value)
{
	GObject *pind = gtk_builder_get_object(pUiPanel, "ind2");	// run-time association w/ "ID" field in UI
	if(pind)
	{
		gtk_toggle_button_set_active((GtkToggleButton *)pind, value);
	}
}

void
Cwsw_Board__Set_kBoardLed4(bool value)
{
	GObject *pind = gtk_builder_get_object(pUiPanel, "ind3");	// run-time association w/ "ID" field in UI
	if(pind)
	{
		gtk_toggle_button_set_active((GtkToggleButton *)pind, value);
	}
}

// ---- /Common API / Highly Customized ------------------------------------- }

// ========================================================================== }
