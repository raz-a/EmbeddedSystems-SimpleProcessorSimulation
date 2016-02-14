#include "Instruction.h"

namespace Project1
{
	Instruction::Instruction(Opcode op, unsigned char reg, unsigned char operandA, unsigned char operandB)
	{
		this->op = op;
		this->reg = reg;
		this->operand[0] = operandA;
		this->operand[1] = operandB;
	}

	std::ostream & operator<<(std::ostream &os, const Instruction & inst)
	{
		os << "<" << OpCodeNames[inst.op] << ",R" << +inst.reg << "," << +inst.operand[0] << "," << +inst.operand[1] << ">";
		return os;
	}
}


