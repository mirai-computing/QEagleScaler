#!/bin/bash
board=myboard
QEagleScaler scale $board-2x.sch $board-1x.sch 2
QEagleScaler scale $board-2x.brd $board-1x.brd 2
QEagleScaler libsuffix $board-1x.sch $board-1x.sch '.2x' ''
QEagleScaler libsuffix $board-1x.brd $board-1x.brd '.2x' ''

