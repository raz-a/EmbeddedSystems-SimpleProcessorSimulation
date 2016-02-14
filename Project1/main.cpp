/*
	Testing environment
*/

#include <iostream>
#include <string>
#include "Opcode.h"
#include "Instruction.h"

using namespace Project1;

void main()
{
	Instruction i(MUL, 12, 3, 6);

	std::cout << i;

	int x;

	std::cin >> x;
}