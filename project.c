#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    if (PC % 4)     // Checks for word allignment. (If it is a multiple of 4.)
    {
        //  Assigns the instruction to the instruction at index PC of Mem. Does a bit
        //  adjustment to get the actual location.
        instruction = Mem[PC>>2];
        return 0;   // Because the word allignment was correct, there is no hault condition.
    }
    else
    {
        return 1;   // If the word allignment is incorrect, there is a hault condition.
    }
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    //op:	         instruction [31-26]
	//r1:            instruction [25-21]
	//r2:	         instruction [20-16]
	//r3:	         instruction [15-11]
	//funct:	     instruction [5-0]
	//offset:	     instruction [15-0]
	//jsec:     	 instruction [25-0]

    // When I refer to XOR, it should only get the first n bits to the left of the spot mentioned.
    // n refers to 1<<n.
    // I'm doing this late and need to double check this, but it should do the following:
    *op = (instruction>>25);    // Only get the bits left of the 25th bit.
    *r1 = (1<<5 ^ instruction>>20);     // Only get the bits left of the 20th bit, and XOR it.
    *r2 = (1<<5 ^ instruction>>15);     // Only get the bits left of the 15th bit, and XOR it.
    *r3 = (1<<5 ^ instruction>>10);     // Only get the bits left of the 10th bit, and XOR it.
    *funct = (1<<6 ^ instruction);      // Get the first 6 bits of instruction.
    *offset = (1<<16 ^ instruction);    // Get the first 16 bits of instruction.
    *jsec = (1<<26 ^ instruction);      // Get the first 26 bits of instruction.

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    *data1 = Reg[r1];   // Reads the register at index r1 and set it to the information stored at data1.
    *data2 = Reg[r2];   // Reads the register at index r2 and set it to the information stored at data2.
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value){ //Haven't really tested yet (unsure if works)
    if (offset & 0x8000) //if bit is negative
    {
        *extended_value = offset | 0xFFFF0000;
    }
    else //if bit is positive
    {
        *extended_value = offset & 0x0000FFFF;
    }
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC)//extremely unsure if this works
{
    //default update
    *PC = *PC + 4;
    //in event of branching
    if (Branch == 1 && Zero == 1)
    {
        *PC = *PC + (extended_value << 2);
    }
    //in event of a jump
    if (Jump == 1)
    {
        *PC = (*PC & 0xF0000000) | (jsec << 2);
    }
}


