`timescale 1ns / 1ps
// Testbench for 8:1 Multiplexer
// Destiny Ellenwood

module mux8to1_tb;
    reg  [2:0] S_t;   // select lines
    reg  [7:0] D_t;   // data inputs
    wire       Out_y_t; // output

    // instantiate DUT
    mux8to1 UUT (
        .S(S_t),
        .D(D_t),
        .Y(Out_y_t)
    );

    integer i;

    initial begin
        D_t = 8'd0;
        S_t = 3'd0;

        // iterate through each data input bit
        for (i = 0; i <= 7; i = i + 1) begin
            #80;             // delay before switching
            D_t = 8'd0;      // reset data inputs
            D_t[i] = 1'b1;   // set i-th input high
        end
    end

    // toggle select every 10ns
    always #10 S_t = S_t + 1;
endmodule
