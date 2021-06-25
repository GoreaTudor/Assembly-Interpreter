# Assembly-Interpreter
I'm trying to make my own assembly interpreter with a few commands

Logic Operators:
 0 - AND  A/adr/[adr]
 1 - OR   A/adr/[adr]
 2 - NOT  A/adr/[adr]

Arithmetic Operators:
 3 - ADD  $adr/[adr]
 4 - SUB  $adr/[adr]
 5 - INC  $adr/[adr]
 6 - DEC  $adr/[adr]
 7 - SHL 
 8 - SHR 

Memory Operators:
 9 - LOAD   $adr/nr  -- Overwrites the contents of the Accumulator with the values from the register with adress adr
10 - STORE  $adr/nr  -- Stores the contents of the Accumulator to the register with address adr

Jumpers:
11 - JMP  adr
12 - JZ   adr
13 - JNZ  adr
14 - JN   adr

15 - STOP
