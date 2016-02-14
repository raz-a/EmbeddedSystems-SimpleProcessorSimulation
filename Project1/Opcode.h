/*
	Opcode.h
	Defines the Opcodes being used in this processor
	
	Raz Aloni
	2/14/2016
*/

#ifndef OPCODE_H
#define OPCODE_H

#include <string>

namespace Project1
{
	enum Opcode
	{
		ADD,
		SUB,
		MUL,
		ST,
		NOP
	};

	const std::string OpCodeNames[] =
	{
		"ADD",
		"SUB",
		"MUL",
		"ST",
		"NOP"
	};
}

#endif

