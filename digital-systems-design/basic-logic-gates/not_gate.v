`timescale 1ns / 1ps


// not_gate.v
module not_gate(
    input wire a,        // Single input
    output wire out      // Output is the inverse of input
);

    // Combinational logic: NOT gate
    assign out = ~a;

endmodule

