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

		// Create resultBuffer
		this->ResultBuffer = deque<Result>();
	}

	void Processor::NextClock()
	{
		// Result Buffer -> Register File

		if (this->ResultBuffer.size())
		{
			Result tempR = this->ResultBuffer.front();
			this->ResultBuffer.pop_front();
			this->RegisterFile[tempR.reg] = make_tuple(true, tempR.value);
		}
		
		// Partial Result Buffer -> Result Buffer
		if (this->PartialResultBuffer != EMPTY_INSTRUCTION)
		{
			this->ResultBuffer.push_back(Result{
				this->PartialResultBuffer.reg,
				(unsigned char)(this->PartialResultBuffer.operand[0] * this->PartialResultBuffer.operand[1])
			});
		}

		// Arithmetic Instruction Buffer -> Partial Result Buffer/Result Buffer
		this->PartialResultBuffer = EMPTY_INSTRUCTION;

		switch (this->ArithInstructionBuffer.op)
		{
			case (MUL) :
				this->PartialResultBuffer = this->ArithInstructionBuffer;
				break;
			case (ADD) :
				this->ResultBuffer.push_back(Result{
					this->ArithInstructionBuffer.reg,
					(unsigned char)(this->ArithInstructionBuffer.operand[0] + this->ArithInstructionBuffer.operand[1])
				});
				break;
			case (SUB) :
				this->ResultBuffer.push_back(Result{
					this->ArithInstructionBuffer.reg,
					(unsigned char)(this->ArithInstructionBuffer.operand[0] - this->ArithInstructionBuffer.operand[1])
				});
				break;
		}

		// Address Buffer -> Data Memory
		if (this->AddressBuffer != EMPTY_RESULT)
		{
			this->DataMem[this->AddressBuffer.value] = make_tuple(true, get<1>(this->RegisterFile[this->AddressBuffer.reg]));
		}

		// Store Instruction Buffer -> Address Buffer
		this->AddressBuffer = EMPTY_RESULT;

		if (this->StoreInstructionBuffer != EMPTY_INSTRUCTION)
		{
			this->AddressBuffer = Result{
				this->StoreInstructionBuffer.reg,
				(unsigned char)(this->StoreInstructionBuffer.operand[0] + this->StoreInstructionBuffer.operand[1])
			};
		}

		// Instruction Buffer -> Arithmetic Instruction Buffer/Store Instruction Buffer
		this->ArithInstructionBuffer = EMPTY_INSTRUCTION;
		this->StoreInstructionBuffer = EMPTY_INSTRUCTION;

		if (this->InstructionBuffer != EMPTY_INSTRUCTION)
		{
			if (this->InstructionBuffer.op == ST)
			{
				this->StoreInstructionBuffer = this->InstructionBuffer;
			}
			else
			{
				this->ArithInstructionBuffer = this->InstructionBuffer;
			}
		}

		// Instruction Memory to Instruction Buffer
		this->InstructionBuffer = EMPTY_INSTRUCTION;

		if (this->InstructionMem.size() != 0)
		{
			string s = this->InstructionMem.front();
			this->InstructionMem.pop_front();
			this->InstructionBuffer = this->processInstruction(s);
		}

	}

	bool Processor::IsSteadyState()
	{
		if (this->InstructionMem.size() > 0)
		{
			return false;
		}

		if (this->InstructionBuffer != EMPTY_INSTRUCTION)
		{
			return false;
		}

		if (this->ArithInstructionBuffer != EMPTY_INSTRUCTION)
		{
			return false;
		}

		if (this->StoreInstructionBuffer != EMPTY_INSTRUCTION)
		{
			return false;
		}

		if (this->PartialResultBuffer != EMPTY_INSTRUCTION)
		{
			return false;
		}

		if (this->AddressBuffer != EMPTY_RESULT)
		{
			return false;
		}

		for (unsigned int i = 0; i < ResultBuffer.size(); i++)
		{
			if (this->ResultBuffer[i] != EMPTY_RESULT)
			{
				return false;
			}
		}
		
		return true;
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
		os << "INM:";

		for (unsigned int i = 0; i < p.InstructionMem.size(); i++)
		{
			if (i != 0)
			{
				os << ",";
			}

			os << p.InstructionMem[i];
		}

		os << endl << "INB:";

		if ((Instruction)p.InstructionBuffer != EMPTY_INSTRUCTION)
		{
			os << p.InstructionBuffer;
		}

		os << endl << "AIB:";

		if ((Instruction)p.ArithInstructionBuffer != EMPTY_INSTRUCTION)
		{
			os << p.ArithInstructionBuffer;
		}

		os << endl << "SIB:";

		if ((Instruction)p.StoreInstructionBuffer != EMPTY_INSTRUCTION)
		{
			os << p.StoreInstructionBuffer;
		}

		os << endl << "PRB:";

		if ((Instruction)p.PartialResultBuffer != EMPTY_INSTRUCTION)
		{
			os << p.PartialResultBuffer;
		}

		os << endl << "ADB:";

		if ((Result)p.AddressBuffer != EMPTY_RESULT)
		{
			os << p.AddressBuffer;
		}

		os << endl << "REB:";

		for (unsigned int i = 0; i < p.ResultBuffer.size(); i++)
		{
			if (i != 0)
			{
				os << ",";
			}

			os << p.ResultBuffer[i];
		}

		os << endl << "RGF:";

		for (int i = 0; i < 16; i++)
		{
			if (get<0>(p.RegisterFile[i]))
			{
				os << "<R" << i << "," << +get<1>(p.RegisterFile[i]) << ">";

				if (i != 15)
				{
					os << ',';
				}
			}
		}

		os << endl << "DAM:";

		for (int i = 0; i < 16; i++)
		{
			if (get<0>(p.DataMem[i]))
			{
				os << "<" << i << "," << +get<1>(p.DataMem[i]) << ">";

				if (i != 15)
				{
					os << ',';
				}
			}
		}

		return os;
	}
}


