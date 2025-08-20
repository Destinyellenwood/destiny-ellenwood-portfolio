`timescale 1ns / 1ps

// Testbench for Experiment_2 Multi-Function Gate
// Destiny Ellenwood

module Experiment_2_Sim;

    // Inputs
    reg A;
    reg B;
    reg X;
    reg Y;

    // Output
    wire F;

    // Instantiate the Unit Under Test (UUT)
    Experiment_2 UUT (
        .A(A),
        .B(B),
        .X(X),
        .Y(Y),
        .F(F)
    );

    // Initialize inputs
    initial begin
        A = 1'b0;
        B = 1'b0;
        X = 1'b0;
        Y = 1'b0;
    end

    // Toggle inputs at different intervals
    always #5  A = ~A;
    always #10 B = ~B;
    always #20 X = ~X;
    always #40 Y = ~Y;

endmodule
