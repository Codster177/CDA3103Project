#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero){
    // Switch to control which ALU control gets executed
	switch ((int) ALUControl)
	{
        case 0://add
            *ALUresult = A + B;
            break;

        case 1: //subtract
            *ALUresult = A - B;
            break;

        case 2:
            if ((signed)A < (signed)B)
            {
                *ALUresult = 1;
            }
            else
            {
                *ALUresult = 0;
            }
            break;

        case 3:
            if (A < B)
            {
                *ALUresult = 1;
            }
            else
            {
                *ALUresult = 0;
            }
            break;

        case 4: //and
            *ALUresult = A & B;
            break;

        case 5: //or

            *ALUresult = A | B;
            break;


        case 6: //left shift
            *ALUresult = B << 16;
            break;

        case 7: //not
            *ALUresult = ~A;
            break;
	}
	if(*ALUresult == 0){
		*Zero = 1;//true 0
	}
	else{
		*Zero = 0;//Not 0
	}
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    if (PC % 4 == 0)     // Checks for word allignment. (If it is a multiple of 4.)
    {
        //  Assigns the instruction to the instruction at index PC of Mem. Does a bit
        //  adjustment to get the actual location.
        *instruction = Mem[(PC>>2)];
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
    *op = (instruction>>26) & 0x0000003f;    // Only get the bits left of the 25th bit.
    *r1 = (instruction>>21) & 0x1f;     // Only get the bits left of the 20th bit, and XOR it.
    *r2 = (instruction>>16) & 0x1f;     // Only get the bits left of the 15th bit, and XOR it.
    *r3 = (instruction>>11) & 0x1f;     // Only get the bits left of the 10th bit, and XOR it.
    *funct = (instruction) & 0x0000003f;      // Get the first 6 bits of instruction.
    *offset = (instruction) & 0x0000ffff;    // Get the first 16 bits of instruction.
    *jsec = (instruction) & 0x03ffffff;      // Get the first 26 bits of instruction.

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
    if (ALUSrc == 1)
    {
        data2 = extended_value;
    }

    if (ALUOp == 7)
    {
        if (funct == 32)
        {
            ALUOp = 0;
        }
        else if (funct == 34)
        {
            ALUOp = 1;
        }
        else if (funct == 42)
        {
            ALUOp = 2;
        }
        else if (funct == 43)
        {
            ALUOp = 3;
        }
        else if (funct == 36)
        {
            ALUOp = 4;
        }
        else if (funct == 37)
        {
            ALUOp = 5;
        }
        else if (funct == 6)
        {
            ALUOp = 6;
        }
        else if (funct == 39)
        {
            ALUOp = 7;
        }
        else
        {
            return 1;
        }
    }
    ALU(data1, data2, ALUOp, ALUresult, Zero);
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    if (ALUresult % 4 != 0)
    {
        return 1;
    }
    if (MemRead == 1)
    {
        *memdata = Mem[ALUresult >> 2];
    }
    if (MemWrite == 1)
    {
        Mem[ALUresult >> 2] == data2;
    }
    return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    unsigned dest;
    if (RegDst == 0)
    {
        dest = r2;
    }
    else if (RegDst == 1)
    {
        dest = r3;
    }
    if (RegWrite == 1)
    {
        if (MemtoReg == 1)
        {
            Reg[dest] = memdata;
        }
        else if (MemtoReg == 0)
        {
            Reg[dest] = ALUresult;
        }
    }
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
        *PC = (jsec << 2) | (*PC & 0xf0000000);
    }
}