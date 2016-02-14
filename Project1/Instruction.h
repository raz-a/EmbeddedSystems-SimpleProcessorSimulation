/*
	Instruction.h
	Defines the Instruction Struct

	Raz Aloni
	2/14/2016
*/

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>
#include "Opcode.h"

namespace Project1
{
	struct Instruction
	{
		Opcode op;
		unsigned char reg;
		unsigned char operand[2];

		Instruction();
		Instruction(Opcode op, unsigned char reg, unsigned char operandA, unsigned char operandB);
		friend std::ostream& operator<<(std::ostream &os, const Instruction &inst);
	};

	const Instruction EMPTY_INSTRUCTION(NOP, 0xFF, 0xFF, 0xFF);
}

#endif