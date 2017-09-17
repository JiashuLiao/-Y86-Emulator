# -Y86-Emulator
Rutgers CS 211 Assignment

The Y86 architecture has eight registers, three condition codes, a program counter and memory
that holds machine instructions and data. All addresses, immediate values and displacements are
32 bit little-endian values.

Each of the eight registers has a 4-bit ID that fits into the Y86 instructions. 

The condition codes are single-bit flags set by arithmetic or logical instructions.

The program counter is the address of the next machine instruction to execute. Total memory size
will have to be determined as part of emulator execution.
The Y86 instruction set is modeled on the larger Intelx86 instruction set, but is not a direct subset.

Using array to store the data. Then check every line to decode. For different cases, I used if-else statement to decode the program. Some illegal input will make the emulator quit. If user input -h they will get help information. 
