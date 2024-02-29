# TCP1101
The Assembly Language Interpreter that is being given here functions as a Virtual Machine (VM)
environment designed with processing ASM input files in mind. It goes through each instruction in
the given ASM file methodically, one after the other, until it produces a final output. The output is
sent to a text file, whose name is the same as the original ASM file plus the suffix ".txt". This
interpreter has four flags: Overflow Flag (OF), Underflow Flag (UF), Carry Flag (CF), and Zero
Flag (ZF). It consists of seven registers, designated as R0 through R6. The interpreter also has
memory locations 0 through 63 as well as a Program Counter (PC).
