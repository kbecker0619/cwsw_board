/** @file bd_gtk.c
 *	@brief	One-sentence short description of file.
 *
 *	Description:
 *
 *	Copyright (c) 2020 Kevin L. Becker. All rights reserved.
 *
 *	Original:
 *	Created on: Jun 26, 2020
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

// declare sans header the simulated "hardware" buttons belonging to the UI panel.
extern GObject *btn0;
extern GObject *btn1;
extern GObject *btn2;
extern GObject *btn3;
extern GObject *btn4;
extern GObject *btn5;
extern GObject *btn6;
extern GObject *btn7;

static bool initialized = false;

static int    argc = 0;
static char **argv = NULL;

// don't consider this button to belong to the simulated "hardware", but rather, a part of the UI panel that wouldn't be available on HW.
static GObject *btnQuit		= NULL;

static GtkBuilder *pUiPanel	= NULL;
static GObject *pWindow		= NULL;
static GError *error		= NULL;

static ptEvQ_QueueCtrlEx pOsEvqx = NULL;


// ========================================================================== }
// ----	Private Functions -----------------------------------------------------
// ========================================================================== {
/* i know very well that this is more complicated than it need be, however, this UI is there to use
 * and show the CWSW BSP, and as such, i need to translate GTK events into things the BSP under-
 * stands.
 *
 * for simpleness, each event and each button has its own callback, which then set flags as
 * appropriate for the CWSW code to process.
 */

/* on a physical board, my DI task will read all inputs at once, and will iterate through the
 * handlers for the individual assignments. to kinda-sorta replicate that behavior, we'll have one
 * button handler, and will detect which button was pressed by the widget address.
 *
 * note that on a real board, the DI action would be descended from a task, not a dispatched event
 * from the GUI framework.
 */
static void
cbButtonClicked(GtkWidget *widget, gpointer data)
{
	UNUSED(widget);
	UNUSED(data);
	return;
	// no recognized objects, just leave
}


// time handling from demo @ http://zetcode.com/gui/gtk2/gtkevents/
//	this one designed to be called @ 1ms intervals. it is intended to simulate a 1ms heartbeat tic
//	from a real exercise kit.
static gboolean
tmHeartbeat(GtkWidget *widget)
{
	UNUSED(widget);
	tedlos_schedule(pOsEvqx);
	return true;	// false
}


// ========================================================================== }
// ----	Public Functions ------------------------------------------------------
// ========================================================================== {

/* no-meaning demo function from gtk site
 */
