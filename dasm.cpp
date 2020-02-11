/**
 * dasm.cpp takes one command line argument of the name of an object file.
 * It uses this name to open an accompanying symbol file and blank sic file.
 * It reads these files and writes to the sic file in maching language by
 * converting the information found in the object and symbol files and
 * using our knowledge of types of addressing and formatting
 */

// include dasm and table headers (includes libraries)
#include "dasm.h"
#include "table.h"

// create file streams
ifstream xeFileLoad; // read
ifstream symFileLoad; // read
ofstream writeToSic; // write to

// Define variables
vector<string> loadXeFile;
vector<string> loadSymFile;
vector<string> loadValues;

vector<int> varNames;
vector<int> varAddresses;

vector<string> symbolFileNames;
vector<int> symbolFileValues;
vector<char> symbolFileFlags;

char delim = '.';
char whiteSpace;
char fileName[100];
char sourceFile[100];
char symbolFile[100];
char sic[100] = ".sic";
char sym[100] = ".sym";

int length = 0;
int iterate = 0;
int progSize;
int currentAddress;
int startAddress;
int extendedFormat = 0x000FFFFF;
int regularFormat = 0x000FFF;

string nineSpace = "         ";
string eightSpace = "        ";
string twelveSpace = "            ";
string fifteenSpace = "               ";
string fin = "END";
string lt = "LTORG";
string extendedBs = "BASE    ";
string rw = " RESW    ";
string rb = " RESB    ";
string sub = "RSUB";

// create helper methods designed to format sic file appropriately
/**
 * Offsets stream name left by width of 7
 */
void shiftLeftSeven(string currName) {
	writeToSic << setw(7) << left << currName;
}

/**
 * Offsets stream name left by width of 7 with a space of 9
 */
void shiftLeftSevenWithSpace(string currName) {
	writeToSic << nineSpace << setw(7) << left << currName;
}

/**
 * Shifts the stream left by width 7, length of name and provided string
 */
void shiftLeftSevenWithChar(string letter, string currName) {
	writeToSic << setw(7) << left << currName << letter;
}

/**
 * Shifts the stream left by width of 7, 10, and length of names
 */
void shiftLeftSevenTen(string opName, string currName) {
	writeToSic << setw(7) << opName << setw(10) << left << currName << endl;
}

/**
 * Offsets stream name left by width of 8
 */
void shiftLeftEight(string currName) {
	writeToSic << setw(8) << left << currName;
}

/**
 * Shifts stream by 8 spaces
 */
void shiftEightSpace() {
	writeToSic << eightSpace;
}

/**
 * Shifts the stream left by width of 9, current name, and provided string
 */
void shiftLeftNineWithChar(string letter, string currName) {
	writeToSic << setw(9) << left << currName + letter << endl;
}

/**
 * Offsets stream name left by width of 10 in both streams
 */
void shiftLeftTen(string currName) {
	writeToSic << setw(10) << left << currName << endl;
}

/**
 * Offsets stream name right by width of 14
 */
void shiftRightFourteen(string currName) {
	writeToSic << setw(14) << right << currName << endl;
}

/**
 * Shifts stream right by width 17 and length of name
 */
void shiftRightSeventeen(string currName) {
	writeToSic << setw(17) << right << extendedBs << currName << endl;
}

/**
 * Helps format for all formats besides 5
 */
void formatOthers(int destination) {
	writeToSic << destination << endl;
}

/**
 * Inserts a provided string into the stream
 */
void insertString(string letter) {
	writeToSic << letter;
}

/**
 * Checks extended format
 */
void checkFormat(int format) {
	if (format) {
		insertString("+");
	} else {
		insertString(" ");
	}
}

/**
 * Inserts start format in the stream
 */
void startFormat(string storSubStr1, string address) {
	writeToSic << setw(9) << left << storSubStr1 << "START   " << address
			<< endl;
}

/**
 * Inserts end format in the stream
 */
void endFormat(string currName) {
	writeToSic << nineSpace << setw(8) << left << fin << currName << endl;
}

/**
 * Refer to opcode table to analyze instructions with format number 1.
 * From there receive according address and name and format them onto the
 * sic file correctly, managing space and registers.
 */
