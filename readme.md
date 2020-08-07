@page board_head Component: CWSW Board Support

# This folder is the root container for Board Support modules.

The main point of this folder, is to connect the MCU-specific possibilities, to the features provided by the board. It should have NO knowledge at all of the intended usage; that would be the purpose of the "bsp" or the application.

_folder structure is slightly different than for other CWSW components - here, the header for a given board is in the "root" of that board's home folder, instead of `inc` folder._

