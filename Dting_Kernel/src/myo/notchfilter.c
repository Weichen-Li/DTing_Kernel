/******************************* SOURCE LICENSE *********************************
Copyright (c) 2013 MicroModeler.

A non-exclusive, nontransferable, perpetual, royalty-free license is granted to the Licensee to
use the following Information for academic, non-profit, or government-sponsored research purposes.
Use of the following Information under this License is restricted to NON-COMMERCIAL PURPOSES ONLY.
Commercial use of the following Information requires a separately executed written license agreement.

This Information is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

******************************* END OF LICENSE *********************************/

// A commercial license for MicroModeler DSP can be obtained at http://www.micromodeler.com/launch.jsp

#include "notchfilter.h"

#include <stdlib.h> // For malloc/free
#include <string.h> // For memset


float32_t notchfilter_coefficients[21] =
{
0.16650257, 0.014891881, 0.028819100, 0.014316039, 0.0048414804, -0.0040578425,
-0.026501975, -0.043499276, -0.047840951, -0.054090420, 0.94062391, -0.054090420,
-0.047840951, -0.043499276, -0.026501975, -0.0040578425, 0.0048414804, 0.014316039,
0.028819100, 0.014891881, 0.16650257
};


notchfilterType *notchfilter_create( void )
{
notchfilterType *result = (notchfilterType *)malloc( sizeof( notchfilterType ) ); // Allocate memory for the object
notchfilter_init( result ); // Initialize it
return result; // Return the result
}

void notchfilter_destroy( notchfilterType *pObject )
{
free( pObject );
}

void notchfilter_init( notchfilterType * pThis )
{
arm_fir_init_f32( &pThis->instance, notchfilter_numTaps, notchfilter_coefficients, pThis->state, notchfilter_blockSize );
notchfilter_reset( pThis );

}

void notchfilter_reset( notchfilterType * pThis )
{
memset( &pThis->state, 0, sizeof( pThis->state ) ); // Reset state to 0
pThis->output = 0; // Reset output

}

int notchfilter_filterBlock( notchfilterType * pThis, float * pInput, float * pOutput, unsigned int count )
{
arm_fir_f32( &pThis->instance, (float32_t *)pInput, (float32_t *)pOutput, count );
return count;

}