void firstFormat(OpCode currOp, int currName, int currObj, int currPos) {
	// define variables
	string currObjName, currSymName, currLitName;
	int currAd, actualSize = symbolFileValues.size() - 1;
	currObjName = currOp.getNameVal(currName);

	// get according name to format
	for (int i = 0; i < loadValues.size(); i++) {
		currLitName = loadValues[i];
		currAd = varAddresses[i];
		if (currentAddress == currAd) {
			// manage format
			shiftLeftTen(currLitName);
			break;
		}
	}

	// get according address to format
	for (int i = 0; i < actualSize; i++) {
		currSymName = symbolFileNames[i];
		if (currentAddress == symbolFileValues[i]) {
			shiftLeftEight(currSymName);
			break;
		} else {
			i = i + 1;
			if (i >= actualSize) {
				shiftLeftSevenWithSpace(currObjName);
			}
		}
	}
}

/**
 * Writes to first and second registers
 */
void writeToRegisters(int firstReg, int secondReg) {
	// first register requires a comma and a space
	if (firstReg == 0) {
		insertString("A, ");
	}
	if (secondReg == 0) {
		insertString("A");
	}
	if (firstReg == 1) {
		insertString("X, ");
	}
	if (secondReg == 1) {
		insertString("X");
	}
	if (firstReg == 2) {
		insertString("L, ");
	}
	if (secondReg == 2) {
		insertString("L");
	}
	if (firstReg == 3) {
		insertString("B, ");
	}
	if (secondReg == 3) {
		insertString("B");
	}
	if (firstReg == 4) {
		insertString("S, ");
	}
	if (secondReg == 4) {
		insertString("S");
	}
	if (firstReg == 5) {
		insertString("T, ");
	}
	if (secondReg == 5) {
		insertString("T");
	}
	if (firstReg == 6) {
		insertString("F, ");
	}
	if (secondReg == 6) {
		insertString("F");
	}
}

/**
 * Refer to opcode table to analyze instructions with format number 2.
 * From there receive according address and name and format them onto the
 * sic file correctly, managing space and registers.
 */
void secondFormat(OpCode currOp, int currName, int currObj, int currPos) {
	// Define variables
	string nameOfFile;
	nameOfFile = currOp.getNameVal(currName);
	int actualSize = symbolFileValues.size() - 1;
	string formatTwo = loadXeFile[currObj].substr(currPos + 2, 1);
	string formatThree = loadXeFile[currObj].substr(currPos + 3, 1);
	// base 16
	int firstReg = strtol(formatTwo.c_str(), NULL, 16);
	int secondReg = strtol(formatThree.c_str(), NULL, 16);

	// get appropriate address to use to format sic file
	for (int i = 0; i < actualSize; i++) {
		string currName = symbolFileNames[i];
		int currVal = symbolFileValues[i];
		// manage format
		if (currentAddress == currVal) {
			shiftLeftEight(currName);
			break;
		} else {
			i = i + 1;
			if (i >= actualSize) {
				shiftLeftSeven(nameOfFile);
			}

		}
	}

	// get appropriate name to use to format sic file
	for (int i = 0; i < loadValues.size(); i++) {
		int currAd = varAddresses[i];
		string currName = loadValues[i];
		if (currentAddress == currAd) {
			// manage format
			shiftLeftTen(currName);
			shiftRightFourteen(lt);
			return;
		}
	}

	// call writeToRegisters method
	writeToRegisters(firstReg, secondReg);
}

/**
 * Refer to opcode table to analyze instructions with format number 2.
 * From there receive according address and name and format them onto the
 * sic file correctly, managing space and registers.
 * Third format instructions including RSUB and instructions that start with
 * the letter J have variations and may include a + sign before depending
 * on direct or indirect addressing
 */
