#include "stdafx.h" 
#include "Instruction.h"
#include "Errors.h"
#include "Assembler.h"
/**/
/*
Instruction::Instruction()
NAME

    Instruction::Instruction() -initializes(resets) the values

SYNOPSIS

    Instruction::Instruction() - constructor for the Instruction class.

DESCRIPTION

    This function is the constructor of the Instruction class and initializes/resets its variable values.

RETURNS
    no return value as it is a constructor.

AUTHOR
    Sanskar Adhikari

DATE
    9:40 PM 04/03/2022
*/
/**/
Instruction::Instruction() {
    isFormatError = false;
   //nothing to do for now
}
/* Instruction::Instruction()  */

/**/
/*
Instruction::InstructionType Instruction::ParseInstruction()
NAME

    Instruction::InstructionType Instruction::ParseInstruction() -parse an instruction and return its type

SYNOPSIS

    Instruction::InstructionType Instruction::ParseInstruction(string a_line)
    string a_line    -->   The line to be parsed

DESCRIPTION

    This function parses an instruction and return its type

RETURNS
    Instruction::InstructionType (like ST_Comment, ST_Error)

AUTHOR
    Sanskar Adhikari

DATE
    11:40 AM 04/29/2022
*/
/**/
Instruction::InstructionType Instruction::ParseInstruction(string a_line)
{
    // Record the original statement.  This will be needed in the sceond pass.
    m_instruction = a_line;

    // Delete any comment from the line.
    DeleteComment( a_line );
    
    // Record label, opcode, and operands.  Up to you to deal with formatting errors.
    bool isFormatError = RecordFields( a_line );
    

    // Check if this is a comment.
    if (m_Label.empty() && m_OpCode.empty())
    {
        return InstructionType::ST_Comment;
    }
    // Return the instruction type.  This has to be handled in the code.
    return m_type;
}
/*Instruction::InstructionType Instruction::ParseInstruction(string a_line)*/


/**/
/*
bool Instruction::RecordFields( )
NAME

    bool Instruction::RecordFields( const string &a_line ) - Record the fields that make up the instructions.

SYNOPSIS

    bool Instruction::RecordFields( const string &a_line )
    string a_line    -->   The line whose fields are to be recorded. Passes as const reference to avoid copy

DESCRIPTION

    This function records the fields that make up the instruction

RETURNS
    bool true if sucessfully records the fields, false if there is format error

AUTHOR
    Sanskar Adhikari

DATE
    8:40 AM 05/1/2022
*/
/**/
bool Instruction::RecordFields( const string &a_line )
{
    // Get the fields that make up the instruction.
     isFormatError = ! ParseLineIntoFields( a_line, m_Label, m_OpCode, m_Operand1, m_Operand2);
    

    // if code was a comment, there is nothing to do.
    if (m_OpCode.empty() && m_Label.empty()) return isFormatError;

 
    // For the sake of comparing, convert the op code to upper case.
    for (char& c : m_OpCode)
    {
        c = toupper(c);
    }
  
    //if the line is empty or has spaces then it is comment.
    if (a_line.length() == 0 || all_of(a_line.begin(), a_line.end(), isspace))
    {
        m_type = InstructionType::ST_Comment;
    }
    else if (m_OpCode == "END")
    {
        m_type = InstructionType::ST_End;
    }
    else if (find(ASSEMBLY.begin(), ASSEMBLY.end(), m_OpCode) != ASSEMBLY.end())
    {
        m_type = InstructionType::ST_AssemblerInstr;
        m_NumOpCode = (int)distance(ASSEMBLY.begin(),find(ASSEMBLY.begin(), ASSEMBLY.end(), m_OpCode))+1;
    }
    else if (find(MACHINE.begin(), MACHINE.end(), m_OpCode) != MACHINE.end())
    {
        m_type = InstructionType::ST_MachineLanguage;
        m_NumOpCode = (int)distance(MACHINE.begin(), find(MACHINE.begin(), MACHINE.end(), m_OpCode))+1;
    }
    else
    {
        m_type = InstructionType::ST_Error;
    }
    // Record whether the operands are numeric and their value if they are.
    m_IsNumericOperand1 = isStrNumber(m_Operand1);
    if (m_IsNumericOperand1)
    {
        try
        {
            m_Operand1NumericValue = stoi(m_Operand1);  //m_Operand1NumericValue is int so if user enters long long stoi will fail. So, to ensure no execption is thrown checking here
        }
        catch (invalid_argument&)
        {
            m_IsNumericOperand1 = false;
        }
    }

    m_IsNumericOperand2 = isStrNumber(m_Operand2);
    if (m_IsNumericOperand2)
    {
        try
        {
            m_Operand2NumericValue = stoi(m_Operand2);  //m_Operand2NumericValue is int so if user enters long long stoi will fail
        }
        catch (invalid_argument&)
        {
            m_IsNumericOperand2 = false;
        }
    }
    return isFormatError;
}
/*bool Instruction::RecordFields( const string &a_line )*/


