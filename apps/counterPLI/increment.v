module main;
reg val;
reg clk;

initial begin
    val = 0;
    clk = 0;
end

always #5 clk = ~clk;

always #10 $increment(val);

initial #100 $finish;

initial begin
    $dumpfile("increment.vcd");
    $dumpvars;
end

endmodule