int thirdFormat(OpCode currOp, int currName, int currObj, int currPos) {
	// Define variables
	bool formatTable[6];
	string opName, format, formatFive, name;
	opName = currOp.getNameVal(currName);

	// format accordingly
	format = loadXeFile[currObj].substr(currPos + 1, 2);
	formatFive = loadXeFile[currObj].substr(currPos, 2 * (3 + formatTable[5]));
	// base 16
	int flagValues = strtol(format.c_str(), NULL, 16);
	int actualSize = symbolFileValues.size() - 1, destination = 0, symSize =
			symbolFileValues.size();

	// assign bit position
	for (int i = 0; i < 6; i++) {
		int position = 5 - i;
		formatTable[i] = currOp.bitwiseValue(flagValues, position);
	}

	// get appropriate address to use to format sic file
	for (int i = 0; i < actualSize; i++) {
		name = symbolFileNames[i];
		int currVal = symbolFileValues[i];
		if (currentAddress == currVal) {
			shiftLeftEight(name);
			break;
		} else {
			i = i + 1;
			if (i >= actualSize) {
				shiftEightSpace();
			}
		}
	}

	// get appropriate name to use to format sic file
	for (int i = 0; i < loadValues.size(); i++) {
		int currAd = varAddresses[i], currLength = varNames[i];
		string position = loadXeFile[currObj].substr(
				currPos + (2 * (3 + formatTable[5])), currLength);
		string currName = loadValues[i];

		if (currentAddress == currAd) {
			// base 16
			int updatedPos = strtol(position.c_str(), NULL, 16);

			// check if extended format
			checkFormat(formatTable[5]);
			// further format streams
			shiftLeftSevenTen(opName, currName);
			shiftRightFourteen(lt);
			checkFormat(formatTable[5]);

			int ans = (3 + (varNames[i] / 2));
			return ans;
		}
	}
	// check if + sign is needed
	checkFormat(formatTable[5]);

	// for extended format
	if (formatTable[5]) {
		// base 16, format, length 8
		string str = loadXeFile[currObj].substr(currPos, 8);
		destination = strtol(str.c_str(), NULL, 16);
		destination &= extendedFormat;
	} else { // short format
		// base 16, format, length 6
		string str = loadXeFile[currObj].substr(currPos, 6);
		destination = strtol(str.c_str(), NULL, 16);
		destination &= regularFormat;
	}

	// relative addressing
	if (formatTable[3]) {
		destination = destination + startAddress;
	} else if (formatTable[4]) {
		destination = destination + 3 + currentAddress;
	}

	// further format third format instructions that begin with J
	if (opName == "JSUB" or opName == "JLT" or opName == "JEQ"
			or opName == "JGT" or opName == "J") {
		if (formatTable[5]) {
			destination &= extendedFormat;
		} else {
			destination &= regularFormat;
		}
	}

	// indirect addressing
	if (formatTable[1] == formatTable[0]) {
		// format
		shiftLeftEight(opName);

		for (int i = 0; i < symSize; i++) {
			// define variables
			int currVal = symbolFileValues[i];
			string currName = symbolFileNames[i];

			// deal with RSUB instruction
			if (opName != sub and destination == currVal) {
				if (formatTable[2]) {
					shiftLeftNineWithChar(",X", currName);
				} else {
					shiftLeftNineWithChar("", currName);
				}
				break;
			}
		}
		// first line
		if (sub == opName) {
			writeToSic << endl;
		}
	} else if (formatTable[0]) {
		// include @ sign
		shiftLeftSevenWithChar("@", opName);
		for (int i = 0; i < symSize; i++) {
			int currVal = symbolFileValues[i];
			string currName = symbolFileNames[i];
			if (opName != sub and destination == currVal) {
				// further formatting
				if (formatTable[2]) {
					shiftLeftNineWithChar(",X", currName);
				} else {
					shiftLeftNineWithChar("", currName);
				}
				break;
			}
		}
	} else {
		// include # sign
		shiftLeftSevenWithChar("#", opName);
		if (!formatTable[5]) {
			formatOthers(destination);
		}
	}
	// check for LBD
	if ("LDB" == opName) {
		startAddress = destination;
		for (int i = 0; i < symSize; i++) {
			// define variable
			int currVal = symbolFileValues[i];
			string currName = symbolFileNames[i];
			// check for appropriate position to write to
			if (destination == currVal) {
				shiftLeftTen(currName);
				shiftRightSeventeen(currName);
				int ans = 3 + formatTable[5];
				return ans;
			}
		}
	}
	int ans = 3 + formatTable[5];
	// return ans to further reference what has been written
	// to sic file so far
	return ans;
}

/**
 *  Depending on opcode format declared in the created table,
 *  call appropriate formatting methods to write to sic file
 */
