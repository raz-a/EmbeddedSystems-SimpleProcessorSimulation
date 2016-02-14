/*
	Result.h
	Defines the Instruction-Result Struct

	Raz Aloni
	2/14/2016
*/

#ifndef RESULT_H
#define RESULT_H

#include <iostream>

namespace Project1
{
	struct Result
	{
		unsigned char reg;
		unsigned char value;

		Result();
		Result(unsigned char reg, unsigned char value);

		bool operator==(const Result &res);
		bool operator!=(const Result &res);
		friend std::ostream& operator<<(std::ostream &os, const Result &res);
	};

	const Result EMPTY_RESULT(0xFF, 0xFF);
}

#endif
