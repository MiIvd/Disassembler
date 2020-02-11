/**
 * This class creates an opcode table to reference corresponding
 * opcode instructions and characteristics; including format, name, and
 * opcode value in C++
 */

// include table code header
#include "table.h"

/**
 * Create an table struct object with 3 fields
 */
struct table {
	int intCode;
	int intFormat;
	string strName;
};

/**
 * Create an opcode table for reference
 * Defined as a constant for the struct object
 * REFERENCE PAGE 496+ IN TEXTBOOK
 */
const struct table opcodeTable[] =
{
		{0x90,2,"ADDR",}, {0x40,3,"AND"}, {0xB4,2,"CLEAR"}, {0x28,3,"COMP"}, {0x88,3,"COMPF"}, {0xA0,2,"COMPR"},
		{0x18,3,"ADD"}, {0x58,3,"ADDF"}, {0x24,3,"DIV"}, {0x64,3,"DIVF"}, {0x9C,2,"DIVR"}, {0xC4,1,"FIX"},
		{0x04,3,"LDX"}, {0xD0,3,"LPS"}, {0x20,3,"MUL"}, {0x60,3,"MULF"}, {0x98,2,"MULR"}, {0xC8,1,"NORM"},
		{0xF4,1,"HIO"}, {0x3C,3,"J"}, {0x30,3,"JEQ"}, {0x34,3,"JGT"}, {0x38,3,"JLT"}, {0x48,3,"JSUB"}, {0x00,3,"LDA"},
		{0xC0,1,"FLOAT"}, {0x68,3,"LDB"}, {0x50,3,"LDCH"}, {0x70,3,"LDF"}, {0x08,3,"LDL"}, {0x6C,3,"LDS"}, {0x74,3,"LDT"},
		{0x44,3,"OR"}, {0xD8,3,"RD"}, {0xAC,2,"RMO"}, {0x4C,3,"RSUB"}, {0xA4,2,"SHIFTL"}, {0xA8,2,"SHIFTR"},
		{0xD4,3,"STI"}, {0x14,3,"STL"}, {0x7C,3,"STS"}, {0xE8,3,"STSW"}, {0x84,3,"STT"}, {0x10,3,"STX"},
		{0xF0,1,"SIO"}, {0xEC,3,"SSK"}, {0x0C,3,"STA"}, {0x78,3,"STB"}, {0x54,3,"STCH"}, {0x80,3,"STF"},
		{0x1C,3,"SUB"}, {0x5C,3,"SUBF"}, {0x94,2,"SUBR"}, {0xB0,2,"SVC"}, {0xE0,3,"TD"}, {0xF8,1,"TIO"},
		{0x2C,3,"TIX"}, {0xB8,2,"TIXR"}, {0xDC,3,"WD"}
};


/**
 * Given int, iterate through opcode table until corresponding name is returned
 * Used to analyze all formats
 */
string OpCode::getNameVal(int opCode) {
	// define variable
	int code;

	// check for equivalent opCode
	for (int i = 0; i < tableSize; i++) {
		if (opcodeTable[i].intCode == opCode) {
			// return opCode name
			string ans = opcodeTable[i].strName;
			return ans;
		}
	}
	code = opCode & 0xFC;

	//recursive call until opCode name is returned
	return getNameVal(code);
}

/**
 * This method receives and input and shifts it right to the corresponding position
 * Used to analyze third format
 */
bool OpCode::bitwiseValue(int input, int position) {
	// define variables
	bool ans;
	int foo = input >> position;

	// & is bitwise operator
	ans = foo & 1;

	// return boolean
	return ans;
}

/**
 * Given int, iterate through opCode table until corresponding format is returned
 */
int OpCode::getFormalVal(int opCode) {
	// define variable
	opCode = opCode & 0xFC;

	// check for equivalent opCode
	for (int i = 0; i < tableSize; i++) {
		if (opcodeTable[i].intCode == opCode) {
			// return opCode format
			return opcodeTable[i].intFormat;
		}
	}

	// otherwise return 0
	return 0;
}
