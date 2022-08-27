//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"

// Don't forget to comment the function headers.

/**/
/*
FileAccess::FileAccess()
NAME

    FileAccess::FileAccess() - Initiates/opens a file from which the code is to be read.

SYNOPSIS

    FileAccess( int argc, char *argv[] );
    argc    -->    The total number of command line arguments
    argv    -->    User entered array of characters

DESCRIPTION

    This function is a constructor for FileAccess class that aims to open the file to read the code.
    Reports error if unsuccessful.

RETURNS
    no return value as it is a constructor.

AUTHOR
    Sanskar Adhikari

DATE
    7:54 PM 04/04/2022
*/
/**/
FileAccess::FileAccess( int argc, char *argv[] )
{
    // Check that there is exactly one run time parameter.
    if( argc != 2 ) {
        cerr << "Usage: Assem <FileName>" << endl;
        exit( 1 );
    }
    // Open the file.  One might question if this is the best place to open the file.
    // One might also question whether we need a file access class.
    m_sfile.open( argv[1], ios::in );

    // If the open failed, report the error and terminate.
    if( ! m_sfile ) {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit( 1 ); 
    }
}
/* FileAccess::FileAccess( int argc, char *argv[] ) */


/**/
/*
FileAccess::~FileAccess()
NAME

    FileAccess::~FileAccess() - Closes the open file

SYNOPSIS

    FileAccess::~FileAccess();

DESCRIPTION

    This function is a destructor for FileAccess class. It closes the open file

RETURNS
    no return value as it is a destructor.

AUTHOR
    Sanskar Adhikari

DATE
    7:57 PM 04/04/2022
*/
/**/
FileAccess::~FileAccess( )
{
    // Not that necessary in that the file will be closed when the program terminates, but good form.
    m_sfile.close( );
}
/* FileAccess::~FileAccess( ) */


/**/
/*
FileAccess::GetNextLine( )
NAME

    FileAccess::GetNextLine( ) - Get the next line from the text file

SYNOPSIS

    FileAccess::GetNextLine( string &a_line );
    string &a_line    ->    Contains the string of the line


DESCRIPTION

    This function reads the next line from the open file if any other line exists.

RETURNS
    returns true if next line exists and false otherwise

AUTHOR
    Sanskar Adhikari

DATE
    8:05 PM 04/04/2022
*/
/**/
bool FileAccess::GetNextLine( string &a_line )
{
    // If there is no more data, return false.
    if( m_sfile.eof() ) {
    
        return false;
    }
    getline( m_sfile, a_line );
    
    // Return indicating success.
    return true;
}
/* bool FileAccess::GetNextLine( string &a_line ) */


/**/
/*
FileAccess::rewind( )
()
NAME

   void FileAccess::rewind( ) - Goes back to the beginning of the file

SYNOPSIS

    void FileAccess::rewind( );

DESCRIPTION

    This function rewinds the pointer back to the beginning of the file

RETURNS
    void
AUTHOR
    Sanskar Adhikari

DATE
    08:10 PM 04/04/2022
*/
/**/
void FileAccess::rewind( )
{
    // Clean all file flags and go back to the beginning of the file.
    m_sfile.clear();
    m_sfile.seekg( 0, ios::beg );
}
/* void FileAccess::rewind( ) */
    
