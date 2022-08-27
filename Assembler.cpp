//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"
#include "Instruction.h"
#include "SymTab.h"
#include <iostream>

/**/
/*
Assembler::Assembler()
NAME

    Assembler::Assembler() - constructor for the assembler with argc and argc as 
    its arguments

SYNOPSIS

    Assembler::Assembler( int argc, char *argv[] );
    argc   -->    Total number of arguements in command line
    argv   -->    User entered array of strings.

DESCRIPTION

    This function is the constructor of the Assembler class.

RETURNS
    no return value as it is a constructor.

AUTHOR
    Sanskar Adhikari

DATE
    9:40 PM 04/04/2022
*/
/**/
Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv )
{
    m_numErrors = 0;
}  
/* Assembler::Assembler( int argc, char *argv[] ) */


// Destructor currently does nothing.  You might need to add something as 
//you develope this project.  If not, we can delete it.
Assembler::~Assembler( )
{
}

/**/
/*
Assembler::PassI()
NAME

    Assembler::PassI( )  - helps in populating the symbol table

SYNOPSIS

    void Assembler::PassI( );


DESCRIPTION

    This function established the location of the labels and records error if found.

RETURNS
    void

AUTHOR
    Sanskar Adhikari

DATE
    9:45 PM 04/05/2022
*/
/**/
void Assembler::PassI( ) 
{
    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for( ; ; ) {

        // Read the next line from the source file.
        string line; 
        if( ! m_facc.GetNextLine( line ) ) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }
        // Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( line );

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement and report 
        //an error if it isn't.
        if( st == Instruction::InstructionType::ST_End ) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip comments.
        if( st == Instruction::InstructionType::ST_Comment )  
        {
        	continue;
	    }
        
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if( m_inst.isLabel( ) ) {

            m_symtab.AddSymbol( m_inst.GetLabel( ), loc );
        }
        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );
    }
}
/* void Assembler::PassI( ) */


