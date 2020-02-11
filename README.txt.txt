--------------------------------------------------------------------------------
IN CLASS ACCOUNT CSSC0898
Milad Balkhinezhad #818423301
Ryan Shaffar #817862884
Na Thavonekham #813967369
Sallymae Reyes #818296564
--------------------------------------------------------------------------------
CS 530 Assignment #2 - DASM DISASSEMBLER FOR SIC/XE

File Manifest: dasm.cpp, dasm.h, table.cpp, table.h, Makefile, README.md, 
designDocument.jpeg, textReference.jpeg, result.jpeg

OBJECTIVE: To create a program that disassembles object code into assembly code
using the SIC/XE instructional set.

COMPILE: Compile using a C++ compiler, recommended G++. Tested to be run on
Edoras or a standard Debian based linux distribution using a shell tool and 
compatible with Eclipse. Able to compile using the provided makefile. Call 'make' to 
compile the files into an executable called 'dasm.' Standard, must include a
-std=gnu++11 when compiling to prevent errors.

The program is designed to open an object file (.obj) and a symbol file (.sym), which
containing their respective SIC/XE object code & symbols. The .sym file contains the SYMTAB
and LITTAB that was generated during initial assembly. The disassembler will then use both
files to disassemble the object code and generate a source file (.sic)
Assumed are the object and symbol files are provided in the same source folder,
as well as a black sic file.

A single command line argument is needed when running the program, the name
of the object file. Ex "sample.obj"

SIC/XE (Simplified Instructional Computer/Extra Equipment) is a hypothetical 
simplified machine language for learning purposes in our Systems Programming 
course, and is introduced in "System Software: An Introduction to Systems 
Programming", by Leland Beck.

Files provided for input:
.obj file: contains object code
.sym file: contains SYMTAB and LITTAB

Output files:
.sic file: contains original source code assembled

-------------------------------------------------------------------------------
DESIGN DESCRIPTION: The program consists of two source files and two header files

**refer to the designDocument.jpeg for visual design decription

table.cpp: this source file creates an opcode table with 3 characteristic fields; 'intCode' for the 
code value of the instruction, 'intFormat' to specify weather it is format 1, 2, or 3, 
and 'strName' for the string name of the instruction.

table.h: This is the header file for the table.cpp file. This header file includes
all of the libraries necessary for the disassembler. The methods defines the table size
to fit the 59 opcode instructions. The method defines the opcode associated class as well
as defining the values and position of the bits. This information was pulled from
page 496 of the textbook "System Software" by Leland Beck.

dasm.cpp: This source file takes one command line argument, which is the name of the object file to be disassembled.
Within the object file are methods to analyze the instructions formats 1 through 3. The third format instruction include RSUB as well
as instructions that begin with the letter J. These formats may include a '+' sign in front depending if it is direct or indirect. 
Once the object is compared and paired with it's according address and name, it is then formatted into a sic file with its correct space and registers.
The main method in this file makes sure to check for the correct formatting of the command line.
 
dasm.h: This header file for object file 'dasm.cpp' creates a disassembler object 'openAllFiles' that disassembles
code from the objFile. The file includes the table header that comes with the libraries 
necessary.

LESSONS LEARNED: SIC/XE architecture, C++ language, file reading and modification
through dedicated program, disassembling an assembled program.

BUGS OR DEFICIENCIES: No bugs or deficiencies have been found within our code. 
We have built it with efficiency in mind. If any flaws are found, please contact
balkhinezhad@gmail.com
-------------------------------------------------------------------------------
