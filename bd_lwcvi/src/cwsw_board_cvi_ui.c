/* This is intended to be the file that is created by the LabWindows/CVI UI editor.
 * It is not intended to be used directly in the demonstration project, as it is an architectural
 * misfit. Instead, copy or move the contents of this generated file to the modules appropriate to
 * the architecture.
 */
#include <cvirte.h>
#include <userint.h>
#include "board.h"

static int panelHandle;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "board.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

