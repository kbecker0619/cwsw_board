# GTK Board

This board digitizes a hypothetical "getting started" exercise board w/ 8 input buttons and 8 on/off indicators.

I also produces a 1-ms tic that can by used as the CWSW Lib's timer tic input.

Assignments:

For Stop Lamp:

Indicators:
* Ind 0: "Green" lamp
* Ind 1: "Yellow" lamp
* Ind 2: "Red" lamp
* Ind 7: "Shift" indicator

Buttons:
* Btn 0: "Go" (unpause") (debugging / example timer management)
* Btn 1: "Pause" (debugging / example timer management)
* Btn 2: "Walk" input (shortens duration of green, if currently active, and lengthens duration of red, to maintain overall cycle timing)
* Btn 3: "Force Yellow"
* Btn 7: "Shift" - toggle on/off action. when on, multiple buttons can be pressed at the same time. Upon "release", the chord event will be posted


# misc
C compiler flag: `pkg-config --cflags gtk+-3.0`
