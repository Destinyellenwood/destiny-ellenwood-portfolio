// 2:1 Multiplexer - Behavioral Form
module mux2to1_behavioral (
    input  wire A,
    input  wire B,
    input  wire S,
    output reg  Y
);
    always @(*) begin
        case (S)
            1'b0: Y = A;
            1'b1: Y = B;
        endcase
    end
endmodule
