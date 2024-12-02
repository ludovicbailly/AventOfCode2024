#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#include "readInput.h"

#define MAX_NUMBER_OF_ITEMS_PER_LINE    10

typedef struct
{
    uint8_t NumberOfItemInLine;
    uint32_t ItemTable[MAX_NUMBER_OF_ITEMS_PER_LINE];
}OneLineData_s;

static OneLineData_s ParseLine(char * ptrCharLine);
static bool IsSafe(OneLineData_s data);
static bool IsSafeProblemDampener(OneLineData_s data);

int main (void)
{
    bool parsingIsFisnihed = false;
    uint32_t numberOfItems = 0uL;

    OneLineData_s *ptrData = NULL;

    // Init file reader. Will be adapted or reused next days
    readInputInit();

    // Parse the lines of the input file
    do
    {
        char *lineToProcess = GetNextLine();

        if( lineToProcess == NULL )
        {
            // End of file print result and exit
            parsingIsFisnihed = true;
        }
        else
        {
            // Process line
            if( numberOfItems == 0uL )
            {
                ptrData = (OneLineData_s *)malloc( sizeof( OneLineData_s ) );
            }
            else
            {
                ptrData = (OneLineData_s *)realloc( ptrData, (numberOfItems +1uL ) * sizeof( OneLineData_s ) );
            }

            if( ptrData != NULL )
            {
                // clear buffer
                for( uint8_t i=0u; i< MAX_NUMBER_OF_ITEMS_PER_LINE; i++)
                {
                    ptrData[numberOfItems].ItemTable[i] = 0;
                }
                ptrData[numberOfItems] = ParseLine( lineToProcess );
                numberOfItems++;
            }
        }
    } while ( parsingIsFisnihed == false );

    uint32_t isSafeCounter = 0uL;
    uint32_t isSafeCounterProblemDampener = 0uL;
    // Analyze line per line
    for( uint32_t i = 0uL; i < numberOfItems; i++ )
    {
        if( IsSafe( ptrData[i] ) == true )
        {
            isSafeCounter++;
        }
        if( IsSafeProblemDampener( ptrData[i] ) == true )
        {
            isSafeCounterProblemDampener++;
        }
    }

    printf( "--> Safe number is %d\n", isSafeCounter );
    printf( "--> Safe number Problem Dampener is %d\n", isSafeCounterProblemDampener );
    return 0;
}

static OneLineData_s ParseLine( char * ptrCharLine )
{
    OneLineData_s data;

    data.NumberOfItemInLine = 0u;

    uint32_t value = 0uL;
    uint16_t indexInString = 0u;

    while( ptrCharLine[indexInString] != '\n')
    {
        // Char is a number
        if( ( ptrCharLine[indexInString] >= '0' ) && ( ptrCharLine[indexInString] <= '9' ) )
        {
            value *= 10u;
            value += (uint32_t)(ptrCharLine[indexInString] - '0');
        }
        // Char is a space
        else if( ptrCharLine[indexInString] == ' ' )
        {
            data.ItemTable[data.NumberOfItemInLine] = value;
            data.NumberOfItemInLine ++;
            value = 0u;
        }
        else
        {
            // Debug
            printf( "---> Unreconized char: -> %c hex: %x \n", ptrCharLine[indexInString], ptrCharLine[indexInString] );
        }

        indexInString++;
    }

    // Latest value
    data.ItemTable[data.NumberOfItemInLine] = value;
    data.NumberOfItemInLine ++;

    return data;
}

static bool IsSafe(OneLineData_s data)
{
    #define MAX_INC 3

    bool isSafe = true; // Let's detect defects
    bool isIncreasing;

    if(data.NumberOfItemInLine >= 2)
    {
        // determine if it is increasing or decreasing
        if( data.ItemTable[0] < data.ItemTable[1] )
        {
            isIncreasing = true;
        }
        else
        {
            isIncreasing = false;
        }

        // Verify if the order is rigth
        for( uint16_t i = 0u; (i < data.NumberOfItemInLine - 1u) && ( isSafe == true ); i++ )
        {
            if( isIncreasing == true)
            {
                // is increasing.
                if( ( data.ItemTable[i] >= data.ItemTable[i+1u] ) || ( (data.ItemTable[i+1u] - data.ItemTable[i]) > MAX_INC ) )
                {
                    isSafe = false;
                }
            }
            else
            {
                // is decreasing
                if( ( data.ItemTable[i] <= data.ItemTable[i+1u] ) || ( (data.ItemTable[i] - data.ItemTable[i+1u]) > MAX_INC ) )
                {
                    isSafe = false;
                }
            }
        }
    }
    else
    {
        // Too short to process
        isSafe = false;
    }
    return isSafe; 
}

static bool IsSafeProblemDampener(OneLineData_s data)
{
    bool isSafe = IsSafe(data);

    // A serie can be safe if one item is remove. Try brut force!!!
    for(uint16_t i=0; (i< data.NumberOfItemInLine) && ( isSafe == false); i++ )
    {
        OneLineData_s underTest = data;
        for( uint16_t j= i; j< underTest.NumberOfItemInLine-1; j++)
        {
            underTest.ItemTable[j] = underTest.ItemTable[j+1];
        }
        underTest.NumberOfItemInLine--;
        isSafe = IsSafe( underTest );
    }
    return isSafe; 
}
