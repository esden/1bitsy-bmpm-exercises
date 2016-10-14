set history file ~/.gdb_history
set history save on
set target-async on
set confirm off
set mem inaccessible-by-default off
target extended-remote /dev/ttyACM0
monitor version
monitor tpwr enable
monitor jtag_scan
attach 1
