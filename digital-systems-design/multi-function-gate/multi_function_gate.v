// Experiment 2 - Multi-Function Gate
// Destiny Ellenwood

module Experiment_2 (
    input wire X,
    input wire Y,
    input wire A,
    input wire B,
    output wire F
);
    assign F = (~A & ~B & X) | (~X & A & B) | (Y & ~A & B) | (Y & A & B);
endmodule
