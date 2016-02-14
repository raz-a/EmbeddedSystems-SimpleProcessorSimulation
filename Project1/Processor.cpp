#include "Processor.h"

namespace Project1
{
	Processor::Processor(ifstream  *instructions, ifstream *registers, ifstream *datamemory)
	{
		string s;

		// Init Instruction Memory to empty
		this->InstructionMem = deque<string>();

		// Set Registers and DataMemory to all false and empty
		for (int i = 0; i < 16; i++)
		{
			this->DataMem[i] = make_tuple(false, 0xFF);
			this->RegisterFile[i] = make_tuple(false, 0xFF);
		}

		// Populate instructions
		while (true)
		{
			getline(*instructions, s);

			if(instructions->eof())
			{
				break;
			}

			this->InstructionMem.push_back(s);
		}

		instructions->close();

		// Populate Registers
		while (true)
		{
			getline(*registers, s);

			if (registers->eof())
			{
				break;
			}

			size_t r = s.find_first_of('R', 0);
			size_t comma = s.find_first_of(',', r);
			size_t endBracket = s.find_first_of('>', comma);

			unsigned char regNum = (unsigned char)stoi(s.substr(r + 1, comma - r - 1));
			unsigned char value = (unsigned char)stoi(s.substr(comma + 1, endBracket - comma - 1));

			this->RegisterFile[regNum] = make_tuple(true, value);
		}

		registers->close();

		// Populate Data Memory
		while (true)
		{
			getline(*datamemory, s);

			if (datamemory->eof())
			{
				break;
			}

			size_t beginBracket = s.find_first_of('<', 0);
			size_t comma = s.find_first_of(',', beginBracket);
			size_t endBracket = s.find_first_of('>', comma);

			unsigned char addr = (unsigned char)stoi(s.substr(beginBracket + 1, comma - beginBracket - 1));
			unsigned char value = (unsigned char)stoi(s.substr(comma + 1, endBracket - comma - 1));

			this->DataMem[addr] = make_tuple(true, value);
		}

		datamemory->close();

		determineSteadyState();
	}

	void Processor::NextClock()
	{

	}

	void Processor::determineSteadyState()
	{

	}

	Instruction Processor::processInstruction(string inst)
	{
		Instruction i;

		bool storeInstruction = false;

		size_t beginBracket = inst.find_first_of('<', 0);
		size_t comma1 = inst.find_first_of(',', beginBracket);

		string op = inst.substr(beginBracket + 1, comma1 - beginBracket - 1);

		if (op == "ST")
		{
			i.op = ST;
			storeInstruction = true;
		}
		else if (op == "ADD")
		{
			i.op = ADD;
		}
		else if (op == "SUB")
		{
			i.op = SUB;
		}
		else
		{
			i.op = MUL;
		}

		size_t r1 = inst.find_first_of('R', comma1);
		size_t comma2 = inst.find_first_of(',', r1);

		i.reg = (unsigned char)stoi(inst.substr(r1 + 1, comma2 - r1 - 1));

		size_t r2 = inst.find_first_of('R', comma2);
		size_t comma3 = inst.find_first_of(',', r2);

		i.operand[0] = get<1>(this->RegisterFile[(unsigned char)stoi(inst.substr(r2 + 1, comma3 - r2 - 1))]);

		size_t endBracket = inst.find_first_of('>', comma3);

		if (storeInstruction)
		{
			i.operand[1] = (unsigned char)stoi(inst.substr(comma3 + 1, endBracket - comma3 - 1));
		}
		else
		{
			size_t r3 = inst.find_first_of('R', comma3);
			i.operand[1] = get<1>(this->RegisterFile[(unsigned char)stoi(inst.substr(r3 + 1, endBracket - r3 - 1))]);
		}

		return i;

	}

	ostream & operator<<(ostream & os, const Processor & p)
	{
		return os;
	}
}


