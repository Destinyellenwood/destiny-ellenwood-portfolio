// 3-to-8 Decoder built using two 2-to-4 decoders
// Destiny Ellenwood

module decoder3to8(
    input  wire [2:0] inp,   // 3-bit input
    output wire [7:0] outp   // one-hot output
);
    wire temp;

    // Invert MSB to control lower decoder
    not myinverter(temp, inp[2]);

    // Lower half (enable when MSB = 0)
    decoder2to4 dec0 (
        .enable(temp),
        .inp(inp[1:0]),
        .outp(outp[3:0])
    );

    // Upper half (enable when MSB = 1)
    decoder2to4 dec1 (
        .enable(inp[2]),
        .inp(inp[1:0]),
        .outp(outp[7:4])
    );
endmodule
