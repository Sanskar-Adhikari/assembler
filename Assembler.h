//
//		Assembler class.  This is a container for all the components
//		that make up the assembler.
//
#pragma once 

#include "SymTab.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "Emulator.h"

class Assembler {
public:
    Assembler( int argc, char *argv[] );
    ~Assembler( );

    // Pass I - establish the locations of the symbols
    void PassI( );

    // Pass II - generate a translation
    void PassII();

    // Display the symbols in the symbol table.
    void DisplaySymbolTable() { m_symtab.DisplaySymbolTable(); }
    
    // Run emulator on the translation.
    void RunProgramInEmulator();

    //Process the assembly instructions 
    void AssemblyInst(const string& line, int& val);

    //Process the machine instructions
    void MachineInst(const string& line, int& val, int & location);

    //displays the content in the terminal
    void DisplayContents(int& values, int& loc);
   

private:

    FileAccess m_facc;	    // File Access object
    SymbolTable m_symtab;   // Symbol table object
    Instruction m_inst;	    // Instruction object
    emulator m_emul;        // Emulator object
    int m_numErrors = 0;    //number of errors found during the translation

    
};

