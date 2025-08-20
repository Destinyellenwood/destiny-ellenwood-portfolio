`timescale 1ns / 1ps
// Three-Bit Binary Adder (structural version)
// Destiny Ellenwood

module three_bit_adder(
    input  [2:0] X,
    input  [2:0] Y,
    output [2:0] S,
    output       Cout
);
    wire C1, C2;

    // sum bits
    assign S[0] = X[0] ^ Y[0];
    assign C1   = X[0] & Y[0];

    assign S[1] = X[1] ^ Y[1] ^ C1;
    assign C2   = (X[1] & Y[1]) | (Y[1] & C1) | (X[1] & C1);

    assign S[2] = X[2] ^ Y[2] ^ C2;
    assign Cout = (X[2] & Y[2]) | (Y[2] & C2) | (X[2] & C2);

endmodule
