`timescale 1ns / 1ps

module not_gate_tb;

    reg a;
    wire out;

    // Instantiate the NOT gate
    not_gate uut (
        .a(a),
        .out(out)
    );

    initial begin
        $display("A | OUT");
        $display("--------");

        a = 0; #10; $display("%b |  %b", a, out);
        a = 1; #10; $display("%b |  %b", a, out);

        $stop;
    end

endmodule
