/*
	Processor.h
	Defines the Processor

	Raz Aloni
	2/14/2016
*/

#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <iostream>
#include <fstream>
#include <deque>
#include <tuple>

#include "Opcode.h"
#include "Instruction.h"
#include "Result.h"

using namespace std;

namespace Project1
{
	class Processor
	{
		public:
			bool SteadyState;
			Processor(ifstream *instructions, ifstream *registers, ifstream *datamemory);
			void NextClock();
			friend ostream& operator<<(ostream &os, const Processor &p);
		private:
			deque<string> InstructionMem;
			Instruction InstructionBuffer;
			tuple<bool, unsigned char> RegisterFile[16];
			tuple<bool, unsigned char> DataMem[16];
			Instruction ArithInstructionBuffer;
			Instruction StoreInstructionBuffer;
			Instruction PartialResultBuffer;
			Result AddressBuffer;
			Result ResultBuffer;

			void determineSteadyState();
			Instruction processInstruction(string inst);
	};
}

#endif
