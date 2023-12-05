# CDA-processor-simulator
1. Purpose
This project is intended to help you understand the instructions of a very
simple assembly language and how to simulate the execution of the
resulting
machine code representation of a program.
2. Problem
In this project you will write a behavioral simulator for the machine code
created using the assembler code provided. This simulator will read in a
text file
consisting of LC3100 machine code instructions (represented as decimal
values),
and execute the program, then display the values of register files and
memory
after each instruction is completed. Running any reasonable length program
will
generate a large amount of output, but it will make debugging easier.
3. LC3100 Instruction-Set Architecture
For the first several projects, you will be gradually "building" the
LC3100
(Little Computer for CDA 3100). The LC3100 is very simple, but it is
general
enough to solve complex problems. For this project, you will only need to
know
the instruction set and instruction format of the LC3100.
The LC3100 is an 8-register, 32-bit computer. All addresses are
word-addresses. The LC3100 has 65536 words of memory. By assembly-
language
convention, register 0 will always contain 0 (i.e. the machine will not
enforce
this, but no assembly-language program should ever change register 0 from
its
initial value of 0).
There are 4 instruction formats (bit 0 is the least-significant bit).
Bits
31-25 are unused for all instructions, and should always be 0.
R-type instructions (add, nand):
bits 24-22: opcode
bits 21-19: reg A
bits 18-16: reg B
bits 15-3: unused (should all be 0)
bits 2-0: destReg
I-type instructions (lw, sw, beq):
bits 24-22: opcode
bits 21-19: reg A
bits 18-16: reg B
bits 15-0: offsetField (an 16-bit, 2's complement number with a range
of
-32768 to 32767)
O-type instructions (halt, noop):
bits 24-22: opcode
bits 21-0: unused (should all be 0)
--------------------------------------------------------------------------
-----
Table 1: Description of Machine Instructions
--------------------------------------------------------------------------
-----
Assembly language Opcode in binary Action
name for instruction (bits 24, 23, 22)
--------------------------------------------------------------------------
-----
add (R-type format) 000 add contents of regA with
contents of regB,
store
results in
destReg.
nand (R-type format) 001 nand contents of regA with
contents of regB,
store
results in
destReg.
lw (I-type format) 010 load regB from memory. Memory
address is formed
by adding
offsetField with
the contents of
regA.
sw (I-type format) 011 store regB into memory. Memory
address is formed
by adding
offsetField with
the contents of
regA.
beq (I-type format) 100 if the contents of regA and
regB are the
same, then branch
to the address
PC+1+offsetField,
where PC is the
address of the
beq instruction.
xxx (O-type format) 101 Unused for this assignment.
halt (O-type format) 110 increment the PC (as with all
instructions),
then halt the
machine (let the
simulator
notice that the
machine
halted).
noop (O-type format) 111 do nothing.
--------------------------------------------------------------------------
-----
4. Assembly Code
For this (and later assignments, you are provided an assembler to enable
you to
write test cases in LC3100 assembly code instead of LC3100 machine code.
The format for a line of assembly code is (<white> means a series of tabs
and/or spaces):
label<white>instruction<white>field0<white>field1<white>field2<white>comme
nts
The leftmost field on a line is the label field. Valid labels contain a
maximum of 6 characters and can consist of letters and numbers (but must
start
with a letter). The label is optional (the white space following the label
field is required). Labels make it much easier to write assembly-language
programs, since otherwise you would need to modify all address fields each
time
you added a line to your assembly-language program!
After the optional label is white space. Then follows the instruction
field,
where the instruction can be any of the assembly-language instruction
names
listed in the above table. After more white space comes a series of
fields.
All fields are given as decimal numbers or labels. The number of fields
depends on the instruction, and unused fields should be ignored (treat
them
like comments).
R-type instructions (add, nand) instructions require 3 fields: field0
is regA, field1 is regB, and field2 is destReg.
I-type instructions (lw, sw, beq) require 3 fields: field0 is regA,
field1
is regB, and field2 is either a numeric value for offsetField or a
symbolic
address. Numeric offsetFields can be positive or negative; symbolic
addresses are discussed below.
O-type instructions (noop and halt) require no fields.
Symbolic addresses refer to labels. For lw or sw instructions, the
assembler
should compute offsetField to be equal to the address of the label. This
could
be used with a zero base register to refer to the label, or could be used
with
a non-zero base register to index into an array starting at the label.
For beq
instructions, the assembler should translate the label into the numeric
offsetField needed to branch to that label.
After the last used field comes more white space, then any comments. The
comment field ends at the end of a line. Comments are vital to creating
understandable assembly-language programs, because the instructions
themselves
are rather cryptic.
In addition to LC3100 instructions, an assembly-language program may
contain
directions for the assembler. The only assembler directive we will use is
.fill
(note the leading period). .fill tells the assembler to put a number into
the
place where the instruction would normally be stored. .fill instructions
use
one field, which can be either a numeric value or a symbolic address. For
example, ".fill 32" puts the value 32 where the instruction would normally
be
stored. .fill with a symbolic address will store the address of the
label.
In the example below, ".fill start" will store the value 2, because the
label
"start" is at address 2.
The assembler makes two passes over the assembly-language program. In the
first pass, it will calculate the address for every symbolic label.
Assume
that the first instruction is at address 0. In the second pass, it will
generate a machine-language instruction (in decimal) for each line of
assembly
language. For example, here is an assembly-language program (that counts
down
from 5, stopping when it hits 0).
lw 0 1 five load reg1 with 5 (uses symbolic address)
lw 1 2 3 load reg2 with -1 (uses numeric address)
start add 1 2 1 decrement reg1
beq 0 1 2 goto end of program when reg1==0
beq 0 0 start go back to the beginning of the loop
noop
done halt end of program
five .fill 5
neg1 .fill -1
stAddr .fill start will contain the address of start (2)
Here is the symbol table generated at the end of pass one (note that the
symbol
table will not be written to the file - unless you do it for debugging
purposes)
start 2
done 6
five 7
neg1 8
stAddr 9
And here is the corresponding machine language:
(address 0): 8454151 (hex 0x810007)
(address 1): 9043971 (hex 0x8a0003)
(address 2): 655361 (hex 0xa0001)
(address 3): 16842754 (hex 0x1010002)
(address 4): 16842749 (hex 0x100fffd)
(address 5): 29360128 (hex 0x1c00000)
(address 6): 25165824 (hex 0x1800000)
(address 7): 5 (hex 0x5)
(address 8): -1 (hex 0xffffffff)
(address 9): 2 (hex 0x2)
Be sure you understand how the above assembly-language program got
translated
to machine language.
Since your programs will always start at address 0, your program should
only
output the contents, not the addresses.
8454151
9043971
655361
16842754
16842749
29360128
25165824
5
- 1
2
5. Behavioral Simulator
The first assignment is to write a program that can simulate any
legal LC3100 machine-code program. The input for this part will be the
machine-code file that you created with your assembler. With a program
name
of "simulate" and a machine-code file of "program.mc", your program should
be
run as follows:
simulate program.mc > output
This directs all printfs to the file "output".
The simulator should begin by initializing all registers and the program
counter to 0. The simulator will then simulate the program until the
program
executes a halt.
The simulator should call printState (included below) before executing
each
instruction and once just before exiting the program. This function
prints the
current state of the machine (program counter, registers, memory).
printState
will print the memory contents for memory locations defined in the
machine-code
file (addresses 0-9 in the example used in assignment 1).
5.1 Test Cases
You will write a suite of test cases to validate any LC3100 simulator.
The test cases for the simulator part of this project will be short
assembly-language programs that, after being assembled into machine code,
serve
as input to a simulator. You will submit your suite of test cases
together
with your simulator, and we will grade your test suite according to how
thoroughly it exercises an LC3100 simulator. Each test case may execute
at
most 200 instructions on a correct simulator, and your test suite may
contain
up to 20 test cases. These limits are much larger than needed for full
credit
(the solution test suite is composed of a couple test cases, each
executing
less than 40 instructions).
graded.
5.2. Simulator Hints
While this assigmnent is fairly easy, the next ones will not be. I
suggest that
you do incremental testing of your program. This means start with
confirming
that you are able to load the machine code and the initial print state is
correct.
Then chose one instruction at a time to test. The instruction "halt"
should be the
first test. Create a machine code program consisting of just the halt
instruction,
assemble then simulate it. My advice is to do this well before you have
started
programming the simulator to handle any other instructions. Once this
works, you
can add another instruction to your simulator ("add" would be a good
choice) and
write an assembly language program that consists of two instructions - add
followed
by halt. Assemble, simulate and debug as necessary. This incremental
development
style is better in general and for the complex projects will will soon be
doing,
it is necessary (at least it will minimize frustration).
Also, be careful how you handle offsetField for lw, sw, and beq. Remember
that it's
a 2's complement 16-bit number, so you need to convert a negative
offsetField
to a negative 32-bit integer on the Sun workstations (by sign extending
it).
To do this, use the following function.
int
convertNum(int num)
{
/* convert a 16-bit number into a 32-bit Sun integer */
if (num & (1<<15) ) {
num -= (1<<16);
}
return(num);
}
An example run of the simulator (not for the specified task of
multiplication)
is included at the end of this posting.
6. Grading, Formatting and Test Cases
We will grade primarily on functionality, correctly simulating all
instructions, input and output format, and comprehensiveness of the
test suites.
To help you validate your project, your submission will be graded
using scripts The results from the grader will not be very illuminating;
they won't tell you where your problem is or give you the test programs.
The best way to debug your program is to generate your own test cases,
figure out the correct answers, and compare your program's output to the
correct answers. This is also one of the best ways to learn the concepts
in the project.
The student suite of test cases for the simulator project will be graded
according to how thoroughly they test an LC3100 simulator. We will judge
thoroughness of the test suite by how well it exposes bugs in our
simulator.
For your simulator test suite, the grader will correctly assemble each
test case, then use it as input to our "buggy" simulator which tests for
common implementation errors. A test case exposes a buggy simulator by
causing it to generate a different answer from a correct simulator.
The test suite is graded based on how many of the buggy simulators were
exposed by at least one test case.
Because all programs will be graded in a semi-automated manner using
scripts,
you must be careful to follow the exact formatting rules in the project
description:
1) Don't modify printState or stateStruct at all. Download
this handout into your program electronically (don't re-type it) to
avoid typos.
2) Call printState exactly once before each instruction
executes and once just before the simulator exits. Do not call
printState at any other time.
3) Don't print the sequence "@@@" anywhere except in printState.
4) state.numMemory must be equal to the number of lines in the
machine-code file.
5) Initialize all registers to 0.
7. Turning in the Project
Use the canvas link for this project to submit your program that simulates
the
LC3100 machine code as well as a set of assembly language files that
constitute
your test cases.