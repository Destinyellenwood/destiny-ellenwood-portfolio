// 2-to-4 Decoder with Enable (procedural design)
// Destiny Ellenwood

module decoder2to4(
    input  wire       enable,   // active-high enable
    input  wire [1:0] inp,      // 2-bit input
    output reg  [3:0] outp      // one-hot output
);
    always @(*) begin
        if (enable) begin
            case (inp)
                2'b00: outp = 4'b0001;
                2'b01: outp = 4'b0010;
                2'b10: outp = 4'b0100;
                2'b11: outp = 4'b1000;
                default: outp = 4'b0000;
            endcase
        end else begin
            outp = 4'b0000; // disabled → all outputs low
        end
    end
endmodule
