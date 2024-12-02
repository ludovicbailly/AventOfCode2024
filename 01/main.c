#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


#include "readInput.h"

typedef struct
{
    uint32_t A;
    uint32_t B;
}OneLineData_s;

static OneLineData_s ParseLine(char * ptrCharLine);
static void SortData( OneLineData_s * ptrData, uint32_t numberOfItems);

int main (void)
{
    bool parsingIsFisnihed = false;
    uint32_t numberOfItems = 0uL;

    OneLineData_s *ptrData = NULL;

    // Init file reader. Will be adapted or reused next days
    readInputInit();


    // Parse the line of the input file
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
                ptrData[numberOfItems] = ParseLine( lineToProcess );
                numberOfItems++;
            }
        }
    } while ( parsingIsFisnihed == false );

    // Once data are sorted, solution is easier
    SortData( ptrData, numberOfItems );

    // Just compare each line one to the other
    uint32_t sum = 0uL;
    for( uint32_t i = 0uL; i< numberOfItems; i++ )
    {
        sum += (uint32_t)abs( (int32_t)ptrData[i].A - (int32_t)ptrData[i].B );
    }
    //Day 1-1 solution
    printf( "--> Sum is %d\n", sum );

    // Search in B list occurencies of A list.
    uint32_t similarity = 0uL;
    
    for( uint32_t i = 0uL; i < numberOfItems; i++ )
    {
        for( uint32_t j = 0uL; j < numberOfItems; j++ )
        {
            if( ptrData[i].A == ptrData[j].B )
            {
                similarity += ptrData[i].A;
            }
        }
    }
    //Day 1-2 solution
    printf( "--> Similarity is %d\n", similarity );
 
    return 0;
}

static OneLineData_s ParseLine( char * ptrCharLine )
{
    OneLineData_s data;
    sscanf( ptrCharLine, "%u %u", &data.A, &data.B );
    return data;
}

static void SortData( OneLineData_s * ptrData, uint32_t numberOfItems)
{
    uint32_t i, j, temp;

    // Sort A and B
    for ( i = 0uL; i < numberOfItems - 1uL; i++ ) 
    {
        for ( j = 0uL; j < numberOfItems - i - 1uL; j++ ) 
        {
            if ( ptrData[j].A > ptrData[j + 1].A ) 
            {
                temp = ptrData[j].A;
                ptrData[j].A = ptrData[j + 1].A;
                ptrData[j + 1].A = temp;
            }

            if ( ptrData[j].B > ptrData[j + 1].B ) 
            {
                temp = ptrData[j].B;
                ptrData[j].B = ptrData[j + 1].B;
                ptrData[j + 1].B = temp;
            }
        }
    }
}