/**/
/*
Assembler::PassII()
NAME

    Assembler::PassII( )  - goes line by line and checks for errors and if none calculates 
                            content value as per the instruction

SYNOPSIS

    void Assembler::PassII( );

DESCRIPTION

    This function goes line by line and looks for error. 
    If errors are found it reports immediately after offending statement.
    If not calculates the content value

RETURNS
    void

AUTHOR
    Sanskar Adhikari

DATE
    9:45 PM 04/05/2022
*/
/**/
void Assembler::PassII()
{
    //wait for user to hit enter
    cout << "Press Enter to continue: " << endl;
    cin.ignore();
    m_facc.rewind();  //goes to the beginning of the file
    Errors::InitErrorReporting();
    int loc = 0;      //Location of the instructions to be generated

    cout << "Translation of Program: " << endl;
    cout << left << setw(10) << "Location" << left << setw(22) << "Contents" 
         << left << setw(35) << "Original Statement" << endl;

    //process each line of code successively
    while (true)
    {
        Errors::InitErrorReporting();
        int values = 0;  //values of the contents
       
        string line;     //records the next line.
        if ( !m_facc.GetNextLine(line))
        {
            //if there is no end statement, report error
            Errors::RecordError("ERROR! Missing an End Statement");
            m_numErrors++;
            Errors::DisplayErrors();
            return;
            
        }
        //parse the line and get the type of instruction
        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        //determine if end is the last element 
        if (st == Instruction::InstructionType::ST_End)
        {
            while (m_facc.GetNextLine(line))
            {
                if (!line.empty())  //if there is something after end statement
                {
                    //if the line after end statement is not comment, report error
                    if (m_inst.ParseInstruction(line) != Instruction::InstructionType::ST_Comment) 
                    {
                        Errors::RecordError("ERROR! End statement is not the last statement");
                        m_numErrors++;
                        Errors::DisplayErrors();
                       
                    }
                }
            }
                cout << "\t\t\t\t" << m_inst.GetInstruction() << endl;
                break;
        }

        //if the line is comment, just print the line as is.
        if (st == Instruction::InstructionType::ST_Comment)
        {
            cout << " \t\t\t\t" << m_inst.GetInstruction() << endl;
            continue;
        }
        if (m_inst.isError())
        {
                Errors::RecordError("ERROR! Extra operand found: " + m_inst.GetInstruction());
                m_numErrors++;
        }

        //check for bad instruction type
        if (st == Instruction::InstructionType::ST_Error)
        {
            Errors::RecordError(" ERROR! BAD opcode " + m_inst.GetInstruction());
            m_numErrors++;
        }

        //check to see if label start with number and report error if true
        if (m_inst.GetLabel()[0] >= '0' && m_inst.GetLabel()[0] <= '9')
        {
            Errors::RecordError("ERROR: Label " + m_inst.GetLabel() +
                                " cannot start with number");
            m_numErrors++;
        }

        //making sure label is not too big
        if (m_inst.GetLabel().length() > 10)
        {
            Errors::RecordError("ERROR! The label size is too large for"+ m_inst.GetLabel());
            m_numErrors++;
        }

        //if the next location exceeds 999999, then report error
        if (m_inst.LocationNextInstruction(loc) > 999999)
        {
            Errors::RecordError("ERROR! Memory too large...Insufficient memory for translation");
            m_numErrors++;
        }
        int temp_loc = 0;
        if (m_inst.isLabel())
        {
            if (m_symtab.LookupSymbol(m_inst.GetLabel(), temp_loc) && temp_loc == m_symtab.multiplyDefinedSymbol)
            {
                Errors::RecordError("ERROR! Multiple labels with same name");
                m_numErrors++;
            }
        }
        //if operand value > 10'000 report error
        if (m_inst.Operand1NumericValue() > 10000 || m_inst.Operand2NumericValue() > 10000)
        {
            Errors::RecordError("ERROR! Value too large for " + m_inst.GetOpCode());
            m_numErrors++;
        }
         
        values = 0;

        //if assembly instruction call AssemblyInst function
        if (st == Instruction::InstructionType::ST_AssemblerInstr)
                AssemblyInst(line, values);
        
        int location = 0;

        //if it is machine, call MachineInst function
        if (st == Instruction::InstructionType::ST_MachineLanguage)
                MachineInst(line, values, location);

        DisplayContents(values, loc);
        Errors::DisplayErrors();
    }
   
}
/*void Assembler::PassII()*/

 
/**/
/*
Assembler::AssemblyInst()
NAME

    Assembler::AssemblyInst()  - runs the assembly instruction and reports errors if any

SYNOPSIS

    void Assembler::AssemblyInst(string& line, int& values)
    &line    -->    Gets the line to process assembly instructions. Passed as a reference to avoid copy.
    &values    -->    The value of content. Passed as a reference so that the value changes in the passII function as well, which calls DisplayContent function  

DESCRIPTION

    This function processes the assembly instructions and reports errors if found. Else update content

RETURNS
    void

AUTHOR
    Sanskar Adhikari

DATE
    10:45 AM 04/26/2022
*/
/**/
void Assembler::AssemblyInst(const string& line, int& values)
{

    //all but "END" needs operand1, so if not found report error
    if (m_inst.GetOpCode() != "END" && m_inst.GetOp1().empty())
    {
        Errors::RecordError("ERROR! No operand found for:" + line);
        m_numErrors++;
    }

    //operand 1 must be numeric, if not report error
    if (!m_inst.IsNumericOperand1()&& !m_inst.GetOp1().empty())
    {
        Errors::RecordError("ERROR! Operand 1 must be numeric for : " + line);
        m_numErrors++;
    }
    //assembly instruction do not need operand2, so if it exists report error
    if (!m_inst.GetOp2().empty())
    {
        Errors::RecordError("ERROR! Operand 2 not needed for assembly instruction: " + line);
        m_numErrors++;
    }

    //ORG does not need label, if found report error
    if (m_inst.GetOpCode() == "ORG" && !m_inst.GetLabel().empty())
    {
        Errors::RecordError("ERROR! Label found for ORG which is not needed:" + line);
        m_numErrors++;
        values = 0;
    }

    //all but ORG needs label, if not found report error
     if (m_inst.GetOpCode() != "ORG" && m_inst.GetLabel().empty())
     {
        Errors::RecordError("ERROR! No label defined called " + m_inst.GetOpCode());
        m_numErrors++;
     }
     //calculating value
    if (m_inst.GetOpCode() == "DC")
    {
        values = m_inst.Operand1NumericValue();
    }
    else
    {
        values = 0;
    }
}
/*void Assembler::AssemblyInst(string& line, int& values)*/


