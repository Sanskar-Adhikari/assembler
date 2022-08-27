//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//
#pragma once

// The elements of an instruction.
class Instruction {

public:

    Instruction( );
    ~Instruction( ) { };

    // Codes to indicate the type of instruction we are processing.  Why is this inside the
    // class?  We should make this an enum class.  We will do this during a lecture.
    enum class InstructionType {
        ST_MachineLanguage, 	// A machine language instruction.
        ST_AssemblerInstr,      // Assembler Language instruction.
        ST_Comment,             // Comment or blank line
        ST_End,                 // end instruction.
        ST_Error                // Statement has an error.
    };
    // Parse the Instruction.
    InstructionType ParseInstruction(string a_line);

    // Compute the location of the next instruction.
    int LocationNextInstruction(int a_loc);

    // To access the label
    inline string &GetLabel( ) {
        return m_Label;
    };
    // To determine if a label is blank.
    inline bool isLabel( ) {
        return ! m_Label.empty();
    };

    //access the original instruction
    inline string& GetInstruction() {
        return m_instruction;
    }

    //access operand 1
    inline string GetOp1() {
        return m_Operand1;
    }

    //access operand 2
    inline string GetOp2() {
        return m_Operand2;
    }

    //getter function for opcode
    inline string GetOpCode() {
        return m_OpCode;
    }

    //return int opcode
    inline int& GetOpCodeInt() {
       return m_NumOpCode;
    }

    //bool function for checking if operand 1 is numeric
    inline bool IsNumericOperand1() {
        return m_IsNumericOperand1;
    }

    //bool function for checking if operand 2 is numeric
    inline bool IsNumericOperand2() {
        return m_IsNumericOperand2;
    }

    //getter function for Operand1NumericValue
    inline int Operand1NumericValue() {
        return m_Operand1NumericValue;
    }

    //getter function for Operand2NumericValue
    inline int Operand2NumericValue() {
        return m_Operand2NumericValue;
    }

    //bool sunction that returns true if there is format error
    inline bool isError()
    {
        return isFormatError;
    }

private:


    // The elemements of a instruction
    string m_Label;        // The label.
    string m_OpCode;       // The symbolic op code.
    string m_Operand1;     // The first operand. 
    string m_Operand2;     // The second operand.


    string m_instruction;    // The original instruction.

    // Derived values.
    int m_NumOpCode = 0;     // The numerical value of the op code for machine language equivalents.
    InstructionType m_type = InstructionType::ST_Error; // The type of instruction.

    bool m_IsNumericOperand1 = false;// == true if the operand 1 is numeric.
    int m_Operand1NumericValue=0 ;   // The value of the operand 1 if it is numeric.

    bool m_IsNumericOperand2 = false;// == true if the operand 2 is numeric.
    int m_Operand2NumericValue = 0;   // The value of the operand 2 if it is numeric.

    vector<string> ASSEMBLY = { "DC","DS","ORG","END" };
    vector<string> MACHINE = { "ADD","SUB","MULT","DIV","LOAD","STORE","ADDR","SUBR","MULTR","DIVR","READ","WRITE","B","BM","BZ","BP","HALT"};
    


    // Delete any comments from the statement.
    void DeleteComment(string &a_line)
    {
        size_t isemi1 = a_line.find(';');
        if (isemi1 != string::npos)
        {
            a_line.erase(isemi1);
        }
    }

    //Records the format error
    bool isFormatError;

    // Record the fields of the instructions.
    bool RecordFields( const string &a_line );

    // Get the fields that make up the statement.  This function returns false if there
    // are extra fields.
    bool ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
        string& a_Operand1, string& a_Operand2);

    // Check if a string contains a number. 
    bool isStrNumber(const string& a_str);

};