int reformat(int currObj, int currPos) {
	// create object
	OpCode currOp = *new OpCode;
	string subStr = loadXeFile[currObj].substr(currPos, 2);
	// base 16
	int currName = (int) strtol(subStr.c_str(), NULL, 16);
	int definedFormat = currOp.getFormalVal(currName);

	if (definedFormat == 1) {
		firstFormat(currOp, currName, currObj, currPos);
	}
	if (definedFormat == 2) {
		secondFormat(currOp, currName, currObj, currPos);
	}
	if (definedFormat == 3) {
		definedFormat = thirdFormat(currOp, currName, currObj, currPos);
	}
	return (definedFormat * 2);
}

/**
 *  Begins writing to sic file with three columns
 *  Inlude beginning address
 */
void startOfFile(int currRow) {
	// define variables
	string lastLine;
	string firstColumn = loadXeFile[currRow].substr(1, 6);
	string secondColumn = loadXeFile[currRow].substr(13, 6);
	string thirdColumn = loadXeFile[currRow].substr(7, 6);
	stringstream createStream;

	//base 16
	int progStartAddress = currentAddress = strtol(thirdColumn.c_str(), NULL,
			16);
	progSize = strtol(secondColumn.c_str(), NULL, 16);

	// write to sic file
	createStream << progStartAddress;
	string address = createStream.str();
	startFormat(firstColumn, address);
}

/**
 *  This method reads the data load from files
 *  to begin conversion to SIC/XE language
 */
void convert(int currRow) {
	// define variables
	string format = loadXeFile[currRow].substr(7, 2);
	// base 16
	int textLength = strtol(format.c_str(), NULL, 16);
	int constant = 9;
	int sLength = 2 * textLength + 9;
	int size = symbolFileValues.size();

	// create columns
	do {
		int objSize = reformat(currRow, constant);
		int divideSize = objSize / 2;
		currentAddress = currentAddress + divideSize;
		constant = constant + objSize;
	} while (constant < sLength);

	for (int i = 0; i < symbolFileValues.size(); i++) {
		if (currentAddress <= symbolFileValues[i]) {
			if ((currentAddress % 3) == 0) { // divisible by 3
				writeToSic << setw(8) << left << symbolFileNames[i] << rw;
				if (!(i + 1 < symbolFileValues.size())) {
					writeToSic << setw(8) << left << (progSize - symbolFileValues[i]) / 3 << endl;
					int convert = currentAddress + (progSize - symbolFileValues[i]);
					currentAddress = convert;
				} else {
					writeToSic << setw(8) << left << (symbolFileValues[i + 1] - symbolFileValues[i]) / 3 << endl;
					int convert = currentAddress + (symbolFileValues[i + 1] - symbolFileValues[i]);
					currentAddress = convert;

				}
			} else { // not divisible
				writeToSic << setw(8) << left << symbolFileNames[i] << rb;
				if (!(i + 1 < symbolFileValues.size())) {
					writeToSic << setw(8) << left << (progSize - symbolFileValues[i]) << endl;
					int convert = currentAddress + (progSize - symbolFileValues[i]) / 3;
					currentAddress = convert;
				} else {
					writeToSic << setw(8) << left << (symbolFileValues[i + 1] - symbolFileValues[i]) << endl;
					int convert =  currentAddress + (symbolFileValues[i + 1] - symbolFileValues[i]) / 3;
					currentAddress = convert;
				}
			}
		}
	}
}

/**
 *  This method analyzes instructions, there format
 *  and how much memory in address they take up
 */
void analyze(int currRow) {
	// come back to
	string formatOne = loadXeFile[currRow].substr(1, 6);
	string formatTwo = loadXeFile[currRow].substr(7, 2);

	// convert to ints
	int i = 0;
	int modifyAddress = strtol(formatOne.c_str(), NULL, 16);
	int modLength = strtol(formatTwo.c_str(), NULL, 16);
	int address = strtol(formatOne.c_str(), NULL, 16);

	// compare char
	while (loadXeFile[i][0] != 'T') {
		i++;
	}

	while (address < modLength) {
		// base 16
		int curr = strtol(formatTwo.c_str(), NULL, 16);
		address = address + curr;
		i++;
	}
	i--;
	int subtractLength = strtol(formatTwo.c_str(), NULL, 16);
	address = address - subtractLength;

	// modify address
	int diffAddress = modifyAddress - address;
	int position = 2 * (diffAddress) + 10;
	loadXeFile[i][position] += progSize;
}

/**
 *  This method determines if all the appropriate
 *  names and values have been written to the sic file.
 *  If confirmed end of file, write corresponding info
 */
