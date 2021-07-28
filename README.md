# Assembly-Interpreter
I'm trying to make my own assembly interpreter with a few commands

.

%adr - Value from the address adr (variable)

&adr - Value from the address that is contained in adr (pointer)

value - A numeric value (a number)

A - The Accumulator (main register)

line - where to jump in a (un)conditional jump

.

Arithmetic Operators:

 - ADD  %adr / &adr / value

 - SUB  %adr / &adr / value 			

 - INC  %adr / &adr / value / A

 - DEC  %adr / &adr / value / A

.

Logic Operators:

 - AND  %adr / &adr / value

 - OR   %adr / &adr / value

 - NOT  %adr / &adr / value

.

Bit Operators:

 - SHL  %adr / &adr / A

 - SHR  %adr / &adr / A

.

Memory Operators:

 - STORE  %adr / &adr

 - LOAD   %adr / &adr / value

.

Jumpers:

 - JMP  line

 - JZ   line

 - JNZ  line

 - JLZ  line

 - JMZ  line

 - JC   line

 - JNC  line

.

 - HLT
