/**
 * Header file for OpCode.cpp
 * Includes all libraries needed for entire project
 */
#ifndef OPCODE
#define OPCODE

// All library includes
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <stdio.h>

using namespace std;

/**
 * Creates opcode associated class, defining its methods
 */
class OpCode {

public:
	// define tableSize
	int tableSize = 59;

	// define methods used
	int getFormalVal(int codeVal);
	string getNameVal(int codeVal);
	bool bitwiseValue(int currBit, int currPos);
};

#endif
