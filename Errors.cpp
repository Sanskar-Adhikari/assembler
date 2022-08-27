#include "stdafx.h"
#include "Errors.h"

vector<string> Errors::m_ErrorMsgs;

/**/
/*
Errors::DisplayErrors()
NAME

    Errors::DisplayErrors() - Prints out the errors that are recorded

SYNOPSIS

    void Errors::DisplayErrors( );

DESCRIPTION

    This function prints the existing errors after translating the code

RETURNS
    void

AUTHOR
    Sanskar Adhikari

DATE
    7:40 PM 04/04/2022
*/
/**/
void Errors::DisplayErrors()
{
    for (int i = 0; i < m_ErrorMsgs.size(); i++)
    {
        cout << m_ErrorMsgs[i] << endl;
    }
}
/* void Errors::DisplayErrors() */
