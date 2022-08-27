#include "stdafx.h"
#include "Emulator.h"

/**/
/*
emulator::insertMemory()
NAME

    emulator::insertMemory()  - records content of the assembler in the specific location

SYNOPSIS

   bool insertMemory( int a_location, long long a_contents )
   a_location    ->    Location where the translated code is stored
   a_contents    ->    The translated code to be stored.


DESCRIPTION

    This function stores the translated code in the specific assembler memory location.

RETURNS
    bool true if the content was stored in the memory location successfully or else false if failure.

AUTHOR
    Sanskar Adhikari

DATE
    10:45 PM 04/06/2022
*/
/**/
bool emulator::insertMemory(int a_location, long long a_contents)
{
    if (a_location >= 0 && a_location < MEMSZ)
    {
        m_memory[a_location] = a_contents;
        return true;
    }
    return false;
}
/*bool emulator::insertMemory(int a_location, long long a_contents)*/


/**/
/*
emulator::runProgram()
NAME

    emulator::runProgram - run the emulator by processing the code that was translated

SYNOPSIS

    bool emulator::runProgram()

DESCRIPTION

    This function executes the instruction found in memory

RETURNS
    returns true if the code was processed/emulated succesfully
    returns false otherwise -- if error occured

AUTHOR
    Sanskar Adhikari

DATE
    11:40 PM 04/29/2022
*/
/**/
bool emulator::runProgram()
{
    int opCode = 0;
    int location = 0;
    int operand1 = 0;
    int operand2 = 0;
    int contents = 0;
    int userValue = 0;

    for (int loc = 0; loc < MEMSZ; loc++)
    {
        contents = (int) m_memory[loc];
        if (contents != 0)
        {
            opCode = contents / 10000000;
            operand1 = (contents / 1000000) % 10;
            operand2 = contents % 1000000;

            switch (opCode)
            {
            //add 
            case 1:
                m_registers[operand1] += m_memory[operand2];
                break;
            //sub
            case 2:
                m_registers[operand1] -= m_memory[operand2];
                break;
            //mult
            case 3:
                m_registers[operand1] *= m_memory[operand2];
                break;
            //div
            case 4:
                m_registers[operand1] /= m_memory[operand2];
                break;
            //load
            case 5:
                m_registers[operand1] = m_memory[operand2];
                break;
            //store
            case 6:
                m_memory[operand2] = m_registers[operand1];
                break;
            //addr
            case 7:
                if (operand1 <= 9 && operand2 <= 9)
                {
                    m_registers[operand1] = m_registers[operand1] + m_registers[operand2];
                    break;
                }
            //subr
            case 8:
                if (operand1 <= 9 && operand2 <= 9)
                {
                    m_registers[operand1] = m_registers[operand1] - m_registers[operand2];
                    break;
                }
            //multr
            case 9:
                if (operand1 <= 9 && operand2 <= 9)
                {
                    m_registers[operand1] = m_registers[operand1] * m_registers[operand2];
                    break;
                }
            //divr
            case 10:
                if (operand1 <= 9 && operand2 <= 9)
                {
                    m_registers[operand1] = m_registers[operand1] / m_registers[operand2];
                    break;
                }
            //read
            case 11:
                cout << "?";
                cin >> userValue;
                if (userValue < MEMSZ)
                {
                    m_memory[operand2] = userValue;
                    break;
                }
                cout << "Input too large to compute" << endl;
                return false;
            //write
            case 12:
                cout << m_memory[operand2] << endl;
                break;
            //b
            case 13:
                loc = operand2;
                break;
            //bm
            case 14:
                if (m_registers[operand1] < 0)
                {
                    loc = operand2 - 1;
                }
                break;
            //bz
            case 15:
                if (m_registers[operand1] == 0)
                {
                    loc = operand2 - 1;
                }
                break;
            //bp
            case 16:
                if (m_registers[operand1] > 0)
                {
                    loc = operand2 - 1;
                }
                break;
            //halt
            case 17:
                loc = 1'000'000;
                break;
            }

        }
    }
    return false;
}
/*bool emulator::runProgram()*/
