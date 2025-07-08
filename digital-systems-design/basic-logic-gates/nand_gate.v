`timescale 1ns / 1ps


// nand_gate.v
module nand_gate(
    input wire a,       // First input
    input wire b,       // Second input
    output wire out     // Output is inverted AND
);

    // Combinational logic: NAND gate
    assign out = ~(a & b);

endmodule
