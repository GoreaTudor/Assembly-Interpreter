# Assembly-Interpreter
I'm trying to make my own assembly interpreter with a few commands

Logic Operators:
 0 - AND  $adr / [adr]			--	Perform AND operation between the contents of the accumulator and the value from register with address adr
 1 - OR   $adr / [adr]			-- Perform AND operation between the contents of the accumulator and the value from register with address adr
 2 - NOT  $adr / [adr]			-- Perform AND operation between the contents of the accumulator and the value from register with address adr

Arithmetic Operators:
 3 - ADD  $adr / [adr] / nr   -- Adds the contents of the Accumulator to the value of the number
 4 - SUB  $adr / [adr] / nr			-- Subtracts
 5 - INC  $adr / [adr]
 6 - DEC  $adr / [adr]
 7 - SHL 
 8 - SHR 

Memory Operators:
 9 - LOAD   $adr/nr  			-- Overwrites the contents of the Accumulator with the values from the register with adress adr or with the number nr
10 - STORE  $adr 			 			-- Stores the contents of the Accumulator to the register with address adr

Jumpers:
11 - JMP  adr
12 - JZ   adr
13 - JNZ  adr
14 - JN   adr

15 - STOP