void
create_radio_buttons (void) {

   GtkWidget *window, *radio1, *radio2, *box, *entry;
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
   gtk_box_set_homogeneous (GTK_BOX (box), TRUE);

   // Create a radio button with a GtkEntry widget
   radio1 = gtk_radio_button_new (NULL);
   entry = gtk_entry_new ();
   gtk_container_add (GTK_CONTAINER (radio1), entry);


   // Create a radio button with a label
   radio2 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1),
                                                         "I’m the second radio button.");

   // Pack them into a box, then show all the widgets
   gtk_box_pack_start (GTK_BOX (box), radio1, true, true, 10);
   gtk_box_pack_start (GTK_BOX (box), radio2, true, true, 10);
   gtk_container_add (GTK_CONTAINER (window), box);
   gtk_widget_show_all (window);
   return;
}


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
	if(gtk_builder_add_from_file(pUiPanel, "../app/board.ui", &error) == 0)
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
		extern void cbUiButtonPressed(GtkWidget *widget, gpointer data);
		extern void cbUiButtonReleased(GtkWidget *widget, gpointer data);

		// make the "x" in the window upper-right corner close the window
		g_signal_connect(pWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
		btnQuit = gtk_builder_get_object(pUiPanel, "btnQuit");
		if(!btnQuit)	{ bad_init = true; }

		if(!bad_init)		// connect quite, get handle for button 0
		{
			// make the quit button an alias for the "X"
			g_signal_connect(btnQuit, "clicked", G_CALLBACK(gtk_main_quit), NULL);

			/* we want button-press and button-release events. for convenience and exploration, we'll also
			 * capture the click event.
			 */
			btn0 = gtk_builder_get_object(pUiPanel, "btn0");// run-time association w/ "ID" field in UI
			if(!btn0)	{ bad_init = true; }
		}

		if(!bad_init)		// connect btn0, get handle for button 1
		{
			g_signal_connect(btn0, "clicked",	G_CALLBACK(cbButtonClicked), NULL);
			g_signal_connect(btn0, "pressed",	G_CALLBACK(cbUiButtonPressed), NULL);
			g_signal_connect(btn0, "released",	G_CALLBACK(cbUiButtonReleased), NULL);

			btn1 = gtk_builder_get_object(pUiPanel, "btn1");
			if(!btn1)	{ bad_init = true; }
		}

		if(!bad_init)		// connect btn1, get handle for button 2
		{
			g_signal_connect(btn1, "clicked", G_CALLBACK(cbButtonClicked), NULL);
			g_signal_connect(btn1, "clicked", G_CALLBACK(cbUiButtonPressed), NULL);
			g_signal_connect(btn1, "clicked", G_CALLBACK(cbUiButtonReleased), NULL);

			btn2 = gtk_builder_get_object(pUiPanel, "btn2");
			if(!btn2)	{ bad_init = true; }
		}

		if(!bad_init)		// connect btn2, get handle for button 3
		{
			g_signal_connect(btn2, "clicked", G_CALLBACK(cbButtonClicked), NULL);
			g_signal_connect(btn2, "clicked", G_CALLBACK(cbUiButtonPressed), NULL);
			g_signal_connect(btn2, "clicked", G_CALLBACK(cbUiButtonReleased), NULL);

			btn3 = gtk_builder_get_object(pUiPanel, "btn3");
			if(!btn3)	{ bad_init = true; }
		}

		if(!bad_init)		// connect btn3, get handle for button 4
		{
			g_signal_connect(btn3, "clicked", G_CALLBACK(cbButtonClicked), NULL);
			g_signal_connect(btn3, "clicked", G_CALLBACK(cbUiButtonPressed), NULL);
			g_signal_connect(btn3, "clicked", G_CALLBACK(cbUiButtonReleased), NULL);

			btn4 = gtk_builder_get_object(pUiPanel, "btn4");
			if(!btn4)	{ bad_init = true; }
		}

		if(!bad_init)		// connect btn4, get handle for button 5
		{
			g_signal_connect(btn4, "clicked", G_CALLBACK(cbButtonClicked), NULL);
			g_signal_connect(btn4, "clicked", G_CALLBACK(cbUiButtonPressed), NULL);
			g_signal_connect(btn4, "clicked", G_CALLBACK(cbUiButtonReleased), NULL);

			btn5 = gtk_builder_get_object(pUiPanel, "btn5");
			if(!btn5)	{ bad_init = true; }
		}

		if(!bad_init)		// connect btn5, get handle for button 6
		{
			g_signal_connect(btn5, "clicked", G_CALLBACK(cbButtonClicked), NULL);
			g_signal_connect(btn5, "clicked", G_CALLBACK(cbUiButtonPressed), NULL);
			g_signal_connect(btn5, "clicked", G_CALLBACK(cbUiButtonReleased), NULL);

			btn6 = gtk_builder_get_object(pUiPanel, "btn6");
			if(!btn6)	{ bad_init = true; }
		}

		if(!bad_init)		// connect btn6, get handle for button 7
		{
			g_signal_connect(btn6, "clicked", G_CALLBACK(cbButtonClicked), NULL);
			g_signal_connect(btn6, "clicked", G_CALLBACK(cbUiButtonPressed), NULL);
			g_signal_connect(btn6, "clicked", G_CALLBACK(cbUiButtonReleased), NULL);

			btn7 = gtk_builder_get_object(pUiPanel, "btn7");
			if(!btn7)	{ bad_init = true; }
		}

		if(!bad_init)		// set up 1ms heartbeat
		{
			g_timeout_add(tmr1ms, (GSourceFunc) tmHeartbeat, (gpointer)pWindow);
		}
	}

	if(bad_init)
	{
		gtk_widget_destroy((GtkWidget *)pWindow);
		return kErr_Bsp_InitFailed;
	}

	create_radio_buttons();

	SET(kBoardLed1, kLogicalOff);
//	SET(kBoardLed2, kLogicalOff);
//	SET(kBoardLed3, kLogicalOff);

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
	gtk_main();
}

// ---- /General Functions -------------------------------------------------- }

// ---- Common API / Highly Customized -------------------------------------- {


void
Cwsw_Board__Set_kBoardLed1(bool value)
{
	GObject *pind = gtk_builder_get_object(pUiPanel, "ind0");	// run-time association w/ "ID" field in UI
	if(pind)	{ gtk_check_menu_item_set_active((GtkCheckMenuItem *)pind, value); }
	pind = gtk_builder_get_object(pUiPanel, "rb0");				// run-time association w/ "ID" field in UI
	if(pind)	{ gtk_check_menu_item_set_active((GtkCheckMenuItem *)pind, value); }
}

// ---- /Common API / Highly Customized ------------------------------------- }

// ========================================================================== }
