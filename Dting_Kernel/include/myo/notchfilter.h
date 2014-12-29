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

// Begin header file, notchfilter.h

#ifndef NOTCHFILTER_H_ // Include guards
#define NOTCHFILTER_H_

//#define ARM_MATH_CM4 // Use ARM Cortex M4
//#define __FPU_PRESENT 1 // Does this device have a floating point unit?
#include <arm_math.h> // Include CMSIS header

// Link with library: libarm_cortexM4_mathL.a (or equivalent)
// Add CMSIS/Lib/GCC to the library search path
// Add CMSIS/Include to the include search path
extern float32_t notchfilter_coefficients[21];
static const int notchfilter_numTaps = 21;
static const int notchfilter_blockSize = 64;

typedef struct
{
arm_fir_instance_f32 instance;
float32_t state[85];
float32_t output;
} notchfilterType;


notchfilterType *notchfilter_create( void );
void notchfilter_destroy( notchfilterType *pObject );
void notchfilter_init( notchfilterType * pThis );
void notchfilter_reset( notchfilterType * pThis );
#define notchfilter_writeInput( pThis, input ) \
arm_fir_f32( &pThis->instance, &input, &pThis->output, 1 );

#define notchfilter_readOutput( pThis ) \
pThis->output

int notchfilter_filterBlock( notchfilterType * pThis, float * pInput, float * pOutput, unsigned int count );
#define notchfilter_outputToFloat( output ) \
(output)

#define notchfilter_inputFromFloat( input ) \
(input)

#endif // NOTCHFILTER_H_

