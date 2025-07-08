`timescale 1ns / 1ps


// or_gate.v
module or_gate(
    input wire a,       // First input
    input wire b,       // Second input
    output wire out     // Output of OR operation
);

    // Combinational logic: OR gate
    assign out = a | b;

endmodule
