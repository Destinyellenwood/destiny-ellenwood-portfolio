`timescale 1ns / 1ps

module and_gate_tb;

    // Declare testbench signals
    reg a;
    reg b;
    wire out;

    // Instantiate the AND gate
    and_gate uut (
        .a(a),
        .b(b),
        .out(out)
    );

    // Test sequence
    initial begin
        // Display header
        $display("A B | OUT");
        $display("----------");

        // Apply inputs
        a = 0; b = 0; #10; $display("%b %b |  %b", a, b, out);
        a = 0; b = 1; #10; $display("%b %b |  %b", a, b, out);
        a = 1; b = 0; #10; $display("%b %b |  %b", a, b, out);
        a = 1; b = 1; #10; $display("%b %b |  %b", a, b, out);

        $stop;
    end

endmodule
