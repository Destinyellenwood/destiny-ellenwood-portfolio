# Three-Bit Binary Adder (Verilog + FPGA)

A modular **3-bit adder** written in Verilog, verified with simulation and tested on FPGA hardware (Basys 3, Artix-7).  
The design computes `X[2:0] + Y[2:0]` and produces a 3-bit sum plus a carry-out.

---

## Design Overview
- **Bit 0**: `S0 = X0 ⊕ Y0`, `C1 = X0·Y0`  
- **Bit 1**: `S1 = X1 ⊕ Y1 ⊕ C1`, `C2 = X1Y1 + X1C1 + Y1C1`  
- **Bit 2**: `S2 = X2 ⊕ Y2 ⊕ C2`, `Cout = X2Y2 + X2C2 + Y2C2`  

X[2:0] + Y[2:0] → {Cout, S[2:0]}


---

## Files
- `src/three_bit_adder.v` – 3-bit binary adder  
- `sim/three_bit_adder_tb.v` – testbench that exhaustively drives all input combinations using a binary counter–style toggle pattern  

---

## Testbench Methodology
Instead of manually looping through all 64 combinations of X and Y, the testbench flips each input bit at a different interval:  

```verilog
always #5   X[0] = ~X[0];
always #10  X[1] = ~X[1];
always #20  X[2] = ~X[2];

always #40  Y[0] = ~Y[0];
always #80  Y[1] = ~Y[1];
always #160 Y[2] = ~Y[2];

