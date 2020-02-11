/**
 * Header file for dasm.cpp
 */

#ifndef DASM
#define DASM

// include table header (includes libraries)
#include "table.h"

/**
 * Create dasm object
 */
class dasm {

public:
	// create openFiles method to disassemble code
    void openAllFiles(string objFile);
};

#endif
