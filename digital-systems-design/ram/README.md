# 32x4 RAM / ROM in Verilog (Basys-3 FPGA)

## Overview
This project implements a **32-word x 4-bit synchronous RAM** on the Basys-3 FPGA.  
- **Write:** Data is written on the rising edge of `CLK` when `WE=1`.  
- **Read:** Data is read synchronously from the selected address.  
- Can also act as a **ROM** by preloading truth tables (F1, F2) and disabling writes.  

## Files
- `ram32x4.v` — 32x4 RAM design (synchronous read/write)  
- `ram_sim.v` — simulation testbench for functional verification  
- `data_tester.v` — generates logic functions to preload/test RAM contents  
- `ram_top.v` — simple Basys-3 wrapper mapping switches, buttons, and LEDs  

## Basys-3 Mapping
- **SW[4:0]** → Address lines  
- **SW[8:5]** → Data inputs (D3..D0)  
- **BTN0 (WE)** → Write Enable  
- **BTN1 (CLK)** → Write Clock (rising edge triggers write)  
- **LED[3:0]** → RAM output  

## Usage
1. **Simulation**: Run `ram_sim.v` to verify write/read operations.  
2. **Hardware**:  
   - Set address on `SW[4:0]` and data on `SW[8:5]`.  
   - Hold **WE=1**, tap **CLK** → data is written.  
   - Release **WE=0**, select any address → LEDs display stored data.  
3. Preload truth tables (e.g., F1, F2) into RAM to implement ROM behavior.  

## Key Takeaways
- Built a synchronous RAM with Verilog `always @(posedge clk)`  
- Verified correct read/write using simulation and FPGA hardware  
- Demonstrated how RAM can emulate ROM for combinational functions  
