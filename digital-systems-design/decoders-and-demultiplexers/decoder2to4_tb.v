// Testbench for 2-to-4 Decoder
// Destiny Ellenwood

`timescale 1ns/1ps

module decoder2to4_tb();
    reg enable;
    reg [1:0] inp;
    wire [3:0] outp;

    // Instantiate UUT
    decoder2to4 UUT (
        .enable(enable),
        .inp(inp),
        .outp(outp)
    );

    integer i;

    initial begin
        enable = 1'b1;   // enable active
        inp = 2'b00;

        // Iterate through all 4 input combinations
        for (i = 0; i < 4; i = i + 1) begin
            #20 inp = i;
        end

        // Test disable condition
        #20 enable = 1'b0;
    end
endmodule
