`timescale 1ns / 1ps

module or_gate_tb;

    // Testbench signals
    reg a;
    reg b;
    wire out;

    // Instantiate the OR gate
    or_gate uut (
        .a(a),
        .b(b),
        .out(out)
    );

    initial begin
        $display("A B | OUT");
        $display("----------");

        a = 0; b = 0; #10; $display("%b %b |  %b", a, b, out);
        a = 0; b = 1; #10; $display("%b %b |  %b", a, b, out);
        a = 1; b = 0; #10; $display("%b %b |  %b", a, b, out);
        a = 1; b = 1; #10; $display("%b %b |  %b", a, b, out);

        $stop;
    end

endmodule