/**/
/*
Assembler::MachineInst()
NAME

    Assembler::MachineInst()  - runs the machine instruction and reports errors if any.
SYNOPSIS

    void Assembler::MachineInst(string& line, int& values, int& location)
    &line    -->    Gets the line to process assembly instructions. Passed as a reference to avoid copy.
    &values    -->    The value of content. Passed as a reference so that the value changes in the passII function as well, which calls DisplayContent function
    &location    -->  Look up location and use the value to calculate the content. Pass as a reference so that the value gets modified in pass II.
DESCRIPTION

    This function processes the machine instructions and reports errors if found, or else, update content
    Default register value is considered 9.

RETURNS
    void

AUTHOR
    Sanskar Adhikari

DATE
    11:45 AM 04/26/2022
*/
/**/
void Assembler::MachineInst(const string& line, int& values, int& location)
{
    int temp = 0;
    if (!m_inst.IsNumericOperand1())
        temp = 9;  //keeping default register value as 9
    else
        temp = m_inst.Operand1NumericValue();

    //for 01 ADD  02 SUB  03 MULT 04 DIV  05 LOAD 06 STORE
    if (m_inst.GetOpCodeInt() >= 1 && m_inst.GetOpCodeInt() <= 6)
    {
        //needs both operands. If not found, report error
        if (m_inst.GetOp1().empty() || m_inst.GetOp2().empty())
        {
            Errors::RecordError("ERROR! Both operands needed for: " + m_inst.GetInstruction());
            m_numErrors++;
        }
        //operand 1 must be register. If not, report error
        if (!m_inst.IsNumericOperand1())
        {
            Errors::RecordError("ERROR! Operand 1 must be register value/ numeric: " + m_inst.GetInstruction());
            m_numErrors++;
        }
        //operand 2 must be ADDR. If not repport error
        if (m_inst.IsNumericOperand2())
        {
            Errors::RecordError("ERROR! Operand 2 must be address " + m_inst.GetInstruction());
            m_numErrors++;
        }
        //if operand 2/addr is undefined report error
        int temploc;
        if (m_symtab.LookupSymbol(m_inst.GetOp2(), temploc) == 0)
        {
            Errors::RecordError("ERROR! Label " + m_inst.GetOp2() + " undefined ");
            m_numErrors++;
        }
     }
    if (m_inst.GetOpCode() == "READ" || m_inst.GetOpCode() == "WRITE")
    {
        //at least 1 operand needed for read and write, if not found report error
        if (m_inst.GetOp1().empty())
        {
            Errors::RecordError("Operand 1 not found for " + m_inst.GetOpCode());
            m_numErrors++;
            
        }
        //read write can have 1 or 2 operands. For 1 operand, it must be addr value
        else if (!m_inst.GetOp1().empty() && m_inst.GetOp2().empty())
        {
            if (m_inst.IsNumericOperand1())
            {
                Errors::RecordError("For read/write statement with 1 operand, the operand must be non numeric/label " );
                m_numErrors++;
            }
            //if addr is undefined report error
            int temploc;
            if (m_symtab.LookupSymbol(m_inst.GetOp1(), temploc) == 0)
            {
                Errors::RecordError("Label " + m_inst.GetOp1() + " undefined ");
                m_numErrors++;
            }

        }
        //for read/write with 2 operands, first should be register and second should be addr. If not report error
        else if (!m_inst.GetOp1().empty() && !m_inst.GetOp2().empty())
        {
            if (!m_inst.IsNumericOperand1() || m_inst.IsNumericOperand2())
            {
                Errors::RecordError("For read/write statement with 2 operand, first one should be resister value and second one should be label ");
                m_numErrors++;
            }        
            //if addr is undefined report error
            int temploc;
            if (m_symtab.LookupSymbol(m_inst.GetOp2(), temploc) == 0)
            {
                Errors::RecordError("Label " + m_inst.GetOp2() + " undefined ");
                m_numErrors++;
            }
        }

        //calculating value
        if ((m_symtab.LookupSymbol(m_inst.GetOp1(), location))|| (m_symtab.LookupSymbol(m_inst.GetOp2(),location)))
        {
            values = 10000000 * m_inst.GetOpCodeInt() + temp * 1000000 + location;
        }
    }
    //for 13 B  14 BM  15 BZ  16 BP
   else if (m_inst.GetOpCodeInt() >= 13 && m_inst.GetOpCodeInt() <= 16)
     {
        //oprand 2 is required, if not report error
        if (m_inst.GetOp2().empty())
        {
            Errors::RecordError("For " + m_inst.GetOpCode() + " , the second operand must be present ");
            m_numErrors++;
        }
        //operand 2 must be addr, if not report error
            if (m_inst.IsNumericOperand2())
            {
                Errors::RecordError("For " + m_inst.GetOpCode()+ " , the second one must be address ");
                    m_numErrors++;
            }
            //if label is not defined report error
            int temploc;
            if (m_symtab.LookupSymbol(m_inst.GetOp2(), temploc) == 0)
            {
                Errors::RecordError("Label "+ m_inst.GetOp2()+" undefined ");
                m_numErrors++;
            } 
            //calculating value
            m_symtab.LookupSymbol(m_inst.GetOp2(), location);
            values = 10000000 * m_inst.GetOpCodeInt() + temp * 1000000 + location;

    }
    //for 07 ADDR  08 SUBR  09 MULTR 10 DIV
    else if (m_inst.GetOpCodeInt() >= 7 && m_inst.GetOpCodeInt() <= 10)
    {
        //both operand should be registers, if not report error
        if (!m_inst.IsNumericOperand1() || !m_inst.IsNumericOperand2())
        {
            Errors::RecordError("Needs both operand to be numeric");
            m_numErrors++;
        }
        values = 10000000 * m_inst.GetOpCodeInt() + temp * 1000000 + m_inst.Operand2NumericValue() * 100000;
    }      
    else
    {
        if (m_symtab.LookupSymbol(m_inst.GetOp2(), location))
        {
            values = 10000000 * m_inst.GetOpCodeInt() + temp * 1000000 + location;
        }
    } 
    if (m_inst.GetOpCode() == "HALT")
    {
        //HALT does not need label, if found report error

        if (!m_inst.GetLabel().empty())
        {
            Errors::RecordError("Label found in HALT");
            m_numErrors++;
        }
        //HALT does not need operand, if found report error
        if (!m_inst.GetOp1().empty())
        {
            Errors::RecordError("Operand non needed in HALT");
            m_numErrors++;
        }
        values = 10000000 * m_inst.GetOpCodeInt() + temp * 1000000;
    }
}
/*void Assembler::MachineInst(const string& line, int& values, int& location)*/

