//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "SymTab.h"

/**/
/*
SymbolTable::AddSymbol()
NAME

    SymbolTable::AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

    void AddSymbol( const string &a_symbol, int a_loc );
    	a_symbol	-> The name of the symbol to be added to the symbol table.
    	a_loc		-> the location to be associated with the symbol.

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.

RETURNS
    void

AUTHOR
    Sanskar Adhikari

DATE
    7:40 PM 04/03/2022
*/
/**/
void SymbolTable::AddSymbol( const string &a_symbol, int a_loc )
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {

        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}
/* void SymbolTable::AddSymbol( const string &a_symbol, int a_loc ); */


/**/
/*
SymbolTable::DisplaySymbolTable()
NAME

    SymbolTable::DisplaySymbolTable - Displays the symbol table on the screen.

SYNOPSIS

    void SymbolTable::DisplaySymbolTable();

DESCRIPTION

    This function will display the symbol table on the screen.

RETURNS
    void

AUTHOR
    Sanskar Adhikari

DATE
    7:44 PM 04/03/2022
*/
/**/
void SymbolTable::DisplaySymbolTable() {
    int symbolCount = 0;  //This is the number of symbol in symbol table.
    cout << "Output from test program:" << endl <<endl;
    cout << "Symbol Table: " << endl << endl;
    cout << setw(12) << left << "Symbol#" << setw(12) << left << "Symbol" << setw(12) << left << "Location" << endl;

    //Iterate through the map and display the symbols and their location
    map<string, int>::iterator it;
    for (it = m_symbolTable.begin(); it != m_symbolTable.end(); it++)
    {
        cout << setw(12) << left << symbolCount << setw(12) << left << it->first << setw(12) << left << it->second << endl;
        symbolCount++;
    }
    cout << endl;
    cout << "__________________________________________________________" << endl << endl;
}
/* void SymbolTable::DisplaySymbolTable(); */


/**/
/*
SymbolTable::LookupSymbol()
NAME

    SymbolTable::LookupSymbol - Looks if the symbol exists in the symbol table

SYNOPSIS

     bool LookupSymbol(const string& a_symbol, int& a_loc);
     a_symbol  -->  Symbol to be looked for
     a_loc     -->  The location that will be changed if the symbol is found

DESCRIPTION

    This function will look if symbol a_symbol exists in the symbol table. If it exists a_loc is changed
    to the location of the symbol.

RETURNS
    true if the symbol a_symbol is found
    false if the symbol a_symbol is not found

AUTHOR
    Sanskar Adhikari

DATE
    7:55 PM 04/03/2022
*/
/**/
bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc) {

    //If the symbol is found record the address and return true
    if (m_symbolTable.find(a_symbol) != m_symbolTable.end())
    {
        a_loc = m_symbolTable[a_symbol];
        return true;
    }
    
    //The symbol does not exist in the symbolTable so return fase
    return false;
}
/* bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc); */