/**/
/*
Instruction::ParseLineIntoFields()
NAME

    bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
    string& a_Operand1, string& a_Operand2) - Parses the instructions into fields.

SYNOPSIS

    bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
    string& a_Operand1, string& a_Operand2)
    string a_line    -->   The line to parse.
    a_label          -->   Records the label in the instruction
    a_OpCode         -->   Records the opCode in the instruction
    a_Operand1       -->   Records the Operand1 in the instruction
    a_Operand2       -->   Records the Operand2 in the instruction 

DESCRIPTION

    This function parses the instruction into fields. (label, opcode, operand1, operand2)

RETURNS
    bool true if sucessfully parses the instruction into fields, false if there were additional characters than the described format

AUTHOR
    Sanskar Adhikari

DATE
    9:40 AM 05/1/2022
*/
/**/
bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
    string& a_Operand1, string& a_Operand2)
{
    // Get rid of any commas from the line.
    replace(a_line.begin(), a_line.end(), ',', ' ');

    // Get the elements of the line.  That is the label, op code, operand1, and operand2.
    string endStr;
    a_label = a_OpCode = a_Operand1 = a_Operand2 = "";
    istringstream ins(a_line);
    if (a_line[0] == ' ' || a_line[0] == '\t')
    {
        a_label = "";
        ins >> a_OpCode >> a_Operand1 >> a_Operand2 >> endStr;
    }
    else
    {
        ins >> a_label >> a_OpCode >> a_Operand1 >> a_Operand2 >> endStr;
    }
    // If there is extra data, return false.
    return endStr.empty() ? true : false;
}
/*bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
    string& a_Operand1, string& a_Operand2)*/


/**/
/*
Instruction::isStrNumber(const string& a_str)
NAME

    bool Instruction::isStrNumber(const string& a_str)

SYNOPSIS

    bool Instruction::isStrNumber(const string& a_str)
    string a_str    -->   String that is checked if it contains number

DESCRIPTION

    This function checks if a string has numbers or not.

RETURNS
    bool true if the string is all numbers, false otherwise

AUTHOR
    Sanskar Adhikari

DATE
    9:55 AM 05/1/2022
*/
/**/
bool Instruction::isStrNumber(const string& a_str)
{
    if (a_str.empty()) return false;

    // If there is a - or a plus, make sure there are more characters.
    int ichar = 0;
    if (a_str[0] == '-' || a_str[0] == '+')
    {
        ichar++;
        if (a_str.length() < 2) return false;
    }
    // Make sure that the remaining characters are all digits
    for (; ichar < a_str.length(); ichar++)
    {
        if (!isdigit(a_str[ichar])) return false;
    }
    return true;
}


/**/
/*
int Instruction::LocationNextInstruction() 
NAME

    int Instruction::LocationNextInstruction(int a_loc)

SYNOPSIS

    int Instruction::LocationNextInstruction(int a_loc)
    int a_loc    -->    location of current instruction.

DESCRIPTION

    This function calculates and returns the location of next instruction

RETURNS
    int - the value for the next instruction's location 

AUTHOR
    Sanskar Adhikari

DATE
    11:55 AM 05/1/2022
*/
/**/
int Instruction::LocationNextInstruction(int a_loc)
{
    int myloc=0;
    if (m_OpCode == "DS" || m_OpCode == "ORG")
    {
        myloc = a_loc + m_Operand1NumericValue;
    } 
    else
    {
        myloc = a_loc + 1;
    }
    return myloc;
}
/*int Instruction::LocationNextInstruction(int a_loc)*/