/**/
/*
Assembler::DisplayContents()
NAME

    Assembler::DisplayContents() - displays the contents of instructions in terminal

SYNOPSIS

    void Assembler::DisplayContents(int &values, int &loc)
    &values   -->    The actual value of contents to be processed and displayed.
    &loc   -->    The value of location of the instruction.


DESCRIPTION

    This function displays the content in the terminal.

RETURNS
    void

AUTHOR
    Sanskar Adhikari

DATE
    9:40 AM 25/04/2022
*/
/**/
void Assembler::DisplayContents(int& values, int& loc)
{
    //values are integers so to match with 9 digit format converting it to string and adding required number of 0s
    string mystring = to_string(values);
    size_t len = std::to_string(values).length();
    size_t difference = 9 - len;
    for (size_t i = 0; i < difference; i++)
    {
        mystring = "0" + mystring;
    }
    if (values == 0)
    {
        mystring = "";
    }
    //passing to the emulaor
    m_emul.insertMemory(loc, values);
    //outputting the content and instructions
    cout << left << setw(10) << loc << left << setw(22);
    cout << mystring << left << setw(35);
    cout << m_inst.GetInstruction() << endl;

    //finding out the location of next instruction
    loc = m_inst.LocationNextInstruction(loc);
}
/*void Assembler::DisplayContents(int &values, int &loc)*/

/**/
/*
Assembler::RunProgramInEmulator()
NAME

    Assembler::RunProgramInEmulator()  - checks errors and runs the emulator if no errors are found

SYNOPSIS

    void Assembler::RunProgramInEmulator()


DESCRIPTION

    This function checks for errors and runs the emulator if none are found.

RETURNS
    void

AUTHOR
    Sanskar Adhikari

DATE
    9:45 PM 04/06/2022
*/
/**/
void Assembler::RunProgramInEmulator()
{
    cout << "__________________________________________________________" << endl << endl;
    cout << "Press enter to continue" << endl;
    cin.ignore();
    cout << "Results from emulating program" << endl;
    //if errors, dont run the emulator
    if (m_numErrors!=0)
    {
        cout << "Emulator cannot start because of errors" << endl;
    }
    else
    {
        m_emul.runProgram();
    }
    cout << "End of emulation" << endl;
}
/*void Assembler::RunProgramInEmulator() */