void endOfFile(int currRow) {
	// define variables
	int position, symSize = symbolFileValues.size();
	string format;
	format = loadXeFile[currRow].substr(1, 6);
	position = strtol(format.c_str(), NULL, 16);

	for (int i = 0; i < symSize; i++) {
		int currValue = symbolFileValues[i];
		string currName = symbolFileNames[i];
		// if corresponding value
		if (position == currValue) {
			// input end format to sic file
			endFormat(currName);
		}
	}
}

/**
 *  Depending on information received from object file,
 *  analyze and disassemble - calling the appropriate methods
 */
void readAndWrite() {
	for (int i = 0; i < loadXeFile.size(); i++) {
		char ans = loadXeFile[i][0];
		if (ans == 'E') {
			endOfFile(i);
		}
		if (ans == 'M') {
			analyze(i);
		}
		if (ans == 'T') {
			convert(i);
		}
		if (ans == 'H') {
			startOfFile(i);
		}
	}
}

/**
 * Read xe object and symbol files, store information line by line,
 * appropriate to how the files are formatted
 */
void loadAllFiles() {
	// define variables
	int i = 2;
	string fileLine;
	int actualSize = loadSymFile.size() - 1;
	char nil = (char) NULL;

	// read and store symbol file
	while (symFileLoad.good()) {
		getline(symFileLoad, fileLine);
		loadSymFile.push_back(fileLine);
	}

	// read and store xe object file
	do {
		getline(xeFileLoad, fileLine);
		loadXeFile.push_back(fileLine);
	} while (xeFileLoad.good());

	//copies the contents of files and sticks it in storage
	for (i = 2; i < loadSymFile.size() - 1; i++) {
		string curr = loadSymFile[i];
		if (loadSymFile[i][0] != nil) {
			symbolFileValues.push_back(strtol(curr.substr(8, 6).c_str(), NULL, 16));
			symbolFileFlags.push_back(strtol(curr.substr(16, 1).c_str(), NULL, 16));
			symbolFileNames.push_back(curr.substr(0, 6));
		} else {
			i = i + 3;
			break;
		}
	}

	// base 16
	for (int k = i; k < loadSymFile.size() - 1; k++) {
		varNames.push_back(strtol(loadSymFile[i].substr(19, 1).c_str(), NULL, 16));
		varAddresses.push_back(strtol(loadSymFile[i].substr(24, 6).c_str(), NULL, 16));
		loadValues.push_back(loadSymFile[k].substr(8, 6));
	}

	// call readAndWrite method
	readAndWrite();
}

/**
 *  Open files using the command line argument provided.
 *  Use string manipulation to open other files and
 *  store files according how they are traditionally structured
 */

void dasm::openAllFiles(string objFile) {
	// get just the name of the file (delimiter is period)
	for (unsigned int i = 0; i < objFile.length(); i++) {
		char tmp;
		tmp = objFile[i];

		if (tmp != delim) {
			fileName[i] = tmp;
		} else {
			break;
		}
	}

	// figure out how long name of file is
	for (int i = 0; i < 100; i++) {
		// white is null
		if (fileName[i] != whiteSpace) {
			length++;
		}
	}

	// copy name array twice
	for (int i = 0; i < length; i++) {
		sourceFile[i] = fileName[i];
		symbolFile[i] = fileName[i];
	}

	// create symbol and source file name using the filename and length
	for (int i = length; i < length + 4; i++) {
		sourceFile[i] = sic[iterate];
		symbolFile[i] = sym[iterate];
		iterate++;
	}

	// open files
	xeFileLoad.open(objFile);
	symFileLoad.open(symbolFile);
	writeToSic.open(sourceFile);

	// check if any erros opening files
	if (!xeFileLoad.is_open() || !symFileLoad.is_open()
			|| !writeToSic.is_open()) {
		cout << "Error opening files!" << endl;
		return;
	}

	// call storeFiles method
	loadAllFiles();
}

/**
 *  Main method, gets command line argument and
 *  calls openFile method.
 *  Throws error if incorrect command line format.
 */
int main(int argc, char **argv) {
	// define variables
	dasm dasmClass;
	string fileName;

	// check if user typed in filename
	if (argc != 2) {
		cout << "Incorrect format" << endl;
		return 0;
	}

	// open files
	fileName = argv[1];
	dasmClass.openAllFiles(fileName);

	return 0;
}
