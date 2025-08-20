# Decoder and Demultiplexer (Verilog)

## Overview
This project implements decoder circuits in Verilog as part of combinational logic design. Decoders take an `n`-bit input and activate exactly one of the `2^n` outputs.

The work includes:
- A **2-to-4 decoder** with enable input
- A **3-to-8 decoder** built hierarchically using two 2-to-4 decoders
- Testbenches for both designs to verify correctness

## Files
- `decoder2to4.v` – 2-to-4 decoder module
- `decoder3to8.v` – 3-to-8 decoder module (uses two 2-to-4 blocks)
- `decoder2to4_tb.v` – testbench for 2-to-4 decoder
- `decoder3to8_tb.v` – testbench for 3-to-8 decoder

## 2-to-4 Decoder
- **Inputs:** `enable`, `inp[1:0]`
- **Output:** `outp[3:0]` (one-hot)
- Behavior:
  - When `enable = 1`, the output matches the input.
  - When `enable = 0`, output is `0000`.

## 3-to-8 Decoder
- **Input:** `inp[2:0]`
- **Output:** `outp[7:0]` (one-hot)
- Built hierarchically: one 2-to-4 handles lower half, another handles upper half.

## Simulation
- The 2-to-4 testbench cycles through all inputs and verifies the enable function.
- The 3-to-8 testbench iterates through all 8 input states and validates one-hot output.

## Key Takeaways
- Designed and verified hierarchical decoders in Verilog.
- Gained experience with one-hot output logic and modular coding.
- Practiced building larger circuits from smaller reusable blocks.

