# Multiplexer Designs (Verilog + FPGA)

This project demonstrates how multiplexers can be used in combinational logic design.  
A multiplexer (MUX) selects one of several inputs and routes it to a single output based on control lines.  
Designs include a **2:1 multiplexer** in both logic and behavioral styles, and a **general 8:1 multiplexer**.

---

## Designs
- **2:1 MUX (Logic Form)** – Boolean expression with `assign`
- **2:1 MUX (Behavioral Form)** – `case` statement in an `always` block
- **8:1 MUX** – Procedural design with three select bits (`S2, S1, S0`)

---

## Files
- `src/mux2to1_logic.v` – 2:1 multiplexer (logic form)
- `src/mux2to1_behavioral.v` – 2:1 multiplexer (behavioral form)
- `src/mux8to1.v` – 8:1 multiplexer
- `sim/mux8to1_tb.v` – testbench verifying all select values

---

## Testbench
The 8:1 testbench:
- Iterates `S` from `000` → `111`
- Activates one data input at a time  
- Confirms the output always matches the selected input

---


