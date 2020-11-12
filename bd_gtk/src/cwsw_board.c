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

//#include "ManagedAlarms.h"	// temporary until i get architecture sorted out. the BSP should not know about
//#include "tedlosevents.h"
//

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
static GObject *pWindow		= NULL;
static GError *error		= NULL;


// ========================================================================== }
// ----	Private Functions -----------------------------------------------------
// ========================================================================== {
// time handling from demo @ http://zetcode.com/gui/gtk2/gtkevents/
//	this one designed to be called @ 1ms intervals. it is intended to simulate a 1ms heartbeat tic
//	from a real exercise kit.
static gboolean
tmHeartbeat(GtkWidget *widget)
{
	UNUSED(widget);
//	cbHEARTBEAT_ACTION();	<<== defined as `tedlos_schedule(pOsEvqx)`
	return (gboolean)true;
}


static gboolean
gtkidle(gpointer user_data)
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
Cwsw_Board__Init(void)
{
	bool bad_init = false;
	if(!Get(Cwsw_Arch, Initialized))
	{
		return kErr_Lib_NotInitialized;
	}

	// initialize gtk lib. in this environment, no command line options are available.
	gtk_init(&argc, &argv);

	/* Construct a GtkBuilder instance and load our UI description */
	pUiPanel = gtk_builder_new();
	if(gtk_builder_add_from_file(pUiPanel, "../../cwsw_cfg/bsp/gtkboard.ui", &error) == 0)
	{
		g_printerr("Error loading file: %s\n", error->message);
		g_clear_error(&error);
		return kErr_Bsp_InitFailed;
	}

	/* Connect signal handlers to the constructed widgets. */
	// here & below: reaction to bad "connection" call from https://developer.gnome.org/gtk3/stable/GtkWidget.html#gtk-widget-destroy
	pWindow = gtk_builder_get_object (pUiPanel, "GTK_Board");		// run-time association, must match "ID" field.
	if(pWindow)
	{
		extern bool di_button_init(GtkBuilder *pUiPanel);

		// make the "x" in the window upper-right corner close the window
		g_signal_connect(pWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
		btnQuit = gtk_builder_get_object(pUiPanel, "btnQuit");
		if(!btnQuit)	{ bad_init = true; }

		if(!bad_init)		// connect quit, get handles for buttons
		{
			// make the quit button an alias for the "X"
			g_signal_connect(btnQuit, "clicked", G_CALLBACK(gtk_main_quit), NULL);

			bad_init = di_button_init(pUiPanel);
		}

		if(!bad_init)		// set up 1ms heartbeat
		{
			g_timeout_add(1, (GSourceFunc) tmHeartbeat, (gpointer)pWindow);		/* hard-coded 1 ms tic rate */
		}

		// set up idle callback
	    g_idle_add (gtkidle, NULL);

	}

	if(bad_init)
	{
		gtk_widget_destroy((GtkWidget *)pWindow);
		return kErr_Bsp_InitFailed;
	}

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
