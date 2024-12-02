#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "readInput.h"

#define SIZE_OF_TEXT_LINE_BUFFER 	64
#define INPUT_FILE_NAME     "input.txt"

static char TextLineBuffer[ SIZE_OF_TEXT_LINE_BUFFER ]; // Store here a text line 
static FILE *Fptr = NULL;
static uint32_t CurrentLineIndex;                       // Just used to debug

void readInputInit( void )
{
    // Open a file in read mode
    Fptr = fopen( INPUT_FILE_NAME, "r" );
    CurrentLineIndex = 0uL;
    if( Fptr == NULL )
    {
        printf( "Error, file is not opened!!" );
    }
}

char *GetNextLine( void )
{
    if( Fptr == NULL )
    {
        printf( "Error, No file to read" );
    }
    // Reset line
    memset( TextLineBuffer, 0u, sizeof( TextLineBuffer ) );
    uint16_t index = 0;
    int currentChar;
    do
    {
        // Pour construire une ligne
        currentChar = getc( Fptr );
        if( index < SIZE_OF_TEXT_LINE_BUFFER )
        {
            TextLineBuffer[index++] = (char)currentChar;
        }
    }while( ( currentChar != '\n' ) && ( currentChar >= 0 ) );
    CurrentLineIndex ++;
   
   // Empty line or too small one
    if( index <= 2)
    {
        // End of file
        printf( "%d lines found\n", CurrentLineIndex );
        return NULL;
    }

    return TextLineBuffer;
}

