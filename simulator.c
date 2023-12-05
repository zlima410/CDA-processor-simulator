/* instruction-level simulator for LC3100 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8       /* number of machine registers */
#define MAXLINELENGTH 1000

#define ADD 0
#define NAND 1
#define LW 2
#define SW 3
#define BEQ 4
#define JALR 5
#define HALT 6
#define NOOP 7

typedef struct stateStruct
{
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *); //print the state of the machine
int convertNum(int num); //convert a 16-bit number into a 32-bit integer
void executeInstruction(int instruction, stateType *state); //execute the instruction based on the 32-bit instruction and the state of the machine

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;
    int executionCount = 0;

    //read in machine code to the simulator
    if (argc != 2)
    {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }
    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL)
    {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; state.numMemory++)
    {
        if (sscanf(line, "%d", state.mem + state.numMemory) != 1)
        {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory,
               state.mem[state.numMemory]);
    }

    //initialize the state of the machine and all registers to 0 then print the state
    for (int i = 0; i < NUMREGS; i++)
    {
        state.reg[i] = 0;
    }
    printState(&state);

    state.pc = 0;

    //execute the instructions until the HALT instruction is reached
    while (state.pc < state.numMemory)
    {
        executionCount++; //increment the executionCounter every time an instruction is executed
        
        int instruction = state.mem[state.pc]; //get the instruction from memory

        executeInstruction(instruction, &state); //execute the instruction

        printState(&state); //print the state of the machine

        if ((instruction >> 22) == HALT) //if the instruction is HALT, break out of the loop
        {
            break;
        }
    }

    //prints the results at the end of the simulation
    printf("machine halted\n");
    printf("total of %d instructions executed\n", executionCount);
    printf("final state of machine:\n");

    printState(&state); //print the final state of the machine

    fclose(filePtr); //close the file

    return (0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++)
    {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++)
    {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
    if (num & (1 << 15))
    {
        num -= (1 << 16);
    }
    return (num);
}

void executeInstruction(int instruction, stateType *state)
{
    //initialize variables with the values from the 32-bit instruction
    int opcode = (instruction >> 22) & 0x7;
    int regA = (instruction >> 19) & 0x7;
    int regB = (instruction >> 16) & 0x7;
    int destReg = instruction & 0x7;
    int offsetField = instruction & 0xFFFF;

    switch (opcode)
    {
    case ADD:
        state->reg[destReg] = state->reg[regA] + state->reg[regB];
        state->pc++;
        break;
    case NAND:
        state->reg[destReg] = ~(state->reg[regA] & state->reg[regB]);
        state->pc++;
        break;
    case LW:
        state->reg[regB] = state->mem[state->reg[regA] + convertNum(offsetField)];
        state->pc++;
        break;
    case SW:
        state->mem[state->reg[regA] + convertNum(offsetField)] = state->reg[regB];
        state->pc++;
        break;
    case BEQ:
        if (state->reg[regA] == state->reg[regB])
        {
            state->pc += convertNum(offsetField) + 1;
        }
        else
        {
            state->pc++;
        }
        break;
    case JALR:
        state->reg[regB] = state->pc + 1;
        state->pc = state->reg[regA];
        break;
    case HALT:
        state->pc++;
        break;
    case NOOP:
        state->pc++;
        break;
    default:
        printf("Invalid opcode: %d\n", opcode);
        exit(1);
    }
}