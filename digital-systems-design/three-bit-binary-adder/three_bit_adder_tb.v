`timescale 1ns / 1ps
// Testbench for Three-Bit Binary Adder
// Destiny Ellenwood

module three_bit_adder_tb;

    // Inputs (reg because we drive them)
    reg  [2:0] X;
    reg  [2:0] Y;

    // Outputs (wire because they come from DUT)
    wire [2:0] S;
    wire       Cout;

    // Instantiate the Unit Under Test (UUT)
    three_bit_adder UUT (
        .X(X),
        .Y(Y),
        .S(S),
        .Cout(Cout)
    );

    // Initialize inputs
    initial begin
        X = 3'b000;
        Y = 3'b000;
    end

    // Toggle input bits at different rates
    always #5   X[0] = ~X[0];
    always #10  X[1] = ~X[1];
    always #20  X[2] = ~X[2];

    always #40  Y[0] = ~Y[0];
    always #80  Y[1] = ~Y[1];
    always #160 Y[2] = ~Y[2];

endmodule
