#!/bin/bash
board=myboard
QEagleScaler scale $board-1x.sch $board-2x.sch 0.5
QEagleScaler scale $board-1x.brd $board-2x.brd 0.5
QEagleScaler libsuffix $board-2x.sch $board-2x.sch '' '.2x'
QEagleScaler libsuffix $board-2x.brd $board-2x.brd '' '.2x'

