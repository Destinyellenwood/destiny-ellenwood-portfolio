// 2:1 Multiplexer - Logic Form
module mux2to1_logic (
    input  wire A,
    input  wire B,
    input  wire S,
    output wire Y
);
    assign Y = (A & ~S) | (B & S);
endmodule
