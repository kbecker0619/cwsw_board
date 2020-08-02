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

#define  PANEL                           1
#define  PANEL_Green                     2
#define  PANEL_Yellow                    3
#define  PANEL_Red                       4
#define  PANEL_btnYellow                 5       /* callback function: cbBtnYellow */
#define  PANEL_btnWalk                   6       /* callback function: cbBtnGo */
#define  PANEL_btnPause                  7       /* callback function: cbBtnPause */
#define  PANEL_btnGo                     8       /* callback function: cbBtnGo */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK cbBtnGo(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK cbBtnPause(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK cbBtnYellow(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
