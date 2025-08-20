`timescale 1ns/1ps
module ram_top (
    input  [4:0] SW,      // SW[4:0] = address (32x4)
    input  [3:0] DIN,     // SW[8:5] = data inputs
    input        WE_BTN,  // Write Enable (active high)
    input        CLK_BTN, // Write clock button
    output [3:0] LED      // LEDs show RAM output
);

    wire [3:0] dout;

    // Instantiate your 32x4 RAM
    ram32x4 RAM_U (
        .clk   (CLK_BTN),
        .write (WE_BTN),
        .addr  (SW),
        .din   (DIN),
        .dout  (dout)
    );

    assign LED = dout;

endmodule
