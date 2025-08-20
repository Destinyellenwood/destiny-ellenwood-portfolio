`timescale 1ns / 1ps
module RAM_SIM();
    wire [3:0] data_t;
    reg [3:0] address_t;
    reg we_t, clk_t;
    wire [3:0] out_t;

    RAM YAY (.data(data_t), .address(address_t), .we(we_t), .clk(clk_t), .out(out_t));
    data_tester data_test_uut(address_t[0], address_t[1], address_t[2], address_t[3],
                              data_t[0], data_t[1], data_t[2], data_t[3]);

    integer i;
    initial begin
        clk_t = 1'b0;
        we_t = 1'b0;
        address_t = 4'd0;
        for (i = 0; i <= 3; i = i + 1) begin
            #40 address_t = address_t + 1;
        end
    end

    always #5 clk_t = ~clk_t;
    always #20 we_t = ~we_t;
endmodule
