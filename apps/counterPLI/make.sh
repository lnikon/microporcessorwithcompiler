iverilog-vpi increment.c;
iverilog -oincrement.vvp increment.v;
vvp -M. -mincrement increment.vvp;
