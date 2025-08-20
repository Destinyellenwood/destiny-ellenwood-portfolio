// Testbench for 3-to-8 Decoder
// Destiny Ellenwood

`timescale 1ns/1ps

module decoder3to8_tb();
    reg  [2:0] inp_t;     // test input
    wire [7:0] outp_t;    // observed output

    // Instantiate UUT
    decoder3to8 UUT (
        .inp(inp_t),
        .outp(outp_t)
    );

    integer i;

    initial begin
        inp_t = 3'b000;   // start at 0

        // Iterate through all 8 input combinations
        for (i = 0; i < 8; i = i + 1) begin
            #50 inp_t = inp_t + 1;
        end
    end
endmodule
