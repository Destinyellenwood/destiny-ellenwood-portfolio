`timescale 1ns / 1ps
module data_tester(
    input A, B, C, D,
    output P, Q, R, S
);
    assign P = A & B;
    assign Q = B | C;
    assign R = ~(C & D);
    assign S = ~(C | D);
endmodule
