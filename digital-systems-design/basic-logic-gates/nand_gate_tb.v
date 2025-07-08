`timescale 1ns / 1ps

module nand_gate_tb;

    reg a;
    reg b;
    wire out;

    // Instantiate the NAND gate
    nand_gate uut (
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
