`timescale 1ns / 1ps
module RAM (
    input [3:0] data,
    input [3:0] address,
    input we,
    input clk,
    output reg [3:0] out
);
    reg [3:0] mem [0:15]; // 16x4 RAM

    always @(posedge clk) begin
        if (we)
            mem[address] <= data; // write operation
        out <= mem[address];      // read operation
    end
endmodule
