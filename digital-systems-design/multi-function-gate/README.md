# Multi-Function Logic Gate (Verilog + FPGA)

A digital design project implementing a **multi-function gate** in Verilog, tested through simulation and deployed on an FPGA (Basys 3, Artix-7).  
The design takes four inputs and produces one output based on a simplified Boolean expression derived from a custom truth table.  

---

## Objective
The goal of this project was to design a combinational circuit from scratch, starting from a functional description, then moving through:
- Deriving and simplifying the Boolean expression  
- Implementing the circuit in Verilog  
- Writing a testbench to verify correctness  
- Synthesizing the design and testing it on FPGA hardware  

---

## Boolean Logic
\[
F = A’B’X + X’AB + YA’B + YAB
\]

---

## Files
- `src/multi_function_gate.v` – Main Verilog design module  
- `sim/multi_function_gate_tb.v` – Testbench that toggles all input combinations to verify output correctness  

---

## Simulation & Hardware Testing
- The testbench systematically drives all 16 possible input states (`A, B, X, Y`) and confirms the output matches the Boolean expression.  
- Simulation results aligned perfectly with the expected truth table.  
- Hardware verification was performed using the **Basys 3 FPGA board**: switches were mapped to inputs (`X, Y, A, B`) and the output (`F`) was displayed on an LED.  

---

## Key Takeaways
- Full workflow from **problem statement → truth table → Boolean expression → Verilog HDL → FPGA deployment**  
- Importance of testbenches in validating combinational logic prior to synthesis  
- Gained experience with Vivado for synthesis, implementation, and hardware bring-up  

---

