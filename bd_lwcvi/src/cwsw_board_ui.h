/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2020. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
	extern "C" {
#endif

	/* Panels and Controls: */

#define  PANEL                           1       /* callback function: cbPanel */
#define  PANEL_Green                     2
#define  PANEL_Yellow                    3
#define  PANEL_Red                       4
#define  PANEL_btn3                 5       /* callback function: cbBtnYellow */
#define  PANEL_btn2                   6       /* callback function: cbBtnWalk */
#define  PANEL_btn1                  7       /* callback function: cbBtnPause */
#define  PANEL_btnGo                     8       /* callback function: cbBtnGo */
#define  PANEL_TIMER                     9       /* callback function: tmHeartbeat */
#define  PANEL_Walk                      10


	/* Menu Bars, Menus, and Menu Items: */

		 /* (no menu bars in the resource file) */


	/* Callback Prototypes: */

int  CVICALLBACK cbBtn0(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK cbBtn1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK cbBtn2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK cbBtn3(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK cbPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK tmHeartbeat(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
	}
#endif
