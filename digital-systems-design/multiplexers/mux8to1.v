`timescale 1ns / 1ps
// 8:1 Multiplexer (procedural design)
// Destiny Ellenwood

module mux8to1 (
    input  wire [2:0] S,   // select lines
    input  wire [7:0] D,   // data inputs
    output reg        Y    // output
);
    always @(*) begin
        case (S)
            3'b000: Y = D[0];
            3'b001: Y = D[1];
            3'b010: Y = D[2];
            3'b011: Y = D[3];
            3'b100: Y = D[4];
            3'b101: Y = D[5];
            3'b110: Y = D[6];
            3'b111: Y = D[7];
            default: Y = 1'b0;
        endcase
    end
endmodule
