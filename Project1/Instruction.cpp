#include "Instruction.h"

namespace Project1
{
	Instruction::Instruction()
	{
		*this = EMPTY_INSTRUCTION;
	}

	Instruction::Instruction(Opcode op, unsigned char reg, unsigned char operandA, unsigned char operandB)
	{
		this->op = op;
		this->reg = reg;
		this->operand[0] = operandA;
		this->operand[1] = operandB;
	}

	bool Instruction::operator==(const Instruction & inst)
	{
		if (this->op != inst.op)
		{
			return false;
		}

		if (this->reg != inst.reg)
		{
			return false;
		}

		for (int i = 0; i < 1; i++)
		{
			if (this->operand[i] != inst.operand[i])
			{
				return false;
			}
		}

		return true;
	}

	bool Instruction::operator!=(const Instruction & inst)
	{
		return !(*this == inst);
	}

	std::ostream & operator<<(std::ostream &os, const Instruction & inst)
	{
		os << "<" << OpCodeNames[inst.op] << ",R" << +inst.reg << "," << +inst.operand[0] << "," << +inst.operand[1] << ">";
		return os;
	}
}


